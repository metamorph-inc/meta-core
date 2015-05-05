using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Xunit;

namespace ComponentAndArchitectureTeamTest
{
    public class TestMetaAvmProject
    {
        [Fact]
        public void MakeRelativePath_URIEscape()
        {
            Assert.Equal(@".\12{34}4 5%78&9\file", AVM.DDP.MetaAvmProject.MakeRelativePath(@"C:\test", @"C:\test\12{34}4 5%78&9\file"));
        }

        [Fact]
        public void MakeRelativePath_UpADir()
        {
            Assert.Equal(@"..\..\123", AVM.DDP.MetaAvmProject.MakeRelativePath("C:\\test\\dir\\", @"c:\123"));
        }
    }
}
