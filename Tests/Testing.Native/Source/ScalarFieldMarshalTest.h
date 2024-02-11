#pragma once

#include "FieldMarshalTestBase.h"

namespace Testing
{
	struct FieldDummyStruct
	{
		bool operator==(const FieldDummyStruct& other) const
		{
			return X == other.X;
		}

		float X;
	};


	template<typename T>
	using FieldData = std::tuple<std::string_view, std::tuple<T, T>>;

	template<typename T>
	class ScalarFieldMarshalTest : public FieldMarshalTestBase
	{
	protected:
		ScalarFieldMarshalTest() = default;

		~ScalarFieldMarshalTest() override = default;

		static FieldData<T> s_FieldData;
	};

	TYPED_TEST_SUITE_P(ScalarFieldMarshalTest);

	TYPED_TEST_P(ScalarFieldMarshalTest, FieldTest)
	{
		auto& fieldData = ScalarFieldMarshalTest<TypeParam>::s_FieldData;
		std::string_view fieldName = std::get<0>(fieldData);
		auto fieldValues = std::get<1>(fieldData);
		TypeParam oldValue = std::get<0>(fieldValues);
		TypeParam newValue = std::get<1>(fieldValues);

		auto value = this->m_FieldTestObject.GetFieldValue<TypeParam>(fieldName);
		if constexpr (std::same_as<TypeParam, float> || std::same_as<TypeParam, double>) 
		{
			ASSERT_LE(value - oldValue, static_cast<TypeParam>(0.001));
		}
		else
		{
			ASSERT_EQ(value, oldValue);
		}
		this->m_FieldTestObject.SetFieldValue<TypeParam>(fieldName, newValue);
		value = this->m_FieldTestObject.GetFieldValue<TypeParam>(fieldName);
		if constexpr (std::same_as<TypeParam, float> || std::same_as<TypeParam, double>)
		{
			ASSERT_LE(value - newValue, static_cast<TypeParam>(0.001));
		}
		else 
		{
			ASSERT_EQ(value, newValue);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(ScalarFieldMarshalTest, FieldTest);

	using FieldTypes = ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, bool, FieldDummyStruct>;
	INSTANTIATE_TYPED_TEST_SUITE_P(Member, ScalarFieldMarshalTest, FieldTypes);

	template <>
	FieldData<int8_t> ScalarFieldMarshalTest<int8_t>::s_FieldData 
	{
		"SByteFieldTest", { 10, 20 }
	};
	template <>
	FieldData<uint8_t> ScalarFieldMarshalTest<uint8_t>::s_FieldData {
		"ByteFieldTest", { 10, 20 }
	};

	template <>
	FieldData<int16_t> ScalarFieldMarshalTest<int16_t>::s_FieldData {
		"ShortFieldTest", { 10, 20 }
	};
	template <>
	FieldData<uint16_t> ScalarFieldMarshalTest<uint16_t>::s_FieldData {
		"UShortFieldTest", { 10, 20 }
	};

	template <>
	FieldData<int32_t> ScalarFieldMarshalTest<int32_t>::s_FieldData {
		"IntFieldTest", { 10, 20 }
	};
	template <>
	FieldData<uint32_t> ScalarFieldMarshalTest<uint32_t>::s_FieldData {
		"UIntFieldTest", { 10, 20 }
	};

	template <>
	FieldData<int64_t> ScalarFieldMarshalTest<int64_t>::s_FieldData {
		"LongFieldTest", { 10, 20 }
	};
	template <>
	FieldData<uint64_t> ScalarFieldMarshalTest<uint64_t>::s_FieldData {
		"ULongFieldTest", { 10, 20 }
	};

	template <>
	FieldData<float> ScalarFieldMarshalTest<float>::s_FieldData 
	{
		"FloatFieldTest", { 10.0f, 20.0f }
	};
	template <>
	FieldData<double> ScalarFieldMarshalTest<double>::s_FieldData {
		"DoubleFieldTest", { 10.0, 20.0 }
	};

	template <>
	FieldData<bool> ScalarFieldMarshalTest<bool>::s_FieldData {
		"BoolFieldTest", { false, true }
	};

	template <>
	FieldData<FieldDummyStruct> ScalarFieldMarshalTest<FieldDummyStruct>::s_FieldData {
		"DummyStructFieldTest", { { 0.0f }, { 50.0f } }
	};
}
