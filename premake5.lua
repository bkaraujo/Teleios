workspace "Teleios"
   architecture "x64"
   configurations { "Alpha", "Beta", "Release" }
   startproject "sandbox"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "engine/premake5.lua"
include "sandbox/premake5.lua"