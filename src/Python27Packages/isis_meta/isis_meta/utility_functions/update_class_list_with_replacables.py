import OMPython
import json
import os
import sys
import subprocess

def main():
    print 'Get all classes and detect test benches... using getTestBenches.mos'
    # generate test_benches.json file
    subprocess.check_output([os.path.join(os.environ['OPENMODELICAHOME'], 'bin', 'omc'), 'getTestBenches.mos'])

    test_benches = {}
    
    with open('test_benches.json', 'r') as f_p:
        test_benches = json.load(f_p)

    

    OMPython.execute("cd()")
    modelica_lib_loaded = OMPython.execute('loadModel(Modelica,{"3.2"})')
    print modelica_lib_loaded
    c2m2l_loaded = OMPython.execute('loadFile("C2M2L_Ext/package.mo")')
    print c2m2l_loaded
    

    # allClassNames = OMPython.execute('getClassNames(C2M2L_Ext, qualified = true, recursive=true, sort=true)')

    # allClassNames = allClassNames['SET1']['Set1']
    
    # test_benches_new = {}
    # num_all = len(allClassNames)
    # index = 0
    # tbs = 0
    # for item in allClassNames:
        # index = index + 1
        # sys.stdout.write('{0}/{1} [{2} test benches] {3}\r'.format(index, num_all, tbs, item))
        # sys.stdout.flush()
        # isExtends = False
        # #isExtends = OMPython.execute('extendsFrom({0}, Icons.TestBench)'.format(item))
        # isExtends = isExtends or OMPython.execute('extendsFrom({0}, C2M2L_Ext.Icons.TestBench)'.format(item))
        # if isExtends:
            # tbs = tbs + 1
            # test_benches_new[item] = isExtends
            # print '{0} {1}'.format(isExtends, item)
    # print test_benches_new
    # return
    test_benches_out = {}
      
    num_all = len(test_benches.keys())
    index = 0
    tbs = 0
    w_r = 0
    print 'Detecting replacable components in test benches...'
    for class_name in test_benches.keys():
        index = index + 1
        sys.stdout.write('{0}/{1} [{2} test benches {3} with replacable] {4}\r'.format(index, num_all, tbs, w_r, class_name))
        sys.stdout.flush()
        if test_benches[class_name]:
            tbs = tbs + 1
            #print class_name
            command = 'getComponents({0})'.format(class_name)
            #print command
            try:
                components = OMPython.execute(command)
            except Exception as e:
                #print 'Failed: {0}'.format(command)
                pass

            # iterate through SET2
            for k in components['SET2'].keys():
                component = components['SET2'][k]
                # replacable
                if len(component) > 7 and component[7] == 'true':
                    w_r = w_r + 1
                    if not class_name in test_benches_out:
                        test_benches_out[class_name] = {'parameters': []}
                    #print ' - {0} {1}'.format(component[0], component[1])
                    test_benches_out[class_name]['parameters'].append({'constraint_clause':component[0], 'name':component[1], 'default_instance':None})
            
    with open('test_benches_out.json', 'w') as f_p:
        json.dump(test_benches_out, f_p)

    return test_benches_out
if __name__ == '__main__':
    main()