using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;
using System.IO;
using GME.MGA;

namespace SubTreeMergeTest
{
    public class SubTreeMergeModelFixture
    {
        internal string mgaFile = null;

        public SubTreeMergeModelFixture()
        {
            this.mgaFile = ImportXME2Mga();
        }

        public static string ImportXME2Mga()
        {
            var xmePath = Path.Combine("..", "..", "SubTreeMergeModel.xme");
            string projectConnStr;
            MgaUtils.ImportXMEForTest(xmePath, out projectConnStr);
            return projectConnStr.Substring("MGA=".Length);
        }
    }

    public class SubTreeMergeTest : IUseFixture<SubTreeMergeModelFixture>
    {
        [STAThread]
        static void Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                System.Reflection.Assembly.GetAssembly(typeof(SubTreeMergeTest)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
        }

        private string mgaFile { get { return this.fixture.mgaFile; } }
        private SubTreeMergeModelFixture fixture;

        public void SetFixture(SubTreeMergeModelFixture data)
        {
            this.fixture = data;
        }

        [Fact]
        void TestSubTreeMergeModel()
        {
            string mgaFileCopy = mgaFile + "copy.mga";
            File.Copy(mgaFile, mgaFile + "copy.mga", true);
            MgaProject project = new MgaProject();
            project.OpenEx("MGA=" + this.mgaFile, "CyPhyML", null);
            try
            {
                project.BeginTransactionInNewTerr(transactiontype_enum.TRANSACTION_NON_NESTED);
                try
                {
                    string mergeMeModelPath = "/@ComponentAssemblies/@MergeMe";
                    MgaFCO mergeMe = (MgaFCO)project.ObjectByPath[mergeMeModelPath];
                    int startingDescendantFCOs = ((MgaModel)mergeMe).GetDescendantFCOs(mergeMe.Project.CreateFilter()).Count;

                    var fcos = ((MgaModel)mergeMe).GetDescendantFCOs(project.CreateFilter()).Cast<MgaFCO>().Select(
                        x => x.AbsPath).ToList();

                    foreach (MgaObject obj in mergeMe.ChildObjects)
                    {
                        if (obj.MetaBase.Name != "Connector" && obj.MetaBase.Name != "Axis" /* keep these, since they have refport connections*/
                            && obj.Status == (int)objectstatus_enum.OBJECT_EXISTS /* connections may be deleted when their endpoints are */)
                        {
                            obj.DestroyObject();
                        }
                    }

                    var subTreeMerge = new SubTreeMerge.SubTreeMerge();
                    subTreeMerge.merge(mergeMe, mgaFileCopy);
                    Assert.Equal(SubTreeMerge.SubTreeMerge.Errors.NoError, subTreeMerge.exitStatus);
                    MgaFCO newMergeMe = (MgaFCO)project.ObjectByPath[mergeMeModelPath];

                    Assert.Equal(newMergeMe.ID, ((MgaReference)project.ObjectByPath["/@ComponentAssemblies/@AsmWithRef/@MergeMe"]).Referred.ID);
                    Assert.Equal(newMergeMe.ID, ((MgaReference)project.ObjectByPath["/@Testing/@TestBench/@MergeMe"]).Referred.ID);
                    Assert.Equal(2, ((MgaReference)project.ObjectByPath["/@ComponentAssemblies/@AsmWithRef/@MergeMe"]).UsedByConns.Count);
                    Assert.Equal(2, ((MgaReference)project.ObjectByPath["/@Testing/@TestBench/@MergeMe"]).UsedByConns.Count);
                    //Debugging:
                    //var fcos2 = ((MgaModel)newMergeMe).GetDescendantFCOs(project.CreateFilter()).Cast<MgaFCO>().Select(x => x.AbsPath)
                    //    .Where(x => fcos.Contains(x) == false);
                    //HashSet<string> originalObjects = new HashSet<string>();
                    //foreach (string abspath in fcos)
                    //    originalObjects.Add(abspath);
                    //foreach (string abspath in ((MgaModel)newMergeMe).GetDescendantFCOs(project.CreateFilter()).Cast<IMgaFCO>().Select(x => x.AbsPath))
                    //    originalObjects.Remove(abspath);
                    // Console.Out.WriteLine(string.Join("\n", originalObjects));
                    Assert.Equal(startingDescendantFCOs, ((MgaModel)newMergeMe).GetDescendantFCOs(project.CreateFilter()).Count);
                }
                finally
                {
                    project.CommitTransaction();
                }
            }
            finally
            {
                if (System.Diagnostics.Debugger.IsAttached)
                {
                    project.Save(project.ProjectConnStr + "_testoutput.mga", true);
                }
                project.Close(true);
            }
        }
    }
}
