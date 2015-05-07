// -----------------------------------------------------------------------
// <copyright file="ElaboratorRunner.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace ElaboratorTest
{
    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Text;
    using GME.MGA;
    using Xunit;

    public static class MgaHelper
    {
        public static void CheckParadigmVersionUpgrade(MgaProject project)
        {
            GME.Util.MgaRegistrar registar = new GME.Util.MgaRegistrar();
            string MetaConnStr;
            object MetaGuid = null;
            object ProjectMetaGuid;
            project.BeginTransactionInNewTerr();
            try
            {
                registar.QueryParadigm(project.MetaName, out MetaConnStr, ref MetaGuid, GME.Util.regaccessmode_enum.REGACCESS_BOTH);
                ProjectMetaGuid = project.MetaGUID;
            }
            finally
            {
                project.AbortTransaction();
            }
            if (((Array)ProjectMetaGuid).Cast<byte>().SequenceEqual(((Array)MetaGuid).Cast<byte>()) == false)
            {
                Xunit.Assert.True(false, string.Format("Please upgrade {0} to the latest registered {1} paradigm", project.ProjectConnStr, project.MetaName));
            }
        }
    }

    /// <summary>
    /// TODO: Update summary.
    /// </summary>
    public class ElaboratorRunner
    {
        public static bool RunElaborator(string projectPath, string absPath)
        {
            bool result = false;
            Assert.True(File.Exists(projectPath), "Project file does not exist.");
            string ProjectConnStr = "MGA=" + Path.GetFullPath(projectPath);

            MgaProject project = new MgaProject();
            project.OpenEx(ProjectConnStr, "CyPhyML", null);
            try
            {
                var terr = project.BeginTransactionInNewTerr();
                var testObj = project.ObjectByPath[absPath] as MgaFCO;
                if (testObj == null)
                {
                    throw new ApplicationException(absPath + " not found in " + project.ProjectConnStr);
                }
                project.AbortTransaction();

                MgaFCOs fcos = (MgaFCOs)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaFCOs"));

                var elaborator = new CyPhyElaborateCS.CyPhyElaborateCSInterpreter();
                result = elaborator.RunInTransaction(project, testObj, fcos, 128);
            }
            finally
            {
                project.Close(true);
            }

            return result;
        }
    }
}
