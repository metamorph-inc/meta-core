""" Read in parts from a cad metrics file. """
import numpy as np
from lxml import etree
import re

from geom_utils import matrix_from_nx_ny_ro

conv_dict = {"inch" : 0.0254, "millimeter" : 0.001, "mm" : 0.001, "m" : 1.0}


def get_parts(cad_metric_xml_file):
    """
    Return a list of part information.
    """
    xml_root = etree.XML(open(cad_metric_xml_file, "r").read())

    part_info = []

    ## Only want leaf instances
    instance_id_raw = []
    instances = xml_root.xpath(r"//Assembly//CADComponent")
    for inst in instances:
        if len(inst) == 0:
            instance_id_raw.append(inst.get("ComponentInstanceID"))
        else:
            instance_id_raw.append("{fake}")
            
    instance_id = [re.search(r"{(.*)}", raw).groups()[0] for raw in instance_id_raw]

    for i, child_metric in enumerate(xml_root.xpath(r"//MetricComponents//MetricComponent")):
        if instance_id_raw[i] == "{fake}":
            continue
        length_unit = child_metric.xpath(r"Units/@Distance")[0]
        try:
            conv_factor = conv_dict[length_unit]
        except KeyError:
            raise KeyError("Unknown unit for length: '{}'".format(length_unit))
            


        stl_suffix = "_asm.stl" if child_metric.get("Type") == "ASSEMBLY" else "_prt.stl"

        part_dict = {"stl_name" : child_metric.get("Name") + stl_suffix,
                     "instance" : instance_id[i],
                     "conv_fac" : conv_factor}

        part_info.append(part_dict)

    for i, child_metric in enumerate(xml_root.xpath(r"//ChildMetric")):

        mat = np.array([[1.0, 0, 0, 0],
                         [0, 1.0, 0, 0],
                         [0, 0, 1.0, 0],
                         [0, 0, 0, 1.0]])

        flat = [float(f) for f in child_metric.xpath(r"RotationMatrix//Column/@Value")]
        mat[0, :3] = flat[0:3]
        mat[1, :3] = flat[3:6]
        mat[2, :3] = flat[6:9]

        ## Will recalculate a fresh matrix because the ones in the file usually include
        ## a scale component too
        n_x = np.dot(mat[:3, :3], np.array([1, 0, 0]))
        n_y = np.dot(mat[:3, :3], np.array([0, 1, 0]))

        tran = np.zeros(3)
        tran[0] = float(child_metric.xpath(r"Translation/@X")[0]) * part_info[i]["conv_fac"]
        tran[1] = float(child_metric.xpath(r"Translation/@Y")[0]) * part_info[i]["conv_fac"]
        tran[2] = float(child_metric.xpath(r"Translation/@Z")[0]) * part_info[i]["conv_fac"]


        part_info[i].update({"trans" : matrix_from_nx_ny_ro(n_x, n_y, tran)})

    return part_info

def get_parts_creo(cad_metric_xml_file):

    xml_root = etree.XML(open(cad_metric_xml_file, "r").read())

    part_info = []

    for comp in xml_root.xpath(r"//COMPONENT"):
        mat = np.array([[1.0, 0, 0, 0],
                         [0, 1.0, 0, 0],
                         [0, 0, 1.0, 0],
                         [0, 0, 0, 1.0]])

        mat[0, :] = eval(comp.xpath(r"*//ROW1")[0].text)
        mat[1, :] = eval(comp.xpath(r"*//ROW2")[0].text)
        mat[2, :] = eval(comp.xpath(r"*//ROW3")[0].text)
        mat[3, :] = eval(comp.xpath(r"*//ROW4")[0].text)

        mat[:3, 3] *= 0.001

        part_dict = {"trans" : mat,
                     "name" : comp.get("_ID"),
                     "class" : comp.get("Component_Class"),
                     "instance" : comp.get("Name")
                     }

        part_info.append(part_dict)

    return part_info

if __name__ == "__main__":
#    print get_parts(r"..\..\..\output_fov\faked_metrics.xml")
    parts = get_parts(r"..\..\..\output_fov\CADAssembly_metrics.xml")

    for part in parts:
        print()
#        print part
        for f in part:
            print(f)
            print(part[f])

    print()

#    print get_parts_creo(r"DriverVision_Info.xml")

