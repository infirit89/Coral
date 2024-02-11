using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testing.Managed;

public class PropertyMarshalTest
{
    public class DummyClass 
    {
        public float X;
        public bool Y;
    }

    public struct DummyStruct 
    {
        public float X;
        public int Y;
    }

    public sbyte SBytePropertyTest { get; set; } = 10;
    public byte BytePropertyTest { get; set; } = 10;
    public short ShortPropertyTest { get; set; } = 10;
    public ushort UShortPropertyTest { get; set; } = 10;
    public int IntPropertyTest { get; set; } = 10;
    public uint UIntPropertyTest { get; set; } = 10;
    public long LongPropertyTest { get; set; } = 10;
    public ulong ULongPropertyTest { get; set; } = 10;
    public float FloatPropertyTest { get; set; } = 10.0f;
    public double DoublePropertyTest { get; set; } = 10.0;
    public bool BoolPropertyTest { get; set; } = false;
    public string StringPropertyTest { get; set; } = "Hello";
    public DummyClass DummyClassPropertyTest { get; set; }
    public DummyStruct DummyStructPropertyTest { get; set; }

    public int[] IntArrayProp { get; set; } = new int[] { 6, 10, 16, 24 };
}
