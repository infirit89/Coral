#pragma once

#include "PropertyMarshalTestBase.hpp"

namespace Testing
{
	class ObjectPropertyMarshalTest : public PropertyMarshalTestBase
	{
	protected:
		ObjectPropertyMarshalTest() = default;
		~ObjectPropertyMarshalTest() override = default;
	};

	TEST_F(ObjectPropertyMarshalTest, StringPropertyTest) 
	{
		Coral::ScopedString value = m_PropertyTestObject.GetPropertyValue<Coral::String>("StringPropertyTest");
		ASSERT_EQ(value, "Hello");
		Coral::String newValue = Coral::String::New("Hello, World!");
		m_PropertyTestObject.SetPropertyValue("StringPropertyTest", newValue);
		value = m_PropertyTestObject.GetPropertyValue<Coral::String>("StringPropertyTest");
		ASSERT_EQ((Coral::String)value, newValue);
	}

	TEST_F(ObjectPropertyMarshalTest, DummyClassPropertTest) 
	{
		auto dummyClassInstance = Globals::TestAssembly
		                              ->GetType("Testing.Managed.PropertyMarshalTest+DummyClass")
		                              .CreateInstance();

		constexpr float valueF = 500.0f;
		constexpr bool valueB = true;
		dummyClassInstance.SetFieldValue<float>("X", valueF);
		dummyClassInstance.SetFieldValue<bool>("Y", valueB);
		m_PropertyTestObject.SetPropertyValue("DummyClassPropertyTest", dummyClassInstance);
		dummyClassInstance.Destroy();
		dummyClassInstance = m_PropertyTestObject.GetPropertyValue<Coral::ManagedObject>("DummyClassPropertyTest");
		ASSERT_EQ(dummyClassInstance.GetFieldValue<float>("X"), valueF);
		ASSERT_EQ(dummyClassInstance.GetFieldValue<bool>("Y"), valueB);
		dummyClassInstance.Destroy();
	}
}
