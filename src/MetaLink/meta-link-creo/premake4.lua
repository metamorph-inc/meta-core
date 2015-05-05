
local third_party_dir = "../../../3rdParty"
local boost_dir = third_party_dir.."/boost-1_53_0"
local protobuf_dir = third_party_dir.."/protoc-2.5.0-win32"

solution "MetaLinkCreo"
   configurations { "Debug", "Release" }
   location "premake4"
 
   project "MetaLinkTest"
      kind "ConsoleApp"
      language "C++"
      files { "./src/meta-link-main.cc" }
      links { "MetaLinkLib", "libprotobuf" }
      libdirs { boost_dir.."/x64/lib", protobuf_dir.."/x64/lib" }
      includedirs { boost_dir, protobuf_dir }
      location "premake4"
 
      configuration "Debug"
         defines { "DEBUG", "_SCL_SECURE_NO_WARNINGS", "_WIN32_WINNT=0x0501" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG", "_SCL_SECURE_NO_WARNINGS", "_WIN32_WINNT=0x0501" }
         flags { "Optimize" }    

   project "MetaLinkLib"
      kind "StaticLib"
      language "C++"
      files { "./src/BridgeClient.cc",  "./src/BridgeClient.h", 
              "./src/FramedEdit.cc",    "./src/FramedEdit.h",
              "./src/gen/AssemblyInterface.pb.cc", "./src/gen/AssemblyInterface.pb.h", 
              "./src/gen/MetaLinkMsg.pb.cc", "./src/gen/MetaLinkMsg.pb.h", 
              "./src/gen/XsdTypes.pb.cc", "./src/gen/XsdTypes.pb.h" }
      includedirs { boost_dir, protobuf_dir }
      location "premake4"
 
      configuration "Debug"
         defines { "DEBUG", "_SCL_SECURE_NO_WARNINGS", "_WIN32_WINNT=0x0501" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG", "_SCL_SECURE_NO_WARNINGS", "_WIN32_WINNT=0x0501" }
         flags { "Optimize" }    

