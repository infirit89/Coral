#pragma once

#include "Globals.hpp"
#include <Coral/ManagedArray.hpp>

#include <gtest/gtest.h>

namespace Testing 
{
	class StringManagedArrayTest : public testing::Test 
	{
	protected:
		StringManagedArrayTest() 
		{
			m_StringType = Globals::TestAssembly->GetType("System.String");
		}

		~StringManagedArrayTest() override = default;

		std::array<Coral::ScopedString, 6> m_StringValues = {
			Coral::String::New("test"),
			Coral::String::New("test1"),
			Coral::String::New("test2"),
			Coral::String::New("test3"),
			Coral::String::New("test4"),
			Coral::String::New("test5")
		};

		Coral::Type m_StringType;
	};

	TEST_F(StringManagedArrayTest, StringArrayTest) 
	{
		Coral::ManagedArray arr = Coral::ManagedArray::New(m_StringType, m_StringValues.size());

		for (int32_t i = 0; i < arr.GetLength(0); i++) 
		{
			arr.SetValue<Coral::String>(i, m_StringValues.at(i));
			Coral::ScopedString result = arr.GetValue<Coral::String>(i);
			ASSERT_EQ(result, m_StringValues.at(i));
		}

		arr.Destroy();
	}

	TEST_F(StringManagedArrayTest, ResizeRankOneStringArrayTest) 
	{
		Coral::ManagedArray arr = Coral::ManagedArray::New(m_StringType, m_StringValues.size() - 3);

		for (int32_t i = 0; i < arr.GetLength(0); i++)
			arr.SetValue<Coral::String>(i, m_StringValues.at(i));

		arr.Resize(m_StringValues.size());

		for (int32_t i = m_StringValues.size() - 2; i < arr.GetLength(0); i++)
		{
			arr.SetValue<Coral::String>(i, m_StringValues.at(i));
			Coral::ScopedString result = arr.GetValue<Coral::String>(i);
			ASSERT_EQ(result, m_StringValues.at(i));
		}

		arr.Destroy();
	}


	class ObjectManagedArrayTest : public testing::Test
	{
	protected:
		ObjectManagedArrayTest()
		{
			m_DummyClassItemType = Globals::TestAssembly->GetType("Testing.Managed.DummyClassItem");
			m_Objects[0] = m_DummyClassItemType.CreateInstance();
			m_Objects[1] = m_DummyClassItemType.CreateInstance(std::get<0>(m_DummyClassValues[1]), std::get<1>(m_DummyClassValues[1]));
		}

		~ObjectManagedArrayTest() override 
		{
			for (auto& object : m_Objects)
				object.Destroy();
		}

		std::array<std::tuple<Coral::ScopedString, float>, 2> m_DummyClassValues = {
			{ { Coral::String::New("Default"), 5.123f }, { Coral::String::New("Item2"), 0.0004f } }
		};

		std::array<Coral::ManagedObject, 2> m_Objects;
		Coral::Type m_DummyClassItemType;
	};


	TEST_F(ObjectManagedArrayTest, DummyClassArrayTest) 
	{
		Coral::ManagedArray arr = Coral::ManagedArray::New(m_DummyClassItemType, m_Objects.size());

		for (int32_t i = 0; i < arr.GetLength(0); i++)
		{
			arr.SetValue<Coral::ManagedObject>(i, m_Objects[i]);
			m_Objects[i].Destroy();
			m_Objects[i] = arr.GetValue<Coral::ManagedObject>(i);
			Coral::ScopedString testStringResult = m_Objects[i].GetFieldValue<Coral::String>("TestString");
			float testFloatResult = m_Objects[i].GetFieldValue<float>("TestFloat");

			ASSERT_EQ(testStringResult, std::get<0>(m_DummyClassValues[i]));
			ASSERT_EQ(testFloatResult, std::get<1>(m_DummyClassValues[i]));
		}

		arr.Destroy();
	}

	TEST_F(ObjectManagedArrayTest, ResizeRankOneDummyClassArrayTest) 
	{
		Coral::ManagedArray arr = Coral::ManagedArray::New(m_DummyClassItemType, m_Objects.size() - 1);

		for (int32_t i = 0; i < arr.GetLength(0); i++)
			arr.SetValue<Coral::ManagedObject>(i, m_Objects.at(i));

		arr.Resize(m_Objects.size());

		for (int32_t i = m_Objects.size(); i < arr.GetLength(0); i++)
		{
			arr.SetValue<Coral::ManagedObject>(i, m_Objects.at(i));
			m_Objects.at(i).Destroy();
			m_Objects[i] = arr.GetValue<Coral::ManagedObject>(i);

			Coral::ScopedString testStringResult = m_Objects.at(i).GetFieldValue<Coral::String>("TestString");
			float testFloatResult = m_Objects.at(i).GetFieldValue<float>("TestFloat");

			ASSERT_EQ(testStringResult, std::get<0>(m_DummyClassValues.at(i)));
			ASSERT_EQ(testFloatResult, std::get<1>(m_DummyClassValues.at(i)));
		}

		arr.Destroy();
	}
}
