using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME;
using GME.Util;
using GME.MGA;
using GME.MGA.Meta;

namespace GME.CSharp
{
    class MgaGateway
    {
        public MgaGateway(IMgaProject project)
        {
            this.project = project;
        }

        public IMgaProject project = null;
        public IMgaTerritory territory = null;

        private bool projectWasInTransaction = false;

        #region TRANSACTION HANDLING
        public void BeginTransaction(transactiontype_enum mode = transactiontype_enum.TRANSACTION_GENERAL)
        {
            if (this.projectWasInTransaction == false)
            {
                project.BeginTransaction(territory, mode);
            }
        }

        public void CommitTransaction()
        {
            if ((project.ProjectStatus & 8) != 0 &&
                this.projectWasInTransaction == false)
            {
                project.CommitTransaction();
            }
        }

        public void AbortTransaction()
        {
            if ((project.ProjectStatus & 8) != 0 &&
                this.projectWasInTransaction == false)
            {
                project.AbortTransaction();
            }
        }

        public delegate void voidDelegate();
        public void PerformInTransaction(
            voidDelegate d,
            transactiontype_enum mode = transactiontype_enum.TRANSACTION_GENERAL)
        {
            this.projectWasInTransaction = (project.ProjectStatus & 8) != 0;

            this.BeginTransaction(mode);

            try
            {
                d();
                this.CommitTransaction();
            }
            catch (Exception)
            {
                // TODO: What should we do here?
               throw;
            }
        }
        #endregion
        #region UTILITIES
        public IMgaMetaBase GetMetaByName(string name)
        {
            try
            {
                return project.RootMeta.RootFolder.get_DefinedFCOByName(name, false) as MgaMetaFCO;
            }
#pragma warning disable 0168
            catch (System.Runtime.InteropServices.COMException e)
            {
                return project.RootMeta.RootFolder.get_DefinedFolderByName(name, false) as MgaMetaFolder;
            }
#pragma warning restore 0168
        }

        #endregion


    }
}
