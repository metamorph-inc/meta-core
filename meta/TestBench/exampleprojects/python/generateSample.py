import sys
sys.path.insert(0, '../../lib/python')
import TestBenchManifest


tbm = TestBenchManifest.TestBenchManifest()


# Set basic attributes
tbm.Name = "ABC Analysis"
tbm.DesignID = "123-245-2342-23421"
tbm.Status = "unexecuted"


# Add Parameters
p1 = TestBenchManifest.Parameter()
p1.Name = "Parameter1"
p1.Unit = "Millimeter"
p1.Value = 1.2
tbm.Parameters.append(p1)

p2 = TestBenchManifest.Parameter()
p2.Name = "Parameter2"
p2.Unit = "Kilogram"
p2.Value = 99.21
tbm.Parameters.append(p2)


# Add Metrics
m1 = TestBenchManifest.Metric()
m1.Name = "Metric1"
m1.Unit = "DegreeAngle"
m1.Value = None
tbm.Metrics.append(m1)


# Add Artifacts
a1 = TestBenchManifest.Artifact()
a1.Location = "design1235.metadesign.xml"
a1.Tag = "Design Model"
tbm.Artifacts.append(a1)

a2 = TestBenchManifest.Artifact()
a2.Location = "components/index.json"
a2.Tag = "Component Model Index"
tbm.Artifacts.append(a2)


# Add Execution Steps
s1 = TestBenchManifest.Step()
s1.Description = "Build CAD Assembly of system. " \
                 "In post-processing, write calculated " \
                 "metrics and artifact locators back to this file."
s1.Type = "CMD"
s1.PreProcess = None
s1.PostProcess = "retrieveMetrics.py"
s1.Invocation = "buildIt.bat"
tbm.Steps.append(s1)

s2 = TestBenchManifest.Step()
s2.Description = "Submit artifacts to iFAB analysis. c" \
                 "In post-processing, poll their service until " \
                 "results are available, then write them back to this file."
s2.Type = "REST"
s2.PreProcess = "sendArtifactsToIFAB.py"
s2.PostProcess = "retrieveMetrics.py"
s2.Invocation = "https://ifab.com/analyze"
s2p = TestBenchManifest.Parameter()
s2p.Value = "1245522"
s2p.Name = "PackageID"
s2.Parameters.append(s2p)
tbm.Steps.append(s2)


print(tbm.SerializeToString())