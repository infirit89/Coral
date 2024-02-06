using Coral.Managed.Interop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Coral.Managed;

using static ManagedHost;

internal static class ManagedArray
{
    internal struct ManagedArrayContainer 
    {
        public IntPtr Handle;
        public int Rank;
    }

    [UnmanagedCallersOnly]
    internal static unsafe void CreateArray(int InTypeID, IntPtr InLengths, int InLengthsCount, ManagedArrayContainer* OutArray)
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
            }

            var lengths = new NativeArray<int>(InLengths, InLengthsCount);
            Console.WriteLine(string.Join(' ', lengths.ToArray()));

            Array result = Array.CreateInstance(type!, lengths.ToArray());
            var handle = GCHandle.Alloc(result, GCHandleType.Normal);
            AssemblyLoader.RegisterHandle(type!.Assembly, handle);
            (*OutArray).Handle = GCHandle.ToIntPtr(handle);
            (*OutArray).Rank = result.Rank;
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
}
