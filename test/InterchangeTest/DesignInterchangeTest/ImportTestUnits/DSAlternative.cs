using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using GME.MGA;
using Xunit;

namespace DesignImporterTests
{
    public class DSAlternativeFixture : DesignImporterTestFixtureBase
    {
        public override String pathXME
        {
            get { return "DSAlternative\\DSAlternative.xme"; }
        }
    }

    public class DSAlternative : DSRoundTripBase, IUseFixture<DSAlternativeFixture>
    {
        [Fact]
        public void WheelParameter()
        {
            string asmName = "WheelParameter";
            RunRoundTrip(asmName);
        }

        [Fact]
        public void WheelProperty()
        {
            string asmName = "WheelProperty";
            RunRoundTrip(asmName);
        }

        public override string AdmPath { get { return fixture.AdmPath; } }
        public override MgaProject proj { get { return fixture.proj; } }
        public override string FolderName { get { return "DesignSpaces"; } }
        DSAlternativeFixture fixture;
        public void SetFixture(DSAlternativeFixture data)
        {
            fixture = data;
        }
    }
}
