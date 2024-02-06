#include "CoralPCH.hpp"
#include "ManagedArray.hpp"
#include "CoralManagedFunctions.hpp"
#include "Type.hpp"

namespace Coral {

	ManagedArray ManagedArray::New(Type& InElementType, const int32_t* InLengths, size_t InLengthsSize)
	{
		ManagedArray array;
		s_ManagedFunctions.CreateArrayFptr(InElementType.GetTypeId(), InLengths, InLengthsSize, &array);
		return array;
	}

	int32_t ManagedArray::GetLength(int32_t InDimension) 
	{
		return s_ManagedFunctions.GetArrayLengthFptr(m_Handle, InDimension);
	}
}

