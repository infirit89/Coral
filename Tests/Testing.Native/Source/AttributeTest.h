#pragma once

#include "Globals.h"

#include <Coral/Attribute.hpp>

#include <gtest/gtest.h>

namespace Testing 
{
#define STRINGIFY(value) #value

	class AttributeTest : public testing::Test
	{
	protected:
		AttributeTest() 
		{
			auto& assembly = *Globals::TestAssembly;
			m_AttributeTestType = assembly.GetType("Testing.Managed.AttributeTest");
			m_DummyAttributeType = assembly.GetType("Testing.Managed.DummyAttribute");

			m_AttributeFieldTestValue = m_AttributeTestType.GetStaticFieldValue<float>(STRINGIFY(m_AttributeFieldTestValue));
			m_AttributePropertyTestValue = m_AttributeTestType.GetStaticFieldValue<float>(STRINGIFY(m_AttributePropertyTestValue));
			m_AttributeMethodTestValue = m_AttributeTestType.GetStaticFieldValue<float>(STRINGIFY(m_AttributeMethodTestValue));
		}

		~AttributeTest() override = default;

		Coral::Type m_AttributeTestType;
		Coral::Type m_DummyAttributeType;
		float m_AttributeFieldTestValue;
		float m_AttributePropertyTestValue;
		float m_AttributeMethodTestValue;
	};

	TEST_F(AttributeTest, FieldAttributeValueTest) 
	{
		for (const auto& field : m_AttributeTestType.GetFields())
		{
			for (auto& attribute : field.GetAttributes())
			{
				auto& currentAttributeType = attribute.GetType();
				if (currentAttributeType == m_DummyAttributeType)
					ASSERT_EQ(attribute.GetFieldValue<float>("SomeValue"), m_AttributeFieldTestValue);
			}
		}
	}

	TEST_F(AttributeTest, PropertyAttributeValueTest)
	{
		for (const auto& _property : m_AttributeTestType.GetProperties())
		{
			for (auto& attribute : _property.GetAttributes())
			{
				auto& currentAttributeType = attribute.GetType();
				if (currentAttributeType == m_DummyAttributeType)
					ASSERT_EQ(attribute.GetFieldValue<float>("SomeValue"), m_AttributePropertyTestValue);
			}
		}
	}

	TEST_F(AttributeTest, MethodAttributeValueTest)
	{
		for (const auto& method : m_AttributeTestType.GetMethods())
		{
			for (auto& attribute : method.GetAttributes())
			{
				auto& currentAttributeType = attribute.GetType();
				if (currentAttributeType == m_DummyAttributeType)
					ASSERT_EQ(attribute.GetFieldValue<float>("SomeValue"), m_AttributeMethodTestValue);
			}
		}
	}
}
