using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations.Schema;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testing.Managed;

public class DummyClassItem
{
    public DummyClassItem() { }

    public DummyClassItem(string s, float f) 
    {
        TestString = s;
        TestFloat = f;
    }

    public string TestString = "Default";
    public float TestFloat = 5.123f;
}

public struct DummyStructItem 
{
    public int TestInt;
    public float TestFloat;
}
