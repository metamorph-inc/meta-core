using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2Modelica_v2.Modelica
{
    public class ComponentInfo
    {
        public ComponentInfo(GME.MGA.IMgaFCO gmeObject, string modelType = null, CyPhyCOMInterfaces.IMgaTraceability traceability = null)
        {
            this.Name = gmeObject.Name;
            this.AbsPath = gmeObject.AbsPath;
            this.ID = gmeObject.ID;
            this.Path = this.GetPath();
            this.Kind = gmeObject.Meta.Name;
            this.ModelType = modelType;
            if (this.Kind == typeof(CyPhy.Component).Name)
            {
                var component = CyPhyClasses.Component.Cast(gmeObject);
                this.AVMID = component.Attributes.AVMID;
            }

            if (traceability != null)
            {
                string id = string.Empty;
                if (traceability.TryGetMappedObject(gmeObject.ID, out id))
                {
                    GME.MGA.MgaFCO baseComponent = gmeObject.Project.GetFCOByID(id);
                    if (baseComponent != null)
                    {
                        if (baseComponent.Status == 1)
                        {
                            // object is already deleted
                            // try to get it in a different way.
                            var idChain = gmeObject.RegistryValue["Elaborator/ID_Chain"];
                            if (string.IsNullOrWhiteSpace(idChain) == false)
                            {
                                id = idChain.Split(new char[] { ',' }).LastOrDefault();
                                if (id != null)
                                {
                                    baseComponent = gmeObject.Project.GetFCOByID(id);
                                    if (baseComponent != null)
                                    {
                                        this.Reference = new ComponentInfo(baseComponent, modelType);
                                    }
                                }
                            }
                        }
                        else
                        {
                            this.Reference = new ComponentInfo(baseComponent, modelType);
                        }
                    }
                }
            }
        }

        public string Name { get; set; }
        public string AbsPath { get; set; }
        public string ID { get; set; }
        public string Path { get; set; }
        public string Kind { get; set; }
        public string AVMID { get; set; }
        public string ModelType { get; set; }

        public ComponentInfo Reference { get; set; }

        private string GetPath()
        {
            var sb = new StringBuilder();
            bool first = true;
            foreach (var item in this.AbsPath.Split(new string[] { "/@" }, StringSplitOptions.None))
            {
                if (first)
                {
                    first = false;
                    continue;
                }
                if (item.IndexOf('|') > 0)
                {
                    sb.Append("/@" + item.Substring(0, item.IndexOf('|')));
                }
                else
                {
                    sb.Append("/@" + item);
                }
            }

            return sb.ToString();
        }
    }
}
