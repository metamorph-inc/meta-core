# .py script to update settings.js 

import sys
import os
import json



def update_settings(settings_file):
    file = open(settings_file, "r")
    data = json.load(file)
    file.close()
    
    data["path_to_instance_stls"] = "STL_BINARY"
    data["metrics_file"] = "CADAssembly_metrics.xml"
    data["path_to_instance_xmls"] = "ComponentACMs"
    try:
        from mayavi import mlab
        data["show_3d"] == True
    except ImportError:
        data["show_3d"] == False
    file = open(settings_file, "w+")
    file.write(json.dumps(data))
    file.close()
          
        
if __name__ == "__main__":
    settings_file = "settings.js"
    update_settings(settings_file)