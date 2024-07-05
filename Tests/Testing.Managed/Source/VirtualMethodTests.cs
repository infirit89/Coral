using System;

namespace Testing.Managed
{
	public class VirtualMethodTests
	{
		public virtual string TestMe() { return ""; }
	}

	public class Override1 : VirtualMethodTests
	{
		public override string TestMe()
		{
			return "Override1";
		}
	}

	public class Override2 : VirtualMethodTests
	{
		public override string TestMe()
		{
			return "Override2";
		}
	}
}
