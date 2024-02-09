#pragma once

namespace Coral {

	class Type;

	class ManagedArray
	{
	public:
		static ManagedArray New(Type& InElementType, int32_t InLength);
		static ManagedArray New(Type& InElementType, const int32_t* InLengths, size_t InLengthsSize);

		int32_t GetLength(int32_t InDimension);

		void* Data();

		// NOTE: only for sz arrays
		void Resize(int32_t InNewSize);

		template<typename TReturn>
		TReturn GetValue(int32_t InIndex)
		{
			TReturn value;
			GetValueRaw(InIndex, &value);
			return value;
		}

		template <typename TReturn>
		TReturn GetValue(const int32_t* InIndices, int32_t InIndicesSize)
		{
			TReturn value;
			GetValueRaw(InIndices, InIndicesSize, &value);
			return value;
		}

		template<typename TValue>
		void SetValue(int32_t InIndex, TValue InValue)
		{
			SetValueRaw(InIndex, &InValue);
		}

		template<typename TValue>
		void SetValue(const int32_t* InIndices, int32_t InIndicesSize, TValue InValue)
		{
			SetValueRaw(InIndices, InIndicesSize, &InValue);
		}

		void GetValueRaw(int32_t InIndex, void* OutValue);
		void GetValueRaw(const int32_t* InIndices, size_t InIndicesSize, void* OutValue);

		void SetValueRaw(int32_t InIndex, void* InValue);
		void SetValueRaw(const int32_t* InIndices, size_t InIndicesSize, void* InValue);

		Type& GetType();
	private:
		void* m_Handle = nullptr;
		int32_t m_Rank = 0;
		Type* m_Type = nullptr;
	};
}
