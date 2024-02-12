#include "InternalCalls.hpp"

namespace Testing 
{
	Coral::Type InternalCalls::s_InternalCallTestType;

	void InternalCalls::Register(Coral::ManagedAssembly& assembly)
	{
		s_InternalCallTestType = assembly.GetType("Testing.Managed.InternalCallTest");
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "SByteMarshalIcall", reinterpret_cast<void*>(&SByteMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "ByteMarshalIcall", reinterpret_cast<void*>(&ByteMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "ShortMarshalIcall", reinterpret_cast<void*>(&ShortMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "UShortMarshalIcall", reinterpret_cast<void*>(&UShortMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "IntMarshalIcall", reinterpret_cast<void*>(&IntMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "UIntMarshalIcall", reinterpret_cast<void*>(&UIntMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "LongMarshalIcall", reinterpret_cast<void*>(&LongMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "ULongMarshalIcall", reinterpret_cast<void*>(&ULongMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "FloatMarshalIcall", reinterpret_cast<void*>(&FloatMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "DoubleMarshalIcall", reinterpret_cast<void*>(&DoubleMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "BoolMarshalIcall", reinterpret_cast<void*>(&BoolMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "IntPtrMarshalIcall", reinterpret_cast<void*>(&IntPtrMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "StringMarshalIcall", reinterpret_cast<void*>(&StringMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "StringMarshalIcall2", reinterpret_cast<void*>(&StringMarshalIcall2));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "DummyStructMarshalIcall", reinterpret_cast<void*>(&DummyStructMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "DummyStructPtrMarshalIcall", reinterpret_cast<void*>(&DummyStructPtrMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "TypeMarshalIcall", reinterpret_cast<void*>(&TypeMarshalIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "EmptyArrayIcall", reinterpret_cast<void*>(&EmptyArrayIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "FloatArrayIcall", reinterpret_cast<void*>(&FloatArrayIcall));
		assembly.AddInternalCall("Testing.Managed.InternalCalls", "NativeInstanceIcall", reinterpret_cast<void*>(&NativeInstanceIcall));
	}
}
