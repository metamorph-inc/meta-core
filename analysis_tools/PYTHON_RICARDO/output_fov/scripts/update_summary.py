import sys
import json
from optparse import OptionParser

def update(output):
    content = json.load(output)
    
    vchp = None
    vcmu = None
    vcacvp = None
    vcfcvp = None
    dhp = None
    dfcvp = None
    dmu = None
    tcacvp = None
    tchp = None
    tcfcvp = None
    tcmu = None
	
    vchp = content["vehicle_commander_horizon_percentage"]
    vcmu = content["vehicle_commander_max_uplook"]
    vcacvp = content["vehicle_commander_aft_closest_visible_point"]
    vcfcvp = content["vehicle_commander_fore_closest_visible_point"]
    dhp = content["driver_horizon_percentage"]
    dfcvp = content["driver_fore_closest_visible_point"]
    dmu = content["driver_max_uplook"]
    tcacvp = content["troop_commander_aft_closest_visible_point"]
    tchp = content["troop_commander_horizon_percentage"]
    tcfcvp = content["troop_commander_fore_closest_visible_point"]
    tcmu = content["troop_commander_max_uplook"]
	
    results = [vchp, vcmu, vcacvp, vcfcvp, dhp, dfcvp, dmu, tcacvp, tchp, tcfcvp, tcmu]

    with open("testbench_manifest.json", "r") as file:
        summary = json.load(file)

        if not any(results) is None:     
            for metric in summary["Metrics"]:
                if metric["Name"] == "VC_Field_of_View_Horizontal":
                    metric["Value"] = str(float(vchp)*100.0)
                    metric["Unit"] = '%'
                if metric["Name"] == "VC_Field_of_View_Up":
                    metric["Value"] = str(vcmu)
                    metric["Unit"] = 'deg'
                if metric["Name"] == "VC_Field_of_View_Ground_aft":
                    metric["Value"] = str(vcacvp)
                    metric["Unit"] = 'm'
                if metric["Name"] == "VC_Field_of_View_Ground_front":
                    metric["Value"] = str(vcfcvp)
                    metric["Unit"] = 'm'
                if metric["Name"] == "Driver_Field_of_View_Horizontal":
                    metric["Value"] = str(float(dhp)*100.0)
                    metric["Unit"] = '%'
                if metric["Name"] == "Driver_Field_of_View_Ground":
                    metric["Value"] = str(dfcvp)
                    metric["Unit"] = 'm'
                if metric["Name"] == "Driver_Field_of_View_Up":
                    metric["Value"] = str(dmu)
                    metric["Unit"] = 'deg'
                if metric["Name"] == "TC_Field_of_View_Ground_aft":
                    metric["Value"] = str(tcacvp)
                    metric["Unit"] = 'm'
                if metric["Name"] == "TC_Field_of_View_Horizontal":
                    metric["Value"] = str(float(tchp)*100.0)
                    metric["Unit"] = '%'
                if metric["Name"] == "TC_Field_of_View_Ground_front":
                    metric["Value"] = str(tcfcvp)
                    metric["Unit"] = 'm'
                if metric["Name"] == "TC_Field_of_View_Up":
                    metric["Value"] = str(tcmu)
                    metric["Unit"] = 'deg'
            summary["AnalysisStatus"] = "OK"
        else:
            summary["AnalysisStatus"] = "Failed"
        
        with open("testbench_manifest.json", "w") as updated:
            json.dump(summary, updated, indent=4)
              
   
if __name__=="__main__":
    with open("output.json", "r") as output:
        update(output)
    