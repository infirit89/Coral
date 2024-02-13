#include "CoralPCH.hpp"
#include "ManagedArray.hpp"
#include "CoralManagedFunctions.hpp"
#include "Type.hpp"
#include "TypeCache.hpp"

namespace Coral {
	ManagedArray ManagedArray::New(Type& InElementType, int32_t InLength)
	{
		constexpr size_t lengthsSize = 1;
		const int32_t lengths[lengthsSize] = { InLength };
		return ManagedArray::New(InElementType, lengths, lengthsSize);
	}
	ManagedArray ManagedArray::New(Type& InElementType, const int32_t* InLengths, size_t InLengthsSize)
	{
		ManagedArray array;
		s_ManagedFunctions.CreateArrayFptr(InElementType.GetTypeId(), InLengths, InLengthsSize, &array);
		return array;
	}

	void ManagedArray::Destroy()
	{
		if (!m_Handle)
			return;

		s_ManagedFunctions.DestroyObjectFptr(m_Handle);
		m_Handle = nullptr;
		m_Type = nullptr;
	}

	int32_t ManagedArray::GetLength(int32_t InDimension) 
	{
		return s_ManagedFunctions.GetArrayLengthFptr(m_Handle, InDimension);
	}

	void* ManagedArray::Data()
	{
		return s_ManagedFunctions.GetArrayDataReferenceFptr(m_Handle);
	}

	void ManagedArray::Resize(int32_t InNewSize)
	{
		if (InNewSize < 0 || m_Rank > 1)
			return;

		Coral::ManagedArray temp = Coral::ManagedArray::New(GetType().GetElementType(), InNewSize);
		memcpy(temp.Data(), Data(), std::min(InNewSize, GetLength(0)) * GetType().GetElementType().GetSize());
		*this = temp;
	}

	void ManagedArray::GetValueRaw(int32_t InIndex, void* OutValue)
	{
		constexpr size_t indicesSize = 1;
		const int32_t indices[indicesSize] = { InIndex };
		GetValueRaw(indices, indicesSize, OutValue);
	}

	void ManagedArray::GetValueRaw(const int32_t* InIndices, size_t InIndicesSize, void* OutValue)
	{
		s_ManagedFunctions.GetArrayValueFptr(m_Handle, InIndices, InIndicesSize, OutValue);
	}

	void ManagedArray::SetValueRaw(int32_t InIndex, void* InValue)
	{
		constexpr size_t indicesSize = 1;
		const int32_t indices[indicesSize] = { InIndex };
		SetValueRaw(indices, indicesSize, InValue);
	}

	void ManagedArray::SetValueRaw(const int32_t* InIndices, size_t InIndicesSize, void* InValue)
	{
		s_ManagedFunctions.SetArrayValueFptr(m_Handle, InIndices, InIndicesSize, InValue);
	}

	Type& ManagedArray::GetType()
	{
		if (!m_Type)
		{
			Type type;
			s_ManagedFunctions.GetObjectTypeIdFptr(m_Handle, &type.m_Id);
			m_Type = TypeCache::Get().CacheType(std::move(type));
		}

		return *m_Type;
	}
}
