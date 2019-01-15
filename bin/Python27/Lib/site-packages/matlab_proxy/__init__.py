import sys
import six
import pickle
import json
import re
import os
import os.path
import platform
import warnings
import subprocess

from base64 import b64encode, b64decode

class AnalysisError(Exception):
    """The client will convert this to openmdao.api.AnalysisError.
    The proxy server does not have openmdao"""
    pass


class MatlabInProcessProxyReplacement(object):
    def __init__(self, engine):
        self.engine = engine

    def addpath(self, path, nargout=0):
        self.engine.addpath(path, nargout=0)

    def __getattr__(self, name):
        import numpy

        def _invoke(args, nargout=1, bare=False, input_names=None, output_names=None, stdout=None, stderr=None):
            import openmdao.api
            import matlab
            import matlab.engine
            import matlab.mlarray

            def transcode(val):
                if numpy and isinstance(val, numpy.ndarray):
                    return val.tolist()
                if numpy and isinstance(val, (numpy.float16, numpy.float32, numpy.float64)):
                    return float(val)
                return val
            args = [transcode(val) for val in args]

            try:
                if bare:
                    nargout = 0
                    for i, input_name in enumerate(input_names):
                        arg = args[i]

                        if isinstance(args[i], list):
                            if len(arg) and isinstance(arg[0], six.string_types):
                                pass
                            else:
                                arg = matlab.double(arg)
                        self.engine.workspace[str(input_name)] = arg

                    getattr(self.engine, name)(nargout=nargout, stdout=stdout, stderr=stderr)
                    outputs = []

                    def convert_to_list(array, size):
                        if len(size) == 1:
                            return list(array)
                        return list((convert_to_list(l, size[1:]) for l in array))
                    for output_name in output_names:
                        output = self.engine.workspace[str(output_name)]
                        if isinstance(output, matlab.mlarray.double):
                            output = convert_to_list(output, output.size)
                        outputs.append(output)
                    # open('debug.txt', 'a').write(repr(outputs) + '\n')
                else:
                    outputs = getattr(self.engine, name)(*args, nargout=nargout, stdout=stdout, stderr=stderr)
            except Exception as e:
                if type(e).__module__ in ('matlab', 'matlab.engine'):
                    e = openmdao.api.AnalysisError(getattr(e, 'message', getattr(e, 'args', ['unknown MATLAB exception'])[0]))
                    raise e


            # print(repr(outputs))
            return outputs

        _invoke.__name__ = name
        return _invoke

    def quit(self):
        self.engine.quit()


class EngineProxyServer(object):
    def __init__(self, engine):
        self.engine = engine

    def addpath(self, path):
        self.engine.addpath(path, nargout=0)

    def invoke(self, name, args, nargout, bare, input_names, output_names):
        import matlab
        import matlab.engine
        import matlab.mlarray
        out = six.StringIO()
        err = six.StringIO()
        args = pickle.loads(b64decode(args))
        if bare:
            nargout = 0
            for i, input_name in enumerate(input_names):
                arg = args[i]

                if isinstance(args[i], list):
                    if len(arg) and isinstance(arg[0], six.string_types):
                        pass
                    else:
                        arg = matlab.double(arg)

                try:
                    self.engine.workspace[input_name.encode('ascii', 'ignore')] = arg
                except ValueError as e:
                    if e.message == 'invalid field for MATLAB struct':
                        for name in arg:
                            if not re.match('^[a-zA-Z][a-zA-Z0-9_]{0,62}$', name):
                                raise ValueError('invalid field for MATLAB struct "{0}". '.format(name) +
                                    'MATLAB fields must start with a letter and contain only letters, numbers, and underscores. ' +
                                    'MATLAB fields must contain 63 characters or fewer.')
                    raise

            getattr(self.engine, name)(nargout=nargout, stdout=out, stderr=err)
            outputs = []

            def convert_to_list(array, size):
                if len(size) == 1:
                    return list(array)
                return list((convert_to_list(l, size[1:]) for l in array))
            for output_name in output_names:
                output = self.engine.workspace[str(output_name)]
                if isinstance(output, matlab.mlarray.double):
                    output = convert_to_list(output, output.size)
                outputs.append(output)
            # open('debug.txt', 'a').write(repr(outputs) + '\n')
        else:
            outputs = getattr(self.engine, name)(*args, nargout=nargout, stdout=out, stderr=err)

        return {"output": pickle.dumps(outputs), "stdout": out.getvalue(), "stderr": err.getvalue()}

    def quit(self):
        self.engine.quit()


class EngineProxyClient(object):
    def __init__(self, proxy):
        self.proxy = proxy
        numpy = None
        try:
            import numpy
        except ImportError:
            pass
        self.numpy = numpy

    def addpath(self, path, nargout=0):
        self.proxy.addpath(path)

    def quit(self):
        self.proxy.quit()
        self.proxy = None

    def __del__(self):
        if self.proxy:
            self.quit()

    def __getattr__(self, name):
        numpy = self.numpy

        def transcode(val):
            if numpy and isinstance(val, numpy.ndarray):
                return val.tolist()
            if numpy and isinstance(val, (numpy.float16, numpy.float32, numpy.float64)):
                return float(val)
            return val

        def invoke(args, **kwargs):
            # (*args, nargout=len(self._output_names), stdout=out, stderr=err)
            args = map(transcode, args)
            kwargs = {k: transcode(v) for k, v in kwargs.iteritems()}

            # need to pickle here in case args contains type information lost during json transform (eg. int vs float)
            ret = self.proxy.invoke(name, b64encode(pickle.dumps(args)), kwargs.get('nargout'), bare=kwargs['bare'],
                input_names=kwargs['input_names'], output_names=kwargs['output_names'])
            for output in ('stdout', 'stderr'):
                stdout = kwargs.get(output)
                if stdout:
                    stdout.write(ret[output])

            return pickle.loads(ret["output"])

        return invoke


def get_matlab_engine():
    if sys.platform == 'win32':
        matlab = get_preferred_matlab()
        if not matlab:
            warnings.warn("MATLAB not found in registry. Using Python implementation.", RuntimeWarning)
            return None
        if matlab[0] == platform.architecture()[0]:
            MATLABROOT = matlab[2]
            engine = import_matlab_python_engine(MATLABROOT)
            return MatlabInProcessProxyReplacement(engine.start_matlab())
        else:
            python_exe = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'dist_{}\\python.exe'.format(matlab[0]))
            if not os.path.isfile(python_exe):
                raise Exception("'{}' does not exist. Run `setup.py py2exe` with Python {} in the matlab_proxy dir".format(python_exe, matlab[0]))
            return get_engine_proxy(matlab[2], python_exe)
    try:
        import matlab.engine
        return MatlabInProcessProxyReplacement(matlab.engine.start_matlab())
    except ImportError as e:
        warnings.warn("Failed to import matlab.engine: %s" % e, RuntimeWarning)
        return None


def get_engine_proxy(MATLABROOT, python_exe):
    import openmdao.api
    worker = subprocess.Popen([python_exe, '-E', '-S', '-u', os.path.abspath(__file__), MATLABROOT],
        stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.STDOUT)

    magic = worker.stdout.readline().rstrip('\n')
    if magic != HANDSHAKE_MAGIC:
        rest, _ = worker.communicate()
        raise Exception(magic + '\n' + rest)

    def dispatch(method, *args, **kwargs):
        worker.stdin.write(method + '\n')
        # print >> sys.stderr, "AAArgs:", args
        worker.stdin.write(json.dumps(args, ensure_ascii=False) + '\n')
        worker.stdin.write(json.dumps(kwargs, ensure_ascii=False) + '\n')
        e_str = worker.stdout.readline().rstrip('\n')
        # print >> sys.stderr, "XXX:", e_str, "---", len(e_str)
        e = pickle.loads(b64decode(json.loads(e_str)))
        ret_str = worker.stdout.readline().rstrip('\n')
        # print >> sys.stderr, "YYY:", ret_str, "---", len(ret_str)
        ret = pickle.loads(b64decode(json.loads(ret_str)))
        if e:
            if isinstance(e, AnalysisError):
                e = openmdao.api.AnalysisError(getattr(e, 'message', getattr(e, 'args', ['unknown MATLAB exception'])[0]))
            raise e
        return ret

    class Proxy(object):
        def addpath(self, *args, **kwargs):
            return dispatch('addpath', *args, **kwargs)

        def invoke(self, *args, **kwargs):
            return dispatch('invoke', *args, **kwargs)

        def quit(self, *args, **kwargs):
            return dispatch('quit', *args, **kwargs)

    eng = EngineProxyClient(Proxy())

    return eng


def get_preferred_matlab():
    """Return a 3-tuple (arch, version, MATLABROOT) of the latest MATLAB found in the registry."""
    try:
        import _winreg as winreg
    except ImportError:
        import winreg

    def get_latest_matlab(reg_wow64):
        try:
            matlab = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r'SOFTWARE\MathWorks\MATLAB', 0, winreg.KEY_READ | reg_wow64)
        except WindowsError as e:
            if e.winerror != 2:
                raise
            return None
        with matlab:
            matlab_versions = []
            try:
                for i in six.moves.range(1000):
                    matlab_versions.append(winreg.EnumKey(matlab, i))
            except WindowsError as e:
                if e.winerror != 259:
                    raise
            matlab_versions.sort(cmp=lambda a, b: -matlab_version_cmp(a, b))
            for matlab_version in matlab_versions:
                with winreg.OpenKey(matlab, matlab_version, 0, winreg.KEY_READ | reg_wow64) as matlab_version_key:
                    try:
                        value, type_ = winreg.QueryValueEx(matlab_version_key, 'MATLABROOT')
                    except WindowsError as e:
                        if e.winerror != 2:
                            raise
                    else:
                        if type_ in (winreg.REG_SZ, winreg.REG_EXPAND_SZ):
                            return (matlab_version, value)

    m64 = get_latest_matlab(winreg.KEY_WOW64_64KEY)
    m32 = get_latest_matlab(winreg.KEY_WOW64_32KEY)
    if m64 is None and m32 is None:
        return None
    if matlab_version_cmp(m64, m32) == 1 or (matlab_version_cmp(m64, m32) == 0 and platform.architecture()[0] == '64bit'):
        return ('64bit',) + m64
    else:
        return ('32bit',) + m32


def matlab_version_cmp(a, b):
    if a is None and b is None:
        return 0
    if a is None or b is None:
        return cmp(a, b)
    # e.g. R2016a: 9.0
    # R2015aSP1: 8.5.1
    a_match = re.match('(\\d+)\\.(\\d+)(?:\\.(\\d+))?', a)
    b_match = re.match('(\\d+)\\.(\\d+)(?:\\.(\\d+))?', b)
    if a_match is None and b_match is None:
        return cmp(a, b)
    if a_match is None:
        return -1
    if b_match is None:
        return 1
    return cmp([int(x) for x in a_match.groups(0)],
               [int(x) for x in b_match.groups(0)])


def import_matlab_python_engine(MATLABROOT):
    win_bit = {'32bit': 'win32',
            '64bit': 'win64'}[platform.architecture()[0]]
    old_path = os.environ['PATH']
    os.environ['PATH'] = r'{}\bin\{}'.format(MATLABROOT, win_bit) + os.pathsep + os.environ['PATH']
    # numpy modifies PATH to add tbb.dll et al. But its tbb.dll is incompatible with MATLAB's
    # os.environ['PATH'] = os.pathsep.join(p for p in os.environ['PATH'].split(os.pathsep) if 'numpy' not in p)
    try:
        sys.path.insert(0, r"{}\extern\engines\python\dist\matlab\engine\{}".format(MATLABROOT, win_bit))
        sys.path.insert(1, r"{}\extern\engines\python\dist".format(MATLABROOT))
        try:
            import importlib
            importlib.import_module('matlabengineforpython{}'.format('_'.join(map(str, sys.version_info[0:2]))))
            from matlab import engine
            return engine
        finally:
            del sys.path[:2]
    finally:
        os.environ['PATH'] = old_path

HANDSHAKE_MAGIC = 'matlab_\bproxy'

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('MATLABROOT')
    args = parser.parse_args()

    # move __main__.AnalysisError to matlab_proxy.AnalysisError so the client can unpickle it
    # (these few lines are easier than ensuring matlab_proxy can be imported in all cases)
    from types import ModuleType
    matlab_proxy = ModuleType('matlab_proxy')
    matlab_proxy.AnalysisError = AnalysisError
    AnalysisError.__module__ = matlab_proxy.__name__
    AnalysisError = matlab_proxy.AnalysisError
    sys.modules['matlab_proxy'] = matlab_proxy

    MATLABROOT = args.MATLABROOT

    engine = EngineProxyServer(import_matlab_python_engine(MATLABROOT).start_matlab())
    sys.stdout.write(HANDSHAKE_MAGIC + '\n')

    while True:
        # debug = open('method.txt', 'wb')
        # while True:
        #    debug.write(sys.stdin.readline())
        #    debug.flush()
        method = sys.stdin.readline().rstrip('\n')
        args = sys.stdin.readline().rstrip('\n')
        args = json.loads(args)
        kwargs = json.loads(sys.stdin.readline().rstrip('\n'))
        e = None
        ret = None
        try:
            ret = getattr(engine, method)(*args, **kwargs)
        except Exception as e:
            # import traceback
            # traceback.print_exc(100, open('exception{}.txt'.format(method), 'w'))
            # n.b. consumer doesn't have these modules, so create an exception of a different type
            if type(e).__module__ in ('matlab', 'matlab.engine'):
                e = AnalysisError(getattr(e, 'message', getattr(e, 'args', ['unknown MATLAB exception'])[0]))

        sys.stdout.write(json.dumps(b64encode(pickle.dumps(e))) + '\n')
        # open('exception{}.txt'.format(method), 'w').write(pickle.dumps(ret))
        sys.stdout.write(json.dumps(b64encode(pickle.dumps(ret))) + '\n')
        sys.stdout.flush()
        if method == 'quit':
            break
