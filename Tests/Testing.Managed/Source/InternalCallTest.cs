using Coral.Managed.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using static Testing.Managed.InternalCalls;

namespace Testing.Managed
{
    [CoralTestClass]
    public class InternalCallTest
    {
        public static float StaticMethodTest(float value)
        {
            return value;
        }

        public static int StaticMethodTest(int value)
        {
            return value;
        }

        [CoralTestMethod]
        public void SByteMarshalTest()
        {
            unsafe { Assert.AreEqual<sbyte>(SByteMarshalIcall(10), 20); }
        }

        [CoralTestMethod]
        public void ByteMarshalTest()
        {
            unsafe { Assert.AreEqual<byte>(ByteMarshalIcall(10), 20); }
        }

        [CoralTestMethod]
        public void ShortMarshalTest()
        {
            unsafe { Assert.AreEqual<short>(ShortMarshalIcall(10), 20); }
        }

        [CoralTestMethod]
        public void UShortMarshalTest()
        {
            unsafe { Assert.AreEqual<ushort>(UShortMarshalIcall(10), 20); }
        }

        [CoralTestMethod]
        public void IntMarshalTest()
        {
            unsafe { Assert.AreEqual(IntMarshalIcall(10), 20); }
        }

        [CoralTestMethod]
        public void UIntMarshalTest()
        {
            unsafe { Assert.AreEqual(UIntMarshalIcall(10), 20U); }
        }

        [CoralTestMethod]
        public void LongMarshalTest()
        {
            unsafe { Assert.AreEqual(LongMarshalIcall(10), 20L); }
        }

        [CoralTestMethod]
        public void ULongMarshalTest()
        {
            unsafe { Assert.AreEqual(ULongMarshalIcall(10), 20UL); }
        }

        [CoralTestMethod]
        public void FloatMarshalTest()
        {
            unsafe { Assert.IsLess(Math.Abs(FloatMarshalIcall(10.0f) - 20.0f), 0.001f); }
        }

        [CoralTestMethod]
        public void DoubleMarshalTest()
        {
            unsafe { Assert.IsLess(Math.Abs(DoubleMarshalIcall(10.0) - 20.0), 0.001); }
        }

        [CoralTestMethod]
        public void BoolMarshalTest()
        {
            unsafe { Assert.IsTrue(BoolMarshalIcall(false)); }
        }

        [CoralTestMethod]
        public void EmptyArrayTest()
        {
            unsafe
            {
                using var arr = EmptyArrayIcall();
                foreach (var item in arr)
                {
                    Console.WriteLine(item);
                }
            }

            // Pass!
            //return true;
        }

        [CoralTestMethod]
        public void FloatArrayTest()
        {
            float[] requiredValues = new[] { 5.0f, 10.0f, 15.0f, 50.0f };

            unsafe
            {
                using var arr = FloatArrayIcall();
                for (int i = 0; i < arr.Length; i++)
                {
                    if (requiredValues[i] != arr[i])
                        Assert.Fail();
                }
            }

            // Pass!
            //return true;
        }

        [CoralTestMethod]
        public void IntPtrMarshalTest()
        {
            IntPtr data = Marshal.AllocCoTaskMem(Marshal.SizeOf<int>());
            Marshal.WriteInt32(data, 50);
            bool success;
            unsafe { success = Marshal.ReadInt32(IntPtrMarshalIcall(data), 0) == 100; }
            Marshal.FreeCoTaskMem(data);
            Assert.IsTrue(success);
        }

        [CoralTestMethod]
        public void StringMarshalTest()
        {
            unsafe { Assert.IsTrue(StringMarshalIcall("Hello") == "Hello"); }
        }

        [CoralTestMethod]
        public void StringMarshalTest2()
        {
            unsafe { StringMarshalIcall2("Hello, World!"); }
            // Pass!
            //return true;
        }

        [CoralTestMethod]
        public void DummyStructMarshalTest()
        {
            DummyStruct s = new()
            {
                X = 10,
                Y = 15.0f,
                Z = 100
            };

            unsafe
            {
                var newS = DummyStructMarshalIcall(s);
                Assert.AreEqual(newS.X, 20);
                Assert.IsLess(Math.Abs(newS.Y) - 30.0f, 0.001f);
                Assert.AreEqual(newS.Z, 200);
            }
        }

        [CoralTestMethod]
        public void DummyStructPtrMarshalTest()
        {
            DummyStruct s = new()
            {
                X = 10,
                Y = 15.0f,
                Z = 100
            };

            unsafe
            {
                var newS = DummyStructPtrMarshalIcall(&s);
                Assert.AreEqual(newS->X, 20);
                Assert.IsLess(Math.Abs(newS->Y) - 30.0f, 0.001f);
                Assert.AreEqual(newS->Z, 200);
            }
        }

        [CoralTestMethod]
        public void TypeMarshalTest()
        {
            var t = typeof(InternalCallTest);
            unsafe { Assert.IsTrue(TypeMarshalIcall(t)); }
        }

        [CoralTestMethod]
        public void NativeInstanceTest()
        {
            InstanceTest? instanceTest;
            unsafe { instanceTest = NativeInstanceIcall(); }
            Assert.AreEqual(instanceTest?.X, 500.0f);
        }
    }
}
