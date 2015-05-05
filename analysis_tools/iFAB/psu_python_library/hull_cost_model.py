""" 
hull_cost_model.py
Matthew Woodruff (mjw5407@arl.psu.edu)
The Pennsylvania State University
Applied Research Laboratory
2013

Compute the cost and lead time for the hull based on physical 
description
"""
import copy
import argparse
import math
import json
import StringIO
from collections import namedtuple

class NoHeaderError(Exception):
    pass

def associate(table):
    """
    Convert a list-of-lists to a dict-of-dicts.
    """
    # stage 1: dict of lists
    stage1 = dict([[row[0], row[1:]] for row in table])
    header = stage1.get("header", None)
    if header is None:
        msg = "Need a header to make an associative table.\n"\
              "First row was {0}".format(table[0])
        raise NoHeaderError(msg)
    # stage 2: dict of dicts
    stage2 = {}
    for key in [key for key in stage1.iterkeys() if key != "header"]:
        stage2[key] = dict(zip(header, stage1[key]))
    return stage2


class CostModelError(Exception):
    pass

class HullCostAndTime(object):
    """
    the CostModel contains the logic for each kind of hull production
    activity
    """
    def __init__(self, tables):
        """
        tables: dict of tables (each table is a list of lists)
        Because of shared setups, the model is stateful and a new
        instance should be used for each evaluation.
        """
        # check for appropriate keys
        expected = ["model", "shared_setups"]
        for table in expected: # raise error if missing data
            if tables.get(table, None) is None:
                msg = "Hull Cost Model requires a table named {0}"
                raise CostModelError(msg.format(table))
        self.tables = tables # for now
        self.model = associate(tables["model"])
        self.operations = self.model.keys()
        for row in tables["materials"][1:]:
            row[0] = tuple(row[0])
        self.materials = associate(tables["materials"])
        self.material_types = associate(tables["material_type"])
        self.opnames = dict(tables["operations"])
        self.setups_counted = []

    def material_areas(self, plates):
        """
        Compute total area for each (material, thickness)
        """
        areas = {}
        for plate in plates:
            key = plate["key"]
            material_type = self.material_types[plate["material"]]
            area = areas.get(key, 0)
            area += plate["area"]
            areas[key] = area
        return areas

    def material_sheets(self, areas):
        """
        Compute number of sheets for each material.
        We're not solving the cutting stock problem, and we're 
        not concerned with panels that are too big to cut out 
        of a single sheet.
        Instead, we compute total area for each type of steel, add
        ten percent for scrap, then divide by the area of a sheet 
        to get the total number of sheets needed.
        """
        sheets = {}
        for key, area in areas.iteritems():
            area *= 1.1 # ten percent scrap
            sheetlength = self.materials[key]["length"]
            sheetwidth = self.materials[key]["width"]
            number = area / (sheetlength * sheetwidth)
            sheets[key] = math.ceil(number) # round up to full sheet
        return sheets

    def nsteelsheets(self, plates):
        """
        Total number of steel sheet sides = 2 * number of sheets
        (Info needed for blasting.)
        """
        areas = self.material_areas(plates)
        sheets = self.material_sheets(areas)

        nsheets = sum([sheets[key] for key in sheets.iterkeys() if 
                       self.material_types[key[0]]["type"] == "Steel"])

        return 2.0 * nsheets

    def cuttime(self, plates, technology):
        """
        Cutting time is computed the same way for both Plasma
        and Laser.
        Total cut time depends on thickness, perimeter,
        and material of panels.
        Assuming you have to cut every edge and you can't use the
        edges of the sheets that the plates are cut from.
        """
        totaltime = 0
        for plate in plates:
            key = plate["key"]
            material = self.materials[key]
            if material["cut"] == technology:
                perimeter = 2.0 * plate["perimeter"] # 2 sides
                speed = material["cutspeed"]
                totaltime += perimeter / speed
        return totaltime

    def plasmacuttime(self, plates):
        return self.cuttime(plates, "Plasma")

    def lasercuttime(self, plates):
        return self.cuttime(plates, "Laser")

    def waterjet(self, plates):
        return self.cuttime(plates, "Waterjet")

    def perimeter(self, plates, kind):
        """
        Used to compute edge prep time.  Steel and aluminum are 
        separate.
        """
        perimeter = 0
        for plate in plates:
            if self.material_types[plate["material"]]["type"] == kind:
                perimeter += 2.0 * plate["perimeter"] # 2 sides
        return perimeter

    def steelperimeter(self, plates):
        return self.perimeter(plates, "Steel")

    def alperimeter(self, plates):
        return self.perimeter(plates, "Aluminum")

    def npanels(self, plates):
        """
        Total number of panels (all plates) is needed for 
        build and tack operation
        """
        return len(plates)

    def weldtime(self, plates, technology):
        """
        Welding time depends on perimeter of plates.
        We're apparently not concerned that we might need to weld
        incompatible plates.
        """
        totaltime = 0
        for plate in plates:
            key = plate["key"]
            material = self.materials[key]
            if material["weld"] == technology:
                perimeter = 2.0 * plate["perimeter"] # 2 sides
                speed = material["weldspeed"]
                totaltime += (perimeter / (2.0*speed))
        return totaltime

    def migtime(self, plates):
        return self.weldtime(plates, "MIG")

    def tigtime(self, plates):
        return self.weldtime(plates, "TIG")

    def pwmtime(self, plates):
        """
        Return 1 for one post-weld machining
        """
        return 1

    def hullarea(self, plates):
        """
        Hull surface area: needed for painting, includes both sides,
        units are square meters.
        """
        in2 = 2.0 * sum([plate["area"] for plate in plates])
        factor = 0.00064516 # to convert to square meters
        return in2 * factor

    def shipprep(self, plates):
        """
        Return 1 for one shipping prep
        """
        return 1

    def howmuch(self, op, plates):
        """
        for given op and plates, how much work needs to be 
        done?  Units vary by operation.
        """
        qtys = {
                "blast"         : self.nsteelsheets,
                "plasma"        : self.plasmacuttime,
                "laser"         : self.lasercuttime,
                "steelprep"     : self.steelperimeter,
                "alprep"        : self.alperimeter,
                "buildntack"    : self.npanels,
                "mig"           : self.migtime,
                "tig"           : self.tigtime,
                "pwm"           : self.pwmtime,
                "paint"         : self.hullarea,
                "shipprep"      : self.shipprep,
                "waterjet"      : self.waterjet
               }
        try:
            qty = qtys[op] # KeyError if invalid op
        except KeyError:
            msg = "{0}: Model has no formula for "\
                  "work quantity".format(op)
            raise CostModelError(msg)
        return qty(plates)

    def howfast(self, op):
        """
        Look up rate in the table.  Some rates are 1, meaning that
        the quantity of work is already expressed in minutes.
        """
        return self.model[op]["workrate"]

    def setup(self, op):
        """
        Some operations share setups, so if they are both present,
        only count one of the setups.  
        Assume that this method is only called if there is in fact
        a setup for the operation.
        """
        for group in self.tables["shared_setups"]:
            if op in group:
                for otherop in group:
                    if otherop in self.setups_counted:
                        self.setups_counted.append(op)
                        return 0
        self.setups_counted.append(op)
        return self.model[op]["setup"]

    def ohqty(self, op, size, laborneeded):
        """
        A special exception for painting: overhead is charged by
        surface area rather than time.
        """
        if op in ["paint"]:
            return size
        else:
            return laborneeded
    
    def prepare_plates(self, plates):
        """
        Check that material and dimensional data are present in the
        model for all plates
        Annotate each plate with its key
        Convert units from mm to inches
        """
        plates = copy.copy(plates)
        counter = 0
        for plate in plates:
            fields = ["material", "thickness", "area", "perimeter"]
            for field in fields:
                data = plate.get(field, None)
                if data is None:
                    msg = "No {0} specified for plate {1}"
                    raise CostModelError(msg.format(field, counter))
            material = (plate["material"], plate["thickness"])
            plate["key"] = material
            material_data = self.materials.get(material, None)
            if material_data is None:
                msg = "No material data for ({0}, {1}) (plate {2})"
                raise CostModelError(
                        msg.format(material[0], material[1], counter))
            # Excel model uses inches
            plate["area"] = plate["area"] / 25.4 ** 2 # mm2 to in2
            plate["perimeter"] = plate["perimeter"] / 25.4 # mm to in
            counter += 1
        return plates

    def build_cost_and_lead_time(self, plates):
        """
        plates: list of summary data about plates
                   (material, thickness, perimeter, area)
        Note that we charge by the hour but work by the minute,
        so we convert laborneeded to hours
        """
        costs = [] 
        times = [] 
        for op in self.operations:
            size        = self.howmuch(op, plates)
            workrate    = self.howfast(op)
            if size > 0:
                setup   = self.setup(op)
            else:
                setup   = 0
            laborneeded = (setup + size / workrate) / 60.0
            labqty      = self.model[op]["labqty"]
            labrate     = self.model[op]["labrate"]
            ohqty       = self.ohqty(op, size, laborneeded)
            ohrate      = self.model[op]["ohrate"]
            cost        = laborneeded * labqty * labrate \
                          + ohqty * ohrate
            time        = laborneeded
            times.append(time)
            costs.append(cost)
        header = ["operation", "cost", "time"]
        operations = [self.opnames[op] 
                      for op in self.operations]
        table = [dict(zip(header, record))
                 for record in zip(operations, costs, times)]
        return {"table": table,
                "cost": sum(costs), "time": sum(times)}

    def material_cost_and_lead_time(self, plates):
        cost = 0
        time = 0
        sheets = self.material_sheets(self.material_areas(plates))
        for key in sheets.iterkeys():
            matl = self.materials[key]
            thickness = key[1]
            volume = thickness * matl["length"] * matl["width"]
            volume *= sheets[key]
            mass = volume * self.material_types[key[0]]["density"]
            matlcost = mass * matl["cost"]
            cost += matlcost
            time = max(time, matl["leadtime"])
        return (cost, time)

def mass(plates, materials):
    """ compute mass of each plate based on material data """
    totalmass = 0
    matl_data = dict([(row[0], row[1:]) for row in materials])

    for plate in plates:
        matl = plate.get("material", "MIL_DTL_12560")
        this_matl_data = matl_data.get(matl, [-1, "x", "x", 0.0])
        density = this_matl_data[-1] # per cubic inch

        thickness = plate.get("thickness", 1.5)
        area = plate.get("area", 0)

        volume = thickness * area
        plate["mass"] = volume * density
        totalmass += plate["mass"]

    return totalmass

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("hullplates", type=argparse.FileType("r"),
                        help="json list of plates in the hull")
    parser.add_argument("-d", "--model-data", 
                        type=argparse.FileType("r"),
                        default="hull_cost_data.json",
                        help="json cost model data")
    return parser.parse_args()

def evaluate(plates, tables):
    model = HullCostAndTime(tables)
    plates = model.prepare_plates(plates)
    build    =  model.build_cost_and_lead_time(plates)
    material =  model.material_cost_and_lead_time(plates)
    matl_cost = material[0]
    matl_time = material[1] * 7 # convert weeks to days
    build_cost = build["cost"]
    build_time = build["time"] / 8.0 # convert hours to days
    total_mass = mass(plates, tables["material_type"])
    report = {}
    report["plates"] = plates
    report["Build"] = build["table"]
    report["Build Cost"] = build_cost
    report["Build Time"] = build_time
    report["Material Cost"] = matl_cost
    report["Material Lead Time"] = matl_time
    report["Total Cost"] = build_cost + matl_cost
    report["Total Time"] = build_time + matl_time
    report["Total Mass"] = total_mass * 0.453592 # convert to kg

    return report

if __name__ == "__main__":
    args = get_args()
    plates = json.load(args.hullplates)
    tables = json.load(args.model_data)
    build, material, report = evaluate(plates, tables)
    print report

# vim:ts=4:sw=4:expandtab:fdm=indent:wrap lbr:ai:colorcolumn=70
