#pragma once

#include "Globals.h"
#include <gtest/gtest.h>

namespace Testing 
{
	class PropertyMarshalTestBase : public testing::Test
	{
	protected:
		PropertyMarshalTestBase()
		{
			auto& propertyTestType = Globals::TestAssembly->GetType("Testing.Managed.PropertyMarshalTest");
			m_PropertyTestObject = propertyTestType.CreateInstance();
		}

		~PropertyMarshalTestBase() override
		{
			m_PropertyTestObject.Destroy();
		}

		Coral::ManagedObject m_PropertyTestObject;
	};


}
