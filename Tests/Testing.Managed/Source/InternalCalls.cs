using Coral.Managed.Interop;
using System;

namespace Testing.Managed
{
    internal static class InternalCalls
    {
        internal static unsafe delegate*<sbyte, sbyte> SByteMarshalIcall = default;
        internal static unsafe delegate*<byte, byte> ByteMarshalIcall = default;
        internal static unsafe delegate*<short, short> ShortMarshalIcall = default;
        internal static unsafe delegate*<ushort, ushort> UShortMarshalIcall = default;
        internal static unsafe delegate*<int, int> IntMarshalIcall = default;
        internal static unsafe delegate*<uint, uint> UIntMarshalIcall = default;
        internal static unsafe delegate*<long, long> LongMarshalIcall = default;
        internal static unsafe delegate*<ulong, ulong> ULongMarshalIcall = default;
        internal static unsafe delegate*<float, float> FloatMarshalIcall = default;
        internal static unsafe delegate*<double, double> DoubleMarshalIcall = default;
        internal static unsafe delegate*<bool, bool> BoolMarshalIcall = default;
        internal static unsafe delegate*<IntPtr, IntPtr> IntPtrMarshalIcall = default;
        internal static unsafe delegate*<NativeString, NativeString> StringMarshalIcall = default;
        internal static unsafe delegate*<NativeString, void> StringMarshalIcall2 = default;
        internal static unsafe delegate*<ReflectionType, bool> TypeMarshalIcall = default;
        internal static unsafe delegate*<NativeArray<float>> FloatArrayIcall = default;
        internal static unsafe delegate*<NativeArray<int>> EmptyArrayIcall = default;
        internal static unsafe delegate*<NativeInstance<InstanceTest>> NativeInstanceIcall = default;

        internal struct DummyStruct
        {
            public int X;
            public float Y;
            public int Z;
        }
        internal static unsafe delegate*<DummyStruct, DummyStruct> DummyStructMarshalIcall = default;
        internal static unsafe delegate*<DummyStruct*, DummyStruct*> DummyStructPtrMarshalIcall = default;
    }
}
