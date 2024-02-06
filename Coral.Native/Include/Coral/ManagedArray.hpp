#pragma once

namespace Coral {

	class Type;

	class ManagedArray
	{
	public:
		static ManagedArray New(Type& InElementType, const int32_t* InLengths, size_t InLengthsSize);

		int32_t GetLength(int32_t InDimension);
	private:
		void* m_Handle = nullptr;
		int32_t m_Rank = 0;
		Type* m_ElementType = nullptr;
	};
}
