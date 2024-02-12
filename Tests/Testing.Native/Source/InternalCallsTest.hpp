#pragma once

#include "Globals.hpp"
#include <Coral/ManagedArray.hpp>

#include <gtest/gtest.h>

namespace Testing
{
	class InternalCallsTestFixture : public testing::Test
	{
	protected:
		InternalCallsTestFixture() = default;
		~InternalCallsTestFixture() override = default;
	};

	class InternalCallsTest : public InternalCallsTestFixture
	{
	public:
		explicit InternalCallsTest(bool success) 
			: m_Success(success) {}

		void TestBody() override 
		{
			ASSERT_TRUE(m_Success);
		}

	private:
		bool m_Success;
	};

	void RegisterManagedTests() 
	{
		Coral::ManagedArray tests = Globals::TestType.InvokeStaticMethod<Coral::ManagedArray>("CollectTests");

		int32_t testCount = tests.GetLength(0);
		for (size_t i = 0; i < testCount; i++)
		{
			Coral::ManagedObject testObject = tests.GetValue<Coral::ManagedObject>(i);
			
			Coral::ScopedString testName = testObject.GetFieldValue<Coral::String>("Name");
			bool success = testObject.InvokeMethod<bool>("Run");
			testing::RegisterTest(
			    "InternalCalls", std::string(testName).c_str(), nullptr, nullptr,
				__FILE__, __LINE__,
			    [success]() -> InternalCallsTestFixture*
			    {
					return new InternalCallsTest(success);
				}
			);

			testObject.Destroy();
		}
	}
}
