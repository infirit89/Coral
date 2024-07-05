#pragma once

#include "MethodMarshalTestBase.hpp"

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
		ASSERT_EQ(*result, newValue);
		ASSERT_LT(result->Y - newValue.Y, 0.001f);
	}

	TEST_F(ObjectMethodMarshalTest, DummyClassMethodTest)
	{
		MethodDummyStruct value1 = { 10, 10.0f, 10 };
		MethodDummyStruct value2 = { 20, 20.0f, 20 };
		
		auto dummyClassInstance = Globals::TestAssembly
		                              ->GetType("Testing.Managed.MemberMethodTest+DummyClass")
		                              .CreateInstance(value1);

		m_MethodTestObject.InvokeMethod("DummyClassMethodTest", dummyClassInstance);
		auto result = dummyClassInstance.InvokeMethod<MethodDummyStruct>("GetValues");
		ASSERT_EQ(result, value2);
		ASSERT_LT(result.Y - value2.Y, 0.001f);
		dummyClassInstance.Destroy();
	}
}
