using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CyPhyCar
{
    class AdamsCarFile
    {
        class Block
        {
            public string Type;
            public string Content;
        }
        private List<Block> Blocks = new List<Block>();
        public AdamsCarFile(string content)
        {
            Parse(new StringReader(content));
        }
        public override string ToString()
        {
            StringBuilder b = new StringBuilder();
            foreach (var block in Blocks)
            {
                b.Append(block.Content);
            }
            return b.ToString();
        }
        private void Parse(TextReader reader)
        {
            string line;
            Block currentblock = null;
            List<string> prebuffered = new List<string>();
            while ((line = reader.ReadLine()) != null)
            {
                if (line.StartsWith("$"))
                {
                    prebuffered.Add(line);
                }
                else if (line.StartsWith("["))
                {
                    if (currentblock == null)
                    {
                        currentblock = new Block() { Type = line.Substring(1, line.Length - 2) };
                    }
                    else
                    {
                        Blocks.Add(currentblock);
                        currentblock = new Block() { Type = line.Substring(1, line.Length - 2) }; 
                    }
                    foreach (string s in prebuffered)
                    {
                        currentblock.Content += s + "\n";
                    }
                    prebuffered.Clear();
                    currentblock.Content += line+"\n";
                }
                else if (currentblock != null)
                {
                    foreach (string s in prebuffered)
                    {
                        currentblock.Content += s + "\n";
                    }
                    prebuffered.Clear();
                    currentblock.Content += line + "\n";
                }
            }
            Blocks.Add(currentblock);
        }
        private Dictionary<string, string> ParseKeyValuePairs(Block block)
        {
            Dictionary<string, string> result = new Dictionary<string, string>();
            StringReader reader = new StringReader(block.Content);
            string line;
            while ((line = reader.ReadLine()) != null)
            {
                if (line.Contains('='))
                {
                    var elements = line.Trim().Split('=');
                    result.Add(elements[0].Trim(), elements[1].Trim());
                }
            }
            return result;
        }
        public void Replace(AdamsCarFileReplacement replacement, string newvalue)
        {
            foreach (var block in Blocks)
            {
                if (block.Type == replacement.EntryType)
                {
                    bool success = false;
                    if (replacement.IdKeys != null)
                    {
                        Dictionary<string, string> dict = ParseKeyValuePairs(block);
                        if (!dict.ContainsKey(replacement.KeyToReplace)) continue;
                        success = true;
                        foreach (KeyValuePair<string, string> key in replacement.IdKeys)
                        {
                            if (dict.ContainsKey(key.Key))
                            {
                                if (!dict[key.Key].Contains(key.Value))
                                {
                                    success = false;
                                    break;
                                }
                            }
                            else
                            {
                                success = false;
                                break;
                            }
                        }
                    }
                    else if (replacement.IdStrings != null)
                    {
                        success = true;
                        foreach (string str in replacement.IdStrings)
                        {
                            if (!block.Content.Contains(str))
                            {
                                success = false;
                                break;
                            }
                        }
                    }
                    if (!success) continue;
                    StringReader reader = new StringReader(block.Content);
                    StringBuilder newstr = new StringBuilder();
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {
                        string newline = line;
                        if (line.Contains('='))
                        {
                            var elements = line.Trim().Split('=');
                            if (elements[0].Trim() == replacement.KeyToReplace)
                            {
                                newline = elements[0] + '=' + (replacement.FileRef?" 'mdids://isis_test/":" ") + newvalue.Replace("\\","/") + (replacement.FileRef?"'":"");
                            }
                        }
                        newstr.AppendLine(newline);
                    }
                    block.Content = newstr.ToString();
                }
            }
        }
    }
}
