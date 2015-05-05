import sys
import json
from optparse import OptionParser

def update(output):
    content = json.load(output)
    
    min_fore = None
    nfa = None
    min_aft = None
    
    min_fore = content["min_fire_dist_fore_180"]
    nfa = content["no_fire_area"]
    min_aft = content["min_fire_dist_aft_180"]
    
    results = [min_fore, nfa, min_aft]

    with open("testbench_manifest.json", "r") as file:
        summary = json.load(file)

        if not any(results) is None:     
            for metric in summary["Metrics"]:
                if metric["Name"] == "Field_of_Fire_Depression_Forward":
                    metric["Value"] = str(min_fore)
                    metric["Unit"] = 'm'
                if metric["Name"] == "Field_of_Fire_Traverse":
                    metric["Value"] = str(nfa)
                    metric["Unit"] = 'm^2'
                if metric["Name"] == "Field_of_Fire_Depression_Aft":
                    metric["Value"] = str(min_aft)
                    metric["Unit"] = 'm'
            summary["AnalysisStatus"] = "OK"
        else:
            summary["AnalysisStatus"] = "Failed"
        
        with open("testbench_manifest.json", "w") as updated:
            json.dump(summary, updated, indent=4)
              
   
if __name__=="__main__":
    with open("output.json", "r") as output:
        update(output)
    