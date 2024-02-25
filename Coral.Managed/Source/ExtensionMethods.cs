using System;
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
            throw new ArgumentException("arr must have the same number of dimensions as there are elements in newSizes", "newSizes");

        Array temp = Array.CreateInstance(InArray.GetType().GetElementType()!, InNewSizes);
        int[] sizesToCopy = new int[InNewSizes.Length];
        for (int i = 0; i < sizesToCopy.Length; i++)
            sizesToCopy[i] = Math.Min(InNewSizes[i], InArray.GetLength(i));

        int[] currentPositions = new int[sizesToCopy.Length];
        CopyArray(InArray, temp, sizesToCopy, currentPositions);

        return temp;
    }

    private static void CopyArray(Array InArray, Array OutTemp, int[] InSizesToCopy, int[] CurrentPositions, int InDimmension = 0)
    {
        if (InArray.Rank - 1 == InDimmension)
        {
            //Copy this Array
            for (int i = 0; i < InSizesToCopy[InDimmension]; i++)
            {
                CurrentPositions[InDimmension] = i;
                OutTemp.SetValue(InArray.GetValue(CurrentPositions), CurrentPositions);
            }
        }
        else
        {
            //Recursion one dimmension higher
            for (int i = 0; i < InSizesToCopy[InDimmension]; i++)
            {
                CurrentPositions[InDimmension] = i;
                CopyArray(InArray, OutTemp, InSizesToCopy, CurrentPositions, InDimmension + 1);
            }
        }
    }
}
