using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Testing.Managed
{
    [AttributeUsage(AttributeTargets.Method | AttributeTargets.Field | AttributeTargets.Property)]
    public class DummyAttribute : Attribute
    {
        public float SomeValue;
    }

    public class AttributeTest
    {
        private const float m_AttributeFieldTestValue = 1000.0f;
        private const float m_AttributePropertyTestValue = 10000.0f;
        private const float m_AttributeMethodTestValue = 10.0f;

        [Dummy(SomeValue = m_AttributeFieldTestValue)]
        public float AttributeFieldTest = 50.0f;

        [Dummy(SomeValue = m_AttributePropertyTestValue)]
        public float AttributePropertyTest { get; private set; } = 50.0f;

        [Dummy(SomeValue = m_AttributeMethodTestValue)]
        public void AttributeMethodTest() { }
    }
}
