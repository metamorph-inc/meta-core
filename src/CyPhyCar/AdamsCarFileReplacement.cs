using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhyCar
{
    class AdamsCarFileReplacement
    {
        public string EntryType;
        public Dictionary<string, string> IdKeys;
        public string[] IdStrings;
        public string KeyToReplace;
        public bool FileRef = true;
        public static AdamsCarFileReplacement FromString(string input){
            AdamsCarFileReplacement result = new AdamsCarFileReplacement();
            string[] elems = input.Split('/');
            result.EntryType = elems[0];
            if (elems[1].Length > 0)
            {
                result.IdKeys = new Dictionary<string, string>();
                string[] elems2 = elems[1].Split('=');
                result.IdKeys.Add(elems2[0], elems2[1]);
            }
            result.KeyToReplace = elems[2];
            result.FileRef = false;
            return result;
        }
    }
}
