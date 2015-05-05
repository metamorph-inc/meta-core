using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ISIS.Web;


namespace ISIS.VehicleForge
{
    public class VFJobExecutor : IJobExecutor
    {
        VehicleForge.VFWebClient vfAuthSession;

        public VFJobExecutor(VehicleForge.VFWebClient vfAuth)
        {
            this.vfAuthSession = vfAuth;
        }

        public IJob CreateJob(string dirPath, string jobTitle)
        {
            throw new NotImplementedException();
        }

        public bool Start(IJob job)
        {
            throw new NotImplementedException();
        }

        public string CheckStatus(IJob job)
        {
            throw new NotImplementedException();
        }

        public string GetResults(IJob job)
        {
            throw new NotImplementedException();
        }

        public bool Kill(IJob job)
        {
            throw new NotImplementedException();
        }


        public bool RemoveFromJobManager(IJob job)
        {
            throw new NotImplementedException();
        }
    }

}
