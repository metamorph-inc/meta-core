using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using GME.MGA;
using edu.vanderbilt.isis.meta;
using CyPhyMetaLinkBridgeClient;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Threading;
using GME.CSharp;
using Xunit;
using Microsoft.Win32.SafeHandles;
using Microsoft.Win32;
using System.Net.Sockets;
using System.Net;
using System.Threading.Tasks;
using CyPhyGUIs;

namespace CyPhyPropagateTest
{
    public class MetaLinkTestBase
    {
        protected string OriginalTestModelDir
        {
            get
            {
                return Path.Combine("..", "..", "..", "..", "models", "MetaLinkTest");
            }
        }
        protected virtual string TestModelDir
        {
            get
            {
                return Path.Combine(Path.GetTempPath(), "CyPhyMLTest_" + this.GetType().Name);
            }
        }

        public string MetaLinkLogFilename { get; private set; }

        protected List<Edit> receivedMessages;
        protected BlockingCollection<Edit> receivedMessagesQueue;
        protected BlockingCollection<Edit> sentMessagesQueue;
        protected BlockingCollection<Edit> addonMessagesQueue = new System.Collections.Concurrent.BlockingCollection<Edit>(new ConcurrentQueue<Edit>());
        protected MetaLinkBridgeClient testingClient;
        protected Process metalink;
        protected IntPtr metalinkJob;
        protected StreamWriter metalinkLogStream;


        protected static readonly string testAVMId = "AVM.Component:50ec54a86e6cc33768468c31"; // AVMID of Damper_2
        protected static readonly string testAssemblyId = "d28b3661-f32e-493d-8fd3-0ddc1a64a384";
        protected static readonly string testAssemblyHierarchy_2Id = "a8381762-8cb8-426f-811f-6f1137f7ec7a";
        protected static readonly string testInputFilename = @"\MSDTutorial2_1.mga";
        protected static readonly string testXMEFilename = "MSDTutorial2_1.xme";
        protected static readonly string testComponentPath = "/@Imported_Components/@Damper_2";
        protected static readonly string testAssemblyPath = "/@Generated configurations/@MyMassSpringDamper/@MyMassSpringDamper_cfg1";
        protected static readonly string testAssemblyPath2 = "/@Generated configurations/@MyMassSpringDamper/@MassSpringDamper_Hierarchy";
        protected static readonly string origin = "CyPhyMetaLinkTest";
        protected static readonly string massInstanceGuid = "{bdb724ab-cb47-47ae-ad3e-7ea5fcd42333}";
        protected static readonly string springInstanceGuid = "{cbfb58bf-127e-4843-9d0c-8879953c5a9b}";
        protected static readonly string massName = "Mass__Mass_Steel";
        protected static readonly string springName = "Spring__Spring_Titanium";

        protected static bool debugMetalinkStartup = false;

        protected void SetupTest()
        {
            CyPhyGUIs.CyPhyDirectory.EnsureEmptyDirectory(TestModelDir);
            StartMetaLinkBridge();
            try
            {
                System.Action d = (System.Action)CopyTestModel;
                var res = d.BeginInvoke(null, null);
                WaitForMetaLinkBridgeToBeReady();
                StartTestingClient();
                d.EndInvoke(res);
            }
            catch
            {
                KillMetaLink();
                lock (metalinkLogStream)
                    metalinkLogStream.Dispose();
                throw;
            }
        }


        protected void CopyTestModel()
        {
            CyPhyMetaLink.CyphyMetaLinkUtils.CopyDirectory(OriginalTestModelDir, TestModelDir);
        }

        protected MgaFCO GetTestAssembly(MgaProject project)
        {
            var filter = project.CreateFilter();
            filter.Kind = "ComponentAssembly";
            return project.AllFCOs(filter).Cast<MgaFCO>().Where(fco => fco.GetGuidDisp() == new Guid(testAssemblyId).ToString("B")).First();
        }

        protected void SendInterest(params string[] topics)
        {
            Edit interest = new Edit();
            interest.mode.Add(Edit.EditMode.INTEREST);
            interest.origin.Add(origin);
            interest.topic.AddRange(topics);
            testingClient.SendToMetaLinkBridge(interest);
        }

        protected void KillMetaLink()
        {
            lock (this)
            {
                if (metalink != null)
                {
                    if (metalink.HasExited == false)
                    {
                        metalink.Kill();
                    }
                    metalink.WaitForExit();
                    metalink.Dispose();
                    metalink = null;
                    JobObjectPinvoke.CloseHandle(metalinkJob);
                    metalinkJob = IntPtr.Zero;
                }
            }
        }

        protected void SendToMetaLinkBridgeAndWaitForAddonToReceive(Edit msg)
        {
            msg.guid = Guid.NewGuid().ToString();
            testingClient.SendToMetaLinkBridge(msg);

            Edit recvd;
            while (addonMessagesQueue.TryTake(out recvd, 5000))
            {
                if (recvd.guid == msg.guid)
                {
                    return;
                }
            }
            throw new TimeoutException();
        }

        protected void RunCyPhyMLSync(System.Action<MgaProject, CyPhyMetaLink.CyPhyMetaLinkAddon, CyPhyMetaLink.CyPhyMetalinkInterpreter> testAction)
        {
            try
            {
                if (debugMetalinkStartup)
                {
                    metalink.WaitForExit();
                    string stderr = metalink.StandardError.ReadToEnd();
                    string stdout = metalink.StandardOutput.ReadToEnd();
                }
                Exception exception = null;
                AutoResetEvent workEvent = new AutoResetEvent(false);
                Thread work = new Thread(new ThreadStart(delegate
                {

                    try
                    {
                        // Import XME file to create an MGA
                        String xmeFullPath = Path.Combine(TestModelDir, testXMEFilename);
                        String mgaFullPath = TestModelDir + testInputFilename;
                        MgaUtils.ImportXME(xmeFullPath, mgaFullPath);
                        Assert.True(File.Exists(mgaFullPath), "MGA file not found. XME import may have failed.");

                        MgaProject project = new MgaProject();
                        project.EnableAutoAddOns(true);
                        project.OpenEx("MGA=" + mgaFullPath, "", true);
                        CyPhyMetaLink.CyPhyMetaLinkAddon propagate = null;
                        try
                        {
                            Assert.Contains("MGA.Addon.CyPhyMLPropagate", project.AddOnComponents.Cast<IMgaComponentEx>().Select(x => x.ComponentProgID));
                            propagate = (CyPhyMetaLink.CyPhyMetaLinkAddon)project.AddOnComponents.Cast<IMgaComponent>().Where(comp => comp is CyPhyMetaLink.CyPhyMetaLinkAddon).FirstOrDefault();
                            CyPhyMetaLink.CyPhyMetalinkInterpreter interpreter = new CyPhyMetaLink.CyPhyMetalinkInterpreter();
                            propagate.TestMode = true;
                            propagate.TestMode_NoAutomaticCreoStart = true;
                            propagate.TestMode_CreoJobObject = JobObjectPinvoke.CreateKillOnCloseJob();
                            interpreter.GMEConsole = GME.CSharp.GMEConsole.CreateFromProject(project);
                            interpreter.MgaGateway = new MgaGateway(project);

                            var task = Task.Run(async () => await interpreter.ConnectToMetaLinkBridge(project, 128));
                            task.Wait();
                            propagate.bridgeClient.SocketQueue.EditMessageReceived += msg => addonMessagesQueue.Add(msg);
                            testAction(project, propagate, interpreter);
                        }
                        finally
                        {
                            if (propagate != null)
                            {
                                JobObjectPinvoke.CloseHandle(propagate.TestMode_CreoJobObject);
                                propagate.TestMode_CreoJobObject = IntPtr.Zero;
                            }
                            project.Save(project.ProjectConnStr + "_posttest.mga", true);
                            project.Close(true);
                        }
                    }
                    catch (Exception e)
                    {
                        exception = e;
                        KillMetaLink();
                    }
                    finally
                    {
                        workEvent.Set();
                    }
                }));
                work.SetApartmentState(ApartmentState.STA);
                work.Start();

                ManualResetEvent metalinkEvent = new ManualResetEvent(true);
                metalinkEvent.SafeWaitHandle = new SafeWaitHandle(metalink.Handle, false);

                int handle = WaitHandle.WaitAny(new WaitHandle[] { metalinkEvent, workEvent });
                if (exception != null)
                {
                    throw new Exception("Test failed", exception);
                }
                if (handle == 0)
                {
                    work.Abort();
                    throw new Exception("metalink exited");
                }
            }
            finally
            {
                KillMetaLink();
                lock (metalinkLogStream)
                    metalinkLogStream.Dispose();
            }
        }

        private string GetJavaInstallationPath()
        {
            string javaPath = CyPhyMetaLink.CyPhyMetalinkInterpreter.GetJavaInstallationPath();
            if (javaPath == null)
            {
                Assert.False(true, "Could not find Java installation");
            }
            return javaPath;
        }

        ManualResetEvent metalinkReady;
        public BlockingCollection<string> metalinkOutput = new BlockingCollection<string>(new ConcurrentQueue<string>());
        protected void StartMetaLinkBridge()
        {
            foreach (string filename in new string[] { "CyPhyPropagateTest_recorded_messages.mlp", "CyPhyPropagateTest_Hull_and_Hook.mga_posttest.mga" })
            {
                File.Delete(Path.Combine(TestModelDir, filename));
            }
            string java_exe = Path.Combine(GetJavaInstallationPath(), "bin\\java.exe");
            string metaLinkPath = Path.Combine(META.VersionInfo.MetaPath, @"src\MetaLink\meta-bridge\java-server\target\metalink-java-server-1.1.0.jar"); // dev machine
            if (!File.Exists(metaLinkPath))
            {
                metaLinkPath = Path.Combine(META.VersionInfo.MetaPath, @"bin\metalink-java-server-1.1.0.jar"); // installed machine
            }

            ProcessStartInfo info = new ProcessStartInfo()
            {
                // java -jar C:\Users\meta\Documents\META\src\MetaLink\meta-bridge\java-server\target\metalink-java-server-1.1.0.jar -p C:\Users\meta\Documents\META_MetaLink_HullandHook\partial-component.mlp -r C:\Users\meta\Documents\META_MetaLink_HullandHook\CyPhyPropagateTest_recorded_messages.mlp
                FileName = java_exe,
                Arguments = "-jar \"" + metaLinkPath + "\"" +
                    " -P 0" +
                    //" -p " + HullAndHookDir + @"\CyPhyPropagateTest_partial-component.mlp" +
                    //" -r " + HullAndHookDir + @"\CyPhyPropagateTest_recorded_messages.mlp"
                    "",
                RedirectStandardError = true,
                RedirectStandardOutput = true,
                UseShellExecute = false,
                RedirectStandardInput = true,
                CreateNoWindow = true,
            };
            metalink = new Process();
            metalink.StartInfo = info;
            this.MetaLinkLogFilename = Path.Combine(TestModelDir, "CyPhyPropagateTest_" + Process.GetCurrentProcess().Id + ".txt");
            FileStream outlog = File.Open(MetaLinkLogFilename, FileMode.Create, FileAccess.Write, FileShare.Read);
            metalinkLogStream = new StreamWriter(outlog, Encoding.UTF8);
            metalinkLogStream.AutoFlush = true;
            int metalinkLogStreamNo = 2;
            System.Action outputClosed = () =>
            {
                if (Interlocked.Decrement(ref metalinkLogStreamNo) == 0)
                {
                    lock (metalinkLogStream)
                    {
                        if (metalinkLogStream.BaseStream != null)
                        {
                            metalinkLogStream.Close();
                        }
                    }
                }
            };
            try
            {
                // FIXME: look for "exception caught"
                metalink.ErrorDataReceived += (o, dataArgs) =>
                {
                    if (dataArgs.Data == null)
                    {
                        outputClosed();
                        return;
                    }
                    Console.Error.WriteLine(dataArgs.Data);
                    lock (metalinkLogStream)
                    {
                        if (metalinkLogStream.BaseStream != null) // i.e. if not closed
                        {
                            metalinkLogStream.WriteLine(dataArgs.Data);
                        }
                    }
                };
                metalinkReady = new ManualResetEvent(false);
                metalink.OutputDataReceived += (o, dataArgs) =>
                {
                    if (dataArgs.Data == null)
                    {
                        outputClosed();
                        return;
                    }
                    Console.Out.WriteLine(dataArgs.Data);
                    lock (metalinkLogStream)
                    {
                        if (metalinkLogStream.BaseStream != null) // i.e. if not closed
                        {
                            metalinkLogStream.WriteLine(dataArgs.Data);
                        }
                    }
                    if (dataArgs.Data.Contains("AssemblyDesignBridgeServer started and listening on"))
                    {
                        var substr = "listening on /127.0.0.1:";
                        var index = dataArgs.Data.IndexOf(substr);
                        SocketQueue.port = int.Parse(dataArgs.Data.Substring(index + substr.Length));
                        metalinkReady.Set();
                    }
                    metalinkOutput.Add(dataArgs.Data);
                };
                metalink.Start();
                metalinkJob = JobObjectPinvoke.AssignProcessToKillOnCloseJob(metalink);
                metalink.BeginOutputReadLine();
                metalink.BeginErrorReadLine();
            }
            finally
            {
            }
        }

        public class ProcessWaitHandle : WaitHandle
        {
            private readonly Process process;
            public ProcessWaitHandle(Process process)
            {
                this.process = process;
                this.SafeWaitHandle = new SafeWaitHandle(process.Handle, false);
            }
        }

        protected void WaitForMetaLinkBridgeToBeReady()
        {
            // n.b. This timeout needs to be really long. java.exe consumes .5GiB memory and reads tons of .jars
            // java.exe also contends with other test processes run in parallel
            if (WaitHandle.WaitAny(new WaitHandle[] { metalinkReady, new ProcessWaitHandle(metalink) }, 5 * 60 * 1000) != 0)
            {
                using (FileStream outlog = File.Open(MetaLinkLogFilename, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                {
                    var logContents = new StreamReader(outlog, Encoding.UTF8).ReadToEnd();

                    if (metalink.HasExited && metalink.ExitCode != 0)
                    {
                        throw new ApplicationException("'java.exe metalink-java-server' exited with code " + metalink.ExitCode + "\n" + logContents);
                    }
                    throw new TimeoutException(logContents);
                }
            }
        }

        protected void StartTestingClient()
        {
            testingClient = new MetaLinkBridgeClient();
            receivedMessages = new List<Edit>();
            receivedMessagesQueue = new System.Collections.Concurrent.BlockingCollection<Edit>(new ConcurrentQueue<Edit>());
            sentMessagesQueue = new System.Collections.Concurrent.BlockingCollection<Edit>(new ConcurrentQueue<Edit>());
            Task<bool> startClient = Task.Run(async () =>
                await testingClient.EstablishConnection(msg =>
                    {
                        lock (receivedMessages)
                        {
                            receivedMessages.Add(msg);
                        }
                        receivedMessagesQueue.Add(msg);
                    },
                    connectionClosed: exc => KillMetaLink(),
                    EditMessageSent: msg =>
                    {
                        sentMessagesQueue.Add(msg);
                    }
                )
            );
            startClient.Wait();
        }

        protected void WaitForAllMetaLinkMessages()
        {
            Edit msg = new Edit();
            msg.origin.Add(origin);
            msg.mode.Add(Edit.EditMode.INTEREST);
            Guid msgTopic = Guid.NewGuid();
            msg.topic.Add(msgTopic.ToString());
            testingClient.SendToMetaLinkBridge(msg);
            while (true)
            {
                Edit received;
                if (this.receivedMessagesQueue.TryTake(out received, 5 * 1000) == false)
                    throw new TimeoutException();
                if (received.topic.SequenceEqual(new string[] { msgTopic.ToString() }))
                {
                    lock (receivedMessages)
                    {
                        receivedMessages.Remove(received);
                    }
                    break;
                }
            }
        }


    }
}
