import sys
import json
from optparse import OptionParser

def update(output):
    with open("testbench_manifest.json", "r") as file:
        summary = json.load(file)
        try:
            with open("output.json", "r") as output:
                out = json.load(output)
                for metric in summary["Metrics"]:
                    if metric["Name"] == "Troop_Ergonomics":
                        metric["Value"] = out["MIL-STD-1472"]["Troop_Ergonomics"]
                    if metric["Name"] == "Driver_Ergonomics":
                        metric["Value"] = out["MIL-STD-1472"]["Driver_Ergonomics"]
                    if metric["Name"] == "Gunner_Ergonomics":
                        metric["Value"] = out["MIL-STD-1472"]["Gunner_Ergonomics"]
                    if metric["Name"] == "Vehicle_Cmdr_Ergonomics":
                        metric["Value"] = out["MIL-STD-1472"]["Vehicle_Cmdr_Ergonomics"]
                    if metric["Name"] == "Troop_Cmdr_Ergonomics":
                        metric["Value"] = out["MIL-STD-1472"]["Troop_Cmdr_Ergonomics"]
                summary["Status"] = "OK"
        except IOError:
            for metric in summary["Metrics"]:
                if metric["Name"] == "Troop_Ergonomics":
                    metric["Value"] = False
                if metric["Name"] == "Driver_Ergonomics":
                    metric["Value"] = out["MIL-STD-1472"]["Driver_Ergonomics"]
                if metric["Name"] == "Gunner_Ergonomics":
                    metric["Value"] = False
                if metric["Name"] == "Vehicle_Cmdr_Ergonomics":
                    metric["Value"] = False
                if metric["Name"] == "Troop_Cmdr_Ergonomics":
                    metric["Value"] = False
                summary["Status"] = "Failed"        
  
    with open("testbench_manifest.json", "w") as updated:
        json.dump(summary, updated, indent=4)
              
   
if __name__=="__main__":
    update('output.json')
            
    