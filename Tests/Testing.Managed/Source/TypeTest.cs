using System;

namespace Testing.Managed;

[AttributeUsage(AttributeTargets.Class)]
public class TypeTestAttribute : Attribute 
{
}

[TypeTest]
public class BaseTypeTest
{
}

public class SubclassTypeTest : BaseTypeTest
{
}

public struct StructTypeTest 
{
    public int TestVar1, TestVar2;
}

public static class StaticTest
{
    public static float StaticMethodTest(float value)
    {
        return value;
    }

    public static int StaticMethodTest(int value)
    {
        return value;
    }

    public static bool StaticMethodTest(bool value)
    {
        return value;
    }
}
