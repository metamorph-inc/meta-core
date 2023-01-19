import datetime
import json

class ValueWithUnit:
    def __init__(self):
        self.Name = None
        self.Unit = None
        self.Value = None

class Artifact:
    def __init__(self):
        self.Location = None
        self.Tag = None

class Parameter(ValueWithUnit):
    def __init__(self):
        ValueWithUnit.__init__(self)

class Metric(ValueWithUnit):
    def __init__(self):
        ValueWithUnit.__init__(self)

class Step:
    def __init__(self):
        self.Invocation = None
        self.PreProcess = None
        self.PostProcess = None
        self.Description = None

        # REST, Python, or CMD
        self.Type = None

        # Use ISO-8601 via datetime.datatime.now().isoformat()
        self.ExecutionStartTimestamp = None
        self.ExecutionCompletionTimestamp = None

        self.Parameters = []

class TestBenchManifest:
    def __init__(self):
        self.Steps = []
        self.Metrics = []
        self.Artifacts = []
        self.Parameters = []

        self.Name = None
        self.Created = datetime.datetime.now().isoformat()
        self.DesignID = None

        # unexecuted, in-progress, complete
        self.Status = "unexecuted"

    def SerializeToString(self):
        return json.dumps(self.__dict__,sort_keys=True,indent=2,cls=self.TBJSONEncoder)

    def SerializeToFile(self,path):
        f = open(path,'w')
        json.dump(self,f,indent=2,sort_keys=True,cls=self.TBJSONEncoder)

    class TBJSONEncoder(json.JSONEncoder):
        def default(self,obj):
            return obj.__dict__