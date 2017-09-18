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
using System.Collections.Concurrent;

namespace CyPhyPropagateTest
{

    public class MetaLinkCreoTest : MetaLinkTestBase, IUseFixture<CADCreoTest.Cyphy2CADCreoTest.MetaLinkFixture>, IDisposable
    {
        private CADCreoTest.Cyphy2CADCreoTest.MetaLinkFixture fixture;

        public void SetFixture(CADCreoTest.Cyphy2CADCreoTest.MetaLinkFixture fixture)
        {
            this.fixture = fixture;
        }

        protected BlockingCollection<string> errors = new System.Collections.Concurrent.BlockingCollection<string>(new ConcurrentQueue<string>());

        private void ExeStartupFailed()
        {
            errors.Add("CADCreoParametricMetaLink.exe returned with error.");
        }

        private void ThrowDeferredErrors()
        {
            string error;
            if (errors.TryTake(out error))
            {
                Assert.True(false, error);
            }
        }

        [Fact]
        public void TestStartupComponent()
        {
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
                        ThrowDeferredErrors();
                        Application.DoEvents();
                        ts = DateTime.Now - t1;
                        Edit e;
                        while (addonMessagesQueue.TryTake(out e, 500))
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
                    } while (ts.TotalSeconds < 90 && !success);
                    Assert.True(success);
                }
            );
        }

        [Fact]
        public void TestStartupAssembly()
        {
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
                        ThrowDeferredErrors();
                        Application.DoEvents();
                        ts = DateTime.Now - t1;
                        Edit e;
                        while (addonMessagesQueue.TryTake(out e, 500))
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
                    } while (ts.TotalSeconds < 90 && !success);
                    Assert.True(success);
                }
            );
        }

        [Fact]
        public void TestStartupAssemblyHierarchy()
        {
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
                        ThrowDeferredErrors();
                        Application.DoEvents();
                        ts = DateTime.Now - t1;
                        Edit e;
                        while (addonMessagesQueue.TryTake(out e, 500))
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
                    } while (ts.TotalSeconds < 90 && !success);
                    Assert.True(success);
                }
            );
        }

        public void Dispose()
        {
            errors.Dispose();
        }
    }
}
