project "Sandbox"
   kind "ConsoleApp"
   language "C"
   cdialect "C99"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.c", "src/**.cpp" }

   includedirs {
      "src",
	  "../engine/src"
   }

   links {
      "Engine"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "TL_TARGET_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
       links { }

   filter "configurations:Alpha"
       defines { "TL_BUILD_ALPHA" }
       runtime "Debug"
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