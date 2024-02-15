#pragma once

#include "Globals.hpp"
#include <Coral/ManagedArray.hpp>

#include <gtest/gtest.h>

namespace Testing 
{
	struct DummyStructItem 
	{
		bool operator==(const DummyStructItem& other) const 
		{
			return TestInt == other.TestInt && TestFloat == other.TestFloat;
		}

		int TestInt;
		float TestFloat;
	};

	template <typename T>
	using ArrayData = std::tuple<std::string_view, std::vector<T>>;

	template<typename T>
	class ManagedArrayTest : public testing::Test
	{
	protected:
		ManagedArrayTest() = default;
		~ManagedArrayTest() override = default;

		static ArrayData<T> s_ArrayData;
	};

	TYPED_TEST_SUITE_P(ManagedArrayTest);

	TYPED_TEST_P(ManagedArrayTest, ArrayTest)
	{
		auto& arrayData = ManagedArrayTest<TypeParam>::s_ArrayData;
		std::string_view typeName = std::get<0>(arrayData);
		std::vector<TypeParam> values = std::get<1>(arrayData);

		auto& type = Globals::TestAssembly->GetType(typeName);
		Coral::ManagedArray arr = Coral::ManagedArray::New(type, values.size());
		
		for (int32_t i = 0; i < arr.GetLength(0); i++)
		{
			arr.SetValue<TypeParam>(i, values.at(i));
			TypeParam result = arr.GetValue<TypeParam>(i);
			ASSERT_EQ(result, values.at(i));
		}
		arr.Destroy();
	}

	REGISTER_TYPED_TEST_SUITE_P(ManagedArrayTest, ArrayTest);

	using ArrayTypes = ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, bool, DummyStructItem>;
	INSTANTIATE_TYPED_TEST_SUITE_P(Member, ManagedArrayTest, ArrayTypes);

		template <>
	ArrayData<int8_t> ManagedArrayTest<int8_t>::s_ArrayData {
		"System.SByte", { 10, 20, -5, -10, -20 }
	};
	template <>
	ArrayData<uint8_t> ManagedArrayTest<uint8_t>::s_ArrayData {
		"System.Byte", { 10, 20, 0, 10 }
	};

	template <>
	ArrayData<int16_t> ManagedArrayTest<int16_t>::s_ArrayData {
		"System.Int16", { 10, 20, 256, -1000 }
	};
	template <>
	ArrayData<uint16_t> ManagedArrayTest<uint16_t>::s_ArrayData {
		"System.UInt16", { 10, 20, 2000, 300 }
	};

	template <>
	ArrayData<int32_t> ManagedArrayTest<int32_t>::s_ArrayData {
		"System.Int32", { 10, 20, 40000, -6000000, -3423413 }
	};
	template <>
	ArrayData<uint32_t> ManagedArrayTest<uint32_t>::s_ArrayData {
		"System.UInt32", { 10, 20, 9432423, 321123, 543732, 1235853, 953484 }
	};

	template <>
	ArrayData<int64_t> ManagedArrayTest<int64_t>::s_ArrayData {
		"System.Int64", { 10, 20, -1, -2, -3, -4, -5, -6, -7, 8 -9, 21312, -4234231254353345 }
	};
	template <>
	ArrayData<uint64_t> ManagedArrayTest<uint64_t>::s_ArrayData {
		"System.UInt64", { 10, 20, 293213092132, 903244348923423, 92312392123123, 9095345345, 8958349586546456 }
	};

	template <>
	ArrayData<float> ManagedArrayTest<float>::s_ArrayData {
		"System.Single", { 10.0f, 20.0f, 0.001f, 2.0234f, 5.102f, -0.2313f, 7.0231123f }
	};
	template <>
	ArrayData<double> ManagedArrayTest<double>::s_ArrayData {
		"System.Double", { 10.0, 20.0, 102313.123123, 6544.000123123, -234 }
	};

	template <>
	ArrayData<bool> ManagedArrayTest<bool>::s_ArrayData {
		"System.Boolean", { false, true, false, true, false, true, false, false, false, true, true, true, true }
	};

	template <>
	ArrayData<DummyStructItem> ManagedArrayTest<DummyStructItem>::s_ArrayData {
		"Testing.Managed.DummyStructItem", { { 0, 50.0f }, { -213, -0.1f }, { 12, -20.0f }, { 2, 4.0f }, { -1231, 0.0f } }
	};
}
