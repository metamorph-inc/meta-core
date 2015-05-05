using System;
using System.Collections.Generic;
using GME.MGA;
using System.Diagnostics.Contracts;
using System.IO;
using System.Windows.Forms;
using ISIS.VehicleForge;
using ISIS.Web;
using System.Linq;

namespace ClmLib
{
    public class Clm
    {
        public IMgaModel CyPhyComponent { get; set; }
        public string ProjectDir { get; set; }
        public string ZipDownloadDir { get; set; }
        public string ComponentCategory { get; set; }

        public Clm(IMgaModel cyPhyComponent)
        {

            this.CyPhyComponent = cyPhyComponent as IMgaModel;
            this.ProjectDir = Path.GetDirectoryName(this.CyPhyComponent.Project.ProjectConnStr.Substring("MGA=".Length));

            this.ZipDownloadDir = Path.Combine(this.ProjectDir, "VFComponentDownloads");
        }

        public List<MgaFCO> ShowAndSelectVFComponents(Dictionary<string, ComponentLibraryFilterParameter> filterParameters)
        {
            // VF
            List<MgaFCO> importedComponents = new List<MgaFCO>();

            using (VFExchangeConfig config = new VFExchangeConfig())
            {
                var dialogResult = config.ShowDialog();

                if (dialogResult == DialogResult.OK)
                {
                    Credentials credentials = config.Credentials;

                    // FOR DEBUGGING. Should remove these two lines...
                    // For testing only
                    //if (System.IO.File.Exists("pass.txt"))
                    //{
                    //    string loginInfo = System.IO.File.ReadAllText("pass.txt");
                    //    credentials.Url = loginInfo.Split(',').ElementAt(0);
                    //    credentials.Username = loginInfo.Split(',').ElementAt(1);
                    //    credentials.Password = loginInfo.Split(',').ElementAt(2);
                    //}

                    try
                    {
                        using (VFWebClient vfWebClient = new VFWebClient(credentials.Url, credentials))
                        {
                            VFExchange exchange = new VFExchange(vfWebClient);

                            // in the future, may also allow the user to define parameter ranges in a form?

                            var filter = new VFExchangeFilter()
                            {
                                Category = Clm.GetComponentCategory(this.CyPhyComponent),
                                NumberOfResults = 10,
                                StartPosition = 0,
                                ParameterList = filterParameters
                            };

                            using (VFComponentExchangeForm vfComponentSelection = new VFComponentExchangeForm(exchange, filter, this.ZipDownloadDir))
                            {
                                var dr = vfComponentSelection.ShowDialog();

                                if (dr == DialogResult.OK)
                                {
                                    // import the component zip files from VFComponentDownloads

                                    importedComponents.AddRange(Clm.ImportComponentZips(this.CyPhyComponent.Project, vfComponentSelection.DownloadedComponentZips));
                                }
                            }
                        }
                    }
                    catch (VFException ex)
                    {
                        string message = string.Format("{0}{1}{2}", ex.Message, Environment.NewLine, ex.GetType());

                        MessageBox.Show(message);
                    }

                }
            }

            return importedComponents;
        }

        public Dictionary<string, ComponentLibraryFilterParameter> GenerateFilter(
            IMgaModel alternativeDesignContainer, 
            System.IO.TextWriter errorWriter = null, 
            System.IO.TextWriter infoWriter = null)
        {
            var consoleErrorLogger = (errorWriter == null) ? Console.Error : errorWriter;
            var consoleInfoLogger = (infoWriter == null) ? Console.Out : infoWriter;

            Dictionary<string, ComponentLibraryFilterParameter> filterParams = 
                new Dictionary<string, ComponentLibraryFilterParameter>();

            List<string> filtersWithConflictingMaxMin = new List<string>();

            // Get all the PropertyConstraint objects in the DesignContainer
            var propertyConstraints =
                alternativeDesignContainer.ChildFCOs.OfType<MgaAtom>().Where(x => x.Meta.Name == "PropertyConstraint");

            // Iterate over the PropertyConstraint collection and create/modify filters:
            foreach (MgaFCO propConstraint in propertyConstraints)
            {
                string filterName = null;
                string filterUnit = null;
                string targetType = propConstraint.StrAttrByName["TargetType"];
                double targetValue = propConstraint.FloatAttrByName["TargetValue"];

                // Check if they are connected to a Property
                var propertyConstraintConnectionPoints = propConstraint.PartOfConns;

                // One PropertyConstraint might be connected to multiple Properties (valid in GME); might need to create multiple filters per
                foreach (MgaConnPoint cp in propertyConstraintConnectionPoints)
                {
                    MgaConnection conn = (cp as MgaConnPoint).Owner;

                    if (conn.Meta.Name != "PropertyConstraintBinding")
                    {
                        continue;
                    }
                    else
                    {
                        // get the Property
                        MgaFCO property = (conn as IMgaSimpleConnection).Dst;

                        // We want to skip PropertyConstraints connected directly to the selected Component (they don't agree with Desert...)
                        if (property.ParentModel.ID == this.CyPhyComponent.ID)
                        {
                            errorWriter.WriteLine(
                                "PropertyConstraint '{0}' should not be connected directly to the ComponentRef Property. Constraint on Property '{1}' will be ignored.",
                                propConstraint.Name,
                                property.Name);
                            infoWriter.WriteLine(
                                "Please create a Property within this DesignContainer, name it 'ComponentProperty.Name', and connect the PropertyConstraint.");

                            continue;
                        }

                        // Check if the 'floating' Property has a matching property (by name) in the selected component
                        bool componentHasMatchingProperty = false;
                        
                        foreach (MgaConnPoint connPoint in property.PartOfConns)
                        {
                            MgaConnection valueFlow = connPoint.Owner;

                            if (valueFlow.Meta.Name != "ValueFlow")
                            {
                                continue;
                            }

                            // check if the ValueFlow is FROM the component TO the 'floating' property
                            MgaFCO dst = (valueFlow as IMgaSimpleConnection).Dst;
                            MgaFCO src = (valueFlow as IMgaSimpleConnection).Src;

                            if (property != dst)
                            {
                                errorWriter.WriteLine(
                                    "DesignContainer Property '{0}' is the Source of a ValueFlow into '{1}'; Constraint '{2}' will be ignored.",
                                    property.Name,
                                    dst.Name,
                                    propConstraint.Name);
                                infoWriter.WriteLine(
                                    "To correctly filter your search results, DesignContainer Properties must get their values from Component Properties.");
                                continue;
                            }

                            bool srcIsWithinComponent = src.ParentModel.ID == this.CyPhyComponent.ID;
                            bool namesMatch = property.Name == src.Name;

                            if (srcIsWithinComponent && namesMatch)
                            {
                                componentHasMatchingProperty = true;
                                break;
                            }
                        }

                        if (!componentHasMatchingProperty)
                        {
                            errorWriter.WriteLine(
                                "Property '{0}' is not connected to a Property with a matching name. Constraint '{1}' will be ignored.", 
                                property.Name,
                                propConstraint.Name);
                            infoWriter.WriteLine(
                                "To correctly filter your search results, the names of DesignContainer Properties must match the Properties within Components.");

                            continue;
                        }

                        filterName = property.Name;

                        if ((property as MgaReference).Referred != null)
                        {
                            // TODO: Might need to use Adam's QUDT stuff to get the correct unit; for now, just get the referred unit's name.
                            filterUnit = (property as MgaReference).Referred.Name;
                        }
                    }

                    // if that property already has a filter, modify it (add max, add min)
                    // else create a new filter for this property
                    //bool discreteFilter = false;
                    double newMaximum = double.NaN;
                    double newMinimum = double.NaN;

                    //double myEpsilon = targetValue * 1E-12;


                    double scaledEpsilon = targetValue == 0 ?
                        double.Epsilon :
                        Math.Pow(10, Math.Floor(Math.Log10(targetValue)) - 15);


                    switch (targetType)
                    {
                        case "Must Be Less Than":
                            newMaximum = targetValue;
                            break;
                        case "Must Not Exceed":
                            newMaximum = targetValue + scaledEpsilon;
                            break;
                        case "Must Equal":
                            newMaximum = targetValue + scaledEpsilon;
                            newMinimum = targetValue - scaledEpsilon;
                            //discreteFilter = true;
                            break;
                        case "Must Equal or Exceed":
                            newMinimum = targetValue - scaledEpsilon;
                            break;
                        default: // "Must Exceed"
                            newMinimum = targetValue;
                            break;
                    }

                    // Create a new filter object
                    ContinuousFilterParameter newParam = new ContinuousFilterParameter(filterName);

                    // Check if there is an existing filter with this name
                    if (filterParams.ContainsKey(filterName))
                    {
                        // Get the existing filter
                        ComponentLibraryFilterParameter filterParameter = filterParams[filterName];
                        // Type check it
                        if (filterParameter is ContinuousFilterParameter)
                        {
                            // Assign it to this local var to modify it
                            newParam = filterParameter as ContinuousFilterParameter;
                        }
                    }

                    // Overwrite filter attributes with new info, or leave them as before

                    double currentMinimum = double.MinValue;
                    double currentMaximum = double.MaxValue;

                    // check if the PropertyConstraint could affect the filter's minimum value
                    if (!double.IsNaN(newMinimum))
                    {
                        // if the current MinValue is of type 'double', compare with newMinimum, possibly overwrite
                        if (double.TryParse(newParam.MinValue, out currentMinimum))
                        {
                            if (newMinimum > currentMinimum)
                            {
                                // the 'new' PropertyConstraint minimum is higher (more strict) than the existing one; overwrite it
                                newParam.MinValue = newMinimum.ToString("G17");
                            }
                        }
                    }

                    // check if the PropertyConstraint could affect the filter's maximum value
                    if (!double.IsNaN(newMaximum))
                    {
                        // if the current MaxValue is of type 'double', compare with newMaximum, possibly overwrite
                        if (double.TryParse(newParam.MaxValue, out currentMaximum))
                        {
                            if (newMaximum < currentMaximum)
                            {
                                // the 'new' PropertyConstraint maximum is lower (more strict) than the existing one; overwrite it
                                newParam.MaxValue = newMaximum.ToString("G17");
                            }
                        }
                    }

                    newParam.Unit = filterUnit != null ? filterUnit : null;
                    filterParams[filterName] = newParam;

                    // check if a filter's minimum is greater than its maximum, log the conflict
                    if (double.TryParse(newParam.MaxValue, out currentMaximum) &&
                        double.TryParse(newParam.MinValue, out currentMinimum))
                    {
                        if (currentMaximum < currentMinimum)
                        {
                            filtersWithConflictingMaxMin.Add(filterName);
                        }
                    }

                    //if (discreteFilter)
                    //{
                    //    continue;
                    //    //DiscreteFilterParameter newParam = new DiscreteFilterParameter(filterName);

                    //    //List<string> filterParameterValues = new List<string>();
                    //    //filterParameterValues.Add(targetValue.ToString());
                    //    //newParam.Values = filterParameterValues;

                    //    //filterParams[filterName] = newParam;
                    //}
                    //else
                    //{
                        // Make a ContinuousFilter (this is done in all cases)
                    //}
                }
            }

            foreach (string name in filtersWithConflictingMaxMin)
            {
                if (filterParams.ContainsKey(name))
                {
                    string min = (filterParams[name] as ContinuousFilterParameter).MinValue;
                    string max = (filterParams[name] as ContinuousFilterParameter).MaxValue;

                    consoleErrorLogger.WriteLine(
                        "Property '{0}' has conflicting PropertyConstraints (Min {1} > Max {2}); no filter will be generated.",
                        name,
                        min,
                        max);

                    filterParams.Remove(name);
                }
            }

            return filterParams;
        }

        public List<IMgaFCO> GetRelatedComponents()
        {
            string classification = Clm.GetComponentCategory(this.CyPhyComponent);

            List<IMgaFCO> result = new List<IMgaFCO>();

            MgaFilter filter = this.CyPhyComponent.Project.CreateFilter();
            filter.Kind = "Component";

            var components = this.CyPhyComponent.Project.AllFCOs(filter);

            foreach (IMgaFCO componentItem in components)
            {
                if (Clm.CompareClassifications(componentItem.StrAttrByName["Classifications"], classification))
                {
                    result.Add(componentItem);
                }
            }

            // TODO: filter the 'result' based on any parameters from the DesignContainer

            return result;
        }

        #region Static functions

        public static MgaFCO ImportComponentZip(MgaProject mgaProject, string zipPath)
        {
            try
            {
                CyPhyComponentImporter.CyPhyComponentImporterInterpreter importer =
                    new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();

                importer.Initialize(mgaProject);

                return importer.ImportFile(mgaProject, Path.GetDirectoryName(mgaProject.ProjectConnStr.Substring("MGA=".Length)), Path.GetFullPath(zipPath)) as MgaFCO;
            }
            catch (Exception)
            {
                throw new VFDownloadFailedException();
            }
        }

        public static List<MgaFCO> ImportComponentZips(MgaProject mgaProject, List<string> zipPaths)
        {
            IMgaFCOs importedComponentFcos;

            try
            {
                CyPhyComponentImporter.CyPhyComponentImporterInterpreter importer =
                    new CyPhyComponentImporter.CyPhyComponentImporterInterpreter();

                importer.Initialize(mgaProject);

                importedComponentFcos =
                    importer.ImportFiles(mgaProject, Path.GetDirectoryName(mgaProject.ProjectConnStr.Substring("MGA=".Length)), zipPaths.ToArray());
            }
            catch (Exception)
            {
                throw new VFDownloadFailedException();
            }

            return importedComponentFcos.OfType<MgaFCO>().ToList();
        }

        public static string GetComponentCategory(IMgaModel component)
        {
            Contract.Requires(component != null);

            string componentCategory = "";

            if (component != null)
            {
                componentCategory = component.StrAttrByName["Classifications"];
            }

            return componentCategory;
        }

        public static bool CompareClassifications(
            string classification1,
            string classification2)
        {
            if (string.IsNullOrWhiteSpace(classification1) ||
                string.IsNullOrWhiteSpace(classification2))
            {
                return false;
            }

            return classification1.Trim() == classification2.Trim();
        }

        #endregion
    }
}
