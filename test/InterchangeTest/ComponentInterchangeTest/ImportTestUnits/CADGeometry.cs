using CyPhyComponentExporter;
using CyPhyComponentImporter;
using GME.MGA;
using System;
using System.IO;
using System.Windows.Forms;
using Xunit;
using GME.MGA.Parser;
using System.Reflection;

namespace ComponentImporterUnitTests
{

    public class CADGeometryFixture : IDisposable
    {
        private Exception exception;

        private MgaProject _project;

        public string Directory
        {
            get
            {
                return Path.Combine(Path.GetDirectoryName(Assembly.GetAssembly(typeof(CADGeometryFixture)).CodeBase.Substring("file:///".Length)), "../../../ImportTestModels/CADGeometry");
            }
        }

        public MgaProject Project
        {
            get
            {
                if (this.exception != null)
                {
                    throw this.exception;
                }
                return this._project;
            }
        }

        public CADGeometryFixture()
        {
            try
            {
                this._project = new MgaProjectClass();
                this._project.EnableAutoAddOns(true);
                this._project.Create("MGA=" + Path.Combine(this.Directory , "test.mga"), "CyPhyML");
                new MgaParserClass().ParseProject(this._project, Path.Combine(this.Directory, "CADGeometry.xme"));
                this._project.Notify(globalevent_enum.GLOBALEVENT_OPEN_PROJECT_FINISHED);
                Application.DoEvents();
                Application.DoEvents();
            }
            catch (Exception ex)
            {
                this.exception = ex;
            }
        }

        public void Dispose()
        {
            if (this._project != null && (this._project.ProjectStatus & 1) != 0)
            {
                this._project.Close(false);
            }
        }
    }

    public class CADGeometry : IUseFixture<CADGeometryFixture>
    {
        private CADGeometryFixture fixture;

        [Fact]
        public void RoundTripTest()
        {
            CyPhyComponentExporterInterpreter exporter = new CyPhyComponentExporterInterpreter();
            string acmDirectory = Path.Combine(this.fixture.Directory, "exported_components");
            if (Directory.Exists(acmDirectory))
            {
                Directory.Delete(acmDirectory, true);
            }
            Directory.CreateDirectory(acmDirectory);
            exporter.OutputDir = acmDirectory;
            exporter.InvokeEx(this.fixture.Project, null, null, 128);
            string inputFilename = this.fixture.Project.ProjectConnStr.Substring("MGA=".Length);

            MgaProjectClass outputProject = new MgaProjectClass();
            outputProject.EnableAutoAddOns(true);
            string outputFilename = Path.Combine(this.fixture.Directory, "output", "test_imported.mga");
            Directory.CreateDirectory(Path.GetDirectoryName(outputFilename));
            outputProject.Create("MGA=" + outputFilename, "CyPhyML");
            try
            {
                outputProject.Notify(globalevent_enum.GLOBALEVENT_OPEN_PROJECT_FINISHED);
                Application.DoEvents();
                Application.DoEvents();
                outputProject.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_GENERAL);
                try
                {
                    CyPhyComponentImporterInterpreter cyPhyComponentImporterInterpreter = new CyPhyComponentImporterInterpreter();
                    foreach (string current in Directory.EnumerateFiles(acmDirectory, "*.zip"))
                    {
                        cyPhyComponentImporterInterpreter.Initialize(outputProject);
                        cyPhyComponentImporterInterpreter.ImportFile(outputProject, this.fixture.Directory, Path.Combine(acmDirectory, current));
                    }
                }
                finally
                {
                    outputProject.CommitTransaction();
                }
            }
            finally
            {
                outputProject.Close(false);
            }
            this.fixture.Project.Close(false);

            {
                var inputProject = new MgaProjectClass();
                bool ro_mode;
                inputProject.Open("MGA=" + inputFilename, out ro_mode);
                try
                {
                    outputProject = new MgaProjectClass();
                    outputProject.Open("MGA=" + outputFilename, out ro_mode);
                    try
                    {
                        inputProject.BeginTransactionInNewTerr();
                        outputProject.BeginTransactionInNewTerr();
                        try
                        {
                            foreach (string path in new string[] { "/@Imported_Components/@Component/@Extrusion/@DirectionReferencePoint",
                                "/@Imported_Components/@Component/@Extrusion/@ExtrusionHeight" })
                            {
                                // there's nowhere in the .acm file to put this ID
                                ((MgaFCO)outputProject.RootFolder.ObjectByPath[path]).SetStrAttrByNameDisp("ID",
                                    ((MgaFCO)inputProject.RootFolder.ObjectByPath[path]).GetStrAttrByNameDisp("ID"));
                            }
                        }

                        finally
                        {
                            inputProject.CommitTransaction();
                            outputProject.CommitTransaction();
                        }
                    }
                    finally
                    {
                        outputProject.Close(false);
                    }
                }
                finally
                {
                    inputProject.Close(false);
                }
            }

            ComponentExporterUnitTests.Tests.runCyPhyMLComparator(inputFilename, outputFilename, this.fixture.Directory);
        }

        public void SetFixture(CADGeometryFixture data)
        {
            this.fixture = data;
        }
    }
}
