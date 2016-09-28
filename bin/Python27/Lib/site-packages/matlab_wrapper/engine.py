import os.path
import smop.backend


def import_mfile(mFile):
    buf = open(mFile).read().replace("\r\n", "\n")
    func_list = smop.parse.parse(buf if buf[-1] == '\n' else buf + '\n', mFile)
    return func_list


class SMOPEngine(object):
    def __init__(self):
        self.paths = []
        self.cache = {}

    def addpath(self, path, nargout=0):
        self.paths.append(path)

    def quit(self):
        pass

    def __getattr__(self, name):
        invoke = self.cache.get(name)
        if invoke:
            return invoke

        for mFile in (p for p in (os.path.join(path, name + '.m') for path in self.paths) if os.path.isfile(p)):
            func_list = import_mfile(mFile)
            break
        else:
            raise Exception('Could not find {}.m in {}'.format(name, ';'.join(self.paths)))

        imports = ['from smop.runtime import *',
                   'from smop.core import *']

        # FIXME won't work for files with dependencies
        source = '\n\n'.join(imports + [smop.backend.backend(func_obj) for func_obj in func_list])
        with open(name + '.py', 'wb') as out:
            out.write(source)
        code = compile(source, name + '.py', 'exec')
        mod = {}
        # eval(code, six.moves.builtins, mod)
        eval(code, mod, mod)
        self.func = mod[name]

        def invoke(*args, **kwargs):
            # nargout=0, stdout=None, stderr=None
            return self.func(*args)

        self.cache[name] = invoke
        return invoke
