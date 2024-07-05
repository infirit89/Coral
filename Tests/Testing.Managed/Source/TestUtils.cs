using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Testing.Managed
{
    internal class AssertException : Exception { }

    internal class Assert
    {
        public static void AreEqual<T>(T value1, T value2)
        {
            if (value1 is null)
                throw new AssertException();

            if (!value1.Equals(value2))
                throw new AssertException();
        }

        public static void IsTrue(bool condition) 
        {
            if (!condition)
                throw new AssertException();
        }

        public static void IsLess<T>(T value1, T value2) where T : IComparable
        {
            if (!(value1.CompareTo(value2) < 0))
                throw new AssertException();
        }

        public static void IsGreater<T>(T value1, T value2) where T : IComparable 
        {
            if (!(value1.CompareTo(value2) > 0))
                throw new AssertException();
        }

        public static void Fail() 
        {
            throw new AssertException();
        }
    }

    internal class TestUtils
    {
        public static TestContainer[] CollectTests()
        {
            List<TestContainer> tests = new List<TestContainer>();

            var types = Assembly.GetExecutingAssembly()
                                .GetTypes()
                                .Where(
                                    typeInfo => 
                                        typeInfo.GetCustomAttributes(typeof(CoralTestClassAttribute), false)
                                        .Any());
            foreach (var type in types)
            {
                var methods = type.GetMethods().Where(methodInfo => methodInfo.GetCustomAttributes(typeof(CoralTestMethodAttribute), false).Any());
                object? context = Activator.CreateInstance(type);
                if (context == null)
                    continue;

                foreach (var method in methods) 
                {
                    tests.Add(new TestContainer($"{type.FullName}.{method.Name}", () => 
                    {
                        try
                        {
                            method.Invoke(context, BindingFlags.DoNotWrapExceptions, null, null, null);
                        }
                        catch
                        {
                            throw;
                        }
                    }));
                }
            }

            return tests.ToArray();
        }

        internal class TestContainer
        {
            public readonly string Name;
            private readonly Action m_Func;

            internal TestContainer(string InName, Action InFunction)
            {
                Name = InName;
                m_Func = InFunction;
            }

            public bool Run()
            {
                try
                {
                    m_Func();
                    return true;
                }
                catch (AssertException) 
                {
                    return false;
                }
            }
        }
    }


    [AttributeUsage(AttributeTargets.Method)]
    public class CoralTestMethodAttribute : Attribute { }

    [AttributeUsage(AttributeTargets.Class)]
    public class CoralTestClassAttribute : Attribute { }
}
