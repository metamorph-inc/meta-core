using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;
using edu.vanderbilt.isis.meta;
using GME.CSharp;
using GME.MGA;
using System.Diagnostics;
using CyPhyPropagateTest;
using System.Windows.Forms;
using System.Threading.Tasks;

namespace CyPhyPropagateTest
{

    public class MetaLinkCreoTest : MetaLinkTestBase, IUseFixture<CADCreoTest.Cyphy2CADCreoTest.MetaLinkFixture>
    {
        private CADCreoTest.Cyphy2CADCreoTest.MetaLinkFixture fixture;

        public void SetFixture(CADCreoTest.Cyphy2CADCreoTest.MetaLinkFixture fixture)
        {
            this.fixture = fixture;
        }

        public static void KillCreo()
        {
            try
            {
	            Process[] proc = Process.GetProcessesByName("xtop");
                if (proc.Length != 0)
                {
                    proc[0].Kill();
                    proc[0].WaitForExit();
                }
                proc = Process.GetProcessesByName("nmsd");
                if (proc.Length != 0)
                {
                    proc[0].Kill();
                    proc[0].WaitForExit();
                }
                proc = Process.GetProcessesByName("CADCreoParametricMetaLink");
                if (proc.Length != 0)
                {
                    proc[0].Kill();
                    proc[0].WaitForExit();
                }
            }
            catch (Exception)

            {
            }
        }

        private void ExeStartupFailed()
        {
            Assert.True(false, "CADCreoParametricMetaLink.exe returned with error.");
        }

        [Fact]
        public void TestStartupComponent()
        {
            KillCreo();
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    try
                    {
                        project.BeginTransactionInNewTerr();
                        var component = ISIS.GME.Dsml.CyPhyML.Classes.Component.Cast((MgaModel)project.RootFolder.ObjectByPath[testComponentPath]);
                        var cadModel = component.Children.CADModelCollection.First();
                        propagate.TestMode_NoAutomaticCreoStart = false;
                        propagate.StartupFailureCallback = this.ExeStartupFailed;
                        project.AbortTransaction();
                        propagate.StartEditingComponent(component, (MgaFCO)cadModel.Impl, false);
                    }
                    finally
                    {
                        //project.AbortTransaction();
                    }
                    DateTime t1 = DateTime.Now;
                    TimeSpan ts;
                    bool success = false;
                    do
                    {
                        Application.DoEvents();
                        ts = DateTime.Now - t1;
                        Edit e;
                        while (addonMessagesQueue.TryTake(out e))
                        {
                            if (e.actions.Count > 0 && e.actions[0].notices.Count > 0)
                            {
                                if (e.actions[0].notices[0].noticeMode == Notice.NoticeMode.DONE)
                                {
                                    success = true;
                                    break;
                                }
                            }
                        }
                    } while (ts.TotalSeconds < 45 && !success);
                    Assert.True(success);
                }
            );
            KillCreo();
        }

        [Fact]
        public void TestStartupAssembly()
        {
            KillCreo();
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    try
                    {
                        project.BeginTransactionInNewTerr();
                        var assembly = ISIS.GME.Dsml.CyPhyML.Classes.ComponentAssembly.Cast((MgaModel)project.RootFolder.ObjectByPath[testAssemblyPath]);
                        project.AbortTransaction();
                        propagate.TestMode_NoAutomaticCreoStart = false;
                        propagate.StartupFailureCallback = this.ExeStartupFailed;
                        interpreter.StartAssemblySync(project, (MgaFCO)assembly.Impl, 128);
                    }
                    finally
                    {
//                        project.AbortTransaction();
                    }
                    DateTime t1 = DateTime.Now;
                    TimeSpan ts;
                    bool success = false;
                    do
                    {
                        Application.DoEvents();
                        ts = DateTime.Now - t1;
                        Edit e;
                        while (addonMessagesQueue.TryTake(out e))
                        {
                            if (e.actions.Count > 0 && e.actions[0].notices.Count > 0)
                            {
                                if (e.actions[0].notices[0].noticeMode == Notice.NoticeMode.DONE)
                                {
                                    success = true;
                                    break;
                                }
                            }
                        }
                    } while (ts.TotalSeconds < 45 && !success);
                    Assert.True(success);
                }
            );
            KillCreo();
        }

        [Fact]
        public void TestStartupAssemblyHierarchy()
        {
            KillCreo();
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    try
                    {
                        project.BeginTransactionInNewTerr();
                        var assembly = ISIS.GME.Dsml.CyPhyML.Classes.ComponentAssembly.Cast((MgaModel)project.RootFolder.ObjectByPath[testAssemblyPath2]);
                        project.AbortTransaction();
                        propagate.TestMode_NoAutomaticCreoStart = false;
                        propagate.StartupFailureCallback = this.ExeStartupFailed;
                        interpreter.StartAssemblySync(project, (MgaFCO)assembly.Impl, 128);
                    }
                    finally
                    {
//                        project.AbortTransaction();
                    }
                    DateTime t1 = DateTime.Now;
                    TimeSpan ts;
                    bool success = false;
                    do
                    {
                        Application.DoEvents();
                        ts = DateTime.Now - t1;
                        Edit e;
                        while (addonMessagesQueue.TryTake(out e))
                        {
                            if (e.actions.Count > 0 && e.actions[0].notices.Count > 0)
                            {
                                if (e.actions[0].notices[0].noticeMode == Notice.NoticeMode.DONE)
                                {
                                    success = true;
                                    break;
                                }
                            }
                        }
                    } while (ts.TotalSeconds < 45 && !success);
                    Assert.True(success);
                }
            );
            KillCreo();
        }
    }
}
