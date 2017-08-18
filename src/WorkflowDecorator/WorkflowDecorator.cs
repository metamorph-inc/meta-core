using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System.Drawing;

using GME;
using GME.MGA;
using GME.MGA.Meta;
using GME.Util;
using System.Reflection;
using System.Drawing.Drawing2D;
using WorkflowDecorator;
using System.Windows.Forms;
using Newtonsoft.Json;
using META;
using System.Diagnostics;

namespace GME.CSharp
{
    [Guid("2108C2FF-AD6B-42DC-BDCC-C27957299303"),
    ProgId("MGA.Decorator.Workflow"),
    ClassInterface(ClassInterfaceType.AutoDual)]
    [ComVisible(true)]
    public class WorkflowDecorator : IMgaDecoratorCommon, IMgaElementDecorator
    {
        public WorkflowDecorator()
        {

        }
        public WorkflowDecorator(IMgaElementDecoratorEvents events)
        {

        }

        ~WorkflowDecorator()
        {
            interpreters = null;
            workflow = null;
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
        }

        // store the coordinates where it must be drawn
        int x, y;
        int w, h;

        // name
        string name = null;
        SizeF LabelSize;

        // color of place and its label
        Color color = Color.Black;
        Color labelColor = Color.Black;

        public void Destroy()
        {
        }

        public void DragEnter(out uint dropEffect, ulong pCOleDataObject, uint keyState, int pointx, int pointy, ulong transformHDC)
        {
            dropEffect = 0;
        }

        public void DragOver(out uint dropEffect, ulong pCOleDataObject, uint keyState, int pointx, int pointy, ulong transformHDC)
        {
            dropEffect = 0;
        }

        public void Draw(uint hdc)
        {
            // VERY important to cast to int
            System.IntPtr hdcptr;
            unchecked { hdcptr = (System.IntPtr)(int)hdc; }

            // Create graphics object
            Graphics g = Graphics.FromHdc(hdcptr);

            // Set up string format for the label
            StringFormat sf = new StringFormat(StringFormatFlags.NoClip);
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;

            // Drawing style
            // n.b. GME decorators are based on pixels and shouldn't be scaled by DPI changes
            Font font = new Font(SystemFonts.DefaultFont.FontFamily, 12f, GraphicsUnit.Pixel);
            Pen pen = new Pen(color);
            Brush brush = new SolidBrush(color);
#if DEBUG
            // Draw the place
            g.DrawRectangle(pen, x, y, w, h);
#endif
            Color contentColor = Color.Black;
            string Content = "";
            //if (myobj == null)
            //{
            //  g.Dispose();
            //  return;
            //}
            if (LastMetaKind == "Task")
            {
                Icon icon = null;
                if (interpreters[TaskProgId].isValid)
                {
                    contentColor = Color.Blue;
                    Content = interpreters[TaskProgId].ToolTip;
                    if (string.IsNullOrWhiteSpace(Content))
                    {
                        Content = interpreters[TaskProgId].Name;
                    }
                    if (Content.StartsWith("MGA.Interpreter.", StringComparison.InvariantCultureIgnoreCase))
                    {
                        Content = Content.Substring("MGA.Interpreter.".Length);
                    }
                    MgaRegistrar registrar = new MgaRegistrar();
                    string DllFileName = registrar.LocalDllPath[TaskProgId];
                    try
                    {
                        // if the value is ,IDI_COMPICON get the icon from the dll
                        string iconFileNameGuess = Path.ChangeExtension(DllFileName, ".ico");
                        string iconPath = null;
                        try
                        {
                            iconPath = registrar.ComponentExtraInfo[regaccessmode_enum.REGACCESS_BOTH, TaskProgId, "Icon"];
                        }
                        catch (COMException)
                        {
                        }

                        if (File.Exists(iconFileNameGuess))
                        {
                            icon = Icon.ExtractAssociatedIcon(iconFileNameGuess);
                        }
                        else if (iconPath != null && File.Exists(iconPath))
                        {
                            icon = Icon.ExtractAssociatedIcon(iconPath);
                        }
                        else
                        {
                            icon = Icon.ExtractAssociatedIcon(DllFileName);
                        }
                    }
                    catch (ArgumentException)
                    {
                    }
                }
                else
                {
                    contentColor = Color.Red;
                    Content = "INVALID" + Environment.NewLine + TaskProgId;
                    icon = InvalidTask;
                }
                using (Icon icon2 = new Icon(icon, IconWidth, IconHeight))
                    g.DrawIcon(icon2, new Rectangle(x, y, IconWidth, IconHeight));

                g.DrawString(
                    Content,
                    font,
                    new SolidBrush(contentColor),
                    new RectangleF(
                        x,
                        y + h + 15,
                        g.MeasureString(Content, SystemFonts.DefaultFont).Width,
                        g.MeasureString(Content, SystemFonts.DefaultFont).Height),
                    sf);
            }
            else if (LastMetaKind == "WorkflowRef")
            {
                int i = 0;
                Icon icon = null;

                foreach (TaskInfo taskInfo in workflow)
                {
                    if (taskInfo.IsComComponent)
                    {
                        var comComponent = new ComComponent(taskInfo.COMName);
                        // FIXME cache this per-process for performance
                        if (comComponent.isValid)
                        {
                            MgaRegistrar registrar = new MgaRegistrar();
                            string DllFileName = registrar.LocalDllPath[comComponent.ProgId];
                            try
                            {
                                // if the value is ,IDI_COMPICON get the icon from the dll
                                string iconFileNameGuess = Path.ChangeExtension(DllFileName, ".ico");
                                string iconPath = null;
                                try
                                {
                                    iconPath = registrar.ComponentExtraInfo[regaccessmode_enum.REGACCESS_BOTH, taskInfo.COMName, "Icon"];
                                }
                                catch (COMException)
                                {
                                }

                                if (File.Exists(iconFileNameGuess))
                                {
                                    icon = Icon.ExtractAssociatedIcon(iconFileNameGuess);
                                }
                                else if (iconPath != null && File.Exists(iconPath))
                                {
                                    icon = Icon.ExtractAssociatedIcon(iconPath);
                                }
                                else
                                {
                                    icon = Icon.ExtractAssociatedIcon(DllFileName);
                                }
                            }
                            catch (ArgumentException)
                            {
                            }
                        }
                        else
                        {
                            // draw error image
                            icon = InvalidTask;
                        }
                    }
                    else
                    {
                        string iconFileName = taskInfo.IconName;
                        if (!string.IsNullOrWhiteSpace(iconFileName) &&
                            File.Exists(iconFileName))
                        {
                            Bitmap bitmap = (Bitmap)Image.FromFile(iconFileName);
                            icon = Icon.FromHandle(bitmap.GetHicon());
                        }
                        else
                        {
                            icon = InvalidTask;
                        }
                    }

                    try
                    {
                        int IconStartX = x + (IconWidth + TaskPadding) * i;
                        g.DrawIcon(new Icon(icon, IconWidth, IconHeight),
                            new Rectangle(IconStartX, y, IconWidth, IconHeight));

                        if (taskInfo != workflow.Reverse().FirstOrDefault())
                        {
                            Pen p = new Pen(Color.Black, LineWidth * g.DpiX / 96);
                            p.StartCap = LineStartCap;
                            p.EndCap = LineEndCap;
                            int LineStartX = IconStartX + IconWidth;
                            g.DrawLine(p,
                                new Point(
                                    LineStartX + LineStartPadding,
                                    y + IconHeight / 2),
                                new Point(
                                    LineStartX + TaskPadding - LineEndPadding,
                                    y + IconHeight / 2));
                        }
                        i++;
                    }
                    catch (ArgumentException)
                    {
                    }

                }
                if (workflow != null)
                {
                    if (workflow.Count == 0 ||
                        icon == null)
                    {
                        icon = UndefinedWorkFlow;
                        g.DrawIcon(
                            new Icon(icon, IconWidth, IconHeight),
                            new Rectangle(x, y, IconWidth, IconHeight));
                    }
                }
            }

            // Draw the label
            g.DrawString(name, font, new SolidBrush(labelColor),
                new RectangleF(x + w / 2 - LabelSize.Width / 2, y + h + 5, LabelSize.Width, 10), sf);

            font.Dispose();
            sf.Dispose();
            g.Dispose();
        }

        public void DrawEx(uint hdc, ulong gdip)
        {
            Draw(hdc);
        }

        public void SetLocation(int sx, int sy, int ex, int ey)
        {
            x = sx; y = sy; w = ex - sx; h = ey - sy;
        }

        public void Drop(ulong pCOleDataObject, uint dropEffect, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void DropFile(ulong hDropInfo, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void GetFeatures(out uint features)
        {
            features =
                Defines.F_MOUSEEVENTS;
            //Defines.F_HASLABEL |
            //Defines.F_RESIZABLE |
            //Defines.F_RESIZEAFTERMOD;
            //Defines.F_HASSTATE |
            //Defines.F_ANIMATION;
        }

        struct Defines
        {
            public const uint F_RESIZABLE = 1 << 0;
            public const uint F_MOUSEEVENTS = 1 << 1;
            public const uint F_HASLABEL = 1 << 2;
            public const uint F_HASSTATE = 1 << 3;
            public const uint F_HASPORTS = 1 << 4;
            public const uint F_ANIMATION = 1 << 5;
            public const uint F_IMGPATH = 1 << 6;
            public const uint F_RESIZEAFTERMOD = 1 << 7;
        };

        public void GetLabelLocation(out int sx, out int sy, out int ex, out int ey)
        {
            sx = x + w / 2 - (int)LabelSize.Width / 2;
            sy = y + h + 5;
            ex = sx + (int)LabelSize.Width;
            ey = y + w + 15;
        }

        public void GetLocation(
            out int sx,
            out int sy,
            out int ex,
            out int ey)
        {
            sx = x;
            sy = y;
            ex = x + w;
            ey = y + h;
        }

        public void GetMnemonic(out string mnemonic)
        {
            mnemonic = "MGA.Decorator.Workflow";
        }

        public void GetParam(string name, out object value)
        {
            value = null;
        }

        public void GetPortLocation(MgaFCO fco, out int sx, out int sy, out int ex, out int ey)
        {
            sx = sy = ex = ey = 0;
        }

        public MgaFCOs GetPorts()
        {
            return null;
        }

        public void GetPreferredSize(out int sizex, out int sizey)
        {
            sizex = w;
            sizey = h;
        }

        MgaFCO myobj = null;
        MgaMetaFCO mymetaobj = null;

        public void Initialize(
            MgaProject project,
            MgaMetaPart meta,
            MgaFCO obj)
        {
            // only store temporarily, they might be unavailable later
            myobj = obj;
            mymetaobj = null;
            LastMetaKind = myobj.Meta.Name;

            // obtain the metaobject
            GetMetaFCO(meta, out mymetaobj);

            if (obj != null)
            {
                // concrete object
                name = myobj.Name;
                if (myobj.Meta.Name == "Task")
                {
                    // task
                    // get progid check whether it is already in the cache
                    TaskProgId = myobj.StrAttrByName["COMName"];
                    if (interpreters.Keys.Contains(TaskProgId) == false)
                    {
                        // create an instance
                        ComComponent task = new ComComponent(TaskProgId);
                        interpreters.Add(TaskProgId, task);
                    }
                    // save parameters
                    Parameters = myobj.StrAttrByName["Parameters"];
                    h = IconHeight;
                    w = IconWidth;
                }
                else if (myobj.Meta.Name == "WorkflowRef")
                {
                    // Workflow reference get the tasks
                    // TODO: get those in the right order
                    workflow.Clear();
                    MgaReference wfRef = myobj as MgaReference;
                    Queue<string> items = new Queue<string>();
                    List<MgaAtom> tasks = new List<MgaAtom>();
                    List<MgaFCO> processed = new List<MgaFCO>();

                    if (wfRef.Referred != null)
                    {
                        tasks.AddRange(wfRef.Referred.ChildObjects.OfType<MgaAtom>());

                        MgaAtom StartTask = null;

                        StartTask = tasks.
                            Where(x => x.ExSrcFcos().Count() == 0).
                            FirstOrDefault();

                        if (StartTask != null)
                        {
                            this.EnqueueTask(StartTask as MgaFCO);
                            processed.Add(StartTask as MgaFCO);

                            MgaFCO NextTask = StartTask.ExDstFcos().FirstOrDefault();

                            // avoid loops
                            while (NextTask != null &&
                                processed.Contains(NextTask) == false)
                            {
                                processed.Add(NextTask as MgaFCO);
                                this.EnqueueTask(NextTask);
                                NextTask = NextTask.ExDstFcos().FirstOrDefault();
                            }
                        }
                    }
                    h = IconHeight;
                    if (workflow.Count > 0)
                    {
                        w = IconWidth * workflow.Count +
                            TaskPadding * (workflow.Count - 1);
                    }
                    else
                    {
                        w = IconWidth;
                    }
                }
            }
            else
            {
                // not a concreter object (maybe in part browser?)
                name = mymetaobj.DisplayedName;
            }

            // to handle color and labelColor settings in GME
            if (!GetColorPreference(out color, "color"))
            {
                color = Color.Black;
            }
            if (!GetColorPreference(out labelColor, "nameColor"))
            {
                labelColor = Color.Black;
            }

            // null them for sure
            // myobj = null;
            mymetaobj = null;
        }

        protected string GetDirectoryFromConnStr(string connectionString)
        {
            if (!string.IsNullOrWhiteSpace(connectionString) &&
                connectionString.Length > 3)
            {
                if (connectionString.Substring(0, 4) == "MGA=")
                {
                    connectionString = connectionString.Substring("MGA=".Length);
                    connectionString = new FileInfo(connectionString).Directory.FullName;
                }
            }

            return connectionString;
        }

        protected void SetTaskIconPath(TaskInfo task, string iconName)
        {
            foreach (var basePath in this.iconPaths)
            {
                var fullPath = Path.Combine(basePath, iconName);
                if (File.Exists(fullPath))
                {
                    task.SetIconPath(fullPath);
                    break;
                }
            }
        }

        List<string> iconPaths = new List<string>();

        protected void InitializeIconPaths(MgaFCO task)
        {
            string allIconPaths = new MgaRegistrar().get_IconPath(regaccessmode_enum.REGACCESS_BOTH);
            string projectDir = GetDirectoryFromConnStr(task.Project.ProjectConnStr);
            string paradigmDir = GetDirectoryFromConnStr(task.Project.ParadigmConnStr);

            if (!string.IsNullOrWhiteSpace(allIconPaths) &&
                !string.IsNullOrWhiteSpace(projectDir) &&
                !string.IsNullOrWhiteSpace(paradigmDir))
            {
                allIconPaths = allIconPaths.Replace("$PROJECTDIR", projectDir);
                allIconPaths = allIconPaths.Replace("$PARADIGMDIR", paradigmDir);
                this.iconPaths = allIconPaths.Split(';').ToList();
            }
        }

        public void EnqueueTask(MgaFCO task)
        {
            if (this.iconPaths.Count == 0)
            {
                InitializeIconPaths(task);
            }

            string metaName = task.MetaBase.Name;

            if (metaName == "ExecutionTask")
            {
                TaskInfo taskInfo = new TaskInfo(metaName);
                SetTaskIconPath(taskInfo, EXECUTIONTASK_ICON_NAME);

                workflow.Enqueue(taskInfo);
            }
            else
            {
                workflow.Enqueue(new TaskInfo(task.StrAttrByName["COMName"], metaName));
            }
        }

        public void InitializeEx(
            MgaProject project,
            MgaMetaPart meta,
            MgaFCO obj,
            IMgaCommonDecoratorEvents eventSink,
            ulong parentWnd)
        {
            try
            {
                Initialize(project, meta, obj);
                if (name != null)
                {
                    IntPtr parentHwnd;
                    unchecked { parentHwnd = (IntPtr)(int)parentWnd; }
                    using (Graphics g = Graphics.FromHwnd(parentHwnd))
                    {
                        LabelSize = g.MeasureString(name, SystemFonts.DefaultFont);
                    }
                }
            }
            catch (Exception ex)
            {
                // sometimes the mgafco is inaccessible

                Trace.TraceWarning(ex.ToString().Replace('\n','\t'));
            }
        }

        public void MenuItemSelected(uint menuItemId, uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        #region Mouse events

        static readonly int S_DECORATOR_EVENT_NOT_HANDLED = 0x00737002;
        public void MouseLeftButtonDoubleClick(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
            if (Parameters == null || TaskProgId == null)
            {
                throw new COMException("Not handled", S_DECORATOR_EVENT_NOT_HANDLED);
            }

            ComComponent c = new ComComponent(TaskProgId);
            Dictionary<string, string> ParametersDict = new Dictionary<string, string>();
            if (string.IsNullOrWhiteSpace(Parameters) == false)
            {
                try
                {
                    ParametersDict = (Dictionary<string, string>)JsonConvert.DeserializeObject(Parameters, typeof(Dictionary<string, string>));
                }
                catch (Newtonsoft.Json.JsonReaderException) { }
            }

            foreach (var parameter in c.WorkflowParameterList.Where(p => !ParametersDict.ContainsKey(p)))
            {
                ParametersDict[parameter] = c.GetWorkflowParameterValue(parameter);
            }

            List<ParameterSettingsForm.Parameter> parameters;
            parameters = ParametersDict.Select(x => new ParameterSettingsForm.Parameter()
            {
                Name = x.Key,
                Value = x.Value,
            }).ToList();

            myobj.Project.BeginTransactionInNewTerr();
            if (c.isValid == false || myobj.IsLibObject || myobj.HasReadOnlyAccess())
            {
                myobj.Project.AbortTransaction();
                return;
            }
            using (ParameterSettingsForm form = new ParameterSettingsForm(parameters, TaskProgId))
            {
                form.ShowDialog();
                Dictionary<String, String> d = form.parameters.ToDictionary(p => p.Name, p => p.Value);
                string serialized = JsonConvert.SerializeObject(d, Formatting.Indented);
                try
                {
                    Parameters = myobj.StrAttrByName["Parameters"] = serialized;
                }
                catch
                {
                    myobj.Project.AbortTransaction();
                    return;
                }
                myobj.Project.CommitTransaction();
            }
        }

        public void MouseLeftButtonDown(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseLeftButtonUp(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseMiddleButtonDoubleClick(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseMiddleButtonDown(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseMiddleButtonUp(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseMoved(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseRightButtonDoubleClick(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseRightButtonDown(ulong hCtxMenu, uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseRightButtonUp(uint nFlags, int pointx, int pointy, ulong transformHDC)
        {
        }

        public void MouseWheelTurned(uint nFlags, int distance, int pointx, int pointy, ulong transformHDC)
        {
        }

        #endregion

        public void OperationCanceled()
        {
        }

        public void SaveState()
        {
        }

        public void SetActive(bool isActive)
        {
        }

        public void SetParam(string name, object value)
        {
        }

        public void SetSelected(bool isSelected)
        {
        }

        /// <summary>
        /// Get the meta object
        /// </summary>
        /// <param name="metaPart">MetaPart</param>
        /// <param name="metaFco">returns the MetaFCO</param>
        /// <returns>true if succeeded</returns>
        bool GetMetaFCO(MgaMetaPart metaPart, out MgaMetaFCO metaFco)
        {
            metaFco = null;
            if (metaPart == null)
                return false;

            metaFco = metaPart.Role.Kind;
            return metaFco != null;
        }

        /// <summary>
        /// Return GME registry value
        /// </summary>
        /// <param name="val">returned value</param>
        /// <param name="path">path in registry</param>
        /// <returns>true if settins found and non-empty</returns>
        bool GetPreference(out string val, string path)
        {
            val = null;
            if (myobj != null)
            {
                val = myobj.RegistryValue[path];
            }
            else if (mymetaobj != null)
            {
                val = mymetaobj.RegistryValue[path];
            }
            return !string.IsNullOrEmpty(val);
        }

        // Get integer property from GME registry (if hex true, string is handled as hexa starting with 0x)
        bool GetPreference(out int val, string path, bool hex)
        {
            val = 0;
            string strVal;
            if (GetPreference(out strVal, path))
            {
                if (hex && strVal.Length >= 2)
                {
                    return int.TryParse(strVal.Substring(2), // omit 0x
                            System.Globalization.NumberStyles.HexNumber,
                            System.Globalization.NumberFormatInfo.InvariantInfo,
                            out val);
                }
                else if (!hex)
                {
                    return int.TryParse(strVal, out val);
                }
            }
            return false;
        }

        // Get color settings
        bool GetColorPreference(out Color color, string path)
        {
            int i;
            if (GetPreference(out i, path, true))
            {
                int r = (i & 0xff0000) >> 16;
                int g = (i & 0xff00) >> 8;
                int b = i & 0xff;
                color = Color.FromArgb(255, r, g, b);
                return true;
            }
            color = Color.Black;
            return false;
        }

        public const string EXECUTIONTASK_ICON_NAME = "ExecutionTask.png";

        Dictionary<string, ComComponent> interpreters =
            new Dictionary<string, ComComponent>();

        Queue<TaskInfo> workflow =
            new Queue<TaskInfo>();

        public string TaskProgId { get; set; }

        public string Parameters { get; set; }

        public string LastMetaKind { get; set; }

        #region Tunable Parameters

        public int IconHeight { get { return 32; } }

        public int IconWidth { get { return 32; } }

        public int TaskPadding
        {
            get { return IconWidth; }
        }

        public Icon InvalidTask
        {
            get { return SystemIcons.Warning; }
        }

        public Icon UndefinedWorkFlow
        {
            get { return SystemIcons.Question; }
        }

        public int LineStartPadding { get { return 2; } }

        public int LineEndPadding { get { return 2; } }

        [ComVisible(false)]
        public LineCap LineStartCap { get { return LineCap.Flat; } }

        [ComVisible(false)]
        public LineCap LineEndCap { get { return LineCap.ArrowAnchor; } }

        public float LineWidth { get { return 1; } }

        #endregion
    }

    public class TaskInfo
    {
        public bool IsComComponent
        {
            get
            {
                return !string.IsNullOrWhiteSpace(this.COMName);
            }
        }

        public string COMName
        {
            get
            {
                return this._COMName;
            }
        }

        public string MetaName
        {
            get
            {
                return this._MetaName;
            }
        }

        public string IconName
        {
            get
            {
                return this._IconName;
            }
        }

        protected string _COMName = string.Empty;
        protected string _MetaName = string.Empty;
        protected string _IconName = string.Empty;

        public TaskInfo(string metaName)
        {
            this._MetaName = metaName;
        }

        public TaskInfo(string COMName, string metaName)
        {
            this._COMName = COMName;
            this._MetaName = metaName;
        }

        public void SetIconPath(string iconName)
        {
            this._IconName = iconName;
        }
    }
}

