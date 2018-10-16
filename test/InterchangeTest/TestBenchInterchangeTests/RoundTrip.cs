using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using GME.MGA;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using Xunit;
using System.Diagnostics;

namespace TestBenchInterchangeTests
{
    public abstract class TBRoundTripBase
    {
        public abstract MgaProject proj { get; }
        public abstract string AtmPath { get; }
        public virtual string FolderName { get { return "TestBenches"; } }

        public void CopyMgaAndRunImporter(string tbName)
        {
            //proj.Save(proj.ProjectConnStr + asmName + ".mga", true);
            File.Copy(proj.ProjectConnStr.Substring("MGA=".Length), (proj.ProjectConnStr + tbName + ".mga").Substring("MGA=".Length), true);

            MgaProject proj2 = (MgaProject)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaProject"));
            proj2.OpenEx(proj.ProjectConnStr + tbName + ".mga", "CyPhyML", null);
            proj2.BeginTransactionInNewTerr();
            try
            {
                MgaFCO oldTestBench = (MgaFCO)proj2.RootFolder.GetObjectByPathDisp("/@" + FolderName + "/@" + tbName + "|kind=TestBench");
                Assert.NotNull(oldTestBench);
                oldTestBench.DestroyObject();
                CyPhyML.TestBenchType testBench;
                using (StreamReader streamReader = new StreamReader(Path.Combine(AtmPath, tbName + ".atm")))
                {
                    avm.TestBench avmTestBench = OpenMETA.Interchange.AvmXmlSerializer.Deserialize<avm.TestBench>(streamReader);
                    testBench = CyPhy2TestBenchInterchange.TestBenchInterchange2CyPhy.Convert(avmTestBench, proj2);
                }
            }
            finally
            {
                proj2.CommitTransaction();
                if (Debugger.IsAttached)
                {
                    proj2.Save(null, true);
                }
                proj2.Close(true);
            }
        }

        public MgaFCO RunExporter(string tbName)
        {
            MgaFCO testBench;
            proj.BeginTransactionInNewTerr();
            try
            {
                testBench = (MgaFCO)proj.RootFolder.GetObjectByPathDisp("/@" + FolderName + "/@" + tbName + "|kind=TestBench");
                Assert.NotNull(testBench);

                new CyPhyTestBenchExporter.CyPhyTestBenchExporterInterpreter().ExportToFile(CyPhyMLClasses.TestBench.Cast(testBench), AtmPath);
            }
            finally
            {
                proj.AbortTransaction();
            }

            Assert.True(File.Exists(Path.Combine(AtmPath, tbName + ".atm")));
            return testBench;
        }

        public void RunRoundTrip(string asmName)
        {
            File.Delete(Path.Combine(AtmPath, asmName + ".atm"));
            RunExporter(asmName);
            CopyMgaAndRunImporter(asmName);
            ComponentExporterUnitTests.Tests.runCyPhyMLComparator(proj.ProjectConnStr.Substring("MGA=".Length),
                (proj.ProjectConnStr + asmName + ".mga").Substring("MGA=".Length), Environment.CurrentDirectory);
        }
    }
}
