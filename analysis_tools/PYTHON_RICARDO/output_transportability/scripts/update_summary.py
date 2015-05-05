import sys
import json
from optparse import OptionParser

def update(output):
    content = json.load(output)
    
    c17_air = None
    highway_road = None
    rail = None
    well_deck_water = None
    mpf_water = None
    c5_air = None
    road = None
    aft_approach_angle = None
    ground_clearance = None
    front_approach_angle = None
    lifting_metrics = None
    tie_down_metrics = None
    mooring = None
    tow = None
    lift = None
    pintles = None
    
    c17_air = content["Transportation_Compatibility"]["C17_Air"]
    highway_road = content["Transportation_Compatibility"]["Highway_Road"]
    rail = content["Transportation_Compatibility"]["Rail"]
    well_deck_water = content["Transportation_Compatibility"]["Well_Deck_Water"]
    mpf_water = content["Transportation_Compatibility"]["MPF_Water"]
    c5_air = content["Transportation_Compatibility"]["C5_Air"]
    road = content["Transportation_Compatibility"]["Road"]
    aft_approach_angle = content["Approach_Angles"].get("Aft_approach_angle_[deg]",0)
    ground_clearance = content["Approach_Angles"].get("Ground_clearance_[m]",0)
    front_approach_angle = content["Approach_Angles"].get("Front_approach_angle_[deg]",0)
    tow = content["Count_for_components_in_towing_class"]
    lift = content["Count_for_components_in_lifting_class"]
    pintles = content["Count_for_components_in_pintle_class"]
    mooring = content["Count_for_components_in_mooring_class"]
    if all(val=='Pass' for val in content["Tie_Down_Metrics"].values()):
        tie_down_metrics = True
    else:
        tie_down_metrics = False
    if all(val=='Pass' for val in content["Lifting_Metrics"].values()):
        lifting_metrics = True
    else:
        lifting_metrics = False
    if all([c17_air, c5_air]):
        air = True
    else:
        air = False
	
    results = [c17_air, highway_road,
                rail, well_deck_water, mpf_water, c5_air, road, aft_approach_angle,
                ground_clearance, front_approach_angle, tie_down_metrics, lifting_metrics,
                air, mooring, tow, lift, pintles]

    with open("testbench_manifest.json", "r") as file:
        summary = json.load(file)

        if not any(results) is None:     
            for metric in summary["Metrics"]:
                if metric["Name"] == "Air_Transportation_Compatibility":
                    metric["Value"] = str(air)
                if metric["Name"] == "Highway_Road_Transportation_Standards":
                    metric["Value"] = str(highway_road)
                if metric["Name"] == "Rail_Transportation_Compatibility":
                    metric["Value"] = str(rail)
                if metric["Name"] == "Well_Deck_Water_Transportation_Compatibility":
                    metric["Value"] = str(well_deck_water)
                if metric["Name"] == "MPF_Water_Transportation_Compatibility":
                    metric["Value"] = str(mpf_water)
                if metric["Name"] == "Road_Transportation_Standard":
                    metric["Value"] = str(road)
                if metric["Name"] == "Lift_Eyes":
                    metric["Value"] = str(lifting_metrics)
                if metric["Name"] == "Tow_Eyes":
                    metric["Value"] = str(tie_down_metrics)
                if metric["Name"] == "Departure_Angle":
                    metric["Value"] = str(aft_approach_angle)
                    metric["Unit"] = 'deg'
                if metric["Name"] == "Ground_Clearance":
                    metric["Value"] = str(ground_clearance)
                    metric["Unit"] = 'm'
                if metric["Name"] == "Approach_Angle":
                    metric["Value"] = str(front_approach_angle)
                    metric["Unit"] = 'deg'
                if metric["Name"] == "Mooring_Bits":
                    metric["Value"] = str(mooring)
                if metric["Name"] == "Tow_Provisions":
                    metric["Value"] = str(tow)
                if metric["Name"] == "Lift_Provisions":
                    metric["Value"] = str(lift)
                if metric["Name"] == "Tow_Pintles":
                    metric["Value"] = str(pintles)
            summary["AnalysisStatus"] = "OK"
        else:
            summary["AnalysisStatus"] = "Failed"
        
        with open("testbench_manifest.json", "w") as updated:
            json.dump(summary, updated, indent=4)
              
   
if __name__=="__main__":
    with open("output.json", "r") as output:
        update(output)
    