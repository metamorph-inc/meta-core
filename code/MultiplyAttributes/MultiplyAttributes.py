
attr_map = { "Weight": 1000,
     "Volume": 100 * 1000 }

import sys
import os
import os.path

import udm

if os.path.dirname(udm.__file__) != os.path.dirname(os.path.abspath(__file__)):
    raise ImportError('Need to import the udm.pyd next to this file: ' + udm.__file__ + ";" + __file__)

def dfs(start, getter):
    q = []
    q.extend(start)
    while len(q) != 0:
        o = q.pop()
        q.extend(getter(o))
        yield o

if __name__ == '__main__':
    import shutil
    srcfile = sys.argv[1]
    destfile = os.path.splitext(srcfile)[0] + "_multiplied.mga"
    shutil.copy(srcfile, destfile)

    meta_dn = udm.SmartDataNetwork(udm.uml_diagram())
    meta_dn.open(os.path.join(os.path.dirname(__file__), r"CyPhyML_udm.xml"), "")
    meta = udm.map_uml_names(meta_dn.root)
    
    dn = udm.SmartDataNetwork(meta_dn.root)
    dn.open(destfile, "")
    
    q = [dn.root]
    while len(q) != 0:
        o = q.pop()
        q.extend(o.children())
        if o.is_instance:
            for (attrname, mult) in attr_map.items():
                try:
                    # i.e. o.Weight = o.archetype.Weight * 1000
                    o.__setattr__(attrname, o.archetype.__getattr__(attrname) * mult)
                except:
                    pass
        
    dn.close_with_update()
