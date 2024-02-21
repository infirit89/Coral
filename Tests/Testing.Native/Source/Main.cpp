#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <functional>
#include <ranges>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>
#include <Coral/ManagedArray.hpp>
#include <Coral/TypeCache.hpp>

#include <gtest/gtest.h>

#include "Globals.hpp"
#include "NativeTest.hpp"
#include "InternalCalls.hpp"
#include "AttributeTest.hpp"
#include "ScalarFieldMarshalTest.hpp"
#include "ObjectFieldMarshalTest.hpp"
#include "ScalarPropertyMarshalTest.hpp"
#include "ObjectPropertyMarshalTest.hpp"
#include "ScalarMethodMarshalTest.hpp"
#include "ObjectMethodMarshalTest.hpp"
#include "InternalCallsTest.hpp"
#include "TypeTest.hpp"
#include "ManagedArrayTest.hpp"
#include "ObjectManagedArrayTest.hpp"

void ExceptionCallback(std::string_view InMessage)
{
	std::cout << "\033[1;31m " << "Unhandled native exception: " << InMessage << "\033[0m\n";
}

using namespace Testing;

int main(int argc, char** argv)
{
	
	#ifdef CORAL_TESTING_DEBUG
	const char* ConfigName = "Debug";
#else
	const char* ConfigName = "Release";
#endif

	auto exeDir = std::filesystem::path(argv[0]).parent_path();
	
	Globals::HostSettings = {
		.CoralDirectory = exeDir.string(),
		.ExceptionCallback = ExceptionCallback
	};

	Globals::HostInstance.Initialize(Globals::HostSettings);

	Globals::AssemblyLoadContext = Globals::HostInstance.CreateAssemblyLoadContext("TestContext");
	auto assemblyPath = exeDir / "Testing.Managed.dll";
	Globals::TestAssembly = &Globals::AssemblyLoadContext.LoadAssembly(assemblyPath.string());
	auto& assembly = *Globals::TestAssembly;
	InternalCalls::Register(assembly);
	Globals::TestAssembly->UploadInternalCalls();
	Globals::TestType = assembly.GetType("Testing.Managed.TestUtils");

	auto& instanceTestType = assembly.GetType("Testing.Managed.InstanceTest");
	Globals::TestInstance = instanceTestType.CreateInstance();
	Globals::TestInstance.SetFieldValue("X", 500.0f);

	testing::InitGoogleTest(&argc, argv);
	RegisterManagedTests();
	int result = RUN_ALL_TESTS();

	Coral::GC::Collect();
	Globals::HostInstance.UnloadAssemblyLoadContext(Globals::AssemblyLoadContext);

	return result;
}
