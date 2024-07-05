#pragma once

#include "FieldMarshalTestBase.hpp"

namespace Testing
{
	class ObjectFieldMarhalTest : public FieldMarshalTestBase
	{
	protected:
		ObjectFieldMarhalTest() {}
		~ObjectFieldMarhalTest() override {}
	};

	TEST_F(ObjectFieldMarhalTest, StringFieldTest) 
	{
		Coral::ScopedString value = m_FieldTestObject.GetFieldValue<Coral::String>("StringFieldTest");
		ASSERT_EQ(value, "Hello");
		Coral::String newValue = Coral::String::New("Hello, World!");
		m_FieldTestObject.SetFieldValue("StringFieldTest", newValue);
		value = m_FieldTestObject.GetFieldValue<Coral::String>("StringFieldTest");
		ASSERT_EQ((Coral::String)value, newValue);
	}

	TEST_F(ObjectFieldMarhalTest, DummyClassFieldTest) 
	{
		auto dummyClassInstance = Globals::TestAssembly
		                              ->GetType("Testing.Managed.FieldMarshalTest+DummyClass")
		                              .CreateInstance();

		constexpr float value = 500.0f;
		dummyClassInstance.SetFieldValue<float>("X", value);
		m_FieldTestObject.SetFieldValue("DummyClassFieldTest", dummyClassInstance);
		dummyClassInstance.Destroy();
		dummyClassInstance = m_FieldTestObject.GetFieldValue<Coral::ManagedObject>("DummyClassFieldTest");
		ASSERT_EQ(dummyClassInstance.GetFieldValue<float>("X"), value);
		dummyClassInstance.Destroy();
	}
}
