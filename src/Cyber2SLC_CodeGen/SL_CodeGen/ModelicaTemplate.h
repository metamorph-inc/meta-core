#ifndef MODELICA_TEMPLATE_H
#define MODELICA_TEMPLATE_H

namespace Modelica_Template
{
	const std::string& get_h_tpl()
	{
		static std::string h_tpl;
		if (h_tpl.empty())
		{
			h_tpl += "// Wrapper files\n";
			h_tpl += "\n";
			h_tpl += "#pragma once\n";
			h_tpl += "\n";
			h_tpl += "#ifdef __cplusplus\n";
			h_tpl += "extern \"C\" {\n";
			h_tpl += "#endif\n";
			h_tpl += "\n";
			h_tpl += "#ifdef _MSC_VER\n";
			h_tpl += "#ifndef EXTERNAL_FUNCTION_EXPORT\n";
			h_tpl += "#	define EXTLIB2_EXPORT __declspec( dllexport )\n";
			h_tpl += "#else\n";
			h_tpl += "#	define EXTLIB2_EXPORT __declspec( dllimport )\n";
			h_tpl += "#endif\n";
			h_tpl += "#endif\n";
			h_tpl += "\n";
			h_tpl += "#include \"{{COMPONENT_NAME}}/{{TOPSUBSYTEM_NAME}}_sl.h\"\n";
			h_tpl += "\n";
			h_tpl += "EXTLIB2_EXPORT void * {{COMPONENT_NAME}}_wrapper_init({{SINGLE_VAR_TYPE}} {{SINGLE_VAR_NAME}}{{#INPUT_ARG}}, {{VAR_TYPE}} {{VAR_NAME}}{{/INPUT_ARG}});\n";
			h_tpl += "EXTLIB2_EXPORT void {{COMPONENT_NAME}}_wrapper_destroy( {{TOPSUBSYTEM_NAME}}_context * ptr );\n";
			h_tpl += "EXTLIB2_EXPORT void {{COMPONENT_NAME}}_wrapper_main({{TOPSUBSYTEM_NAME}}_context * context{{#INPUT_SIGNAL}},{{SIGNAL_TYPE}} {{SIGNAL_NAME}}{{/INPUT_SIGNAL}}{{#OUTPUT_SIGNAL}}, {{SIGNAL_TYPE}} *{{SIGNAL_NAME}}{{/OUTPUT_SIGNAL}} );\n";
			h_tpl += "\n";
			h_tpl += "#ifdef __cplusplus\n";
			h_tpl += "}\n";
			h_tpl += "#endif\n";
		}
		return h_tpl;
	}

	const std::string& get_c_tpl()
	{
		static std::string c_tpl;
		if(c_tpl.empty())
		{
			c_tpl += "#include \"{{COMPONENT_NAME}}_wrapper.h\"\n";
			c_tpl += "\n";
			c_tpl += "#include <stdlib.h>\n";
			c_tpl += "\n";
			c_tpl += "void * {{COMPONENT_NAME}}_wrapper_init({{SINGLE_VAR_TYPE}} {{SINGLE_VAR_NAME}}{{#INPUT_ARG}}, {{VAR_TYPE}} {{VAR_NAME}}{{/INPUT_ARG}})\n";
			c_tpl += "{\n";
			c_tpl += "\t{{TOPSUBSYTEM_NAME}}_context * context;\n";
			c_tpl += "\tcontext = ({{TOPSUBSYTEM_NAME}}_context *) calloc( 1, sizeof( {{TOPSUBSYTEM_NAME}}_context ) );\n";
			c_tpl += "\t{{TOPSUBSYTEM_NAME}}_init( context );  //Simulink\n";
	//		c_tpl += "\t{{TOPSUBSYTEM_NAME}}_paramInit( context{{#VARS}}, {{VAR_NAME}}{{/VARS}});  \n";
			c_tpl += "\t{{#PARAM_INIT}}\n";
			c_tpl += "\tcontext->{{PARAM_NAME}} = {{VAR_NAME}};\n";
			c_tpl += "\t{{/PARAM_INIT}}\n";
			c_tpl += "\treturn (void *) context;  \n";
			c_tpl += "}\n";
			c_tpl += "\n";
			c_tpl += "void {{COMPONENT_NAME}}_wrapper_destroy( {{TOPSUBSYTEM_NAME}}_context * ptr )\n";
			c_tpl += "{\n";
			c_tpl += "\tif ( ptr != NULL )\n";
			c_tpl += "\t\tfree( ({{TOPSUBSYTEM_NAME}}_context *)ptr );\n";
			c_tpl += "}\n";
			c_tpl += "\n";
			c_tpl += "void {{COMPONENT_NAME}}_wrapper_main( {{TOPSUBSYTEM_NAME}}_context * context{{#INPUT_SIGNAL}},{{SIGNAL_TYPE}} {{SIGNAL_NAME}}{{/INPUT_SIGNAL}}{{#OUTPUT_SIGNAL}}, {{SIGNAL_TYPE}} *{{SIGNAL_NAME}}{{/OUTPUT_SIGNAL}})\n";
			c_tpl += "{\n";
			c_tpl += "\t{{TOPSUBSYTEM_NAME}}_context * tcontext = ({{TOPSUBSYTEM_NAME}}_context *) context;\n";
			c_tpl += "\t{{TOPSUBSYTEM_NAME}}_main( tcontext{{#INPUT_SIGNAL}},{{SIGNAL_NAME}}{{/INPUT_SIGNAL}}{{#OUTPUT_SIGNAL}},{{SIGNAL_NAME}}{{/OUTPUT_SIGNAL}});\n";
			c_tpl += "}\n";
		}
		return c_tpl;
	}

	const std::string& get_vcxproj_tpl()
	{
		static std::string vcxproj_tpl;
		if(vcxproj_tpl.empty())
		{
			vcxproj_tpl += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
			vcxproj_tpl += "<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
			vcxproj_tpl += "<ItemGroup Label=\"ProjectConfigurations\">\n";
			vcxproj_tpl += "<ProjectConfiguration Include=\"Debug|Win32\">\n";
			vcxproj_tpl += "  <Configuration>Debug</Configuration>\n";
			vcxproj_tpl += "  <Platform>Win32</Platform>\n";
			vcxproj_tpl += " </ProjectConfiguration>\n";
			vcxproj_tpl += "<ProjectConfiguration Include=\"Release|Win32\">\n";
			vcxproj_tpl += " <Configuration>Release</Configuration>\n";
			vcxproj_tpl += " <Platform>Win32</Platform>\n";
			vcxproj_tpl += " </ProjectConfiguration>\n";
			vcxproj_tpl += " </ItemGroup>\n";
			vcxproj_tpl += " <PropertyGroup Label=\"Globals\">\n";
			vcxproj_tpl += "  <ProjectGuid>{{PROJECT_ID}}</ProjectGuid>\n";
			vcxproj_tpl += " <TargetFrameworkVersion>v4.0</TargetFrameworkVersion>\n";
			vcxproj_tpl += " <Keyword>ManagedCProj</Keyword>\n";
			vcxproj_tpl += " <RootNamespace>{{COMPONENT_NAME}}</RootNamespace>\n";
			vcxproj_tpl += " </PropertyGroup>\n";
			vcxproj_tpl += "<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n";
			vcxproj_tpl += " <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">\n";
			vcxproj_tpl += " <ConfigurationType>StaticLibrary</ConfigurationType>\n";
			vcxproj_tpl += "<UseDebugLibraries>true</UseDebugLibraries>\n";
			vcxproj_tpl += " <CLRSupport>false</CLRSupport>\n";
			vcxproj_tpl += "<CharacterSet>Unicode</CharacterSet>\n";
			vcxproj_tpl += "</PropertyGroup>\n";
			vcxproj_tpl += "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">\n";
			vcxproj_tpl += "<ConfigurationType>StaticLibrary</ConfigurationType>\n";
			vcxproj_tpl += " <UseDebugLibraries>false</UseDebugLibraries>\n";
			vcxproj_tpl += " <CLRSupport>false</CLRSupport>\n";
			vcxproj_tpl += " <CharacterSet>Unicode</CharacterSet>\n";
			vcxproj_tpl += "</PropertyGroup>\n";
			vcxproj_tpl += " <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n";
			vcxproj_tpl += " <ImportGroup Label=\"ExtensionSettings\">\n";
			vcxproj_tpl += " </ImportGroup>\n";
			vcxproj_tpl += " <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\n";
			vcxproj_tpl += "  <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n";
			vcxproj_tpl += "</ImportGroup>\n";
			vcxproj_tpl += "<ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\n";
			vcxproj_tpl += "  <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\n";
			vcxproj_tpl += "</ImportGroup>\n";
			vcxproj_tpl += "<PropertyGroup Label=\"UserMacros\" />\n";
			vcxproj_tpl += "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\n";
			vcxproj_tpl += "  <LinkIncremental>true</LinkIncremental>\n";
			vcxproj_tpl += "  <OutDir>$(SolutionDir)$(Configuration)\\$(ProjectName)</OutDir>\n";
			vcxproj_tpl += "</PropertyGroup>\n";
			vcxproj_tpl += "<PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\n";
			vcxproj_tpl += " <LinkIncremental>false</LinkIncremental>\n";
			vcxproj_tpl += "  <OutDir>$(SolutionDir)$(Configuration)\\$(ProjectName)</OutDir>\n";
			vcxproj_tpl += "</PropertyGroup>\n";
			vcxproj_tpl += "<ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\n";
			vcxproj_tpl += "  <ClCompile>\n";
			vcxproj_tpl += "    <WarningLevel>Level3</WarningLevel>\n";
			vcxproj_tpl += "   <Optimization>Disabled</Optimization>\n";
			vcxproj_tpl += "  <PreprocessorDefinitions>WIN32;_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
			vcxproj_tpl += "  <AdditionalIncludeDirectories>{{INCLUDE_PATH}}</AdditionalIncludeDirectories>\n";
			vcxproj_tpl += " </ClCompile>\n";
			vcxproj_tpl += "<Link>\n";
			vcxproj_tpl += "  <GenerateDebugInformation>true</GenerateDebugInformation>\n";
			vcxproj_tpl += "</Link>\n";
			vcxproj_tpl += " </ItemDefinitionGroup>\n";
			vcxproj_tpl += " <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\n";
			vcxproj_tpl += "  <ClCompile>\n";
			vcxproj_tpl += "   <WarningLevel>Level3</WarningLevel>\n";
			vcxproj_tpl += "   <PreprocessorDefinitions>WIN32;NDEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
			vcxproj_tpl += "   <AdditionalIncludeDirectories>{{INCLUDE_PATH}}</AdditionalIncludeDirectories>\n";
			vcxproj_tpl += " </ClCompile>\n";
			vcxproj_tpl += "<Link>\n";
			vcxproj_tpl += "  <GenerateDebugInformation>true</GenerateDebugInformation>\n";
			vcxproj_tpl += "</Link>\n";
			vcxproj_tpl += "</ItemDefinitionGroup>\n";
			vcxproj_tpl += "<ItemGroup>\n";
			vcxproj_tpl += " {{#INCLUDE}}\n";
			vcxproj_tpl += " <ClInclude Include=\"{{INCLUDE_H}}\" />\n";
			vcxproj_tpl += " {{/INCLUDE}}\n";
			vcxproj_tpl += "</ItemGroup>\n";
			vcxproj_tpl += " <ItemGroup>\n";
			vcxproj_tpl += "  {{#COMPILE}}\n";
			vcxproj_tpl += "  <ClCompile Include=\"{{COMPILE_C}}\" />\n";
			vcxproj_tpl += "  {{/COMPILE}}\n";
			vcxproj_tpl += "</ItemGroup>\n";
			vcxproj_tpl += "<Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\n";
			vcxproj_tpl += "<ImportGroup Label=\"ExtensionTargets\">\n";
			vcxproj_tpl += "</ImportGroup>\n";
			vcxproj_tpl += "</Project>\n";
		}
		return vcxproj_tpl;
	}

	const std::string& get_sln_tpl()
	{
		static std::string sln_tpl;
		if(sln_tpl.empty())
		{
			sln_tpl += "Microsoft Visual Studio Solution File, Format Version 11.00\n";
			sln_tpl += "# Visual Studio 2010\n";
			sln_tpl += "Project(\"{{SOLUTION_ID}}\") = \"{{COMPONENT_NAME}}\", \"{{COMPONENT_NAME}}.vcxproj\", \"{{PROJECT_ID}}\"\n";
			sln_tpl += "EndProject\n";
			sln_tpl += "Global\n";
			sln_tpl += "	GlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
			sln_tpl += "		Debug|Win32 = Debug|Win32\n";
			sln_tpl += "		Release|Win32 = Release|Win32\n";
			sln_tpl += "	EndGlobalSection\n";
			sln_tpl += "	GlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
			sln_tpl += "		{{PROJECT_ID}}.Debug|Win32.ActiveCfg = Debug|Win32\n";
			sln_tpl += "		{{PROJECT_ID}}.Debug|Win32.Build.0 = Debug|Win32\n";
			sln_tpl += "		{{PROJECT_ID}}.Release|Win32.ActiveCfg = Release|Win32\n";
			sln_tpl += "		{{PROJECT_ID}}.Release|Win32.Build.0 = Release|Win32\n";
			sln_tpl += "	EndGlobalSection\n";
			sln_tpl += "	GlobalSection(SolutionProperties) = preSolution\n";
			sln_tpl += "		HideSolutionNode = FALSE\n";
			sln_tpl += "	EndGlobalSection\n";
			sln_tpl += "EndGlobal\n";
		}
		return sln_tpl;
	}

	const std::string& get_filter_tpl()
	{
		static std::string filter_tpl;
		if(filter_tpl.empty())
		{
			filter_tpl += "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
			filter_tpl += "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n";
			filter_tpl += "  <ItemGroup>\n";
			filter_tpl += "    <Filter Include=\"Source Files\">\n";
			filter_tpl += "      <UniqueIdentifier>{{SOURCE_ID}}</UniqueIdentifier>\n";
			filter_tpl += "      <Extensions>cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx</Extensions>\n";
			filter_tpl += "    </Filter>\n";
			filter_tpl += "    <Filter Include=\"Header Files\">\n";
			filter_tpl += "      <UniqueIdentifier>{{HEADER_ID}}</UniqueIdentifier>\n";
			filter_tpl += "      <Extensions>h;hpp;hxx;hm;inl;inc;xsd</Extensions>\n";
			filter_tpl += "    </Filter>\n";
			filter_tpl += "  </ItemGroup>\n";
			filter_tpl += "  <ItemGroup>\n";
			filter_tpl += "    {{#INCLUDE_H}}\n";
			filter_tpl += "    <ClInclude Include=\"{{INCLUDE_H}}\" >\n";
			filter_tpl += "      <Filter>Header Files</Filter>\n";
			filter_tpl += "    </ClInclude>\n";
			filter_tpl += "    {{/INCLUDE_H}}\n";
			filter_tpl += "  </ItemGroup>\n";
			filter_tpl += "  <ItemGroup>\n";
			filter_tpl += "    {{#COMPILE}}\n";
			filter_tpl += "    <ClCompile Include=\"{{COMPILE_C}}\">\n";
			filter_tpl += "      <Filter>Source Files</Filter>\n";
			filter_tpl += "    </ClCompile>\n";
			filter_tpl += "    {{/COMPILE}}\n";
			filter_tpl += "  </ItemGroup>\n";
			filter_tpl += "</Project>\n";
		}
		return filter_tpl;
	}

	const std::string& get_mo_tpl()
	{
		static std::string mo_tpl;
		if(mo_tpl.empty())
		{
			mo_tpl += "within CyPhy.Simulink;\n";
			mo_tpl += "class {{COMPONENT_NAME}}_wrapper\n";
			mo_tpl += "  extends ExternalObject;\n";
			mo_tpl += "  function constructor\n";
			mo_tpl += "    output {{COMPONENT_NAME}}_wrapper context;\n";
			mo_tpl += "{{#PARAMETER}}\n";
			mo_tpl += "	   input {{PARAMETER_TYPE}} {{PARAMETER_NAME}};\n";
			mo_tpl += "{{/PARAMETER}}\n";
			mo_tpl += "    external \"C\" context = {{COMPONENT_NAME}}_wrapper_init({{SINGLE_VAR}}{{#VARS}}, {{VAR_NAME}}{{/VARS}});\n";
			mo_tpl += "  end constructor;\n";
			mo_tpl += "\n";
			mo_tpl += "  function destructor\n";
			mo_tpl += "    input {{COMPONENT_NAME}}_wrapper context;\n";
			mo_tpl += "    external \"C\" {{COMPONENT_NAME}}_wrapper_destroy( context );\n";
			mo_tpl += "  end destructor;\n";
			mo_tpl += "end {{COMPONENT_NAME}}_wrapper;\n";
			mo_tpl += "\n";
			mo_tpl += "function {{COMPONENT_NAME}}_wrapper_main\n";
			mo_tpl += "    \"{{COMPONENT_NAME}} code external function wrapper.\"\n";
			mo_tpl += "  input {{COMPONENT_NAME}}_wrapper context;\n";
			mo_tpl += "  {{#INPUT_SIGNAL}}\n";
			mo_tpl += "  public\n";
			//mo_tpl += "  \tinput {{DATA_TYPE}} {{DATA_NAME}};\n\n";
			mo_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Input {{DATA_NAME}};\n\n";
			mo_tpl += "  {{/INPUT_SIGNAL}}\n";
			mo_tpl += "  {{#OUTPUT_SIGNAL}}";
			mo_tpl += "  public\n";
			//mo_tpl += "  \toutput {{DATA_TYPE}} {{DATA_NAME}};\n";
			mo_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Output {{DATA_NAME}};\n\n";
			mo_tpl += "  {{/OUTPUT_SIGNAL}}\n";
			mo_tpl += "  external \"C\" {{COMPONENT_NAME}}_wrapper_main(context{{#INPUT_SIGNAL}},{{DATA_NAME}}{{/INPUT_SIGNAL}}{{#OUTPUT_SIGNAL}},{{DATA_NAME}}{{/OUTPUT_SIGNAL}});\n";
			mo_tpl += "  annotation(Include=\"#include <Simulink/{{COMPONENT_NAME}}_wrapper.h>\", Library=\"Simulink\\{{COMPONENT_NAME}}\\Release\\{{COMPONENT_NAME}}\\{{COMPONENT_NAME}}\");\n";
			mo_tpl += "end {{COMPONENT_NAME}}_wrapper_main;\n";
			mo_tpl += "\n";
			mo_tpl += "// This part has the interface elements instantiated from the CyPhyML component model.\n";
			mo_tpl += "model {{COMPONENT_NAME}}_type\n";
			//mo_tpl += "     extends ISIS.Icons.Simulink;\n";
			mo_tpl += "\n";  
			mo_tpl += " {{COMPONENT_NAME}}_wrapper tcontext = {{COMPONENT_NAME}}_wrapper({{SINGLE_VAR}}{{#VARS}}, {{VAR_NAME}}{{/VARS}});\n";
			mo_tpl += "\n"; 
			mo_tpl += " {{#BUSPORT_DEF}}\n";
			mo_tpl += "  public\n";
			mo_tpl += " {{PORT_TYPE}} {{PORT_NAME}};\n";
			mo_tpl += " {{/BUSPORT_DEF}}\n";
			mo_tpl += "  {{#INPUT_SIGNAL_DEF}}\n";
			mo_tpl += "  public\n";
			mo_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Input {{DATA_NAME}};\n\n";
			mo_tpl += "  {{/INPUT_SIGNAL_DEF}}\n";
			mo_tpl += "  {{#OUTPUT_SIGNAL_DEF}}";
			mo_tpl += "  public\n";
			mo_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Output {{DATA_NAME}};\n\n";
			mo_tpl += "  {{/OUTPUT_SIGNAL_DEF}}\n";
			mo_tpl += "{{#PARAMETER}}\n";
			mo_tpl += "	   parameter {{PARAMETER_TYPE}} {{PARAMETER_NAME}};\n";
			mo_tpl += "{{/PARAMETER}}\n";
			mo_tpl += "  output Boolean sampleTrigger;\n";
			mo_tpl += "\n";
			mo_tpl += "equation\n";
			mo_tpl += "  sampleTrigger = sample( 0, {{SAMPLE_PERIOD_NAME}} );\n";
			mo_tpl += "  when sampleTrigger then\n";
			mo_tpl += "		{{#LEFT}}({{/LEFT}}{{FIRST_DATA_NAME}}{{#OUTPUT_SIGNAL_A}},{{DATA_NAME}}{{/OUTPUT_SIGNAL_A}}{{#RIGHT}}){{/RIGHT}} = {{COMPONENT_NAME}}_wrapper_main( tcontext{{#INPUT_ARG}},{{DATA_NAME}}{{/INPUT_ARG}});\n";
			//mo_tpl += "	{{#LEFT}}({{/LEFT}}{{FIRST_DATA_NAME}}.{{FIRST_PORT_NAME}}{{#OUTPUT_PORT}},{{PORT_NAME}}.{{OUT_PORT}}{{/OUTPUT_PORT}}{{#RIGHT}}){{/RIGHT}} := {{COMPONENT_NAME}}_wrapper_main( tcontext{{#INPUT_PORT}},{{PORT_NAME}}.{{IN_PORT}}{{/INPUT_PORT}});\n";
			mo_tpl += "end when;\n";
			mo_tpl += "\n";
			mo_tpl += "  annotation (uses(Modelica(version=\"3.2\")));\n";
			mo_tpl += "end {{COMPONENT_NAME}}_type;\n";
		}
		return mo_tpl;
	}

//-----------------------------------------------------------------------------------------------------------

	const std::string& get_mo_wrapper_tpl()
	{
		static std::string mo_wrapper_tpl;
		if(mo_wrapper_tpl.empty())
		{
			mo_wrapper_tpl += "within CyPhy.Simulink;\n";
			mo_wrapper_tpl += "class {{COMPONENT_NAME}}_wrapper\n";
			mo_wrapper_tpl += "  extends ExternalObject;\n";
			mo_wrapper_tpl += "  function constructor\n";
			mo_wrapper_tpl += "    output {{COMPONENT_NAME}}_wrapper context;\n";
			mo_wrapper_tpl += "{{#PARAMETER}}\n";
			mo_wrapper_tpl += "	   input {{PARAMETER_TYPE}} {{PARAMETER_NAME}};\n";
			mo_wrapper_tpl += "{{/PARAMETER}}\n";
			mo_wrapper_tpl += "    external \"C\" context = {{COMPONENT_NAME}}_wrapper_init({{SINGLE_VAR}}{{#VARS}}, {{VAR_NAME}}{{/VARS}});\n";
			mo_wrapper_tpl += "  end constructor;\n";
			mo_wrapper_tpl += "\n";
			mo_wrapper_tpl += "  function destructor\n";
			mo_wrapper_tpl += "    input {{COMPONENT_NAME}}_wrapper context;\n";
			mo_wrapper_tpl += "    external \"C\" {{COMPONENT_NAME}}_wrapper_destroy( context );\n";
			mo_wrapper_tpl += "  end destructor;\n";
			mo_wrapper_tpl += "end {{COMPONENT_NAME}}_wrapper;\n";
		}
		return mo_wrapper_tpl;
	}


	const std::string& get_mo_wrapper_main_tpl()
	{
		static std::string mo_wrapper_main_tpl;
		if(mo_wrapper_main_tpl.empty())
		{
			mo_wrapper_main_tpl += "within CyPhy.Simulink;\n";
			mo_wrapper_main_tpl += "function {{COMPONENT_NAME}}_wrapper_main\n";
			mo_wrapper_main_tpl += "    \"{{COMPONENT_NAME}} code external function wrapper.\"\n";
			mo_wrapper_main_tpl += "  input {{COMPONENT_NAME}}_wrapper context;\n";
			mo_wrapper_main_tpl += "  {{#INPUT_SIGNAL}}\n";
			mo_wrapper_main_tpl += "  public\n";
			//mo_wrapper_main_tpl += "  \tinput {{DATA_TYPE}} {{DATA_NAME}};\n\n";
			mo_wrapper_main_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Input {{DATA_NAME}};\n\n";
			mo_wrapper_main_tpl += "  {{/INPUT_SIGNAL}}\n";
			mo_wrapper_main_tpl += "  {{#OUTPUT_SIGNAL}}";
			mo_wrapper_main_tpl += "  public\n";
			//mo_wrapper_main_tpl += "  \toutput {{DATA_TYPE}} {{DATA_NAME}};\n";
			mo_wrapper_main_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Output {{DATA_NAME}};\n\n";
			mo_wrapper_main_tpl += "  {{/OUTPUT_SIGNAL}}\n";
			mo_wrapper_main_tpl += "  external \"C\" {{COMPONENT_NAME}}_wrapper_main(context{{#INPUT_SIGNAL}},{{DATA_NAME}}{{/INPUT_SIGNAL}}{{#OUTPUT_SIGNAL}},{{DATA_NAME}}{{/OUTPUT_SIGNAL}});\n";
			mo_wrapper_main_tpl += "  annotation(Include=\"#include <Simulink/{{COMPONENT_NAME}}_wrapper.h>\", Library=\"Simulink\\{{COMPONENT_NAME}}\\Release\\{{COMPONENT_NAME}}\\{{COMPONENT_NAME}}\");\n";
			mo_wrapper_main_tpl += "end {{COMPONENT_NAME}}_wrapper_main;\n";
		}
		return mo_wrapper_main_tpl;
	}


	const std::string& get_mo_type_tpl()
	{
		static std::string mo_type_tpl;
		if(mo_type_tpl.empty())
		{
			mo_type_tpl += "within CyPhy.Simulink;\n";
			mo_type_tpl += "model {{COMPONENT_NAME}}_type\n";
			mo_type_tpl += "// This part has the interface elements instantiated from the CyPhyML component model.\n";
			mo_type_tpl += "\n";  
			mo_type_tpl += " {{COMPONENT_NAME}}_wrapper tcontext = {{COMPONENT_NAME}}_wrapper({{SINGLE_VAR}}{{#VARS}}, {{VAR_NAME}}{{/VARS}});\n";
			mo_type_tpl += "\n"; 
			mo_type_tpl += " {{#BUSPORT_DEF}}\n";
			mo_type_tpl += "  public\n";
			mo_type_tpl += " {{PORT_TYPE}} {{PORT_NAME}};\n";
			mo_type_tpl += " {{/BUSPORT_DEF}}\n";
			mo_type_tpl += "  {{#INPUT_SIGNAL_DEF}}\n";
			mo_type_tpl += "  public\n";
			mo_type_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Input {{DATA_NAME}};\n\n";
			mo_type_tpl += "  {{/INPUT_SIGNAL_DEF}}\n";
			mo_type_tpl += "  {{#OUTPUT_SIGNAL_DEF}}";
			mo_type_tpl += "  public\n";
			mo_type_tpl += "	\tModelica.Blocks.Interfaces.{{DATA_TYPE}}Output {{DATA_NAME}};\n\n";
			mo_type_tpl += "  {{/OUTPUT_SIGNAL_DEF}}\n";
			mo_type_tpl += "{{#PARAMETER}}\n";
			mo_type_tpl += "	   parameter {{PARAMETER_TYPE}} {{PARAMETER_NAME}};\n";
			mo_type_tpl += "{{/PARAMETER}}\n";
			mo_type_tpl += "  output Boolean sampleTrigger;\n";
			mo_type_tpl += "\n";
			mo_type_tpl += "equation\n";
			mo_type_tpl += "  sampleTrigger = sample( 0, {{SAMPLE_PERIOD_NAME}} );\n";
			mo_type_tpl += "  when sampleTrigger then\n";
			mo_type_tpl += "		{{#LEFT}}({{/LEFT}}{{FIRST_DATA_NAME}}{{#OUTPUT_SIGNAL_A}},{{DATA_NAME}}{{/OUTPUT_SIGNAL_A}}{{#RIGHT}}){{/RIGHT}} = {{COMPONENT_NAME}}_wrapper_main( tcontext{{#INPUT_ARG}},{{DATA_NAME}}{{/INPUT_ARG}});\n";
			//mo_type_tpl += "	{{#LEFT}}({{/LEFT}}{{FIRST_DATA_NAME}}.{{FIRST_PORT_NAME}}{{#OUTPUT_PORT}},{{PORT_NAME}}.{{OUT_PORT}}{{/OUTPUT_PORT}}{{#RIGHT}}){{/RIGHT}} := {{COMPONENT_NAME}}_wrapper_main( tcontext{{#INPUT_PORT}},{{PORT_NAME}}.{{IN_PORT}}{{/INPUT_PORT}});\n";
			mo_type_tpl += "end when;\n";
			mo_type_tpl += "\n";
			mo_type_tpl += "  annotation (uses(Modelica(version=\"3.2\")));\n";
			mo_type_tpl += "end {{COMPONENT_NAME}}_type;\n";
		}
		return mo_type_tpl;
	}

//-----------------------------------------------------------------------------------------------------------	

	const std::string& get_pkg_tpl()
	{
		static std::string pkg_tpl;
		if(pkg_tpl.empty())
		{
			pkg_tpl += "within CyPhy;\n";
			pkg_tpl += "package Simulink\n"; 
			pkg_tpl += "    extends Modelica.Icons.Package;\n";
			pkg_tpl += "\n";
			pkg_tpl += " // This part has the interface elements instantiated from the CyPhyML component model.\n";
			pkg_tpl += "\n";
			pkg_tpl += "annotation(uses(Modelica(version = \"3.2\")), version = \"2\", conversion(noneFromVersion = \"\", noneFromVersion = \"1\"));\n";
			pkg_tpl += "end Simulink;\n";
		}
		return pkg_tpl;
	}

};


#endif
