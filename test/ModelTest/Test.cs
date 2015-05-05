using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;

using GME.MGA;
using GME.CSharp;
using System.Text.RegularExpressions;
using System.IO;

namespace ModelTest
{
    public class Test
    {
        [Fact]
        [Trait("Model","Box")]
        public void BoxModelImport()
        {
            var p_BoxModel = Path.Combine(META.VersionInfo.MetaPath,
                                          "models",
                                          "Box");
            var p_BoxModelXME = Path.Combine(p_BoxModel,
                                             "box.xme");
            var p_BoxModelMGA = p_BoxModelXME.Replace("box.xme", "box_import_test.mga");

            
            Assert.True(File.Exists(p_BoxModelXME), "box.xme not found at expected location: " + p_BoxModelXME);

            // Delete existing mga
            try
            {
                File.Delete(p_BoxModelMGA);
            }
            catch (FileNotFoundException)
            {
                // it's okay
            }

            GME.MGA.MgaUtils.ImportXME(p_BoxModelXME, p_BoxModelMGA);
            Assert.True(File.Exists(p_BoxModelMGA), "MGA file not found. Import may have failed.");
        }
    }
}
