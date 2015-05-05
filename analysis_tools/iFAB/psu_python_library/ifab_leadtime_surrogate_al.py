from sys import argv
from sys import stderr

import json

try:
    with open("testbench_manifest.json", "r+") as j:
        manifest = json.load(j)
except IOError:
    manifest={"Parameters":[], "Metrics":[]}

mass = None
for param in manifest["Parameters"]:
    if param["Name"] == "MassInput":
        mass = param["Value"]

if mass is None:
    mass = float(argv[1])

surrogate_leadtime = 144.3 - (1.6897 * 10**5) / (mass)

for metric in manifest["Metrics"]:
    if metric["Name"] == "surrogate_leadtime":
        metric["Value"] = surrogate_leadtime
        metric["Unit"] = "days"
        
with open("testbench_manifest.json", "w") as json_file:
    json.dump(manifest, json_file, indent=4)

stderr.write("{0:.2f}\n".format(surrogate_leadtime))
