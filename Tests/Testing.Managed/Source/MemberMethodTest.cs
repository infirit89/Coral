using System;
using System.Runtime.InteropServices;

namespace Testing.Managed;


public class MemberMethodTest
{
	public struct DummyStruct
	{
		public int X;
		public float Y;
		public int Z;
	}

	public class DummyClass 
	{
		public DummyClass(DummyStruct value) 
		{
			X = value.X;
			Y = value.Y;
			Z = value.Z;
		}

		public DummyStruct GetValues() 
		{
			return new() { X = X, Y = Y, Z = Z };
		}

		public int X;
		public float Y;
		public int Z;
	}
	
	public sbyte SByteMethodTest(sbyte InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public byte ByteMethodTest(byte InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public short ShortMethodTest(short InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public ushort UShortMethodTest(ushort InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public int IntMethodTest(int InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public uint UIntMethodTest(uint InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public long LongMethodTest(long InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public ulong ULongMethodTest(ulong InValue)
	{
		InValue *= 2;
		return InValue;
	}
	
	public float FloatMethodTest(float InValue)
	{
		InValue *= 2.0f;
		return InValue;
	}
	
	public double DoubleMethodTest(double InValue)
	{
		InValue *= 2.0;
		return InValue;
	}
	
	public bool BoolMethodTest(bool InValue)
	{
		InValue = !InValue;
		return InValue;
	}
	
	public IntPtr IntPtrMethodTest(IntPtr InValue)
	{
		Marshal.WriteInt32(InValue, 50);
		return InValue;
	}
	
	public string StringMethodTest(string InValue)
	{
		InValue += ", World!";
		return InValue;
	}

	public DummyStruct DummyStructMethodTest(DummyStruct InValue)
	{
		InValue.X *= 2;
		InValue.Y *= 2.0f;
		InValue.Z *= 2;
		return InValue;
	}

	public int OverloadMethodTest(int InValue)
	{
		return InValue + 1000;
	}

	public float OverloadMethodTest(float InValue)
	{
		return InValue + 10.0f;
	}

	public unsafe DummyStruct* DummyStructPtrMethodTest(DummyStruct* InValue)
	{
		InValue->X *= 2;
		InValue->Y *= 2.0f;
		InValue->Z *= 2;
        return InValue;
	}

	public void DummyClassMethodTest(DummyClass InValue) 
	{
        InValue.X *= 2;
        InValue.Y *= 2.0f;
        InValue.Z *= 2;
    }
}