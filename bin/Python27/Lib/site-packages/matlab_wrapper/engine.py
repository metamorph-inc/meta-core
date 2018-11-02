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
        def invoke(args, **kwargs):
            code = self.cache.get(name)
            if not code:

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
                self.cache[name] = code

            mod = {}
            if kwargs['bare']:
                for i, input_name in enumerate(kwargs['input_names']):
                    mod[input_name] = args[i]
            # eval(code, six.moves.builtins, mod)
            eval(code, mod, mod)
            if kwargs['bare']:
                ret = []
                for i, output_name in enumerate(kwargs['output_names']):
                    # FIXME
                    ret.append(mod[output_name])
                return ret
            else:
                func = mod[name]
                return func(*args)

        return invoke
