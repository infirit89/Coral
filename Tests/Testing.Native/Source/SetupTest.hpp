#pragma once

#include "Globals.hpp"
#include "InternalCalls.hpp"

#include <gtest/gtest.h>

namespace Testing
{
	class SetUpTest : public testing::Test
	{
	protected:
		SetUpTest() = default;
		~SetUpTest() override = default;
	};

#define EXIT_ON_FAILURE(condition)\
	if (!(condition))\
		exit(EXIT_FAILURE)

	TEST_F(SetUpTest, InitializeTest) 
	{
		bool value = Globals::HostInstance.Initialize(Globals::HostSettings);
		ASSERT_TRUE(value);
		EXIT_ON_FAILURE(value);
	}

	TEST_F(SetUpTest, LoadAssemblyTest) 
	{
		Globals::AssemblyLoadContext = Globals::HostInstance.CreateAssemblyLoadContext("TestContext");
		Globals::TestAssembly = &Globals::AssemblyLoadContext.LoadAssembly(Globals::AssemblyPath.string());
		ASSERT_EQ(Globals::TestAssembly->GetLoadStatus(), Coral::AssemblyLoadStatus::Success);
		EXIT_ON_FAILURE(Globals::TestAssembly->GetLoadStatus() == Coral::AssemblyLoadStatus::Success);

		auto& assembly = *Globals::TestAssembly;
		InternalCalls::Register(assembly);
		Globals::TestAssembly->UploadInternalCalls();
		Globals::TestType = assembly.GetType("Testing.Managed.TestUtils");
		auto& instanceTestType = assembly.GetType("Testing.Managed.InstanceTest");
		Globals::TestInstance = instanceTestType.CreateInstance();
		Globals::TestInstance.SetFieldValue("X", 500.0f);
	}
}
