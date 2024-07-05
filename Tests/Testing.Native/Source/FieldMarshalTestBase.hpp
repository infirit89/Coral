#pragma once

#include "Globals.hpp"

#include <gtest/gtest.h>

namespace Testing
{
	class FieldMarshalTestBase : public testing::Test
	{
	protected:
		FieldMarshalTestBase() 
		{
			auto& fieldTestType = Globals::TestAssembly->GetType("Testing.Managed.FieldMarshalTest");
			m_FieldTestObject = fieldTestType.CreateInstance();
		}

		~FieldMarshalTestBase() override 
		{
			m_FieldTestObject.Destroy();
		}

		Coral::ManagedObject m_FieldTestObject;
	};
}
