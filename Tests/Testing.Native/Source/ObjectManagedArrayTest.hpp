#pragma once

#include "Globals.hpp"
#include <Coral/ManagedArray.hpp>

#include <gtest/gtest.h>

namespace Testing 
{
	class ObjectManagedArrayTest : public testing::Test
	{
	protected:
		ObjectManagedArrayTest() = default;
		~ObjectManagedArrayTest() override = default;
	};

	TEST_F(ObjectManagedArrayTest, StringArrayTest) 
	{
		using namespace Coral;
		auto& stringType = Globals::TestAssembly->GetType("System.String");
		std::vector<Coral::String> values = 
		{ 
			String::New("test"),
			String::New("test1"),
			String::New("test2"),
			String::New("test3"),
			String::New("test4"),
			String::New("test5")
		};

		Coral::ManagedArray arr = Coral::ManagedArray::New(stringType, values.size());

		for (int32_t i = 0; i < arr.GetLength(0); i++) 
		{
			arr.SetValue<String>(i, values.at(i));
			ScopedString result = arr.GetValue<String>(i);
			ASSERT_EQ(result, values.at(i));
		}

		arr.Destroy();
	}

	TEST_F(ObjectManagedArrayTest, DummyClassArrayTest) 
	{
		auto& dummClassItemType = Globals::TestAssembly->GetType("Testing.Managed.DummyClassItem");


	}
}
