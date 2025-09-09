premake.api.addAllowed("debuggertype", "NativeWithManagedCore")

project "Coral.Native"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"
    staticruntime "Off"

	dependson "Coral.Managed"

	targetdir("../Build/%{cfg.buildcfg}")
	objdir("../Intermediates/%{cfg.buildcfg}")

    pchheader "CoralPCH.hpp"
    pchsource "Source/CoralPCH.cpp"

    forceincludes { "CoralPCH.hpp" }

    filter { "action:xcode4" }
        pchheader "Source/CoralPCH.hpp"
    filter { }

    files {
        "Source/**.cpp",
        "Source/**.hpp",

        "Include/Coral/**.hpp",
    }

    includedirs { "Source/", "Include/Coral/" }
    externalincludedirs { "../NetCore/" }

    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "On"

    filter { "configurations:Release" }
        runtime "Release"
        symbols "Off"
        optimize "On"

	filter { "system:windows" }
        architecture "x86_64"
		defines { "CORAL_WINDOWS" }

	filter { "system:linux" }
		defines { "CORAL_LINUX" }

	filter { "system:macosx" }
        architecture "ARM64"
		defines { "CORAL_MACOSX" }
