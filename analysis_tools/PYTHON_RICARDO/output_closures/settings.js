{
    "design_name" : "VU_assembly",
    "metrics_file" : "CADAssembly_metrics.xml",
    "path_to_instance_xmls" : "ComponentACMs",
    "path_to_instance_stls" : "STL_BINARY",
	"instance_file": "component_index.json",
    "output_json_file": "output.json",
    "show_2d" : true,
    "ground_slope" : -30.0,
    "connectivity" : {"7198__SPRING_AXIAL_HELICAL": "45__HATCH_ASSEMBLY_DRIVER_COMMANDER",
                      "7179__SPRING_TORSION_LAMINATED": "45__HATCH_ASSEMBLY_DRIVER_COMMANDER",
                      "7180__SPRING_TORSION_LAMINATED": "45__HATCH_ASSEMBLY_DRIVER_COMMANDER",

                      "7197__SPRING_AXIAL_HELICAL": "50__HATCH_ASSEMBLY_DRIVER_COMMANDER",
                      "7176__SPRING_TORSION_LAMINATED": "50__HATCH_ASSEMBLY_DRIVER_COMMANDER",
                      "7178__SPRING_TORSION_LAMINATED": "50__HATCH_ASSEMBLY_DRIVER_COMMANDER",

                      "7433__SPRING_AXIAL_HELICAL" : "62__HATCH_ASSEMBLY_CARGO",
                      "7434__SPRING_AXIAL_HELICAL" : "55__HATCH_ASSEMBLY_CARGO",

                      "7435__ACTUATOR_LINEAR_HYDRAULIC": "42__HATCH_ASSEMBLY_REAR_RAMP"},
    "dist_to_ground" : {"42__HATCH_ASSEMBLY_REAR_RAMP": 0.69}
}