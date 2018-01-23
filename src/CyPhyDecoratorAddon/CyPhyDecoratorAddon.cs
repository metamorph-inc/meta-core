using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using GME.CSharp;
using GME;
using System.Windows.Forms;
using GME.MGA;
using GME.MGA.Core;
using System.Linq;
using GME.MGA.Meta;
using META;

namespace CyPhyDecoratorAddon
{
	[Guid(ComponentConfig.guid),
	ProgId(ComponentConfig.progID),
	ClassInterface(ClassInterfaceType.AutoDual)]
	[ComVisible(true)]
	public class CyPhyDecoratorAddon :
		IMgaComponentEx,
		IGMEVersionInfo,
		IMgaEventSink
	{

		private MgaAddOn addon;
		private bool componentEnabled = true;
		
		bool isXMLImportInProgress { get; set; }
		bool isProjectInTransation { get; set; }

		// Event handlers for addons
		#region MgaEventSink members
		public void GlobalEvent(globalevent_enum @event)
		{
			if (@event == globalevent_enum.GLOBALEVENT_CLOSE_PROJECT)
			{
				Marshal.FinalReleaseComObject(addon);
				addon = null;
			}
			else if (@event == globalevent_enum.APPEVENT_XML_IMPORT_BEGIN)
			{
				isXMLImportInProgress = true;
			}
			else if (@event == globalevent_enum.APPEVENT_XML_IMPORT_END)
			{
				isXMLImportInProgress = false;
			}

			//else if (@event == globalevent_enum.GLOBALEVENT_COMMIT_TRANSACTION)
			//{
			//  isProjectInTransation = true;
			//}
			//else if (@event == globalevent_enum.GLOBALEVENT_DESTROY_TERRITORY)
			//{
			//  isProjectInTransation = false;
			//}
			if (!componentEnabled)
			{
				return;
			}

			// TODO: Handle global events
			// MessageBox.Show(@event.ToString());
		}

		/// <summary>
		/// Called when an FCO or folder changes
		/// </summary>
		/// <param name="subject">
		///   the object the event(s) happened to
		/// </param>
		/// <param name="eventMask">
		///   objectevent_enum values ORed together
		/// </param>
		/// <param name="param">
		///   extra information provided for cetertain event types
		/// </param>
		public void ObjectEvent(
			MgaObject subject,
			uint eventMask,
			object param)
		{
			if (!componentEnabled)
			{
				return;
			}
			else if (isXMLImportInProgress)
			{
				return;
			}
			//else if (isProjectInTransation)
			//{
			//  return;
			//}

            if (subject.HasReadOnlyAccess() ||
                subject.IsLibObject)
            {
                return;
            }

			uint uOBJEVENT_CREATED = 0;
			uint uOBJEVENT_COPIED = 0;

			unchecked { uOBJEVENT_CREATED = (uint)objectevent_enum.OBJEVENT_CREATED; }
			unchecked { uOBJEVENT_COPIED = (uint)objectevent_enum.OBJEVENT_COPIED; }



			if ((eventMask & uOBJEVENT_COPIED) != 0)
			{
				isCopied = true;
			}
			else if ((eventMask & uOBJEVENT_CREATED) != 0 && subject.Status == 0)
                // check Status, since object can be created and deleted in same tx
			{
				if (isCopied)
				{
					// handle copy event
					isCopied = false;
				}
				else
				{
                    //subject.Project.RootMeta.RootFolder.DefinedFCOByName["Task", 
                    //MgaMetaBase task;
                    //if (task.MetaRef == subject.MetaBase.MetaRef)
                    {

                    }
					// handle new object event
                    bool isBasicTask = (subject.MetaBase.Name == "Task");

					if (subject.MetaBase.Name == "Task" || subject.MetaBase.Name == "ExecutionTask")
					{
                        if (subject.MetaBase.Name == "Task" && string.IsNullOrEmpty((subject as MgaFCO).StrAttrByName["COMName"]) == false)
                        {
                            return;
                        }
                        if (subject.MetaBase.Name == "ExecutionTask" && string.IsNullOrEmpty((subject as MgaFCO).StrAttrByName["Invocation"]) == false)
                        {
                            return;
                        }
                        using (InterpreterSelectionForm form = new InterpreterSelectionForm())
                        {
                            form.addon = this;
                            form.Init();

                            IEnumerable<MgaAtom> taskChildren = subject.ExGetParent().
                                                                        ChildObjects.
                                                                        OfType<MgaAtom>().
                                                                        Where(x => x.ExDstFcos().Count() == 0).
                                                                        Where(x => x.ID != subject.ID);

                            form.lbTasks.Items.Clear();
                            foreach (var currTask in taskChildren)
                            {
                                var atomWrapper = new MgaAtomWrapper(currTask);
                                form.lbTasks.Items.Add(new MgaAtomWrapper(currTask));
                                form.lbTasks.SelectedItem = atomWrapper;
                            }

                            if (form.lbTasks.Items.Count > 0)
                            {
                                form.lbTasks.SetSelected(0, true);
                            }

                            if (!isBasicTask) // remove interpreter selection and reset positions
                            {
                                form.lbInterpreters.Items.Clear();
                                form.lbInterpreters.Visible = false;
                                form.lblSelectInterpreter.Visible = false;
                                form.chbAutoConnect.Location = form.label1.Location;
                                form.label1.Location = form.lblSelectInterpreter.Location;
                                form.lbTasks.Location = form.lbInterpreters.Location;
                            }

                            DialogResult dgr = form.ShowDialog();
                            if (dgr == DialogResult.OK)
                            {
                                if (isBasicTask)
                                {
                                    ComComponent c = form.lbInterpreters.SelectedItem as ComComponent;
                                    try
                                    {
                                        if (c != null &&
                                            c.isValid)
                                        {
                                            (subject as MgaFCO).StrAttrByName["COMName"] = c.ProgId;
                                        }
                                    }
                                    catch
                                    {
                                        MessageBox.Show("Cannot save interpreter settings. 'COMName' is not a parameter of 'Task'.");
                                    }
                                }

                                //Flow
                                if (form.chbAutoConnect.Checked && (form.lbTasks.SelectedItem != null))
                                {
                                    MgaAtomWrapper selectedTask = form.lbTasks.SelectedItem as MgaAtomWrapper;
                                    MgaAtom lastInWorkflow = null;

                                    if (selectedTask != null)
                                    {
                                        lastInWorkflow = selectedTask.Atom;
                                    }

                                    if (lastInWorkflow != null)
                                    {
                                        MgaMetaRole role = ((subject.ExGetParent() as MgaModel).
                                            Meta as MgaMetaModel).RoleByName["Flow"];

                                        (subject.ExGetParent() as MgaModel).CreateSimplerConnDisp(
                                            role,
                                            lastInWorkflow as MgaFCO,
                                            subject as MgaFCO);
                                    }
                                }
                            }
                        }
					}
				}
			}

			// TODO: Handle object events (OR eventMask with the members of objectevent_enum)
			// Warning: Only those events are received that you have subscribed for by setting ComponentConfig.eventMask

			// MessageBox.Show(eventMask.ToString());

		}

		bool isCopied { get; set; }

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

		public void InvokeEx(
			MgaProject project,
			MgaFCO currentobj,
			MgaFCOs selectedobjs,
			int param)
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

    internal class MgaAtomWrapper
    {
        protected MgaAtom atom;

        public MgaAtom Atom
        {
            get
            {
                return atom;
            }
        }

        public MgaAtomWrapper(MgaAtom atom)
        {
            this.atom = atom;
        }

        public override string ToString()
        {
            return atom.Name;
        }
    }
}
