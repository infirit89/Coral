#pragma once

#include "Globals.hpp"

#include <Coral/HostInstance.hpp>
#include <Coral/Assembly.hpp>
#include <Coral/Type.hpp>
#include <Coral/ManagedArray.hpp>
#include <Coral/ManagedObject.hpp>
#include <Coral/GC.hpp>

#include <gtest/gtest.h>

namespace Testing
{
	class NativeTest : public testing::Test
	{
	protected:
		NativeTest()
		    : m_Assembly(*Globals::TestAssembly) { }

		~NativeTest() override = default;

		Coral::ManagedAssembly& m_Assembly;
	};
}
