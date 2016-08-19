from enthought.traits.api import HasTraits
from openmdao.lib.datatypes.api import Int, List
from openmdao.main.interfaces import implements, IDOEgenerator
#from model_calls import run_model

class ListGen(HasTraits):
    """
    DOEgenerator that returns rows in a CSV file.
    Plugs into the DOEgenerator socket on a DOEdriver.
    """
    
    implements(IDOEgenerator)
    
#    inputs = List([], iotype='in', desc='List of parameters.')

    def __init__(self, driver, inputs, *args, **kwargs):
        super(ListGen, self).__init__(*args, **kwargs)
        self.driver = driver
        self.inputs = inputs

    def __iter__(self):
        """ Return an iterator over our sets of input values. """
        return self._next_row()

    def _next_row(self):
        nsims = len(self.inputs)
        for i in range(nsims):
            print 'Running simulation',i+1,'of',nsims,'with inputs',self.inputs[i]
            yield run_model(self.driver, self.inputs[i])

