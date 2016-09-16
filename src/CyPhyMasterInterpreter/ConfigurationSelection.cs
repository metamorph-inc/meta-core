using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;
using System.Runtime.InteropServices;

namespace CyPhyMasterInterpreter
{
    public class ConfigurationSelection
    {
        //public IMgaFCOs AllConfigurations { get; set; }
        public IMgaModel Context { get; set; }
        public IMgaFCOs SelectedConfigurations { get; set; }
        public bool PostToJobManager { get; set; }
        public bool KeepTemporaryModels { get; set; }
        // TODO: add Job Manager Instance selection (maybe url/port number is enough or some kind of descriptor)

        public bool OpenDashboard { get; set; }
    }

    [Guid("C0FFF9E6-6E0E-471C-9A2D-182DFC244A7F"),
    ProgId("CyPhyMasterInterpreter.ConfigurationSelectionLight"),
    ClassInterface(ClassInterfaceType.AutoDual),
    ComVisible(true)]
    public class ConfigurationSelectionLight
    {
        public string ContextId { get; set; }
        public string[] SelectedConfigurationIds { get; set; }
        public bool PostToJobManager { get; set; }
        public bool KeepTemporaryModels { get; set; }

        public void SetSelectedConfigurationIds(object config_ids)
        {
            SelectedConfigurationIds = ((System.Collections.IEnumerable)config_ids).Cast<string>().ToArray();
        }

        [ComVisible(false)]
        public ConfigurationSelection GetConfigurationSelection(MgaProject project)
        {
            ConfigurationSelection config = new ConfigurationSelection()
            {
                PostToJobManager = this.PostToJobManager,
                KeepTemporaryModels = this.KeepTemporaryModels,
                //OpenDashboard=default
            };
            try
            {
                project.BeginTransactionInNewTerr();

                using (CyPhyGUIs.GMELogger logger = new CyPhyGUIs.GMELogger(project))
                {
                    config.Context = this.GetGMEObjectFromIdentification<MgaModel>(project, this.ContextId);

                    if (config.Context == null)
                    {
                        logger.WriteError("Context was not found based on this id '{0}'", this.ContextId);
                    }

                    config.SelectedConfigurations = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));
                    foreach (var selectedId in this.SelectedConfigurationIds)
                    {
                        var selectedElement = this.GetGMEObjectFromIdentification<MgaFCO>(project, selectedId);

                        if (selectedElement != null)
                        {
                            // should be CWC or Component assembly
                            config.SelectedConfigurations.Append(selectedElement);
                        }
                        else
                        {
                            logger.WriteError("Configuration was not found based on this id '{0}'", selectedId);
                        }
                    }
                }
            }
            finally
            {
                project.AbortTransaction();
            }

            return config;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="project"></param>
        /// <param name="identification"></param>
        /// <returns></returns>
        [ComVisible(false)]
        public T GetGMEObjectFromIdentification<T>(MgaProject project, string identification) where T : IMgaObject
        {
            T result = default(T);

            // is project in transaction already?
            bool projectWasNotInTransaction = (project.ProjectStatus & 8) == 0;

            try
            {
                if (projectWasNotInTransaction)
                {
                    // open a transaction if it is not in a transaction already.
                    project.BeginTransactionInNewTerr();
                }

                // regexp for GME ID - case insensitive
                string idPattern = @"(id-006[0-9a-f]{1}-[0-9a-f]{8})"; // hexadecimal char: [0-9a-f]

                System.Text.RegularExpressions.Regex regexId =
                    new System.Text.RegularExpressions.Regex(idPattern, System.Text.RegularExpressions.RegexOptions.IgnoreCase);

                bool isId = regexId.IsMatch(identification);

                // regexp for GUID - case insensitive
                string guidPattern = @"(\{[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}\})";

                System.Text.RegularExpressions.Regex regexGuid =
                    new System.Text.RegularExpressions.Regex(guidPattern, System.Text.RegularExpressions.RegexOptions.IgnoreCase);

                bool isGuid = regexGuid.IsMatch(identification);

                if (isId)
                {
                    // assume it is an id
                    identification = identification.ToLowerInvariant();

                    result = (T)project.GetObjectByID(identification);
                }
                else if (isGuid)
                {
                    // FIXME: does not work for folders

                    // assume it is a GUID
                    identification = identification.ToLowerInvariant();

                    // this may take time, no better method on the GME API at this point.
                    foreach (MgaFCO fco in project.AllFCOs(project.CreateFilter()))
                    {
                        if (fco.GetGuidDisp() == identification)
                        {
                            result = (T)fco;
                            break;
                        }
                    }
                }
                else if (identification.StartsWith("/"))
                {
                    // assume it is an AbsPath
                    if (identification.StartsWith("/@") == false)
                    {
                        // inject @ signs for the user
                        // FIXME: what if the name has / for any objects?
                        identification = identification.Replace("/", "/@");
                    }

                    result = (T)project.ObjectByPath[identification];
                }
                else
                {
                    throw new FormatException(string.Format("Identification must be a GME ID 'id-006X-YYYYYYYY' or a GUID '{{guid}}' or an AbsPath '/@...' : given value:'{0}'", identification));
                }

            }
            finally
            {
                if (projectWasNotInTransaction)
                {
                    project.AbortTransaction();
                }
            }

            return result;
        }

        [ComVisible(false)]
        public T GetGMEObjectFromIdentification<T>(MgaProject project, Guid guid) where T : IMgaObject
        {
            return this.GetGMEObjectFromIdentification<T>(project, guid.ToString("B"));
        }
    }
}
