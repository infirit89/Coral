#pragma once

#include "Globals.hpp"

#include <gtest/gtest.h>

namespace Testing 
{
	class TypeTest : public testing::Test
	{
	protected:
		TypeTest() 
		{
			m_BaseTypeName = "Testing.Managed.BaseTypeTest";
			m_BaseTypeTest = Globals::TestAssembly
			                             ->GetType(m_BaseTypeName);

			m_SubclassTypeTest = Globals::TestAssembly
			                                 ->GetType("Testing.Managed.SubclassTypeTest");

			m_StructTypeTest = Globals::TestAssembly
			                       ->GetType("Testing.Managed.StructTypeTest");
		}
		~TypeTest() override = default;

		Coral::Type m_BaseTypeTest,
					m_SubclassTypeTest,
					m_StructTypeTest;
		std::string_view m_BaseTypeName;
	};

	TEST_F(TypeTest, BaseTypeHasTypeTestAttribute) 
	{
		auto& typeTestAttribute = Globals::TestAssembly->GetType("Testing.Managed.TypeTestAttribute");
		ASSERT_TRUE(m_BaseTypeTest.HasAttribute(typeTestAttribute));
	}

	TEST_F(TypeTest, BaseTypeFullName) 
	{
		Coral::ScopedString fullName = m_BaseTypeTest.GetFullName();
		ASSERT_EQ(fullName, m_BaseTypeName);
	}

	TEST_F(TypeTest, SubclassTypeIsChildOfBaseType) 
	{
		ASSERT_TRUE(m_SubclassTypeTest.IsSubclassOf(m_BaseTypeTest));
	}

	TEST_F(TypeTest, BaseTypeIsAssignableToSelf) 
	{
		ASSERT_TRUE(m_BaseTypeTest.IsAssignableTo(m_BaseTypeTest));
	}

	TEST_F(TypeTest, StructTypeSizeTest) 
	{
		constexpr int typeSize = 8;
		ASSERT_EQ(m_StructTypeTest.GetSize(), typeSize);
	}

	TEST_F(TypeTest, ManagedStaticMethodTest)
	{
		auto& testType = Globals::TestAssembly->GetType("Testing.Managed.StaticTest");
		float valueF = 50.0f;
		int valueI = 1000;
		bool valueB = true;
		float resultF = testType.InvokeStaticMethod<float>("StaticMethodTest", valueF);
		EXPECT_EQ(valueF, resultF);
		int resultI = testType.InvokeStaticMethod<int>("StaticMethodTest", valueI);
		EXPECT_EQ(valueI, resultI);
		bool resultB = testType.InvokeStaticMethod<bool>("StaticMethodTest", valueB);
		EXPECT_TRUE(resultB);
	}
}
