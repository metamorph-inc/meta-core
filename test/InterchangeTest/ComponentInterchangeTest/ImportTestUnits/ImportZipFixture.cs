using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Xunit;

namespace ComponentImporterUnitTests
{
    public class ImportZipFixture
    {
        public ImportZipFixture()
        {
            // First, copy BlankInputModel/InputModel.xme into the test folder
            File.Delete(ImportZip.mgaPath);
            GME.MGA.MgaUtils.ImportXME(Common.blankInputModelPath, ImportZip.mgaPath);
            Assert.True(File.Exists(ImportZip.mgaPath), "Input model not found; import may have failed.");
        }
    }
}
