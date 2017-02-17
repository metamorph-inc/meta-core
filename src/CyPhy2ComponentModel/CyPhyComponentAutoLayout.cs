using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CyPhy = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyClasses = ISIS.GME.Dsml.CyPhyML.Classes;
using GME.MGA;
using System.Collections;

/*******************************************************
 * WARNING -- FIRST DRAFT
 * This code is far more complicated than necessary.
 * In the future, it will hopefully be more maintainable
 * and easily extended to include more aspects.
 ********************************************************/

namespace CyPhy2ComponentModel
{
    public static class CyPhyComponentAutoLayout
    {
        // Prioritize kinds by aspect
        private static Dictionary<String, ArrayList> d_ClassPrioritiesByAspect = new Dictionary<string, ArrayList>()
        {
            {
                "All", new ArrayList() {
                    typeof(CyPhy.Connector),
                    typeof(CyPhy.Port),
                    typeof(CyPhy.DomainModelPort),
                    typeof(CyPhy.ModelicaModel),
                    typeof(CyPhy.ManufacturingModel),
                    typeof(CyPhy.CADModel),
                    typeof(CyPhy.DomainModel),
                    typeof(ISIS.GME.Common.Interfaces.FCO)
                }
            }
        };

        private static void SetCoordinates(ISIS.GME.Common.Interfaces.FCO fco, String s_Coord)
        {
            string[] sa_coords = s_Coord.Split(',');
            int x = int.Parse(sa_coords[0]);
            int y = int.Parse(sa_coords[1]);

            foreach (IMgaPart part in (fco.Impl as IMgaFCO).Parts)
            {
                part.SetGmeAttrs(null, x, y);
            }
        }

        private static Dictionary<String, String> d_AspectPositionRegnode = new Dictionary<string, string> {
            { "All", "PartRegs/All/Position" }
        };

        private class PositionTracker
        {
            private int i_x;
            private int i_y;
            private static int i_MaxX = 1000;
            private static int i_DefaultVertOffset = 125;
            private static int i_DefaultX = 25;

            public PositionTracker()
            {
                i_x = i_DefaultX;
                i_y = 25;
            }

            public String GetCoordinates()
            {
                return String.Format("{0},{1}", i_x, i_y);
            }

            public void Increment(ISIS.GME.Common.Interfaces.FCO fco)
            {
                if (fco is CyPhy.ValueFlowTarget)
                {
                    int i_CharsOver10 = Math.Max(fco.Name.Length - 12, 12);
                    int i_XOffset = 160 + (i_CharsOver10 * 10);
                    i_x += i_XOffset;
                }
                else if (fco is CyPhy.Connector)
                {
                    i_x += 250;
                }
                else if (fco is CyPhy.Port)
                {
                    i_x += 100;
                }
                else if (fco is CyPhy.ModelicaConnector)
                {
                    i_x += 250;
                }
                else
                {
                    i_x += 250;
                }

                if (i_x > i_MaxX)
                {
                    NextLine();
                }
            }

            public String GetCoordinatesAndIncrement(ISIS.GME.Common.Interfaces.FCO fco)
            {
                String s_Rtn = GetCoordinates();
                Increment(fco);
                return s_Rtn;
            }

            public void NextLine()
            {
                i_x = i_DefaultX;
                i_y += i_DefaultVertOffset;
            }
        }

        public static void LayoutChildrenByName(ISIS.GME.Common.Interfaces.Model parent)
        {
            PositionTracker pt = new PositionTracker();

            foreach (ISIS.GME.Common.Interfaces.FCO child in parent.AllChildren.OrderBy(child => child.Name))
            {
                String s_Coord = pt.GetCoordinatesAndIncrement(child);
                SetCoordinates(child, s_Coord);
            }
        }

        public static void LayoutComponent(CyPhy.Component c)
        {
            PositionTracker pt = new PositionTracker();

            Dictionary<string, bool> d_IsPositioned = new Dictionary<string, bool>();

            foreach (CyPhy.Connector conn in c.Children.ConnectorCollection)
            {
                String s_Coord = pt.GetCoordinatesAndIncrement(conn);
                SetCoordinates(conn, s_Coord);
                d_IsPositioned[conn.ID] = true;
            }
            foreach (CyPhy.Port p in c.Children.PortCollection)
            {
                String s_Coord = pt.GetCoordinatesAndIncrement(p);
                SetCoordinates(p, s_Coord);
                d_IsPositioned[p.ID] = true;
            }
            foreach (CyPhy.Parameter p in c.Children.ParameterCollection)
            {
                String s_Coord = pt.GetCoordinatesAndIncrement(p);
                SetCoordinates(p, s_Coord);
                d_IsPositioned[p.ID] = true;
            }
            foreach (CyPhy.Property p in c.Children.PropertyCollection)
            {
                String s_Coord = pt.GetCoordinatesAndIncrement(p);
                SetCoordinates(p, s_Coord);
                d_IsPositioned[p.ID] = true;
            }
            foreach (CyPhy.CustomFormula cf in c.Children.CustomFormulaCollection)
            {
                String s_Coord = pt.GetCoordinatesAndIncrement(cf);
                SetCoordinates(cf, s_Coord);
                d_IsPositioned[cf.ID] = true;
            }
            foreach (CyPhy.Resource r in c.Children.ResourceCollection)
            {
                String s_Coord = pt.GetCoordinatesAndIncrement(r);
                SetCoordinates(r, s_Coord);
                d_IsPositioned[r.ID] = true;
            }
            pt.NextLine();
            pt.Increment(null);
            foreach (ISIS.GME.Common.Interfaces.FCO fco in c.AllChildren.Where(fco =>
                fco is CyPhy.ModelicaModel || fco is CyPhy.CADModel || fco is CyPhy.ManufacturingModel))
            {
                Boolean b;
                d_IsPositioned.TryGetValue(fco.ID, out b);
                if (!b)
                {
                    String s_Coord = pt.GetCoordinatesAndIncrement(fco);
                    SetCoordinates(fco, s_Coord);
                    d_IsPositioned[fco.ID] = true;
                }
                pt.Increment(null);
                pt.Increment(null);
            }
            foreach (ISIS.GME.Common.Interfaces.FCO fco in c.AllChildren.Where(fco =>
                fco is ISIS.GME.Common.Interfaces.Atom ||
                fco is ISIS.GME.Common.Interfaces.Model ||
                fco is ISIS.GME.Common.Interfaces.Reference ||
                fco is ISIS.GME.Common.Interfaces.Set))
            {
                Boolean b;
                d_IsPositioned.TryGetValue(fco.ID, out b);
                if (!b)
                {
                    String s_Coord = pt.GetCoordinatesAndIncrement(fco);
                    SetCoordinates(fco, s_Coord);
                    d_IsPositioned[fco.ID] = true;
                }
            }

            /*foreach (KeyValuePair<String, ArrayList> kvp in d_ClassPrioritiesByAspect)
            {
                String s_CurrentAspect = kvp.Key;
                foreach (var t in kvp.Value)
                {
                    
                }
            }*/
        }
    }
}
