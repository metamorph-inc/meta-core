using CyPhyPET;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Xunit;

namespace CyPhyPETTest
{
    public class DesignVariableTest
    {
        [Fact]
        public void TestSingleValue()
        {
            var dv = new AVM.DDP.PETConfig.DesignVariable();
            PET.ParseDesignVariableRange("\"asdf\\n\\tasdf\"", dv);
            Assert.Equal(new string[] { "asdf\n\tasdf" }, dv.items);
            PET.ParseDesignVariableRange("\"asdf\";\"jkl;\"", dv);
            Assert.Equal(new string[] { "asdf", "jkl;" }, dv.items);

            PET.ParseDesignVariableRange("[1,2]", dv);
            Assert.Equal(1.0, dv.RangeMin);
            Assert.Equal(2.0, dv.RangeMax);

            PET.ParseDesignVariableRange("1.0,2", dv);
            Assert.Equal(1.0, dv.RangeMin);
            Assert.Equal(2.0, dv.RangeMax);

            PET.ParseDesignVariableRange("(1,2.0)", dv);
            Assert.Equal(1.0000000000000002, dv.RangeMin);
            Assert.Equal(1.9999999999999998, dv.RangeMax);

            PET.ParseDesignVariableRange("(1e0,2.0]", dv);
            Assert.Equal(1.0000000000000002, dv.RangeMin);
            Assert.Equal(2.0, dv.RangeMax);

            PET.ParseDesignVariableRange("[1e+0,1]", dv);
            Assert.Equal(1, dv.RangeMin);
            Assert.Equal(1, dv.RangeMax);

            Assert.Throws<ApplicationException>(() =>
            {
                PET.ParseDesignVariableRange("\"unclosed string", dv);
            });
            Assert.Throws<ApplicationException>(() =>
            {
                PET.ParseDesignVariableRange("1,2,3", dv);
            });
            Assert.Throws<ApplicationException>(() =>
            {
                PET.ParseDesignVariableRange("(1,1)", dv);
            });
            Assert.Throws<ApplicationException>(() =>
            {
                PET.ParseDesignVariableRange("(1,1]", dv);
            });
            Assert.Throws<ApplicationException>(() =>
            {
                PET.ParseDesignVariableRange("[1,1)", dv);
            });
        }

        [DllImport("ucrtbase", CallingConvention = CallingConvention.Cdecl)]
        static extern double nextafter(double x, double y);

        public class TupleList<T1, T2> : List<Tuple<T1, T2>>
        {
            public void Add(T1 item, T2 item2)
            {
                Add(new Tuple<T1, T2>(item, item2));
            }
        }

        [Fact]
        public void TestNextAfter()
        {
            List<Tuple<double, double>> cases = new TupleList<double, double>()
            {
                {1.0, 2.0},
                {2.0000000000000004, -1},
                {2.0000000000000004, Double.PositiveInfinity},
                {2.0000000000000004, Double.NegativeInfinity},
                {0, 1},
                {0, -1},
            };

            foreach (var case_ in cases)
            {
                Assert.Equal(nextafter(case_.Item1, case_.Item2), PET.nextafter(case_.Item1, case_.Item2));
            }
        }
    }
}
