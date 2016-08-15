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
        private bool projectHasBeenAborted = false;
        #region TRANSACTION HANDLING
        public void BeginTransaction(transactiontype_enum mode = transactiontype_enum.TRANSACTION_GENERAL)
        {
            project.BeginTransaction(territory, mode);
        }

        public void CommitTransaction()
        {
            project.CommitTransaction();
        }

        public void AbortTransaction()
        {
            project.AbortTransaction();
        }

        public delegate void voidDelegate();
        public void PerformInTransaction(
            voidDelegate d,
            transactiontype_enum mode = transactiontype_enum.TRANSACTION_GENERAL,
            bool abort = true)
        {
            this.projectWasInTransaction = (project.ProjectStatus & 8) != 0;
            this.projectHasBeenAborted = false;

            if (this.projectWasInTransaction == false)
            {
                BeginTransaction(mode);
            }

            if (this.projectWasInTransaction && abort)
            {
                CommitTransaction();
                BeginTransaction(mode);
            }

            try
            {
                d();
                if (abort)
                {
                    AbortTransaction();
                    projectHasBeenAborted = true;
                    if (this.projectWasInTransaction)
                    {
                        BeginTransaction(mode);
                    }
                }
                else
                {
                    if (this.projectWasInTransaction == false)
                    {
                        CommitTransaction();
                    }
                }
            }
            catch
            {
                if (projectHasBeenAborted == false)
                {
                    AbortTransaction();
                }

                if (this.projectWasInTransaction)
                {
                    BeginTransaction(mode);
                }

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