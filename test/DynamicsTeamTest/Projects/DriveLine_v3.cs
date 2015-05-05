using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace DynamicsTeamTest.Projects
{
    public partial class DriveLine_v3 : IUseFixture<DriveLine_v3Fixture>
    {
        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("CyPhy2Modelica", "DriveLine_v3")]
        public void Dynamics_CurrentObjectNull()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            string ProjectConnStr = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var interpreter = new CyPhy2Modelica_v2.CyPhy2Modelica_v2Interpreter();
                interpreter.Initialize(project);

                Assert.DoesNotThrow(() => interpreter.InvokeEx(project, null, null, 16));
            }
            finally
            {
                project.Close(true);
            }
        }

        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("CyPhySoT", "DriveLine_v3")]
        public void CyPhySoT_CurrentObjectNull()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            string ProjectConnStr = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var interpreter = new CyPhySoT.CyPhySoTInterpreter();
                interpreter.Initialize(project);

                Assert.DoesNotThrow(() => interpreter.InvokeEx(project, null, null, 16));
            }
            finally
            {
                project.Close(true);
            }
        }

        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("CyPhyPET", "DriveLine_v3")]
        public void CyPhyPET_CurrentObjectNull()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            string ProjectConnStr = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var interpreter = new CyPhyPET.CyPhyPETInterpreter();
                interpreter.Initialize(project);

                Assert.DoesNotThrow(() => interpreter.InvokeEx(project, null, null, 16));
            }
            finally
            {
                project.Close(true);
            }
        }

        [Fact]
        [Trait("Model", "DriveLine_v3")]
        [Trait("CyPhyMultiJobRun", "DriveLine_v3")]
        public void CyPhyMultiJobRun_CurrentObjectNull()
        {
            Assert.True(File.Exists(mgaFile), "Failed to generate the mga.");
            string ProjectConnStr = "MGA=" + mgaFile;

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var interpreter = new CyPhyMultiJobRun.CyPhyMultiJobRunInterpreter();
                interpreter.Initialize(project);

                Assert.DoesNotThrow(() => interpreter.InvokeEx(project, null, null, 16));
            }
            finally
            {
                project.Close(true);
            }
        }
    }
}
