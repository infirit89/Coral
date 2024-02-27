#pragma once

namespace Testing 
{
	struct DummyStructItem
	{
		bool operator==(const DummyStructItem& other) const
		{
			return TestInt == other.TestInt && TestFloat == other.TestFloat;
		}

		int TestInt;
		float TestFloat;
	};
}
