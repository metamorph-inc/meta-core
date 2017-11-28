using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.Util;
using System.IO;

namespace GenerateInterpreterRegistration
{
    class Program
    {
        static string CommonPrefix(List<string> strings)
        {
            if (strings.Count == 0)
            {
                return "";
            }
            for (int i = strings.Select(s => s.Length).Min() - 1; i > 0; i--)
            {
                if (strings.All(x => x.Substring(0, i) == strings.ElementAt(0).Substring(0, i)))
                {
                    return strings.ElementAt(0).Substring(0, i);
                }
            }
            return "";
        }

        static void Main(string[] args)
        {
            // WARNING: doesn't work on disabled components

            IMgaRegistrar reg = (IMgaRegistrar)Activator.CreateInstance(Type.GetTypeFromProgID("Mga.MgaRegistrar"));
            Array components = (Array)(object)reg.GetAssociatedComponentsDisp("CyPhyML", GME.MGA.componenttype_enum.COMPONENTTYPE_ALL, regaccessmode_enum.REGACCESS_SYSTEM);
            List<string> progids = components.Cast<string>().ToList();
            var progidToDll = progids.ToDictionary(p => p, p => reg.get_LocalDllPath(p));
            foreach (var item in progidToDll.Where(kvp => kvp.Value.StartsWith(System.Environment.GetEnvironmentVariable("GME_ROOT"))
                || kvp.Value.Contains("ForgePort")
                || kvp.Value.StartsWith("GAC")).ToList())
            {
                Console.WriteLine("xxx " + item.Key);
                progidToDll.Remove(item.Key);
            }
            progids = progidToDll.Keys.ToList();
            progids.Sort();
            List<string> dlls = progidToDll.Values.ToList();

            //foreach (string dll in dlls)
            //    Console.WriteLine(dll);
            if (CommonPrefix(dlls).EndsWith("src/"))
            {
                Console.Error.WriteLine("Dlls are not in the same src dir");
                Environment.Exit(3);
            }
            //dlls.ForEach(Console.WriteLine);
            string meta_path = CommonPrefix(dlls);
            if (meta_path.EndsWith("src\\"))
                meta_path = meta_path.Substring(0, meta_path.Length - 4);
            Console.WriteLine(": META_PATH=" + meta_path);
            Console.WriteLine("Setlocal EnableDelayedExpansion");
            string exit_on_error = " || exit /b !ERRORLEVEL!";

            foreach (var systemOrUser in new string[] { "2", "1" })
            {
                Console.Write("bin\\Python27\\Scripts\\Python.exe -c \"");
                Console.Write(String.Format("reg = __import__('win32com.client').client.DispatchEx('Mga.MgaRegistrar'); [reg.UnregisterComponent(progid, {0}) for progid in reg.GetAssociatedComponentsDisp('CyPhyML', 7, {0})]", systemOrUser));
                Console.Write("\"");
                Console.WriteLine(exit_on_error);
            }

            Console.Write("bin\\Python27\\Scripts\\Python.exe -c \"");
            Console.Write(@"import sys; import os.path; filename=r'generated\CyPhyML\models\CyPhyML.mta'; not os.path.isfile(filename) and sys.exit(0); reg = __import__('win32com.client').client.DispatchEx('Mga.MgaRegistrar'); reg.RegisterParadigmFromDataDisp('MGA=' + os.path.abspath(filename), 1)");
            Console.Write("\"");
            Console.WriteLine(exit_on_error);

            Console.Write("reg add HKLM\\Software\\META /v META_PATH /t REG_SZ /d \"%~dp0\\\" /f /reg:64");
            Console.WriteLine(exit_on_error);


            Console.WriteLine("rem TODO: register more Decorators?");
            Console.WriteLine(@"set regsvr32=%windir%\System32\regsvr32.exe");
            Console.WriteLine(@"if %PROCESSOR_ARCHITECTURE% == x86 set regsvr32=%windir%\SysNative\regsvr32.exe");

            Console.WriteLine(@"set PATH=C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.6.2 Tools;C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.6.1 Tools;C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.6 Tools;c:\Program Files (x86)\Microsoft SDKs\Windows\v8.1A\bin\NETFX 4.5.1 Tools;C:\Program Files (x86)\Microsoft SDKs\Windows\v8.0A\bin\NETFX 4.0 Tools;%PATH%
gacutil.exe /nologo /u JobManagerLib
if exist ""src\JobManager\JobManagerLib\bin\Release\JobManagerLib.dll"" gacutil.exe /nologo /i ""src\JobManager\JobManagerLib\bin\Release\JobManagerLib.dll""  || exit /b !ERRORLEVEL!
gacutil.exe /l JobManagerLib");


            foreach (var dll in new string[]{
                "src\\bin\\CPMDecorator.dll",
                "src\\WorkflowDecorator\\bin\\Release\\WorkflowDecorator.dll"
            }.Select(d => Path.Combine(meta_path, d))            
            .Concat(progids.Select(progid => reg.get_LocalDllPath(progid))))
            {
                //Console.WriteLine(progid + " " + reg.get_LocalDllPath(progid));
                string relative_dll = dll.Substring(meta_path.Length);
                try
                {
                    System.Reflection.AssemblyName.GetAssemblyName(dll);
                    Console.Write("if exist " + "\"" + relative_dll + "\" ");
                    Console.Write(@"%windir%\Microsoft.NET\Framework64\v4.0.30319\RegAsm.exe /nologo /codebase " + "\"" + relative_dll + "\"");
                    Console.WriteLine(exit_on_error);
                }
                catch (BadImageFormatException)
                {
                    Console.Write("if exist " + "\"" + relative_dll + "\" ");
                    Console.Write("%regsvr32% /s \"" + relative_dll + "\"");
                    Console.WriteLine(exit_on_error);
                }
                catch (FileNotFoundException)
                {
                    Console.WriteLine(": doesnt exist " + dll);
                }
            }
 

            return;
            //get_LocalDllPath 
        }
    }
}
