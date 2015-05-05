import sys
import json
from optparse import OptionParser

def update(output):
    content = json.load(output)
    
    tet = None
    
    tet = content["total_exit_time_tb033"]
    litter = content["litter_fits_tb036"]
    evac = content["crew_evac_passed_tb035"]
    egress = content["egress_overview_passed_tb034"]
    
    results = [tet]

    with open("testbench_manifest.json", "r") as file:
        summary = json.load(file)

        if not any(results) is None:     
            for metric in summary["Metrics"]:
                if metric["Name"] == "Embarked_Troop_Deployment":
                    metric["Value"] = str(tet)
                    metric["Unit"] = 'sec'
                if metric["Name"] == "Litter":
                    metric["Value"] = str(litter)
                if metric["Name"] == "Crew_Evacuation":
                    metric["Value"] = str(evac)
                if metric["Name"] == "Emergency_Egress":
                    metric["Value"] = str(egress)
            summary["AnalysisStatus"] = "OK"
        else:
            summary["AnalysisStatus"] = "Failed"
        
        with open("testbench_manifest.json", "w") as updated:
            json.dump(summary, updated, indent=4)
              
   
if __name__=="__main__":
    with open("output.json", "r") as output:
        update(output)