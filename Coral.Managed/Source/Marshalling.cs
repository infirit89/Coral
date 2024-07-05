using Coral.Managed.Interop;

using System;
using System.Linq;
using System.Reflection;
using System.Reflection.Metadata.Ecma335;
using System.Runtime.InteropServices;

namespace Coral.Managed;

public static class Marshalling
{
	internal struct ArrayContainer
	{
		public IntPtr Data;
		public int Length;
	};

	internal struct ObjectContainer 
	{
		public IntPtr Handle;
	}

	public static void MarshalReturnValue(object? InTarget, object? InValue, MemberInfo? InMemberInfo, IntPtr OutValue)
	{
		if (InMemberInfo == null)
			return;

		Type? type = null;
		if (InMemberInfo is FieldInfo fieldInfo)
		{
			type = fieldInfo.FieldType;
		}
		else if (InMemberInfo is PropertyInfo propertyInfo)
		{
			type = propertyInfo.PropertyType;
		}
		else if (InMemberInfo is MethodInfo methodInfo)
		{
			type = methodInfo.ReturnType;
		}

		if (type == null)
			return;

		MarshalObject(InTarget, InValue, type, OutValue, InMemberInfo);
	}

	private struct ArrayObject
	{
		public ArrayObject() { }

		public IntPtr Handle = IntPtr.Zero;
		public IntPtr Padding = IntPtr.Zero;
	}

	public static object? MarshalArray(IntPtr InArray, Type? InElementType)
	{
		if (InElementType == null)
			return null;

		var arrayContainer = MarshalPointer<ArrayContainer>(InArray);
		return GCHandle.FromIntPtr(arrayContainer.Data).Target;

#if false
		// NOTE(infirit89): this if never succeeds????
		if (ArrayStorage.HasFieldArray(null, null))
		{
			var fieldArray = ArrayStorage.GetFieldArray(null, null, null);

			if (arrayContainer.Data == fieldArray!.Value.AddrOfPinnedObject())
			{
				return fieldArray.Value.Target;
			}
		}

		var elements = Array.CreateInstance(InElementType, arrayContainer.Length);

		if (InElementType.IsValueType)
		{
			int elementSize = Marshal.SizeOf(InElementType);

			unsafe
			{
				for (int i = 0; i < arrayContainer.Length; i++)
				{
					IntPtr source = (IntPtr)(((byte*)arrayContainer.Data.ToPointer()) + (i * elementSize));
					elements.SetValue(Marshal.PtrToStructure(source, InElementType), i);
				}
			}
		}
		else
		{
			unsafe
			{
				for (int i = 0; i < arrayContainer.Length; i++)
				{
					IntPtr source = (IntPtr)(((byte*)arrayContainer.Data.ToPointer()) + (i * Marshal.SizeOf<ArrayObject>()));
					var managedObject = MarshalPointer<ArrayObject>(source);
					var target = GCHandle.FromIntPtr(managedObject.Handle).Target;
					elements.SetValue(target, i);
				}
			}
		}



		return elements;
#endif
	}

	/*public static void CopyArrayToBuffer(GCHandle InArrayHandle, Array? InArray, Type? InElementType)
	{
		if (InArray == null || InElementType == null)
			return;

		var elementSize = Marshal.SizeOf(InElementType);
		int byteLength = InArray.Length * elementSize;

		int offset = 0;

		foreach (var elem in InArray)
		{
			var elementHandle = GCHandle.Alloc(elem, GCHandleType.Pinned);

			unsafe
			{
				Buffer.MemoryCopy(elementHandle.AddrOfPinnedObject().ToPointer(), ((byte*)mem.ToPointer()) + offset, elementSize, elementSize);
			}

			offset += elementSize;

			elementHandle.Free();
		}

		ArrayContainer container = new()
		{
			Data = mem,
			Length = InArray.Length
		};

		var handle = GCHandle.Alloc(container, GCHandleType.Pinned);
		
		unsafe
		{
			Buffer.MemoryCopy(handle.AddrOfPinnedObject().ToPointer(), InBuffer.ToPointer(), Marshal.SizeOf<ArrayContainer>(), Marshal.SizeOf<ArrayContainer>());
		}

		handle.Free();
	}*/

	public static object? MarshalPointer(IntPtr InValue, Type InType)
	{
		if (InType.IsPointer || InType == typeof(IntPtr))
			return InValue;

		// NOTE(Peter): Marshal.PtrToStructure<bool> doesn't seem to work
		//				instead we have to read it as a single byte and check the raw value
		if (InType == typeof(bool))
			return Marshal.PtrToStructure<byte>(InValue) > 0;

		if (InType == typeof(string))
		{
			var nativeString = Marshal.PtrToStructure<NativeString>(InValue);
			return nativeString.ToString();
		}
		else if (InType == typeof(NativeString))
		{
			return Marshal.PtrToStructure<NativeString>(InValue);
		}

		if (InType.IsSZArray)
			return MarshalArray(InValue, InType.GetElementType());

		if (InType.IsGenericType)
		{
			if (InType == typeof(NativeArray<>).MakeGenericType(InType.GetGenericArguments().First()))
			{
				var elements = Marshal.ReadIntPtr(InValue, 0);
				var elementCount = Marshal.ReadInt32(InValue, Marshal.SizeOf<IntPtr>());
				var genericType = typeof(NativeArray<>).MakeGenericType(InType.GetGenericArguments().First());
				return TypeInterface.CreateInstance(genericType, elements, elementCount);
			}
		}

		if (InType.IsClass)
		{
			var handlePtr = Marshal.ReadIntPtr(InValue);
			var handle = GCHandle.FromIntPtr(handlePtr);
			return handle.Target;
		}

		return Marshal.PtrToStructure(InValue, InType);	
	}

	public static void MarshalObject(object? InTarget, object? InValue, Type InType, IntPtr OutValue, MemberInfo? InMemberInfo = null)
	{
		if (InValue == null)
			return;

		Type type = InType;

		if (type.IsArray)
		{
			var fieldArray = ArrayStorage.GetFieldArray(InTarget, InValue, InMemberInfo);

            if (fieldArray != null)
            {
                unsafe
                {
                    ArrayContainer* container = (ArrayContainer*)OutValue;
                    (*container).Data = GCHandle.ToIntPtr(fieldArray.Value);
                    (*container).Length = (fieldArray.Value.Target as Array)!.Rank;
                }
            }
            else
            {
                Marshal.WriteIntPtr(OutValue, IntPtr.Zero);
            }
        }
        else if (InValue is bool)
        {
            unsafe
            {
                *(bool*)OutValue = (bool)InValue;
            }
        }
        else if (InValue is string str)
		{
			NativeString nativeString = str;
			Marshal.StructureToPtr(nativeString, OutValue, false);
		}
		else if (InValue is NativeString nativeString)
		{
			Marshal.StructureToPtr(nativeString, OutValue, false);
		}
		else if (type.IsPointer)		// what happens if type is a pointer to a class?
		{
			unsafe
			{
				if (InValue == null)
				{
					Marshal.WriteIntPtr(OutValue, IntPtr.Zero);
				}
				else
				{
					void* valuePointer = Pointer.Unbox(InValue);
					Buffer.MemoryCopy(&valuePointer, OutValue.ToPointer(), IntPtr.Size, IntPtr.Size);
				}
			}
		}
        else if (type.IsClass)
        {
            var handle = GCHandle.Alloc(InValue, GCHandleType.Normal);
            AssemblyLoader.RegisterHandle(type.Assembly, handle);
            unsafe
            {
                ObjectContainer* container = (ObjectContainer*)OutValue;
                (*container).Handle = GCHandle.ToIntPtr(handle);
            }
        }
        else
		{
			int valueSize = type.IsEnum ? Marshal.SizeOf(Enum.GetUnderlyingType(type)) : Marshal.SizeOf(type);
			var handle = GCHandle.Alloc(InValue, GCHandleType.Pinned);
			unsafe
			{
				Buffer.MemoryCopy(handle.AddrOfPinnedObject().ToPointer(), OutValue.ToPointer(), valueSize, valueSize);
			}

			handle.Free();
		}
    }

	public static T? MarshalPointer<T>(IntPtr InValue) => Marshal.PtrToStructure<T>(InValue);

	public static IntPtr[] NativeArrayToIntPtrArray(IntPtr InNativeArray, int InLength)
	{
		try
		{
			if (InNativeArray == IntPtr.Zero || InLength == 0)
				return [];

			IntPtr[] result = new IntPtr[InLength];

			for (int i = 0; i < InLength; i++)
				result[i] = Marshal.ReadIntPtr(InNativeArray, i * Marshal.SizeOf<nint>());

			return result;
		}
		catch (Exception ex)
		{
			ManagedHost.HandleException(ex);
			return [];
		}
	}

	public static object?[]? MarshalParameterArray(IntPtr InNativeArray, int InLength, MethodBase? InMethodInfo)
	{
		if (InMethodInfo == null)
			return null;

		if (InNativeArray == IntPtr.Zero || InLength == 0)
			return null;

		var parameterInfos = InMethodInfo.GetParameters();
		var parameterPointers = NativeArrayToIntPtrArray(InNativeArray, InLength);
		var result = new object?[parameterPointers.Length];

		for (int i = 0; i < parameterPointers.Length; i++)
		{
			result[i] = MarshalPointer(parameterPointers[i], parameterInfos[i].ParameterType);
		}

		return result;
	}
	
}
