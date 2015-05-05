// -----------------------------------------------------------------------
// <copyright file="ModelConfig.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhy2Modelica_v2
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ModelConfig
    {
        /// <summary>
        /// Gets or sets model_file_name.
        /// </summary>
        /// <value>String - Absolute or relative path to the .mo file..</value>
        public string model_file_name { get; set; }

        /// <summary>
        /// Gets or sets model_name.
        /// </summary>
        /// <value>String - Full name of the model including path within package.</value>
        public string model_name { get; set; }

        /// <summary>
        /// Gets or sets verification_model_name.
        /// </summary>
        /// <value>String - Full name of the verification model including path within package.</value>
        public string verification_model_name { get; set; }

        /// <summary>
        /// Gets or sets MSL_version.
        /// </summary>
        /// <value>String - Chosen Modelica Standard Library version.</value>
        public string MSL_version { get; set; }

        /// <summary>
        /// Gets or sets result_file.
        /// </summary>
        /// <value>String - Name and path to result .mat file.</value>
        public string result_file { get; set; }

        public Dictionary<string, string> metrics_map { get; set; }

        public List<string> lib_package_paths { get; set; }

        public List<string> lib_package_names { get; set; }

        public Experiment experiment { get; set; }

        public class Experiment
        {
            public string StartTime { get; set; }
            public string StopTime { get; set; }
            public string NumberOfIntervals { get; set; }
            public string Interval { get; set; }
            public string IntervalMethod { get; set; }
            public string Tolerance { get; set; }
            public AlgorithmClass Algorithm { get; set; }
            public class AlgorithmClass
            {
                public string Dymola { get; set; }
                public string OpenModelica { get; set; }
                public string JModelica { get; set; }

                public AlgorithmClass()
                {
                    this.Dymola = "dassl";
                    this.OpenModelica = "dassl";
                    this.JModelica = "cvode";
                }
            }
            public string ToolSelection { get; set; }
            public Experiment()
            {
                this.StartTime = "0";
                this.StopTime = "1";
                this.NumberOfIntervals = "500";
                this.Interval= "0";
                this.IntervalMethod = "NumberOfIntervals";
                this.Tolerance = "1e-04";
                this.Algorithm = new AlgorithmClass();
                this.ToolSelection = "Dymola__latest_";
            }
        }

        public ModelConfig()
        {
            result_file = "";
            MSL_version = "3.2";
            lib_package_paths = new List<string>();
            lib_package_names = new List<string>();
            metrics_map = new Dictionary<string, string>();
            experiment = new Experiment();
        }
    }
}
