#pragma once

#include "Core.hpp"
#include "String.hpp"

namespace Coral {

	struct UnmanagedArray;
	enum class AssemblyLoadStatus;
	class ManagedObject;
	enum class GCCollectionMode;
	enum class ManagedType;
	class ManagedField;
	class ManagedArray;

	using SetInternalCallsFn = void (*)(void*, int32_t);
	using CreateAssemblyLoadContextFn = int32_t (*)(String);
	using UnloadAssemblyLoadContextFn = void (*)(int32_t);
	using LoadAssemblyFn = int32_t (*)(int32_t, String);
	using GetLastLoadStatusFn = AssemblyLoadStatus (*)();
	using GetAssemblyNameFn = String (*)(int32_t);

#pragma region TypeInterface

	using GetAssemblyTypesFn = void (*)(int32_t, TypeId*, int32_t*);
	using GetSystemTypesFn = void (*)(TypeId*, int32_t*);
	using GetTypeIdFn = void (*)(String, TypeId*);
	using GetFullTypeNameFn = String (*)(TypeId);
	using GetAssemblyQualifiedNameFn = String (*)(TypeId);
	using GetBaseTypeFn = void (*)(TypeId, TypeId*);
	using GetTypeSizeFn = int32_t (*)(TypeId);
	using IsTypeSubclassOfFn = Bool32 (*)(TypeId, TypeId);
	using IsTypeAssignableToFn = Bool32 (*)(TypeId, TypeId);
	using IsTypeAssignableFromFn = Bool32 (*)(TypeId, TypeId);
	using IsTypeSZArrayFn = Bool32 (*)(TypeId);
	using IsTypeArrayFn = Bool32 (*)(TypeId);
	using GetElementTypeFn = void (*)(TypeId, TypeId*);
	using GetTypeMethodsFn = void (*)(TypeId, ManagedHandle*, int32_t*);
	using GetTypeFieldsFn = void (*)(TypeId, ManagedHandle*, int32_t*);
	using GetTypePropertiesFn = void (*)(TypeId, ManagedHandle*, int32_t*);
	using HasTypeAttributeFn = Bool32 (*)(TypeId, TypeId);
	using GetTypeAttributesFn = void (*)(ManagedHandle, TypeId*, int32_t*);
	using GetTypeAttributeFn = void (*)(TypeId, TypeId, ManagedHandle*);
	using GetTypeManagedTypeFn = ManagedType (*)(TypeId);
	using GetTypeMethodFn = ManagedHandle (*)(TypeId, String, const ManagedType*, int32_t);
	using GetTypeField = ManagedHandle (*)(TypeId, String);

#pragma endregion

#pragma region MethodInfo
	using GetMethodInfoNameFn = String (*)(ManagedHandle);
	using GetMethodInfoReturnTypeFn = void (*)(ManagedHandle, TypeId*);
	using GetMethodInfoParameterTypesFn = void (*)(ManagedHandle, TypeId*, int32_t*);
	using GetMethodInfoAccessibilityFn = TypeAccessibility (*)(ManagedHandle);
	using GetMethodInfoAttributesFn = void (*)(ManagedHandle, TypeId*, int32_t*);
#pragma endregion

#pragma region FieldInfo
	using GetFieldInfoNameFn = String (*)(ManagedHandle);
	using GetFieldInfoTypeFn = void (*)(ManagedHandle, TypeId*);
	using GetFieldInfoAccessibilityFn = TypeAccessibility (*)(ManagedHandle);
	using GetFieldInfoAttributesFn = void (*)(ManagedHandle, TypeId*, int32_t*);
	using HasFieldInfoAttributeFn = Bool32 (*)(ManagedHandle, TypeId);
#pragma endregion

#pragma region PropertyInfo
	using GetPropertyInfoNameFn = String (*)(ManagedHandle);
	using GetPropertyInfoTypeFn = void (*)(ManagedHandle, TypeId*);
	using GetPropertyInfoAttributesFn = void (*)(ManagedHandle, TypeId*, int32_t*);
#pragma endregion

#pragma region Attribute
	using GetAttributeFieldValueFn = void (*)(ManagedHandle, String, void*);
	using GetAttributeTypeFn = void (*)(ManagedHandle, TypeId*);
#pragma endregion

#pragma region ManagedArray
	using CreatedArrayFn = void (*)(TypeId, const int32_t*, int32_t, ManagedArray*);
	using GetArrayLengthFn = int (*)(void*, int32_t);
	using GetArrayValueFn = void (*)(void*, const int32_t*, int32_t, void*);
	using SetArrayValueFn = void (*)(void*, const int32_t*, int32_t, void*);
	using GetArrayDataReferenceFn = void* (*)(void*);
	using ResizeRankOneArrayFn = void (*)(int32_t, ManagedArray*);
	using ResizeRankNArrayFn = void (*)(const int32_t*, int32_t, ManagedArray*);
#pragma endregion

	using CreateObjectFn = void* (*)(TypeId, Bool32, const void**, const ManagedType*, int32_t);
	using InvokeMethodFn = void (*)(void*, String, const void**, const ManagedType*, int32_t);
	using InvokeMethodRetFn = void (*)(void*, String, const void**, const ManagedType*, int32_t, void*);
	using InvokeMethodByMethodInfoFn = void (*)(void*, ManagedHandle, int32_t, const void**, bool);
	using InvokeStaticMethodFn = void (*)(TypeId, String, const void**, const ManagedType*, int32_t);
	using InvokeStaticMethodRetFn = void (*)(TypeId, String, const void**, const ManagedType*, int32_t, void*);
	using SetFieldValueFn = void (*)(void*, String, void*);
	using SetFieldValueByFieldInfoFn = void (*)(void*, int, void*);
	using GetFieldValueFn = void (*)(void*, String, void*);
	using GetFieldValueByFieldInfoFn = void (*)(void*, int, void*);
	using GetStaticFieldValueFn = void (*)(TypeId, String, void*);
	using SetPropertyValueFn = void (*)(void*, String, void*);
	using GetPropertyValueFn = void (*)(void*, String, void*);
	using DestroyObjectFn = void (*)(void*);
	using GetObjectTypeIdFn = void (*)(void*, int32_t*);

	using CollectGarbageFn = void (*)(int32_t, GCCollectionMode, Bool32, Bool32);
	using WaitForPendingFinalizersFn = void (*)();

	struct ManagedFunctions
	{
		SetInternalCallsFn SetInternalCallsFptr = nullptr;
		LoadAssemblyFn LoadAssemblyFptr = nullptr;
		UnloadAssemblyLoadContextFn UnloadAssemblyLoadContextFptr = nullptr;
		GetLastLoadStatusFn GetLastLoadStatusFptr = nullptr;
		GetAssemblyNameFn GetAssemblyNameFptr = nullptr;

#pragma region TypeInterface

		GetAssemblyTypesFn GetAssemblyTypesFptr = nullptr;
		GetSystemTypesFn GetSystemTypesFptr = nullptr;
		GetTypeIdFn GetTypeIdFptr = nullptr;
		GetFullTypeNameFn GetFullTypeNameFptr = nullptr;
		GetAssemblyQualifiedNameFn GetAssemblyQualifiedNameFptr = nullptr;
		GetBaseTypeFn GetBaseTypeFptr = nullptr;
		GetTypeSizeFn GetTypeSizeFptr = nullptr;
		IsTypeSubclassOfFn IsTypeSubclassOfFptr = nullptr;
		IsTypeAssignableToFn IsTypeAssignableToFptr = nullptr;
		IsTypeAssignableFromFn IsTypeAssignableFromFptr = nullptr;
		IsTypeSZArrayFn IsTypeSZArrayFptr = nullptr;
		IsTypeArrayFn IsTypeArrayFptr = nullptr;
		GetElementTypeFn GetElementTypeFptr = nullptr;
		GetTypeMethodsFn GetTypeMethodsFptr = nullptr;
		GetTypeFieldsFn GetTypeFieldsFptr = nullptr;
		GetTypePropertiesFn GetTypePropertiesFptr = nullptr;
		HasTypeAttributeFn HasTypeAttributeFptr = nullptr;
		GetTypeAttributesFn GetTypeAttributesFptr = nullptr;
		GetTypeAttributeFn GetTypeAttributeFptr = nullptr;
		GetTypeManagedTypeFn GetTypeManagedTypeFptr = nullptr;
		GetTypeMethodFn GetTypeMethodFptr = nullptr;
		GetTypeField GetTypeFieldFptr = nullptr;

#pragma endregion

#pragma region MethodInfo
		GetMethodInfoNameFn GetMethodInfoNameFptr = nullptr;
		GetMethodInfoReturnTypeFn GetMethodInfoReturnTypeFptr = nullptr;
		GetMethodInfoParameterTypesFn GetMethodInfoParameterTypesFptr = nullptr;
		GetMethodInfoAccessibilityFn GetMethodInfoAccessibilityFptr = nullptr;
		GetMethodInfoAttributesFn GetMethodInfoAttributesFptr = nullptr;
#pragma endregion

#pragma region FieldInfo
		GetFieldInfoNameFn GetFieldInfoNameFptr = nullptr;
		GetFieldInfoTypeFn GetFieldInfoTypeFptr = nullptr;
		GetFieldInfoAccessibilityFn GetFieldInfoAccessibilityFptr = nullptr;
		GetFieldInfoAttributesFn GetFieldInfoAttributesFptr = nullptr;
		HasFieldInfoAttributeFn HasFieldInfoAttributeFptr = nullptr;
#pragma endregion

#pragma region PropertyInfo
		GetPropertyInfoNameFn GetPropertyInfoNameFptr = nullptr;
		GetPropertyInfoTypeFn GetPropertyInfoTypeFptr = nullptr;
		GetPropertyInfoAttributesFn GetPropertyInfoAttributesFptr = nullptr;
#pragma endregion

#pragma region Attribute
		GetAttributeFieldValueFn GetAttributeFieldValueFptr = nullptr;
		GetAttributeTypeFn GetAttributeTypeFptr = nullptr;
#pragma endregion

#pragma region ManagedArray
		CreatedArrayFn CreateArrayFptr = nullptr;
		GetArrayLengthFn GetArrayLengthFptr = nullptr;
		GetArrayValueFn GetArrayValueFptr = nullptr;
		SetArrayValueFn SetArrayValueFptr = nullptr;
		GetArrayDataReferenceFn GetArrayDataReferenceFptr = nullptr;
		ResizeRankOneArrayFn ResizeRankOneArrayFptr = nullptr;
		ResizeRankNArrayFn ResizeRankNArrayFptr = nullptr;
#pragma endregion

		CreateObjectFn CreateObjectFptr = nullptr;
		CreateAssemblyLoadContextFn CreateAssemblyLoadContextFptr = nullptr;
		InvokeMethodFn InvokeMethodFptr = nullptr;
		InvokeMethodRetFn InvokeMethodRetFptr = nullptr;
		InvokeMethodByMethodInfoFn InvokeMethodByMethodInfoFptr = nullptr;
		InvokeStaticMethodFn InvokeStaticMethodFptr = nullptr;
		InvokeStaticMethodRetFn InvokeStaticMethodRetFptr = nullptr;
		SetFieldValueFn SetFieldValueFptr = nullptr;
		SetFieldValueByFieldInfoFn SetFieldValueByFieldInfoFptr = nullptr;
		GetFieldValueFn GetFieldValueFptr = nullptr;
		GetFieldValueByFieldInfoFn GetFieldValueByFieldInfoFptr = nullptr;
		GetStaticFieldValueFn GetStaticFieldValueFptr = nullptr;
		SetPropertyValueFn SetPropertyValueFptr = nullptr;
		GetPropertyValueFn GetPropertyValueFptr = nullptr;
		DestroyObjectFn DestroyObjectFptr = nullptr;
		GetObjectTypeIdFn GetObjectTypeIdFptr = nullptr;

		CollectGarbageFn CollectGarbageFptr = nullptr;
		WaitForPendingFinalizersFn WaitForPendingFinalizersFptr = nullptr;
	};

	inline ManagedFunctions s_ManagedFunctions;

}
