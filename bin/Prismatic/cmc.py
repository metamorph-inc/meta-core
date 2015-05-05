#!/usr/bin/python
# cmc - CyPhy Module Composer
# Convert a CyPhy model into a PRISMATIC model.

# TODO:
# - Deal with bond graph dynamics.
# - Deal with edges of system.

# COPYRIGHT START
# Copyright (c) 2011, Vanderbilt University and Smart Information Flow Technologies (SIFT).
# Developed with the sponsorship of the Defense Advanced Research Projects Agency (DARPA).
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this data, including any software or models in source or binary form, as well
# as any drawings, specifications, and documentation (collectively "the Data"),
# to deal in the Data without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Data, and to permit persons to whom the Data is furnished to
# do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Data.

# THE DATA IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS,
# SPONSORS, DEVELOPERS, CONTRIBUTORS, OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE DATA OR THE USE OR
# OTHER DEALINGS IN THE DATA.
# COPYRIGHT END

import json
import os
import pprint
import re
import stat
import sys
import time
import types
from xml.dom.minidom import parseString
from xml.dom import Node

def parse_xml(xml):
    model = {}
    components={}
    dom = parseString(xml)
    need_inputs = set()
    # A <prismatic> node contains a component model in the XML text.
    for prismatic in dom.getElementsByTagName('PrismaticCode'):
        parent = prismatic.parentNode
        container = parent.getAttribute('name')
        code = prismatic.getAttribute('Source').decode('string_escape')
        # sys.stderr.write(code)
        if container not in components.keys():
            components[container] = {}
            components[container]['prismatic'] = {}
        components[container]['prismatic']['code'] = code
        components[container]['prismatic']['name'] = container
        need_inputs.add(parent)

    # A <mode> node contains the (name, condition, probability,
    # behavior) representation we came up with, as attributes.
    for mode in dom.getElementsByTagName('Mode'):
        parent = mode.parentNode.parentNode; # Skip over <modes> tag too....
        container = parent.getAttribute('name')
        name = mode.getAttribute('name')
        if container not in components.keys():
            components[container] = {}
        if 'modes' not in components[container].keys():
            components[container]['modes'] = {}
        if name not in components[container]['modes'].keys():
            components[container]['modes'][name] = {}
        try:
            probability = float(mode.getAttribute('Probability'))
        except ValueError:
            probability = None
        components[container]['modes'][name]['condition'] = mode.getAttribute('Condition')
        components[container]['modes'][name]['probability'] = probability
        components[container]['modes'][name]['behavior'] = mode.getAttribute('Behavior')
        # sys.stderr.write("Need inputs for %s %s %s\n" % (parent.getAttribute('name'), parent.getAttribute('_id'), parent.nodeName))
        need_inputs.add(parent)

    # Gather inputs to this model...slightly hairy. Start with
    # siblings of the prismatic node, which may be input connections.
    for parent in need_inputs:
        container = parent.getAttribute('name')
        # sys.stderr.write("Connections for %s\n" % (container));
        for sibling in parent.childNodes:
            type = sibling.nodeName
            if type not in CONNECTION_TYPES:
                # if type == '#text':
                #     continue
                # sys.stderr.write("  %s(%s) not CON_TYPE\n" % (sibling.getAttribute('name'), type));
                continue
            # sys.stderr.write("  %s(%s) %s is CON_TYPE\n" % (sibling.getAttribute('name'), type, sibling.getAttribute('_id')));
            source = get_source(sibling, dom)
            if not (source and source.parentNode != sibling.parentNode):
                # if not source:
                #     sys.stderr.write("    aborting: no source\n")
                # else:
                #     sys.stderr.write("    aborting: no parent node\n")
                continue
            source_type = source.nodeName
            # sys.stderr.write("    Source: %s(%s) %s\n" % (source.getAttribute('name'), source_type, source.getAttribute('_id')));
            # If this sibling is an input connection, its source has a different parent.
            # Iteratively trace the source back until we have something that
            # is not an internal connection type.
            while source_type in INTERNAL_CONNECTION_TYPES:
                source = get_source(source, dom)
                source_type = source.nodeName
            if 'inputs' not in components[container].keys():
                components[container]['inputs'] = {}
            components[container]['inputs'][sibling.getAttribute('name')] = source.parentNode.getAttribute('name'), source.getAttribute('name')
            # sys.stderr.write("  FOUND: %s %s (%s)\n" % (source.parentNode.getAttribute('name'), source.getAttribute('name'), source.nodeName))


    model['components'] = components
    model['properties'] = {}
    for prop in dom.getElementsByTagName('ProbabilisticCTL_Requirement'):
        description = prop.getAttribute('Description')
        model['properties'][description] = []
        for condition in prop.childNodes:
            if condition.nodeName != 'Condition':
                continue
            pctl = condition.getAttribute('PCTL')
            model['properties'][description]
            model['properties'][description].append(pctl);

    return model

def parse_xml_hardcoded(xml):
    return { u'EngTransDriveTrain': { u'inputs': { u'TransmissionOut': [ u'TransmissionGeneric',
                                                                         u'OutRot']}},
             u'TransmissionControllerFcn': { u'inputs': { u'GearRatio': [ u'TransmissionControllerModel',
                                                                          u'Ratio'],
                                                          u'TCTopInRPM': [ u'TransmissionGeneric',
                                                                           u'InRPM'],
                                                          u'TCTopOutRPM': [ u'TransmissionGeneric',
                                                                            u'OutRPM']}},
             u'TransmissionControllerModel': { u'inputs': { u'InRPM': [ u'TransmissionControllerFcn',
                                                                        u'TCTopInRPM'],
                                                            u'OutRPM': [ u'TransmissionControllerFcn',
                                                                         u'TCTopOutRPM']},
                                               u'modes': { u'nominal': { u'behavior': u'Ratio =\n\tInRPM <= 1800 * 0.235849057 ? 0.235849057 :\n\tInRPM <= 1800 * 0.327868852 ? 0.327868852 :\n\tInRPM <= 1800 * 0.431034483 ? 0.431034483 :\n\tInRPM <= 1800 * 0.598802395 ? 0.598802395 :\n\tInRPM <= 1800 * 1 ? 1 :\n\tInRPM <= 1800 * 1.388888889 ? 1.388888889 :\n\t1',
                                                                         u'condition': None,
                                                                         u'probability': None}}},
             u'TransmissionGeneric': { u'inputs': { u'InGearRatio': [ u'TransmissionControllerFcn',
                                                                      u'GearRatio'],
                                                    u'InRot': 5},
                                       u'modes': { u'failed': { u'behavior': u'OutRPM = 0',
                                                                u'condition': u'mode = mode_nominal & InRot < 5000',
                                                                u'probability': 1e-05},
                                                   u'nominal': { u'behavior': u'OutRot = InRot * InGearRatio; OutRPM = InRot * InGearRatio; InRPM = InRot;',
                                                                 u'condition': None,
                                                                 u'probability': None},
                                                   u'overheated': { u'behavior': u'OutRPM = 0',
                                                                    u'condition': u'mode = mode_nominal & InRot > 5000',
                                                                    u'probability': 1e-05}}}}

def print_prism_header(source_filename):
    print time.strftime("// This file was auto-generated by " + sys.argv[0] + " from " + source_filename + " on %m/%d/%Y at %H:%M:%S.\n", time.localtime())
    print '''
// COPYRIGHT START
// Copyright (c) 2011, Vanderbilt University and Smart Information Flow Technologies (SIFT).
// Developed with the sponsorship of the Defense Advanced Research Projects Agency (DARPA).
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this data, including any software or models in source or binary form, as well
// as any drawings, specifications, and documentation (collectively "the Data"),
// to deal in the Data without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Data, and to permit persons to whom the Data is furnished to
// do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Data.
//
// THE DATA IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS,
// SPONSORS, DEVELOPERS, CONTRIBUTORS, OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE DATA OR THE USE OR
// OTHER DEALINGS IN THE DATA.
// COPYRIGHT END

dtmc

'''

def print_component_header(component_name, component):
  line = "// Component: " + component_name + " //"
  print "/" * len(line)
  print line
  print "/" * len(line)


def parse_behavior(component_name, behavior):
    parsed = {}
    eqs = behavior.split(';')
    for equation in eqs:
        if equation == '':
            continue
        # Namespace identifiers with component name.
        equation = re.sub(r"[a-zA-Z]\w*", component_name + "_\g<0>", equation)
        # sys.stderr.write(equation)
        lhs,rhs = re.split(r"\s*[<>]?=\s*", equation, 1)
        s = re.split(r"\s*[<>]?=\s*", equation, 1)
        parsed[lhs] = rhs;
    return parsed


def print_output_formulas(component_name, component):
    # Collect behaviors in the form $behaviors->{mode}->{formula} = equation.
    behaviors = {}
    if 'modes' not in component.keys():
        return
    for mode_name,mode in component['modes'].iteritems():
        behaviors[mode_name] = parse_behavior(component_name, mode['behavior'])
    # Here we have a structure like mode->formula_name->equation. Need
    # to turn this into:
    # formula_name = (mode = X) ? eqX : (mode = Y) ? eqY : eqNOMINAL;
    for behavior in behaviors['nominal']:
        print "formula " + behavior + " ="
        for name in behaviors.keys():
            if name == 'nominal':
                continue
            if behavior in behaviors[name]:
                print "\t" + component_name + "_mode = " + component_name + "_mode_" + name + " ? " + behaviors[name][behavior] + " :"
        print "\t" + behaviors['nominal'][behavior] + ";"
        print ""
    print ""

# Namespace and print the embedded PRISMATIC model.
def print_prismatic(component_name, component):
    code = component['prismatic']['code']
    short_name = component['prismatic']['name']
    code = re.sub(short_name, component_name, code)
    print code


# Print the constants for each mode, ensuring 0 is nominal (our convention).
def print_mode_constants(component_name, component):
    # By convention, nominal mode is represented by 0.
    print "const int " + component_name + "_mode_nominal = 0;"
    mode_num = 1
    for mode in component['modes']:
        if mode == 'nominal':
            continue
        print "const int " + component_name + "_mode_" + mode + " = " + str(mode_num) + ";"
        mode_num += 1
    print ""


# Print the module section of the PRISM model. Contains the variable
# declarations and transitions.
def print_module(component_name, component):
    mode_var = component_name + "_mode"
    num_modes = len(component['modes'])
    print "module " + component_name
    print "\t" + mode_var + ": [0.. " + str(num_modes) + "] init " + component_name + "_mode_nominal;"

    # Collect the transitions structured by condition:
    # $transitions = {
    #   condition -> {dest1 -> prob},
    #   condition -> {dest2 -> prob},
    # }
    # to make it easy to spit out transitions.
    transitions = {}
    for mode_name,mode in component['modes'].iteritems():
        if 'condition' not in mode.keys() or not mode['condition']:
            continue
        if mode['condition'] not in transitions.keys():
            transitions[mode['condition']] = {}
        transitions[mode['condition']][mode_name] = mode['probability']
        #$transitions->{$mode->{condition}}->{$mode_name} = $mode->{probability};

    # Actually print the transitions.
    for condition,out in transitions.iteritems():
        # Namespace identifiers with component name.
        condition = re.sub(r"[a-zA-Z]\w*", component_name + "_\g<0>", condition)
        print "\t[] " + condition + " ->"
        stay_prob = 1 - sum(out.values())
        if stay_prob < 0:
            sys.stderr.write("Transitions on '$condition' sum to greater than 1\n")
        transition_lines = ()
        for dest,prob in out.iteritems():
            dest_mode = component_name + "_mode_" + dest
            transition_lines = transition_lines + ("\t\t" + str(prob) + " : (" + mode_var + "' = " + dest_mode + ")",)
        if stay_prob > 0:
            transition_lines = transition_lines + ("\t\t" + str(stay_prob) + " : true",)
        print " +\n".join(transition_lines) + ";"

    print "endmodule // " + component_name
    print ""


# Do breadth-first search, looking for element by ID.
# TODO: Keep a cache if too inefficient.
def get_element_by_id(id, dom):
    candidates = [dom.documentElement]
    while (candidates):
        candidate = candidates.pop()
        if candidate.nodeType != Node.ELEMENT_NODE:
            continue
        if candidate.hasAttribute('_id') and id == candidate.getAttribute('_id'):
            return candidate
        candidates.extend(candidate.childNodes)
    return None

def get_source(element, dom):
    # mysrcs = [x for (x,y) in element.attributes.items() if x in SRC_TYPES]
    # if len(mysrcs) > 1:
    #     sys.stderr.write('+++  Sources: ' + ', '.join(mysrcs) + "\n")
    for name,value in element.attributes.items():
        # /^src/ is the convention for source of a connection.
        if name in SRC_TYPES:
            srcids = value.split()
            # if len(srcids) > 1:
            #     sys.stderr.write("!!! Multiple srcs (%s)!!!\n" % (len(srcids)))
            #     for s in srcids:
            #         ss = get_element_by_id(s, dom)
            #         sys.stderr.write("  %s: %s\n" % (s, ss.getAttribute('name')))
            for srcid in srcids:
                srcid = srcids[0]
                # This finds all nodes with attribute '_id' equal to the source id.
                src = get_element_by_id(srcid, dom)
                return src;
        # else:
        #     sys.stderr.write("    %s is not src\n" % (name));


def print_connections(model):
    line = "// Connections //"
    print "/" * len(line)
    print line
    print "/" * len(line)

    for component_name,component in model['components'].iteritems():
        if 'inputs' not in component.keys():
            continue
        for dst,src in component['inputs'].iteritems():
            if isinstance(src, types.TupleType):
                src = '_'.join(src)
            print "formula " + component_name + "_" + dst + " = " + str(src) + ";"

def print_properties(model):
    for description,pctl in model['properties'].iteritems():
        print "// " + description
        print "\n".join(pctl)
        print ''

def print_prismatic_script(infile, model):
    print '#!/usr/bin/python'
    print 'import json'
    print 'import os'
    print 'import re'
    print 'import subprocess'
    print 'import tempfile'
    print ''
    print 'model = """'
    print_prism_header(infile);
    
    for component_name,component in model['components'].iteritems():
        print_component_header(component_name, component)
        print_output_formulas(component_name, component)
        if 'prismatic' in component.keys():
            print_prismatic(component_name, component)
        elif 'modes' in component.keys() and 'nominal' in component['modes'].keys():
            print_mode_constants(component_name, component)
            print_module(component_name, component)
        else:
            sys.stderr.write(component_name + ' does not have nominal mode defined: skipping.\n')
    
    print_connections(model)
    print '"""'
    
    print 'properties = """'
    print_properties(model)
    print '"""'
    
    print ''
    
    print 'with open("prismatic.pm", "w") as modelfile:'
    print '  modelfile.write(model)'
    print 'with open("prismatic.pctl", "w") as propertiesfile:'
    print '  propertiesfile.write(properties)'
    print 'prismout = subprocess.check_output(["c:/Program Files (x86)/prism-4.0.3/bin/prism.bat", modelfile.name, propertiesfile.name, "-fixdl"], shell=(os.name == "nt"))'
    print 'results = {}'
    print 'results["output"] = prismout'
    print 'results["properties"] = {}'
    print 'for prop in re.finditer("Model checking: (.*)", prismout):'
    print '  result = re.search("Result: (\S+)", prismout[prop.start(0):])'
    print '  results["properties"][prop.group(1)] = result.group(1)'
    print 'with open("prismatic-results.json", "w") as outfile:'
    print '  json.dump(results, outfile, indent=2)'


##################
# main starts here
##################
# FFL_MODE_LABELS = ['nominal', 'degraded', 'no_flow', 'lost']
# FOLDER_TYPES = ['RootFolder', 'Components', 'ComponentAssemblies', 'Testing', 'TestComponents']
# CONNECTION_TYPES = ['ElectricalSignalPort', 'InformationFlow', 'InSignal']
CONNECTION_TYPES = ['ElectricalSignalPort', 'InSignal', 'RotationalPowerPort']
INTERNAL_CONNECTION_TYPES = ['InformationFlow', 'PowerFlow', 'SignalConnection']
SRC_TYPES = ['srcInformationFlow', 'srcPowerFlow', 'srcInformationFlow_end_', 'srcPowerFlow_end_']

# COMPONENT_TYPES = ['BondGraph', 'Component', 'Components', 'ComponentAssembly', 'ComponentAssemblies', 'TestBench']
# OUTPUT_TYPES = ['ElectricalSignalPort', 'InSignal', 'OutSignal', 'Parameter']

transmission_function = '''
function [Ratio] = fcn(InRPM, OutRPM)

#Modelica
    constant Real ShiftRPM = 1800;
    constant Real G1 = 0.235849057;
    constant Real G2 = 0.327868852;
    constant Real G3 = 0.431034483;
    constant Real G4 = 0.598802395;
    constant Real G5 = 1;
    constant Real G6 = 1.388888889;
    constant Real G1Low = 0;
    constant Real G1High = ShiftRPM * G1;
    constant Real G2Low = G1High;
    constant Real G2High = ShiftRPM * G2;
    constant Real G3Low = G2High;
    constant Real G3High = ShiftRPM * G3;
    constant Real G4Low = G3High;
    constant Real G4High = ShiftRPM * G4;
    constant Real G5Low = G4High;
    constant Real G5High = ShiftRPM * G5;
    constant Real G6Low = G5High;
    constant Real G6High = 99999;
  algorithm 
    if OutRPM < G1High then 
        Ratio:=G1;
    elseif OutRPM >= G2Low and OutRPM < G2High then
      Ratio:=G2;

    elseif OutRPM >= G3Low and OutRPM < G3High then
      Ratio:=G3;

    elseif OutRPM >= G4Low and OutRPM < G4High then
      Ratio:=G4;

    elseif OutRPM >= G5Low and OutRPM < G5High then
      Ratio:=G5;

    elseif OutRPM >= G6Low and OutRPM < G6High then
      Ratio:=G6;


    else     Ratio:=1;

    end if;
'''
pp = pprint.PrettyPrinter(indent=2)
if ( len(sys.argv) < 2) or ( len(sys.argv) > 3 ):
    print "Usage: " + sys.argv[0] + " <json-or-xml-file>" + " [output path]"
    sys.exit(1)

model = ''

infile = sys.argv[1]
outpath = ''
if ( len( sys.argv ) <= 2 ):
    outpath = '.'
else:
    outpath = sys.argv[2]

input = file(infile).read()
base,extension = os.path.splitext(infile)
if extension.lower() == '.json':
    model = json.loads(input)
elif extension.lower() == '.xml':
    model = parse_xml(input)
else:
  print "Cannot detect file type. Please give .json or .xml extension"
  sys.exit(1)

outfilename = outpath + '\\' + 'prismatic.py'
with open(outfilename, 'w') as outfile:
    sys.stdout = outfile
    print_prismatic_script(infile, model)
sys.stdout = sys.__stdout__
# Anyone can execute the generated script.
os.chmod(outfilename, 0755)
