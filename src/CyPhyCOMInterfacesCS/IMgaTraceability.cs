using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
namespace CyPhyCOMInterfaces
{
    [Guid("2396D93E-96EF-4D7C-B6FE-8A8C9E665BA6")]
    [ComVisible(true)]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
	public interface IMgaTraceability 
	{
		[return: MarshalAs(UnmanagedType.BStr)]
		string GetMappedObject([MarshalAs(UnmanagedType.BStr)] [In] string copy);
		bool TryGetMappedObject([MarshalAs(UnmanagedType.BStr)] [In] string copy, [MarshalAs(UnmanagedType.BStr)] out string original);
		void CopyTo([MarshalAs(UnmanagedType.Interface)] [In] IMgaTraceability copy);
		void AddItem([MarshalAs(UnmanagedType.BStr)] [In] string copy, [MarshalAs(UnmanagedType.BStr)] [In] string original);
	}
}
