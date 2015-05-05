using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhy2CAD_CSharp.TestBenchModel
{
    public class KinematicGravity
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Z { get; set; }
        public bool Active { get; set; }

        public KinematicGravity(double x, double y, double z, bool active)
        {
            X = x;
            Y = y;
            Z = z;
            Active = active;
        }

        public Multibody.ModelLoadsGravity GenerateMultiBodyOutput()        
        {
            Multibody.ModelLoadsGravity gravityLoad = new Multibody.ModelLoadsGravity();
            if (Active)
                gravityLoad.Active = Multibody.ModelLoadsGravityActive.on;
            else
                gravityLoad.Active = Multibody.ModelLoadsGravityActive.off;
            
            //gravityLoad.MetricID

            return gravityLoad;
        }
    }

    public class KinematicFEAExport
    {
        public string ComponentID { get; set; }
        public string Type { get; set; }
    }

    public class KinematicMotion
    {
    }
}
