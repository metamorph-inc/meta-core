import _winreg as wreg
import cPickle as pickle

class WindowsRegistry:

    def __init__(self, company="ISIS", project="META", write=1):
        """
        handle registry access
        """
        self.write = write
        self.company = company
        self.project = project
        self.keyname = "Software\\%s\\%s" % (self.company, self.project)

        try:
            self.key = wreg.OpenKey(wreg.HKEY_CURRENT_USER, self.keyname)
        except:
            if self.write:
                self.key = wreg.CreateKey(wreg.HKEY_CURRENT_USER, self.keyname)

    def set(self, name, value):
        " set value in registry "
        if not self.write:
            raise Exception, "registry is read only"
        wreg.SetValue(self.key, name, wreg.REG_SZ,str(value))

    def pset(self, name, value):
        " set using pickle "
        self.set(name, pickle.dumps(value))

    def get(self, name):
        " get value out of registry "
        return wreg.QueryValue(self.key, name)

    def pget(self, name):
        " get using pickle "
        return pickle.loads(self.get(name))

    def close(self):
        " close the key finally "
        self.key.Close()

    def __del__(self):
        self.close()
