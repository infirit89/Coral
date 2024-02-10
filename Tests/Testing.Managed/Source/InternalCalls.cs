using Coral.Managed.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testing.Managed
{
    internal static class InternalCalls
    {
        internal static unsafe delegate*<sbyte, sbyte> SByteMarshalIcall;
        internal static unsafe delegate*<byte, byte> ByteMarshalIcall;
        internal static unsafe delegate*<short, short> ShortMarshalIcall;
        internal static unsafe delegate*<ushort, ushort> UShortMarshalIcall;
        internal static unsafe delegate*<int, int> IntMarshalIcall;
        internal static unsafe delegate*<uint, uint> UIntMarshalIcall;
        internal static unsafe delegate*<long, long> LongMarshalIcall;
        internal static unsafe delegate*<ulong, ulong> ULongMarshalIcall;
        internal static unsafe delegate*<float, float> FloatMarshalIcall;
        internal static unsafe delegate*<double, double> DoubleMarshalIcall;
        internal static unsafe delegate*<bool, bool> BoolMarshalIcall;
        internal static unsafe delegate*<IntPtr, IntPtr> IntPtrMarshalIcall;
        internal static unsafe delegate*<NativeString, NativeString> StringMarshalIcall;
        internal static unsafe delegate*<NativeString, void> StringMarshalIcall2;
        internal static unsafe delegate*<ReflectionType, bool> TypeMarshalIcall;
        internal static unsafe delegate*<NativeArray<float>> FloatArrayIcall;
        internal static unsafe delegate*<NativeArray<int>> EmptyArrayIcall;
        internal static unsafe delegate*<NativeInstance<InstanceTest>> NativeInstanceIcall;

        internal struct DummyStruct
        {
            public int X;
            public float Y;
            public int Z;
        }
        internal static unsafe delegate*<DummyStruct, DummyStruct> DummyStructMarshalIcall;
        internal static unsafe delegate*<DummyStruct*, DummyStruct*> DummyStructPtrMarshalIcall;
    }
}
