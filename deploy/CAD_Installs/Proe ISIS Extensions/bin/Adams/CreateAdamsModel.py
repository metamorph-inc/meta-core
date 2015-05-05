import os, os.path
import math
import argparse
import CreateAdamsModel_analysis as MA
import CreateAdamsModel_cadMetrics as CM
import CreateAdamsModel_cadPostProcessing as CP

# Global parameters - change if needed
globalParams = {"Analysis_File": "Analysis_MBD.xml",
                "CADMetrics_File": "CADAssembly_metrics.xml",
                "CADPostProc_File": "ComputedValues.xml",
                "AdamsExtension": "cmd",
                "PI": math.pi,
                "z_Axis": [0.0, 0.0, 1.0],  # must be a unit-vector!
                "Length": "mm",
                "Angle": "deg",  # don't change, will be reused!
                "Force": "newton",
                "Mass": "kg",
                "Time": "sec",
                "CoordinateSystem": "cartesian",
                "OrientationType": "body313",
                "MaterialName": "Steel",
                "MaterialYoung": "2.07E+005",
                "MaterialPoisson": "0.29",
                "MaterialDensity": "7.801E-006",
                "GroundName": "ground",
                "Ground_Prefix": "!\ndefaults model &\n   part_name = ground\n",
                "Component_Prefix": "!\ndefaults coordinate_system &\n   default_coordinate_system = .ExcavatorTest.ground\n",
                "Marker_Prefix": "MARKER_",
                "Joint_Prefix": "JOINT_",
                "FEA_Tool": "ABAQUS",
                "FEA_Type": "Standard",
                "FEA_Prefix": "LT_",
                "FEA_Suffix": ".lod",
                "FEA_Marker": "FEA_Origin",
                "SimTime": 5.0,
                "SimStep": 500,
                "SimScriptName": "Last_Sim",
                "AnalysisName": "Analysis_for_FEA",
                "GravityActive": "on",
                "Gravity_x": 0.0,
                "Gravity_y": -9.81,
                "Gravity_z": 0.0,
                "GEN_Fixed": "Fixed",  # translation for joint type(s)
                "ADS_Fixed": "fix",
                "Parasolid_Path": "Parasolid"}

# map CAD output joint names to Adams names
jointTypes = {"REVOLUTE": "revolute",
              "PRISMATIC": "translational",
              "FIXED": "fixed",
              "CYLINDRICAL": "cylindrical"}

scriptName = "CreateAdamsModel.py"
# Should merge bodies connected by fixed joints?
mergeFixedBodies = False
# Global id counters for joints/forces/motions and markers, respectively
idCounter = 1
idMarker = 1
idJoint = 1
precision = 0.0001
args = None

def vectorcompare(v1, v2):
    global precision
    diff = [math.fabs(v1[i] - v2[i]) for i in [0, 1, 2]]
    if diff[0] < precision and diff[1] < precision and diff[2] < precision:
        return True
    else:
        return False

def vectorclean(v):
    global precision
    result = [0 if math.fabs(i - 0) < precision else i for i in v]
    return result

def vectornorm(v):
    axis_sum = math.sqrt(sum([x ** 2 for x in v]))
    axis_unit = [x / axis_sum for x in v]
    return axis_unit

class ProcessingError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)

class MBD_Model(object):
    def __init__(self, name):
        self.name = name
        self.units = None
        self.ground = None
        self.simulation = None
        self.material_dict = {}
        self.motion_list = []
        self.result_FEA_list = []
        self.component_dict = {}
        self.joint_dict = {}
        self.merge_list = []
        self.terrainfile = None
        self.contacts = []

    def get_name(self):
        return self.name

    def add_cad_model(self, cad):
        self.cad_model = cad

    def add_component(self, component):
        self.component_dict[component.get_componentId()] = component

    def get_component(self, componentID):
        if componentID in self.component_dict:
            return self.component_dict[componentID]
        else:
            return None

    def add_contact(self, c):
        self.contacts.append(c)

    def add_fea_load(self, fea):
        self.result_FEA_list.append(fea)
        #if fea.get_componentID() in self.component_dict:
        #    self.component_dict[fea.get_componentID()].set_fea_flag()

    def add_gravity(self, gravity):
        self.gravity = gravity

    def add_ground(self, ground):
        self.ground = ground
        # add to component dictionary such markers associated with the ground can be found
        self.component_dict[ground.get_componentId()] = ground

    def add_joint(self, joint):
        self.joint_dict[joint.get_id()] = joint

    def get_joint(self, jointID):
        if jointID in self.joint_dict:
            return self.joint_dict[jointID]
        else:
            return None

    def add_material(self, material):
        self.material_dict[material.get_name()] = material

    def add_motion(self, motion):
        self.motion_list.append(motion)

    def add_simulation(self, simulation):
        self.simulation = simulation

    def add_units(self, units):
        self.units = units

    def add_merge(self, merge):
        self.merge_list.append(merge)

    def get_components(self):
        return self.component_dict.iteritems()

    def exportMerge2Adams(self):
        yString = ''
        for m in self.merge_list:
            yString += m.export2Adams()
        return yString

    def exportCAD2Adams(self):
        yString = '!\n'
        yString += '!----- Graphics from Parasolid file -----!\n!\n'
        yString += 'file parasolid read &\n'
        yString += '   file_name = "' + os.getcwd() + os.sep + self.cad_model + '_asm.x_t" &\n'
        yString += '   model_name = .' + self.name + '\n!\n'
        if self.terrainfile is not None:
            yString += '!\n'
            yString += '!----- Import terrain -----!\n!\n'
            yString += 'file parasolid read &\n'
            yString += '   file_name = "' + os.getcwd() + os.sep + self.terrainfile + '.x_t" &\n'
            yString += '   model_name = .' + self.name + '\n!\n'
            yString += 'part modify rigid_body name_and_position &\n'
            yString += '   part_name = .' + self.name + '.' + self.terrainfile + ' &\n'
            yString += '   new_part_name = .' + self.name + '.TERRAIN &\n'
            yString += '   location = 0.0, -1200.0, -1200.0 \n!\n'

        return yString

    def exportComponents2Adams(self):
        yString = '!\n'
        yString += '!--------- Rigid parts ----------!\n!\n'
        yString += globalParams['Ground_Prefix']
        if (self.ground is not None):
            yString += self.component_dict[self.ground.get_componentId()].export2Adams()
        for item in sorted(self.component_dict):
            if self.ground is None or item != self.ground.get_componentId():
                yString += self.component_dict[item].export2Adams(self.units.is_radian())
        return yString

    def exportFEA2Adams(self):
        yString = '!\n'
        yString += '!----- FEA load export -----!\n!\n'
        for item in self.result_FEA_list:
            yString += item.export2Adams()
        return yString

    def exportJoint2Adams(self):
        yString = '!\n'
        yString += '!----- Joints -----!\n!\n'
        for item in sorted(self.joint_dict):
            yString += self.joint_dict[item].export2Adams()
        return yString

    def exportMaterial2Adams(self):
        yString = '!\n'
        yString += '!----- Materials -----!\n!\n'
        for item in self.material_dict:
            yString += self.material_dict[item].export2Adams()
        return yString

    def exportContact2Adams(self):
        yString = '!\n'
        yString += '!----- Contacts -----!\n!\n'
        for contact in self.contacts:
            yString += contact.export2Adams()
        return yString

    def exportMotion2Adams(self):
        yString = '!\n'
        yString += '!----- Motions -----!\n!\n'
        for item in self.motion_list:
            yString += item.export2Adams()
        return yString

    def exportName2Adams(self):
        yString = '!\n!\n'
        yString += '!----- Adams/View Model -----!\n!\n!\n'
        yString += 'model create &\n'
        yString += '   model_name = ' + self.name + '\n'
        yString += '!\nview erase\n!\n'
        return yString

    def export2AdamsPrefix(self):
        yString = '!\n'
        yString += '! File automatically generated by ' + scriptName + ' script\n'
        yString += '!\n'
        return yString

    def export2AdamsSuffix(self):
        yString = '!\n'
        yString += '!----- Show model ----!\n!\n'
        yString += 'model display &\n'
        yString += '   model_name = ' + self.name + '\n!\n'
        return yString

    def export1stPass(self):
        yString = self.units.export2Adams()
        yString += self.exportName2Adams()
        yString += self.exportMaterial2Adams()
        yString += self.exportCAD2Adams()
        yString += self.exportMerge2Adams()
        yString += self.exportComponents2Adams()
        for item in sorted(self.component_dict):
            if self.ground is None or item != self.ground.get_componentId():
                yString += self.component_dict[item].exportInfoRequest()
        return yString

    def export2Adams(self):
        yString = self.export2AdamsPrefix()
        yString += self.units.export2Adams()
        yString += self.exportName2Adams()
        yString += self.exportMaterial2Adams()
        yString += self.exportCAD2Adams()
        yString += self.exportMerge2Adams()
        yString += self.exportComponents2Adams()
        yString += self.exportJoint2Adams()
        yString += self.exportContact2Adams()
        yString += self.exportMotion2Adams()
        yString += self.gravity.export2Adams()
        yString += self.simulation.export2Adams()
        yString += self.exportFEA2Adams()
        yString += self.export2AdamsSuffix()
        return yString


class MBD_Units(object):
    def __init__(self, unitObject=None, length=globalParams['Length'], angle=globalParams['Angle'],
                 force=globalParams['Force'], mass=globalParams['Mass'], time=globalParams['Time'],
                 coordinateSystem=globalParams['CoordinateSystem'], orientationType=globalParams['OrientationType']):
        if isinstance(unitObject, MA.UnitsType):
            self.length = unitObject.get_Length()
            self.angle = unitObject.get_Angle()
            self.force = unitObject.get_Force()
            self.mass = unitObject.get_Mass()
            self.time = unitObject.get_Time()
        else:
            self.length = length
            self.angle = angle
            self.force = force
            self.mass = mass
            self.time = time
        self.coordinateSystem = coordinateSystem
        self.orientationType = orientationType

    def is_radian(self):
        if self.angle == globalParams['Angle']:  # expected default is degree
            return False
        else:
            return True

    def export2Adams(self):
        yString = '!\n'
        yString += '!----- Default Units for Model -----!\n!\n!\n'
        yString += 'defaults units &\n'
        yString += '   length = ' + self.length + ' &\n'
        yString += '   angle = ' + self.angle + ' &\n'
        yString += '   force = ' + self.force + ' &\n'
        yString += '   mass = ' + self.mass + ' &\n'
        yString += '   time = ' + self.time + ' &\n'
        yString += '   coordinate_system_type = ' + self.coordinateSystem + ' &\n'
        yString += '   orientation_type = ' + self.orientationType + '\n'
        yString += '!\n'
        return yString


class MBD_Material(object):
    def __init__(self, name, young, poisson, density, modelname):
        self.name = name
        self.young = young  # Young's modulus
        self.poisson = poisson  # Poisson's ration
        self.density = density
        self.modelname = modelname

    def get_name(self):
        return self.name

    def export2Adams(self):
        yString = '!\n'
        yString += 'material create &\n'
        yString += '   material_name = .' + self.modelname + '.' + self.name + ' &\n'
        yString += '   youngs_modulus = ' + str(self.young) + ' &\n'
        yString += '   poissons_ratio = ' + str(self.poisson) + ' &\n'
        yString += '   density = ' + str(self.density) + '\n!\n'
        return yString


class MBD_Marker(object):
    def __init__(self, counter, location, orientation, existing = False):
        # self.id = id   # = MetricID in the Assembly_MBD.xml
        self.__existing = existing
        self.adamsid = counter  # Adams will need a unique number for each marker
        self.location = [float(x) for x in location.split(";")]
        if orientation is None:
            self.axis = [0.0, 0.0, 0.0]
        else:
            self.axis = [float(x) for x in orientation.split(";")]
            self.axis = vectornorm(vectorclean(self.axis))
        if not self.check_zero_axis(self.axis):
            self.adams_geotrafo()
        else:
            self.orientation = self.axis

    def get_adamsid(self):
        return self.adamsid

    def get_id(self):
        return self.adamsid

    def check_zero_axis(self, axis):
        if axis[0] or axis[1] or axis[2]:
            return False
        else:
            return True

    def adams_geotrafo(self):
        # Perform only if constraint axis exists and is not a zero-vector
        # then compute orientation in Euler angles (313)
        # ##
        # Compute unit vector of given axis
        axis_unit = vectornorm(self.axis)
        # Check whether axis is z-Axis or (-1)*z-Axis!
        if axis_unit == globalParams['z_Axis']:
            self.orientation = [0.0, 0.0, 0.0]
            return
        elif axis_unit == [(-1) * x for x in globalParams['z_Axis']]:
            self.orientation = [0.0, globalParams['PI'], 0.0]
            return
        # Compute the cross product of z-Axis and the given unit axis
        cross_product = [0.0, 0.0, 0.0]
        cross_product[0] = globalParams['z_Axis'][1] * axis_unit[2] - globalParams['z_Axis'][2] * axis_unit[1]
        cross_product[1] = - globalParams['z_Axis'][0] * axis_unit[2] + globalParams['z_Axis'][2] * axis_unit[0]
        cross_product[2] = globalParams['z_Axis'][0] * axis_unit[1] + globalParams['z_Axis'][1] * axis_unit[0]
        # Compute the unit vector of cross product -> rotation vector
        cross_sum = sum([x ** 2 for x in cross_product])
        rot_vector = [x / cross_sum for x in cross_product]
        # Compute dot product of z-Axis and the given unit axis
        dot_product = globalParams['z_Axis'][0] * axis_unit[0] + \
                      globalParams['z_Axis'][1] * axis_unit[1] + \
                      globalParams['z_Axis'][2] * axis_unit[2]
        rot_angle = math.acos(dot_product)
        # We have the Euler-axis (rot_vector) and angle (rot_angle)
        # Compute the quaternions based on axis/angle
        quat = [0.0, 0.0, 0.0, 0.0]
        quat[0] = rot_vector[0] * math.sin(rot_angle / 2)
        quat[1] = rot_vector[1] * math.sin(rot_angle / 2)
        quat[2] = rot_vector[2] * math.sin(rot_angle / 2)
        quat[3] = math.cos(rot_angle / 2)
        quat = vectornorm(quat)
        # Compute the 313-orientation based on quaternion
        self.orientation = [0.0, 0.0, 0.0]
        #self.orientation[0] = math.atan2((quat[0]*quat[2]+quat[1]*quat[3]),-(quat[1]*quat[2]-quat[0]*quat[3]))
        #self.orientation[1] = math.acos(quat[2]**2 + quat[3]**2 - quat[0]**2 - quat[1]**2)
        #self.orientation[2] = math.atan2((quat[0]*quat[2]-quat[1]*quat[3]), (quat[1]*quat[2]-quat[0]*quat[3]))
        self.orientation[0] = math.atan2((quat[0] * quat[2] + quat[1] * quat[3]),
                                         (quat[0] * quat[3] - quat[1] * quat[2]))
        self.orientation[1] = math.acos(quat[3] ** 2 - quat[0] ** 2 - quat[1] ** 2 + quat[2] ** 2)
        self.orientation[2] = math.atan2((quat[0] * quat[2] + quat[1] * quat[3]),
                                         (quat[0] * quat[3] - quat[1] * quat[2]))

    def exportOrientation(self, isRad=False):
        yString = ''
        if isRad:
            yString += str(self.orientation[0]) + ', ' + str(self.orientation[1]) + ', ' + str(self.orientation[2])
        else:
            yString += str(self.orientation[0] * (180 / globalParams['PI'])) + 'd, '
            yString += str(self.orientation[1] * (180 / globalParams['PI'])) + 'd, '
            yString += str(self.orientation[2] * (180 / globalParams['PI'])) + 'd '
        return yString

    def get_name(self):
        return str(globalParams['Marker_Prefix']) + str(self.adamsid);

    def export2Adams(self, parentname, isRad=False):
        if self.__existing:
            return ''
        yString = '!\n'
        yString += 'marker create &\n'
        yString += '   marker_name = .' + parentname + '.' + self.get_name() + ' &\n'
        yString += '   location = ' + str(self.location[0]) + ', ' + str(self.location[1]) + ', ' + str(
            self.location[2]) + ' &\n'
        yString += '   orientation = ' + str(self.orientation[0]) + ', ' + str(self.orientation[1]) + ', ' + str(
            self.orientation[2]) + '\n'
        return yString

class MBD_Body:
    def __init__(self, modelname, componentID, cadmodelname, existingbody=True, materialid=None, ground=False, readInfo = False):
        self.__componentID = componentID  # component id (instance id)
        self.__ownpartname = cadmodelname  # name of part
        self.__modelname = modelname  # name of the whole model
        self.materialid = materialid
        self.marker_dict = {}
        self.ground = ground
        self.__mergedbodies = []
        self.__existing = existingbody
        self.__geometries = []
        self.__metricstranslation = [] # translation coming from the metrics file
        if readInfo:
            self.read_infofile(self.get_ownPartname()+'.nfo')

    def merge_body(self, body):
        self.__mergedbodies.append(body)

    def get_componentId(self):
        if self.ground:
            return globalParams['GroundName']
        else:
            return self.__componentID

    def add_marker(self, marker):
        self.marker_dict[marker.get_id()] = marker

    def add_geometry(self, geom):
        self.__geometries.append(geom)

    def get_geometries(self):
        if self.__geometries is None:
            return []
        else:
            return self.__geometries

    def read_infofile(self, filename):
        try:
            f = open(filename, 'r')
            geommode = False
            while True:
                l = f.readline()
                if len(l) == 0:
                    return
                if not geommode:
                    if l.startswith('  Geometry:'):
                        geommode = True
                else:
                    if l == '\n' or l == ' \n':
                        geommode = False
                        continue
                    else:
                        lsplit = l.split()
                        self.add_geometry(lsplit[0])
        except:
            pass

    def get_marker(self, markerID):
        if markerID in self.marker_dict:
            return self.marker_dict[markerID]
        else:
            return None

    def get_modelname(self):
        return self.__modelname

    def get_ownPartname(self):
        return self.__ownpartname

    def get_fullname(self):
        return '.' + self.get_modelname() + '.' + self.get_ownPartname()

    def get_fea_marker_name(self):
        return '.' + self.__modelname + '.' + self.__ownpartname + '.' + globalParams['FEA_Marker'];

    def is_existing(self):
        return self.__existing

    def exportInfoRequest(self):
        yString = 'list_info part &\n'
        yString += '   part_name = .' + self.get_modelname() + '.' + self.get_ownPartname() + ' &\n'
        yString += '   file_name = "' + self.get_ownPartname() + '.nfo" &\n'
        yString += '   write_to_terminal = off &\n'
        yString += '   brief = off \n!\n'
        return yString

    def export2Adams(self, isRad=False):
        yString = '!\n'
        yString += '!----- ' + str(self.get_componentId()) + ' -----!\n!\n'
        yString += 'defaults coordinate_system &\n'
        yString += '   default_coordinate_system = .' + str(self.__modelname) + '.' + globalParams['GroundName'] + '\n'
        yString += '!\n'

        if not self.__existing:
            yString += 'part create rigid_body name_and_position &\n'
            yString += '   part_name = .' + self.get_modelname() + '.' + self.get_ownPartname() + ' \n!\n'
            yString += 'part create rigid_body mass_properties &\n'
            yString += '   part_name = .' + self.get_modelname() + '.' + self.get_ownPartname() + ' &\n'
            yString += '   material_type = .' + self.get_modelname() + '.' + globalParams['MaterialName'] + ' \n!\n'
            #yString += '   center_of_mass_marker = cm \n!\n'

        for child in self.__mergedbodies:
            if not child.is_existing:
                raise ProcessingError(
                    'Child of a merged body must be an existing part [' + child.get_ComponentInstanceID + '].')
            yString += 'part create rigid_body mass_properties &\n'
            yString += '   part_name = .' + child.get_modelname() + '.' + child.get_ownPartname() + ' &\n'
            yString += '   material_type = .' + child.get_modelname() + '.' + globalParams['MaterialName'] + ' \n!\n'

            yString += 'part merge rigid_body &\n'
            yString += '   part_name = .' + child.get_modelname() + '.' + child.get_ownPartname() + ' &\n'
            yString += '   into_part_name = .' + self.get_modelname() + '.' + self.get_ownPartname() + '\n!\n'

        yString += 'part create rigid_body mass_properties &\n'
        yString += '   part_name = .' + self.get_modelname() + '.' + self.get_ownPartname() + ' &\n'
        yString += '   material_type = .' + self.get_modelname() + '.' + globalParams['MaterialName'] + ' \n'

        for item in self.marker_dict:
            yString += self.marker_dict[item].export2Adams(self.__modelname + '.' + self.__ownpartname, isRad)
        if not self.ground:
            yString += '!\n'
            yString += 'marker create &\n'
            yString += '   marker_name = ' + self.get_fea_marker_name() + ' &\n'
            yString += '   location = (LOC_RELATIVE_TO({0, 0, 0}, .' + self.__modelname + '.' + self.__ownpartname + '.cm)) &\n'
            yString += '   orientation = (ORI_RELATIVE_TO({0d,0d,0d}, .' + self.__modelname + '.' + globalParams[
                'GroundName'] + '))\n'
        return yString

class MBD_Contact(object):
    def __init__(self, id, bodyI, bodyJ):
        self.__bodyI = bodyI
        self.__bodyJ = bodyJ
        self.id = id
    def export2Adams(self):
        geomJ = self.__bodyJ.get_geometries()[0]
        for geomI in self.__bodyI.get_geometries():
            yString = 'contact create &\n'
            yString += '   contact_name = test_' + str(self.id) + ' &\n'
            yString += '   i_geometry_name = ' + self.__bodyI.get_fullname() + '.' + geomI + ' &\n'
            yString += '   j_geometry_name = ' + self.__bodyJ.get_fullname() + '.' + geomJ + ' &\n'
            yString += '   stiffness = 1.0E+005 &\n'
            yString += '   damping = 10.0 &\n'
            yString += '   dmax = 0.11 &\n'
            yString += '   exponent = 2.2 &\n'
            yString += '   mu_static = 1.0 &\n'
            yString += '   mu_dynamic = 1.6 &\n'
            yString += '   coulomb_friction = dynamics_only &\n'
            yString += '   friction_transition_velocity = 1000.0 &\n'
            yString += '   stiction_transition_velocity = 100.0 \n!\n'
            return yString

class MBD_Joint(object):
    def __init__(self, id, idCounter, modelname, type, componentI, markerI, componentJ, markerJ):
        self.adamsid = idCounter
        if id is None or len(id) is 0:
            id = str(globalParams['Joint_Prefix']) + str(self.adamsid)
        self.id = id
        self.modelname = modelname
        self.type = type
        self.__bodyI = componentI
        self.markerI = markerI
        self.__bodyJ = componentJ
        self.markerJ = markerJ

    def get_id(self):
        return self.id

    def export2Adams(self):
        yString = '!\n'
        yString += 'constraint create joint ' + str(self.type).lower() + ' &\n'
        yString += '   joint_name = .' + self.modelname + '.' + str(self.id) + ' &\n'
        yString += '   adams_id = ' + str(self.adamsid) + ' &\n'
        yString += '   i_marker_name = .' + self.modelname + \
                   '.' + self.__bodyI.get_ownPartname() + '.' + self.markerI.get_name() + ' &\n'
        yString += '   j_marker_name = .' + self.modelname + \
                   '.' + self.__bodyJ.get_ownPartname() + '.' + self.markerJ.get_name() + ' \n'
        return yString


class MBD_Motion(object):
    def __init__(self, modelname, motionObject, counter):
        self.modelname = modelname
        if isinstance(motionObject, MA.MotionType):
            self.active = motionObject.get_Active()
            self.motionid = motionObject.get_MotionID()
            self.type = motionObject.get_Type()
            self.jointid = motionObject.get_JointID()
            self.freedom = motionObject.get_FreedomType()
            self.timederivative = motionObject.get_TimeDerivative()
            self.function = motionObject.get_Function()
            self.adamsid = counter
        else:
            self.active = 'off'

    def export2Adams(self):
        yString = ''
        if self.active.lower() == 'on':
            yString += '!\n'
            yString += 'constraint create motion_generator &\n'
            yString += '   motion_name = .' + self.modelname + '.' + self.motionid + ' &\n'
            yString += '   adams_id = ' + str(self.adamsid) + ' &\n'
            yString += '   type_of_freedom = ' + str(self.freedom).lower() + ' &\n'
            yString += '   joint_name = .' + self.modelname + '.' + self.jointid + ' &\n'
            yString += '   time_derivative = ' + str(self.timederivative).lower() + ' &\n'
            yString += '   function = "' + self.function + '"\n'
            # yString += 'constraint attributes &\n'
            #yString += '   constraint_name = .'+self.modelname+'.'+self.motionid+' &\n'
            #yString += '   name_visibility = off\n'
        return yString


class MBD_Gravity(object):
    def __init__(self, gravityObject):
        self.active = globalParams['GravityActive']
        if gravityObject is None:
            self.x = globalParams['Gravity_x']
            self.y = globalParams['Gravity_y']
            self.z = globalParams['Gravity_z']
        else:
            temp = [float(x) for x in gravityObject.Value.split(";")]
            self.x = temp[0]
            self.y = temp[1]
            self.z = temp[2]

    def export2Adams(self):
        yString = '!\n'
        yString += '!----- Gravity acceleration (Accgrav) -----!\n!\n!\n'
        if self.active.lower() == 'on':
            yString += 'force create body gravitational  &\n'
            yString += '   gravity_field_name = gravity  &\n'
            yString += '   x_component_gravity = ' + str(self.x) + ' &\n'
            yString += '   y_component_gravity = ' + str(self.y) + ' &\n'
            yString += '   z_component_gravity = ' + str(self.z) + '  \n'
            yString += '!\n'
        return yString


class MBD_FEA_Load(object):
    def __init__(self, component, feaObject):
        self.__body = component
        if isinstance(feaObject, MA.Metric_FEAType):
            self.feaTool = feaObject.get_FEA_Tool()
            self.feaType = feaObject.get_Type()
        else:
            self.feaTool = globalParams['FEA_Tool']
            self.feaType = globalParams['FEA_Type']

    def get_componentID(self):
        return self.__body.get_componentId()

    def export2Adams(self):
        # here we export for a single FEA load only!
        yString = ''
        if self.__body is not None:
            yString += 'file fea_loads write &\n'
            yString += '   file_name = "' + os.getcwd() + os.sep + globalParams[
                'FEA_Prefix'] + self.__body.get_ownPartname() + globalParams['FEA_Suffix'] + '" &\n'
            yString += '   format = ' + self.feaTool.upper() + ' &\n'
            yString += '   marker_name = ' + self.__body.get_fea_marker_name() + ' &\n'
            yString += '   analysis_name = ' + globalParams['AnalysisName'] + ' &\n'
            standard = 'no' if (self.feaType.lower() == globalParams['FEA_Type'].lower()) else 'yes'
            yString += '   no_inertia = ' + standard + '\n'
            yString += '!\n'
        return yString


class MBD_Merge(object):
    def __init__(self, mainmodel, modellist):
        self.main = mainmodel
        self.models = []
        for m in modellist:
            if m is not self.main:
                self.models.append(m)

    def export2Adams(self):
        yString = ''
        for m in self.models:
            yString += 'part merge rigid_body &\n'
            yString += '   part_name = .' + m.get_modelname() + '.' + m.get_partname() + ' &\n'
            yString += '   into_part_name = .' + self.main.get_modelname() + '.' + self.main.get_partname() + '\n'
            yString += '!\n'
        return yString


class MBD_Simulation(object):
    def __init__(self, modelname, sim):
        self.modelname = modelname
        if isinstance(sim, MA.SimulationType):
            self.time = sim.get_Time()
            self.steps = sim.get_Steps()
        else:
            self.time = globalParams['SimTime']
            self.steps = globalParams['SimSteps']

    def export2Adams(self):
        yString = '!\n!\n'
        yString += '!----- Simulation Scripts -----!\n!\n!\n'
        yString += 'simulation script create &\n'
        yString += '   sim_script_name = .' + self.modelname + '.' + globalParams['SimScriptName'] + ' &\n'
        yString += '   commands = "simulation single_run transient type=auto_select initial_static=no '
        yString += 'end_time=' + str(self.time) + ' number_of_steps=' + str(
            self.steps) + ' model_name=.' + self.modelname + '", &\n'
        yString += '              "simulation single_run save analysis_name=' + globalParams['AnalysisName'] + '"\n\n'
        yString += 'simulation single scripted &\n'
        yString += '   sim_script_name = .' + self.modelname + '.' + globalParams['SimScriptName'] + ' &\n'
        yString += '   reset_before_and_after = yes\n'
        yString += '!\n'
        return yString


class AssemblyMetrics:
    def __init__(self, cadmetrics):
        self.__cadcomponent_dict = self.__setupmetricsdict(cadmetrics)  # Associating component id with metrics id
        self.__metriccomp_dict = self.__setupmetriccompdict(cadmetrics)  # Associating metrics id with metrics data
        self.__rootasmid = cadmetrics.get_Assemblies().get_Assembly()[0].get_CADComponent().get_ComponentInstanceID()
        # Assumption: parasolid file name is identical to assembly name and the assembly is the 1st component in the list
        self.__parasolidname = self.get_CADModelName(self.__rootasmid)
        self.__kinematicdict = self.__setupkinematics(cadmetrics.get_Joints().get_JointsMetaData())

    def get_RootAsmID(self):
        return self.__rootasmid

    def __setupkinematics(self, jointsmetadata):
        dict = {}
        for jmdata in jointsmetadata:
            for compdata in jmdata.get_ComponentInstanceData():
                dict[compdata.get_ComponentInstanceID()] = False
        return dict

    def __setupmetricsdict(self, cadmetrics):
        dict = {}
        for asm in cadmetrics.get_Assemblies().get_Assembly():
            dict[asm.get_CADComponent().get_ComponentInstanceID()] = asm.get_CADComponent().get_MetricID()
            for comp in asm.get_CADComponent().get_CADComponent():
                dict[comp.get_ComponentInstanceID()] = comp.get_MetricID()
        return dict

    def __setupmetriccompdict(self, cadmetrics):
        dict = {}
        for mcomp in cadmetrics.get_MetricComponents().get_MetricComponent():
            dict[mcomp.get_MetricID()] = mcomp
        return dict

    def get_ComponentIDs(self):
        return self.__cadcomponent_dict.keys()

    def get_CADModelName(self, componentID):
        if componentID in self.__cadcomponent_dict:
            return self.__metriccomp_dict[self.__cadcomponent_dict[componentID]].get_Name()
        else:
            return None

    def get_CADModelType(self, componentID):
        if componentID in self.__cadcomponent_dict:
            return self.__metriccomp_dict[self.__cadcomponent_dict[componentID]].get_Type()
        else:
            return None

    def is_Kinematic(self, componentID):
        if componentID in self.__kinematicdict:
            return self.__kinematicdict[componentID]
        else:
            return True

    def is_assembly(self, componentID):
        return self.__metriccomp_dict[self.__cadcomponent_dict[componentID]].get_Type() == "ASSEMBLY"

    def get_Children(self, componentID):
        children = []
        if self.__metriccomp_dict[self.__cadcomponent_dict[componentID]].get_Children() is None:
            return children
        for child in self.__metriccomp_dict[self.__cadcomponent_dict[componentID]].get_Children().get_ChildMetric():
            children.append(child.get_ComponentInstanceID())
        return children

    def get_parent(self, componentid):
        # Assemblies can't have parents
        if self.is_assembly(componentid):
            return None
        for comp in self.__cadcomponent_dict.keys():
            if componentid in self.get_Children(comp):
                return comp
        return None

    def get_parasolidname(self):
        return self.__parasolidname


class CADPostProc:
    def __init__(self, cadpostproc):
        self.ground = None
        for comp in cadpostproc.get_Component():
            for metric in comp.get_Metrics().get_Metric():
                if metric.get_MetricID() == 'Anchor':
                    self.ground = metric.get_ArrayValue()

    def get_Ground(self):
        return self.ground


def setup_model(analysis, cadmetrics, cadpostproc, firstpass):
    global idCounter
    global idMarker
    global idJoint
    global jointTypes
    global args
    # print 'Model: %s' % (analysis.get_Name())
    # create model, pass its name
    mod = MBD_Model(analysis.get_Name())
    # add units
    mod.add_units(MBD_Units(analysis.get_Units()))
    # add material - currently steel will be supported as defined in the globalParams
    mod.add_material(MBD_Material(globalParams['MaterialName'], globalParams['MaterialYoung'],
                                  globalParams['MaterialPoisson'], globalParams['MaterialDensity'],
                                  analysis.get_Name()))
    # add Parasolid
    asm_metrics = AssemblyMetrics(cadmetrics)
    postproc = CADPostProc(cadpostproc)

    mod.add_cad_model(globalParams['Parasolid_Path'] + str(os.sep) + asm_metrics.get_parasolidname())

    fixedsets = []

    # Setup bodies
    processedcomps = []
    # Do assemblies first
    for compid in asm_metrics.get_ComponentIDs():
        if compid == asm_metrics.get_RootAsmID():
            continue
        if asm_metrics.is_assembly(compid):
            processedcomps.append(compid)
            if (asm_metrics.is_Kinematic((compid))):
                # Create bodies for all children (but not for the assembly)
                for child in asm_metrics.get_Children(compid):
                    mod.add_component(MBD_Body(analysis.get_Name(), child, asm_metrics.get_CADModelName(child), True, readInfo=not firstpass))
                    processedcomps.append(child)
            else:
                # Create a body for the assembly and add children as sub-parts (parts to be merged)
                added_comp = MBD_Body(analysis.get_Name(), compid, asm_metrics.get_CADModelName(compid), False, readInfo=not firstpass)
                for child in asm_metrics.get_Children(compid):
                    added_comp.merge_body(MBD_Body(analysis.get_Name(), child, asm_metrics.get_CADModelName(child), True))
                    processedcomps.append(child)
                mod.add_component(added_comp)

    # Whatever remains and not has been processed yet
    for compid in asm_metrics.get_ComponentIDs():
        if compid == asm_metrics.get_RootAsmID():
            continue
        if compid not in processedcomps:
            mod.add_component(MBD_Body(analysis.get_Name(), compid, asm_metrics.get_CADModelName(compid), True, readInfo=not firstpass))

    # Setup joints
    if cadmetrics.get_Joints() is not None:
        for joint in cadmetrics.get_Joints().get_Joint():
            if len(joint.get_ConstrainedToComponents().get_ConstrainedToComponent()) == 0:
                print "WARNING: " + joint.get_AssembledComponentInstanceID() + " is constrained to nothing."
                continue
            for constrainedtocomp in joint.get_ConstrainedToComponents().get_ConstrainedToComponent():
                added_instance_id = constrainedtocomp.get_ComponentInstanceID()
                assembly_instance_id = joint.get_AssembledComponentInstanceID()
                # Not interested in constraints to the top assembly (there's no top assembly in Adams)
                if (added_instance_id == asm_metrics.get_RootAsmID()):
                    continue
                if (asm_metrics.get_CADModelType(added_instance_id) == "ASSEMBLY" and asm_metrics.is_Kinematic(added_instance_id)):
                    print "Skipping joint " + joint.get_ID() + ", added component is a kinematic assembly: " + added_instance_id
                    continue
                if (asm_metrics.get_CADModelType(assembly_instance_id) == "ASSEMBLY" and asm_metrics.is_Kinematic(assembly_instance_id)):
                    print "Skipping joint " + joint.get_ID() + ", assembled component is a kinematic assembly: " + assembly_instance_id
                    continue
                component_added = mod.get_component(added_instance_id)
                if component_added is None:
                    added_instance_id = asm_metrics.get_parent(added_instance_id)
                    component_added = mod.get_component(added_instance_id)
                if component_added is None:
                    print "Component is not found, but referred to in a joint: " + added_instance_id
                    return None
                component_assembly = mod.get_component(assembly_instance_id)
                if component_assembly is None:
                    assembly_instance_id = asm_metrics.get_parent(assembly_instance_id)
                    component_assembly = mod.get_component(assembly_instance_id)
                if component_assembly is None:
                    print "Component is not found, but referred to in a joint: " + assembly_instance_id
                    return None
                asm_marker = MBD_Marker(idMarker, joint.get_GlobalCoordinateSystem().get_Location().get_ArrayValue(),
                                        joint.get_GlobalCoordinateSystem().get_Orientation().get_ArrayValue())
                component_assembly.add_marker(asm_marker)
                idMarker += 1
                added_marker = MBD_Marker(idMarker, joint.get_GlobalCoordinateSystem().get_Location().get_ArrayValue(),
                                          joint.get_GlobalCoordinateSystem().get_Orientation().get_ArrayValue())
                component_added.add_marker(added_marker)
                idMarker += 1
                if joint.get_Type() in jointTypes:
                    mod.add_joint(MBD_Joint(joint.get_ID(), idJoint, analysis.get_Name(), jointTypes[joint.get_Type()],
                                            component_assembly, asm_marker, component_added, added_marker))
                    # done = False
                    # if mergeFixedBodies and joint.get_Type() == "FIXED":
                    #     for s in fixedsets:
                    #         if component_assembly in s:
                    #             s.add(component_added)
                    #             done = True
                    #     if not done:
                    #         for s in fixedsets:
                    #             if component_added in s:
                    #                 s.add(component_assembly)
                    #                 done = True
                    #     if not done:
                    #         s = set()
                    #         s.add(component_assembly)
                    #         s.add(component_added)
                    #         fixedsets.append(s)
                    idJoint += 1
                else:
                    print "Invalid joint type: " + joint.get_Type() + ". Please correct this error."
                    return None
    else:
        print "No joints section present in metrics file."
        return None

    for s in fixedsets:
        for i in s:
            print i.get_partname()
        first = s.pop()
        mod.add_merge(MBD_Merge(first, s))
        print "\n"

    # add anchor/ground
    if args.terrain is not None:
        mod.terrainfile = args.terrain
        terrain = MBD_Body(analysis.get_Name(), "TERRAIN", 'TERRAIN', readInfo=not firstpass)
        terrain_marker = MBD_Marker(idMarker, "0;0;0", None)
        terrain.add_marker(terrain_marker)
        mod.add_component(terrain)
        idMarker += 1
    elif postproc.get_Ground() is None:
        print "No ground is present."
        return None
    else:
        anchor = analysis.get_Ground().get_CyphyId()

    groundpos = None
    if postproc.get_Ground() is None:
        groundpos = "0;0;0"
    else:
        groundpos = postproc.get_Ground()

    ground = MBD_Body(analysis.get_Name(), "ground", 'ground', ground=True)  # no material
    ground_marker = MBD_Marker(idMarker, groundpos, None)
    idMarker += 1
    ground.add_marker(ground_marker)
    mod.add_ground(ground)

    if args.terrain is not None:
        mod.add_joint(MBD_Joint("JOINT_root", idJoint, analysis.get_Name(), "fixed", ground, ground_marker,
                                terrain, terrain_marker))
        idJoint += 1
    elif anchor is None:
        print 'WARNING: No anchor component was specified. No body is going to be constrained to the ground.'
    # Fix the root to the ground
    elif anchor is not None and ground is not None:
        ground_added_marker = MBD_Marker(idMarker, postproc.get_Ground(), None)
        mod.get_component(anchor).add_marker(ground_added_marker)
        idMarker += 1
        mod.add_joint(MBD_Joint("JOINT_root", idJoint, analysis.get_Name(), "fixed", ground, ground_marker,
                                mod.get_component(anchor), ground_added_marker))
        idJoint += 1

    # Contacts
    for contact in analysis.get_Contact():
        mod.add_contact(MBD_Contact(idJoint, mod.get_component(contact.get_CyphyId1()), mod.get_component(contact.get_CyphyId2())))
        idJoint += 1

    for item in analysis.get_Loads().get_Motion():
        mod.add_motion(MBD_Motion(analysis.get_Name(), item, idCounter))
        idCounter += 1

    # add gravity
    mod.add_gravity(MBD_Gravity(analysis.get_Loads().get_Gravity()))
    # add simulation
    mod.add_simulation(MBD_Simulation(analysis.get_Name(), analysis.get_Simulation()))
    # add FEA export
    for k, v in mod.get_components():
        if (k != "ground"):
            mod.add_fea_load(MBD_FEA_Load(v, None))

    return mod


def AdamsExport(model, firstpass, isSilent=True):
    cwd = os.getcwd()
    filename = cwd + str(os.sep) + "adams_output" + '.' + globalParams['AdamsExtension']
    with open(filename, "w") as file:
        if firstpass:
            file.write(model.export1stPass())
        else:
            file.write(model.export2Adams())
    if not isSilent:
        print model.export2Adams()
    return

def main():
    global args
    parser = argparse.ArgumentParser(description='Creates an Adams .cmd file to build up the kinematic model.')
    parser.add_argument('-terrain');
    parser.add_argument('-firstpass');
    args = parser.parse_args()

    missing = False
    if not os.path.isfile(os.getcwd() + os.sep + globalParams["Analysis_File"]):
        print 'File ' + os.getcwd() + os.sep + globalParams["Analysis_File"] + ' is missing'
        missing = True
    if not os.path.isfile(os.getcwd() + os.sep + globalParams["CADMetrics_File"]):
        print 'File ' + os.getcwd() + os.sep + globalParams["CADMetrics_File"] + ' is missing'
        missing = True
    if not os.path.isfile(os.getcwd() + os.sep + globalParams["CADPostProc_File"]):
        print 'File ' + os.getcwd() + os.sep + globalParams["CADPostProc_File"] + ' is missing'
        missing = True

    if missing:
        exit(-1)

    try:
        analysis = MA.parse(os.getcwd() + os.sep + globalParams["Analysis_File"], True)
    except MA.GDSParseError as e:
        print "Unable to parse " + globalParams["Analysis_File"] + ", exiting. Error message: " + e.message
        exit(-1)

    try:
        cadmetrics = CM.parse(os.getcwd() + os.sep + globalParams["CADMetrics_File"], True)
    except CM.GDSParseError as e:
        print "Unable to parse " + globalParams["CADMetrics_File"] + ", exiting. Error message: " + e.message
        exit(-1)

    try:
        cadpostproc = CP.parse(os.getcwd() + os.sep + globalParams["CADPostProc_File"], True)
    except CM.GDSParseError as e:
        print "Unable to parse " + globalParams["CADPostProc_File"] + ", exiting. Error message: " + e.message
        exit(-1)

    firstpass = False

    if args.firstpass:
        firstpass = True

    model = setup_model(analysis, cadmetrics, cadpostproc, firstpass)

    try:
        if model is None:
            print "Unable to setup model, exiting."
            exit(-1)

        AdamsExport(model, firstpass, True)
    except ProcessingError as e:
        print "Processing error: " + e.message
        exit(-1)
    #except:
    #    print "Unexpected exception during script running."
    #    exit(-1)


if __name__ == '__main__':
    main()
