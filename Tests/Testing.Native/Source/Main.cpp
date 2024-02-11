#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <functional>
#include <ranges>

#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>
#include <Coral/ManagedArray.hpp>
#include <Coral/TypeCache.hpp>

#include <gtest/gtest.h>

#include "Globals.h"
#include "NativeTest.h"
#include "InternalCalls.h"
#include "AttributeTest.h"
#include "ScalarFieldMarshalTest.h"
#include "ObjectFieldMarshalTest.h"
#include "ScalarPropertyMarshalTest.h"
#include "ObjectPropertyMarshalTest.h"
#include "ScalarMethodMarshalTest.h"
#include "ObjectMethodMarshalTest.h"

Coral::Type g_TestsType;

void ExceptionCallback(std::string_view InMessage)
{
	std::cout << "\033[1;31m " << "Unhandled native exception: " << InMessage << "\033[0m\n";
}

//struct Test
//{
//	std::string Name;
//	std::function<bool()> Func;
//};
//std::vector<Test> tests;
//
//void RegisterTest(std::string_view InName, std::function<bool()> InFunc)
//{
//	tests.push_back(Test{ std::string(InName), std::move(InFunc) });
//}
//
//void RegisterMemberMethodTests(Coral::HostInstance& InHost, Coral::ManagedObject InObject)
//{
//	RegisterTest("SByteTest", [InObject]() mutable{ return InObject.InvokeMethod<char8_t, char8_t>("SByteTest", 10) == 20; });
//	RegisterTest("ByteTest", [InObject]() mutable{ return InObject.InvokeMethod<uint8_t, uint8_t>("ByteTest", 10) == 20; });
//	RegisterTest("ShortTest", [InObject]() mutable{ return InObject.InvokeMethod<int16_t, int16_t>("ShortTest", 10) == 20; });
//	RegisterTest("UShortTest", [InObject]() mutable{ return InObject.InvokeMethod<uint16_t, uint16_t>("UShortTest", 10) == 20; });
//	RegisterTest("IntTest", [InObject]() mutable{ return InObject.InvokeMethod<int32_t, int32_t>("IntTest", 10) == 20; });
//	RegisterTest("UIntTest", [InObject]() mutable{ return InObject.InvokeMethod<uint32_t, uint32_t>("UIntTest", 10) == 20; });
//	RegisterTest("LongTest", [InObject]() mutable{ return InObject.InvokeMethod<int64_t, int64_t>("LongTest", 10) == 20; });
//	RegisterTest("ULongTest", [InObject]() mutable{ return InObject.InvokeMethod<uint64_t, uint64_t>("ULongTest", 10) == 20; });
//	RegisterTest("FloatTest", [InObject]() mutable{ return InObject.InvokeMethod<float, float>("FloatTest", 10.0f) - 20.0f < 0.001f; });
//	RegisterTest("DoubleTest", [InObject]() mutable{ return InObject.InvokeMethod<double, double>("DoubleTest", 10.0) - 20.0 < 0.001; });
//	//RegisterTest("BoolTest", [InObject]() mutable { return InObject.InvokeMethod<bool, bool>("BoolTest", false); });
//	RegisterTest("IntPtrTest", [InObject]() mutable{ int32_t v = 10; return *InObject.InvokeMethod<int32_t*, int32_t*>("IntPtrTest", &v) == 50; });
//	RegisterTest("StringTest", [InObject, &InHost]() mutable
//	{
//		Coral::ScopedString str = InObject.InvokeMethod<Coral::String, Coral::String>("StringTest", Coral::String::New("Hello"));
//		return str == "Hello, World!";
//	});
//	
//	RegisterTest("DummyStructTest", [InObject]() mutable
//	{
//		DummyStruct value =
//		{
//			.X = 10,
//			.Y = 10.0f,
//			.Z = 10,
//		};
//		auto result = InObject.InvokeMethod<DummyStruct, DummyStruct&>("DummyStructTest", value);
//		return result.X == 20 && result.Y - 20.0f < 0.001f && result.Z == 20;
//	});
//	RegisterTest("DummyStructPtrTest", [InObject]() mutable
//	{
//		DummyStruct value =
//		{
//			.X = 10,
//			.Y = 10.0f,
//			.Z = 10,
//		};
//		auto* result = InObject.InvokeMethod<DummyStruct*, DummyStruct*>("DummyStructPtrTest", &value);
//		return result->X == 20 && result->Y - 20.0f < 0.001f && result->Z == 20;
//	});
//
//	RegisterTest("OverloadTest", [InObject]() mutable
//	{
//		return InObject.InvokeMethod<int32_t, int32_t>("Int32 OverloadTest(Int32)", 50) == 1050;
//	});
//
//	RegisterTest("OverloadTest", [InObject]() mutable
//	{
//		return InObject.InvokeMethod<float, float>("OverloadTest", 5) == 15.0f;
//	});
//}
//
//
//void RunTests()
//{
//	size_t passedTests = 0;
//	for (size_t i = 0; i < tests.size(); i++)
//	{
//		const auto& test = tests[i];
//		bool result = test.Func();
//		if (result)
//		{
//			std::cout << "\033[1;32m[" << i + 1 << " / " << tests.size() << " (" << test.Name << "): Passed\033[0m\n";
//			passedTests++;
//		}
//		else
//		{
//			std::cerr << "\033[1;31m[" << i + 1 << " / " << tests.size() << " (" << test.Name << "): Failed\033[0m\n"; 
//		}
//	}
//	std::cout << "[NativeTest]: Done. " << passedTests << " passed, " << tests.size() - passedTests  << " failed.\n";
//}

using namespace Testing;

int main(int argc, char** argv)
{
	
	#ifdef CORAL_TESTING_DEBUG
	const char* ConfigName = "Debug";
#else
	const char* ConfigName = "Release";
#endif

	auto exeDir = std::filesystem::path(argv[0]).parent_path();
	
	Coral::HostSettings settings = {
		.CoralDirectory = exeDir.string(),
		.ExceptionCallback = ExceptionCallback
	};
	
	Globals::HostInstance.Initialize(settings);

	Globals::AssemblyLoadContext = Globals::HostInstance.CreateAssemblyLoadContext("TestContext");
	auto assemblyPath = exeDir / "Testing.Managed.dll";
	Globals::TestAssembly = &Globals::AssemblyLoadContext.LoadAssembly(assemblyPath.string());
	auto& assembly = *Globals::TestAssembly;
	InternalCalls::Register(assembly);
	Globals::TestAssembly->UploadInternalCalls();
	Globals::TestType = assembly.GetType("Testing.Managed.TestUtils");

	auto& instanceTestType = assembly.GetType("Testing.Managed.InstanceTest");
	Globals::TestInstance = instanceTestType.CreateInstance();
	Globals::TestInstance.SetFieldValue("X", 500.0f);

	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	Coral::GC::Collect();
	Globals::HostInstance.UnloadAssemblyLoadContext(Globals::AssemblyLoadContext);

	return result;

	//auto& fieldTestType = assembly.GetType("Testing.Managed.FieldMarshalTest");
	//std::cout << fieldTestType.IsAssignableTo(fieldTestType) << std::endl;

	//auto fieldTestObject = fieldTestType.CreateInstance();

	//auto dummyClassInstance = assembly.GetType("Testing.Managed.DummyClass").CreateInstance();
	//dummyClassInstance.SetFieldValue("X", 500.0f);

	//struct DummyStruct
	//{
	//	float X;
	//} ds;
	//ds.X = 50.0f;
	//fieldTestObject.SetFieldValue("DummyClassTest", dummyClassInstance);
	//fieldTestObject.SetFieldValue("DummyStructTest", ds);
	//fieldTestObject.InvokeMethod("TestClassAndStruct");
	//dummyClassInstance.Destroy();


	//for (auto fieldInfo : fieldTestType.GetFields())
	//{
	//	auto& type = fieldInfo.GetType();
	//	auto accessibility = fieldInfo.GetAccessibility();

	//	auto attributes = fieldInfo.GetAttributes();
	//	for (auto attrib : attributes)
	//	{
	//		auto& attribType = attrib.GetType();

	//		if (attribType.GetFullName() == "Testing.Managed.DummyAttribute")
	//			std::cout << attrib.GetFieldValue<float>("SomeValue") << std::endl;
	//	}
	//}

	//for (auto propertyInfo : fieldTestType.GetProperties())
	//{
	//	auto& type = propertyInfo.GetType();

	//	auto attributes = propertyInfo.GetAttributes();
	//	for (auto attrib : attributes)
	//	{
	//		auto& attribType = attrib.GetType();

	//		if (attribType.GetFullName() == "Testing.Managed.DummyAttribute")
	//			std::cout << attrib.GetFieldValue<float>("SomeValue") << std::endl;
	//	}
	//}
	//
	//auto& memberMethodTestType = assembly.GetType("Testing.Managed.MemberMethodTest");

	//// for (auto methodInfo : memberMethodTestType.GetMethods())
	//// {
	//// 	auto& type = methodInfo.GetReturnType();
	//// 	auto accessibility = methodInfo.GetAccessibility();
	//// 	std::cout << methodInfo.GetName() << ", Returns: " << type.GetFullName() << std::endl;
	//// 	const auto& parameterTypes = methodInfo.GetParameterTypes();
	//// 	for (const auto& paramType : parameterTypes)
	//// 	{
	//// 		std::cout << "\t" << paramType->GetFullName() << std::endl;
	//// 	}

	//// 	auto attributes = methodInfo.GetAttributes();
	//// 	for (auto attrib : attributes)
	//// 	{
	//// 		auto& attribType = attrib.GetType();

	//// 		if (attribType.GetFullName() == "Testing.Managed.DummyAttribute")
	//// 			std::cout << attrib.GetFieldValue<float>("SomeValue") << std::endl;
	//// 	}
	//// }

	//auto memberMethodTest = memberMethodTestType.CreateInstance();

	//RegisterFieldMarshalTests(hostInstance, fieldTestObject);
	//RegisterMemberMethodTests(hostInstance, memberMethodTest);
	//RunTests();

	//memberMethodTest.Destroy();
	//fieldTestObject.Destroy();

	//auto& virtualMethodTestType1 = assembly.GetType("Testing.Managed.Override1");
	//auto& virtualMethodTestType2 = assembly.GetType("Testing.Managed.Override2");

	//auto instance1 = virtualMethodTestType1.CreateInstance();
	//auto instance2 = virtualMethodTestType2.CreateInstance();

	//instance1.InvokeMethod("TestMe");
	//instance2.InvokeMethod("TestMe");

	//instance.Destroy();
	//instance1.Destroy();
	//instance2.Destroy();

	//hostInstance.UnloadAssemblyLoadContext(loadContext);

	//Coral::GC::Collect();

	//std::cin.get();

	//loadContext = hostInstance.CreateAssemblyLoadContext("ALC2");
	//auto& newAssembly = loadContext.LoadAssembly(assemblyPath.string());

	//auto ls = newAssembly.GetLoadStatus();

	//RegisterTestInternalCalls(newAssembly);
	//newAssembly.UploadInternalCalls();

	//auto& testsType2 = newAssembly.GetType("Testing.Managed.Tests");
	//g_TestsType = testsType2;

	//auto& instanceTestType2 = newAssembly.GetType("Testing.Managed.InstanceTest");
	//instance = instanceTestType2.CreateInstance();
	//instance.SetFieldValue("X", 500.0f);

	//Coral::ManagedObject testsInstance2 = testsType2.CreateInstance();
	//testsInstance2.InvokeMethod("RunManagedTests");
	//testsInstance2.Destroy();
	//instance.Destroy();

	//std::cin.get();

	return 0;
}
