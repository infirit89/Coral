using Coral.Managed.Interop;

using System;

namespace Testing.Managed;
public class FieldMarshalTest
{
    public class DummyClass
    {
        public float X;
    }

    public struct DummyStruct
    {
        public float X;
    }

    public sbyte SByteFieldTest = 10;
	public byte ByteFieldTest = 10;
	public short ShortFieldTest = 10;
	public ushort UShortFieldTest = 10;
	public int IntFieldTest = 10;
	public uint UIntFieldTest = 10;
	public long LongFieldTest = 10;
	public ulong ULongFieldTest = 10;
	public float FloatFieldTest = 10.0f;
	public double DoubleFieldTest = 10.0;
	public bool BoolFieldTest = false;
	public string StringFieldTest = "Hello";
	public DummyClass DummyClassFieldTest = null!;
	public DummyStruct DummyStructFieldTest;

	public int[] IntArrayTest = [5, 2, 1, 64];

	public void ArrayParamTest(NativeArray<float> InArray)
	{
		foreach (var f in InArray)
			Console.WriteLine(f);
	}

	public float[] ArrayReturnTest()
	{
		return [10.0f, 20.0f, 30.0f, 40.0f, 50.0f];
	}

	public void PrintIntArray() 
	{
		Console.WriteLine(string.Join(' ', IntArrayTest));
	}

	public void TestClassAndStruct()
	{
		Console.WriteLine(DummyClassFieldTest.X);
		Console.WriteLine(DummyStructFieldTest.X);
	}

}