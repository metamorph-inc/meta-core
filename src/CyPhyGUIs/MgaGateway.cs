using GME.MGA;
using GME.MGA.Meta;
using System.Runtime.InteropServices;

namespace GME.CSharp
{
    public class MgaGateway
    {
        public MgaGateway(IMgaProject project)
        {
            this.project = project;
        }

        public IMgaProject project = null;

        private bool projectWasInTransaction = false;
        private bool projectHasBeenAborted = false;

        public void BeginTransaction(transactiontype_enum mode = transactiontype_enum.TRANSACTION_GENERAL)
        {
            var territory = project.BeginTransactionInNewTerr(mode);
            Marshal.FinalReleaseComObject(territory);
        }

        public void CommitTransaction()
        {
            if ((project.ProjectStatus & 8) != 0)
            {
                project.CommitTransaction();
            }
        }

        public void AbortTransaction()
        {
            if ((project.ProjectStatus & 8) != 0)
            {
                project.AbortTransaction();
            }
        }

        public delegate void voidDelegate();
        public void PerformInTransaction(
            voidDelegate d,
            transactiontype_enum mode = transactiontype_enum.TRANSACTION_NON_NESTED,
            bool abort = true)
        {
            this.projectWasInTransaction = (project.ProjectStatus & 8) != 0;
            this.projectHasBeenAborted = false;

            if (this.projectWasInTransaction == false)
            {
                BeginTransaction(mode);
            }

            try
            {
                d();
                if (this.projectWasInTransaction == false && abort)
                {
                    projectHasBeenAborted = true;
                    AbortTransaction();
                }
                else if (this.projectWasInTransaction == false)
                {
                    CommitTransaction();
                }
            }
            catch
            {
                if (this.projectWasInTransaction == false && projectHasBeenAborted == false)
                {
                    AbortTransaction();
                }

                throw;
            }
        }


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


    }
}
