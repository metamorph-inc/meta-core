using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace ComponentImporterUnitTests
{    
    public class Unit
    {
        private static readonly string testPath = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "ImportTestModels",
            "Unit");

        private static readonly string orgXmePathInputModel = Path.Combine(
            META.VersionInfo.MetaPath,
            "test",
            "InterchangeTest",
            "ComponentInterchangeTest",
            "SharedModels",
            "BlankInputModel",
            "InputModel.xme");

        private static readonly string xmePathInputModel = Path.Combine(
            testPath,
            "Model.mga");
        
        private static readonly string acmPath = Path.Combine(
            testPath,
            "Component.acm");

        [Fact]
        public void SetUnit()
        {
            File.Copy(orgXmePathInputModel, xmePathInputModel, true);

            String connString;
            MgaUtils.ImportXMEForTest(xmePathInputModel, out connString);
            var mgaPath = connString.Substring("MGA=".Length);
            Assert.True(File.Exists(mgaPath), "Input model not found; import may have failed.");

            var project = GetProject(mgaPath);

            var importer = new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();
            importer.Initialize(project);

            project.BeginTransactionInNewTerr();
            try
            {
                var fco = importer.ImportFile(project, testPath, acmPath);
                ISIS.GME.Dsml.CyPhyML.Interfaces.Component comp = ISIS.GME.Dsml.CyPhyML.Classes.Component.Cast(fco);

                var millimeter = comp.Children.PropertyCollection
                                              .First(p => p.Name.Equals("mm"))
                                              .Referred.unit;
                Assert.NotNull(millimeter);
                Assert.Equal("Millimeter", millimeter.Name);

                var henry = comp.Children.PropertyCollection
                                         .First(p => p.Name.Equals("Henry"))
                                         .Referred.unit;
                Assert.NotNull(henry);
                Assert.Equal("Henry", henry.Name);

                var acre = comp.Children.PropertyCollection
                                        .First(p => p.Name.Equals("acre"))
                                        .Referred.unit;
                Assert.NotNull(acre);
                Assert.Equal("Acre", acre.Name);
            }
            finally
            {
                project.CommitTransaction();
            }

            project.Save();
            project.Close();
        }


        public static MgaProject GetProject(String filename)
        {
            MgaProject result = null;

            if (filename != null && filename != "")
            {
                if (Path.GetExtension(filename) == ".mga")
                {
                    result = new MgaProject();
                    if (System.IO.File.Exists(filename))
                    {
                        Console.Out.Write("Opening {0} ... ", filename);
                        bool ro_mode;
                        result.Open("MGA=" + Path.GetFullPath(filename), out ro_mode);
                    }
                    else
                    {
                        Console.Out.Write("Creating {0} ... ", filename);
                        result.Create("MGA=" + filename, "CyPhyML");
                    }
                    Console.Out.WriteLine("Done.");
                }
                else
                {
                    Console.Error.WriteLine("{0} file must be an mga project.", filename);
                }
            }
            else
            {
                Console.Error.WriteLine("Please specify an Mga project.");
            }

            return result;
        }
    }
}
