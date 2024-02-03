#pragma once

#include "Core.hpp"
#include "String.hpp"

namespace Coral {

	class Type;
	class Attribute;

	class MethodInfo
	{
	public:
		MethodInfo() = default;
		MethodInfo(ManagedHandle handle) 
		 : m_Handle(handle) { }

		String GetName() const;

		Type& GetReturnType();
		const std::vector<Type*>& GetParameterTypes();

		TypeAccessibility GetAccessibility() const;

		std::vector<Attribute> GetAttributes() const;
		ManagedHandle GetHandle() const { return m_Handle; }

	private:
		ManagedHandle m_Handle = -1;
		Type* m_ReturnType = nullptr;
		std::vector<Type*> m_ParameterTypes;

		friend class Type;
		friend class ManagedObject;
	};
}
