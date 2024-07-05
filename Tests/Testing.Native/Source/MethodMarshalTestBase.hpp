#pragma once

#include "Globals.hpp"

#include <gtest/gtest.h>

namespace Testing
{
	struct MethodDummyStruct
	{
		bool operator==(const MethodDummyStruct& other) const
		{
			return X == other.X && Y == other.Y && Z == other.Z;
		}

		int X;
		float Y;
		int Z;
	};

	class MethodMarshalTestBase : public testing::Test 
	{
	protected:
		MethodMarshalTestBase() 
		{
			auto& methodTestType = Globals::TestAssembly->GetType("Testing.Managed.MemberMethodTest");
			for(const auto& method : methodTestType.GetMethods())
			{
				Coral::ScopedString methodName = method.GetName();
				std::cout << std::string(methodName) << '\n';
			}
			m_MethodTestObject = methodTestType.CreateInstance();
		}

		~MethodMarshalTestBase() override
		{
			m_MethodTestObject.Destroy();
		}

		Coral::ManagedObject m_MethodTestObject;
	};

	TEST_F(MethodMarshalTestBase, OverloadInt32MethodTest)
	{
		int32_t value1 = 50, value2 = 1050;
		int32_t result = m_MethodTestObject.InvokeMethod<int32_t, int32_t&>("OverloadMethodTest", value1);
		ASSERT_EQ(result, value2);
	}

	TEST_F(MethodMarshalTestBase, OverloadFloatMethodTest)
	{
		float value1 = 5, value2 = 15.0f;
		float result = m_MethodTestObject.InvokeMethod<float, float&>("OverloadMethodTest", value1);
		ASSERT_EQ(result, value2);
	}

	TEST_F(MethodMarshalTestBase, Override1MethodTest) 
	{
		auto virtualMethodTestInstance = Globals::TestAssembly
		                                      ->GetType("Testing.Managed.Override1")
		                                      .CreateInstance();
		
		Coral::ScopedString value = virtualMethodTestInstance.InvokeMethod<Coral::String>("TestMe");
		ASSERT_EQ(value, "Override1");
		virtualMethodTestInstance.Destroy();
	}

	TEST_F(MethodMarshalTestBase, Override2MethodTest)
	{
		auto virtualMethodTestInstance = Globals::TestAssembly
		                                     ->GetType("Testing.Managed.Override2")
		                                     .CreateInstance();

		Coral::ScopedString value = virtualMethodTestInstance.InvokeMethod<Coral::String>("TestMe");
		ASSERT_EQ(value, "Override2");
		virtualMethodTestInstance.Destroy();
	}

}
