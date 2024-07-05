#pragma once

#include "MethodMarshalTestBase.hpp"

namespace Testing {

	template <typename T>
	using MethodData = std::tuple<std::string_view, std::tuple<T, T>>;

	template <typename T>
	class ScalarMethodMarshalTest : public MethodMarshalTestBase
	{
	protected:
		ScalarMethodMarshalTest() = default;
		~ScalarMethodMarshalTest() override = default;

		static MethodData<T> s_MethodData;
	};

	TYPED_TEST_SUITE_P(ScalarMethodMarshalTest);

	TYPED_TEST_P(ScalarMethodMarshalTest, MethodTest)
	{
		auto& methodData = ScalarMethodMarshalTest<TypeParam>::s_MethodData;
		std::string_view methodName = std::get<0>(methodData);
		auto methodValues = std::get<1>(methodData);
		TypeParam value1 = std::get<0>(methodValues);
		TypeParam value2 = std::get<1>(methodValues);
		
		auto value = this->m_MethodTestObject.InvokeMethod<TypeParam, TypeParam&>(methodName, value1);
		ASSERT_EQ(value, value2);
	}

	REGISTER_TYPED_TEST_SUITE_P(ScalarMethodMarshalTest, MethodTest);

	using MethodTypes = ::testing::Types<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, float, double, bool, MethodDummyStruct>;
	INSTANTIATE_TYPED_TEST_SUITE_P(Member, ScalarMethodMarshalTest, MethodTypes);

	template <>
	MethodData<int8_t> ScalarMethodMarshalTest<int8_t>::s_MethodData {
		"SByteMethodTest", { 10, 20 }
	};
	template <>
	MethodData<uint8_t> ScalarMethodMarshalTest<uint8_t>::s_MethodData {
		"ByteMethodTest", { 10, 20 }
	};

	template <>
	MethodData<int16_t> ScalarMethodMarshalTest<int16_t>::s_MethodData {
		"ShortMethodTest", { 10, 20 }
	};
	template <>
	MethodData<uint16_t> ScalarMethodMarshalTest<uint16_t>::s_MethodData {
		"UShortMethodTest", { 10, 20 }
	};

	template <>
	MethodData<int32_t> ScalarMethodMarshalTest<int32_t>::s_MethodData {
		"IntMethodTest", { 10, 20 }
	};
	template <>
	MethodData<uint32_t> ScalarMethodMarshalTest<uint32_t>::s_MethodData {
		"UIntMethodTest", { 10, 20 }
	};

	template <>
	MethodData<int64_t> ScalarMethodMarshalTest<int64_t>::s_MethodData {
		"LongMethodTest", { 10, 20 }
	};
	template <>
	MethodData<uint64_t> ScalarMethodMarshalTest<uint64_t>::s_MethodData {
		"ULongMethodTest", { 10, 20 }
	};

	template <>
	MethodData<float> ScalarMethodMarshalTest<float>::s_MethodData {
		"FloatMethodTest", { 10.0f, 20.0f }
	};
	template <>
	MethodData<double> ScalarMethodMarshalTest<double>::s_MethodData {
		"DoubleMethodTest", { 10.0, 20.0 }
	};

	template <>
	MethodData<bool> ScalarMethodMarshalTest<bool>::s_MethodData {
		"BoolMethodTest", { false, true }
	};

	template <>
	MethodData<MethodDummyStruct> ScalarMethodMarshalTest<MethodDummyStruct>::s_MethodData {
		"DummyStructMethodTest", { { 10, 10.0f, 10 }, { 20, 20.0f, 20 } }
	};
}
