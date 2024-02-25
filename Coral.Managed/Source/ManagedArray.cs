using Coral.Managed.Interop;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Coral.Managed;

using static ManagedHost;

internal static class ManagedArray
{
    [UnmanagedCallersOnly]
    internal static unsafe void CreateArray(int InTypeID, IntPtr InLengths, int InLengthsCount, Marshalling.ArrayContainer* OutArray)
    {
        try
        {
            if (OutArray == null)
                return;

            if (!TypeInterface.s_CachedTypes.TryGetValue(InTypeID, out var type))
                LogMessage($"Failed to find type with id '{InTypeID}'.", MessageLevel.Error);

            if (InLengths == IntPtr.Zero || InLengthsCount == 0) 
            {
                // NOTE (infirit89): this error message is a bit misleading
                LogMessage($"Cannot create an array with length 0", MessageLevel.Error);
                return;
            }

            var lengths = new NativeArray<int>(InLengths, InLengthsCount);
            
            Array result = Array.CreateInstance(type!, lengths.ToArray());
            var handle = GCHandle.Alloc(result, GCHandleType.Normal);

            int typeAssemblyId = type!.Assembly.GetName().Name!.GetHashCode();
            if (!AssemblyLoader.TryGetAssembly(typeAssemblyId, out var assembly))
                assembly = Assembly.GetExecutingAssembly();
            
            AssemblyLoader.RegisterHandle(assembly!, handle);
            (*OutArray).Data = GCHandle.ToIntPtr(handle);
            (*OutArray).Length = result.Rank;
        }
        catch (Exception ex) 
        {
            HandleException(ex);
        }
    }

    [UnmanagedCallersOnly]
    internal static void DestroyObject(IntPtr InArrayHandle) 
    {
        try
        {
            GCHandle.FromIntPtr(InArrayHandle).Free();
        }
        catch (Exception ex) 
        {
            HandleException(ex);
        }
    }

    [UnmanagedCallersOnly]
    internal static int GetLength(IntPtr InHandle, int InDimension) 
    {
        try
        {
            var handle = GCHandle.FromIntPtr(InHandle);
            if (handle.Target == null)
                return -1;

            return (handle.Target as Array)!.GetLength(InDimension);
        }
        catch (Exception ex) 
        {
            HandleException(ex);
            return -1;
        }
    }

    [UnmanagedCallersOnly]
    internal static void GetValue(IntPtr InArrayHandle, IntPtr InIndices, int InIndicesCount, IntPtr OutValue)
    {
        try 
        {
            var target = GCHandle.FromIntPtr(InArrayHandle).Target as Array;
            if (target == null)
            {
                LogMessage($"Cannot get value of an array with handle {InArrayHandle}. Target was null.", MessageLevel.Error);
                return;
            }

            if (InIndices == IntPtr.Zero || InIndicesCount == 0)
            {
                // NOTE (infirit89): this error message is a bit misleading
                LogMessage($"Cannot get the value of array with invalid indices", MessageLevel.Error);
                return;
            }

            var indices = new NativeArray<int>(InIndices, InIndicesCount);
            object? value = target.GetValue(indices.ToArray());
            if (value == null)
                return;

            Marshalling.MarshalObject(target, value, value.GetType(), OutValue);
        }
        catch(Exception ex)
        {
            HandleException(ex);
        }
    }

    [UnmanagedCallersOnly]
    internal static void SetValue(IntPtr InArrayHandle, IntPtr InIndices, int InIndicesCount, IntPtr InValue) 
    {
        try
        {
            var target = GCHandle.FromIntPtr(InArrayHandle).Target as Array;
            if (target == null)
            {
                LogMessage($"Cannot set value of an array with handle {InArrayHandle}. Target was null.", MessageLevel.Error);
                return;
            }

            if (InIndices == IntPtr.Zero || InIndicesCount == 0)
            {
                // NOTE (infirit89): this error message is a bit misleading
                LogMessage($"Cannot set the value of array with invalid indices", MessageLevel.Error);
                return;
            }

            var indices = new NativeArray<int>(InIndices, InIndicesCount);
            object? value = Marshalling.MarshalPointer(InValue, target.GetType().GetElementType()!);
            target.SetValue(value, indices);
        }
        catch (Exception ex) 
        {
            HandleException(ex);
        }
    }

    [UnmanagedCallersOnly]
    internal static unsafe IntPtr GetDataReference(IntPtr InArrayHandle) 
    {
        try
        {
            var target = GCHandle.FromIntPtr(InArrayHandle).Target as Array;
            if (target == null)
            {
                LogMessage($"Cannot get the data reference of an array with handle {InArrayHandle}. Target was null.", MessageLevel.Error);
                return IntPtr.Zero;
            }
            Array.Copy()

            return (IntPtr)Unsafe.AsPointer(ref MemoryMarshal.GetArrayDataReference(target));
        }
        catch (Exception ex) 
        {
            HandleException(ex);
            return IntPtr.Zero;
        }
    }

    [UnmanagedCallersOnly]
    public static unsafe void ResizeRankOne(int InNewSize, Marshalling.ArrayContainer* InContainer) 
    {
        try
        {
            GCHandle handle = GCHandle.FromIntPtr((*InContainer).Data);
            var target = handle.Target as Array;
            if (target == null) 
            {
                LogMessage($"Cannot resize an array with handle {(*InContainer).Data}. Target was null.", MessageLevel.Error);
                return;
            }

            if (target.Rank > 1) 
            {
                LogMessage($"Method ResizeRankOne cannot resize an array with a rank bigger than one for handle {(*InContainer).Data}.", MessageLevel.Error);
                return;
            }

            if (InNewSize < 0) 
            {
                LogMessage($"Invalid size argument: {InNewSize}.", MessageLevel.Error);
                return;
            }

            if (target.Length == InNewSize)
                return;

            Type elementType = target.GetType().GetElementType()!;
            Array temp = Array.CreateInstance(elementType, InNewSize);
            Buffer.MemoryCopy(
                Unsafe.AsPointer(ref MemoryMarshal.GetArrayDataReference(target)),
                Unsafe.AsPointer(ref MemoryMarshal.GetArrayDataReference(temp)),
                temp.Length * elementType.GetSize(),
                Math.Min(InNewSize, target.Length) * elementType.GetSize());

            handle.Target = temp;
        }
        catch (Exception ex)
        {
            HandleException(ex);
        }
    }

    [UnmanagedCallersOnly]
    public static unsafe void ResizeRankN(IntPtr InLengths, int InLengthsCount, Marshalling.ArrayContainer* InContainer) 
    {
        try
        {
            GCHandle handle = GCHandle.FromIntPtr((*InContainer).Data);
            var target = handle.Target as Array;
            if (target == null)
            {
                LogMessage($"Cannot resize an array with handle {(*InContainer).Data}. Target was null.", MessageLevel.Error);
                return;
            }

            var lengths = new NativeArray<int>(InLengths, InLengthsCount);
            handle.Target = target.Resize(lengths);
        }
        catch (Exception ex)
        {
            HandleException(ex);
        }
    }
}
