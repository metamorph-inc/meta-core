namespace JobManager.Jenkins.UserProfile
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    public class TrustInfo
    {
        public double score { get; set; }
        public double percentile { get; set; }
    }

    /// <summary>
    /// VF user profile response structure /rest/user/get_user_profile
    /// </summary>
    public class UserProfile
    {
        public string username { get; set; }
        public string interests { get; set; }
        public string profileImage { get; set; }
        public object skypeName { get; set; }
        public string mission { get; set; }
        public string expertise { get; set; }
        public string userSince { get; set; }
        public string fullName { get; set; }
        public TrustInfo trustInfo { get; set; }
        public List<string> projects { get; set; }
    }
}
