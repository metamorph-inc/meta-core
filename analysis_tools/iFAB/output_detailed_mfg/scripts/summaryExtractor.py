# Copyright 2012-2013 The Pennsylvania State University
#
# This script reads the cost and lead time from the detailed manufacturability JSON
# results returned from iFAB Foundry and updates the contents of the summary results JSON file
# (e.g., summary.testresults.json).  This summary results JSON file must already exist and must define
# the desired metrics.  The AnalysisStatus will be set to OK or FAILED, and the Value fields for the
# cost and lead time metrics will be set.
#
# Example Usage:
#       python summaryExtractor.py -i detailed.manufacturabilityResults.json -o testbench_manifest.json
#
# If -i is not specified, then it reads from STDIN.
# If -o is not specified, then it modifies the file "testbench_manifest.json"

import sys
import json
from optparse import OptionParser

def readManufacturabilityResults(file):
	""" Read the cost and lead time from a detailed manufacturability results JSON file. """
	content = json.load(file)

	if "status" in content[0]:
		if content[0]["status"] == "error":
			print >> sys.stderr, content[0]["message"]
			return None, None, False

	if "manufacturable" in content[0] and content[0]["manufacturable"]:
		cost = float(content[0]["bestScore"]["cost($)"])
		time = float(content[0]["bestScore"]["time(min)"]) / (60*24)
		return cost, time, True
	else:
		cost = 0
		time = 0

		if "manufacturabilityAssessment" in content[0]:
			for part in content[0]["manufacturabilityAssessment"]:
				if "cost($)" in part:
					if "quantity" in part:
						cost = cost + float(part["quantity"]) * float(part["cost($)"])
					else:
						cost = cost + float(part["cost($)"])
				if "time(min)" in part:
					time = max(time, float(part["time(min)"]) / (60*24))

		return cost, time, False



def modifySummary(filename, cost, leadTime, isManufacturable):
	""" Modifies an existing testbench_manifest file, changing the AnalysisStatus and Value fields. """
	print filename
	with open(filename, "r") as file:
		summary = json.load(file)

	summary["TierLevel"] = 2
	summary["TestBench"] = "Foundry"

	if cost and leadTime:
		summary["AnalysisStatus"] = "OK"

		for metric in summary["Metrics"]:
			if metric["Name"] == "Vehicle_Unit_Cost":
				metric["Value"] = str(cost)
				metric["Unit"] = "dollars"
			elif metric["Name"] == "Manufacturing_Lead_Time":
				metric["Value"] = str(leadTime)
				metric["Unit"] = "days"
			elif metric["Name"] == "Manufacturable":
				metric["Value"] = str(isManufacturable)
				metric["Unit"] = ""
	else:
		summary["AnalysisStatus"] = "FAILED"

		for metric in summary["Metrics"]:
			if metric["Name"] == "Vehicle_Unit_Cost":
				metric["Value"] = str(0.0)
				metric["Unit"] = "dollars"
			elif metric["Name"] == "Manufacturing_Lead_Time":
				metric["Value"] = str(0.0)
				metric["Unit"] = "days"
			elif metric["Name"] == "Manufacturable":
				metric["Value"] = "false"
				metric["Unit"] = ""


	with open(filename, "w") as file:
		json.dump(summary, file, indent=4)

parser = OptionParser()
parser.add_option("-i", "--input", dest="input", help="Detailed manufacturability JSON results from iFAB")
parser.add_option("-o", "--output", dest="output", help="The testbench_manifest JSON file to modify")
(options, args) = parser.parse_args()

try:
	if options.input:
		with open(options.input, "r") as file:
			cost, leadTime, isManufacturable = readManufacturabilityResults(file)
	else:
		cost, leadTime, isManufacturable = readManufacturabilityResults(sys.stdin)
except:
	print >> sys.stderr, "Error reading manufacturability results:", sys.exc_info()[1]
	cost, leadTime, isManufacturable = None, None, False

print cost
print leadTime
print isManufacturable

if options.output:
	modifySummary(options.output, cost, leadTime, isManufacturable)
else:
	modifySummary("testbench_manifest.json", cost, leadTime, isManufacturable)