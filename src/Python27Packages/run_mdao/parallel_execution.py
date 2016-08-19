from __future__ import print_function
from __future__ import absolute_import

import os.path
import json
import zipfile
import socket
import tempfile
import six

from openmdao.core.mpi_wrap import MPI


def par_clone_and_config(filename):
    wdir = tempfile.mkdtemp(prefix='mdao-')
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()

    # Distributing the config from root node to every worker
    if rank == 0:
        with open(filename, 'r') as mdao_config_json:
            mdao_config = json.loads(mdao_config_json.read())
        # Extra info if SFTP/SCP is used to pull from root
        root_ip = socket.gethostbyname(socket.gethostname())
        root_dir = os.path.dirname(os.path.abspath(filename))
        mdao_config['mpi'] = {'root_ip': root_ip, 'root_dir': root_dir}
    else:
        mdao_config = None
    mdao_config = comm.bcast(mdao_config, root=0)

    # Distributing all artifacts (via MPI, for now)
    if rank == 0:
        zbuff = six.BytesIO()
        with zipfile.ZipFile(zbuff, 'w') as zf:
            # this might be too fancy, we can zip everyhting in 'root_dir'
            for component in six.itervalues(mdao_config['components']):
                try:
                    component_dir = component['details']['directory']
                except KeyError:
                    continue
                for root, dirs, files in os.walk(component_dir):
                    for file in files:
                        zf.write(os.path.join(root, file))
    else:
        zbuff = None
    zbuff = comm.bcast(zbuff, root=0)

    # Extracting all artifacts and cd to the work dir
    os.chdir(wdir)
    with zipfile.ZipFile(zbuff, 'r') as zf:
        zf.extractall()

    return mdao_config
