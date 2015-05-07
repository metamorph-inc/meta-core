from __future__ import with_statement

import sys
import gme
import gc
import os


def compare (file1, file2):
    with gme.Project.open(file1) as project1:
        with gme.Project.open(file2) as project2:
            return compare2(project1, project2)


def iter_parents(fco):
    while fco:
        yield fco
        if fco.ObjType == 6:
            fco = fco.ParentFolder
        else:
            if fco.ParentFolder:
                fco = fco.ParentFolder
            else:
                fco = fco.ParentModel

def fco_path(fco):
    parents = list(iter_parents(fco))
    parents.reverse()
    return "/".join(map(lambda x: x.Name, parents))

def _get_Meta(object):
    if object.ObjType == 6:
        return object.MetaFolder
    return object.Meta

def _get_children(object):
    if object.ObjType == 6:
        return list(object.ChildFolders) + list(object.ChildFCOs)
    return list(object.ChildFCOs)

def compare2(project1, project2):
    toProcess1=[]
    toProcess2=[]
    
    current1=None
    current2=None
    
    project1Root=project1.project.RootFolder
    toProcess1.append(project1Root)
    project2Root=project2.project.RootFolder
    toProcess2.append(project2Root)
    
    current1=toProcess1.pop()

    while current1!=None:
        current1 = current1
        if len(toProcess2)==0:
            print "Print nothing to process for " + current2.Name
            return False

        for index in range(len(toProcess2)):
            current2 = toProcess2[index]
            namesEqual = current1.Name == current2.Name
            kindsEqual = _get_Meta(current1).Name == _get_Meta(current2).Name
            connectionEndpointsEqual = True
            if current1.ObjType == 4 and current2.ObjType == 4:
                def mapConnpoints(conn):
                    ret = map(lambda point: (point.ConnRole, [fco_path(point.Target), map(fco_path, point.References)]), conn.ConnPoints)
                    return dict(ret)
                connectionEndpointsEqual = mapConnpoints(current1) == mapConnpoints(current2)
                # Debugging aid:
                #print "1 " + str(mapConnpoints(current1))
                #print "2 " + str(mapConnpoints(current2))
            if namesEqual and kindsEqual and connectionEndpointsEqual:
                toProcess2.remove(current2)
                break
        else:
            print "Found nothing corresponding to " + fco_path(current1) +" of kind \"" + current1.MetaBase.Name +"\""
            return False
        
        if current1.AbsPath!=current2.AbsPath and current1.Name !=current2.Name :
            # FIXME: KMS: AbsPath includes "relpos". Could this lead to false negatives?
            print current1.AbsPath," != ",current2.AbsPath
            return False
        
        if current1.ObjType != current2.ObjType:
            print "'%s' has differing object types" % current1.AbsPath
            return False

        if _get_Meta(current1).Name != _get_Meta(current2).Name:
            print "'%s' has differing kinds" % current1.AbsPath
            return False

        #FCO: attributes
        if current1.ObjType != 6:
            if not compareAttrib(current1, current2):
                return False
        
        #ATOMS
        if current1.ObjType == current2.ObjType and current2.ObjType ==2 :
            if compareAssoc(current1.PartOfConns,current2.PartOfConns):
                try:
                    current1=toProcess1.pop(0)
                except IndexError:
                    if len(toProcess2) == 0:
                        return True
                    else:
                        print "Atoms:: Wrong"
                        return False
                current2=None
                continue
            else:
                return False
        
        
        #REFERENCE
        if current1.ObjType == 3 and current2.ObjType == 3 :
            if compareReference(current1,current2) and compareAssoc(current1.PartOfConns,current2.PartOfConns):
                try:
                    current1=toProcess1.pop(0)
                except IndexError:
                    if len(toProcess2) == 0:
                        return True
                    else:
                        print "Reference has something"
                        return False
                current2=None
                continue
            else:
                print "compareReference FAILED for "+current1.Name+" "+current2.Name
                return False
        
        
        #Sets
        if current1.ObjType == 5 and current2.ObjType == 5:
            if compareSet(current1,current2)and compareAssoc(current1.PartOfConns,current2.PartOfConns):
                try:
                    current1=toProcess1.pop(0)
                except IndexError:
                    if len(toProcess2) == 0:
                        return True
                    else:
                        print "Set 2 has something"
                        return False
                current2=None
                continue
            else:
                print "compareSet FAILED for "+current1.Name+" "+current2.Name
                return False
        
        if current1.ObjType == 1 and current2.ObjType == 1:
            if not compareAssoc(current1.PartOfConns,current2.PartOfConns):
                return False
        
        #For FCO's that are connections we need to execute just this "if" block
        if current1.ObjType == 4 and current2.ObjType == 4:
            try:
                current1=toProcess1.pop(0)
            except IndexError:
                if len(toProcess2) == 0:
                    return True
                else:
                    print "CONNECTION:: Proc 2 has something"
                    return False
            current2=None
            continue
        
        
        childrenSet1 = _get_children(current1)
        childrenSet2 = _get_children(current2)
        toProcess1.extend(childrenSet1)
        toProcess2.extend(childrenSet2)
        
        if len(childrenSet1)!= len(childrenSet2):
            print "LENGTH of childrenSet FAILED for " + fco_path(current1)
            print "\t" + "File 1: " + ", ".join([child.Name for child in _get_children(current1)])
            print "\t" + "File 2: " + ", ".join([child.Name for child in _get_children(current2)])
            return False
        
        if current1.ObjType == 6 and current2.ObjType == 6:
            toProcess1.extend(current1.ChildFolders)
            toProcess2.extend(current2.ChildFolders)

        try:
            current1=toProcess1.pop(0)
        except IndexError:
            if len(toProcess2) == 0:
                return True
            else:
                print "END Proc 2 has something"
                return False
        continue

def compareSet(*setFCOs):
    array=[]
    for index in range(len(setFCOs)):
        array.append(dict())
        array[index][setFCOs[index].Name]=setFCOs[index].AbsPath
    
    for index in range(len(setFCOs)-1):
        if(array[index] != array[index+1]):
            print "Returning False from "+sys._getframe().f_code.co_name
            return False
    return True

def compareReference(*referenceFCOs):
    array=[]
    for index in range(len(referenceFCOs)):
        array.append(dict())
        array[index][referenceFCOs[index].Name]=referenceFCOs[index].AbsPath
    
    for index in range(len(referenceFCOs)-1):
        if(array[index] != array[index+1]):
            print "Returning False from "+sys._getframe().f_code.co_name
            return False
    return True
   
def compareAssoc(*current):
    array=[]
    for index in range(len(current)):
        array.append(dict())
        for point in current[index]:
            array[index][point.ConnRole]="%s %s" % (point.Target.Name,point.Target.AbsPath)
    
    for index in range(len(current)-1):
        if(array[index] != array[index+1]):
            print "Returning False from "+sys._getframe().f_code.co_name
            return False
    return True

def compareAttrib(fco1, fco2):
    def mapAttribs(fco):
        ret = map(lambda attr: (attr.Meta.Name, attr.Value), fco.Attributes)
        return dict(ret)
    fco1Attribs = mapAttribs(fco1)
    fco2Attribs = mapAttribs(fco2)
    if fco1Attribs != fco2Attribs:
        print "'%s' has differing attributes:" % fco1.AbsPath
        unequalkeys = filter(lambda key: fco1Attribs.get(key) != fco2Attribs.get(key), fco1Attribs.keys())
        print "\n".join(map(lambda key: "Name: %s: '%s' != '%s' " % (key, fco1Attribs.get(key), fco2Attribs.get(key)), unequalkeys))
        return False
    return True

if __name__ == "__main__":

    if(len(sys.argv)==3):
        file1=sys.argv[1]
        file2=sys.argv[2]
        if not os.path.isfile(file1):
            print False
        os.path.isfile(file2)
        if(compare(file1,file2)):
            print "'%s','%s' are the same" % (file1, file2)
        else:
            print "'%s','%s' are not the same" % (file1, file2)
        gc.collect()
    else:
        print "Need exactly two arguments"