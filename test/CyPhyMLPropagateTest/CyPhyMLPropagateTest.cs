using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using GME.MGA;
using GME.CSharp;
using System.Threading;
using Microsoft.Win32.SafeHandles;
using System.Windows.Forms;
using System.IO;
using CyPhyMetaLinkBridgeClient;
using edu.vanderbilt.isis.meta;
using Xunit;
using System.Collections.Concurrent;
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using System.Xml.XPath;
using System.Xml;
using CyPhyMetaLink;

namespace CyPhyPropagateTest
{
    public class CyPhyPropagateTest : MetaLinkTestBase
    {


        static void Main(string[] args)
        {
            int ret = Xunit.ConsoleClient.Program.Main(new string[] {
                System.Reflection.Assembly.GetAssembly(typeof(CyPhyPropagateTest)).CodeBase.Substring("file:///".Length),
                //"/noshadow",
            });
            Console.In.ReadLine();
            //System.Console.Out.WriteLine("HEllo World");
        }


        private static string PrepareComponentUpdateXml(string xmlpath, IDictionary<string, string> paths)
        {
            string xml = File.ReadAllText(xmlpath);

            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xml);

            XmlNamespaceManager manager = new XmlNamespaceManager(doc.NameTable);
            manager.AddNamespace("avm", "avm");
            manager.AddNamespace("cad", "cad");

            XPathNavigator navigator = doc.CreateNavigator();
            var resourceDependencies = navigator.Select("/avm:Component/avm:ResourceDependency", manager).Cast<XPathNavigator>()
                .Concat(navigator.Select("/avm:Component/ResourceDependency", manager).Cast<XPathNavigator>());
            
            foreach (XPathNavigator node in resourceDependencies)
            {
                string path = node.GetAttribute("Path", "avm");
                if (String.IsNullOrWhiteSpace(path))
                {
                    path = node.GetAttribute("Path", "");
                }
                string newpath;
                if (paths.TryGetValue(node.GetAttribute("Name", ""), out newpath))
                {
                    node.MoveToAttribute("Path", "");
                    node.SetValue(newpath);
                }
            }
            StringBuilder sb = new StringBuilder();
            XmlTextWriter w = new XmlTextWriter(new StringWriter(sb));
            doc.WriteContentTo(w);
            w.Flush();
            return sb.ToString();
        }

        [Fact]
        // Update existing AVM component - an existing resource has been replaced
        public void TestUpdateComponentReplaceResource()
        {
            SetupTest();

            SendInterest("0");
            WaitToReceiveMessage();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    string testCompGuid = null;
                    {
                        interpreter.MgaGateway.PerformInTransaction(() =>
                            testCompGuid = (string)((MgaModel)project.RootFolder.ObjectByPath[testComponentPath]).GetAttributeByNameDisp("AVMID"));
                        //SendInterest(CyPhyMetaLink.CyPhyMetaLinkAddon.ComponentTopic, testCompGuid);
                        //WaitToReceiveMessage();
                    }

                    // TODO propagate.StartEditingComponent
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");

                        msg.actions.Add(new edu.vanderbilt.isis.meta.Action());
                        msg.actions[0].subjectID = testCompGuid;
                        msg.actions[0].actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.UPDATE_CYPHY_COMPONENT;
                        msg.actions[0].alien = new Alien();
                        msg.actions[0].alien.encodingMode = Alien.EncodingMode.XML;
                        msg.actions[0].alien.encoded = Encoding.UTF8.GetBytes(
                            PrepareComponentUpdateXml(Path.Combine(TestModelDir, @"UpdateComponentData3.xml"),
                            new Dictionary<string, string>() { 
                            { "test.asm", Path.Combine(TestModelDir, "") },
                            }));
                        msg.actions[0].subjectID = testCompGuid;
                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();

                        interpreter.MgaGateway.PerformInTransaction(() =>
                        {
                            var component = ISIS.GME.Dsml.CyPhyML.Classes.Component.Cast((MgaModel)project.RootFolder.ObjectByPath[testComponentPath]);
                            var cadModel = component.Children.CADModelCollection.First();
                            Assert.Equal(4, cadModel.Children.CADParameterCollection.Count());
                            Assert.Equal(9, cadModel.Children.CADDatumCollection.Count());
                            VerifyResources(component, new string[] { "cadxxx\\test.asm", "manufacturing\\damper_2.xml" });
                        });

                    }
                });
        }


        [Fact]
        // Update existing AVM component - a new resource has been added
        public void TestUpdateComponentNewResource()
        {
            SetupTest();

            SendInterest("0");
            WaitToReceiveMessage();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    string testCompGuid = null;
                    {
                        interpreter.MgaGateway.PerformInTransaction(() =>
                            testCompGuid = (string)((MgaModel)project.RootFolder.ObjectByPath[testComponentPath]).GetAttributeByNameDisp("AVMID"));
                    }

                    // TODO propagate.StartEditingComponent
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");

                        msg.actions.Add(new edu.vanderbilt.isis.meta.Action());
                        msg.actions[0].actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.UPDATE_CYPHY_COMPONENT;
                        msg.actions[0].alien = new Alien();
                        msg.actions[0].alien.encodingMode = Alien.EncodingMode.XML;
                        msg.actions[0].alien.encoded = Encoding.UTF8.GetBytes(
                            PrepareComponentUpdateXml(Path.Combine(TestModelDir, @"UpdateComponentData2.xml"),
                            new Dictionary<string, string>() { 
                            { "DAMPER.PRT", Path.Combine(TestModelDir, "components\\Damper_2\\AVM.Component-50ec54a86e6cc33768468c31v2\\CADXXX\\") },
                            { "test.asm", Path.Combine(TestModelDir, "") }
                            }));
                        msg.actions[0].subjectID = testCompGuid;
                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();

                        interpreter.MgaGateway.PerformInTransaction(() =>
                        {
                            var component = ISIS.GME.Dsml.CyPhyML.Classes.Component.Cast((MgaModel)project.RootFolder.ObjectByPath[testComponentPath]);
                            var cadModel = component.Children.CADModelCollection.First();
                            Assert.Equal(4, cadModel.Children.CADParameterCollection.Count());
                            Assert.Equal(9, cadModel.Children.CADDatumCollection.Count());
                            VerifyResources(component, new string[] { "cadxxx\\test.asm", "cadxxx\\damper.prt", "manufacturing\\damper_2.xml" });
                        });

                    }
                });
        }

        [Fact]
        // Update existing AVM component - no resource change
        public void TestUpdateComponent()
        {
            SetupTest();

            SendInterest("0");
            WaitToReceiveMessage();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    string testCompGuid = null;
                    {
                        interpreter.MgaGateway.PerformInTransaction(() =>
                            testCompGuid = (string)((MgaModel)project.RootFolder.ObjectByPath[testComponentPath]).GetAttributeByNameDisp("AVMID"));
                    }

                    // TODO propagate.StartEditingComponent
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");

                        msg.actions.Add(new edu.vanderbilt.isis.meta.Action());
                        msg.actions[0].actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.UPDATE_CYPHY_COMPONENT;
                        msg.actions[0].alien = new Alien();
                        msg.actions[0].alien.encodingMode = Alien.EncodingMode.XML;
                        msg.actions[0].alien.encoded = Encoding.UTF8.GetBytes(PrepareComponentUpdateXml(Path.Combine(TestModelDir, @"UpdateComponentData.xml"), new Dictionary<string, string>() { { "DAMPER.PRT", Path.Combine(TestModelDir, "components\\Damper_2\\AVM.Component-50ec54a86e6cc33768468c31v2\\CADXXX\\") } }));
                        msg.actions[0].subjectID = testCompGuid;
                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();

                        interpreter.MgaGateway.PerformInTransaction(() =>
                        {
                            var component = ISIS.GME.Dsml.CyPhyML.Classes.Component.Cast((MgaModel)project.RootFolder.ObjectByPath[testComponentPath]);
                            var cadModel = component.Children.CADModelCollection.First();
                            Assert.Equal(4, cadModel.Children.CADParameterCollection.Count());
                            Assert.Equal(9, cadModel.Children.CADDatumCollection.Count());
                            VerifyResources(component, new string[] { "cadxxx\\damper.prt", "manufacturing\\damper_2.xml" });
                        });

                    }
                });
        }

        private void VerifyResources(ISIS.GME.Dsml.CyPhyML.Interfaces.Component component, string[] resources)
        {
            Assert.Equal(resources.Length, component.Children.ResourceCollection.Count());
            foreach (var res in component.Children.ResourceCollection)
            {
                Assert.True(resources.Contains(res.Attributes.Path.ToLower()));
            }
        }

        private void WaitToReceiveMessage()
        {
            Edit received;
            if (this.receivedMessagesQueue.TryTake(out received, 5 * 1000) == false)
            {
                throw new TimeoutException();
            }
            System.Console.WriteLine("aa");
        }

        [Fact]
        // Create AVM component from scratch
        void TestCreateComponent()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");

                        msg.actions.Add(new edu.vanderbilt.isis.meta.Action());
                        msg.actions[0].actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.CREATE_CYPHY_COMPONENT;
                        msg.actions[0].alien = new Alien();
                        msg.actions[0].alien.encodingMode = Alien.EncodingMode.XML;
                        msg.actions[0].alien.encoded = Encoding.UTF8.GetBytes(File.ReadAllText(Path.Combine(TestModelDir,"CreateComponentData.xml")));
                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var imported = project.RootFolder.GetObjectByPathDisp("/@Imported_Components");
                        var result = imported.ChildObjects.Cast<IMgaObject>().Count(o => o.Name == "Damper_Test");
                        Xunit.Assert.Equal(1, result);
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        [Fact]
        // Remove a component and see that only one resync message sent
        void TestAssemblySync()
        {
            SetupTest();

            SendInterest(CyPhyMetaLinkAddon.IdCounter.ToString());
            WaitToReceiveMessage();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    MgaFCO assembly;
                    MgaFCO componentRefToDelete;
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        assembly = GetTestAssembly(project);
                        componentRefToDelete = assembly.ChildObjects.Cast<MgaFCO>().Where(fco => fco.Name.Contains("Mass__Mass_Steel")).First();
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                    interpreter.StartAssemblySync(project, assembly, 128);
                    Application.DoEvents();
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        //connectorCompositionToDelete.DestroyObject();
                        componentRefToDelete.DestroyObject();
                    }
                    finally
                    {
                        project.CommitTransaction();
                    }
                    Application.DoEvents();
                    Thread.Sleep(1000); // XXX don't race with propagate's send message thread
                    WaitForAllMetaLinkMessages();
                    
                    Xunit.Assert.Equal(1,
                        this.receivedMessages.Where(
                            msg => msg.actions.Any(a => a.actionMode == edu.vanderbilt.isis.meta.Action.ActionMode.CLEAR)).Count());
                }
            );
        }


        [Fact]
        // Modify a component in design sync mode, and see that exactly one resync message sent
        void TestAssemblySync_ModifyComponent()
        {
            SetupTest();

            SendInterest(CyPhyMetaLinkAddon.IdCounter.ToString());
            WaitToReceiveMessage();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    MgaFCO assembly;
                    MgaFCO connectorToDelete;
                    
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        assembly = GetTestAssembly(project);
                        MgaReference componentRef = (MgaReference) assembly.ChildObjects.Cast<MgaFCO>().Where(fco => fco.Name.Contains("Mass__Mass_Steel")).First();
                        connectorToDelete = ((MgaModel)componentRef.Referred).ChildFCOs.Cast<MgaFCO>().Where(fco => fco.Name == "PIN").First();
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                    interpreter.StartAssemblySync(project, assembly, 128);
                    Application.DoEvents();
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        connectorToDelete.DestroyObject();
                    }
                    finally
                    {
                        project.CommitTransaction();
                    }
                    Application.DoEvents();
                    Thread.Sleep(1000); // XXX don't race with propagate's send message thread
                    WaitForAllMetaLinkMessages();
                    
                    Xunit.Assert.Equal(1,
                        this.receivedMessages.Where(
                            msg => msg.actions.Any(a => a.actionMode == edu.vanderbilt.isis.meta.Action.ActionMode.CLEAR)).Count());
                    Xunit.Assert.Equal(1,
                        this.receivedMessages.Where(
                            msg => msg.actions.Any(a => a.actionMode == edu.vanderbilt.isis.meta.Action.ActionMode.CREATE_CYPHY_DESIGN)).Count());
                }
            );
        }

        [Fact]
        // Insert an existing AVM component into a design
        void TestInsertComponentIntoDesign()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");
                        edu.vanderbilt.isis.meta.Action action = new edu.vanderbilt.isis.meta.Action()
                        {
                            actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.ADD_COMPONENT_TO_CYPHY_DESIGN,
                            subjectID = testAssemblyId,
                            payload = new Payload()
                        };
                        action.payload.components.Add(new CADComponentType()
                        {
                            Name = "TestInsertComp",
                            AvmComponentID = testAVMId
                        });
                        msg.actions.Add(action);
                        
                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var assembly = GetTestAssembly(project);
                        var insertcomp = assembly.ChildObjects.Cast<MgaFCO>().Where(fco => fco.Name.Contains("TestInsertComp")).First();
                        Xunit.Assert.NotEqual(null, insertcomp);
                        CyPhyML.ComponentRef insertcompref = CyPhyMLClasses.ComponentRef.Cast(insertcomp);
                        Xunit.Assert.NotEqual(null, insertcompref.AllReferred);
                        Xunit.Assert.Equal(true, insertcompref.AllReferred is CyPhyML.Component);
                        Xunit.Assert.Equal(testAVMId, (insertcompref.AllReferred as CyPhyML.Component).Attributes.AVMID);
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        [Fact]
        // Select component - only test that it won't fail
        void TestSelectComponent()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");
                        edu.vanderbilt.isis.meta.Action action = new edu.vanderbilt.isis.meta.Action()
                        {
                            actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.SELECT_CYPHY_COMPONENT,
                            payload = new Payload()
                        };
                        action.payload.components.Add(new CADComponentType()
                        {
                            ComponentID = massInstanceGuid
                        });
                        msg.actions.Add(action);

                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        // Nothing to check here
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        [Fact]
        // Remove component from design
        void TestRemoveComponent()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");
                        edu.vanderbilt.isis.meta.Action action = new edu.vanderbilt.isis.meta.Action()
                        {
                            actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.REMOVE_CYPHY_DESIGN_COMPONENT,
                            subjectID = testAssemblyId,
                            payload = new Payload()
                        };
                        action.payload.components.Add(new CADComponentType()
                        {
                            ComponentID = massInstanceGuid
                        });
                        msg.actions.Add(action);

                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var assembly = GetTestAssembly(project);
                        var masscomp = assembly.ChildObjects.Cast<MgaFCO>().Where(fco => fco.Name.Equals(massName));
                        Xunit.Assert.Equal(0, masscomp.Count());
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        [Fact]
        // Remove multiple components from design
        void TestRemoveComponents()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");
                        edu.vanderbilt.isis.meta.Action action = new edu.vanderbilt.isis.meta.Action()
                        {
                            actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.REMOVE_CYPHY_DESIGN_COMPONENT,
                            subjectID = testAssemblyId,
                            payload = new Payload()
                        };
                        action.payload.components.Add(new CADComponentType()
                        {
                            ComponentID = massInstanceGuid
                        });
                        action.payload.components.Add(new CADComponentType()
                        {
                            ComponentID = springInstanceGuid
                        });
                        msg.actions.Add(action);

                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var assembly = GetTestAssembly(project);
                        
                        Xunit.Assert.Equal(1, assembly.ChildObjects.Cast<MgaFCO>().Where(fco => fco.MetaRole.Kind.Name=="ComponentRef").Count());
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        [Fact]
        // Receive a list of components
        void TestComponentList()
        {
            SetupTest();

            {
                Edit interest = new Edit();
                interest.mode.Add(Edit.EditMode.INTEREST);
                interest.origin.Add(origin);
                interest.topic.Add("0");
                testingClient.SendToMetaLinkBridge(interest);
            } 
            
            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit()
                        {
                            editMode = Edit.EditMode.POST,
                        };
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");
                        msg.actions.Add(new edu.vanderbilt.isis.meta.Action()
                        {
                            actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.REQUEST_COMPONENT_LIST
                        });

                        this.receivedMessages.Clear();
                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                        Thread.Sleep(1000); // XXX don't race with propagate's send message thread
                        WaitForAllMetaLinkMessages();
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        Xunit.Assert.Equal(1, this.receivedMessages.Where(msg => msg.actions.Count == 1 && msg.actions[0].manifest.Count() == 11).Count());
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        [Fact]
        // Create a connector in a component
        void TestInsertIntoComponent()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit();
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");
                        msg.actions.Add(new edu.vanderbilt.isis.meta.Action()
                        {
                            actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.ADD_CONNECTOR_TO_COMPONENT,
                            payload = new Payload()
                        });
                        msg.actions[0].payload.connectors.Add(new ConnectorType()
                        {
                            DisplayName = "TestConnector",
                        });
                        msg.actions[0].payload.connectors[0].Datums.Add(new ConnectorDatumType()
                        {
                            DisplayName = "COMMON_PLANE_1_BOTTOM",
                            ID = "COMMON_PLANE_1_BOTTOM"
                        });
                        msg.actions[0].payload.connectors[0].Datums.Add(new ConnectorDatumType()
                        {
                            DisplayName = "COMMON_PLANE_1_TOP",
                            ID = "COMMON_PLANE_1_TOP"
                        });
                        msg.actions[0].payload.connectors[0].Datums.Add(new ConnectorDatumType()
                        {
                            DisplayName = "COMMON_AXIS",
                            ID = "COMMON_AXIS"
                        });
                        msg.actions[0].subjectID = testAVMId;
                        

                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                        Thread.Sleep(1000); // XXX don't race with propagate's send message thread
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var imported = project.RootFolder.GetObjectByPathDisp("/@Imported_Components");
                        var damper = imported.ChildObjects.Cast<IMgaObject>().Where(o => o.Name == "Damper_2").First();
                        var testconn = damper.ChildObjects.Cast<MgaFCO>().Where(fco => fco.Name.Equals("TestConnector")).First();
                        CyPhyML.Connector conn = CyPhyMLClasses.Connector.Cast(testconn);
                        Xunit.Assert.Equal(conn.Children.CADDatumCollection.Count(), 3);
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        //[Fact]
        // Connect components within a design
        void TestConnectComponents()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    {
                        Edit msg = new Edit();
                        msg.mode.Add(Edit.EditMode.POST);
                        msg.origin.Add(origin);
                        msg.topic.Add("0");
                        msg.actions.Add(new edu.vanderbilt.isis.meta.Action()
                        {
                            payload = new Payload()
                        });
                        msg.actions[0].actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.CONNECT_COMPONENTS_CYPHY;
                        msg.actions[0].payload.components.Add(new CADComponentType()
                        {
                            ComponentID = testAssemblyId
                        });
                        msg.actions[0].payload.components.Add(new CADComponentType()
                        {
                            ComponentID = massInstanceGuid
                        });
                        msg.actions[0].payload.components.Add(new CADComponentType()
                        {
                            ComponentID = springInstanceGuid
                        });
                        //msg.actions[0].subjectID = testAVMId;


                        propagate.EditMessageReceived(msg);
                        Application.DoEvents();
                        Thread.Sleep(1000); // XXX don't race with propagate's send message thread
                    }
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var imported = project.RootFolder.GetObjectByPathDisp("/@Imported_Components");
                        var damper = imported.ChildObjects.Cast<IMgaObject>().Where(o => o.Name == "Damper_2").First();
                        var testconn = damper.ChildObjects.Cast<MgaFCO>().Where(fco => fco.Name.Equals("TestConnector")).First();
                        CyPhyML.Connector conn = CyPhyMLClasses.Connector.Cast(testconn);
                        Xunit.Assert.Equal(conn.Children.CADDatumCollection.Count(), 3);
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        [Fact]
        // Running CAD flattener on the assembly
        void TestCadFlattener()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    project.BeginTransactionInNewTerr();
                    CyPhyML.ComponentAssembly assembly = CyPhyMLClasses.ComponentAssembly.Cast(GetTestAssembly(project));

                    Type t = Type.GetTypeFromProgID("MGA.Interpreter.CyPhyElaborateCS");
                    IMgaComponentEx elaborator = Activator.CreateInstance(t) as IMgaComponentEx;
                    DateTime t1 = DateTime.Now;
                    elaborator.Initialize(project);
                    elaborator.ComponentParameter["automated_expand"] = "true";
                    elaborator.ComponentParameter["console_messages"] = "off";
                    elaborator.InvokeEx(project, GetTestAssembly(project), (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs")), 0);
                    
                    CyPhy2CAD_CSharp.Logger.Instance.Reset();
                    CyPhy2CAD_CSharp.CADFlatDataCreator datacreator = new CyPhy2CAD_CSharp.CADFlatDataCreator("", Path.GetDirectoryName(project.ProjectConnStr.Substring("MGA=".Length)), metalink: true);

                    datacreator.CreateFlatData(assembly);
                    CyPhy2CAD_CSharp.DataRep.CADContainer cadcontainer = datacreator.CreateCADDataContainer(assembly.Guid.ToString(), CyPhy2CAD_CSharp.UtilityHelpers.CleanString2(assembly.Name));

                    // IF this is an empty design, add a root component
                    if (cadcontainer.assemblies.Count == 0)
                    {
                        cadcontainer.AddRootComponent(assembly);
                    }

                    Xunit.Assert.Equal(CyPhy2CAD_CSharp.Logger.Instance.ErrorCnt, 0);

                    project.AbortTransaction();
                }
            );
        }

        [Fact]
        void TestAssemblySyncDiscardComponent()
        {
            SetupTest();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    MgaFCO testAssembly;
                    MgaFCO testAssemblyHierarchy_2;
                    MgaFCO componentInstance;
                    string componentInstanceGuid;
                    string testHierarchy_1RefId;
                    string testAssemblyRefId;
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var filter = project.CreateFilter();
                        filter.Kind = "ComponentAssembly";
                        testAssembly = project.AllFCOs(filter).Cast<MgaFCO>()
                            .Where(fco => fco.GetGuidDisp() == new Guid(testAssemblyId).ToString("B")).First();
                        testAssemblyHierarchy_2 = project.AllFCOs(filter).Cast<MgaFCO>()
                            .Where(fco => fco.GetGuidDisp() == new Guid(testAssemblyHierarchy_2Id).ToString("B")).First();
                        //hullAndHookHookGuid = (string)((MgaModel)project.RootFolder.ObjectByPath[hullAndHookHookPath]).GetAttributeByNameDisp("AVMID");
                        componentInstance = testAssembly.ChildObjects.Cast<MgaFCO>().Where(f => f.Name.Contains("Damper")).First();
                        componentInstanceGuid = componentInstance.GetStrAttrByNameDisp("InstanceGUID");
                        MgaReference testHierarchy_1Ref = testAssemblyHierarchy_2.ChildObjects.Cast<MgaFCO>().OfType<MgaReference>().First();
                        testHierarchy_1RefId = new Guid(testHierarchy_1Ref.GetGuidDisp()).ToString("B");
                        MgaReference testAssemblyRef = ((MgaModel)testHierarchy_1Ref.Referred).ChildObjects.Cast<MgaFCO>().OfType<MgaReference>().First();
                        testAssemblyRefId = new Guid(testAssemblyRef.GetGuidDisp()).ToString("B");
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                    interpreter.StartAssemblySync(project, testAssemblyHierarchy_2, 128);
                    Application.DoEvents();
                    var msg = new Edit();
                    msg.mode.Add(Edit.EditMode.POST);
                    msg.origin.Add(origin);
                    msg.topic.Add((CyPhyMetaLinkAddon.IdCounter-1).ToString());
                    var action = new edu.vanderbilt.isis.meta.Action();
                    action.actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.REMOVE_CYPHY_DESIGN_COMPONENT;
                    action.subjectID = testAssemblyHierarchy_2Id;
                    action.payload = new Payload();
                    action.payload.components.Add(new CADComponentType()
                    {
                        //ComponentID = hookInstanceGuid,
                        ComponentID = testHierarchy_1RefId
                         + testAssemblyRefId
                         + componentInstanceGuid
                    });
                    msg.actions.Add(action);

                    SendToMetaLinkBridgeAndWaitForAddonToReceive(msg);
                    WaitForAllMetaLinkMessages();
                    Application.DoEvents();

                    project.BeginTransactionInNewTerr();
                    try
                    {
                        Xunit.Assert.Equal((int)objectstatus_enum.OBJECT_DELETED, componentInstance.Status);
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        //[Fact]
        void TestAssemblySyncConnect()
        {
            // TODO: this needs to be rewritten
            SetupTest();

            SendInterest("0");
            WaitToReceiveMessage();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    MgaFCO CAD_debug;
                    MgaFCO CAD_debug_copy;
                    MgaFCO hierarchy_2;
                    MgaFCO hookInstance;
                    MgaFCO hookCopyInstance;
                    MgaReference CAD_debug_ref;
                    MgaReference hierarchy_1_ref;
                    MgaReference hierarchy_1_ref_copy;
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        var filter = project.CreateFilter();
                        filter.Kind = "ComponentAssembly";
                        CAD_debug = project.AllFCOs(filter).Cast<MgaFCO>()
                            .Where(fco => fco.GetGuidDisp() == new Guid(testAssemblyId).ToString("B")).First();
                        // FIXME this copy is no good, since CyPhyMetaLink can just create the connection in CAD_debug
                        CAD_debug_copy = CAD_debug.ParentFolder.CopyFCODisp(CAD_debug);
                        CAD_debug_copy.Name = CAD_debug.Name + "_copy";
                        hierarchy_2 = project.AllFCOs(filter).Cast<MgaFCO>()
                            .Where(fco => fco.GetGuidDisp() == new Guid(testAssemblyHierarchy_2Id).ToString("B")).First();
                        //hullAndHookHookGuid = (string)((MgaModel)project.RootFolder.ObjectByPath[hullAndHookHookPath]).GetAttributeByNameDisp("AVMID");
                        hookInstance = CAD_debug.ChildObjects.Cast<MgaFCO>().Where(f => f.Name.Contains("drawbar")).First();
                        hierarchy_1_ref = (MgaReference)hierarchy_2.ChildObjectByRelID[1];
                        CAD_debug_ref = (MgaReference)((MgaModel)hierarchy_1_ref.Referred).ChildObjectByRelID[1];
                        hookCopyInstance = CAD_debug_copy.ChildObjects.Cast<MgaFCO>().Where(f => f.Name.Contains("drawbar")).First();
                        hierarchy_1_ref_copy = (MgaReference)((IMgaModel)hierarchy_2).CopyFCODisp((MgaFCO)hierarchy_1_ref, hierarchy_1_ref.MetaRole);
                        hierarchy_1_ref_copy.Referred = CAD_debug_copy;
                    }
                    finally
                    {
                        project.CommitTransaction();
                    }
                    Edit connectMsg;
                    project.BeginTransactionInNewTerr();
                    try
                    {
                        string hookInstanceGuid;
                        string hookCopyInstanceGuid;
                        hookInstanceGuid = hookInstance.GetStrAttrByNameDisp("InstanceGUID");
                        hookCopyInstanceGuid = hookCopyInstance.GetStrAttrByNameDisp("InstanceGUID");

                        // CyPhyAddon should reassign this (during the last CommitTransaction)
                        Xunit.Assert.NotEqual(hierarchy_1_ref.GetStrAttrByNameDisp("InstanceGUID"),
                            hierarchy_1_ref_copy.GetStrAttrByNameDisp("InstanceGUID"));
                        Xunit.Assert.NotEqual(hookInstanceGuid, hookCopyInstanceGuid);

                        connectMsg = new Edit();
                        connectMsg.mode.Add(Edit.EditMode.POST);
                        connectMsg.origin.Add(origin);
                        connectMsg.topic.Add("0");
                        var action = new edu.vanderbilt.isis.meta.Action();
                        action.actionMode = edu.vanderbilt.isis.meta.Action.ActionMode.REMOVE_CYPHY_DESIGN_COMPONENT;
                        action.payload = new Payload();
                        action.payload.components.Add(new CADComponentType()
                        {
                            ComponentID = Guid.Parse(hierarchy_2.GetGuidDisp()).ToString()
                        });
                        action.payload.components.Add(new CADComponentType()
                        {
                            ComponentID = "2e1091d6-989e-4130-bfed-c046ef0f7011" + "_" +
                                hierarchy_1_ref.GetStrAttrByNameDisp("InstanceGUID")
                                 + CAD_debug_ref.GetStrAttrByNameDisp("InstanceGUID")
                                 + hookInstanceGuid
                        });
                        action.payload.components.Add(new CADComponentType()
                        {
                            ComponentID = "2e1091d6-989e-4130-bfed-c046ef0f7011" + "_" +
                                hierarchy_1_ref_copy.GetStrAttrByNameDisp("InstanceGUID")
                                 + hookCopyInstanceGuid
                        });
                        connectMsg.actions.Add(action);
                    }
                    finally
                    {
                        project.CommitTransaction();
                    }
                    interpreter.StartAssemblySync(project, hierarchy_2, 128);
                    Application.DoEvents();

                    SendToMetaLinkBridgeAndWaitForAddonToReceive(connectMsg);
                    WaitForAllMetaLinkMessages();
                    Application.DoEvents();
                    Thread.Sleep(1000); // XXX don't race with propagate's send message thread
                    WaitForAllMetaLinkMessages();

                    project.BeginTransactionInNewTerr();
                    try
                    {
                        Xunit.Assert.Equal((int)objectstatus_enum.OBJECT_DELETED, hookInstance.Status);
                    }
                    finally
                    {
                        project.AbortTransaction();
                    }
                }
            );
        }

        //[Fact]
        public void TestCopyComponent()
        {
            SetupTest();

            SendInterest("0");
            WaitToReceiveMessage();

            RunCyPhyMLSync(
                (project, propagate, interpreter) =>
                {
                    CyPhyML.Component damper2 = null;
                    MgaFCO firstChild = null;
                    interpreter.MgaGateway.PerformInTransaction(delegate
                    {
                        damper2 = CyPhyMLClasses.Component.Cast(((MgaFCO)project.RootFolder.ObjectByPath[testComponentPath]));
                        firstChild = (MgaFCO)damper2.Children.CADModelCollection.First().Impl;
                    });

                    Func<IEnumerable<string>> componentNames = () => damper2.ParentContainer.AllChildren.Select(f => f.Name);

                    interpreter.MgaGateway.PerformInTransaction(delegate
                    {
                        Assert.False(componentNames().Contains(damper2.Name + "_new"));
                        Assert.False(componentNames().Contains(damper2.Name + "_new_2"));
                    }, transactiontype_enum.TRANSACTION_GENERAL, false);
                    propagate.StartEditingComponent(damper2, firstChild, true);

                    interpreter.MgaGateway.PerformInTransaction(delegate
                    {
                        Assert.True(componentNames().Contains(damper2.Name + "_new"));
                        Assert.False(componentNames().Contains(damper2.Name + "_new_2"));
                    }, transactiontype_enum.TRANSACTION_GENERAL, false);

                    propagate.StartEditingComponent(damper2, firstChild, true);

                    interpreter.MgaGateway.PerformInTransaction(delegate
                    {
                        var x = damper2.ParentContainer.AllChildren.Select(f => f.Name).ToArray();
                        Assert.True(componentNames().Contains(damper2.Name + "_new"));
                        Assert.True(componentNames().Contains(damper2.Name + "_new_2")); // META-2526
                    }, transactiontype_enum.TRANSACTION_GENERAL, false);

                });
        }

    }
}

// cd C:\Users\kevin\meta11\META_MetaLink_HullandHook
// change addcomp.txt topic guids
// java -jar \Users\kevin\Documents\META_13.15\src\MetaLink\meta-bridge\java-client\target\metalink-java-client-1.0.0.jar  -p addcomp.txt
// java -jar \Users\kevin\Documents\META_13.15\src\MetaLink\meta-bridge\java-server\target\metalink-java-server-1.0.0.jar   -r EditComponent.mlp
