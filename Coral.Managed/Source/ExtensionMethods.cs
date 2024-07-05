using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Coral.Managed;

public static class ExtensionMethods
{
	public static bool IsDelegate(this Type InType)
	{
		return typeof(MulticastDelegate).IsAssignableFrom(InType.BaseType);
	}

	public static int GetSize(this Type InType)
	{
        if (InType!.IsValueType)
            return Marshal.SizeOf(InType);

        return IntPtr.Size;
    }

    // from https://stackoverflow.com/a/26531957
    public static Array Resize(this Array InArray, int[] InNewSizes)
    {
        if (InNewSizes.Length != InArray.Rank)
            throw new ArgumentException("Array must have the same number of dimensions as there are elements in InNewSizes", "InNewSizes");

        Array temp = Array.CreateInstance(InArray.GetType().GetElementType()!, InNewSizes);
        int[] sizesToCopy = new int[InNewSizes.Length];
        for (int i = 0; i < sizesToCopy.Length; i++)
            sizesToCopy[i] = Math.Min(InNewSizes[i], InArray.GetLength(i));

        int currentPosition = 0;
        CopyArray(InArray, temp, sizesToCopy, ref currentPosition);

        return temp;
    }

    private static void CopyArray(Array InArray, Array OutTemp, int[] InSizesToCopy, ref int CurrentPosition, int InDimmension = 0)
    {
        if (InArray.Rank - 1 == InDimmension)
        {
            unsafe
            {
                int typeSize = InArray.GetType().GetElementType()!.GetSize();
                int srcOffset = CurrentPosition * (InArray.GetLength(InDimmension)) * typeSize;
                IntPtr src = (IntPtr)Unsafe.AsPointer(ref MemoryMarshal.GetArrayDataReference(InArray)) + srcOffset;
                int destOffset = CurrentPosition * (OutTemp.GetLength(InDimmension)) * typeSize;
                IntPtr dest = (IntPtr)Unsafe.AsPointer(ref MemoryMarshal.GetArrayDataReference(OutTemp)) + destOffset;
                Buffer.MemoryCopy(
                    (void*)src,
                    (void*)dest,
                    OutTemp.GetLength(InDimmension) * typeSize,
                    InSizesToCopy[InDimmension] * typeSize);
            }
        }
        else
        {
            for (int i = 0; i < InSizesToCopy[InDimmension]; i++)
            {
                CurrentPosition += i;
                CopyArray(InArray, OutTemp, InSizesToCopy, ref CurrentPosition, InDimmension + 1);
            }
        }
    }
}
