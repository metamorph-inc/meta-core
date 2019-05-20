#!/usr/bin/env python
from __future__ import absolute_import
from six.moves import range

__author__ = 'Zsolt Lattmann'
__copyright__ = 'Copyright (C) 2013 Vanderbilt University'
__license__ = """
Copyright (C) 2013 Vanderbilt University

Permission is hereby granted, free of charge, to any person obtaining a
copy of this data, including any software or models in source or binary
form, as well as any drawings, specifications, and documentation
(collectively "the Data"), to deal in the Data without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Data, and to
permit persons to whom the Data is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Data.

THE DATA IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS, SPONSORS, DEVELOPERS, CONTRIBUTORS, OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE DATA OR THE USE OR OTHER DEALINGS IN THE DATA.
"""
__status__ = "Prototype"
__maintainer__ = "https://openmodelica.org"

import os
import re
import math
import uuid
import json
import logging
import sys
from optparse import OptionParser

import svgwrite
import OMPython

# OpenModelica setup commands
OMC_SETUP_COMMANDS = ['setCommandLineOptions("+d=nogen,noevalfunc")']

# Bitmap
#   extends GraphicItem
#   extent
#   fileName
#   imageSource

regex_equal_key_value = re.compile("([^ =]+) *= *(\"[^\"]*\"|[^ ]*)")
regex_points = re.compile("{([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}")

regex_type_value = re.compile("(\w+.\w+)*")

# Compile regular expressions ONLY once!
# example: {-100.0,-100.0,100.0,100.0,true,0.16,2.0,2.0, {...
regex_coordSys = re.compile(
    '([+-]?\d+(?:.\d+)?),([+-]?\d+(?:.\d+)?),([+-]?\d+(?:.\d+)?),([+-]?\d+(?:.\d+)?),(\w+),([+-]?\d+(?:.\d+)?),([+-]?\d+(?:.\d+)?),([+-]?\d+(?:.\d+)?),')

# example: Rectangle(true, {35.0, 10.0}, 0, {0, 0, 0}, {255, 255, 255}, LinePattern.Solid, FillPattern.Solid, 0.25, BorderPattern.None, {{-15.0, -4.0}, {15.0, 4.0}}, 0
regex_rectangle = re.compile(
    'Rectangle\((\w+), {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, ([+-]?\d+(?:.\d+)?), {(\d+), (\d+), (\d+)}, {(\d+), (\d+), (\d+)}, (\w+.\w+), (\w+.\w+), ([+-]?\d+(?:.\d+)?), (\w+.\w+), {{([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}}, ([+-]?\d+(?:.\d+)?)')

# example: Line(true, {0.0, 0.0}, 0, {{-30, -120}, {-10, -100}}, {0, 0, 0}, LinePattern.Solid, 0.25, {Arrow.None, Arrow.None}, 3, Smooth.None
regex_line = re.compile(
    'Line\((\w+), {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, ([+-]?\d+(?:.\d+)?), ({{[+-]?\d+(?:.\d+)?, [+-]?\d+(?:.\d+)?}(?:, {[+-]?\d+(?:.\d+)?, [+-]?\d+(?:.\d+)?})*}), {(\d+), (\d+), (\d+)}, (\w+.\w+), ([+-]?\d+(?:.\d+)?), {(\w+.\w+), (\w+.\w+)}, ([+-]?\d+(?:.\d+)?), (\w+.\w+)')

# example: Ellipse(true, {0.0, 0.0}, 0, {0, 0, 0}, {95, 95, 95}, LinePattern.Solid, FillPattern.Solid, 0.25, {{-100, 100}, {100, -100}}, 0, 360)}}
regex_ellipse = re.compile(
    'Ellipse\((\w+), {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, ([+-]?\d+(?:.\d+)?), {(\d+), (\d+), (\d+)}, {(\d+), (\d+), (\d+)}, (\w+.\w+), (\w+.\w+), ([+-]?\d+(?:.\d+)?), {{([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}}, ([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)')

# example: Text(true, {0.0, 0.0}, 0, {0, 0, 255}, {0, 0, 0}, LinePattern.Solid, FillPattern.None, 0.25, {{-150, 110}, {150, 70}}, "%name", 0, TextAlignment.Center
regex_text = re.compile(
    'Text\((\w+), {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, ([+-]?\d+(?:.\d+)?), {(\d+), (\d+), (\d+)}, {(\d+), (\d+), (\d+)}, (\w+.\w+), (\w+.\w+), ([+-]?\d+(?:.\d+)?), {{([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}}, ("[^"]*"), ([+-]?\d+(?:.\d+)?)(?:, ("[^"]*"))?(?:, {([^}]*)})?, (\w+.\w+)')

# example: Polygon(true, {0.0, 0.0}, 0, {0, 127, 255}, {0, 127, 255}, LinePattern.Solid, FillPattern.Solid, 0.25, {{-24, -34}, {-82, 40}, {-72, 46}, {-14, -26}, {-24, -34}}, Smooth.None
regex_polygon = re.compile(
    'Polygon\((\w+), {([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}, ([+-]?\d+(?:.\d+)?), {(\d+), (\d+), (\d+)}, {(\d+), (\d+), (\d+)}, (\w+.\w+), (\w+.\w+), ([+-]?\d+(?:.\d+)?), ({{[+-]?\d+(?:.\d+)?, [+-]?\d+(?:.\d+)?}(?:, {[+-]?\d+(?:.\d+)?, [+-]?\d+(?:.\d+)?})*}), (\w+.\w+)')

# example: {{-100.0, -100.0}, {-100.0, -30.0}, {0.0, -30.0}, {0.0, 0.0}}
regex_points = re.compile('{([+-]?\d+(?:.\d+)?), ([+-]?\d+(?:.\d+)?)}')

# example: Bitmap(true, {0.0, 0.0}, 0, {{-98, 98}, {98, -98}}, "modelica://Modelica/Resources/Images/Mechanics/MultiBody/Visualizers/TorusIcon.png"
# TODO: where is the imageSource?

# def __ask_omc(question, opt=None, parsed=True):
#     p = (question, opt, parsed)
#     if p in omc_cache:
#         return omc_cache[p]
#
#     if opt:
#         expression = question + '(' + opt + ')'
#     else:
#         expression = question
#
#     logger.debug('ask_omc: {0}  - parsed: {1}'.format(expression, parsed))
#
#     try:
#         if parsed:
#             res = OMPython.execute(expression)
#         else:
#             res = OMPython.omc.sendExpression(expression)
#     except Exception as e:
#         logger.error("OMC failed: {0}, {1}, parsed={2}".format(question, opt, parsed))
#         raise e
#
#     omc_cache[p] = res
#
#     return res


omc_cache = {}
graphics_cache = {}


class IconExporter(object):
    def __init__(self, omc_session, icon_dir_name):
        """
        Creates a new instance of IconExporter and passes in an OMCSession
        """

        self.logger = logging.getLogger('py_modelica_exporter.IconExporter')
        self.logger.setLevel(logging.NOTSET)

        self.logger.info('Initializing IconExporter()')

        # start om session
        self.omc = omc_session

        self.icon_dir_name = icon_dir_name

    # get graphics objects from annotation Icon
    def get_graphics_for_class(self, modelica_class):

        # TODO: does not work if a port (same class) is being used multiple times...
        # if modelicaClass in graphics_cache:
        #     return graphics_cache[modelicaClass]

        result = dict()
        result['graphics'] = []

        # answer2 = ask_omc('getIconAnnotation', modelicaClass, parsed=False)
        icon_annotation = self.omc.getIconAnnotation(modelica_class)

        result['coordinateSystem'] = {}
        result['coordinateSystem']['extent'] = [[-100, -100], [100, 100]]

        r = regex_coordSys.search(icon_annotation)

        if r:
            g = r.groups()
            result['coordinateSystem']['extent'] = [[float(g[0]), float(g[1])], [float(g[2]), float(g[3])]]
            result['coordinateSystem']['preserveAspectRatio'] = bool(g[4])
            result['coordinateSystem']['initialScale'] = float(g[5])
            result['coordinateSystem']['grid'] = [float(g[6]), float(g[7])]

            without_coord_sys = icon_annotation[icon_annotation.find(',{'):]
        else:
            # logger.warning('Coordinate system was skipped')
            # logger.warning(answer2)
            without_coord_sys = icon_annotation

        for icon_line in without_coord_sys.split('),'):

            # default values
            graphics_obj = {}

            r = regex_line.search(icon_line)
            if r:
                graphics_obj['type'] = 'Line'
                g = r.groups()
                graphics_obj['visible'] = g[0]
                graphics_obj['origin'] = [float(g[1]), float(g[2])]
                graphics_obj['rotation'] = float(g[3])

                points = []
                gg = re.findall(regex_points, g[4])
                for i in range(0, len(gg)):
                    points.append([float(gg[i][0]), float(gg[i][1])])
                graphics_obj['points'] = points

                graphics_obj['color'] = [int(g[5]), int(g[6]), int(g[7])]
                graphics_obj['pattern'] = g[8]
                graphics_obj['thickness'] = float(g[9])
                graphics_obj['arrow'] = [g[10], g[11]]
                graphics_obj['arrowSize'] = float(g[12])
                graphics_obj['smooth'] = g[13]

            r = regex_rectangle.search(icon_line)
            if r:
                graphics_obj['type'] = 'Rectangle'
                g = r.groups()
                graphics_obj['visible'] = g[0]
                graphics_obj['origin'] = [float(g[1]), float(g[2])]
                graphics_obj['rotation'] = float(g[3])
                graphics_obj['lineColor'] = [int(g[4]), int(g[5]), int(g[6])]
                graphics_obj['fillColor'] = [int(g[7]), int(g[8]), int(g[9])]
                graphics_obj['linePattern'] = g[10]
                graphics_obj['fillPattern'] = g[11]
                graphics_obj['lineThickness'] = float(g[12])
                graphics_obj['borderPattern'] = g[13]
                graphics_obj['extent'] = [[float(g[14]), float(g[15])], [float(g[16]), float(g[17])]]
                graphics_obj['radius'] = float(g[18])

            r = regex_polygon.search(icon_line)
            if r:
                graphics_obj['icon_line'] = icon_line
                graphics_obj['type'] = 'Polygon'
                g = r.groups()
                graphics_obj['visible'] = g[0]
                graphics_obj['origin'] = [float(g[1]), float(g[2])]
                graphics_obj['rotation'] = float(g[3])
                graphics_obj['lineColor'] = [int(g[4]), int(g[5]), int(g[6])]
                graphics_obj['fillColor'] = [int(g[7]), int(g[8]), int(g[9])]
                graphics_obj['linePattern'] = g[10]
                graphics_obj['fillPattern'] = g[11]
                graphics_obj['lineThickness'] = float(g[12])

                points = []
                gg = re.findall(regex_points, g[13])
                for i in range(0, len(gg)):
                    points.append([float(gg[i][0]), float(gg[i][1])])
                graphics_obj['points'] = points

                min_x = 100
                min_y = 100
                max_x = -100
                max_y = -100

                for point in graphics_obj['points']:
                    if min_x > point[0]:
                        min_x = point[0]
                    if max_x < point[0]:
                        max_x = point[0]
                    if min_y > point[1]:
                        min_y = point[1]
                    if max_y < point[1]:
                        max_y = point[1]

                graphics_obj['extent'] = [[min_x, min_y], [max_x, max_y]]

                graphics_obj['smooth'] = g[14]

            r = regex_text.search(icon_line)
            if r:
                graphics_obj['type'] = 'Text'
                g = r.groups()
                graphics_obj['visible'] = g[0]
                graphics_obj['origin'] = [float(g[1]), float(g[2])]
                graphics_obj['rotation'] = float(g[3])
                graphics_obj['lineColor'] = [int(g[4]), int(g[5]), int(g[6])]
                graphics_obj['fillColor'] = [int(g[7]), int(g[8]), int(g[9])]
                graphics_obj['linePattern'] = g[10]
                graphics_obj['fillPattern'] = g[11]
                graphics_obj['lineThickness'] = float(g[12])
                graphics_obj['extent'] = [[float(g[13]), float(g[14])], [float(g[15]), float(g[16])]]
                graphics_obj['textString'] = g[17].strip('"')
                graphics_obj['fontSize'] = float(g[18])
                graphics_obj['fontName'] = g[19]
                if graphics_obj['fontName']:
                    graphics_obj['fontName'] = graphics_obj['fontName'].strip('"')

                graphics_obj['textStyle'] = []
                if g[20]:
                    # text Style can have different number of styles
                    graphics_obj['textStyle'] = regex_type_value.findall(g[20])

                graphics_obj['horizontalAlignment'] = g[21]

            r = regex_ellipse.search(icon_line)
            if r:
                g = r.groups()
                graphics_obj['type'] = 'Ellipse'
                graphics_obj['visible'] = g[0]
                graphics_obj['origin'] = [float(g[1]), float(g[2])]
                graphics_obj['rotation'] = float(g[3])
                graphics_obj['lineColor'] = [int(g[4]), int(g[5]), int(g[6])]
                graphics_obj['fillColor'] = [int(g[7]), int(g[8]), int(g[9])]
                graphics_obj['linePattern'] = g[10]
                graphics_obj['fillPattern'] = g[11]
                graphics_obj['lineThickness'] = float(g[12])
                graphics_obj['extent'] = [[float(g[13]), float(g[14])], [float(g[15]), float(g[16])]]
                graphics_obj['startAngle'] = float(g[17])
                graphics_obj['endAngle'] = float(g[18])

            if not 'type' in graphics_obj:
                graphics_obj['type'] = 'Unknown'
                # logger.error('Unknown graphicsObj: {0}'.format(icon_line))

            result['graphics'].append(graphics_obj)

        graphics_cache[modelica_class] = result

        return result

    def get_graphics_with_ports_for_class(self, modelica_class):
        graphics = self.get_graphics_for_class(modelica_class)
        graphics['className'] = modelica_class
        graphics['ports'] = []
        # answer_full = ask_omc('getComponents', modelicaClass, parsed=False)
        answer_full = self.omc.getComponents(modelica_class, parsed=False)

        comp_id = 0
        for answer in answer_full[2:].split('},{'):
            #print answer
            comp_id += 1
            class_name = answer[0:answer.find(',')]
            component_name = answer[answer.find(',') + 1:][0:answer[answer.find(',') + 1:].find(',')]

            # if ask_omc('isConnector', class_name):
            if self.omc.isConnector(class_name):
                try:
                    comp_annotation = self.omc.getNthComponentAnnotation(modelica_class, comp_id)
                    # comp_annotation = ask_omc(
                    #     'getNthComponentAnnotation', modelicaClass + ', ' + str(comp_id))['SET2']['Set1']
                except KeyError as ex:
                    self.logger.error(
                        'KeyError: {0} componentName: {1} {2}'.format(modelica_class, component_name, ex.message))
                    continue

                # base class graphics for ports
                g_base = []
                base_classes = []
                self.get_base_classes(class_name, base_classes)

                for base_class in base_classes:
                    graphics_base = self.get_graphics_for_class(base_class)
                    g_base.append(graphics_base)

                g = self.get_graphics_for_class(class_name)

                g_this = g['graphics']

                g['graphics'] = []
                for g_b in g_base:
                    for g_i in g_b['graphics']:
                        g['graphics'].append(g_i)
                for g_b in g_this:
                    g['graphics'].append(g_b)

                g['id'] = component_name
                g['className'] = class_name

                desc = self.omc.getComponentComment(modelica_class + ', ' + component_name)
                # desc = ask_omc('getComponentComment', modelicaClass + ', ' + component_name)

                if type(desc) is dict:
                    g['desc'] = ''
                else:
                    g['desc'] = desc.strip().strip('"')

                g['classDesc'] = self.omc.getClassComment(class_name).strip().strip('"')
                # g['classDesc'] = ask_omc('getClassComment', class_name).strip().strip('"')

                min_x = g['coordinateSystem']['extent'][0][0]
                min_y = g['coordinateSystem']['extent'][0][1]
                max_x = g['coordinateSystem']['extent'][1][0]
                max_y = g['coordinateSystem']['extent'][1][1]

                for gs in g['graphics']:
                    # use default values if it is not there
                    if not 'extent' in gs:
                        gs['extent'] = [[-100, -100], [100, 100]]

                    if not 'origin' in gs:
                        gs['origin'] = [0, 0]

                    if min_x > gs['extent'][0][0] + gs['origin'][0]:
                        min_x = gs['extent'][0][0] + gs['origin'][0]
                    if min_x > gs['extent'][1][0] + gs['origin'][0]:
                        min_x = gs['extent'][1][0] + gs['origin'][0]
                    if min_y > gs['extent'][0][1] + gs['origin'][1]:
                        min_y = gs['extent'][0][1] + gs['origin'][1]
                    if min_y > gs['extent'][1][1] + gs['origin'][1]:
                        min_y = gs['extent'][1][1] + gs['origin'][1]
                    if max_x < gs['extent'][1][0] + gs['origin'][0]:
                        max_x = gs['extent'][1][0] + gs['origin'][0]
                    if max_x < gs['extent'][0][0] + gs['origin'][0]:
                        max_x = gs['extent'][0][0] + gs['origin'][0]
                    if max_y < gs['extent'][1][1] + gs['origin'][1]:
                        max_y = gs['extent'][1][1] + gs['origin'][1]
                    if max_y < gs['extent'][0][1] + gs['origin'][1]:
                        max_y = gs['extent'][0][1] + gs['origin'][1]

                g['coordinateSystem']['extent'] = [[min_x, min_y], [max_x, max_y]]

                #print comp_annotation
                index_delta = 7
                if comp_annotation[10] == "-":
                    # fallback to diagram annotations
                    index_delta = 0

                origin_x = comp_annotation[1 + index_delta]
                origin_y = comp_annotation[2 + index_delta]
                x0 = comp_annotation[3 + index_delta]
                y0 = comp_annotation[4 + index_delta]
                x1 = comp_annotation[5 + index_delta]
                y1 = comp_annotation[6 + index_delta]

                rotation = comp_annotation[7 + index_delta]

                g['transformation'] = {}
                g['transformation']['origin'] = [origin_x, origin_y]
                g['transformation']['extent'] = [[x0, y0], [x1, y1]]
                g['transformation']['rotation'] = rotation

                graphics['ports'].append(g)

        return graphics

    def get_gradient_colors(self, start_color, stop_color, mid_points):
        result = []

        start_red = int(start_color[0])
        start_green = int(start_color[1])
        start_blue = int(start_color[2])

        stop_red = int(stop_color[0])
        stop_green = int(stop_color[1])
        stop_blue = int(stop_color[2])

        r_delta = (stop_red - start_red) / (mid_points + 1)
        g_delta = (stop_green - start_green) / (mid_points + 1)
        b_delta = (stop_blue - start_blue) / (mid_points + 1)

        result.append((start_red, start_green, start_blue))

        for i in range(1, mid_points + 1):
            result.append((start_red + i * r_delta, start_green + i * g_delta, start_blue + i * b_delta))

        result.append((stop_red, stop_green, stop_blue))

        return result

    def get_coordinates(self, xy, graphics, min_x, max_y, transformation, coordinate_system):

        x = xy[0] + graphics['origin'][0]
        y = xy[1] + graphics['origin'][1]

        # rotation for the icon
        s = math.sin(graphics['rotation'] / 180 * 3.1415)
        c = math.cos(graphics['rotation'] / 180 * 3.1415)

        x -= graphics['origin'][0]
        y -= graphics['origin'][1]

        xnew = x * c - y * s
        ynew = x * s + y * c

        x = xnew + graphics['origin'][0]
        y = ynew + graphics['origin'][1]

        if transformation and coordinate_system:
            try:
                t_width = abs(max(transformation['extent'][1][0], transformation['extent'][0][0]) - min(
                    transformation['extent'][1][0], transformation['extent'][0][0]))
                t_height = abs(max(transformation['extent'][1][1], transformation['extent'][0][1]) - min(
                    transformation['extent'][1][1], transformation['extent'][0][1]))
                o_width = abs(max(coordinate_system['extent'][1][0], coordinate_system['extent'][0][0]) - min(
                    coordinate_system['extent'][1][1], coordinate_system['extent'][0][1]))
                o_height = abs(max(coordinate_system['extent'][1][1], coordinate_system['extent'][0][1]) - min(
                    coordinate_system['extent'][1][1], coordinate_system['extent'][0][1]))

                if 'extent' in transformation and transformation['extent'][1][0] < transformation['extent'][0][0]:
                    # horizontal flip
                    x = (-xy[0] + graphics['origin'][0]) / o_width * t_width + transformation['origin'][0] + \
                        transformation['extent'][1][0] + t_width / 2
                else:
                    x = (xy[0] + graphics['origin'][0]) / o_width * t_width + transformation['origin'][0] + \
                        transformation['extent'][0][0] + t_width / 2

                if 'extent' in transformation and transformation['extent'][1][1] < transformation['extent'][0][1]:
                    # vertical flip
                    y = (-xy[1] + graphics['origin'][1]) / o_height * t_height + transformation['origin'][1] + min(
                        transformation['extent'][1][1], transformation['extent'][0][1]) + t_height / 2
                else:
                    y = (xy[1] + graphics['origin'][1]) / o_height * t_height + transformation['origin'][1] + min(
                        transformation['extent'][0][1], transformation['extent'][0][1]) + t_height / 2

                s = math.sin(transformation['rotation'] / 180 * 3.1415)
                c = math.cos(transformation['rotation'] / 180 * 3.1415)

                x -= transformation['origin'][0]
                y -= transformation['origin'][1]

                xnew = x * c - y * s
                ynew = x * s + y * c

                x = xnew + transformation['origin'][0]
                y = ynew + transformation['origin'][1]

            except KeyError as ex:
                self.logger.error('Component position transformation failed: {0}', ex.message)
                self.logger.error(graphics)

        x -= min_x
        y = max_y - y

        return x, y

    # get svg object from modelica graphics object
    def get_svg_from_graphics(self, dwg, graphics, min_x, max_y, transformation=None, coordinate_system=None):
        shape = None
        definitions = svgwrite.container.Defs()
        origin = None

        if not 'origin' in graphics:
            graphics['origin'] = (0, 0)

        origin = graphics['origin']

        if graphics['type'] == 'Rectangle' or graphics['type'] == 'Ellipse' or graphics['type'] == 'Text':
            (x0, y0) = self.get_coordinates(graphics['extent'][0], graphics, min_x, max_y, transformation,
                                            coordinate_system)
            (x1, y1) = self.get_coordinates(graphics['extent'][1], graphics, min_x, max_y, transformation,
                                            coordinate_system)

            if graphics['type'] == 'Rectangle':
                shape = dwg.rect((min(x0, x1), min(y0, y1)), (abs(x1 - x0), abs(y1 - y0)), graphics['radius'],
                                 graphics['radius'])

        if graphics['type'] == 'Rectangle' or graphics['type'] == 'Ellipse' or graphics['type'] == 'Polygon':
            if not 'fillPattern' in graphics:
                graphics['fillPattern'] = 'FillPattern.None'

        elif graphics['type'] == 'Line':
            if 'points' in graphics:
                if graphics['smooth'] == 'Smooth.Bezier' and len(graphics['points']) > 2:
                    # TODO: Optimize this part!!!
                    shape = svgwrite.path.Path()
                    x_0, y_0 = self.get_coordinates([graphics['points'][0][0], graphics['points'][0][1]], graphics,
                                                    min_x, max_y, transformation, coordinate_system)
                    shape.push('M', x_0, y_0, 'C')

                    for i in range(1, len(graphics['points']) - 1):
                        x_0, y_0 = self.get_coordinates([graphics['points'][i - 1][0], graphics['points'][i - 1][1]],
                                                        graphics, min_x, max_y, transformation, coordinate_system)
                        x_1, y_1 = self.get_coordinates([graphics['points'][i][0], graphics['points'][i][1]], graphics,
                                                        min_x, max_y, transformation, coordinate_system)
                        x_2, y_2 = self.get_coordinates([graphics['points'][i + 1][0], graphics['points'][i + 1][1]],
                                                        graphics, min_x, max_y, transformation, coordinate_system)
                        x_01 = (x_1 + x_0) / 2
                        y_01 = (y_1 + y_0) / 2
                        x_12 = (x_2 + x_1) / 2
                        y_12 = (y_2 + y_1) / 2
                        shape.push(x_01, y_01, x_1, y_1, x_12, y_12)
                    x_n, y_n = self.get_coordinates([graphics['points'][len(graphics['points']) - 1][0],
                                                     graphics['points'][len(graphics['points']) - 1][1]], graphics,
                                                    min_x, max_y, transformation, coordinate_system)
                    shape.push(x_12, y_12, x_n, y_n, x_n, y_n)
                else:
                    shape = dwg.polyline(
                        [self.get_coordinates([x, y], graphics, min_x, max_y, transformation, coordinate_system) for
                         (x, y) in graphics['points']])
                shape.fill('none', opacity=0)

                # markers
                if graphics['arrow'][0] != 'Arrow.None':
                    url_id_start = graphics['arrow'][0] + '_start' + str(uuid.uuid4())
                    marker = svgwrite.container.Marker(insert=(10, 5), size=(4, 3), orient='auto', id=url_id_start,
                                                       viewBox="0 0 10 10")
                    p = svgwrite.path.Path(d="M 10 0 L 0 5 L 10 10 z")
                    p.fill("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")")
                    marker.add(p)
                    definitions.add(marker)
                    shape['marker-start'] = marker.get_funciri()

                if graphics['arrow'][1] != 'Arrow.None':
                    url_id_end = graphics['arrow'][1] + '_end' + str(uuid.uuid4())
                    marker = svgwrite.container.Marker(insert=(0, 5), size=(4, 3), orient='auto', id=url_id_end,
                                                       viewBox="0 0 10 10")
                    p = svgwrite.path.Path(d="M 0 0 L 10 5 L 0 10 z")
                    p.fill("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")")
                    marker.add(p)
                    definitions.add(marker)
                    shape['marker-end'] = marker.get_funciri()

            else:
                self.logger.error('Not handled: {0}'.format(graphics))
                return None

        elif graphics['type'] == 'Polygon':
            if 'points' in graphics:
                if graphics['smooth'] == 'Smooth.Bezier' and len(graphics['points']) > 2:
                    # TODO: Optimize this part!!!
                    shape = svgwrite.path.Path()
                    x_0, y_0 = self.get_coordinates([graphics['points'][0][0], graphics['points'][0][1]], graphics,
                                                    min_x, max_y, transformation, coordinate_system)
                    shape.push('M', x_0, y_0, 'C')

                    for i in range(1, len(graphics['points']) - 1):
                        x_0, y_0 = self.get_coordinates([graphics['points'][i - 1][0], graphics['points'][i - 1][1]],
                                                        graphics, min_x, max_y, transformation, coordinate_system)
                        x_1, y_1 = self.get_coordinates([graphics['points'][i][0], graphics['points'][i][1]], graphics,
                                                        min_x, max_y, transformation, coordinate_system)
                        x_2, y_2 = self.get_coordinates([graphics['points'][i + 1][0], graphics['points'][i + 1][1]],
                                                        graphics, min_x, max_y, transformation, coordinate_system)
                        x_01 = (x_1 + x_0) / 2
                        y_01 = (y_1 + y_0) / 2
                        x_12 = (x_2 + x_1) / 2
                        y_12 = (y_2 + y_1) / 2
                        shape.push(x_01, y_01, x_1, y_1, x_12, y_12)
                    x_n, y_n = self.get_coordinates([graphics['points'][len(graphics['points']) - 1][0],
                                                     graphics['points'][len(graphics['points']) - 1][1]], graphics,
                                                    min_x, max_y, transformation, coordinate_system)
                    shape.push(x_12, y_12, x_n, y_n, x_n, y_n)
                else:
                    shape = dwg.polygon(
                        [self.get_coordinates([x, y], graphics, min_x, max_y, transformation, coordinate_system) for
                         (x, y) in graphics['points']])
                shape.fill('none', opacity=0)
            else:
                self.logger.error('Not handled: {0}'.format(graphics))
                return None

        elif graphics['type'] == 'Ellipse':
            shape = dwg.ellipse(((x0 + x1) / 2, (y0 + y1) / 2), (abs((x1 - x0) / 2), abs((y1 - y0) / 2)))

        elif graphics['type'] == 'Text':

            extra = {}
            x = (x0 + x1) / 2
            y = (y0 + y1) / 2

            extra['font_family'] = graphics['fontName'] or "Verdana"

            if graphics['fontSize'] == 0:
                extra['font_size'] = "18"
            else:
                extra['font_size'] = graphics['fontSize']

            for style in graphics['textStyle']:
                if style == "TextStyle.Bold":
                    extra['font-weight'] = 'bold'
                elif style == "TextStyle.Italic":
                    extra['font-style'] = 'italic'
                elif style == "TextStyle.UnderLine":
                    extra['text-decoration'] = 'underline'

            extra['alignment_baseline'] = "middle"

            if graphics['horizontalAlignment'] == "TextAlignment.Left":
                extra['text_anchor'] = "start"
                if x0 < x1:
                    x = x0
                else:
                    x = x1
                if y0 < y1:
                    y = y0
                else:
                    y = y1
            elif graphics['horizontalAlignment'] == "TextAlignment.Center":
                extra['text_anchor'] = "middle"
            elif graphics['horizontalAlignment'] == "TextAlignment.Right":
                extra['text_anchor'] = "end"
                if x0 < x1:
                    x = x1
                else:
                    x = x0
                if y0 < y1:
                    y = y1
                else:
                    y = y0

            shape = dwg.text(graphics['textString'].replace('%', ''), None, [x], [y], **extra)

            if graphics['textString'].find('%') != -1:
                extra = {'class': "bbox", 'display': "none"}
                xmin = x0
                ymin = y0
                xmax = x1
                ymax = y1

                if x0 > x1:
                    xmin = x1
                    xmax = x0
                if y0 > y1:
                    ymin = y1
                    ymax = y0

                shape.add(svgwrite.text.TSpan(("{0} {1} {2} {3}".format(xmin, ymin, xmax, ymax)), **extra))
                extra = {'class': "data-bind", 'display': "none"}
                shape.add(svgwrite.text.TSpan(graphics['textString'], **extra))
        else:
            self.logger.error('Not handled: {0}'.format(graphics))
            return None

        dot_size = 4
        dash_size = 16
        space_size = 8

        if 'linePattern' in graphics:
            dot_size *= graphics['lineThickness']
            dash_size *= graphics['lineThickness']
            space_size *= graphics['lineThickness']

            if graphics['linePattern'] == 'LinePattern.None' or graphics['type'] == 'Text':
                pass
            elif graphics['linePattern'] == 'LinePattern.Solid':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")",
                             width='{0}mm'.format(graphics['lineThickness']))
            elif graphics['linePattern'] == 'LinePattern.Dash':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")",
                             width='{0}mm'.format(graphics['lineThickness']))
                shape.dasharray([dash_size, space_size])
            elif graphics['linePattern'] == 'LinePattern.Dot':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")",
                             width='{0}mm'.format(graphics['lineThickness']))
                shape.dasharray([dot_size, space_size])
            elif graphics['linePattern'] == 'LinePattern.DashDot':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")",
                             width='{0}mm'.format(graphics['lineThickness']))
                shape.dasharray([dash_size, space_size, dot_size, space_size])
            elif graphics['linePattern'] == 'LinePattern.DashDotDot':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")",
                             width='{0}mm'.format(graphics['lineThickness']))
                shape.dasharray([dash_size, space_size, dot_size, space_size, dot_size, space_size])

            if graphics['type'] == 'Rectangle':
                if graphics['borderPattern'] == 'BorderPattern.None':
                    pass
                elif graphics['borderPattern'] == 'BorderPattern.Raised':
                    url_id = graphics['borderPattern'] + '_' + str(uuid.uuid4())
                    shape['filter'] = 'url(#' + url_id + ')'

                    svg_filter = svgwrite.filters.Filter(id=url_id, filterUnits="objectBoundingBox", x="-0.1", y="-0.1",
                                                         width="1.2", height="1.2")
                    svg_filter.feGaussianBlur("SourceAlpha", stdDeviation="5", result="alpha_blur")
                    feSL = svg_filter.feSpecularLighting("alpha_blur", surfaceScale="5", specularConstant="1",
                                                         specularExponent="20", lighting_color="#FFFFFF",
                                                         result="spec_light")
                    feSL.fePointLight((-5000, -10000, 10000))
                    svg_filter.feComposite("spec_light", in2="SourceAlpha", operator="in", result="spec_light")
                    svg_filter.feComposite("SourceGraphic", in2="spec_light", operator="out", result="spec_light_fill")

                    definitions.add(svg_filter)
                elif graphics['borderPattern'] == 'BorderPattern.Sunken':
                    self.logger.warning('Not supported: {0}'.format(graphics['borderPattern']))
                elif graphics['borderPattern'] == 'BorderPattern.Engraved':
                    self.logger.warning('Not supported: {0}'.format(graphics['borderPattern']))

        if 'color' in graphics:
            try:
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")",
                             width='{0}mm'.format(graphics['thickness']))
            except TypeError as ex:
                self.logger.error('{0} {1}'.format(graphics['color'], ex.message))

        if 'pattern' in graphics:
            dot_size *= graphics['thickness']
            dash_size *= graphics['thickness']
            space_size *= graphics['thickness']

            if graphics['pattern'] == 'LinePattern.None' or graphics['type'] == 'Text':
                pass
            elif graphics['pattern'] == 'LinePattern.Solid':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")",
                             width='{0}mm'.format(graphics['thickness']))
            elif graphics['pattern'] == 'LinePattern.Dash':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")",
                             width='{0}mm'.format(graphics['thickness']))
                shape.dasharray([dash_size, space_size])
            elif graphics['pattern'] == 'LinePattern.Dot':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")",
                             width='{0}mm'.format(graphics['thickness']))
                shape.dasharray([dot_size, space_size])
            elif graphics['pattern'] == 'LinePattern.DashDot':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")",
                             width='{0}mm'.format(graphics['thickness']))
                shape.dasharray([dash_size, space_size, dot_size, space_size])
            elif graphics['pattern'] == 'LinePattern.DashDotDot':
                shape.stroke("rgb(" + ','.join([str(v) for v in graphics['color']]) + ")",
                             width='{0}mm'.format(graphics['thickness']))
                shape.dasharray([dash_size, space_size, dot_size, space_size, dot_size, space_size])

        if 'fillPattern' in graphics:
            if graphics['fillPattern'] == 'FillPattern.None':
                if graphics['type'] == 'Text':
                    shape.fill("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")")
                else:
                    shape.fill('none', opacity=0)
            elif graphics['fillPattern'] == 'FillPattern.Solid':
                shape.fill("rgb(" + ','.join([str(v) for v in graphics['fillColor']]) + ")")
            elif graphics['fillPattern'] == 'FillPattern.Horizontal':
                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                pattern = svgwrite.pattern.Pattern(id=url_id, insert=(0, 0), size=(5, 5), patternUnits='userSpaceOnUse')

                rect = svgwrite.shapes.Rect(insert=(0, 0), size=(5, 5))
                rect.fill("rgb(" + ','.join([str(v) for v in graphics['fillColor']]) + ")")
                pattern.add(rect)

                svg_path = svgwrite.path.Path(d="M0,0 L5,0")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=2)
                pattern.add(svg_path)

                definitions.add(pattern)

            elif graphics['fillPattern'] == 'FillPattern.Vertical':
                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                pattern = svgwrite.pattern.Pattern(id=url_id, insert=(0, 0), size=(5, 5), patternUnits='userSpaceOnUse')

                rect = svgwrite.shapes.Rect(insert=(0, 0), size=(5, 5))
                rect.fill("rgb(" + ','.join([str(v) for v in graphics['fillColor']]) + ")")
                pattern.add(rect)

                svg_path = svgwrite.path.Path(d="M0,0 L0,5")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=2)
                pattern.add(svg_path)

                definitions.add(pattern)

            elif graphics['fillPattern'] == 'FillPattern.Cross':
                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                pattern = svgwrite.pattern.Pattern(id=url_id, insert=(0, 0), size=(5, 5), patternUnits='userSpaceOnUse')

                rect = svgwrite.shapes.Rect(insert=(0, 0), size=(5, 5))
                rect.fill("rgb(" + ','.join([str(v) for v in graphics['fillColor']]) + ")")
                pattern.add(rect)

                svg_path = svgwrite.path.Path(d="M0,0 L5,0")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=2)
                pattern.add(svg_path)

                svg_path = svgwrite.path.Path(d="M0,0 L0,5")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=2)
                pattern.add(svg_path)

                definitions.add(pattern)

            elif graphics['fillPattern'] == 'FillPattern.Forward':
                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                pattern = svgwrite.pattern.Pattern(id=url_id, insert=(0, 0), size=(7, 7), patternUnits='userSpaceOnUse')

                rect = svgwrite.shapes.Rect(insert=(0, 0), size=(7, 7))
                rect.fill("rgb(" + ','.join([str(v) for v in graphics['fillColor']]) + ")")
                pattern.add(rect)

                svg_path = svgwrite.path.Path(d="M0,0 l7,7")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                svg_path = svgwrite.path.Path(d="M6,-1 l3,3")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                svg_path = svgwrite.path.Path(d="M-1,6 l3,3")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                definitions.add(pattern)

            elif graphics['fillPattern'] == 'FillPattern.Backward':
                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                pattern = svgwrite.pattern.Pattern(id=url_id, insert=(0, 0), size=(7, 7), patternUnits='userSpaceOnUse')

                rect = svgwrite.shapes.Rect(insert=(0, 0), size=(7, 7))
                rect.fill("rgb(" + ','.join([str(v) for v in graphics['fillColor']]) + ")")
                pattern.add(rect)

                svg_path = svgwrite.path.Path(d="M7,0 l-7,7")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                svg_path = svgwrite.path.Path(d="M1,-1 l-7,7")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                svg_path = svgwrite.path.Path(d="M8,6 l-7,7")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                definitions.add(pattern)

            elif graphics['fillPattern'] == 'FillPattern.CrossDiag':

                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                pattern = svgwrite.pattern.Pattern(id=url_id, insert=(0, 0), size=(8, 8), patternUnits='userSpaceOnUse')

                rect = svgwrite.shapes.Rect(insert=(0, 0), size=(8, 8))
                rect.fill("rgb(" + ','.join([str(v) for v in graphics['fillColor']]) + ")")
                pattern.add(rect)

                svg_path = svgwrite.path.Path(d="M0,0 l8,8")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                svg_path = svgwrite.path.Path(d="M8,0 l-8,8")
                svg_path.stroke("rgb(" + ','.join([str(v) for v in graphics['lineColor']]) + ")", width=1)
                pattern.add(svg_path)

                definitions.add(pattern)

            elif graphics['fillPattern'] == 'FillPattern.HorizontalCylinder':

                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                line_color = graphics['lineColor']
                fill_color = graphics['fillColor']

                if not line_color:
                    line_color = 'black'
                if not fill_color:
                    fill_color = 'white'

                gradient = svgwrite.gradients.LinearGradient(id=url_id, x1="0%", y1="0%", x2="0%", y2="100%")

                colors = self.get_gradient_colors(line_color, fill_color, 0)

                stop_values = [
                    (0, 0),
                    (0.3, 1),
                    (0.7, 1),
                    (1, 0)
                ]

                for (stop_value, idx) in stop_values:
                    gradient.add_stop_color(offset=stop_value,
                                            color='rgb({0}, {1}, {2})'.format(colors[idx][0], colors[idx][1],
                                                                              colors[idx][2]), opacity=1)

                definitions.add(gradient)

            elif graphics['fillPattern'] == 'FillPattern.VerticalCylinder':
                url_id = str(uuid.uuid4())
                shape.fill('url(#' + url_id + ')')

                line_color = graphics['lineColor']
                fill_color = graphics['fillColor']

                if not line_color:
                    line_color = 'black'
                if not fill_color:
                    fill_color = 'white'

                gradient = svgwrite.gradients.LinearGradient(id=url_id, x1="0%", y1="0%", x2="100%", y2="0%")

                colors = self.get_gradient_colors(line_color, fill_color, 0)

                stop_values = [
                    (0, 0),
                    (0.3, 1),
                    (0.7, 1),
                    (1, 0)
                ]

                for (stop_value, idx) in stop_values:
                    gradient.add_stop_color(offset=stop_value,
                                            color='rgb({0}, {1}, {2})'.format(colors[idx][0], colors[idx][1],
                                                                              colors[idx][2]), opacity=1)

                definitions.add(gradient)
            elif graphics['fillPattern'] == 'FillPattern.Sphere':
                if graphics['type'] == 'Ellipse':
                    url_id = str(uuid.uuid4())

                    shape.fill('url(#' + url_id + ')')

                    line_color = graphics['lineColor']
                    fill_color = graphics['fillColor']

                    if not line_color:
                        line_color = 'black'
                    if not fill_color:
                        fill_color = 'white'

                    gradient = svgwrite.gradients.RadialGradient(id=url_id, cx="50%", cy="50%", r="55%", fx="50%",
                                                                 fy="50%")
                    colors = self.get_gradient_colors(line_color, fill_color, 9)

                    stop_values = [
                        (0, 10),
                        (0.45, 8),
                        (0.7, 6),
                        (1, 0)
                    ]

                    for (stop_value, idx) in stop_values:
                        gradient.add_stop_color(offset=stop_value,
                                                color='rgb({0}, {1}, {2})'.format(colors[idx][0], colors[idx][1],
                                                                                  colors[idx][2]), opacity=1)

                    definitions.add(gradient)
                elif graphics['type'] == 'Rectangle':
                    url_id = str(uuid.uuid4())

                    shape.fill('url(#' + url_id + ')')

                    line_color = graphics['lineColor']
                    fill_color = graphics['fillColor']

                    if not line_color:
                        line_color = 'black'
                    if not fill_color:
                        fill_color = 'white'

                    gradient = svgwrite.gradients.RadialGradient(id=url_id, cx="50%", cy="50%", r="0.9", fx="50%",
                                                                 fy="50%")

                    colors = self.get_gradient_colors(line_color, fill_color, 0)

                    stop_values = [
                        (0, 1),
                        (1, 0)
                    ]

                    for (stop_value, idx) in stop_values:
                        gradient.add_stop_color(offset=stop_value,
                                                color='rgb({0}, {1}, {2})'.format(colors[idx][0], colors[idx][1],
                                                                                  colors[idx][2]), opacity=1)

                    definitions.add(gradient)
        else:
            shape.fill('none', opacity=0)

        return shape, definitions

    # generate svgs from graphics objects
    def generate_svg(self, filename, icon_graphics):

        width = 100
        height = 100

        min_x = 0
        min_y = 0
        max_x = 100
        max_y = 100

        for icon_graphic in icon_graphics:
            for graphics in icon_graphic['graphics']:
                if not 'origin' in graphics:
                    graphics['origin'] = (0, 0)

                if not 'extent' in graphics:
                    graphics['extent'] = [[-100, -100], [100, 100]]

                if 'extent' in graphics:
                    if min_x > graphics['extent'][0][0] + graphics['origin'][0]:
                        min_x = graphics['extent'][0][0] + graphics['origin'][0]
                    if min_x > graphics['extent'][1][0] + graphics['origin'][0]:
                        min_x = graphics['extent'][1][0] + graphics['origin'][0]
                    if min_y > graphics['extent'][0][1] + graphics['origin'][1]:
                        min_y = graphics['extent'][0][1] + graphics['origin'][1]
                    if min_y > graphics['extent'][1][1] + graphics['origin'][1]:
                        min_y = graphics['extent'][1][1] + graphics['origin'][1]
                    if max_x < graphics['extent'][1][0] + graphics['origin'][0]:
                        max_x = graphics['extent'][1][0] + graphics['origin'][0]
                    if max_x < graphics['extent'][0][0] + graphics['origin'][0]:
                        max_x = graphics['extent'][0][0] + graphics['origin'][0]
                    if max_y < graphics['extent'][1][1] + graphics['origin'][1]:
                        max_y = graphics['extent'][1][1] + graphics['origin'][1]
                    if max_y < graphics['extent'][0][1] + graphics['origin'][1]:
                        max_y = graphics['extent'][0][1] + graphics['origin'][1]

                if 'points' in graphics:
                    for point in graphics['points']:
                        if min_x > point[0] + graphics['origin'][0]:
                            min_x = point[0] + graphics['origin'][0]
                        if min_y > point[1] + graphics['origin'][1]:
                            min_y = point[1] + graphics['origin'][1]
                        if max_x < point[0] + graphics['origin'][0]:
                            max_x = point[0] + graphics['origin'][0]
                        if max_y < point[1] + graphics['origin'][1]:
                            max_y = point[1] + graphics['origin'][1]

                for port in icon_graphic['ports']:
                    if min_x > port['transformation']['extent'][0][0] + port['transformation']['origin'][0]:
                        min_x = port['transformation']['extent'][0][0] + port['transformation']['origin'][0]
                    if min_x > port['transformation']['extent'][1][0] + port['transformation']['origin'][0]:
                        min_x = port['transformation']['extent'][1][0] + port['transformation']['origin'][0]
                    if min_y > port['transformation']['extent'][0][1] + port['transformation']['origin'][1]:
                        min_y = port['transformation']['extent'][0][1] + port['transformation']['origin'][1]
                    if min_y > port['transformation']['extent'][1][1] + port['transformation']['origin'][1]:
                        min_y = port['transformation']['extent'][1][1] + port['transformation']['origin'][1]
                    if max_x < port['transformation']['extent'][1][0] + port['transformation']['origin'][0]:
                        max_x = port['transformation']['extent'][1][0] + port['transformation']['origin'][0]
                    if max_x < port['transformation']['extent'][0][0] + port['transformation']['origin'][0]:
                        max_x = port['transformation']['extent'][0][0] + port['transformation']['origin'][0]
                    if max_y < port['transformation']['extent'][1][1] + port['transformation']['origin'][1]:
                        max_y = port['transformation']['extent'][1][1] + port['transformation']['origin'][1]
                    if max_y < port['transformation']['extent'][0][1] + port['transformation']['origin'][1]:
                        max_y = port['transformation']['extent'][0][1] + port['transformation']['origin'][1]

        # ports can have borders
        min_x -= 5
        max_x += 5
        min_y -= 5
        max_y += 5

        width = max_x - min_x
        height = max_y - min_y

        dwg = svgwrite.Drawing(filename, size=(width, height), viewBox="0 0 " + str(width) + " " + str(height))
        dwg.add(svgwrite.base.Desc(icon_graphics[-1]['className']))

        for icon_graphic in icon_graphics:
            for graphics in icon_graphic['graphics']:
                shape_definitions = self.get_svg_from_graphics(dwg, graphics, min_x, max_y)
                if shape_definitions:
                    shape, definitions = shape_definitions
                    if isinstance(shape, svgwrite.text.Text) and shape.text == 'name':
                        shape.text = filename.split('.')[-2]
                    dwg.add(shape)
                    dwg.add(definitions)

        for icon_graphic in icon_graphics:
            for port in icon_graphic['ports']:
                group = dwg.g(id=port['id'])
                for graphics in port['graphics']:
                    svg_shape = self.get_svg_from_graphics(dwg, graphics, min_x, max_y, port['transformation'],
                                                           port['coordinateSystem'])
                    if svg_shape:
                        group.add(svg_shape[0])
                        group.add(svg_shape[1])

                port_info = dwg.g(id='info', display='none')
                port_info.add(svgwrite.text.Text(port['id'], id='name'))
                port_info.add(svgwrite.text.Text(port['className'], id='type'))
                port_info.add(svgwrite.text.Text(port['classDesc'], id='classDesc'))
                port_info.add(svgwrite.text.Text(port['desc'], id='desc'))

                group.add(port_info)

                dwg.add(group)

        dwg.save()

        return dwg

    def export_icon(self, modelica_class, dir_name=None):

        if dir_name is None:
            dir_name = self.icon_dir_name

        try:
            # get all icons
            icon_graphics = []
            base_classes = []
            self.get_base_classes(modelica_class, base_classes)

            for base_class in base_classes:
                graphics = self.get_graphics_with_ports_for_class(base_class)
                icon_graphics.append(graphics)
            graphics = self.get_graphics_with_ports_for_class(modelica_class)
            icon_graphics.append(graphics)

            # with open(os.path.join(output_dir, self.class_to_filename(modelica_class) + '.json'), 'w') as f_p:
            #     json.dump(iconGraphics, f_p)

            # export svgs
            svg_file_path = os.path.join(dir_name, self.class_to_filename(modelica_class) + ".svg")
            dwg = self.generate_svg(svg_file_path, icon_graphics)

            return svg_file_path
        except:
            return None

    def get_base_classes(self, modelica_class, base_classes):

        inheritance_cnt = self.omc.getInheritanceCount(modelica_class)
        # inheritance_cnt = ask_omc('getInheritanceCount', modelica_class)

        for i in range(1, inheritance_cnt + 1):
            base_class = self.omc.getNthInheritedClass(modelica_class, str(i))
            # base_class = ask_omc('getNthInheritedClass', modelica_class + ', ' + str(i))
            if base_class not in base_classes:
                base_classes.append(base_class)
                self.get_base_classes(base_class, base_classes)

    def class_to_filename(self, cl):
        """
      The file-system dislikes directory separators, and scripts dislike tokens that expand to other names.
      This function uses the same replacement rules as the OpenModelica documentation-generating script.
      """
        return cl.replace("/", "Division").replace("*", "Multiplication")


# def main():
#     parser = OptionParser()
#     parser.add_option("--with-html", help="Generate an HTML report with all SVG-files", action="store_true", dest="with_html", default=False)
#     parser.add_option("--output-dir", help="Directory to generate SVG-files in", type="string", dest="output_dir", default=os.path.abspath('ModelicaIcons'))
#     (options, args) = parser.parse_args()
#     if len(args) == 0:
#       parser.print_help()
#       return
#     global output_dir
#     output_dir = options.output_dir
#     with_html = options.with_html
#
#     # Inputs
#     PACKAGES_TO_LOAD = args
#     PACKAGES_TO_LOAD_FROM_FILE = []
#     PACKAGES_TO_GENERATE = PACKAGES_TO_LOAD
#
#
#     logger.info('Application started')
#     logger.info('Output directory: ' + output_dir)
#
#     if not os.path.exists(output_dir):
#         os.makedirs(output_dir)
#
#     success = True
#
#     for command in OMC_SETUP_COMMANDS:
#         print command,":",OMPython.omc.sendExpression(command)
#     for package in PACKAGES_TO_LOAD:
#         logger.info('Loading package: {0}'.format(package))
#         package_load = OMPython.execute('loadModel(' + package + ')')
#         logger.info('Load success: {0}'.format(package_load))
#         success = success and package_load
#
#     for package in PACKAGES_TO_LOAD_FROM_FILE:
#         logger.info('Loading package from file: {0}'.format(package))
#         package_load = OMPython.execute('loadFile("' + package + '")')
#         logger.info('Load success: {0}'.format(package_load))
#         success = success and package_load
#
#     if success:
#         dwgs = []
#
#         for package in PACKAGES_TO_GENERATE:
#             modelica_classes = ask_omc('getClassNames', package + ', recursive=true, qualified=true, sort=true')['SET1']['Set1']
#
#             for modelica_class in modelica_classes:
#                 logger.info('Exporting: ' + modelica_class)
#
#                 # try:
#                 base_classes = []
#                 getBaseClasses(modelica_class, base_classes)
#                 dwg = exportIcon(modelica_class, base_classes)
#                 dwgs.append(dwg)
#
#                 logger.info('Done: ' + modelica_class)
#                 # except:
#                 #     print 'FAILED: ' + modelica_class
#         if with_html:
#           logger.info('Generating HTML file ...')
#           with open(os.path.join(output_dir, 'index.html'), 'w') as f_p:
#               f_p.write('<html>\n')
#               f_p.write('<head>\n')
#               f_p.write('</head>\n')
#
#               f_p.write('<body>\n')
#
#               for dwg in dwgs:
#                   dwg.write(f_p)
#
#               f_p.write('</body>\n')
#               f_p.write('</html>\n')
#
#           logger.info('HTML file is ready.')
#         print "Generated svg's for %d models" % len(dwgs)
#
#     logger.info('quit OMC')
#     logger.info('End of application')
#
# if __name__ == '__main__':
#     main()
