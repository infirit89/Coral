#pragma once

#include "Core.hpp"
#include "String.hpp"

namespace Coral {

	class Type;
	class Attribute;

	class FieldInfo
	{
	public:
		FieldInfo() = default;
		FieldInfo(ManagedHandle handle)
		    : m_Handle(handle) { }

		String GetName() const;
		Type& GetType();

		TypeAccessibility GetAccessibility() const;

		std::vector<Attribute> GetAttributes() const;
		bool HasAttribute(const Type& InAttributeType) const;

		ManagedHandle GetHandle() const { return m_Handle; }
		operator ManagedHandle() const { return m_Handle; }

	private:
		ManagedHandle m_Handle = -1;
		Type* m_Type = nullptr;

		friend class Type;
	};	
}
