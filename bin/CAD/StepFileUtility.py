# Author: Di Yao (dyao@isis.vanderbilt.edu)
# Name: StepFileUtility.py
# Description: Contains classes and functions to parse a STEP file and create Coordinate System objects.
# Version: 0.1 - first draft
# Date: 11/15/2012



import os
import sys
import logging
import re
import decimal

ENTITY_DEFINITION_RE = re.compile("#(\d+)[^\S\n]?=[^\S\n]?(.*?)\((.*)\)[^\S\n]?;[\\r]?$")



class SimpleStepParser:
    def __init__(self, filename):
        self._filename = filename
        self._entity_map = {}
        self._edge_curve_list = []
        self._vertex_point_list = []
        self._cartesian_point_list = []
        self._line_list = []

    def parse_file(self):
        import Utils
        import string
        global module_logger       

        with open(self._filename, 'r') as f:
            for line in f:
                match_entity_line = ENTITY_DEFINITION_RE.search(line)
                if match_entity_line:
                    instance_id, entity_name, entity_attrs = match_entity_line.groups()
                    instance_id = '#' + instance_id
                    entity_name = entity_name.strip(' \t\n\r')
                    entity_attrs_list, str_len = Utils.process_nested_parent_str(entity_attrs)
                    self._entity_map[instance_id] = (entity_name,entity_attrs_list)
                    if entity_name == 'EDGE_CURVE':
                        self._edge_curve_list.append(instance_id)
                    elif entity_name == 'VERTEX_POINT':
                        self._vertex_point_list.append(instance_id)
                    elif entity_name == 'LINE':
                        self._line_list.append(instance_id)
                    elif entity_name == 'CARTESIAN_POINT':
                        self._cartesian_point_list.append(instance_id)

        
        #print self._entity_map, '\n'
        #print len(self._edge_curve_list)
        #print len(self._vertex_point_list) 
        #print len(self._line_list)
        #print len(self._cartesian_point_list)


class PopulateCoordinateInfo:
    def __init__ (self, parser):
        self._step_parser = parser
        self._coordinate_systems = {}
        self._logger = logging.getLogger('root.PopulateCoordinateInfo')


    def create_coordinate_vector(self):
        self._step_parser.parse_file()
        x_coordinate_vectors_list = {}              #{length, [CoordinateVector]} length=60
        y_coordinate_vectors_list = {}              #{length, [CoordinateVector]} length=45
        z_coordinate_vectors_list = {}              #{length, [CoordinateVector]} length=30
        coordinate_system_map = {}

        # create coordinate axis vectors, add them to 1 of 3 maps
        for edge_id in self._step_parser._edge_curve_list:
            if self._step_parser._entity_map.has_key(edge_id):                  #is edge_curve
                edge_type_id = self._step_parser._entity_map[edge_id][1][3]
                if edge_type_id in self._step_parser._line_list:                #edge_curve refers to a line
                    #self._logger.debug('%s' %edge_id)
                    point_1 = self.find_cartesian_point(self._step_parser._entity_map[edge_id][1][1])
                    point_2 = self.find_cartesian_point(self._step_parser._entity_map[edge_id][1][2])
                    distance = DistanceBTPoints(point_1, point_2)
                    if distance is not None:
                        distance_int, distance_frac = self.analyze_length(distance)
                        #self._logger.debug('%f %f %f ' %(distance, distance_int,distance_frac))
                        if distance_int == 60 and distance_frac > 0:
                            x_coordinate_vector = CoordinateVector(point_1, point_2, distance_int, distance_frac)
                            if distance_frac not in x_coordinate_vectors_list:
                                x_coordinate_vectors_list[distance_frac] = []
                            x_coordinate_vectors_list[distance_frac].append(x_coordinate_vector)
                        elif distance_int == 45 and distance_frac > 0:
                            y_coordinate_vector = CoordinateVector(point_1, point_2, distance_int, distance_frac)
                            if distance_frac not in y_coordinate_vectors_list:
                                y_coordinate_vectors_list[distance_frac] = []
                            y_coordinate_vectors_list[distance_frac].append(y_coordinate_vector)
                        elif distance_int == 30 and distance_frac > 0:
                            z_coordinate_vector = CoordinateVector(point_1, point_2, distance_int, distance_frac)
                            if distance_frac not in z_coordinate_vectors_list:
                                z_coordinate_vectors_list[distance_frac] = []
                            z_coordinate_vectors_list[distance_frac].append(z_coordinate_vector)
        #print '%d' %len(x_coordinate_vectors_list)
        #print '%d' %len(y_coordinate_vectors_list)
        #print '%d' %len(z_coordinate_vectors_list)                    

        #go through the 3 coordinate axis vectors and group them to create coordinate systems
        for id, xvectors in x_coordinate_vectors_list.items():
            for xvector_item in xvectors:
                if id in y_coordinate_vectors_list:
                    for yvector_item in y_coordinate_vectors_list[id]:
                        if SameCoordinateSystem(xvector_item, yvector_item) == True:
                            if id in z_coordinate_vectors_list:
                                for zvector_item in z_coordinate_vectors_list[id]:
                                    if SameCoordinateSystem(xvector_item, zvector_item) == True:
                                        #self._logger.debug('Found xyz vectors for CoordinateSystem [ID = %s]' %id)
                                        if id in coordinate_system_map:
                                            module_logger.debug('CoordinateSystem [ID = %s] already exist' %id)
                                        else:
                                            xvector_item.compute_vector()
                                            yvector_item.compute_vector()
                                            zvector_item.compute_vector()
                                            coordinate_system_map[id] = CoordinateSystem(id, xvector_item, yvector_item, zvector_item)

        for key, value in coordinate_system_map.items():
            self._logger.debug(str(value))
        return coordinate_system_map    


    def find_cartesian_point(self, id):
        cartesian_point = []
        if id in self._step_parser._vertex_point_list:      #is a vertex point
            point_id = self._step_parser._entity_map[id][1][1]
            if self._step_parser._entity_map[point_id][0] == 'CARTESIAN_POINT':
                #print self._step_parser._entity_map[point_id][1][1]
                tmp = self._step_parser._entity_map[point_id][1][1]
                for i in range(0, len(tmp)):
                    cartesian_point.append(float(tmp[i]))
        return cartesian_point

    def analyze_length(self, length):
        length_str = "%0.19f" % length
        pos= length_str.find('.')
        key = int(length_str[0:pos])
        frac_str = length_str[pos::]
        frac_str = '0' + frac_str
        frac = float(frac_str)*1000
        error = abs(round(frac) - frac)
        if error < 0.1:
            return key, round(frac)
        else:
            return key, 0

def DistanceBTPoints(point_1, point_2):                 #in string
    import math    
    distance = None
    if len(point_1) == len(point_2):
        sum = 0
        for i in range(0, len(point_1)):
            sum+=pow(decimal.Decimal(point_1[i]) - decimal.Decimal(point_2[i]), 2)        #convert to decimal before calculation
        distance = decimal.Decimal(math.sqrt(sum))
        
    return distance

def SameCoordinateSystem(coordinate_vector_1, coordinate_vector_2):
    status = False
    if coordinate_vector_1._origin[:] == coordinate_vector_2._origin[:]:
        status = True
    elif coordinate_vector_1._origin[:] == coordinate_vector_2._point[:]:
        status = True
        tmp = coordinate_vector_2._point
        coordinate_vector_2._point = coordinate_vector_2._origin
        coordinate_vector_2._origin = tmp
    elif coordinate_vector_1._point[:] == coordinate_vector_2._origin[:]:
        status = True
        tmp = coordinate_vector_1._point
        coordinate_vector_1._point = coordinate_vector_1._origin
        coordinate_vector_1._origin = tmp
    elif coordinate_vector_1._point[:] == coordinate_vector_2._point[:]:
        status = True
        tmp = coordinate_vector_1._point
        coordinate_vector_1._point = coordinate_vector_1._origin
        coordinate_vector_1._origin = tmp
        tmp = coordinate_vector_2._point
        coordinate_vector_2._point = coordinate_vector_2._origin
        coordinate_vector_2._origin = tmp	

    return status


class CoordinateVector:
    def __init__(self, point_1, point_2, length, length_frac):
        self._origin = point_1
        self._point = point_2
        self._vector = []
        self._length = length
        self._length_frac = length_frac
        self._processed = False

    def __str__(self):
        doc_string = '(CoordVector [%d,%f] origin=%s, point=%s)' %(self._length, self._length_frac, str(self._origin), str(self._point))
        return doc_string

    def compute_vector(self):
        self._vector.append(decimal.Decimal(self._point[0] - self._origin[0]))
        self._vector.append(decimal.Decimal(self._point[1] - self._origin[1]))
        self._vector.append(decimal.Decimal(self._point[2] - self._origin[2]))
        
class CoordinateSystem:
    def __init__(self, id):
        self._cid = id
        self._x = None
        self._y = None
        self._z = None
        
    def __init__(self, id, x, y, z):
        self._cid = id
        self._x = x
        self._y = y
        self._z = z

    def __str__(self):
        doc_string = 'CoordinateSystem[%d] X=%s, Y=%s, Z=%s' %(self._cid, str(self._x), str(self._y), str(self._z))
        return doc_string

if __name__ == '__main__':
    logger = logging.getLogger('root.StepFileUtility')
    if (len(sys.argv) > 1):
        file_name, file_extension = os.path.splitext(sys.argv[1])
        if (file_extension.lower() != '.step'):
            logger.debug('Given input file must be a step file: {0}'.format(sys.argv[1]))
            sys.exit(0)
        if not os.path.exists(sys.argv[1]):
            logger.debug('Given input step file does not exist: {0}'.format(sys.argv[1]))
            sys.exit(0)
        
        # file exists
        simple_parser = SimpleStepParser(sys.argv[1])
        populator = PopulateCoordinateInfo(simple_parser)
        populator.create_coordinate_vector()
    else:
        logger.debug('Must provide an input step file.')
    

