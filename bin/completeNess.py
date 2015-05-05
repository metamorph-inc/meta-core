
import sys
import os
#sys.path.append(r"C:\Program Files\ISIS\Udm\bin")
#if os.environ.has_key("UDM_PATH"):
#    sys.path.append(os.path.join(os.environ["UDM_PATH"], "bin"))
import _winreg as winreg
with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
    meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")
sys.path.append(os.path.join(meta_path, 'bin'))
import udm

import inspect

def log(s):
    print s
try:
    import CyPhyPython # will fail if not running under CyPhyPython
    import cgi
    def log(s):
        CyPhyPython.log(cgi.escape(s))
except ImportError:
    pass

def log_formatted(s):
    print s
try:
    import CyPhyPython # will fail if not running under CyPhyPython
    import cgi
    def log(s):
        CyPhyPython.log(s)
except ImportError:
    pass

	
def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False	
	
	
def start_pdb():
    ''' Starts pdb, the Python debugger, in a console window
    '''
    import ctypes
    ctypes.windll.kernel32.AllocConsole()
    import sys
    sys.stdout = open('CONOUT$', 'wt')
    sys.stdin = open('CONIN$', 'rt')
    import pdb; pdb.set_trace()

class ComputeClassificationCounts(object):
    def compute(self, currentobj, output_dir):
        json_filename = os.path.join(output_dir, 'summary.testresults.json')
        classifications = {}
        tb_metrics = [child for child in currentobj.children() if child.type.name=="Metric"]
        sut = [child for child in currentobj.children() if child.type.name=="TopLevelSystemUnderTest"]
        if len(sut) == 0:
            raise Exception(currentobj.name + ' has no TopLevelSystemUnderTest')
        sut = sut[0]
        import collections
        que = collections.deque()
        que.extend(sut.ref.children())
        #que.appendleft(sut.ref) ## added to allow top level to be scanned
        # get tables of interface pairs [component_name,structure_port,power_port
        import csv
        pairing = []
        table_filename = os.path.join(output_dir, "../../Post_Processing/upd_completeness_table.csv")        
        log("Table filename = "+table_filename)
        with open(table_filename,'rb') as csvfile:
        #with open("c:/temp/upd_completeness_table.csv",'rb') as csvfile:
            freader = csv.reader(csvfile,delimiter=',',quotechar='|')
            for row in freader:
                pairing.append(row)
                #log(str(len(row))+" "+row[0])
                #if len(row) > 2:
                #    log("TABLE FILL....Component: "+row[0]+"    SI:"+row[1]+"   PI:"+row[2])

        
        aggregated_mass = float(0)
        totPwr = 0
        totStr = 0

        numStructIntfs = 0
        numPwrIntfs = 0     # total number of power interfaces in all component instances used
        
        unmatched = 0
        matched = 0
        ignoredPorts = 0
        numAdapters = 0
        numPositionalInterfaces = 0
        while que:
            obj = que.pop()
            if obj.type.name == "ComponentAssembly":
                log("<<<<<<<<<<<<<<<<<<<< ComonentAssembly: >>>>>>>>>>>>>>>>" + obj.name)		
                kidz = obj.children()
                pwrPorts = []
                strPorts = []
                ppConns = []
                jsConns = []		
                asyPwr = 0
                asyStr = 0
                # Kevin Suggests
                # k.srcAggregatePortComposition and k.srcAggregatePortComposition_refport_parent
                for k in kidz:
                    if k.type.name == "AggregatePortComposition":
                        ppItem = [k.srcAggregatePortComposition.parent.name ,k.srcAggregatePortComposition.name]
                        ppConns.append(ppItem)
                        #log("   ...src.powerConn..: ["+ppItem[0]+" ,"+ppItem[1]+"]")
                        ppItem = [k.dstAggregatePortComposition.parent.name ,k.dstAggregatePortComposition.name]
                        ppConns.append(ppItem)
                        asyPwr = asyPwr + 2
                        #log("   ...dst.powerConn..: ["+ppItem[0]+" ,"+ppItem[1]+"]")
                    if k.type.name == "PowerFlow":
                        ppItem = [k.srcPowerFlow.parent.name ,k.srcPowerFlow.name]
                        ppConns.append(ppItem)
                        log("   ...src.POWERFLOW powerConn..: ["+ppItem[0]+" ,"+ppItem[1]+"]")
                        ppItem = [k.dstPowerFlow.parent.name ,k.dstPowerFlow.name]
                        ppConns.append(ppItem)
                        asyPwr = asyPwr + 2
                        log("   ...dst.POWERFLOW.powerConn..: ["+ppItem[0]+" ,"+ppItem[1]+"]")
                    if k.type.name == "JoinStructures":
                        #log("k.ype.name="+k.type.name)
							
                        jsItem = [k.srcJoinStructures.parent.name ,k.srcJoinStructures.name]
                        jsConns.append(jsItem)
                        log("   ...src.JoinStructConn..: ["+jsItem[0]+" ,"+jsItem[1]+"]")
                        jsItem = [k.dstJoinStructures.parent.name ,k.dstJoinStructures.name]
                        jsConns.append(jsItem)
                        asyStr = asyStr + 2
                        # log("   ...dst.JoinStructConn..: ["+jsItem[0]+" ,"+jsItem[1]+"]")  ## Debugging
                        if jsItem[1].find("PositionalInterface") != -1:
                            log("Found Positional Interface:"+jsItem[1])
                            numPositionalInterfaces = numPositionalInterfaces + 1
                        #if(k.srcJoinStructures_refport_parent):						
                        #    log("   ....srcJoinStructures_refport_parent "+k.srcJoinStructures_refport_parent.name) # The part name

                log("For Assembly: "+obj.name+", There are: "+str(asyPwr)+"PowerPorts  and: "+str(asyStr)+" StructuralPorts")
                for pp in ppConns:
                    found = 0
                    targetSI = "INITVAL"
                    for tt in pairing:
                        #log("targSrc: "+pp[0]+".find("+tt[0]+")")
                        if(pp[0].lower().find(tt[0].lower()) == 0) and tt[2] == pp[1]:
                            targetSI = tt[1]
                            #log(" FOUND!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TargetSI is:"+targetSI)
                    log("Looking for:"+pp[0]+" / "+pp[1]+"  -->"+targetSI)
                    if targetSI == "INITVAL":
                        ignoredPorts = ignoredPorts + 1
                        targetSI = "N/A"
                        log("CPTERR: Ignoring Component (Not Found:"+pp[0]+":  port:"+pp[1])
                    if targetSI.lower() == "none":
                        targetSI = "N/A"
                    if targetSI != "N/A":
                        for ss in jsConns:
                            #if pp[0].lower().find(ss[0].lower())== 0:
                                #log("First matches")  
                                #log(" CAN HAZ CONNECTION: |"+pp[0].lower()+"=="+ss[0].lower()+"|  and |"+targetSI.lower()+"=="+ss[1].lower()+"|")                              
                            #if targetSI.lower().find(ss[1].lower()) == 0:
                                #log("Second Matches!!!!")

                            if pp[0].lower().find(ss[0].lower()) == 0 and targetSI.lower().find(ss[1].lower()) == 0:
                                found = 1
                                matched = matched + 1
                                log("+++++  Complete Connection: "+pp[0]+"  Pair:"+pp[1]+"  and:"+ss[1])
                        if found == 0:
                            log("------ Incomplete Connection: "+pp[0]+"  Pair:"+pp[1]+"  should match:"+targetSI)
                            unmatched = unmatched+1
						
                totStr = totStr + asyStr
                totPwr = totPwr + asyPwr


            if obj.type.name == "ComponentRef":
                obj = obj.ref
            que.extend(obj.children())
            def add_classification(component):
                for class_ in (s for s in component.Classifications.replace("\r", "").split("\n") if not s.isspace() and s != ""):
                    classifications.setdefault(class_, [])
                    classifications[class_].append(component)
            if obj.type.name == "Component":
                add_classification(obj)
                if obj.name.find("Adapter") == 0:
                    log("------ Found ADAPTER: "+obj.name+":")
                    numAdapters = numAdapters + 1
                masses = [child for child in obj.children() if (child.type.name=="Property" or child.type.name=="Parameter") and child.name == "mass"]
                # what if more found?
                if len(masses) > 0:
                    #log(masses[0].Value)
                    #if masses[0].Value.isdigit():
                    if is_number(masses[0].Value):
                        aggregated_mass += float(masses[0].Value)
                    else:
                        log("Warning. No/Invalid Mass for Component:"+obj.name)						
                structIntfs = [child for child in obj.children() if child.type.name=="StructuralInterface"]
                # 
                log("StructuralIntefaces: "+str(len(structIntfs))+"  and masses:"+str(len(masses)))
                if len(structIntfs) > 0:
                    numStructIntfs = numStructIntfs + len(structIntfs)
                    dog = "Component:"+obj.name+" SI="
                    for ssii in structIntfs:
                        dog = dog +"["+ssii.name+"]"
                    log(dog)

                pwrIntfs = [child for child in obj.children() if (child.type.name=="Hydraulic Power Port") or (child.type.name=="Thermal Power Port") or (child.type.name=="Electrical Power Port")  or (child.type.name=="Translational Power Port") or (child.type.name=="AggregatePort") or (child.type.name=="MultibodyFramePowerPort")or (child.type.name=="Rotational Power Port")or (child.type.name=="Translational Power Port")]
                #pwrIntfs = [child for child in obj.children() if (child.type.name=="Hydraulic Power Port") or  (child.type.name=="Electrical Power Port")  or (child.type.name=="AggregatePort")]
                # Electrical Power Port,Hydraulic Power Port, AggregatePort, Bus Port, MultibodyFramePowerPort,Rotational Power Port,Translational Power Port
                # , , Bus Port, ,,
                log("PowerInterfaces: "+str(len(pwrIntfs)))
                if len(pwrIntfs) > 0:
                    numPwrIntfs = numPwrIntfs + len(pwrIntfs)
                    dog = "Component:"+obj.name+" Pwr="
                    for ssii in pwrIntfs:
                        dog = dog +"["+ssii.name+"]"
                    log(dog)                    

        
        # Dump Stats
        log("COMPLETENESS Matched Ports:   "+str(matched))
        log("COMPLETENESS unMatched Ports: "+str(unmatched))
        log("COMPLETENESS Number of Adapters:        "+str(numAdapters))
        log("COMPLETENESS Number of Positional Intf: "+str(numPositionalInterfaces))
        log("COMPLETENESS Ignored Ports(Test Comp, Design Containers, N/A or incomplete/incorrect Port Data:  "+str(ignoredPorts))
        log("COMPLETENESS Total Connected Struct/Power Ports:  "+str(totStr)+"/"+str(totPwr))
        log("COMPLETENESS Total Struct Ports Available In All Components:  "+str(numStructIntfs)+",  Diff(Avail-Used)="+str(numStructIntfs - totStr))
        log("COMPLETENESS Total Power  Ports Available In All Components:  "+str(numPwrIntfs)+",  Diff(Avail-Used)="+str(numPwrIntfs - totPwr))
        x1 = float(matched)*100.00/float(totPwr-ignoredPorts)
        x2 = float(unmatched)*20.0/float(totPwr-ignoredPorts)
        x3 = float(numAdapters )*10.0 /float(totPwr-ignoredPorts)
        x4 = float(totStr) * 88.0 / float(numStructIntfs)
        log("(matched/totPwr * 100):=" + str(x1))
        log("(unmatched/totPwr * 20)="+str(x2))
        log("(numAdapters/totPwr * 10)= "+str(x3))
        log("(totStr / numStructIntfs * 88)="+str(x4))
        megaCompletenessScore = (x1) - (x2) - (x3) + (x4)
        log("EarlyAlgorithmCOMPLETENESS Score:  "+str(megaCompletenessScore))
        if(megaCompletenessScore < 0.0):
            megaCompletenessScore = 0.0;
        Aa = (float(matched) + float(numAdapters) * 0.5) / float(totPwr)
        Bb = float(totStr) / float(numStructIntfs)
        luskinCompleteness = (Aa + Bb) / 2.0
        log("COMPLETENESS Score:  "+str(luskinCompleteness))
        
        import json
        json_data = {}
        if os.path.isfile(json_filename):
            with open(json_filename, "r") as json_file:
                json_data = json.load(json_file)
        counts = dict(((class_, len(members)) for class_, members in classifications.iteritems()))
        # must conform to MetaTBReport.cs
        #json_data.setdefault('Metrics', []).append(
        #    {
        #    "Name": "Classification_counts",
            # "ID": "a9986fb8-b319-4f71-ba57-bcxx",
            # "DisplayedName": null,
            # "Unit": "",
        #    "Value": json.dumps(counts)
        #    })
        json_data.setdefault('Metrics', [])
        for item in json_data['Metrics']:
            if isinstance(item, dict) and item.get("ID") == "d408d8c0-fcd0-426a-be4a-c60d1ad9ff4f":
                json_data['Metrics'].remove(item)
        human_models = 0
        for category, count in counts.iteritems():
            if category.startswith("AVM.Ontology.") and category.endswith(".Category.Human_Models"):
                human_models += count
        # META-554
        human_models = human_models - 3
        if human_models < 0:
            human_models = 0 
        # json_data['Metrics'].append(
        # {
            # "Name": "Troop_Capacity", 
            # "DisplayedName": "Troop_Capacity",
            # "Value": human_models,
            # "ID": "d408d8c0-fcd0-426a-be4a-c60d1ad9ff4f",
            # "Unit": ""
        # })
        
        was_appended = False
        log(str(json_data))
        # only master interpreter
        for metric in tb_metrics: 
            if metric.name == "VehicleMass":
                metric.Value = str(aggregated_mass)
                was_appended = True
                break
                
        # for SoT
        for metric in json_data['Metrics']: 
            if metric["Name"] == "VehicleMass":
                metric["Value"] = str(aggregated_mass)
                was_appended = True
                break
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "VehicleMass",
                "DisplayedName": "VehicleMass",
                "Value": aggregated_mass,
                "ID": "d408d8c0-fcd0-426a-be4a-c60d1ad9ff4f",
                "Unit": "kg"
            })
        ############  Completeness Metrics       
  
        for metric in json_data['Metrics']: 
            if metric["Name"] == "MatchedPorts":
                metric["Value"] = str(matched)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "MatchedPorts",
                "DisplayedName": "MatchingPowerAndStructural",
                "Value": matched,
                "ID": "matched-matched-matched",
                "Unit": ""
            })
        for metric in json_data['Metrics']: 
            if metric["Name"] == "UnMatchedPorts":
                metric["Value"] = str(unmatched)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "UnMatchedPorts",
                "DisplayedName": "UnMatchedPowerAndStructural",
                "Value": unmatched,
                "ID": "unmatched-unmatched-unmatched",
                "Unit": ""
            })
        ########################################
        for metric in json_data['Metrics']: 
            if metric["Name"] == "AdaptersUsed":
                metric["Value"] = str(numAdapters)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "AdaptersUsed",
                "DisplayedName": "AdaptersUsedInDesign",
                "Value": numAdapters,
                "ID": "adapters-adapters-adapters",
                "Unit": ""
            })
        ########################################
        for metric in json_data['Metrics']: 
            if metric["Name"] == "PositionalInterfacesUsed":
                metric["Value"] = str(numPositionalInterfaces)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "PositionalInterfacesUsed",
                "DisplayedName": "PositionalInterfacesUsed",
                "Value": numPositionalInterfaces,
                "ID": "posintf-posintf",
                "Unit": ""
            })
         ########################################
        for metric in json_data['Metrics']: 
            if metric["Name"] == "StructuralInterfacesUsed":
                metric["Value"] = str(totStr)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "StructuralInterfacesUsed",
                "DisplayedName": "StructuralInterfacesUsed",
                "Value": totStr,
                "ID": "usedStructural-usedStructural",
                "Unit": ""
            })
         ########################################
        for metric in json_data['Metrics']: 
            if metric["Name"] == "PowerInterfacesUsed":
                metric["Value"] = str(totPwr)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "PowerInterfacesUsed",
                "DisplayedName": "PowerInterfacesUsed",
                "Value": totPwr,
                "ID": "usedPower-usedPower",
                "Unit": ""
            })            
        ########################################
        for metric in json_data['Metrics']: 
            if metric["Name"] == "StructuralInterfacesAvailable":
                metric["Value"] = str(numStructIntfs)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "StructuralInterfacesAvailable",
                "DisplayedName": "StructuralInterfacesInAllComponents",
                "Value": numStructIntfs,
                "ID": "AvailableStructural-AvailableStructural",
                "Unit": ""
            })
        ########################################
        for metric in json_data['Metrics']: 
            if metric["Name"] == "PowerInterfacesAvailable":
                metric["Value"] = str(numPwrIntfs)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "PowerInterfacesAvailable",
                "DisplayedName": "PowerInterfacesInAllComponents",
                "Value": numPwrIntfs,
                "ID": "AvailablePower-AvailablePower",
                "Unit": ""
            })

            ########################################
        for metric in json_data['Metrics']: 
            if metric["Name"] == "CompletenessMetric":
                metric["Value"] = str(luskinCompleteness)
                was_appended = True
                break
                
        if not was_appended:
            json_data['Metrics'].append(
            {
                "Name": "CompletenessMetric",
                "DisplayedName": "CompletenessMetric",
                "Value": luskinCompleteness,
                "ID": "completeness-completeness",
                "Unit": "Fromanz"
            })

 
        log('Aggregated mass: %d' % aggregated_mass)
        log("Total Power/Struct Ports = "+str(totPwr)+"/"+str(totStr))
        with open(json_filename, "w") as json_file:
            json.dump(json_data, json_file, indent=4)

        stat_json_path = os.path.join(output_dir, 'stat.json')
        if os.path.isfile(stat_json_path):
            with open(stat_json_path, 'r') as stat_file:
                stat_json = json.load(stat_file)
        else:
            stat_json = {}
        stat_json.setdefault('ToolSpecifics', {})
        stat_json['ToolSpecifics']['classifications'] = counts
        with open(stat_json_path, 'w') as stat_file:
            json.dump(stat_json, stat_file, indent=4)


# This is the entry point    
def invoke(focusObject, rootObject, componentParameters, **kwargs):
    log(rootObject.name)
    print repr(rootObject.name)
    output_dir = componentParameters['output_dir']
    if output_dir == '':
        output_dir = os.getcwd()
    ComputeClassificationCounts().compute(focusObject, output_dir)
    #log_formatted("Output files are <a href=\"file:///{0}\" target=\"_blank\">{0}</a>.".format(output_dir))
    log("Output files are <a href=\"file:///{0}\" target=\"_blank\">{0}</a>.".format(output_dir))
    log('done')
    with open(os.path.join(output_dir, 'run_classifications.cmd'), 'w'): pass
    componentParameters['runCommand'] = 'run_classifications.cmd'

# Allow calling this script with a .mga file as an argument    
if __name__=='__main__':
    import _winreg as winreg
    with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
        meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")

    # need to open meta DN since it isn't compiled in
    uml_diagram = udm.uml_diagram()
    meta_dn = udm.SmartDataNetwork(uml_diagram)
    import os.path
    CyPhyML_udm = os.path.join(meta_path, r"generated\CyPhyML\models\CyPhyML_udm.xml")
    if not os.path.isfile(CyPhyML_udm):
        CyPhyML_udm = os.path.join(meta_path, r"meta\CyPhyML_udm.xml")
    meta_dn.open(CyPhyML_udm, "")

    dn = udm.SmartDataNetwork(meta_dn.root)
    dn.open(sys.argv[1], "")
    # TODO: what should focusObject be
    # invoke(None, dn.root);
    dn.close_no_update()
    meta_dn.close_no_update()
