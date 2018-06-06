using System.IO;
using GME.MGA.Parser;
using GME.MGA;
using GME.Util;
using GME.MGA.Meta;
using System;

namespace GME.MGA
{
    public static class MgaUtils
    {
        public static void ImportXMEForTest(string xmePath, out string connectionString)
        {
            string mgaPath = Path.Combine(Path.GetDirectoryName(xmePath), Path.GetFileNameWithoutExtension(xmePath) + "_test.mga");

            ImportXMEForTest(xmePath, mgaPath, out connectionString);
        }

        public static void ImportXMEForTest(string xmePath, string mgaPath, out string connectionString)
        {
            connectionString = "MGA=" + mgaPath;
            if (File.Exists(mgaPath))
            {
                // for faster debug cycles, skip importing .xme
                // if (xmePath.Contains("ToyDS.xme") && File.GetLastWriteTimeUtc(xmePath) < File.GetLastWriteTimeUtc(mgaPath))
                // {
                //     return;
                // }
                // delete the file if exists.
                // it could be a test to check if the importer has generated the mga file or not.
                File.Delete(mgaPath);
            }

            ImportXME(xmePath, mgaPath);
        }

        public static void ImportXME(string xmePath, string mgaPath, bool enableAutoAddons = false)
        {
            IMgaParser3 parser = new MgaParser();
            string paradigm;
            string paradigmVersion;
            object paradigmGuid;
            string basename;
            string version;
            parser.GetXMLInfo(xmePath, out paradigm, out paradigmVersion, out paradigmGuid, out basename, out version);

            parser = new MgaParser();
            MgaProject project = new MgaProject();
            MgaResolver resolver = new StrictMgaResolver();
            resolver.IsInteractive = false;
            parser.Resolver = resolver;
            project.Create("MGA=" + Path.GetFullPath(mgaPath), paradigm);
            if (enableAutoAddons)
            {
                project.EnableAutoAddOns(true);
            }
            try
            {
                parser.ParseProject(project, xmePath);
                project.Save();
            }
            finally
            {
                project.Close(true);
            }
        }
    }

    public class StrictMgaResolver : MgaResolver
    {
        private IMgaResolver delegate_;
        public StrictMgaResolver()
        {
            delegate_ = new MgaResolver();
        }

        public bool IsInteractive
        {
            get
            {
                return delegate_.IsInteractive;
            }

            set
            {
                delegate_.IsInteractive = value;
            }
        }

        public bool IsStickyEnabled
        {
            get
            {
                return delegate_.IsStickyEnabled;
            }

            set
            {
                delegate_.IsStickyEnabled = value;
            }
        }

        public void Clear()
        {
            delegate_.Clear();
        }

        public MgaMetaAttribute GetAttrByStrDisp(MgaFCO parent, string kindname)
        {
            return get_AttrByStr(parent, kindname);
        }

        public MgaMetaRole GetConnRoleByMetaDisp(MgaModel parent, MgaMetaAspect aspect, MgaFCO src, MgaFCO src_port, MgaFCO dst, MgaFCO dst_port)
        {
            return delegate_.GetConnRoleByMetaDisp(parent, aspect, src, src_port, dst, dst_port);
        }

        public MgaMetaFolder GetFolderByStrDisp(MgaFolder parent, string kindname)
        {
            return delegate_.GetFolderByStrDisp(parent, kindname);
        }

        public MgaMetaFCO GetKindByStrDisp(MgaFolder parent, string kindname, objtype_enum objtype)
        {
            return delegate_.GetKindByStrDisp(parent, kindname, objtype);
        }

        public MgaMetaRole GetRefRoleByMetaDisp(MgaModel parent, MgaMetaAspect aspect, MgaFCO src)
        {
            return delegate_.GetRefRoleByMetaDisp(parent, aspect, src);
        }

        public MgaMetaRole GetRoleByMetaDisp(MgaModel parent, MgaMetaFCO p_kind, objtype_enum objtype, MgaMetaRole p_role, MgaMetaAspect aspect)
        {
            return delegate_.GetRoleByMetaDisp(parent, p_kind, objtype, p_role, aspect);
        }

        public MgaMetaRole GetRoleByStrDisp(MgaModel parent, string kindname, objtype_enum objtype, string rolename, string aspect)
        {
            return delegate_.GetRoleByStrDisp(parent, kindname, objtype, rolename, aspect);
        }

        public MgaMetaRoles GetRolesByMetasDisp(MgaModel dst_parent, MgaFCOs src, MgaMetaAspect aspect, string sz_aspect)
        {
            return delegate_.GetRolesByMetasDisp(dst_parent, src, aspect, sz_aspect);
        }

        public void getUserOptions()
        {
            delegate_.getUserOptions();
        }

        public MgaMetaAttribute get_AttrByStr(MgaFCO parent, string kindname)
        {
            try
            {
                return delegate_.AttrByStr[parent, kindname];
            }
            catch (System.Runtime.InteropServices.COMException e)
            {
                // Additional information: Could not resolve attribute 'AttributeName'
                // n.b. GME ignore this (bogus)
                // throw new Exception(String.Format("Unknown attribute '{0}' for kind '{1}'", kindname, parent.Meta.Name));
                return new UnimplementedMgaMetaAttributes(e.Message);
            }
        }

        public MgaMetaRole get_ConnRoleByMeta(MgaModel parent, MgaMetaAspect aspect, MgaFCO src, MgaFCO src_port, MgaFCO dst, MgaFCO dst_port)
        {
            return delegate_.ConnRoleByMeta[parent, aspect, src, src_port, dst, dst_port];
        }

        public MgaMetaFolder get_FolderByStr(MgaFolder parent, string kindname)
        {
            return delegate_.FolderByStr[parent, kindname];
        }

        public MgaMetaFCO get_KindByStr(MgaFolder parent, string kindname, objtype_enum objtype)
        {
            return delegate_.KindByStr[parent, kindname, objtype];
        }

        public MgaMetaRole get_RefRoleByMeta(MgaModel parent, MgaMetaAspect aspect, MgaFCO src)
        {
            return delegate_.RefRoleByMeta[parent, aspect, src];
        }

        public MgaMetaRole get_RoleByMeta(MgaModel parent, MgaMetaFCO p_kind, objtype_enum objtype, MgaMetaRole p_role, MgaMetaAspect aspect)
        {
            return delegate_.RoleByMeta[parent, p_kind, objtype, p_role, aspect];
        }

        public MgaMetaRole get_RoleByStr(MgaModel parent, string kindname, objtype_enum objtype, string rolename, string aspect)
        {
            return delegate_.RoleByStr[parent, kindname, objtype, rolename, aspect];
        }

        public MgaMetaRoles get_RolesByMetas(MgaModel dst_parent, MgaFCOs src, MgaMetaAspect aspect, string sz_aspect)
        {
            return delegate_.RolesByMetas[dst_parent, src, aspect, sz_aspect];
        }
    }

    class UnimplementedMgaMetaAttributes : MgaMetaAttribute
    {
        private string message;

        public UnimplementedMgaMetaAttributes(string message)
        {
            this.message = message;
        }

        public MgaConstraints Constraints
        {
            get
            {
                throw new ApplicationException(message);
            }
        }

        public MgaMetaBase DefinedIn
        {
            get
            {
                throw new ApplicationException(message);
            }
        }

        public string DisplayedName
        {
            get
            {
                throw new ApplicationException(message);
            }

            set
            {
                throw new ApplicationException(message);
            }
        }

        public MgaMetaEnumItems EnumItems
        {
            get
            {
                throw new ApplicationException(message);
            }
        }

        public MgaMetaProject MetaProject
        {
            get
            {
                throw new ApplicationException(message);
            }
        }

        public int MetaRef
        {
            get
            {
                throw new ApplicationException(message);
            }

            set
            {
                throw new ApplicationException(message);
            }
        }

        public string Name
        {
            get
            {
                throw new ApplicationException(message);
            }

            set
            {
                throw new ApplicationException(message);
            }
        }

        public objtype_enum ObjType
        {
            get
            {
                throw new ApplicationException(message);
            }
        }

        public MgaMetaRegNodes RegistryNodes
        {
            get
            {
                throw new ApplicationException(message);
            }
        }

        public MgaMetaFCOs UsedIn
        {
            get
            {
                throw new ApplicationException(message);
            }
        }

        public attval_enum ValueType
        {
            get
            {
                throw new ApplicationException(message);
            }

            set
            {
                throw new ApplicationException(message);
            }
        }

        public bool Viewable
        {
            get
            {
                throw new ApplicationException(message);
            }

            set
            {
                throw new ApplicationException(message);
            }
        }

        public MgaMetaConstraint CreateConstraint()
        {
            throw new ApplicationException(message);
        }

        public MgaMetaEnumItem CreateEnumItem()
        {
            throw new ApplicationException(message);
        }

        public void Delete()
        {
            throw new ApplicationException(message);
        }

        public MgaMetaRegNode GetRegistryNodeDisp(string path)
        {
            throw new ApplicationException(message);
        }

        public string GetRegistryValueDisp(string path)
        {
            throw new ApplicationException(message);
        }

        public object get_DefaultValue()
        {
            throw new ApplicationException(message);
        }

        public MgaMetaRegNode get_RegistryNode(string path)
        {
            throw new ApplicationException(message);
        }

        public string get_RegistryValue(string path)
        {
            throw new ApplicationException(message);
        }

        public void SetRegistryValueDisp(string path, string p)
        {
            throw new ApplicationException(message);
        }

        public void set_DefaultValue(string p)
        {
            throw new ApplicationException(message);
        }

        public void set_RegistryValue(string path, string p)
        {
            throw new ApplicationException(message);
        }
    }

}