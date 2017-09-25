using System;
using System.Collections.Generic;
using System.Text;
using GME.MGA;
using GME.CSharp;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using CyPhyMasterInterpreter;

namespace CyPhyMasterExe
{
    public class Program
    {
        [STAThread]
        public static void Main(string[] args)
        {
            try
            {
                // parse command line arguments
                string jobCollectionId = new Guid().ToString("D");
                if (args[0] == "--job-collection-id")
                {
                    jobCollectionId = args[1];
                    args = args.Skip(2).ToArray();
                }
                if (args[0] == "--send-job-collection-done")
                {
                    var dispatch = new JobManagerDispatch(jobCollectionId);
                    dispatch.StartJobManager(Environment.GetFolderPath(Environment.SpecialFolder.Desktop));
                    dispatch.Done();
                    return;
                }
                string projectConnStr = args[0];
                string originalSubjectID = args[1];
                string[] configIDs = args.Skip(2).ToArray();

                if (projectConnStr.StartsWith("MGA=") == false)
                {
                    // use the full absolute path
                    projectConnStr = "MGA=" + Path.GetFullPath(projectConnStr);
                }
                
                MgaProject project = new MgaProject();
                bool ro_mode;
                project.Open(projectConnStr, out ro_mode);

                try
                {
                    // get an instance of the master interpreter
                    using (var master = new CyPhyMasterInterpreter.CyPhyMasterInterpreterAPI(project))
                    {
                        master.SetJobCollectionID(jobCollectionId);
                        // create a configuration for the run
                        var configLight = new CyPhyMasterInterpreter.ConfigurationSelectionLight();
                        configLight.ContextId = originalSubjectID;
                        configLight.SelectedConfigurationIds = configIDs;
                        configLight.KeepTemporaryModels = false;
                        configLight.PostToJobManager = true;

                        // run master interpreter on configuration
                        var results = master.RunInTransactionWithConfigLight(configLight);

                        // summarize results
                        master.WriteSummary(results);
                        System.Environment.Exit(results.Where(r => r.Success == false).Count());
                    }
                }
                finally
                {
                    project.Close(true);
                }
            }
            catch (Exception e)
            {
                System.Console.Error.WriteLine(e.ToString());
                System.Environment.Exit(5);
            }
        }
    }
}
