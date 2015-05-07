
"""This module does blah blah."""
import cad_library.cadMetrics_xml
import cad_library.cadpostprocessing_xml
import math

PRECISION = 0.0001

class Vec3(object):
    """Represents a 3D vector."""
    @classmethod
    def zunit(cls):
        """Unit vector Z direction."""
        return Vec3([0.0, 0.0, 1.0])

    @classmethod
    def xunit(cls):
        """Unit vector Z direction."""
        return Vec3([1.0, 0.0, 0.0])

    @classmethod
    def yunit(cls):
        """Unit vector Z direction."""
        return Vec3([0.0, 1.0, 0.0])

    @classmethod
    def null(cls):
        """Nullvector."""
        return Vec3([0.0, 0.0, 0.0])

    @classmethod
    def parse(cls, vect):
        """Parses a ;-separated string into a Vec3."""
        return Vec3([float(x) for x in vect.split(";")])

    def __init__(self, data):
        """Constructor."""
        self.__data = data

    def __len__(self):
        return math.sqrt(sum([x ** 2 for x in self.__data]))

    def normalize(self):
        """Makes this vector unit-length."""
        length = len(self)
        if length == 0:
            return self
        self.__data = [x / length for x in self.__data]
        return self

    def clean(self):
        """Zeroes out near-zero values."""
        self.__data = [0 if math.fabs(i - 0) < PRECISION else i for i in self.__data]
        return self

    def __add__(self, other):
        """Adds 2 vectors."""
        return Vec3([self.__data[0] + other.__data[0], self.__data[1] + other.__data[1], self.__data[2] + other.__data[2]])

    def __sub__(self, other):
        return Vec3([self.__data[0] - other.__data[0], self.__data[1] - other.__data[1], self.__data[2] - other.__data[2]])

    def copy(self):
        """Returns a copy of this vector."""
        return Vec3(list(self.__data))

    def getnormalized(self):
        """Returns a normalized copy of this vector."""
        return self.copy().normalize()

    def __cmp__(self, other):
        """Compares 2 vectors."""
        if other[0] == self[0] and other[1] == self[1] and other[2] == self[2]:
            return 0
        else:
            return 1

    def __getitem__(self, item):
        """Indexer."""
        return self.__data.__getitem__(item)

    def crossprod(self, other):
        return Vec3([self.__data[1]*other[2]-self.__data[2]*other[2], self.__data[2]*other[0]-self.__data[0]*other[2], self.__data[0]*other[1]-self.__data[1]*other[0]])

    def dotprod(self, other):
        return self[0]*other[0]+self[1]*other[1]+self[2]*other[2]

    def angle(self, other):
        return math.acos(self.dotprod(other)/(len(self)*len(other)))

    def mul(self, val):
        return Vec3([self[0]*val, self[1]*val, self[2]*val])

    def __str__(self):
        return '[' + str(self[0]) + ',' + str(self[1]) + ',' + str(self[2]) + ']'

class Mat4x4(object):
    """Represents a 4x4 matrix (includes 3D translation and rotation)"""
    @classmethod
    def unit(cls):
        """Unit matrix."""
        return Mat4x4([1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1])

    def __init__(self, content):
        """Constructor."""
        self.__data = content

    def mul(self, other):
        """Multiplies this matrix with another. Returns a new matrix. Doesn't modify original ones."""
        newdata = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        for i in range(0, 4):
            for j in range(0, 4):
                for k in range(0, 4):
                    newdata[i * 4 + j] = newdata[i * 4 + j] + self.__data[i * 4 + k] * other.__data[k * 4 + j]
        return Mat4x4(newdata)

    def copy(self):
        """Returns a copy."""
        return Mat4x4(list(self.__data))

    def get_translation(self):
        """Returns a Vec3 representing the translation part."""
        return Vec3([self.__data[3], self.__data[7], self.__data[11]])

    def __str__(self):
        """Returns a str rep."""
        return '[' + str(self.__data[0]) + ',' + str(self.__data[1]) + ',' + str(self.__data[2]) + ',' + str(self.__data[3]) + ']\n[' + str(
            self.__data[4]) + "," + str(self.__data[5]) + "," + str(self.__data[6]) + ',' + str(self.__data[7]) + "]\n" + "[" + str(
            self.__data[8]) + "," + str(self.__data[9]) + "," + str(self.__data[10]) + ',' + str(self.__data[11] + "]\n" + "[" + str(
            self.__data[12]) + ',' + str(self.__data[13]) + ',' + str(self.__data[14]) + ',' + str(self.__data[15]) + "]\n")

    def to_euler313(self):
        """Returns an Euler313 representation of the rotation."""
        axis_unit = Vec3([self.__data[8], self.__data[9], self.__data[10]]).normalize().clean()
        # Check whether axis is z-Axis or (-1)*z-Axis!
        if cmp(axis_unit, Vec3.zunit()) == 0:
            return Vec3([math.atan2(-self.__data[1], self.__data[0]), 0.0, 0.0])
        elif cmp(axis_unit, Vec3([(-1) * x for x in Vec3.zunit()])) == 0:
            return Vec3([-math.atan2(-self.__data[1], self.__data[0]), math.pi, 0.0])

        orientation = [0.0, 0.0, 0.0]

        if axis_unit[2] < 1:
            if axis_unit[2] > -1:
                orientation[0] = math.atan2(self.__data[2], -self.__data[6])
                orientation[1] = math.acos(self.__data[10])
                orientation[2] = math.atan2(self.__data[8], self.__data[9])
                return Vec3(orientation)
            else:  # == -1
                orientation[0] = -math.atan2(-self.__data[1], -self.__data[0])
                orientation[1] = math.pi
                orientation[2] = 0
                return Vec3(orientation)

    def __cmp__(self, other):
        return cmp(self.__data,other.__data)

class CalculixResults(object):
    """Represents information about the Calculix results data."""
    def __init__(self):
        self.job = {}
        self.nodes = {}
        self.elements = {}
        self.results = {}
    
    def enum(self, **enums):
        return type('Enum', (), enums)
    
    def parse_frd(self, filename):
        block_code = self.enum(JOB='1C', NODE='2C', ELEM='3C', STEP='1PSTEP',
                               METRIC='100CL', SECEND='-3', FRDEND='9999')
        prev_el = ''
        update_vars = True
        with open(filename) as f:
            for line in f:
                # If entering new block, update codes
                if update_vars or any([block_code.NODE==line.split()[0], \
                                       block_code.METRIC==line.split()[0]]):
                    # Reason for 'or': There is no end code between 1C and 2C.                 
                    curr_code = line.split()[0]
                    if curr_code != block_code.JOB:
                        try:
                            total = line.split()[1]
                        except IndexError:
                            if line.split()[0] == block_code.FRDEND:
                                pass
                            else:
                                raise Exception('Issue grabbing section totals!')
                        if curr_code == block_code.METRIC:
                            metric = None
                            submetrics = None
                            timestep = float(line.split()[2])
                    update_vars = False
                    continue  # Skip to next line with new codes  

                if block_code.SECEND == line.split()[0]:
                    update_vars = True    
                    continue
                if curr_code == block_code.JOB:
                    self._get_job_information(line)
                elif curr_code == block_code.NODE:
                    self._get_nodes(line, total)
                elif curr_code == block_code.ELEM:
                    prev_el = self._get_elements(line, prev_el, total)
                elif curr_code == block_code.METRIC:
                    metric, submetrics, total = self._get_results(line, metric, submetrics, timestep, total)
                else:
                    raise Exception("Block code discrepancy: " + curr_code)
             
    def _get_job_information(self, line):
        info = line.split()
        try:
            self.job[info[0][2:].lower()] = info[1]
        except IndexError:
            self.job[info[0][2:].lower()] = ''
        finally:
            if '' in self.job.keys():
                del self.job['']
          
          
    def _get_nodes(self, line, total):
        # info: New/Continuing line, Element, X-pos, Y-Pos, Z-pos
        # info[0] = -1, New information. 
        # info[0] = -2, continuation of previous line.
        info = self._split_line(line.split())
        expect_break = False
        if info[0] == -1:
            self.nodes[float(info[1])] = {'x': info[2], 'y': info[3], 'z': info[4]}
        else:
            print "Failing line: " + line
            raise Exception('Unexpected node description.')
        if float(info[1]) == float(total):
            expect_break = True
        return expect_break
        
    
    def _get_elements(self, line, prev_el, total):
        # info: New/Continuing line, Element, ElType, Material Number, nodes
        # info[0] = -1, New information. 
        # info[0] = -2, continuation of previous line.
        info = line.split()
        expect_break = False
        if info[0] == '-1':
            prev_el = float(info[1])
            self.elements[prev_el] = {'type': info[2], 'material': info[3], 'nodes': []}
        elif info[0] == '-2':
            self.elements[prev_el]['nodes'].append([n for n in info[1:]])
            if self.elements[prev_el]['type'] == 6 and len(self.elements[prev_el]['nodes']) != 10:
                raise Exception('Element not fully defined!')
            prev_el = ''
        else:
            print "Failing line: " + line
            raise Exception('Unexpected element description.')        
        return prev_el
        
    def _get_results(self, line, metric, submetrics, timestep, total):
        """ Populate results dictionary given current line.
            Results dictionary form:
                { metric1:  {  
                    timestep1:  {
                        sub-metric1:  {
                            Element: Value }
                        sub-metric2: {
                            Element: Value } } }
                  metric2:  {
                    timestep1:  {
                        sub-metric1:  {
                            Element: Value } } } 
                }
            - Metrics can be defined on different timesteps
                (Timestep1 could be 0-1, Timstep2 could be 1-2)
        """
        info = line.split()
        expect_break = False
        if info[0] == '-4':
            # Metric definition
            metric = info[1]
            total = float(info[2])
            if metric not in self.results:
                self.results[metric] = {}
            self.results[metric][timestep] = {}
        elif info[0] == '-5':
            # Sub-metric definition
            if submetrics:
                submetrics.append(info[1])
            else:
                submetrics = [info[1]]
            self.results[metric][timestep][info[1]] = {}
        elif info[0] == '-1':
            # All sub-metric results for given element
            if len(submetrics) != total:
                raise Exception('Unexpected number of sub-metrics.')
            if len(info) < total + 2:
                # First two indices: "-1" and element number
                # If less than total, negative sign is combining two values, split again
                info = self._split_line(info)
            for i in range(len(submetrics)):
                sub = submetrics[i]
                try:
                    self.results[metric][timestep][sub][float(info[1])] = float(info[2+i])
                except IndexError:
                    # ALL is magnitude of components, not directly given.
                    if sub == 'ALL':
                        self.results[metric][timestep][sub][float(info[1])] = \
                                math.sqrt(sum([float(info[s])**2 for s in range(2,len(info))]))
                    else:
                        raise Exception('Unknown metric: ' + metric)
        else:
            raise Exception('Unknown line code: ' + info[0])
        return metric, submetrics, total
                
    def _split_line(self, info):
        """ Split line with negative sign as delimiter.
            Example: "193 8.02941E-01-2.25000E+02 0.00000E+00"
        """
        d = '-'  # delimiter
        newline = [info[0]]
        for i in range(1,len(info)):
            temp = [e for e in info[i].split(d) if e != '']
            for j in range(0,len(temp)):
                try:
                    # Occurs if value is E-##. Need to concatenate exponent to value.
                    neg_E = temp[j].endswith('E')
                except IndexError:
                    # By deleting previous entries, length has shortened and
                    # last element's index has decreased by one. Not a failure.
                    break
                if neg_E:
                    temp[j] = d + temp[j] + d + temp[j+1]
                    del temp[j+1]  # Delete E-## index, as has now been concatenated.
                else:
                    if j > 0:
                        temp[j] = d + temp[j]  # Make value negative.
            newline.extend(temp)
        return map(float, newline)
                
class AssemblyInfo(object):
    """Represents information about the assembly and analysis data."""
    def __init__(self):
        """Ctor."""
        self.root = None
        self.componentsdict = {}
        self.joints = []

    def read_metrics_file(self, filename):
        """Reads the metrics xml file and builds up structures."""
        cadmetrics = cad_library.cadMetrics_xml.parse(filename, True)

        mdict = {}
        orderdict = {}
        i = 0
        for asm in cadmetrics.get_Assemblies().get_Assembly():
            mdict[asm.get_CADComponent().get_ComponentInstanceID()] = asm.get_CADComponent().get_MetricID()
            orderdict[asm.get_CADComponent().get_ComponentInstanceID()] = i
            i = i + 1
            for comp in asm.get_CADComponent().get_CADComponent():
                mdict[comp.get_ComponentInstanceID()] = comp.get_MetricID()
                orderdict[comp.get_ComponentInstanceID()] = i
                i = i + 1
        cdict = {}
        for mcomp in cadmetrics.get_MetricComponents().get_MetricComponent():
            cdict[mcomp.get_MetricID()] = mcomp
        for i in mdict.keys():
            self.componentsdict[i] = self._read_metrics_data(cdict[mdict[i]], i)
            self.componentsdict[i].order = orderdict[i]

        self._read_kinematics_data(cadmetrics.get_Joints().get_JointsMetaData())

        for comp in self.componentsdict.values():
            comp.resolve_children(self)

        self.root = self.componentsdict[
            cadmetrics.get_Assemblies().get_Assembly()[0].get_CADComponent().get_ComponentInstanceID()]

        lst = []
        ComponentData._walk_steporder(self.root, lst)

        i=0
        for x in lst:
            x.steporder = i
            i = i + 1

        self._read_joint_data(cadmetrics.get_Joints())

    def _read_joint_data(self, jdata):
        """Reads the joints section of the metrics file."""
        for joint in jdata.get_Joint():
            jdata = JointData(self.componentsdict[joint.get_AssembledComponentInstanceID()],
                              [self.componentsdict[constrainedtocomp.get_ComponentInstanceID()] for constrainedtocomp in
                               joint.get_ConstrainedToComponents().get_ConstrainedToComponent()],
                              joint.get_Type())
            jdata.location = Vec3(
                [float(x) for x in joint.get_GlobalCoordinateSystem().get_Location().get_ArrayValue().split(";")])
            jdata.orientation = Vec3(
                [float(x) for x in joint.get_GlobalCoordinateSystem().get_Orientation().get_ArrayValue().split(";")])
            jdata.id = joint.get_ID()
            self.joints.append(jdata)

    def _read_metrics_data(self, mdata, cyphyid):
        """Reads metrics data for an individual component."""
        cdata = ComponentData(cyphyid)
        cdata.import_metricsdata(mdata)
        return cdata

    def _read_kinematics_data(self, kdata):
        """Reads the jointmetadata section of the metrics file."""
        for jmdata in kdata:
            for compdata in jmdata.get_ComponentInstanceData():
                self.componentsdict[compdata.get_ComponentInstanceID()].kinematic = False

    def compare(self, other):
        if cmp(self.root.cyphyid,other.root.cyphyid)!=0:
            print 'Comparison root failed: ' + self.root.cyphyid + ' vs ' + other.root.cyphyid
            return False
        for x in self.componentsdict:
            if x in other.componentsdict:
                if cmp(self.componentsdict[x].localmatrix,other.componentsdict[x].localmatrix) != 0:
                    return False
            else:
                return False
        return True

class CADMetrics(object):
    def __init__(self):
        self.metrics = {}
        self.metricslist = []

    def read_metrics_file(self, filename):
        cadmetrics = cad_library.cadpostprocessing_xml.parse(filename)

        for comp in cadmetrics.get_Component():
            for m in comp.get_Metrics().get_Metric():
                metric = CADMetric()
                metric.id=m.get_MetricID()
                metric.type=m.get_Type()
                metric.metricname=m.get_MetricName()
                if metric.type=="SCALAR":
                    metric.value=float(m.get_ArrayValue())
                elif metric.type=="VECTOR":
                    metric.value = Vec3.parse(m.get_ArrayValue())
                self.metrics[metric.metricname] = metric
                self.metricslist.append(metric)

class CADMetric(object):
    def __init__(self):
        self.type = ""
        self.id = ""
        self.value = None
        self.metricname = None

class JointData(object):
    """Represents data related to one joint."""
    def __init__(self, compfrom, compto, type):
        """Ctor."""
        self.compfrom = compfrom
        self.compto = compto
        self.type = type
        self.location = None
        self.orientation = None
        self.id = None

class Scalar(object):
    def __init__(self, unit, value):
        self.unit = unit
        self.value = value

class Units(object):
    def __init__(self):
        self.distance = None
        self.force = None
        self.mass = None
        self.temp = None
        self.time = None

    def read_from_metric(self, metric):
        self.distance = metric.get_Units().get_Distance()
        self.force = metric.get_Units().get_Force()
        self.mass = metric.get_Units().get_Mass()
        self.temp = metric.get_Units().get_Temperature()
        self.time = metric.get_Units().get_Time()

class ComponentData(object):
    def __init__(self, cyphyid):
        self.cyphyid = cyphyid
        self.localmatrix = None
        self.type = None
        self.cadmodelname = None
        self.children = []
        self.__childlocalmat = []
        self.psolid = None
        self.kinematic = True
        self.parent = None
        self.materialname = None
        self.scalars = {}
        self.units = None
        self.metricsorder = None
        self.steporder = None
        self.centerofgravity = None

    @classmethod
    def _walk_steporder(cls, root, lst):
        for x in root.children:
            ComponentData._walk_steporder(x,lst)
        lst.append(root)

    @classmethod
    def dump_hierarchy(cls, root, level = 0):
        for x in root.children:
            ComponentData.dump_hierarchy(x, level+1)
        print ('\t' * level)  + root.cyphyid + ", " + str(root.steporder) + ', ' + str(root.cadmodelname)

    def import_metricsdata(self, mdata):
        self.type = mdata.get_Type()
        self.cadmodelname = mdata.get_Name()
        if mdata.get_Material() is not None:
            self.materialname = mdata.get_Material().get_Type()
        for scalar in mdata.get_Scalars().get_Scalar():
            self.scalars[scalar.get_Name()] = Scalar(scalar.get_Unit(), scalar.get_Value())

        self.centerofgravity = Vec3([mdata.get_CG().get_X(), mdata.get_CG().get_Y(), mdata.get_CG().get_Z()])

        self.units = Units()
        self.units.read_from_metric(mdata)

        if mdata.get_Children() is not None:
            for child in mdata.get_Children().get_ChildMetric():
                self.children.append(child.get_ComponentInstanceID())
                self.__childlocalmat.append(
                    Mat4x4([float(child.get_RotationMatrix().get_Rows().get_Row()[0].get_Column()[0].get_Value()), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[0].get_Column()[1].get_Value()), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[0].get_Column()[2].get_Value()), \
                            float(child.get_Translation().get_X()), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[1].get_Column()[0].get_Value()), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[1].get_Column()[1].get_Value()), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[1].get_Column()[2].get_Value()), \
                            child.get_Translation().get_Y(), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[2].get_Column()[0].get_Value()), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[2].get_Column()[1].get_Value()), \
                            float(child.get_RotationMatrix().get_Rows().get_Row()[2].get_Column()[2].get_Value()),
                            child.get_Translation().get_Z(), \
                            float(0), float(0), float(0), float(1)]))

    def resolve_children(self, asminfo):
        newchildren = []
        i = 0
        for child in self.children:
            newchildren.append(asminfo.componentsdict[child])
            asminfo.componentsdict[child].localmatrix = self.__childlocalmat[i]
            asminfo.componentsdict[child].parent = self
            i = i + 1
        self.children = newchildren

    def is_assembly(self):
        """True if component is an assembly."""
        return self.type == 'ASSEMBLY'

    def get_global_matrix(self):
        """Returns the transformation matrix relative to the assembly root."""
        # print componentID
        parent = self
        parentlist = []
        while parent is not None:
            parentlist.append(parent)
            parent = parent.parent
        # The last item is the assembly root which doesn't have a matrix
        parentlist.pop(len(parentlist) - 1)
        result = None
        for p in reversed(parentlist):
            if result is None:
                result = p.localmatrix.copy()
            else:
                result = result.mul(p.localmatrix)
        return result
