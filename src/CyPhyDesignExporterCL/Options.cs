using CommandLine;
using CommandLine.Text;

namespace CyPhyDesignExporterCL
{
    public class Options
    {
        [Option('m', "mga", Required = true, HelpText = "MGA file")]
        public string MgaFile { get; set; }

        [Option('d', "design", Required = false, HelpText = "Name of the design (ComponentAssembly or DesignContainer) to be exported.")]
        public string DesignName { get; set; }

        [ParserState]
        public IParserState LastParserState { get; set; }

        [HelpOption]
        public string GetUsage()
        {
            return HelpText.AutoBuild(this, current => HelpText.DefaultParsingErrorsHandler(this, current));
        }

    }
}