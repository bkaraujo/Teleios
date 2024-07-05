project "Engine"
   kind "StaticLib"
   language "C"
   cdialect "C11"
   cppdialect "C++11"
   targetdir "build/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.c", "src/**.cpp" }

   includedirs {
      "src"
   }
   
   flags {
		"MultiProcessorCompile"
   }
   
   defines { "TL_EXPORT", "CGLM_STATIC" }

   targetdir ("../build/" .. OutputDir .. "/%{prj.name}")
   objdir ("../build/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "TL_TARGET_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
       links { "opengl32.lib" }

   filter "configurations:Alpha"
       defines { "TL_BUILD_ALPHA" }
       runtime "Debug"
       optimize "Off"
       symbols "On"

   filter "configurations:Beta"
       defines { "TL_BUILD_BETA" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Release"
       defines { "TL_BUILD_RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "Off"