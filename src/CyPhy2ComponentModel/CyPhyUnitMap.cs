using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using avm;
using CyPhy2ComponentModel;
using GME.CSharp;
using GME.MGA;
using GME.MGA.Meta;
// using domain specific interfaces
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace AVM2CyPhyML
{
    public class CyPhyUnitMap
    {
        public Dictionary<String, CyPhyML.unit> _unitSymbolCyPhyMLUnitMap = new Dictionary<string, CyPhyML.unit>();
        protected List<CyPhyML.Units> _cyPhyMLUnitsFolders;
        public CyPhyML.RootFolder _cyPhyMLRootFolder;

        public void init(bool resetUnitLibrary = false)
        {
            getCyPhyMLUnits();
            getCyPhyMLNamedUnits(resetUnitLibrary);
        }

        private void getCyPhyMLUnits()
        {
            _cyPhyMLUnitsFolders = new List<CyPhyML.Units>();

            // Collect all of the Root Folders in the project.
            // They will be sorted, with the QUDT lib in front, followed by all other libs, then the user's Root Folder.
            var cyPhyMLRootFolderList = new List<CyPhyML.RootFolder>();
            cyPhyMLRootFolderList.AddRange(_cyPhyMLRootFolder.LibraryCollection
                                                             .OrderByDescending(lc => lc.Name.Equals("UnitLibrary QUDT")));
            cyPhyMLRootFolderList.Add(_cyPhyMLRootFolder);

            // Now, for each Root Folder that we gathered, go through and find all units, and add them to our master index.
            if (cyPhyMLRootFolderList.Count > 0)
            {
                cyPhyMLRootFolderList.ForEach(lrf => getCyPhyMLUnits(lrf));
            }
        }


        private void getCyPhyMLUnits(CyPhyML.RootFolder rootFolder)
        {
            foreach (CyPhyML.TypeSpecifications typeSpecifications in rootFolder.Children.TypeSpecificationsCollection)
            {
                foreach (CyPhyML.Units units in typeSpecifications.Children.UnitsCollection)
                {
                    _cyPhyMLUnitsFolders.Add(units);
                }
            }
        }


        private void getCyPhyMLNamedUnits(bool resetUnitLibrary = false)
        {
            if (false == _cyPhyMLUnitsFolders.Any())
            {
                return;
            }

            // If the caller has passed in this map already
            if (resetUnitLibrary)
            {
                _unitSymbolCyPhyMLUnitMap.Clear();
            }

            if (_unitSymbolCyPhyMLUnitMap.Count > 0)
            {
                return;
            }

            foreach (CyPhyML.unit cyPhyMLUnit in _cyPhyMLUnitsFolders.SelectMany(uf => uf.Children.unitCollection))
            {
                // Angle-type measures are an exception to this rule.
                /*
				if (cyPhyMLUnit.Attributes.Abbreviation != "rad" &&
					cyPhyMLUnit.Attributes.Abbreviation != "deg" &&
					isUnitless(cyPhyMLUnit))
				{
					continue;
				}*/

                if (!_unitSymbolCyPhyMLUnitMap.ContainsKey(cyPhyMLUnit.Attributes.Abbreviation))
                {
                    _unitSymbolCyPhyMLUnitMap.Add(cyPhyMLUnit.Attributes.Abbreviation, cyPhyMLUnit);
                }
                else if (cyPhyMLUnit is CyPhyML.conversion_based_unit) // Always prefer SI unit
                {
                    _unitSymbolCyPhyMLUnitMap[cyPhyMLUnit.Attributes.Abbreviation] = cyPhyMLUnit;
                }

                if (!_unitSymbolCyPhyMLUnitMap.ContainsKey(cyPhyMLUnit.Attributes.Symbol))
                {
                    _unitSymbolCyPhyMLUnitMap.Add(cyPhyMLUnit.Attributes.Symbol, cyPhyMLUnit);
                }
                else if (cyPhyMLUnit is CyPhyML.conversion_based_unit) // Always prefer SI unit
                {
                    _unitSymbolCyPhyMLUnitMap[cyPhyMLUnit.Attributes.Symbol] = cyPhyMLUnit;
                }

                if (!_unitSymbolCyPhyMLUnitMap.ContainsKey(cyPhyMLUnit.Attributes.FullName))
                {
                    _unitSymbolCyPhyMLUnitMap.Add(cyPhyMLUnit.Attributes.FullName, cyPhyMLUnit);
                }
                else if (cyPhyMLUnit is CyPhyML.conversion_based_unit) // Always prefer SI unit
                {
                    _unitSymbolCyPhyMLUnitMap[cyPhyMLUnit.Attributes.FullName] = cyPhyMLUnit;
                }
            }
        }

    }
}