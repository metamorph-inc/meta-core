r"""
Distributes a DOE to Celery, one run at a time.

celery -A run_mdao.celery_tasks worker --loglevel=info --pool=gevent -c 1

n.b.: run_mdao depends on the current working directory, so do not run without -c 1

python celery_tasks.py mdao_config.json

import run_mdao.celery_tasks
task = run_mdao.celery_tasks.run_one.delay(r"run_mdao\test\mdao_config_basic_CyPhy.json", {"designVariable.x": "Ia", "designVariable.y": -50.0})
task.get(timeout=4)
"""
from __future__ import absolute_import
from __future__ import print_function

from celery import Celery
import redis

import run_mdao
import os
import os.path
import sys
import StringIO
import json
import tempfile
import zipfile
import hashlib
import shutil
import traceback
import six

# if running the worker with gevent, we need to patch subprocess or the inspect and control commands will time out
# celery worker runs monkey.patch_all, but it does not patch subprocess
try:
    import socket
    from gevent import monkey
    if socket.socket.__module__ == 'gevent.socket':
        monkey.patch_subprocess()
except ImportError:
    pass

# TASK_TIMEOUT_S = 5 * 60
TASK_TIMEOUT_S = float('inf')
redis_host = os.environ.get('RUN_MDAO_REDIS_HOST', '192.168.1.205')

redis_conn = redis.StrictRedis(host=redis_host, port=6379, db=0)
app = Celery('tasks', backend='redis://{}/0'.format(redis_host), broker='redis://{}'.format(redis_host))

app.conf.update(
    CELERYD_PREFETCH_MULTIPLIER=1,
    CELERY_ACCEPT_CONTENT=['json', 'msgpack', 'yaml'],
    CELERY_TASK_SERIALIZER='json'
)

_zip_cache = {}

# celery worker: convert WindowsError to something non-Windows Pythons can deserialize
try:
    WindowsError = __builtins__.WindowsError
except AttributeError:
    class WindowsError(OSError):
        pass


@app.task(ignore_result=False, bind=True, max_retries=3, acks_late=True, track_started=True)
def run_one(self, zipkey, *args):
    try:
        if len(_zip_cache) > 10:
            _zip_cache.clear()
        wdir = tempfile.mkdtemp(prefix='mdao-')
        os.chdir(wdir)
        print('Executing {} in {}'.format(zipkey, wdir))
        input_zip = _zip_cache.get(zipkey)
        if input_zip is None:
            _zip_cache[zipkey] = input_zip = redis_conn.get(zipkey)
        with zipfile.ZipFile(StringIO.StringIO(input_zip), 'r') as zf:
            zf.extractall()
        try:
            run_mdao.run_one('mdao_config.json', *args)
        except Exception as exc:
            self.retry(exc=exc)
        os.chdir('/')
        shutil.rmtree(wdir)
    except WindowsError as e:
        raise OSError(e.errno, traceback.traceback.format_exc())


if __name__ == '__main__':
    input_filename = 'mdao_config.json' if len(sys.argv) <= 1 else sys.argv[1]
    input_filename = os.path.abspath(input_filename)
    os.chdir(os.path.dirname(input_filename))
    mdao_config = json.load(open(input_filename))
    zbuff = StringIO.StringIO()
    with zipfile.ZipFile(zbuff, 'w') as zf:
        # this might be too fancy, we can zip everything in 'root_dir'
        # FIXME will not work for FMU components
        for component in six.itervalues(mdao_config['components']):
            try:
                component_dir = component['details']['directory']
            except KeyError:
                continue
            for root, dirs, files in os.walk(component_dir):
                for file in files:
                    zf.write(os.path.join(root, file))
        zf.write(input_filename, 'mdao_config.json')
    zipkey = 'run_mdao_config_' + hashlib.sha224(zbuff.getvalue()).hexdigest()
    redis_conn.set(zipkey, zbuff.getvalue())
    import run_mdao.drivers
    import run_mdao.celery_tasks

    def _run_one(self, problem, run):
        task = run_mdao.celery_tasks.run_one.delay(zipkey, run)
        task.get(timeout=TASK_TIMEOUT_S)

        self.iter_count += 1
        if self.use_restart:
            self.restart.record_iteration()

    run_mdao.drivers.PredeterminedRunsDriver.run_one = _run_one
    run_mdao.run(input_filename)
