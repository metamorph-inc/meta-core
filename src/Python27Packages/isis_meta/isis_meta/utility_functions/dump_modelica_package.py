

class DumpModelicaPackage():
    def __init__(self, libs):
        import OMPython
        self.om = OMPython
        
        # load packages
        self.ask_omc('loadModel', 'Modelica')
        self.libs = libs
        pass
        
    def ask_omc(self, question, opt=None):
        """
        Thin wrapper around the call to OMC.

        If opt==None passes 'question' straight to OMC.  Else passes 'question(opt)' i.e. it saves 
        having to use quoted parentheses in the calling code.
        """
        if opt is None:
            res = self.om.execute(question)
        else:
            res = self.om.execute(question + '(' + opt + ')')
        return res
        
    def dump(self):
        print self._get_packages(self.libs[0])
        print self._get_models('Modelica.Electrical.Analog.Basic')
        
        print 'Output'
        
    def _get_packages(self, package_name):
        return self._get_class_by_type(package_name, 'Package')
        
    def _get_models(self, package_name):
        return self._get_class_by_type(package_name, 'Model')
        
    def _get_class_by_type(self, package_name, type):
        result = []
        for i in self.ask_omc('getClassNames', package_name)['SET1']['Set1']:
            if self.ask_omc('is' + type, package_name + '.' + i):
                result.append(i)
        return result
        
def main():
    dmp = DumpModelicaPackage(['Modelica'])
    dmp.dump()
    
if __name__ == '__main__':
    main()