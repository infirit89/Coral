#pragma once

#include "Core.hpp"
#include "Utility.hpp"
#include "MethodInfo.hpp"

namespace Coral {

	class ManagedAssembly;
	class Type;

	class ManagedObject
	{
	public:
		ManagedObject() = default;
		ManagedObject(const void* handle)
		    : m_Handle((void*)handle) { }

	public:
		template<typename TReturn, typename... TArgs>
		TReturn InvokeMethod(std::string_view InMethodName, TArgs&&... InParameters) const
		{
			constexpr size_t parameterCount = sizeof...(InParameters);

			TReturn result;
			
			if constexpr (parameterCount > 0)
			{
				const void* parameterValues[parameterCount];
				ManagedType parameterTypes[parameterCount];
				AddToArray<TArgs...>(parameterValues, parameterTypes, std::forward<TArgs>(InParameters)..., std::make_index_sequence<parameterCount> {});
				InvokeMethodRetInternal(InMethodName, parameterValues, parameterTypes, parameterCount, &result);
			}
			else
			{
				InvokeMethodRetInternal(InMethodName, nullptr, nullptr, 0, &result);
			}

			return result;
		}

		template<typename... TArgs>
		void InvokeMethod(std::string_view InMethodName, TArgs&&... InParameters) const
		{
			constexpr size_t parameterCount = sizeof...(InParameters);

			if constexpr (parameterCount > 0)
			{
				const void* parameterValues[parameterCount];
				ManagedType parameterTypes[parameterCount];
				AddToArray<TArgs...>(parameterValues, parameterTypes, std::forward<TArgs>(InParameters)..., std::make_index_sequence<parameterCount> {});
				InvokeMethodInternal(InMethodName, parameterValues, parameterTypes, parameterCount);
			}
			else
			{
				InvokeMethodInternal(InMethodName, nullptr, nullptr, 0);
			}
		}

		template<typename... TArgs>
		void InvokeMethodByMethodInfo(const MethodInfo& InMethodInfo, TArgs&&... InParameters) const
		{
			constexpr size_t parameterCount = sizeof...(InParameters);
			if constexpr (parameterCount > 0) 
			{
				const void* parameterValues[parameterCount];
				AddToValueArray<TArgs...>(parameterValues, std::forward<TArgs>(InParameters)..., std::make_index_sequence<parameterCount> {});
				InvokeMethodByMethodInfoInternal(InMethodInfo, parameterValues, parameterCount);
			}
			else 
			{
				InvokeMethodByMethodInfoInternal(InMethodInfo, nullptr, 0);
			}
		}

		template <typename... TArgs>
		void InvokeMethodByMethodInfoWithUnwrappedExceptions(const MethodInfo& InMethodInfo, TArgs&&... InParameters) const
		{
			constexpr size_t parameterCount = sizeof...(InParameters);
			if constexpr (parameterCount > 0)
			{
				const void* parameterValues[parameterCount];
				AddToValueArray<TArgs...>(parameterValues, std::forward<TArgs>(InParameters)..., std::make_index_sequence<parameterCount> {});
				InvokeMethodByMethodInfoInternal(InMethodInfo, parameterValues, parameterCount, false);
			}
			else
			{
				InvokeMethodByMethodInfoInternal(InMethodInfo, nullptr, 0, false);
			}
		}

		template<typename TValue>
		void SetFieldValue(std::string_view InFieldName, TValue InValue) const
		{
			SetFieldValueRaw(InFieldName, &InValue);
		}

		template<typename TValue>
		void SetFieldValueByHandle(ManagedHandle InFieldHandle, TValue InValue) const
		{
			SetFieldValueByHandleRaw(InFieldHandle, &InValue);
		}

		template<typename TReturn>
		TReturn GetFieldValue(std::string_view InFieldName) const
		{
			TReturn result;
			GetFieldValueRaw(InFieldName, &result);
			return result;
		}

		template<typename TReturn>
		TReturn GetFieldValueByHandle(ManagedHandle InFieldHandle) const
		{
			TReturn result;
			GetFieldValueByHandleRaw(InFieldHandle, &result);
			return result;
		}

		template<typename TValue>
		void SetPropertyValue(std::string_view InPropertyName, TValue InValue) const
		{
			SetPropertyValueRaw(InPropertyName, &InValue);
		}

		template<typename TReturn>
		TReturn GetPropertyValue(std::string_view InPropertyName) const
		{
			TReturn result;
			GetPropertyValueRaw(InPropertyName, &result);
			return result;
		}

		void SetFieldValueRaw(std::string_view InFieldName, void* InValue) const;
		void SetFieldValueByHandleRaw(ManagedHandle InFieldHandle, void* InValue) const;
		void GetFieldValueRaw(std::string_view InFieldName, void* OutValue) const;
		void GetFieldValueByHandleRaw(ManagedHandle InFieldHandle, void* OutValue) const;
		void SetPropertyValueRaw(std::string_view InPropertyName, void* InValue) const;
		void GetPropertyValueRaw(std::string_view InPropertyName, void* OutValue) const;

		const Type& GetType();
		
		void Destroy();

		bool IsValid() const { return m_Handle != nullptr && m_Type != nullptr; }

		const void* GetHandle() const { return m_Handle; }

	private:
		void InvokeMethodInternal(std::string_view InMethodName, const void** InParameters, const ManagedType* InParameterTypes, size_t InLength) const;
		void InvokeMethodRetInternal(std::string_view InMethodName, const void** InParameters, const ManagedType* InParameterTypes, size_t InLength, void* InResultStorage) const;
		void InvokeMethodByMethodInfoInternal(const MethodInfo& InMethodInfo, const void** InParameters, size_t InLength, bool wrapExceptions = true) const;

	private:
		void* m_Handle = nullptr;
		const Type* m_Type = nullptr;

	private:
		friend class ManagedAssembly;
		friend class Type;
	};
	
}

