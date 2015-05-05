using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using Xunit;
using System.IO;

namespace MasterInterpreterTest.FunctionalTests
{
    public class GetConfigurations
    {
        //[Fact]
        public void TestCodeSample()
        {
            string ProjectConnStr;
            MgaUtils.ImportXMEForTest(Path.GetFullPath(@"..\..\..\..\models\DynamicsTeam\MasterInterpreter\MasterInterpreter.xme"), out ProjectConnStr);

            MgaProject project = new MgaProject();
            bool ro_mode;
            project.Open(ProjectConnStr, out ro_mode);

            MgaHelper.CheckParadigmVersionUpgrade(project);

            try
            {
                List<IMgaFCO> objectsToGetConfigurations = null;

                project.BeginTransactionInNewTerr();
                try
                {
                    // discover objects

                    var allObjects = project
                        .RootFolder
                        .ChildFolders
                        .Cast<MgaFolder>()
                        .Where(x => x.Name.StartsWith("0"))
                        .SelectMany(x => x.GetDescendantFCOs(project.CreateFilter()).Cast<IMgaFCO>())
                        .Where(x => x.RootFCO == x);

                    // get all objects from folders starts with 0 within the root folder.
                    objectsToGetConfigurations = allObjects.Where(x => x.AbsPath.Contains("TestingGetConfigurations")).ToList();

                    objectsToGetConfigurations.Sort((x, y) =>
                    {
                        return x.Meta.Name.CompareTo(y.Meta.Name) != 0 ?
                            x.Meta.Name.CompareTo(y.Meta.Name) :
                            x.AbsPath.CompareTo(y.AbsPath);
                    });
                }
                finally
                {
                    project.AbortTransaction();
                }

                Assert.True(objectsToGetConfigurations != null, "There are no object in the project that has to be checked.");

                int numContexts = objectsToGetConfigurations.Count;
                int numSuccess = 0;
                int numFailures = 0;

                bool success = true;

                foreach (var subject in objectsToGetConfigurations)
                {
                    // single test
                    using (var masterInterpreter = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI(project))
                    {

                        IMgaFCOs configurations = null;

                        Assert.ThrowsDelegate d = () =>
                        {
                            configurations = masterInterpreter.GetConfigurations(subject as MgaModel);
                        };

                        Assert.DoesNotThrow(d);
                        //Assert.True(configurations != null, "GetConfiguration returned with null.");

                        if (configurations == null)
                        {
                            numFailures++;
                        }
                        else
                        {
                            numSuccess++;
                        }

                        // print out nicely in the GME console
                        project.BeginTransactionInNewTerr();
                        try
                        {
                            Console.Out.WriteLine("{0} [{1}] has {2} configurations.", subject.Name, subject.Meta.Name, configurations.Count);
                            foreach (IMgaFCO configuration in configurations)
                            {
                                Console.Out.WriteLine(" > {0} - {1}", configuration.Name, configuration.ID);
                            }

                        }
                        finally
                        {
                            project.AbortTransaction();
                        }
                    }
                }

                if (success)
                {
                    Console.Out.WriteLine("[OK] Got configurations for: {0} test benches. Success: {1}, Failed {2}", numContexts, numSuccess, numFailures);
                }
                else
                {
                    Console.Error.WriteLine("[FAILED] Tried to get configurations for: {0} test benches. Success: {1}, Failed {2}", numContexts, numSuccess, numFailures);
                }

                Assert.True(success, "At least one context was failed to check against the expected results.");

            }
            finally
            {
                project.Close(true);
            }



        }


    }
}
