#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <functional>
#include <ranges>

#include <Coral/Assembly.hpp>
#include <Coral/ManagedObject.hpp>
#include <Coral/String.hpp>
#include <Coral/Array.hpp>

#include "Globals.h"

namespace Testing
{
	class InternalCalls
	{
	public:
		static void Register(Coral::ManagedAssembly& assembly);
	private:
		static Coral::Type s_InternalCallTestType;
		static char8_t SByteMarshalIcall(char8_t InValue) { return InValue * 2; }
		static uint8_t ByteMarshalIcall(uint8_t InValue) { return InValue * 2; }
		static int16_t ShortMarshalIcall(int16_t InValue) { return InValue * 2; }
		static uint16_t UShortMarshalIcall(uint16_t InValue) { return InValue * 2; }
		static int32_t IntMarshalIcall(int32_t InValue) { return InValue * 2; }
		static uint32_t UIntMarshalIcall(uint32_t InValue) { return InValue * 2; }
		static int64_t LongMarshalIcall(int64_t InValue) { return InValue * 2; }
		static uint64_t ULongMarshalIcall(uint64_t InValue) { return InValue * 2; }
		static float FloatMarshalIcall(float InValue) { return InValue * 2.0f; }
		static double DoubleMarshalIcall(double InValue) { return InValue * 2.0; }
		static bool BoolMarshalIcall(bool InValue) { return !InValue; }
		static int32_t* IntPtrMarshalIcall(int32_t* InValue)
		{
			*InValue *= 2;
			return InValue;
		}

		static Coral::String StringMarshalIcall(Coral::String InStr)
		{
			return InStr;
		}

		static void StringMarshalIcall2(Coral::String InStr)
		{
			std::cout << std::string(InStr) << std::endl;
		}

		static bool TypeMarshalIcall(Coral::ReflectionType InReflectionType)
		{
			Coral::Type& type = InReflectionType;
			return type == s_InternalCallTestType;
		}

		struct DummyStruct
		{
			int32_t X;
			float Y;
			int32_t Z;
		};
		static DummyStruct DummyStructMarshalIcall(DummyStruct InStruct)
		{
			InStruct.X *= 2;
			InStruct.Y *= 2.0f;
			InStruct.Z *= 2;
			return InStruct;
		}
		static DummyStruct* DummyStructPtrMarshalIcall(DummyStruct* InStruct)
		{
			InStruct->X *= 2;
			InStruct->Y *= 2.0f;
			InStruct->Z *= 2;
			return InStruct;
		}

		static Coral::Array<int32_t> EmptyArrayIcall()
		{
			std::vector<int32_t> empty;
			return Coral::Array<int32_t>::New(empty);
		}

		static Coral::Array<float> FloatArrayIcall()
		{
			std::vector<float> floats = { 5.0f, 10.0f, 15.0f, 50.0f };
			return Coral::Array<float>::New(floats);
		}

		static Coral::ManagedObject NativeInstanceIcall()
		{
			return Globals::TestInstance;
		}

	};
}
