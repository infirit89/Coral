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
        public static bool RunManagedTests()
        {
            CollectTests();

            foreach (var test in s_Tests)
                test.Run();

            Console.WriteLine($"[Test]: Done. {s_PassedTests} passed, {s_Tests.Count - s_PassedTests} failed.");
            return s_Tests.Count - s_PassedTests == 0;
        }

        // NOTE: maybe pass all the tests to c++?
        //		 and let gtest handle the verification?
        private static void CollectTests()
        {
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
                    s_Tests.Add(new TestContainer($"{type.FullName}.{method.Name}", s_Tests.Count + 1, () => 
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

        }

        private class TestContainer
        {
            private readonly string m_Name;
            private readonly int m_TestIndex;
            private readonly Action m_Func;

            internal TestContainer(string InName, int InTestIndex, Action InFunction)
            {
                m_Name = InName;
                m_TestIndex = InTestIndex;
                m_Func = InFunction;
            }

            public void Run()
            {
                try
                {
                    m_Func();
                    Succeeded();
                }
                catch (AssertException) 
                {
                    Failed();
                }
            }

            private void Succeeded() 
            {
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine($"[{m_TestIndex} / {s_Tests.Count} ({m_Name})]: Passed");
                Console.ForegroundColor = ConsoleColor.White;
                Console.Write("");
                s_PassedTests++;
            }

            private void Failed() 
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"[{m_TestIndex} / {s_Tests.Count} ({m_Name})]: Failed");
                Console.ForegroundColor = ConsoleColor.White;
                Console.Write("");
            }
        }

        private static List<TestContainer> s_Tests = new List<TestContainer>();
        private static int s_PassedTests = 0;
    }


    [AttributeUsage(AttributeTargets.Method)]
    public class CoralTestMethodAttribute : Attribute { }

    [AttributeUsage(AttributeTargets.Class)]
    public class CoralTestClassAttribute : Attribute { }
}
