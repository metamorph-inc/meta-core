using System;
using System.Collections.Generic;
using System.IO;
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

            var rfc3986_reserved = "!*'();:@&=+$,/?#[]";
            Action<string> testString = (testData) =>
            {
                foreach (var c in Path.GetInvalidFileNameChars())
                {
                    testData = testData.Replace("" + c, "");
                }
                Assert.Equal(".\\" + testData, AVM.DDP.MetaAvmProject.MakeRelativePath(@"C:\test", "C:\\test\\" + testData));
            };
            testString(rfc3986_reserved);

            var asciis = "";
            for (char i = (char)0x21; i < 128; i++)
            {
                asciis += i;
            }
            testString(asciis);

            var percents = "";
            for (char i = (char)0x21; i < 70; i++)
            {
                percents += "%" + i;
            }
            testString(percents);

            var percentsDoubled = "";
            for (char i = (char)0x21; i < 70; i++)
            {
                percentsDoubled += "%" + i + "0";
            }
            testString(percentsDoubled);
        }

        [Fact]
        public void MakeRelativePath_UpADir()
        {
            Assert.Equal(@"..\..\123", AVM.DDP.MetaAvmProject.MakeRelativePath("C:\\test\\dir\\", @"c:\123"));
        }
    }
}
