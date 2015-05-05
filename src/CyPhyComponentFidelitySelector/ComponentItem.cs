// -----------------------------------------------------------------------
// <copyright file="ComponentItem.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace CyPhyComponentFidelitySelector
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using System.ComponentModel;

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ComponentItem
    {
        internal IMgaModel component { get; set; }

        public string Classification { get; set; }
        public List<IMgaModel> ModelicaModel { get; set; }

        public BindingList<ModelicaModel> ModelicaModelOptions { get; set; }

        public string Guid { get; set; }

        public ComponentItem()
        {

        }

        public ComponentItem(IMgaModel component)
        {
            this.component = component;
            ModelicaModel = new List<IMgaModel>();
            ModelicaModelOptions = new BindingList<ModelicaModel>();

            Classification = component.StrAttrByName["Classifications"];

            component
                .ChildFCOs
                .Cast<IMgaFCO>()
                .Where(x => x.Meta.Name == "ModelicaModel")
                .ToList()
                .ForEach(x => ModelicaModel.Add(x as IMgaModel));

            ModelicaModel.Sort((x, y) => x.GetGuidDisp().CompareTo(y.GetGuidDisp()));
            int id = 0;

            ModelicaModel.ForEach(x => 
                {
                    var opt = new CyPhyComponentFidelitySelector.ModelicaModel()
                    {
                        Key = id++,
                        Name = x.Name,
                        Guid = x.GetGuidDisp()
                    };
                    ModelicaModelOptions.Add(opt);
                });


            Guid = component.GetGuidDisp();
        }
    }
}
