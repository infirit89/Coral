#pragma once

#include "Globals.h"

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
			m_MethodTestObject = methodTestType.CreateInstance();
		}

		~MethodMarshalTestBase() override
		{
			m_MethodTestObject.Destroy();
		}

		Coral::ManagedObject m_MethodTestObject;
	};
}
