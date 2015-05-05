namespace GME.CSharp
{
    using GME.MGA;
    using GME.Util;

    /// <summary>
    /// Defines the static values for COM registration.
    /// </summary>
    internal abstract class ComponentConfig
    {
        /// <summary>
        /// Set paradigm name. Provide * if you want to register it for all paradigms.
        /// </summary>
        public const string ParadigmName = "CyPhyML";

        /// <summary>
        /// Set the human readable name of the interpreter. You can use white space characters.
        /// </summary>
        public const string ComponentName = "CyPhyElaborateCS";

        /// <summary>
        /// Specify an icon path
        /// </summary>
        public const string IconName = "CyPhyElaborateCS.ico";

        /// <summary>
        /// Tool tip will show this text on the GME tool bar.
        /// </summary>
        public const string Tooltip = "CyPhyElaborateCS";

        /// <summary>
        /// Type of the registration mode.
        /// </summary>
        public const regaccessmode_enum RegistrationMode = regaccessmode_enum.REGACCESS_SYSTEM;

        /// <summary>
        /// Programmatic Identifier of the COM component.
        /// </summary>
        public const string ProgID = "MGA.Interpreter.CyPhyElaborateCS";

        /// <summary>
        /// GUID of the COM component.
        /// </summary>
        public const string Guid = "01F4C76D-7980-4C5E-BD65-EA7C0267F55B";

        /// <summary>
        /// Gets or sets the icon path of this component. If null, updated with the assembly path + the iconName dynamically on registration.
        /// </summary>
        public static string IconPath { get; set; }

        /// <summary>
        /// Gets the type of the component. Uncomment the flag if your component is paradigm independent.
        /// </summary>
        public static componenttype_enum ComponentType
        {
            get
            {
                return componenttype_enum.COMPONENTTYPE_INTERPRETER;
            }
        }
    }
}
