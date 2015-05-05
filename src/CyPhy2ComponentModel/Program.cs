using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using GME.MGA;
using GME.MGA.Core;
using GME.MGA.Parser;
using ISIS.GME.Common.Interfaces;

// using domain specific interfaces
using CyPhyML = ISIS.GME.Dsml.CyPhyML.Interfaces;
using CyPhyMLClasses = ISIS.GME.Dsml.CyPhyML.Classes;

namespace CyPhy2ComponentModel {
    
    public static class Convert {

        public static CyPhyML.Component AVMComponent2CyPhyML(CyPhyML.ComponentAssembly cyPhyMLComponentParent, avm.Component avmComponent, bool resetUnitLib = true, object messageConsole = null) {
            CyPhyML.Component c_rtn = AVM2CyPhyML.CyPhyMLComponentBuilder.AVM2CyPhyML(cyPhyMLComponentParent, avmComponent, resetUnitLib, messageConsole);
//            CyPhyComponentAutoLayout.LayoutComponent( c_rtn );
            return c_rtn;
        }

        public static CyPhyML.Component AVMComponent2CyPhyML(CyPhyML.Components cyPhyMLComponentParent, avm.Component avmComponent, bool resetUnitLib = true, object messageConsole = null)
        {
            CyPhyML.Component c_rtn = AVM2CyPhyML.CyPhyMLComponentBuilder.AVM2CyPhyML(cyPhyMLComponentParent, avmComponent, resetUnitLib, messageConsole);
//             CyPhyComponentAutoLayout.LayoutComponent(c_rtn);
             return c_rtn;
         }
        
        public static avm.Component CyPhyML2AVMComponent( CyPhyML.Component cyPhyMLComponent ) {
            return CyPhyML2AVM.AVMComponentBuilder.CyPhyML2AVM(cyPhyMLComponent);
        }
        
    }
    
}
