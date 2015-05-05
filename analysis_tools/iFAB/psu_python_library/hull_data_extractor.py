""" 
hull_data_extractor.py

Matthew Woodruff (mjw5407@arl.psu.edu)
The Pennsylvania State University
Applied Research Laboratory
2013

Find surface area and perimeter of each hull plate
given a step model for the hull
"""

"""
1)  Find the SHAPE_DEFINITION_REPRESENTATION for the soapbox
2)  Get the edge loops and the actual edges that compose them
3)  Do geometry
    a.  Compute perimeter from the edges
    b.  Make a triangulation from each edge loop to compute surface area
4)  Export results to a hull cost model input file. 

- Scan the text of the `DATA` section for a `PRODUCT` that includes `SOAPBOX` in its name.
- Work up-tree from the `PRODUCT` to the `PRODUCT_DEFINITION` to the `PRODUCT_DEFINITION_SHAPE`, to the `SHAPE_DEFINITION_REPRESENTATION`
- Search down-tree from the `SHAPE_DEFINITION_REPRESENTATION` to find `ADVANCED_FACE`.  Those are the hull plates.  Make a list of them.
- Determine surface area and perimeter for each plate.
"""

import os.path
import argparse
import re
import copy
import math
import json
import sys

class HullDataExtractionError(Exception): pass

def lines_are_step_data(lines):
    """
    raise a HullDataExtractionError if the text doesn't look like
    a valid step file.
    """
    expect_in_order = ["ISO-10303-21;",
                       "HEADER;",
                       "ENDSEC;",
                       "DATA;",
                       "ENDSEC;",
                       "END-ISO-10303-21;"]
    for line in lines:
        if len(expect_in_order) == 0:
            break
        if expect_in_order[0] == line:
            expect_in_order.pop(0)

    if len(expect_in_order) > 0:
        msg = "{0} not found where expected".format(
                expect_in_order[0])
        raise HullDataExtractionError(msg)

def unify(lines):
    """
    put one statement on each line
    """
    results = []
    result = []
    for line in lines:
        result.append(line)
        if ";" in line:
            results.append("".join(result))
            result = []
    return results

def identity_of(lines):
    """ extract identity of each line """
    rex = re.compile("^#[0-9]+")
    for line in lines:
        match = rex.search(line)
        if match is None:
            yield None
        else:
            yield line[match.start():match.end()]

def children_of(lines):
    """ extract children from each line """
    args  = re.compile("\(.*\)")
    child = re.compile("#[0-9]+")

    for line in lines:
        arguments = args.search(line)
        if arguments is None:
            yield []
        else:
            arguments = arguments.group()[1:-1].split(",")
            children = [m.group() for m 
                        in (child.search(a) for a in arguments)
                        if m is not None]
            yield children


class StepModel(object):
    """ a Step Model """
    def __init__(self, text):
        """
        extract model information from a step file (text)

        Implementation detail: using lists with a common index for 
        all data, plus a dict to associate identities with indices.
        """
        lines = text.split("\n")
        lines = unify(lines)
        lines_are_step_data(lines)
        self.lines = lines
        self.identities = list(identity_of(self.lines))
        self.indices = dict(((self.identities[i], i) 
                              for i in xrange(len(self.identities))
                              if self.identities[i] is not None))
        self.children = list(children_of(self.lines))
        self.parents = {}
        self._populate_parents()
        self._products = None

    def products(self):
        """
        return all PRODUCT lines in the step data
        """
        if self._products is not None:
            return self._products

        pattern = re.compile("^#[0-9]+ *= *PRODUCT\(")
        self._products = [self.identities[i] 
                          for i in xrange(len(self.identities))
                          if pattern.search(self.lines[i]) is not None]
        return self._products

    def _populate_parents(self):
        """ populate the parents dict based on self.children """
        for ii in xrange(len(self.children)):
            ident = self.identities[ii]
            for child in self.children[ii]:
                parents = self.parents.get(child, [])
                parents.append(ident)
                self.parents[child] = parents

    def find_up_tree(self, ident, astring):
        """ search up the tree for matching (grand)parents """
        visited = list()
        return self._find_up_tree(ident, astring, visited)

    def _find_up_tree(self, ident, astring, visited):
        if ident in visited: #detect cyclic recursion
            return []
        else:
            visited.append(ident)

        parents = self.parents.get(ident, [])
        matching_parents = []
        nonmatching_parents = []
        for parent in parents:
            if astring in self.lines[self.indices[parent]]:
                matching_parents.append(parent)
            else:
                nonmatching_parents.append(parent)

        for parent in nonmatching_parents:
            matching_parents.extend(self._find_up_tree(parent, astring,visited))

        return matching_parents

    def find_down_tree(self, ident, astring):
        """ search down the tree for matching descendants """
        children = self.children[self.indices[ident]]
        matching_children = []
        nonmatching_children = []
        for child in children:
            if astring in self.lines[self.indices[child]]:
                matching_children.append(child)
            else:
                nonmatching_children.append(child)

        for child in nonmatching_children:
            matching_children.extend(self.find_down_tree(child, astring))

        return matching_children

    def geometric_abstract(self, faces):
        # convert to face objects
        newfaces = []
        for face in faces:
            try:
                newfaces.append(Face(self, face))
            except HullDataExtractionError as hdee:
                # edges don't meet up or something
                sys.stderr.write("\nWARNING: ")
                sys.stderr.write(str(hdee))
                sys.stderr.write("\n")
                continue

        flattened = []
        for face in newfaces:
            flattened.append(face.transformed())

        edges = [face.edges for face in newfaces]
        flattened_edges = [[[edge[0][0:2], edge[1][0:2]]
                             for edge in face.edges]
                           for face in flattened]
        perimeters = [face.perimeter() for face in flattened]
        areas = [face.area() for face in flattened]
        normals = [face.normal for face in flattened]

        records = []
        for rec in zip(edges, flattened_edges, perimeters, areas, normals):
            records.append({"edges": rec[0],
                            "flattened_edges": rec[1],
                            "perimeter": rec[2],
                            "area": rec[3],
                            "normal": rec[4]})
        return records

def extract_soapbox(products):
    """
    from among the products, figure out which one is the soapbox
    """
    pattern = re.compile("soapbox", flags=re.I)
    boxes = [line for line in products 
             if pattern.search(line) is not None]
    if len(boxes) == 0:
        return products[0]
    return boxes[0]

def difference(v1, v2):
    """ compute the difference between two vectors """
    return [x1 - x2 for x1, x2 in zip(v1, v2)]

def crossproduct(v1, v2):
    """
    compute cross product of two triples
    """
    msg = "Cross product: Vector {0} does not have length 3."
    if len(v1) != 3:
        raise HullDataExtractionError(msg.format(v1))
    if len(v2) != 3:
        raise HullDataExtractionError(msg.format(v2))

    return [v1[1]*v2[2] - v2[1]*v1[2] ,
            v1[2]*v2[0] - v2[2]*v1[0] ,
            v1[0]*v2[1] - v2[0]*v1[1] ]

def normalize(vv):
    norm = math.sqrt(sum([x**2 for x in vv]))
    if norm > 0:
        normed = [x / norm for x in vv]
        normed = [max(-1.0, x) for x in normed]
        normed = [min(1.0, x) for x in normed]
        return normed
    raise HullDataExtractionError("Can't normalize zero vector")

def mmult(m1, m2):
    """
    specifically for 4x4 affine matrices
    """
    rows = []
    for ii in range(4):
        row = []
        for jj in range(4):
            row.append(sum([a * b for a, b in 
                       zip(m1[ii], [r[jj] for r in m2])]))
        rows.append(row)
    return rows

def mvmult(mm, vv):
    """
    a matrix by a vector
    """
    result = []
    for ii in range(4):
        result.append(sum([a * b for a, b in zip(mm[ii], vv)]))
    return result

def extract_point(line):
    """
    convert a cartesian point line to a list [x,y,z] of floats
    """
    # For example:
    # #16=CARTESIAN_POINT('',(0.E0,0.E0,0.E0));
    # find the second arg and strip its parens, then split that by comma
    args = re.search("\((.*)\)", line).groups()[0]
    args = args.split(",", 1)[1][1:-1]
    args = args.split(",")
    if len(args) != 3:
        msg = "Not a CARTESIAN_POINT:\n{0}".format(line)
        raise HullDataExtractionError(msg)
    return [float(re.sub("[()]", "", arg)) for arg in args]


class Face(object):
    def __init__(self, model, ident):
        """
        convert step data to face
        """
        self.ident = ident # the ADVANCED_FACE

        # find edges
        edges = []
        for edge in model.find_down_tree(ident, "EDGE_CURVE("):
            points = []
            for vertex in model.find_down_tree(edge, "VERTEX_POINT"):
                point = model.children[model.indices[vertex]][0]
                point = extract_point(model.lines[model.indices[point]])
                points.append(point)
            edges.append(points)
        oriented_edges = []
        oriented_edges.append(edges.pop())
        last_edge = oriented_edges[0]
        while len(edges) > 0:
            counter = 0
            while counter < len(edges):
                first = zip(last_edge[1], edges[counter][0])
                error0 = sum([abs(a-b) for a, b in first])
                second = zip(last_edge[1], edges[counter][1])
                error1 = sum([abs(a-b) for a, b in second])

                if error0 < 1e-9:
                    last_edge = edges[counter]
                    break
                if error1 < 1e-9:
                    last_edge = [edges[counter][1], edges[counter][0]]
                    break
                counter += 1
                
            if error0 > 1e-9 and error1 > 1e-9:
                msg = "Edges don't meet up!\nFace {0}\nOriented Edges {2}"\
                      "\nEdges {1}".format(
                        model.lines[model.indices[ident]], 
                        "\n".join([str(e) for e in edges]), 
                        "\n".join([str(e) for e in oriented_edges]))
                raise HullDataExtractionError(msg)

            oriented_edges.append(last_edge)
            edges.pop(counter)

        self.edges = oriented_edges

        self.plane = {}
        try:
            plane = model.find_down_tree(ident, "PLANE(")[0]
#        except IndexError:
#            raise HullDataExtractionError("Face {0} has no reference plane".format(ident))
#        try:
            point = model.find_down_tree(plane, "CARTESIAN_POINT(")[0]
#        except IndexError:
#            raise HullDataExtractionError("Plane {0} has no reference point".format(plane))
            point = extract_point(model.lines[model.indices[point]])
            directions = [extract_point(model.lines[model.indices[direct]])
                          for direct 
                          in model.find_down_tree(plane, "DIRECTION(")]
        except IndexError:
            # need to compute reference plane
            point = self.edges[0][0] # any point on the plane is fine
            # can take the cross product of any two edges
            dir1 = difference(self.edges[0][0], self.edges[0][1])
            dir2 = difference(self.edges[1][0], self.edges[1][1])
            normal = normalize(crossproduct(dir1, dir2))
            directions = [normal, dir1]

        self.plane["point"] = point
        self.plane["directions"] = directions
        self.normal = self.plane["directions"][0]
        self.plane["normal"] = self.normal
    
    def transformed(self):
        """
        transform the face s.t. it is on the x-y plane
        Working in homogeneous coordinates
        """
        nx, ny, nz = self.normal
        # how much to rotate about x to get n into xz plane
        thetax = 0.5 * math.pi - math.acos(ny)
        # how much to rotate about y to get n into yz plane
        thetay = 0.5 * math.pi - math.acos(nx) 

        rotx = [[1, 0, 0, 0],
                [0, math.cos(thetax), -math.sin(thetax), 0],
                [0, math.sin(thetax), math.cos(thetax), 0],
                [0, 0, 0, 1]]
        roty = [[math.cos(thetay), 0, -math.sin(thetay), 0],
                [0, 1, 0, 0],
                [math.sin(thetay), 0, math.cos(thetay), 0],
                [0, 0, 0, 1]]
        px, py, pz = self.plane["point"]
        tran = [[1, 0, 0, -px],
                [0, 1, 0, -py],
                [0, 0, 1, -pz],
                [0, 0, 0, 1]]
        rotation = mmult(rotx, roty)

        transform = mmult(rotation, tran )

        other = copy.copy(self)
        other.edges = []
        for edge in self.edges:
            other.edges.append([mvmult(transform, p+[1])[0:3] 
                                for p in edge])

        return other

    def perimeter(self):
        """
        assume that the face is actually in the xy plane.
        if not, it's the perimeter of the projection of the
        face onto the xy plane
        """
        perimeter = 0
        for edge in self.edges:
            dx, dy, _,  = [b - a for a, b in zip(*edge)]
            length = math.sqrt(dx**2 + dy**2)
            perimeter += length

        return perimeter

    def area(self):
        """
        assume that the face is actually in the xy plane.
        if not, it's the area of the projection of the
        face onto the xy plane
        """
        area = 0
        for edge in self.edges:
            # Green's theorem via Wikipedia
            x1, y1, _ = edge[0]
            x2, y2, _ = edge[1]
            dx = x2 - x1
            dy = y2 - y1
            if abs(x2 - x1) < 1e-9:
                area += 0.5 * x1 * dy
            elif abs(y2 - y1) < 1e-9:
                area += -0.5 * y1 * dx
            else:
                mm = dy / dx
                bb = y1 - mm * x1
                area += 0.5*(0.5*y2**2/mm - bb*y2/mm)
                area += -0.5*(0.5*y1**2/mm - bb*y1/mm)
                area += -0.5*(0.5*mm*x2**2 + bb * x2)
                area += 0.5*(0.5*mm*x1**2 + bb * x1)

        return abs(area)



def extract(stepfile):
    filename = stepfile.name
    dirname = os.path.dirname(filename)
    text = stepfile.read()

    model = StepModel(text)
    products = model.products()
    if len(products) < 1:
        msg = "No PRODUCTs found"
        raise HullDataExtractionError(msg)

    # here's where we assume that all products in the hull are
    # hull plates.
    adrs = []
    for product in products:
        adr = model.find_up_tree(product, "APPLIED_DOCUMENT_REFERENCE")
        adrs.extend(adr)

    subdocs = []
    for adr in adrs:
        subdoc = model.find_down_tree(adr, "DOCUMENT_FILE")
        subdocs.extend(subdoc)

    report = []
    for subdoc in subdocs:
        match = re.search("\('(.*\.stp)'", model.lines[model.indices[subdoc]])
        if match is None:
            continue
        fn = match.groups()[0]
        fn = os.path.join(dirname, fn)
        try:
            with open(fn, 'r') as fp:
                report.extend(extract(fp))
        except IOError:
            msg = "WARNING: Could not find {0}, referenced by {1}\n"
            msg = msg.format(fn, filename)
            sys.stderr.write(msg)
            
    # here's where we also assume that all products in the hull are
    # hull plates.
    sdrs = []
    for product in products:
        sdr = model.find_up_tree(
                product, "SHAPE_DEFINITION_REPRESENTATION")
        sdrs.extend(sdr)


    plates = []
    for sdr in sdrs:
        faces = model.find_down_tree(sdr, "ADVANCED_FACE")
        records = model.geometric_abstract(faces)
        largestPlate = None
        for plate in records:
            if largestPlate is None or largestPlate["area"] < plate["area"]:
                largestPlate = plate
        if largestPlate is not None:
            plates.append(largestPlate)

    ################################################################
    #     ---------    CAUTION!  ----  DANGER!    ---------        #
    #     ---------    ASSUMING MATERIAL TYPE!    ---------        #
    # I AM HARD-CODING MATERIAL INFORMATION UNTIL I HAVE           #
    # A SOURCE FOR IT.  FIX THIS AS SOON AS DATA BECOME AVAILABLE! #
    #                               Matthew Woodruff 23 July 2013  #
    ################################################################

    # Here's where we assume it's not a soapbox
    for plate in plates:
        plate["source"] = filename
        plate["material"] = "MIL-DTL-12560"
        plate["thickness"] = 1.5

    report.extend(plates)

    return report

def summary(report):
    nplates = len(report)
    perimeter = sum([face["perimeter"] for face in report])
    area = sum([face["area"] for face in report])
    return (nplates, perimeter, area)


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("stepfile",
                        help="A STP file containing a hull soapbox",
                        type=argparse.FileType('r'))
    parser.add_argument("-s", "--summary",
                        help="Report totals rather than details",
                        action = "store_true")

    return parser.parse_args()

def cli():
    args = get_args()
    plates = extract(args.stepfile)
    if args.summary:
        nplates, perimeter, area = summary(plates)
        print "{0} plates\n"\
              "{1} mm perimeter\n"\
              "{2} square mm area".format(nplates, perimeter, area)
    else:
        print json.dumps(plates, indent=4)
        print len(plates)

if __name__ == "__main__":
    cli()
    

# vim:ts=4:sw=4:expandtab:fdm=indent:wrap lbr:ai:colorcolumn=74
