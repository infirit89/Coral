#pragma once

#include "Globals.h"

#include <Coral/HostInstance.hpp>
#include <Coral/Assembly.hpp>
#include <Coral/Type.hpp>
#include <Coral/ManagedArray.hpp>
#include <Coral/ManagedObject.hpp>
#include <Coral/GC.hpp>

#include <gtest/gtest.h>

namespace Testing
{
	class NativeTest : public testing::Test
	{
	protected:
		NativeTest()
		    : m_Assembly(*Globals::TestAssembly) { }

		~NativeTest() override = default;

		Coral::ManagedAssembly& m_Assembly;
	};

	TEST_F(NativeTest, ManagedStaticMethodTest)
	{
		auto& testType = Globals::TestAssembly->GetType("Testing.Managed.Tests");
		float valueF = 50.0f;
		int valueI = 1000;
		float resultF = testType.InvokeStaticMethod<float>("StaticMethodTest", valueF);
		EXPECT_EQ(valueF, resultF);
		int resultI = testType.InvokeStaticMethod<int>("StaticMethodTest", valueI);
		EXPECT_EQ(valueI, resultI);
		EXPECT_FALSE(testType.InvokeStaticMethod<bool>("StaticMethodTestFalse"));
		EXPECT_TRUE(testType.InvokeStaticMethod<bool>("StaticMethodTestTrue"));
	}

	TEST_F(NativeTest, ManagedInternalCallsTest)
	{
		bool succeeded = Globals::TestType.InvokeStaticMethod<bool>("RunManagedTests");
		EXPECT_TRUE(succeeded);
	}

	TEST_F(NativeTest, IsTypeAssignableToTest) 
	{
		auto& fieldTestType = m_Assembly.GetType("Testing.Managed.FieldMarshalTest");
		EXPECT_TRUE(fieldTestType.IsAssignableTo(fieldTestType));
	}
}
