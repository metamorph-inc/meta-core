import os, os.path
import math
import argparse
import CreateAdamsModel_analysis as MA
import cad_library
import CreateAdamsModel_cadPostProcessing as CP

# Global parameters - change if needed
globalParams = {"Analysis_File": "Analysis_MBD.xml",
                "CADMetrics_File": "CADAssembly_metrics.xml",
                "CADPostProc_File": "ComputedValues.xml",
                "AdamsExtension": "cmd",
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
                "GEN_Fixed": "Fixed",  # translation for joint type(s)
                "ADS_Fixed": "fix"}

GRAVITY = cad_library.Vec3([0.0, -9.81, 0.0])

# map CAD output joint names to Adams names
JOINTTYPES = {"REVOLUTE": "revolute",
              "PRISMATIC": "translational",
              "FIXED": "fixed",
              "CYLINDRICAL": "cylindrical"}

SCRIPTNAME = "CreateAdamsModel.py"
# Global id counters for joints/forces/motions and markers, respectively
idCounter = 1
idMarker = 1
idJoint = 1
args = None

def vectornorm(v):
    length = math.sqrt(sum([x ** 2 for x in v]))
    if length != 0:
        axis_unit = [x / length for x in v]
    else:
        axis_unit = [x for x in v]
    return axis_unit

class ProcessingError(Exception):
    def __init__(self, value):
        super(ProcessingError, self).__init__()
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
        self.result_fea_list = []
        self.component_dict = {}
        self.joint_dict = {}
        self.merge_list = []
        self.terrainfile = None
        self.contacts = []
        self.__gravity = None

    def get_name(self):
        return self.name

    def add_component(self, component):
        self.component_dict[component.get_componentId()] = component

    def get_component(self, componentid):
        if componentid in self.component_dict:
            return self.component_dict[componentid]
        else:
            return None

    def add_contact(self, contact):
        self.contacts.append(contact)

    def add_fea_load(self, fea):
        self.result_fea_list.append(fea)
        #if fea.get_componentID() in self.component_dict:
        #    self.component_dict[fea.get_componentID()].set_fea_flag()

    def add_gravity(self, gravity):
        self.__gravity = gravity

    def add_ground(self, ground):
        self.ground = ground
        # add to component dictionary such markers associated with the ground can be found
        self.component_dict[ground.get_componentId()] = ground

    def add_joint(self, joint):
        self.joint_dict[joint.get_id()] = joint

    def get_joint(self, jointid):
        if jointid in self.joint_dict:
            return self.joint_dict[jointid]
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

    def export_merge_2_adams(self):
        resultstr = ''
        for merge in self.merge_list:
            resultstr += merge.export2Adams()
        return resultstr

    def export_cad_2_adams(self):
        resultstr = '!\n'
        resultstr += '!----- Read STEP files for each body -----!\n!\n'
        for item in sorted(self.component_dict):
            if item != self.ground.get_componentId():
                resultstr += 'part create rigid_body name_and_position &\n'
                resultstr += '   part_name = ' + self.component_dict[item].get_uniquename() + ' &\n'
                resultstr += '   location = 0.0,0.0,0.0 \n!\n'
                resultstr += 'file geometry read &\n'
                resultstr += '   type_of_geometry = stp &\n'
                if self.component_dict[item].is_assembly():
                    extension = 'asm'
                else:
                    extension = 'prt'
                resultstr += '   file_name = "' + os.getcwd() + os.sep + 'AP203_E2_SINGLE_FILE' + os.sep + self.component_dict[item].get_ownPartname() + '_' + extension + '.stp" &\n'
                resultstr += '   part_name = ' + self.component_dict[item].get_uniquename() + ' \n\n'
                resultstr += 'part modify rigid_body name_and_position &\n'
                resultstr += '   part_name = ' + self.component_dict[item].get_uniquename() + ' &\n'
                trans = self.component_dict[item].get_globalmatrix().get_translation()
                resultstr += '   location = ' + str(trans[0]) + ',' + str(trans[1]) + ',' + str(trans[2]) + ' &\n'
                orient = self.component_dict[item].get_globalmatrix().to_euler313()
                resultstr += '   orientation = ' + str(orient[0]) + ',' + str(orient[1]) + ',' + str(orient[2]) + ' \n!\n'
        if self.terrainfile is not None:
            resultstr += '!\n'
            resultstr += '!----- Import terrain -----!\n!\n'
            resultstr += 'file parasolid read &\n'
            resultstr += '   file_name = "' + os.getcwd() + os.sep + self.terrainfile + '.x_t" &\n'
            resultstr += '   model_name = .' + self.name + '\n!\n'
            resultstr += 'part modify rigid_body name_and_position &\n'
            resultstr += '   part_name = .' + self.name + '.' + self.terrainfile + ' &\n'
            resultstr += '   new_part_name = .' + self.name + '.TERRAIN &\n'
            resultstr += '   location = 0.0, -1200.0, -1200.0 \n!\n'

        return resultstr

    def export_components_2_adams(self):
        resultstr = '!\n'
        resultstr += '!--------- Rigid parts ----------!\n!\n'
        resultstr += globalParams['Ground_Prefix']
        if self.ground is not None:
            resultstr += self.component_dict[self.ground.get_componentId()].export2Adams()
        for item in sorted(self.component_dict):
            if self.ground is None or item != self.ground.get_componentId():
                resultstr += self.component_dict[item].export2Adams(self.units.is_radian())
        return resultstr

    def export_fea_2_adams(self):
        resultstr = '!\n'
        resultstr += '!----- FEA load export -----!\n!\n'
        for item in self.result_fea_list:
            resultstr += item.export2Adams()
        return resultstr

    def export_joint_2_adams(self):
        resultstr = '!\n'
        resultstr += '!----- Joints -----!\n!\n'
        for item in sorted(self.joint_dict):
            resultstr += self.joint_dict[item].export2Adams()
        return resultstr

    def exportMaterial2Adams(self):
        resultstr = '!\n'
        resultstr += '!----- Materials -----!\n!\n'
        for item in self.material_dict:
            resultstr += self.material_dict[item].export2Adams()
        return resultstr

    def exportContact2Adams(self):
        resultstr = '!\n'
        resultstr += '!----- Contacts -----!\n!\n'
        for contact in self.contacts:
            resultstr += contact.export2Adams()
        return resultstr

    def exportMotion2Adams(self):
        resultstr = '!\n'
        resultstr += '!----- Motions -----!\n!\n'
        for item in self.motion_list:
            resultstr += item.export2Adams()
        return resultstr

    def exportName2Adams(self):
        resultstr = '!\n!\n'
        resultstr += '!----- Adams/View Model -----!\n!\n!\n'
        resultstr += 'model create &\n'
        resultstr += '   model_name = ' + self.name + '\n'
        resultstr += '!\nview erase\n!\n'
        return resultstr

    def export2AdamsPrefix(self):
        resultstr = '!\n'
        resultstr += '! File automatically generated by ' + SCRIPTNAME + ' script\n'
        resultstr += '!\n'
        return resultstr

    def export2AdamsSuffix(self):
        resultstr = '!\n'
        resultstr += '!----- Show model ----!\n!\n'
        resultstr += 'model display &\n'
        resultstr += '   model_name = ' + self.name + '\n!\n'
        return resultstr

    def export1stPass(self):
        resultstr = self.units.export2Adams()
        resultstr += self.exportName2Adams()
        resultstr += self.exportMaterial2Adams()
        resultstr += self.export_cad_2_adams()
        resultstr += self.export_merge_2_adams()
        resultstr += self.export_components_2_adams()
        for item in sorted(self.component_dict):
            if self.ground is None or item != self.ground.get_componentId():
                resultstr += self.component_dict[item].exportInfoRequest()
        return resultstr

    def export2Adams(self):
        resultstr = self.export2AdamsPrefix()
        resultstr += self.units.export2Adams()
        resultstr += self.exportName2Adams()
        resultstr += self.exportMaterial2Adams()
        resultstr += self.export_cad_2_adams()
        resultstr += self.export_merge_2_adams()
        resultstr += self.export_components_2_adams()
        resultstr += self.export_joint_2_adams()
        resultstr += self.exportContact2Adams()
        resultstr += self.exportMotion2Adams()
        resultstr += self.__gravity.export2Adams()
        resultstr += self.simulation.export2Adams()
        resultstr += self.export_fea_2_adams()
        resultstr += self.export2AdamsSuffix()
        return resultstr


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
        resultstr = '!\n'
        resultstr += '!----- Default Units for Model -----!\n!\n!\n'
        resultstr += 'defaults units &\n'
        resultstr += '   length = ' + self.length + ' &\n'
        resultstr += '   angle = ' + self.angle + ' &\n'
        resultstr += '   force = ' + self.force + ' &\n'
        resultstr += '   mass = ' + self.mass + ' &\n'
        resultstr += '   time = ' + self.time + ' &\n'
        resultstr += '   coordinate_system_type = ' + self.coordinateSystem + ' &\n'
        resultstr += '   orientation_type = ' + self.orientationType + '\n'
        resultstr += '!\n'
        return resultstr


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
        resultstr = '!\n'
        resultstr += 'material create &\n'
        resultstr += '   material_name = .' + self.modelname + '.' + self.name + ' &\n'
        resultstr += '   youngs_modulus = ' + str(self.young) + ' &\n'
        resultstr += '   poissons_ratio = ' + str(self.poisson) + ' &\n'
        resultstr += '   density = ' + str(self.density) + '\n!\n'
        return resultstr


class MBD_Marker(object):
    def __init__(self, counter, location, orientation, existing=False):
        # self.id = id   # = MetricID in the Assembly_MBD.xml
        self.__existing = existing
        self.adamsid = counter  # Adams will need a unique number for each marker
        self.location = location
        self.axis = orientation
        if self.axis is not None:
            self.axis.normalize().clean()
        else:
            self.axis = cad_library.Vec3.zunit()
        self.calc_orientation()

    def calc_orientation(self):
        # Perform only if constraint axis exists and is not a zero-vector
        # then compute orientation in Euler angles (313)
        # ##
        # Compute unit vector of given axis
        axis_unit = self.axis
        # Check whether axis is z-Axis or (-1)*z-Axis!
        if cmp(axis_unit, cad_library.Vec3.zunit()) == 0:
            self.orientation = [0.0, 0.0, 0.0]
            return
        elif cmp(axis_unit, [(-1) * x for x in cad_library.Vec3.zunit()]) == 0:
            self.orientation = [0.0, math.pi, 0.0]
            return
        # Compute the cross product of z-Axis and the given unit axis
        cross_product = cad_library.Vec3.zunit().crossprod(axis_unit)
        # Compute the unit vector of cross product -> rotation vector
        cross_sum = sum([x ** 2 for x in cross_product])
        rot_vector = [x / cross_sum for x in cross_product]
        # Compute dot product of z-Axis and the given unit axis
        dot_product = cad_library.Vec3.zunit().dotprod(axis_unit)
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
        self.orientation[0] = math.atan2((quat[0] * quat[2] + quat[1] * quat[3]),
                                         (quat[0] * quat[3] - quat[1] * quat[2]))
        self.orientation[1] = math.acos(quat[3] ** 2 - quat[0] ** 2 - quat[1] ** 2 + quat[2] ** 2)
        self.orientation[2] = math.atan2((quat[0] * quat[2] + quat[1] * quat[3]),
                                         (quat[0] * quat[3] - quat[1] * quat[2]))

        # xaxis = None
        # yaxis = None
        # zaxis = None
        # if cmp(self.axis,cad_library.Vec3.zunit()) or cmp(self.axis,cad_library.Vec3.zunit().mul(-1)):
        #     zaxis = self.axis
        #     xaxis = cad_library.Vec3.xunit()
        #     yaxis = cad_library.Vec3.yunit()
        # elif cmp(self.axis,cad_library.Vec3.xunit()) or cmp(self.axis,cad_library.Vec3.xunit().mul(-1)):
        #     zaxis = self.axis
        #     xaxis = cad_library.Vec3.zunit()
        #     yaxis = cad_library.Vec3.yunit()
        # elif cmp(self.axis,cad_library.Vec3.yunit()) or cmp(self.axis,cad_library.Vec3.yunit().mul(-1)):
        #     zaxis = self.axis
        #     xaxis = cad_library.Vec3.xunit()
        #     yaxis = cad_library.Vec3.zunit()
        # else:
        #     zaxis = self.axis
        #     tmp = cad_library.Vec3([self.axis[0], -self.axis[2], self.axis[1]])
        #     yaxis = self.axis.dotprod(tmp)
        #     xaxis = self.axis.dotprod(yaxis)
        #
        # mat = cad_library.Mat4x4([xaxis[0], xaxis[1], xaxis[2], 0.0, yaxis[0], yaxis[1], yaxis[2], 0.0, zaxis[0], zaxis[1], zaxis[2], 0.0, 0.0, 0.0, 0.0, 1.0])
        # return mat.to_euler313()

    def get_adamsid(self):
        return self.adamsid

    def get_id(self):
        return self.adamsid

    def check_zero_axis(self, axis):
        return cmp(axis, cad_library.Vec3.null())

    def get_name(self):
        return str(globalParams['Marker_Prefix']) + str(self.adamsid)

    def export2Adams(self, parentname, isRad=False):
        if self.__existing:
            return ''
        resultstr = '!\n'
        resultstr += 'marker create &\n'
        resultstr += '   marker_name = .' + parentname + '.' + self.get_name() + ' &\n'
        resultstr += '   location = ' + str(self.location[0]) + ', ' + str(self.location[1]) + ', ' + str(self.location[2]) + ' &\n'

        resultstr += '   orientation = ' + str(self.orientation[0]) + ', ' + str(self.orientation[1]) + ', ' + str(self.orientation[2]) + '\n'
        return resultstr

class MBD_Body:
    cadfilesmap = {}
    def __init__(self, modelname, componentID, cadmodelname, globalmatrix=None, existingbody=True, materialid=None, ground=False, readInfo=False, assembly=False):
        self.__componentID = componentID  # component id (cyphy id)
        self.__cadmodelname = cadmodelname  # name of the cad model file
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
        if self.__cadmodelname not in self.cadfilesmap.keys():
            self.cadfilesmap[self.__cadmodelname] = 0
        self.cadfilesmap[self.__cadmodelname] = self.cadfilesmap[self.__cadmodelname] + 1
        self.__uniquecounter = self.cadfilesmap[self.__cadmodelname]
        self.__globalmatrix = globalmatrix
        self.__assembly = assembly

    def merge_body(self, body):
        self.__mergedbodies.append(body)

    def get_componentId(self):
        if self.ground:
            return globalParams['GroundName']
        else:
            return self.__componentID

    def is_assembly(self):
        return self.__assembly

    def add_marker(self, marker):
        self.marker_dict[marker.get_id()] = marker

    def add_geometry(self, geom):
        self.__geometries.append(geom)

    def get_geometries(self):
        if self.__geometries is None:
            return []
        else:
            return self.__geometries

    def get_globalmatrix(self):
        return self.__globalmatrix

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
        return self.__cadmodelname

    def get_uniquename(self):
        if self.__uniquecounter == 1:
            return self.get_ownPartname()
        else:
            return self.get_ownPartname() + str(self.__uniquecounter)

    def get_fullname(self):
        return '.' + self.get_modelname() + '.' + self.get_ownPartname()

    def get_fea_marker_name(self):
        return '.' + self.__modelname + '.' + self.get_uniquename() + '.' + globalParams['FEA_Marker']

    def is_existing(self):
        return self.__existing

    def exportInfoRequest(self):
        resultstr = 'list_info part &\n'
        resultstr += '   part_name = .' + self.get_modelname() + '.' + self.get_uniquename() + ' &\n'
        resultstr += '   file_name = "' + self.get_uniquename() + '.nfo" &\n'
        resultstr += '   write_to_terminal = off &\n'
        resultstr += '   brief = off \n!\n'
        return resultstr

    def export2Adams(self, isRad=False):
        resultstr = '!\n'
        resultstr += '!----- ' + str(self.get_componentId()) + ' -----!\n!\n'
        resultstr += 'defaults coordinate_system &\n'
        resultstr += '   default_coordinate_system = .' + str(self.__modelname) + '.' + globalParams['GroundName'] + '\n'
        resultstr += '!\n'

        if not self.__existing:
            #resultstr += 'part create rigid_body name_and_position &\n'
            #resultstr += '   part_name = .' + self.get_modelname() + '.' + self.get_uniquename() + ' \n!\n'
            resultstr += 'part create rigid_body mass_properties &\n'
            resultstr += '   part_name = .' + self.get_modelname() + '.' + self.get_uniquename() + ' &\n'
            resultstr += '   material_type = .' + self.get_modelname() + '.' + globalParams['MaterialName'] + ' \n!\n'
            #resultstr += '   center_of_mass_marker = cm \n!\n'

        #for child in self.__mergedbodies:
            #if not child.is_existing:
                #raise ProcessingError(
                    #'Child of a merged body must be an existing part [' + child.get_ComponentInstanceID + '].')
            #resultstr += 'part create rigid_body mass_properties &\n'
            #resultstr += '   part_name = .' + child.get_modelname() + '.' + child.get_uniquename() + ' &\n'
            #resultstr += '   material_type = .' + child.get_modelname() + '.' + globalParams['MaterialName'] + ' \n!\n'

            #resultstr += 'part merge rigid_body &\n'
            #resultstr += '   part_name = .' + child.get_modelname() + '.' + child.get_uniquename() + ' &\n'
            #resultstr += '   into_part_name = .' + self.get_modelname() + '.' + self.get_uniquename() + '\n!\n'

        resultstr += 'part create rigid_body mass_properties &\n'
        resultstr += '   part_name = .' + self.get_modelname() + '.' + self.get_uniquename() + ' &\n'
        resultstr += '   material_type = .' + self.get_modelname() + '.' + globalParams['MaterialName'] + ' \n'

        for item in self.marker_dict:
            resultstr += self.marker_dict[item].export2Adams(self.__modelname + '.' + self.get_uniquename(), isRad)
        if not self.ground:
            resultstr += '!\n'
            resultstr += 'marker create &\n'
            resultstr += '   marker_name = ' + self.get_fea_marker_name() + ' &\n'
            resultstr += '   location = (LOC_RELATIVE_TO({0, 0, 0}, .' + self.__modelname + '.' + self.__cadmodelname + '.cm)) &\n'
            resultstr += '   orientation = (ORI_RELATIVE_TO({0d,0d,0d}, .' + self.__modelname + '.' + globalParams[
                'GroundName'] + '))\n'
        return resultstr

class MBD_Contact(object):
    def __init__(self, idstr, bodyI, bodyJ):
        self.__bodyI = bodyI
        self.__bodyJ = bodyJ
        self.id = idstr
    def export2Adams(self):
        geomJ = self.__bodyJ.get_geometries()[0]
        for geomI in self.__bodyI.get_geometries():
            resultstr = 'contact create &\n'
            resultstr += '   contact_name = test_' + str(self.id) + ' &\n'
            resultstr += '   i_geometry_name = ' + self.__bodyI.get_fullname() + '.' + geomI + ' &\n'
            resultstr += '   j_geometry_name = ' + self.__bodyJ.get_fullname() + '.' + geomJ + ' &\n'
            resultstr += '   stiffness = 1.0E+005 &\n'
            resultstr += '   damping = 10.0 &\n'
            resultstr += '   dmax = 0.11 &\n'
            resultstr += '   exponent = 2.2 &\n'
            resultstr += '   mu_static = 1.0 &\n'
            resultstr += '   mu_dynamic = 1.6 &\n'
            resultstr += '   coulomb_friction = dynamics_only &\n'
            resultstr += '   friction_transition_velocity = 1000.0 &\n'
            resultstr += '   stiction_transition_velocity = 100.0 \n!\n'
            return resultstr

class MBD_Joint(object):
    def __init__(self, jointid, idcount, modelname, jointtype, componentI, markerI, componentJ, markerJ):
        self.adamsid = idcount
        if jointid is None or len(jointid) is 0:
            jointid = str(globalParams['Joint_Prefix']) + str(self.adamsid)
        self.id = jointid
        self.modelname = modelname
        self.type = jointtype
        self.__bodyI = componentI
        self.markerI = markerI
        self.__bodyJ = componentJ
        self.markerJ = markerJ

    def get_id(self):
        return self.id

    def export2Adams(self):
        resultstr = '!\n'
        resultstr += 'constraint create joint ' + str(self.type).lower() + ' &\n'
        resultstr += '   joint_name = .' + self.modelname + '.' + str(self.id) + ' &\n'
        resultstr += '   adams_id = ' + str(self.adamsid) + ' &\n'
        resultstr += '   i_marker_name = .' + self.modelname + \
                   '.' + self.__bodyI.get_uniquename() + '.' + self.markerI.get_name() + ' &\n'
        resultstr += '   j_marker_name = .' + self.modelname + \
                   '.' + self.__bodyJ.get_uniquename() + '.' + self.markerJ.get_name() + ' \n'
        return resultstr


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
        resultstr = ''
        if self.active.lower() == 'on':
            resultstr += '!\n'
            resultstr += 'constraint create motion_generator &\n'
            resultstr += '   motion_name = .' + self.modelname + '.' + self.motionid + ' &\n'
            resultstr += '   adams_id = ' + str(self.adamsid) + ' &\n'
            resultstr += '   type_of_freedom = ' + str(self.freedom).lower() + ' &\n'
            resultstr += '   joint_name = .' + self.modelname + '.' + self.jointid + ' &\n'
            resultstr += '   time_derivative = ' + str(self.timederivative).lower() + ' &\n'
            resultstr += '   function = "' + self.function + '"\n'
            # resultstr += 'constraint attributes &\n'
            #resultstr += '   constraint_name = .'+self.modelname+'.'+self.motionid+' &\n'
            #resultstr += '   name_visibility = off\n'
        return resultstr


class MBD_Gravity(object):
    def __init__(self, gravityObject):
        self.active = globalParams['GravityActive']
        if gravityObject is None:
            self.x = GRAVITY[0]
            self.y = GRAVITY[1]
            self.z = GRAVITY[2]
        else:
            temp = cad_library.Vec3.parse(gravityObject.Value)
            self.x = temp[0]
            self.y = temp[1]
            self.z = temp[2]

    def export2Adams(self):
        resultstr = '!\n'
        resultstr += '!----- Gravity acceleration (Accgrav) -----!\n!\n!\n'
        if self.active.lower() == 'on':
            resultstr += 'force create body gravitational  &\n'
            resultstr += '   gravity_field_name = gravity  &\n'
            resultstr += '   x_component_gravity = ' + str(self.x) + ' &\n'
            resultstr += '   y_component_gravity = ' + str(self.y) + ' &\n'
            resultstr += '   z_component_gravity = ' + str(self.z) + '  \n'
            resultstr += '!\n'
        return resultstr


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
        resultstr = ''
        if self.__body is not None:
            resultstr += 'file fea_loads write &\n'
            resultstr += '   file_name = "' + os.getcwd() + os.sep + globalParams['FEA_Prefix'] + self.__body.get_componentId() + globalParams['FEA_Suffix'] + '" &\n'
            resultstr += '   format = ' + self.feaTool.upper() + ' &\n'
            resultstr += '   marker_name = ' + self.__body.get_fea_marker_name() + ' &\n'
            resultstr += '   analysis_name = ' + globalParams['AnalysisName'] + ' &\n'
            standard = 'no' if (self.feaType.lower() == globalParams['FEA_Type'].lower()) else 'yes'
            resultstr += '   no_inertia = ' + standard + '\n'
            resultstr += '!\n'
        return resultstr


class MBD_Merge(object):
    def __init__(self, mainmodel, modellist):
        self.main = mainmodel
        self.models = []
        for m in modellist:
            if m is not self.main:
                self.models.append(m)

    def export2Adams(self):
        resultstr = ''
        for m in self.models:
            resultstr += 'part merge rigid_body &\n'
            resultstr += '   part_name = .' + m.get_modelname() + '.' + m.get_partname() + ' &\n'
            resultstr += '   into_part_name = .' + self.main.get_modelname() + '.' + self.main.get_partname() + '\n'
            resultstr += '!\n'
        return resultstr


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
        resultstr = '!\n!\n'
        resultstr += '!----- Simulation Scripts -----!\n!\n!\n'
        resultstr += 'simulation script create &\n'
        resultstr += '   sim_script_name = .' + self.modelname + '.' + globalParams['SimScriptName'] + ' &\n'
        resultstr += '   commands = "simulation single_run transient type=auto_select initial_static=no '
        resultstr += 'end_time=' + str(self.time) + ' number_of_steps=' + str(
            self.steps) + ' model_name=.' + self.modelname + '", &\n'
        resultstr += '              "simulation single_run save analysis_name=' + globalParams['AnalysisName'] + '"\n\n'
        resultstr += 'simulation single scripted &\n'
        resultstr += '   sim_script_name = .' + self.modelname + '.' + globalParams['SimScriptName'] + ' &\n'
        resultstr += '   reset_before_and_after = yes\n'
        resultstr += '!\n'
        return resultstr

class CADPostProc:
    def __init__(self, cadpostproc):
        self.ground = None
        for comp in cadpostproc.get_Component():
            for metric in comp.get_Metrics().get_Metric():
                if metric.get_MetricID() == 'Anchor':
                    self.ground = metric.get_ArrayValue()

    def get_Ground(self):
        return self.ground


def setup_model(analysis, asminfo, cadpostproc, firstpass):
    global idCounter
    global idMarker
    global idJoint
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
    postproc = CADPostProc(cadpostproc)

    fixedsets = []

    # Setup bodies
    processedcomps = []
    # Do assemblies first
    for compid in asminfo.componentsdict.keys():
        if compid == asminfo.root.cyphyid:
            continue
        comp = asminfo.componentsdict[compid]
        if comp.is_assembly():
            processedcomps.append(compid)
            if comp.kinematic:
                # Create bodies for all children (but not for the assembly)
                for child in comp.children:
                    mod.add_component(MBD_Body(analysis.get_Name(), child.cyphyid, child.cadmodelname, child.get_global_matrix(), True, readInfo=not firstpass))
                    processedcomps.append(child.cyphyid)
            else:
                # Create a body for the assembly and add children as sub-parts (parts to be merged)
                added_comp = MBD_Body(analysis.get_Name(), compid, comp.cadmodelname, comp.get_global_matrix(), False, readInfo=not firstpass, assembly=True)
                for child in comp.children:
                    added_comp.merge_body(MBD_Body(analysis.get_Name(), child.cyphyid, child.cadmodelname, child.get_global_matrix(), True, assembly=False))
                    processedcomps.append(child.cyphyid)
                mod.add_component(added_comp)

    # Whatever remains and not has been processed yet
    for compid in asminfo.componentsdict.keys():
        if compid == asminfo.root.cyphyid:
            continue
        comp = asminfo.componentsdict[compid]
        if compid not in processedcomps:
            mod.add_component(MBD_Body(analysis.get_Name(), comp.cyphyid, comp.cadmodelname, comp.get_global_matrix(), True, readInfo=not firstpass, assembly=False))

    # Setup joints
    for joint in asminfo.joints:
        for constrainedtocomp in joint.compto:
            added_instance_id = constrainedtocomp.cyphyid
            assembly_instance_id = joint.compfrom.cyphyid
            # Not interested in constraints to the top assembly (there's no top assembly in Adams)
            if added_instance_id == asminfo.root.cyphyid or assembly_instance_id == asminfo.root.cyphyid:
                continue
            if asminfo.componentsdict[added_instance_id].is_assembly() and asminfo.componentsdict[added_instance_id].kinematic:
                print "Skipping joint " + joint.id + ", added component is a kinematic assembly: " + added_instance_id
                continue
            if asminfo.componentsdict[added_instance_id].is_assembly() and asminfo.componentsdict[added_instance_id].kinematic:
                print "Skipping joint " + joint.id + ", assembled component is a kinematic assembly: " + assembly_instance_id
                continue
            component_added = mod.get_component(added_instance_id)
            if component_added is None:
                added_instance_id = asminfo.componentsdict[added_instance_id].parent.cyphyid
                if added_instance_id == asminfo.root.cyphyid:
                    continue
                component_added = mod.get_component(added_instance_id)
            if component_added is None:
                print "Component is not found, but referred to in a joint: " + added_instance_id
                return None
            component_assembly = mod.get_component(assembly_instance_id)
            if component_assembly is None:
                assembly_instance_id = asminfo.componentsdict[assembly_instance_id].parent.cyphyid
                if assembly_instance_id == asminfo.root.cyphyid:
                    continue
                component_assembly = mod.get_component(assembly_instance_id)
            if component_assembly is None:
                print "Component is not found, but referred to in a joint: " + assembly_instance_id
                return None
            asm_marker = MBD_Marker(idMarker, joint.location, joint.orientation)
            component_assembly.add_marker(asm_marker)
            idMarker += 1
            added_marker = MBD_Marker(idMarker, joint.location, joint.orientation)
            component_added.add_marker(added_marker)
            idMarker += 1
            if joint.type in JOINTTYPES:
                mod.add_joint(MBD_Joint(joint.id, idJoint, analysis.get_Name(), JOINTTYPES[joint.type],
                                        component_assembly, asm_marker, component_added, added_marker))
                idJoint += 1
            else:
                print "Invalid joint type: " + joint.type + ". Please correct this error."
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
        terrain_marker = MBD_Marker(idMarker, cad_library.Vec3.null(), None)
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
        groundpos = cad_library.Vec3.null()
    else:
        groundpos = cad_library.Vec3.parse(postproc.get_Ground())

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
        ground_added_marker = MBD_Marker(idMarker, groundpos, None)
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
        if k != "ground":
            mod.add_fea_load(MBD_FEA_Load(v, None))

    return mod


def AdamsExport(model, firstpass, issilent=True):
    cwd = os.getcwd()
    filename = cwd + str(os.sep) + "adams_output" + '.' + globalParams['AdamsExtension']
    with open(filename, "w") as resultfile:
        if firstpass:
            resultfile.write(model.export1stPass())
        else:
            resultfile.write(model.export2Adams())
    if not issilent:
        print model.export2Adams()
    return

def main():
    global args
    parser = argparse.ArgumentParser(description='Creates an Adams .cmd file to build up the kinematic model.')
    parser.add_argument('-terrain')
    parser.add_argument('-firstpass')
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

    asminfo = cad_library.AssemblyInfo()

    try:
        analysis = MA.parse(os.getcwd() + os.sep + globalParams["Analysis_File"], True)
    except MA.GDSParseError as exc:
        print "Unable to parse " + globalParams["Analysis_File"] + ", exiting. Error message: " + exc.message
        exit(-1)

    try:
        asminfo.read_metrics_file(os.getcwd() + os.sep + globalParams["CADMetrics_File"])
    except Exception as exc:
        print "Unable to parse " + globalParams["CADMetrics_File"] + ", exiting. Error message: " + exc.message
        exit(-1)

    #cad_library.ComponentData.dump_hierarchy(asminfo.root)

    try:
        cadpostproc = CP.parse(os.getcwd() + os.sep + globalParams["CADPostProc_File"], True)
    except CP.GDSParseError as exc:
        print "Unable to parse " + globalParams["CADPostProc_File"] + ", exiting. Error message: " + exc.message
        exit(-1)

    firstpass = False

    if args.firstpass:
        firstpass = True

    model = setup_model(analysis, asminfo, cadpostproc, firstpass)

    try:
        if model is None:
            print "Unable to setup model, exiting."
            exit(-1)

        AdamsExport(model, firstpass, True)
    except ProcessingError as e:
        print "Processing error: " + e.message
        exit(-1)

    print "adams_output" + '.' + globalParams['AdamsExtension'] + " has been succesfully generated."
    #except:
    #    print "Unexpected exception during script running."
    #    exit(-1)


if __name__ == '__main__':
    main()
