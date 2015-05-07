using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME.MGA;
using System.Reflection;
using Xunit;

namespace TestBenchInterchangeTests
{
    public abstract class GMEProjectFixtureBase : IDisposable
    {
        public String testPath
        {
            get
            {
                return Path.GetDirectoryName(xmePath);
            }
        }

        public abstract String xmePath { get; }
        public MgaProject proj { get; private set; }

        public GMEProjectFixtureBase()
        {
            String mgaConnectionString;
            GME.MGA.MgaUtils.ImportXMEForTest(xmePath, out mgaConnectionString);
            var mgaPath = mgaConnectionString.Substring("MGA=".Length);

            Assert.True(File.Exists(Path.GetFullPath(mgaPath)),
                        String.Format("{0} not found. Model import may have failed.", mgaPath));

            proj = new MgaProject();
            bool ro_mode;
            proj.Open("MGA=" + Path.GetFullPath(mgaPath), out ro_mode);
            proj.EnableAutoAddOns(true);
        }

        public void Dispose()
        {
            proj.Save();
            proj.Close();
        }
    }
}
