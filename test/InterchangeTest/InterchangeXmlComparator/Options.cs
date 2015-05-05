using CommandLine;
using CommandLine.Text;

namespace InterchangeXmlComparator
{
    public enum ComparerModes
    {
        Design,
        Component
    }

    public class Options
    {
        [Option('e', "exported", Required = true, HelpText = "Exported XML")]
        public string ExportedFile { get; set; }

        [Option('d', "desired", Required = true, HelpText = "Desired result.")]
        public string DesiredResultFile { get; set; }

        [Option('m', "mode", Required = true, HelpText = "Comparer mode: Design/Component")]
        public ComparerModes ComparerMode { get; set; }

        [ParserState]
        public IParserState LastParserState { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this, current => HelpText.DefaultParsingErrorsHandler(this, current));
        }

    }
}