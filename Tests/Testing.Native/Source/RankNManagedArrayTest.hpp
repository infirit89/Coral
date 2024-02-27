#pragma once

#include "Globals.hpp"
#include "ManagedArrayTestCommon.hpp"

#include <Coral/ManagedArray.hpp>

#include <gtest/gtest.h>

namespace Testing
{
	class RankNManagedArrayTest : public testing::Test 
	{
	protected:
		RankNManagedArrayTest() {}
		~RankNManagedArrayTest() override {}
	};

	TEST_F(RankNManagedArrayTest, FloatArrayTest)
	{
		constexpr size_t lengthsSize = 3;
		int32_t lengths[lengthsSize] = { 2, 2, 3 };
		int32_t lengths2[lengthsSize] = { 2, 2, 5 };

		Coral::Type type = Globals::TestAssembly->GetType("System.Single");
		Coral::ManagedArray arr = Coral::ManagedArray::New(type, lengths, lengthsSize);

		float values[2][2][5] = 
		{
			{
				{ 1.021f, 0.23f, 23.231f, 8432.32f, 500.002f },
			    { 6.321f, 7.0321f, 8.32f, 0.92f, 0.001f }
			},
			{ 
				{ 1.21f, 0.203f, 3.23, 4832.32f, 50.02f },
			    { 6.32f, 7.01f, 8.2f, 9.02f, 0.011f } 
			},
		};

		for (int32_t i = 0; i < lengths[0]; i++)
		{
			for (int32_t j = 0; j < lengths[1]; j++)
			{
				for (int32_t k = 0; k < lengths[2]; k++)
				{
					int32_t indices[lengthsSize] = { i, j, k };
					arr.SetValue(indices, lengthsSize, values[i][j][k]);
				}
			}
		}

		arr.Resize(lengths2, lengthsSize);

		for (int32_t i = 0; i < lengths2[0]; i++)
		{
			for (int32_t j = 0; j < lengths2[1]; j++)
			{
				for (int32_t k = 3; k < lengths2[2]; k++)
				{
					int32_t indices[lengthsSize] = { i, j, k };
					arr.SetValue(indices, lengthsSize, values[i][j][k]);
					ASSERT_EQ(arr.GetValue<float>(indices, lengthsSize), values[i][j][k]);
				}
			}
		}

		arr.Destroy();
	}

	TEST_F(RankNManagedArrayTest, StructArrayTest) 
	{
		constexpr size_t lengthsSize = 3;
		int32_t lengths[lengthsSize] = { 2, 2, 1 };
		int32_t lengths2[lengthsSize] = { 2, 2, 2 };

		Coral::Type type = Globals::TestAssembly->GetType("Testing.Managed.DummyStructItem");
		Coral::ManagedArray arr = Coral::ManagedArray::New(type, lengths, lengthsSize);

		DummyStructItem values[2][2][2] =
		{
			{
				{ { 1, 0.23f }, { 23, 8432.32f } },
			    { { 6, 7.0321f }, { 8, 0.92f } }
			},
			{ 
				{ { 2, 0.203f }, { 3, 4832.32f } },
			    { { 632, 7.01f }, { 82, 9.02f } }
			},
		};

		for (int32_t i = 0; i < lengths[0]; i++)
		{
			for (int32_t j = 0; j < lengths[1]; j++)
			{
				for (int32_t k = 0; k < lengths[2]; k++)
				{
					int32_t indices[lengthsSize] = { i, j, k };
					arr.SetValue(indices, lengthsSize, values[i][j][k]);
				}
			}
		}

		arr.Resize(lengths2, lengthsSize);

		for (int32_t i = 0; i < lengths2[0]; i++)
		{
			for (int32_t j = 0; j < lengths2[1]; j++)
			{
				for (int32_t k = 1; k < lengths2[2]; k++)
				{
					int32_t indices[lengthsSize] = { i, j, k };
					arr.SetValue(indices, lengthsSize, values[i][j][k]);
					ASSERT_EQ(arr.GetValue<DummyStructItem>(indices, lengthsSize), values[i][j][k]);
				}
			}
		}

		arr.Destroy();
	}

	TEST_F(RankNManagedArrayTest, ClassArrayTest)
	{
		constexpr size_t lengthsSize = 3;
		int32_t lengths[lengthsSize] = { 2, 2, 2 };
		int32_t lengths2[lengthsSize] = { 2, 2, 1 };

		Coral::Type type = Globals::TestAssembly->GetType("Testing.Managed.DummyClassItem");
		Coral::ManagedArray arr = Coral::ManagedArray::New(type, lengths, lengthsSize);

		struct ArrayItem 
		{
			std::string_view SValue;
			float FValue;
		};

		ArrayItem values[2][2][2] = 
		{
			{ 
				{
					{ "Test", 1.023f },
					{ "Test3", 2.023f },
				},
				{
			        { "Test54", 3.432f },
			        { "Test-Test+Test=Test", 4.3205f },
				} 
			}
		};

		for (int32_t i = 0; i < lengths[0]; i++)
		{
			for (int32_t j = 0; j < lengths[1]; j++)
			{
				for (int32_t k = 0; k < lengths[2]; k++)
				{
					int32_t indices[lengthsSize] = { i, j, k };
					Coral::ScopedString sValue = Coral::String::New(values[i][j][k].SValue);
					Coral::ManagedObject value = type.CreateInstance(sValue, values[i][j][k].FValue);
					arr.SetValue(indices, lengthsSize, value);
					value.Destroy();
				}
			}
		}

		arr.Resize(lengths2, lengthsSize);

		for (int32_t i = 0; i < lengths2[0]; i++)
		{
			for (int32_t j = 0; j < lengths2[1]; j++)
			{
				for (int32_t k = 1; k < lengths2[2]; k++)
				{
					int32_t indices[lengthsSize] = { i, j, k };
					Coral::ScopedString sValue = Coral::String::New(values[i][j][k].SValue);
					Coral::ManagedObject value = type.CreateInstance(sValue, values[i][j][k].FValue);
					arr.SetValue(indices, lengthsSize, value);
					value.Destroy();
					Coral::ManagedObject result = arr.GetValue<Coral::ManagedObject>(indices, lengthsSize);

					sValue = result.GetFieldValue<Coral::String>("TestString");
					EXPECT_EQ(sValue, values[i][j][k].SValue);
					float fResult = result.GetFieldValue<float>("TestFloat");
					EXPECT_EQ(fResult, values[i][j][k].FValue);
					result.Destroy();
				}
			}
		}

		arr.Destroy();
	}
}
