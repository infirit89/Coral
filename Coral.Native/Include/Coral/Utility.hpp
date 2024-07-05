#pragma once

#include "Core.hpp"
#include "String.hpp"

namespace Coral {

	enum class ManagedType
	{
		Unknown = 0,

		SByte,
		Byte,
		Short,
		UShort,
		Int,
		UInt,
		Long,
		ULong,

		Float,
		Double,

		Bool,
		Char,

		String,
		Pointer
	};

	template<typename TArg>
	constexpr ManagedType GetManagedType()
	{
		if constexpr (std::is_pointer_v<std::remove_reference_t<TArg>>)
			return ManagedType::Pointer;
		else if constexpr (std::same_as<TArg, uint8_t> || std::same_as<TArg, std::byte>)
			return ManagedType::Byte;
		else if constexpr (std::same_as<TArg, uint16_t>)
			return ManagedType::UShort;
		else if constexpr (std::same_as<TArg, uint32_t> || (std::same_as<TArg, unsigned long> && sizeof(TArg) == 4))
			return ManagedType::UInt;
		else if constexpr (std::same_as<TArg, uint64_t> || (std::same_as<TArg, unsigned long> && sizeof(TArg) == 8))
			return ManagedType::ULong;
		else if constexpr (std::same_as<TArg, char8_t> || std::same_as<TArg, int8_t>)
			return ManagedType::SByte;
		else if constexpr (std::same_as<TArg, int16_t>)
			return ManagedType::Short;
		else if constexpr (std::same_as<TArg, int32_t> || (std::same_as<TArg, long> && sizeof(TArg) == 4))
			return ManagedType::Int;
		else if constexpr (std::same_as<TArg, int64_t> || (std::same_as<TArg, long> && sizeof(TArg) == 8))
			return ManagedType::Long;
		else if constexpr (std::same_as<TArg, float>)
			return ManagedType::Float;
		else if constexpr (std::same_as<TArg, double>)
			return ManagedType::Double;
		else if constexpr (std::same_as<TArg, bool>)
			return ManagedType::Bool;
		else if constexpr (std::same_as<TArg, Coral::String> || std::same_as<TArg, Coral::ScopedString>)
			return ManagedType::String;
		else
			return ManagedType::Unknown;
	}

	template <typename TArg, size_t TIndex>
	inline void AddToArrayI(const void** InArgumentsArr, ManagedType* InParameterTypes, TArg&& InArg)
	{
		InParameterTypes[TIndex] = GetManagedType<std::remove_reference_t<TArg>>();

		if constexpr (std::is_pointer_v<std::remove_reference_t<TArg>>)
		{
			InArgumentsArr[TIndex] = reinterpret_cast<const void*>(InArg);
		}
		else
		{
			InArgumentsArr[TIndex] = reinterpret_cast<const void*>(&InArg);
		}
	}

	template<typename TArg, size_t TIndex>
	inline void AddToTypeArrayI(ManagedType* InParameterTypes)
	{
		InParameterTypes[TIndex] = GetManagedType<std::remove_reference_t<TArg>>();
	}

	template<typename TArg, size_t TIndex>
	inline void AddToValueArrayI(const void** InArgumentsArr, TArg&& InArg)
	{
		if constexpr (std::is_pointer_v<std::remove_reference_t<TArg>>)
		{
			InArgumentsArr[TIndex] = reinterpret_cast<const void*>(InArg);
		}
		else
		{
			InArgumentsArr[TIndex] = reinterpret_cast<const void*>(&InArg);
		}
	}

	template <typename... TArgs, size_t... TIndices>
	inline void AddToArray(const void** InArgumentsArr, ManagedType* InParameterTypes, TArgs&&... InArgs, const std::index_sequence<TIndices...>&)
	{
		(AddToArrayI<TArgs, TIndices>(InArgumentsArr, InParameterTypes, std::forward<TArgs>(InArgs)), ...);
	}

	template<typename... TArgs, size_t... TIndices>
	inline void AddToTypeArray(ManagedType* InParameterTypes, const std::index_sequence<TIndices...>&)
	{
		(AddToTypeArrayI<TArgs, TIndices>(InParameterTypes), ...);
	}

	template<typename... TArgs, size_t... TIndices>
	inline void AddToValueArray(const void** InArgumentsArr, TArgs&&... InArgs, const std::index_sequence<TIndices...>&)
	{
		(AddToValueArrayI<TArgs, TIndices>(InArgumentsArr, std::forward<TArgs>(InArgs)), ...);
	}
}
