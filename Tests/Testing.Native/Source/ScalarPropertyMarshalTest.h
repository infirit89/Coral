#pragma once

#include "PropertyMarshalTestBase.h"

namespace Testing
{
	struct PropertyDummyStruct
	{
		bool operator==(const PropertyDummyStruct& other) const
		{
			return X == other.X && Y == other.Y;
		}

		float X;
		int Y;
	};

	template<typename T>
	using PropertyData = std::tuple<std::string_view, std::tuple<T, T>>;

	template<typename T>
	class ScalarPropertyMarshalTest : public PropertyMarshalTestBase
	{
	protected:
		ScalarPropertyMarshalTest() {}
		~ScalarPropertyMarshalTest() override {}

		static PropertyData<T> m_PropertyData;
	};

	TYPED_TEST_SUITE_P(ScalarPropertyMarshalTest);

	TYPED_TEST_P(ScalarPropertyMarshalTest, PropertyTest) 
	{
		auto& fieldData = ScalarPropertyMarshalTest<TypeParam>::m_PropertyData;
		std::string_view fieldName = std::get<0>(fieldData);
		auto fieldValues = std::get<1>(fieldData);
		TypeParam oldValue = std::get<0>(fieldValues);
		TypeParam newValue = std::get<1>(fieldValues);

		auto value = this->m_PropertyTestObject.GetPropertyValue<TypeParam>(fieldName);
		if constexpr (std::same_as<TypeParam, float> || std::same_as<TypeParam, double>)
		{
			ASSERT_LE(value - oldValue, static_cast<TypeParam>(0.001));
		}
		else
		{
			ASSERT_EQ(value, oldValue);
		}
		this->m_PropertyTestObject.SetPropertyValue<TypeParam>(fieldName, newValue);
		value = this->m_PropertyTestObject.GetPropertyValue<TypeParam>(fieldName);
		if constexpr (std::same_as<TypeParam, float> || std::same_as<TypeParam, double>)
		{
			ASSERT_LE(value - newValue, static_cast<TypeParam>(0.001));
		}
		else
		{
			ASSERT_EQ(value, newValue);
		}
	}

	REGISTER_TYPED_TEST_SUITE_P(ScalarPropertyMarshalTest, PropertyTest);
	using PropertyTypes = ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, bool, PropertyDummyStruct>;
	INSTANTIATE_TYPED_TEST_SUITE_P(Member, ScalarPropertyMarshalTest, PropertyTypes);

	template <>
	PropertyData<int8_t> ScalarPropertyMarshalTest<int8_t>::m_PropertyData {
		"SBytePropertyTest", { 10, 20 }
	};
	template <>
	PropertyData<uint8_t> ScalarPropertyMarshalTest<uint8_t>::m_PropertyData {
		"BytePropertyTest", { 10, 20 }
	};

	template <>
	PropertyData<int16_t> ScalarPropertyMarshalTest<int16_t>::m_PropertyData {
		"ShortPropertyTest", { 10, 20 }
	};
	template <>
	PropertyData<uint16_t> ScalarPropertyMarshalTest<uint16_t>::m_PropertyData {
		"UShortPropertyTest", { 10, 20 }
	};

	template <>
	PropertyData<int32_t> ScalarPropertyMarshalTest<int32_t>::m_PropertyData {
		"IntPropertyTest", { 10, 20 }
	};
	template <>
	PropertyData<uint32_t> ScalarPropertyMarshalTest<uint32_t>::m_PropertyData {
		"UIntPropertyTest", { 10, 20 }
	};

	template <>
	PropertyData<int64_t> ScalarPropertyMarshalTest<int64_t>::m_PropertyData {
		"LongPropertyTest", { 10, 20 }
	};
	template <>
	PropertyData<uint64_t> ScalarPropertyMarshalTest<uint64_t>::m_PropertyData {
		"ULongPropertyTest", { 10, 20 }
	};

	template <>
	PropertyData<float> ScalarPropertyMarshalTest<float>::m_PropertyData {
		"FloatPropertyTest", { 10.0f, 20.0f }
	};
	template <>
	PropertyData<double> ScalarPropertyMarshalTest<double>::m_PropertyData {
		"DoublePropertyTest", { 10.0, 20.0 }
	};

	template <>
	PropertyData<bool> ScalarPropertyMarshalTest<bool>::m_PropertyData {
		"BoolPropertyTest", { false, true }
	};

	template <>
	PropertyData<PropertyDummyStruct> ScalarPropertyMarshalTest<PropertyDummyStruct>::m_PropertyData {
		"DummyStructPropertyTest", { { 0.0f, 0 }, { 50.0f, 20 } }
	};
}
