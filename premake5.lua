
-- premake5.lua
workspace "EmvParser"
   configurations { "Debug", "Release" }
   platforms { "Win32", "Win64", "Linux" }

   include "thirdparty/EMV-tag-parser/projectscript.lua"

	project "EmvParser"
	   kind "WindowedApp"
	   language "C++"
	   targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
	   objdir "obj"
	   
	   dependson {"EmvTagParserLib","wxWidgets"}
	   
	   includedirs({
			"thirdparty/EMV-tag-parser/include"
	   })

	   files ({
			"include/**.h*",
			"src/**.c*",
			"*.rc",
			"*.lua"
		})
		
		defines { "wxCFG=_st" }

		libdirs({
			"thirdparty/EMV-tag-parser/bin"
		})
		
		ignoredefaultlibraries { "MSVCRTD","MSVCRT" }

	  filter "configurations:Debug"
		defines { "DEBUG", "_DEBUG" }
		symbols "On"
		
		links{
			"tlvparser-%{cfg.architecture}-d"
		}

	  filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		
		links{
			"tlvparser-%{cfg.architecture}"
		}

	  filter "platforms:Win32"
		defines{"__WXMSW__","WIN32"}
		system "windows"
		architecture "x86"
		staticruntime "on"

		includedirs({
			"include",
			"thirdparty/wxWidgets/include",
			"thirdparty/wxWidgets/include/msvc"
		})
		
		libdirs({
			"thirdparty/wxWidgets/lib/vc_lib_st",
		})

	  filter "platforms:Win64"
		defines{"__WXMSW__","WIN64"}
		system "windows"
		architecture "x86_64"
		staticruntime "on"

		includedirs({
			"include",
			"thirdparty/wxWidgets/include",
			"thirdparty/wxWidgets/include/msvc"
		})
		
		libdirs({
			"thirdparty/wxWidgets/lib/vc_x64_lib_st",
		})

	  filter "platforms:Linux"
		defines{"LINUX"}
		system "linux"

		includedirs({
			"include"
		})

		buildoptions{"`thirdparty/wxWidgets/wx-config --cxxflags`"}
		linkoptions{"`thirdparty/wxWidgets/wx-config --libs`"}
	   
	   
	project "wxWidgets"
	   kind "Makefile"
	   objdir()
	   
	   location("thirdparty/wxWidgets")
	   targetdir("%{prj.location}/lib")
	   includedirs{}

	   cleancommands {
		   
	   }
	   
	   filter "platforms:Win32"
	   
		   buildcommands {
			  "{CHDIR} %{prj.location}build/msw",
			  "nmake /f makefile.vc BUILD=%{cfg.buildcfg:lower()} RUNTIME_LIBS=static CFG=_st TARGET_CPU=X86"
		   }
	   
		   rebuildcommands {
			  "{CHDIR} %{prj.location}build/msw",
			  "nmake /f makefile.vc BUILD=%{cfg.buildcfg:lower()} RUNTIME_LIBS=static CFG=_st TARGET_CPU=X86"
		   }
	   
	   filter "platforms:Win64"
	   
		   buildcommands {
			  "{CHDIR} %{prj.location}build/msw",
			  "nmake /f makefile.vc BUILD=%{cfg.buildcfg:lower()} RUNTIME_LIBS=static CFG=_st TARGET_CPU=X64"
		   }
	   
		   rebuildcommands {
			  "{CHDIR} %{prj.location}build/msw",
			  "nmake /f makefile.vc BUILD=%{cfg.buildcfg:lower()} RUNTIME_LIBS=static CFG=_st TARGET_CPU=X64"
		   }
		
		filter "platforms:Linux"
		
			buildcommands {
			  "{CHDIR} %{prj.location}",
			  "./configure --disable-shared",
			  "make"
		   }
	   
		   rebuildcommands {
			  "{CHDIR} %{prj.location}",
			  "./configure --disable-shared",
			  "make"
		   }