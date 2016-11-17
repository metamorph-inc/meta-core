using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using GME.CSharp;
using GME.MGA;
using GME;
using GME.MGA.Core;
using System.Windows.Forms;
using System.Linq;
using GME.MGA.Meta;

namespace CyPhyMdaoAddOn
{
    [Guid(ComponentConfig.guid),
    ProgId(ComponentConfig.progID),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class CyPhyMdaoAddOnAddon : IMgaComponentEx, IGMEVersionInfo, IMgaEventSink
    {
        private const string BADGE_COMMON_STYLE = "block:inline;display: inline;padding-left: 6px;margin-left: 6px;padding-right: 6px; margin-right: 6px; width: 60px; text-align: center;";
        private const string ERROR_BADGE = "<div style=\"" + BADGE_COMMON_STYLE +
            " background-color: rgb(255, 145, 145);color: rgb(255, 0, 0);\">Error</div>";

        private const string WARNING_BADGE = "<div style=\"" + BADGE_COMMON_STYLE +
            " background-color: rgb(255, 255, 79);color: rgb(255, 112, 0);\">Warning</div>";

        private const string INFO_BADGE = "<div style=\"" + BADGE_COMMON_STYLE +
            " background-color: rgb(205, 227, 255);color: rgb(0, 51, 255);\">Info</div>";

        private MgaAddOn addon;
        private bool componentEnabled = true;
        private GMEConsole GMEConsole { get; set; }
        // Event handlers for addons
        #region MgaEventSink members
        public void GlobalEvent(globalevent_enum @event)
        {
            if (@event == globalevent_enum.GLOBALEVENT_CLOSE_PROJECT)
            {
                Marshal.FinalReleaseComObject(addon);
                addon = null;
            }
            if (!componentEnabled)
            {
                return;
            }

            //TODO: Handle global events
            //MessageBox.Show(@event.ToString());
        }

        /// <summary>
        /// Called when an FCO or folder changes
        /// </summary>
        /// <param name="subject">the object the event(s) happened to</param>
        /// <param name="eventMask">objectevent_enum values ORed together</param>
        /// <param name="param">extra information provided for cetertain event types</param>
        public void ObjectEvent(MgaObject subject, uint eventMask, object param)
        {
            if (!componentEnabled)
            {
                return;
            }

            if (subject.HasReadOnlyAccess())
            {
                return;
            }

            if (GMEConsole == null)
            {
                GMEConsole = GMEConsole.CreateFromProject(subject.Project);
            }

            // TODO: Handle object events (OR eventMask with the members of objectevent_enum)
            // Warning: Only those events are received that you have subscribed for by setting ComponentConfig.eventMask
            uint uOBJEVENT_ATTR = 0;
            uint uOBJEVENT_CREATED = 0;
            uint uOBJEVENT_NEWCHILD = 0;
            uint uOBJEVENT_LOSTCHILD = 0;
            uint uOBJEVENT_OPENMODEL = 0;

            unchecked { uOBJEVENT_ATTR = (uint)objectevent_enum.OBJEVENT_ATTR; }
            unchecked { uOBJEVENT_CREATED = (uint)objectevent_enum.OBJEVENT_CREATED; }
            unchecked { uOBJEVENT_NEWCHILD = (uint)objectevent_enum.OBJEVENT_NEWCHILD; }
            unchecked { uOBJEVENT_LOSTCHILD = (uint)objectevent_enum.OBJEVENT_LOSTCHILD; }
            unchecked { uOBJEVENT_OPENMODEL = (uint)objectevent_enum.OBJEVENT_OPENMODEL; }

            if ((eventMask & uOBJEVENT_CREATED) != 0)
            {
                if (subject.IsLibObject || subject.IsWritable == false)
                    return;
                if (subject.MetaBase.Name == "TestBenchRef")
                {
                    // set the port label lenght 0
                    // FIXME: why not just change it in the meta?
                    (subject as MgaFCO).RegistryValue["portLabelLength"] = "0";
                    if (subject as MgaReference != null)
                    {
                        UpdateColor((subject as MgaReference).Referred);
                    }
                }
                else if (subject.MetaBase.Name == "Optimizer")
                {
                    (subject as MgaFCO).RegistryValue["portLabelLength"] = "0";
                    UpdateColor(subject as MgaFCO);
                }
                else if (subject.MetaBase.Name == "VariableSweep")
                {
                    (subject as MgaFCO).RegistryValue["dstStyle"] = "arrow";
                    (subject as MgaFCO).RegistryValue["color"] = "0xff80c0";
                }
                else if (subject.MetaBase.Name == "ObjectiveMapping")
                {
                    (subject as MgaFCO).RegistryValue["dstStyle"] = "arrow";
                    (subject as MgaFCO).RegistryValue["color"] = "0x7cadde";
                }
            }
            else if ((eventMask & uOBJEVENT_ATTR) != 0)
            {
                this.DisplayPCCIterations(subject, true);

                if (subject.MetaBase.Name == "ValueFlow")
                {
                    // set the src custom formula's name to the specified name
                    // in the ValueFlow attribute field
                    MgaConnection ValueFlow = subject as MgaConnection;
                    foreach (MgaConnPoint cp in ValueFlow.ConnPoints)
                    {
                        if (cp.ConnRole == "src")
                        {
                            if (cp.Target.Meta.Name == "CustomFormula")
                            {
                                foreach (MgaAttribute attr in ValueFlow.Attributes)
                                {
                                    if (attr.Meta.Name == "FormulaVariableName")
                                    {
                                        if (string.IsNullOrEmpty(attr.StringValue) == false)
                                        {
                                            cp.Target.Name = attr.StringValue;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
                else if (subject.MetaBase.Name == "TestBench")
                {
                    // Excel import helper
                    MgaModel testBench = subject as MgaModel;

                    // call color helper
                    UpdateColor(testBench as MgaFCO);
                }
            }
            else if ((eventMask & uOBJEVENT_NEWCHILD) != 0 || (eventMask & uOBJEVENT_LOSTCHILD) != 0)
            {
                this.DisplayPCCIterations(subject, false);
            }
            else if ((eventMask & uOBJEVENT_OPENMODEL) != 0)
            {
            }
            //MessageBox.Show(eventMask.ToString());

        }

        private void DisplayPCCIterations(MgaObject subject, bool attributeChanged)
        {
            List<string> kinds = new List<string>()
            {
                "PCCDriver",
                "PCCParameterBeta",
                "PCCParameterUniform",
                "PCCParameterLNormal",
                "PCCParameterNormal"
            };

            if (kinds.Contains(subject.MetaBase.Name) == false)
            {
                return;
            }


            MgaModel PCCDriver = null;
            if (attributeChanged && subject.MetaBase.Name == "PCCDriver")
            {
                // TODO: Compute and
                PCCDriver = subject as MgaModel;
            }
            else if (attributeChanged == false && subject.MetaBase.Name == "PCCDriver")
            {
                // Compute and prin

                //GME.MGA.Meta.objtype_enum type;
                //MgaObject parent;
                //(subject as MgaFCO).GetParent(out parent, out type);
                PCCDriver = subject as MgaModel;
            }
            else
            {
                return;
            }

            //Compute and print PCCDriver
            var nbrOfParameters = PCCDriver.ChildFCOs.Cast<MgaFCO>().Where(x => kinds.Contains(x.Meta.Name)).Count();
            var saMethod = PCCDriver.StrAttrByName["PCC_SA_Methods"];
            var upMethod = PCCDriver.StrAttrByName["PCC_UP_Methods"];
            GMEConsole.Out.WriteLine("{2}Iterations : UP : {0}, SA : {1}",
                this.GetNumberOfIterations(upMethod, nbrOfParameters),
                this.GetNumberOfIterations(saMethod, nbrOfParameters),
                INFO_BADGE);
        }

        private void UpdateColor(MgaFCO subject)
        {
            // default values
            string shadowThickness = "12";
            string itemShadowCast = "true";
            string shadowColor = "0x666666";
            string color = "0xc0c0c0";
            string borderColor = "0x000000";

            if (subject != null)
            {
                if (subject.Meta.Name == "TestBench")
                {
                    string Type = subject.StrAttrByName["Type"];

                    if (Type == "Excel")
                    {
                        color = "0x98fb98";
                        borderColor = "0x005500";
                    }
                    else if (Type == "DynSim")
                    {
                        color = "0x7d9ec0";
                        borderColor = "0x00008b";
                    }
                    else if (Type == "SimpleCalc")
                    {
                        color = "0xffad5b";
                        borderColor = "0xa52a00";
                    }
                }
                else if (subject.Meta.Name == "Optimizer")
                {
                    color = "0xff6820";
                    borderColor = "0x8b0000";
                }

                subject.RegistryValue["color"] = color;
                subject.RegistryValue["borderColor"] = borderColor;
                subject.RegistryValue["shadowThickness"] = shadowThickness;
                subject.RegistryValue["itemShadowCast"] = itemShadowCast;
                subject.RegistryValue["shadowColor"] = shadowColor;
            }
        }

        private int GetNumberOfIterations(string method, int nbrOfInputs, int fixedIterations = 1280)
        {
            if (nbrOfInputs == 0)
            {
                return 0;
            }
            int nodeDefault = 5;
            int MI = 4;
            switch (method)
            {
                case "UP_MCS":                      // 1
                    return fixedIterations;
                case "UP_TS":                       // 2
                    return 1 + 2 * nbrOfInputs;
                case "UP_MPP":                      // 3
                    return -1;
                case "UP_FFNI":                     // 4
                    return (int)Math.Pow(5, nbrOfInputs) * nbrOfInputs;
                case "UP_UDR":                      // 5
                    return nodeDefault * nbrOfInputs + 1;
                case "UP_PCE":                      // 6
                    return (int)Math.Pow(5, nbrOfInputs);
                case "SA_SOBOL":                    // 7
                    return 2 * fixedIterations + nbrOfInputs * fixedIterations;
                case "SA_FAST":                     // 9
                    int OMAX = 0;
                    switch (nbrOfInputs)
                    {
                        case 1:
                            return -1;
                        case 2:
                            OMAX = 9;
                            break;
                        case 3:
                            OMAX = 15;
                            break;
                        default:
                            OMAX = this.OMEGA[nbrOfInputs - 1];
                            for (int i = 1; i < nbrOfInputs; i++)
                            {
                                OMAX = OMAX + this.DN[nbrOfInputs - 1 - i];
                            }
                            break;
                    }
                    return 2 * MI * OMAX + 1;
                case "SA_EFAST":                    // 10:
                    int OMi = (int)Math.Floor((double)((fixedIterations - 1) / (2 * MI) / nbrOfInputs));
                    return nbrOfInputs * (2 * MI * OMi + 1);
            }

            return 0;
        }

        public readonly int[] OMEGA = {0, 3, 1, 5, 11, 1, 17, 23, 19, 25, 41, 31,
            23, 87, 67, 73, 85, 143, 149, 99, 119, 237, 267, 283, 151, 385, 157,
            215, 449, 163, 337, 253, 375, 441, 673, 773, 875, 873, 587, 849, 623,
            637, 891, 943, 1171, 1225, 1335, 1725, 1663, 2019};

        public readonly int[] DN = { 4, 8, 6, 10, 20, 22, 32, 40, 38, 26, 56, 62,
            46, 76, 96, 60, 86, 126, 134, 112, 92, 128, 154, 196, 34, 416, 106, 208,
            328, 198, 382, 88, 348, 186, 140, 170, 284, 568, 302, 438, 410, 248, 448,
            388, 596, 216, 100, 488, 166, 0 };

        #endregion

        #region IMgaComponentEx Members

        public void Initialize(MgaProject p)
        {
            // Creating addon
            p.CreateAddOn(this, out addon);
            // Setting event mask (see ComponentConfig.eventMask)
            unchecked
            {
                addon.EventMask = (uint)ComponentConfig.eventMask;
            }
        }

        public void InvokeEx(MgaProject project, MgaFCO currentobj, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException(); // Not called by addon
        }


        #region Component Information
        public string ComponentName
        {
            get { return GetType().Name; }
        }

        public string ComponentProgID
        {
            get
            {
                return ComponentConfig.progID;
            }
        }

        public componenttype_enum ComponentType
        {
            get { return ComponentConfig.componentType; }
        }
        public string Paradigm
        {
            get { return ComponentConfig.paradigmName; }
        }
        #endregion

        #region Enabling
        bool enabled = true;
        public void Enable(bool newval)
        {
            enabled = newval;
        }
        #endregion

        #region Interactive Mode
        protected bool interactiveMode = true;
        public bool InteractiveMode
        {
            get
            {
                return interactiveMode;
            }
            set
            {
                interactiveMode = value;
            }
        }
        #endregion

        #region Custom Parameters
        SortedDictionary<string, object> componentParameters = null;

        public object get_ComponentParameter(string Name)
        {
            if (Name == "type")
                return "csharp";

            if (Name == "path")
                return GetType().Assembly.Location;

            if (Name == "fullname")
                return GetType().FullName;

            object value;
            if (componentParameters != null && componentParameters.TryGetValue(Name, out value))
            {
                return value;
            }

            return null;
        }

        public void set_ComponentParameter(string Name, object pVal)
        {
            if (componentParameters == null)
            {
                componentParameters = new SortedDictionary<string, object>();
            }

            componentParameters[Name] = pVal;
        }
        #endregion

        #region Unused Methods
        // Old interface, it is never called for MgaComponentEx interfaces
        public void Invoke(MgaProject Project, MgaFCOs selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        // Not used by GME
        public void ObjectsInvokeEx(MgaProject Project, MgaObject currentobj, MgaObjects selectedobjs, int param)
        {
            throw new NotImplementedException();
        }

        #endregion

        #endregion

        #region IMgaVersionInfo Members

        public GMEInterfaceVersion_enum version
        {
            get { return GMEInterfaceVersion_enum.GMEInterfaceVersion_Current; }
        }

        #endregion

        #region Registration Helpers

        [ComRegisterFunctionAttribute]
        public static void GMERegister(Type t)
        {
            Registrar.RegisterComponentsInGMERegistry();
        }

        [ComUnregisterFunctionAttribute]
        public static void GMEUnRegister(Type t)
        {
            Registrar.UnregisterComponentsInGMERegistry();
        }

        #endregion
    }
}
