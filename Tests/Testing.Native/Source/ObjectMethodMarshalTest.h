#pragma once

#include "MethodMarshalTestBase.h"

namespace Testing
{
	class ObjectMethodMarshalTest : public MethodMarshalTestBase
	{
	protected:
		ObjectMethodMarshalTest() = default;
		~ObjectMethodMarshalTest() override = default;
	};

	TEST_F(ObjectMethodMarshalTest, IntPtrMethodTest) 
	{
		int32_t oldValue = 10, newValue = 50;
		int32_t value = *m_MethodTestObject.InvokeMethod<int32_t*, int32_t*>("IntPtrMethodTest", &oldValue);
		ASSERT_EQ(value, newValue);
	}

	TEST_F(ObjectMethodMarshalTest, StringMethodTest)
	{
		Coral::ScopedString oldValue = Coral::String::New("Hello");
		std::string_view newValue = "Hello, World!";

		Coral::ScopedString value = m_MethodTestObject.InvokeMethod<Coral::String, Coral::String>("StringMethodTest", oldValue);
		ASSERT_EQ(value, newValue);
	}

	TEST_F(ObjectMethodMarshalTest, DummyStructPtrMethodTest)
	{
		MethodDummyStruct oldValue = { 10, 10.0f, 10 };
		MethodDummyStruct newValue = { 20, 20.0f, 20 };
		auto result = m_MethodTestObject.InvokeMethod<MethodDummyStruct*, MethodDummyStruct*>("DummyStructPtrMethodTest", &oldValue);
		ASSERT_EQ((*result).X, newValue.X);
		ASSERT_EQ(result->Y, newValue.Y);
		ASSERT_LT(result->Y - newValue.Y, 0.001f);
		ASSERT_EQ(result->Z, newValue.Z);
	}

	TEST_F(ObjectMethodMarshalTest, OverloadInt32MethodTest)
	{
		int32_t value1 = 50, value2 = 1050;
		int32_t result = m_MethodTestObject.InvokeMethod<int32_t, int32_t&>("OverloadMethodTest", value1);
		ASSERT_EQ(result, value2);
	}

	TEST_F(ObjectMethodMarshalTest, OverloadFloatMethodTest)
	{
		float value1 = 5, value2 = 15.0f;
		float result = m_MethodTestObject.InvokeMethod<float, float&>("OverloadMethodTest", value1);
		ASSERT_EQ(result, value2);
	}
}
