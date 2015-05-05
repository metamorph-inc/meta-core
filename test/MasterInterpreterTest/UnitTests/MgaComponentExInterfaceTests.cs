namespace MasterInterpreterTest.UnitTests
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using Xunit;
    using GME.MGA;
    using MasterInterpreterTest.Projects;
    using System.IO;

    public class MgaComponentExInterfaceTests : IUseFixture<MasterInterpreterFixture>
    {
        internal string mgaFile { get { return this.fixture.mgaFile; } }
        private MasterInterpreterFixture fixture { get; set; }

        public void SetFixture(MasterInterpreterFixture data)
        {
            this.fixture = data;
        }

        public const string ProgID = "MGA.Interpreter.CyPhyMasterInterpreter";

        [Fact]
        [Trait("MasterInterpreter", "IMgaComponentEx")]
        public void RegisteredForCOMInterop()
        {
            IMgaComponentEx interpreter = GetInterpreterByProgID(ProgID);
            Assert.False(interpreter == null, string.Format("{0} is not a IMgaComponentEx", ProgID));
        }

        [Fact]
        [Trait("MasterInterpreter", "IMgaComponentEx")]
        public void GetProperties()
        {
            IMgaComponentEx interpreter = GetInterpreterByProgID(ProgID);
            Assert.DoesNotThrow(() => { Console.Out.WriteLine("ComponentName: {0}", interpreter.ComponentName); });
            Assert.DoesNotThrow(() => { Console.Out.WriteLine("ComponentParameter[\"key-does-not-exist\"]: {0}", interpreter.ComponentParameter["key-does-not-exist"]); });
            Assert.DoesNotThrow(() => { Console.Out.WriteLine("ComponentProgID: {0}", interpreter.ComponentProgID); });
            Assert.DoesNotThrow(() => { Console.Out.WriteLine("ComponentType: {0}", interpreter.ComponentType); });
            Assert.DoesNotThrow(() => { Console.Out.WriteLine("InteractiveMode: {0}", interpreter.InteractiveMode); });
            Assert.DoesNotThrow(() => { Console.Out.WriteLine("Paradigm: {0}", interpreter.Paradigm); });
        }


        [Fact]
        [Trait("MasterInterpreter", "IMgaComponentEx")]
        public void NullObjectAndInteractiveMode()
        {
            MgaProject project = null;
            MgaFCO currentObj = null;
            MgaFCOs fcos = null;

            MgaObject obj = null;
            MgaObjects objs = null;


            IMgaComponentEx interpreter = GetInterpreterByProgID(ProgID);
            Assert.Throws<ArgumentNullException>(() => { interpreter.Initialize(project); });
            Assert.Throws<NotImplementedException>(() => { interpreter.ObjectsInvokeEx(project, obj, objs, 128); });
            Assert.Throws<ArgumentNullException>(() => { interpreter.InvokeEx(project, currentObj, fcos, 128); });
            
            Assert.True(File.Exists(this.mgaFile), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(this.mgaFile);

            project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                // project is set, but fcos are not
                Assert.DoesNotThrow(() => { interpreter.Initialize(project); });
                Assert.Throws<ArgumentNullException>(() => { interpreter.InvokeEx(project, currentObj, fcos, 128); });

                // fcos are set
                fcos = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                Assert.DoesNotThrow(() => { interpreter.InvokeEx(project, currentObj, fcos, 128); });
                Assert.True(interpreter.InteractiveMode == false, "Interactive is not set correctly. (128) silent mode");
                Assert.DoesNotThrow(() => { interpreter.InvokeEx(project, currentObj, fcos, 16); });
                Assert.True(interpreter.InteractiveMode == true, "Interactive is not set correctly. (16)");
            }
            finally
            {
                project.Close(true);
            }
        }

        [Fact]
        [Trait("MasterInterpreter", "IMgaComponentEx")]
        public void InvokeExShouldNotThrowIfCurrentObjNull()
        {
            Assert.True(File.Exists(this.mgaFile), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(this.mgaFile);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath["/@TestBenches|kind=Testing|relpos=0/@DesignSpace|kind=Testing|relpos=0/@Dynamics|kind=Testing|relpos=0/@MSD_om_DS|kind=TestBench|relpos=0"] as MgaFCO;
                project.AbortTransaction();

                MgaFCOs fcos = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

                IMgaComponentEx interpreter = GetInterpreterByProgID(ProgID);
                Assert.DoesNotThrow(() => { interpreter.Initialize(project); });
                Assert.DoesNotThrow(() => { interpreter.InvokeEx(project, null, fcos, 128); });
            }
            finally
            {
                project.Close(true);
            }
        }

        [Fact]
        [Trait("MasterInterpreter", "IMgaComponentEx")]
        public void InvokeExShouldSucceedForValidContext()
        {
            Assert.True(File.Exists(this.mgaFile), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(this.mgaFile);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath["/@TestBenches|kind=Testing|relpos=0/@DesignSpace|kind=Testing|relpos=0/@Dynamics|kind=Testing|relpos=0/@MSD_om_DS|kind=TestBench|relpos=0"] as MgaFCO;
                project.AbortTransaction();

                MgaFCOs fcos = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

                IMgaComponentEx interpreter = GetInterpreterByProgID(ProgID);
                Assert.DoesNotThrow(() => { interpreter.Initialize(project); });
                Assert.DoesNotThrow(() => { interpreter.InvokeEx(project, testObj, fcos, 128); });
            }
            finally
            {
                project.Close(true);
            }
        }

        private static IMgaComponentEx GetInterpreterByProgID(string progid)
        {
            Type type = null;
            Assert.DoesNotThrow(() => { type = Type.GetTypeFromProgID(progid); });
            Assert.False(type == null, string.Format("Type cannot be created based on progID {0}", progid));

            IMgaComponentEx interpreter = null;
            Assert.DoesNotThrow(() => { interpreter = Activator.CreateInstance(type) as IMgaComponentEx; });
            Assert.False(interpreter == null, string.Format("{0} is not a IMgaComponentEx", progid));
            return interpreter;
        }
    }
}
