// -----------------------------------------------------------------------
// <copyright file="PCCProperty.cs" company="ISIS-Vanderbilt University">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhyPET.PCC
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using GME.MGA.Core;

    using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
    using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

    /// <summary>
    /// Class representing a property in a component with a PCC distribution.
    /// </summary>
    public class PCCProperty
    {
        /// <summary>
        /// The type of distribution.
        /// </summary>
        private string distType;

        /// <summary>
        /// Initializes a new instance of the <see cref="PCCProperty" /> class.
        /// </summary>
        /// <param name="property">A property of a component.</param>
        public PCCProperty(CyPhy.Property property)
        {
            this.CyPhyProperty = property;
            this.DistType = "None";
        }

        /// <summary>
        /// Gets or sets the enclosed property.
        /// </summary>
        public CyPhy.Property CyPhyProperty { get; set; }

        /// <summary>
        /// Gets: the name of the property.
        /// </summary>
        public string Name
        {
            get
            {
                return this.CyPhyProperty.Name;
            }
        }

        /// <summary>
        /// Gets: the ID of the property.
        /// </summary>
        public string ID
        {
            get
            {
                return this.CyPhyProperty.ID;
            }
        }

        /// <summary>
        /// Gets or sets the distribution type for the property.
        /// </summary>
        public string DistType
        {
            get
            {
                if (this.distType == null)
                {
                    this.distType = "None";
                }

                return this.distType;
            }

            set
            {
                if (PCCPropertyInputDistributionEditor.DistToIndex.ContainsKey(value))
                {
                    this.distType = value;
                }
                else
                {
                    this.distType = "None";
                }
            }
        }

        /// <summary>
        /// Gets or sets the first parameter of the distribution type.
        /// </summary>
        public double Param1 { get; set; }

        /// <summary>
        /// Gets or sets the second parameter of the distribution type.
        /// </summary>
        public double Param2 { get; set; }

        /// <summary>
        /// Gets or sets the third parameter of the distribution type.
        /// </summary>
        public double Param3 { get; set; }

        /// <summary>
        /// Gets or sets the forth parameter of the distribution type.
        /// </summary>
        public double Param4 { get; set; }

        /// <summary>
        /// Gets or sets the first parameter of the distribution type.
        /// </summary>
        public void UpdateRegistryNode()
        {
            if (CyPhyPET.PCCPropertyInputDistributionEditor.DistToIndex[this.DistType] > 0)
            {
                (this.CyPhyProperty.Impl as MgaFCO).RegistryValue["Distribution"] = string.Format("{0},{1},{2},{3},{4}", this.DistType, this.Param1, this.Param2, this.Param3, this.Param4);
            }
            else
            {
                var dist = (this.CyPhyProperty.Impl as MgaFCO).RegistryNode["Distribution"];
                if (dist != null)
                {
                    dist.RemoveTree();
                }
            }
        }

        /// <summary>
        /// Updates the parameters and distribution type based on a nodeString from a "Distribution" registry node of a property in a component.
        /// </summary>
        /// <param name="nodeString">String value from an existing registry node.</param>
        /// <returns>Boolean indicating if the nodeString could be parsed.</returns>
        public bool UpdateDistributionFromExistingNode(string nodeString)
        {
            bool success = true;
            try
            {
                var args = nodeString.Split(',');

                this.DistType = args[0];
                if (CyPhyPET.PCCPropertyInputDistributionEditor.DistToIndex.ContainsKey(this.DistType) == false)
                {
                    throw new FormatException(string.Format("Distribution type not valid '{0}'", args[0]));
                }

                this.Param1 = Convert.ToDouble(args[1]);
                this.Param2 = Convert.ToDouble(args[2]);
                this.Param3 = Convert.ToDouble(args[3]);
                this.Param4 = Convert.ToDouble(args[4]);
            }
            catch (FormatException)
            {
                this.DistType = "None";
                success = false;
            }
            catch (IndexOutOfRangeException)
            {
                this.DistType = "None";
                success = false;
            }

            return success;
        }
    }
}
