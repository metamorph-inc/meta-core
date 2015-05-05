#!/usr/bin/env python

# FFNET TESTS
import unittest
import sys; import os
from ffnet import *
from ffnet import _linear, _norms, _normarray, _ffconec, _bconec, _dconec

# Get ffnet module realpath
ffnetpath = os.path.dirname(os.path.realpath(__file__)) + '/'

# ffnet module tests #########################
class Testmlgraph(unittest.TestCase):
    def testEmpty(self):
        arch = ()
        conec = mlgraph(arch)
        self.assertEqual(conec, [])
    def testOneLayer(self):
        arch = (5,)
        conec = mlgraph(arch)
        self.assertEqual(conec, [])
    def testTwoLayers(self):
        arch = (1,1)
        conec = mlgraph(arch)
        self.assertEqual(conec, [(1,2), (0,2)])
    def testThreeLayers(self):
        arch = (2,2,1)
        conec = mlgraph(arch)
        conec0 = [(1, 3), (2, 3), (0, 3), \
                  (1, 4), (2, 4), (0, 4), \
                  (3, 5), (4, 5), (0, 5)]
        self.assertEqual(conec, conec0)
    def testNoBiases(self):
        arch = (2,2,1)
        conec = mlgraph(arch, biases = False)
        conec0 = [(1, 3), (2, 3), \
                  (1, 4), (2, 4), \
                  (3, 5), (4, 5)]
        self.assertEqual(conec, conec0)

class Testimlgraph(unittest.TestCase):
    def testEmpty(self):
        arch = ()
        self.assertRaises(TypeError, imlgraph, arch)
    def testOneLayer(self):
        arch = (5,)
        self.assertRaises(TypeError, imlgraph, arch)
    def testTwoLayers(self):
        arch = (1,1)
        self.assertRaises(TypeError, imlgraph, arch)
    def testThreeLayers(self):
        arch = (2,2,2)
        conec = imlgraph(arch)
        conec0 = [(1, 3), (2, 3), (0, 3), \
                  (1, 4), (2, 4), (0, 4), \
                  (3, 5), (4, 5), (0, 5), \
                  (1, 6), (2, 6), (0, 6), \
                  (1, 7), (2, 7), (0, 7), \
                  (6, 8), (7, 8), (0, 8)]
        self.assertEqual(conec, conec0)
    def testNoBiases(self):
        arch = (2,[(2,), (2,)],2)
        conec = imlgraph(arch, biases = False)
        conec0 = [(1, 3), (2, 3), \
                  (1, 4), (2, 4), \
                  (3, 5), (4, 5), \
                  (1, 6), (2, 6), \
                  (1, 7), (2, 7), \
                  (6, 8), (7, 8),]
        self.assertEqual(conec, conec0)


class Testtmlgraph(unittest.TestCase):
    def testEmpty(self):
        arch = ()
        conec = tmlgraph(arch)
        self.assertEqual(conec, [])
    def testOneLayer(self):
        arch = (5,)
        conec = tmlgraph(arch)
        self.assertEqual(conec, [])
    def testTwoLayers(self):
        arch = (1,1)
        conec = tmlgraph(arch)
        self.assertEqual(conec, [(0,2), (1,2)])
    def testThreeLayers(self):
        arch = (2,1,1)
        conec = tmlgraph(arch)
        conec0 = [(0, 3), (1, 3), (2, 3), \
                  (0, 4), (1, 4), (2, 4), (3, 4)]
        self.assertEqual(conec, conec0)
    def testNoBiases(self):
        arch = (2,1,1)
        conec = tmlgraph(arch, biases = False)
        conec0 = [(1, 3), (2, 3), \
                  (1, 4), (2, 4), (3, 4)]
        self.assertEqual(conec, conec0)

class Testlinear(unittest.TestCase):
    def testEqualInRanges(self):
        #self.assertRaises(ValueError, linear, 1.0, 1.0, 2.0, 3.0)
        self.assertEqual(_linear(1.0, 1.0, 2.0, 3.0), (0, 2.5))
    def testEqualOutRanges(self):
        self.assertEqual(_linear(2.0, 3.0, 1.0, 1.0), (0.0, 1.0))
    def testNormalCase(self):
        self.assertEqual(_linear(0.0, 1.0, 0.0, 2.0), (2.0, 0.0))

class Testnorms(unittest.TestCase):
    def testEmpty(self):
        inarray = [[], []]
        n = _norms(inarray)
        for i in xrange(len(n)):
            self.assertEqual(n[i].tolist(), [])
    def testOneColumn(self):
        inarray = [[0.], [1.], [2.]]
        n = _norms(inarray)
        bool1 = n[0].tolist() == [[0., 2.]]
        bool2 = n[1].tolist() == [[0.5, 0.]]
        bool3 = n[2].tolist() == [[2., 0.]]
        self.assert_(bool1 and bool2 and bool3)
    def testNormalCase(self):
        inarray = [[0.,0.], [0.,1.], [1.,0.], [1.,1.]]
        n = _norms(inarray, lower=0.15, upper=0.85)
        self.assertEqual(n[0].tolist(), [[0., 1.], [0, 1.]])
        self.assertEqual(n[1].tolist(), [[0.7, 0.15], [0.7, 0.15]])
        self.assertAlmostEqual(n[2][0,0], 1.42857143, 8)
        self.assertAlmostEqual(n[2][0,1], -0.21428571, 8)
        
class Testnormarray(unittest.TestCase):
    def testEmpty(self):
        inarray = [[], []]
        n = _normarray(inarray, [])
        for i in xrange(len(n)):
            self.assertEqual(n[i].tolist(), [])
    
    def testOneColumn(self):
        inarray = [[0.], [1.], [1.], [0.]]
        coeff = [[0.7, 0.15]]
        n = _normarray(inarray, coeff)
        for i in xrange(4):
            self.assertAlmostEqual(n[i,0], coeff[0][0]*inarray[i][0] + coeff[0][1], 8)
            
class Testffconec(unittest.TestCase):
    def testEmpty(self):
        conec = []
        self.assertRaises(ValueError, _ffconec, conec)

    def testWithCycles(self):
        from networkx import NetworkXUnfeasible
        conec = [(1, 3), (2, 3), (0, 3), (3, 1), \
                 (1, 4), (2, 4), (0, 4), (4, 2), \
                 (3, 5), (4, 5), (0, 5), (5, 1) ]
        self.assertRaises(NetworkXUnfeasible, _ffconec, conec)

    def testNoCycles(self):
        conec = [(1, 3), (2, 3), (0, 3), \
                 (1, 4), (2, 4), (0, 4), \
                 (3, 5), (4, 5), (0, 5) ]
        n = _ffconec(conec)
        n[2].sort(); n[3].sort(); n[4].sort()
        self.assertEqual(n[2], [1, 2])
        self.assertEqual(n[3], [3, 4])
        self.assertEqual(n[4], [5])
        
class Testbconec(unittest.TestCase):
    def testNoCycles(self):
        conec = [(1, 3), (2, 3), (0, 3), \
                 (1, 4), (2, 4), (0, 4), \
                 (3, 5), (4, 5), (0, 5) ]
        inno = [1,2]
        n = _bconec(conec, inno)
        self.assertEqual(n[1], [8,7])
        
class Testdconec(unittest.TestCase):
    def testNoCycles(self):
        conec = [(1, 3), (2, 3), (0, 3), \
                 (1, 4), (2, 4), (0, 4), \
                 (3, 5), (4, 5), (0, 5) ]
        inno = [1,2]
        n = _dconec(conec, inno)
        self.assertEqual(n[1], [1, 4, 7, 8, 2, 5, 7, 8])
        self.assertEqual(n[2], [0, 4, 8])
        
class Testdconec2(unittest.TestCase):
    def testNoCycles(self):
        conec = [(1, 3), (2, 3), (0, 3), \
                 (1, 4), (0, 4), \
                 (3, 5), (4, 5), (0, 5) ]
        inno = [1,2]
        n = _dconec(conec, inno)
        self.assertEqual(n[1], [1, 4, 6, 7, 2, 6])
        self.assertEqual(n[2], [0, 4, 6])
        
class TestFfnetSigmoid(unittest.TestCase):
    def setUp(self):
        self.conec = [(0, 3), (1, 3), (2, 3), \
                      (0, 4), (1, 4), (2, 4), (3, 4)]
        
        self.net = ffnet(self.conec); self.net([1.,1.]) #try if net works
        self.net.weights = array([1.]*7)
        
        self.tnet = ffnet(self.conec)
        self.tnet.weights = array([ 0.65527021, -1.12400619, 0.02066321, \
                                   0.13930684, -0.40153965, 0.11965115, -1.00622429 ])       
        self.input = [[0.,0.], [0.,1.], [1.,0.], [1.,1.]]
        self.target  = [[1.], [0.], [0.], [1.]]
        
    def testCall(self):
        self.assertEqual(self.net([0., 0.]), self.net.call([0., 0.]))
        self.assertAlmostEqual(self.net([0., 0.])[0], 0.8495477739862124, 8)
        self.assertAlmostEqual(self.net([[0., 0.], [0., 0.]])[1, 0], 0.8495477739862124, 8)
        
    def testDerivative(self):
        self.assertAlmostEqual(self.net.derivative([0., 0.])[0][0], 0.1529465741023702, 8)
        self.assertAlmostEqual(self.net.derivative([0., 0.])[0][1], 0.1529465741023702, 8)
        self.assertAlmostEqual(self.net.derivative([[0., 0.], [0., 0.]])[1, 0, 0], 0.1529465741023702, 8)
        self.assertAlmostEqual(self.net.derivative([[0., 0.], [0., 0.]])[1, 0, 1], 0.1529465741023702, 8)

    def testDerivative2(self):
        conec = [(1, 3), (2, 3), (0, 3), \
                 (1, 4), (0, 4), \
                 (3, 4), (4, 5), (0, 5),
                 (4, 6), (3, 6), (5, 6) ]
        net = ffnet(conec)
        y1n, y2n = net.derivative([1, 1])[0]
        from scipy import derivative
        def func1(x):
            return net([x, 1])[0]
        def func2(x):
            return net([1, x])[0]
        y1 = derivative(func1, 1, dx=0.001)
        y2 = derivative(func2, 1, dx=0.001)
        self.assertAlmostEqual(y1n, y1, 7)
        self.assertAlmostEqual(y2n, y2, 7)

    
    def testSqerror(self):
        err = self.tnet.sqerror(self.input, self.target)
        out = [ (self.tnet(self.input[i])[0] - self.target[i][0])**2 \
                for i in xrange( len(self.input) ) ]
        pyerr = 0.5 * sum(out)
        self.assertNotAlmostEqual(err, pyerr, 8) #err is for normalized data, pyerr for raw data

    def testSqgrad(self):
        #self.tnet._setnorm(self.input, self.target) # Possible bug, this shouldn't be here
        g = self.tnet.sqgrad(self.input, self.target)
        w1 = self.tnet.weights - g

        self.tnet.train_momentum(self.input, self.target, eta=1., momentum=0., maxiter=1)
        w2 = self.tnet.weights
        
        for i in xrange(len(w1)):
            self.assertAlmostEqual(w1[i], w2[i], 8)
            

    def testTrainGenetic(self):
        print "Test of genetic algorithm optimization"
        self.tnet.train_genetic(self.input, self.target, lower = -50., upper = 50., \
                                individuals = 20, generations = 1000)
        self.tnet.test(self.input, self.target)
    
    def testTrainMomentum(self): 
        print "Test of backpropagation momentum algorithm"
        self.tnet.train_momentum(self.input, self.target, maxiter=10000)
        self.tnet.test(self.input, self.target)

    def testTrainRprop(self): 
        print "Test of rprop algorithm"
        self.tnet.randomweights()
        xmi = self.tnet.train_rprop(self.input, self.target, \
                                    a=1.2, b=0.5, mimin=0.000001, mimax=50, \
                                    xmi=0.1, maxiter=10000, disp=1)
        self.tnet.test(self.input, self.target)

    def testTrainCg(self):
        print "Test of conjugate gradient algorithm"
        self.tnet.train_cg(self.input, self.target, maxiter=1000, disp=1)
        self.tnet.test(self.input, self.target)
        
    def testTrainBfgs(self):
        print "Test of BFGS algorithm"
        self.tnet.train_bfgs(self.input, self.target, maxfun = 1000)
        self.tnet.test(self.input, self.target)
        
    def testTrainTnc(self):
        print "Test of TNC algorithm"
        self.tnet.train_tnc(array(self.input), array(self.target), maxfun = 1000)
        self.tnet.test(self.input, self.target)

    def testTrainTncMp(self):
        print "Test of TNC parallel algorithm"
        self.tnet.train_tnc(array(self.input), array(self.target), nproc = 2, maxfun = 1000)
        self.tnet.test(self.input, self.target)
        
    def testRenormalize(self):
        self.tnet._setnorm(self.input, self.target)
        a = self.tnet([1., 1.])
        input2 = [[0.,0.], [0.,2.], [2.,0.], [2.,2.]]
        target2  = [[2.], [0.], [0.], [2.]]
        self.tnet._setnorm(input2, target2)
        b = self.tnet([1., 1.])
        self.assertAlmostEqual(a, b, 15)
        
        self.tnet.renormalize = True
        self.tnet._setnorm(input2, target2)
        c = self.tnet([1., 1.])
        self.assertNotAlmostEqual(a, c, 15)

    def testTestdata(self):
        net = ffnet( mlgraph((1, 5, 1)) )
        input = [1, 2., 5]
        target = [2, 3, 5.]
        net.train_tnc(input, target, maxfun = 10)
        
        
class TestSaveLoadExport(unittest.TestCase):
    def setUp(self):
        conec = imlgraph( (5,5,5) )
        self.net = ffnet(conec)
        
    def tearDown(self):
        import os
        try: os.remove('tmpffnet.f')
        except: pass
        try: 
            os.remove('tmpffnet.so')
            os.remove('tmpffnet2.so')
        except: pass
        try: os.remove('tmpffnet.pyd')
        except: pass
        try: os.remove('tmpffnet.net')
        except: pass

    def testSaveLoad(self):
        res1 = self.net( [ 1, 2, 3, 4, 5. ] )
        savenet( self.net, 'tmpffnet.net' )
        net = loadnet( 'tmpffnet.net' )
        res2 = net( [ 1, 2, 3, 4, 5. ] )
        for i in xrange(5):
            self.assertAlmostEqual(res1[i], res2[i], 8)
        
    def testExportWithDerivative(self):
        print os.getcwd()
        exportnet(self.net, 'tmpffnet.f')
        ## THE BELOW IS PLATFORM AND ffnet.f FILE DEPENDENT 
        ## SHOULD BE COMMENTED FOR RELEASES ???
        from numpy import f2py, array
        f = open( 'tmpffnet.f', 'r' ); source = f.read(); f.close()
        f = open( ffnetpath + 'fortran/ffnet.f', 'r' ); source += f.read(); f.close()
        import sys
        if sys.platform == 'win32':
            eargs = '--compiler=mingw32'
        else: eargs = ''
        f2py.compile(source, modulename = 'tmpffnet', extra_args = eargs, verbose = 0)
        import tmpffnet
        resA1 = tmpffnet.ffnet( [ 1, 2, 3, 4, 5. ] )
        resB1 = tmpffnet.dffnet( [ 1, 2, 3, 4, 5. ] )
        del tmpffnet
        resA = self.net ( [ 1, 2, 3, 4, 5. ] )
        resB = self.net.derivative( [ 1, 2, 3, 4, 5. ] )
        for i in xrange(5):
            self.assertAlmostEqual(resA[i], resA1[i], 15)
            for j in xrange(5):
                self.assertAlmostEqual(resB[i][j], resB1[i][j], 15)

    def testExportNoDerivative(self):
        exportnet(self.net, 'tmpffnet.f', derivative = False)
        ## THE BELOW IS PLATFORM AND ffnet.f FILE DEPENDENT 
        ## SHOULD BE COMMENTED FOR RELEASES ???
        from numpy import f2py
        f = open( 'tmpffnet.f', 'r' ); source = f.read(); f.close()
        f = open( ffnetpath + 'fortran/ffnet.f', 'r' ); source += f.read(); f.close()
        import sys
        if sys.platform == 'win32':
            eargs = '--compiler=mingw32'
        else: eargs = ''
        f2py.compile(source, modulename = 'tmpffnet2', extra_args = eargs, verbose = 0)
        import tmpffnet2
        resA1 = tmpffnet2.ffnet( [ 1, 2, 3, 4, 5. ] )
        resA = self.net ( [ 1, 2, 3, 4, 5. ] )
        for i in xrange(5):
            self.assertAlmostEqual(resA[i], resA1[i], 15)
        self.assertRaises(AttributeError, lambda: tmpffnet2.dffnet([ 1, 2, 3, 4, 5. ]))

class TestDataReader(unittest.TestCase):
    #def setUp(self):
        #self.filename = ffnetpath + 'examples/data/ocr.dat'
    
    def testReadData(self):
        try: data = readdata( ffnetpath + 'examples/data/ocr.dat' )
        except: data = readdata( ffnetpath + '../examples/data/ocr.dat')
        self.assertEqual(data.shape, (68, 74))
        self.assertEqual(data.dtype.name, 'float64')

from _py2f import *
# _py2f module tests #########################
class TestExport2Fortran(unittest.TestCase):  #not finished, just started
    def setUp(self):
        from numpy import array
        self.A = array([[1,2,3], [4,5,6]])
        self.B = array([[1,2,3], [4,5,6.]])
        
    def tearDown(self):
        pass
    
    def testArray(self):
        s1 = farray(self.A, 'test')      
        s2 = farray(self.B, 'test')

def runtest():
   ''' Runs all tests from this module '''
   import sys
   suite = unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])
   unittest.TextTestRunner(verbosity=2).run(suite)        
    

# run tests
if __name__ == '__main__':
    unittest.main(verbosity = 2)
    