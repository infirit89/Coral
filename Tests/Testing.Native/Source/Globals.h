#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <functional>
#include <ranges>

#include <Coral/HostInstance.hpp>
#include <Coral/Assembly.hpp>
#include <Coral/Type.hpp>
#include <Coral/ManagedObject.hpp>

namespace Testing
{
	struct Globals
	{
		static Coral::HostInstance HostInstance;
		static Coral::ManagedAssembly* TestAssembly;
		static Coral::AssemblyLoadContext AssemblyLoadContext;
		static Coral::ManagedObject TestInstance;
		static Coral::Type TestType;
	};
}
