using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Runtime.InteropServices;

using Coral.Managed.Interop;

namespace Testing.Managed {

	public class InstanceTest
	{
		public float X = 50.0f;

		public float Stuff()
		{
			return X * 10.0f;
		}
	}
}
