﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Coral.Managed.Interop;

public sealed class NativeArrayEnumerator<T> : IEnumerator<T>
{
	private readonly T[] m_Elements;
	private int m_Index = -1;

	public NativeArrayEnumerator(T[] elements)
	{
		m_Elements = elements;
	}

	public bool MoveNext()
	{
		m_Index++;
		return m_Index < m_Elements.Length;
	}

	void IEnumerator.Reset() => m_Index = -1;
	void IDisposable.Dispose()
	{
		m_Index = -1;
		GC.SuppressFinalize(this);
	}

	object IEnumerator.Current => Current!;

	public T Current => m_Elements[m_Index];

}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct NativeArray<T> : IDisposable, IEnumerable<T>
{
	private readonly IntPtr m_NativeArray;
	private readonly int m_NativeLength;

	private Bool32 m_IsDisposed;

	public int Length => m_NativeLength;

	public NativeArray(int InLength)
	{
		m_NativeArray = Marshal.AllocHGlobal(InLength * Marshal.SizeOf<T>());
		m_NativeLength = InLength;
	}

	public NativeArray([DisallowNull] T?[] InValues)
	{
		m_NativeArray = Marshal.AllocHGlobal(InValues.Length * Marshal.SizeOf<T>());
		m_NativeLength = InValues.Length;

		for (int i = 0; i < m_NativeLength; i++)
		{
			var elem = InValues[i];

			if (elem == null)
				continue;

			Marshal.StructureToPtr(elem, IntPtr.Add(m_NativeArray, i * Marshal.SizeOf<T>()), false);
		}
	}

	internal NativeArray(IntPtr InArray, int InLength)
	{
		m_NativeArray = InArray;
		m_NativeLength = InLength;
	}

	public T[] ToArray()
	{
		Span<T> data = Span<T>.Empty;

		if (m_NativeArray != IntPtr.Zero && m_NativeLength > 0)
		{
			unsafe { data = new Span<T>(m_NativeArray.ToPointer(), m_NativeLength); }
		}

		return data.ToArray();
	}

	public Span<T> ToSpan()
	{
		unsafe { return new Span<T>(m_NativeArray.ToPointer(), m_NativeLength); }
	}

	public ReadOnlySpan<T> ToReadOnlySpan() => ToSpan();

	public void Dispose()
	{
		if (!m_IsDisposed)
		{
			Marshal.FreeHGlobal(m_NativeArray);
			m_IsDisposed = true;
		}

		GC.SuppressFinalize(this);
	}

	public IEnumerator<T> GetEnumerator() => new NativeArrayEnumerator<T>(this);
	IEnumerator IEnumerable.GetEnumerator() => new NativeArrayEnumerator<T>(this);

	public T? this[int InIndex]
	{
		get => Marshal.PtrToStructure<T>(IntPtr.Add(m_NativeArray, InIndex * Marshal.SizeOf<T>()));
		set => Marshal.StructureToPtr<T>(value!, IntPtr.Add(m_NativeArray, InIndex * Marshal.SizeOf<T>()), false);
	}

	public static NativeArray<T> Map(T[] array)
	{
		var handle = GCHandle.Alloc(array, GCHandleType.Pinned);
		return new(handle.AddrOfPinnedObject(), array.Length);
	}

	public static implicit operator T[](NativeArray<T> InArray) => InArray.ToArray();
	public static implicit operator NativeArray<T>(T[] InArray) => new(InArray);

}

public static class ArrayStorage
{
	private static Dictionary<int, GCHandle> s_FieldArrays = new();

	public static bool HasFieldArray(object? InTarget, MemberInfo? InArrayMemberInfo)
	{
		if (InArrayMemberInfo == null)
			return false;

		int arrayId = InArrayMemberInfo.GetHashCode();
		arrayId += InTarget != null ? InTarget.GetHashCode() : 0;
		return s_FieldArrays.ContainsKey(arrayId);
	}

	public static GCHandle? GetFieldArray(object? InTarget, object? InValue, MemberInfo? InArrayMemberInfo)
	{
		if (InArrayMemberInfo == null && InTarget == null)
			return null;

		int arrayId = InArrayMemberInfo != null ? InArrayMemberInfo.GetHashCode() : 0;
		arrayId += InTarget != null ? InTarget.GetHashCode() : 0;

		if (!s_FieldArrays.TryGetValue(arrayId, out var arrayHandle) || arrayHandle.Target != InValue)
		{
			if (arrayHandle.IsAllocated)
				arrayHandle.Free();

			var arrayObject = InValue as Array;
			arrayHandle = GCHandle.Alloc(arrayObject, GCHandleType.Normal);

			if(InTarget != null)
				AssemblyLoader.RegisterHandle(InTarget.GetType().Assembly, arrayHandle);
			else
                AssemblyLoader.RegisterHandle(Assembly.GetExecutingAssembly(), arrayHandle);
            
            s_FieldArrays[arrayId] = arrayHandle;
		}

		return arrayHandle;
	}

	public static void FreeFieldArrayIfExists(object? InTarget, MemberInfo? InArrayMemberInfo) 
	{
		if (InArrayMemberInfo == null)
			return;

		int arrayId = InArrayMemberInfo.GetHashCode();
		arrayId += InTarget != null ? InTarget.GetHashCode() : 0;
		if (s_FieldArrays.TryGetValue(arrayId, out var handle)) 
		{
			try
			{
				if(GCHandle.ToIntPtr(handle) != IntPtr.Zero)
					handle.Free();
			}
			catch (InvalidOperationException ex) 
			{
				ManagedHost.HandleException(ex);
			}
			s_FieldArrays.Remove(arrayId);
		}
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct NativeInstance<T>
{
	private readonly IntPtr m_Handle;
	private readonly IntPtr m_Unused;

	private NativeInstance(IntPtr handle)
	{
		m_Handle = handle;
		m_Unused = IntPtr.Zero;
	}

	public T? Get()
	{
		if (m_Handle == IntPtr.Zero)
			return default;

		GCHandle handle = GCHandle.FromIntPtr(m_Handle);

		if (!(handle.Target is T))
			return default;
		
		return (T)handle.Target;
	}

	public static implicit operator NativeInstance<T>(T instance)
	{
		return new(GCHandle.ToIntPtr(GCHandle.Alloc(instance, GCHandleType.Pinned)));
	}

	public static implicit operator T?(NativeInstance<T> InInstance)
	{
		return InInstance.Get();
	}
}

[StructLayout(LayoutKind.Sequential)]
public struct NativeString : IDisposable
{
	internal IntPtr m_NativeString;
	private Bool32 m_IsDisposed;

	public void Dispose()
	{
		if (!m_IsDisposed)
		{
			if (m_NativeString != IntPtr.Zero)
			{
				Marshal.FreeCoTaskMem(m_NativeString);
				m_NativeString = IntPtr.Zero;
			}

			m_IsDisposed = true;
		}

		GC.SuppressFinalize(this);
	}

	public override string? ToString() => this;

	public static NativeString Null() => new NativeString(){ m_NativeString = IntPtr.Zero };

	public static implicit operator NativeString(string? InString) => new(){ m_NativeString = Marshal.StringToCoTaskMemAuto(InString) };
	public static implicit operator string?(NativeString InString) => Marshal.PtrToStringAuto(InString.m_NativeString);
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct Bool32
{
	public uint Value { get; set; }

	public static implicit operator Bool32(bool InValue) => new() { Value = InValue ? 1u : 0u };
	public static implicit operator bool(Bool32 InBool32) => InBool32.Value > 0;
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct ReflectionType
{
	private readonly int m_TypeId;

	public int ID => m_TypeId;

	public ReflectionType(int InTypeID)
	{
		m_TypeId = InTypeID;
	}

	public static implicit operator ReflectionType(Type? InType) => new(TypeInterface.s_CachedTypes.Add(InType));

}
