
#include <Windows.h>

#include <wrl/implements.h>

using namespace Microsoft::WRL;

// https://msdn.microsoft.com/en-us/library/ms228772.aspx

class MessageFilter : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IMessageFilter>
{
	//
	// Class containing the IOleMessageFilter
	// thread error-handling functions.

	// Start the filter.
public:
	// IOleMessageFilter functions.
	// Handle incoming thread requests.
	virtual DWORD STDMETHODCALLTYPE HandleInComingCall(DWORD dwCallType, HTASK htaskCaller, DWORD dwTickCount, LPINTERFACEINFO lpInterfaceInfo)
	{
		return SERVERCALL_ISHANDLED;
	}

	// Thread call was rejected, so try again.
	virtual DWORD STDMETHODCALLTYPE RetryRejectedCall(HTASK htaskCallee, DWORD dwTickCount, DWORD dwRejectType)
	{
		if (dwRejectType == SERVERCALL_RETRYLATER)
		{
			// Retry the thread call immediately if return >=0 &
			// <100.
			return 99;
		}
		// Too busy; cancel call.
		return -1;
	}


	virtual DWORD STDMETHODCALLTYPE MessagePending(_In_  HTASK htaskCallee, _In_  DWORD dwTickCount, _In_  DWORD dwPendingType)
	{
		//Return the flag .
		return PENDINGMSG_WAITDEFPROCESS;
	}
};


__declspec(dllexport) int __stdcall Register()
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
	ComPtr<MessageFilter> newFilter = Make<MessageFilter>();
	IMessageFilter* oldFilter = nullptr;
	CoRegisterMessageFilter(newFilter.Get(), &oldFilter);
	if (oldFilter) {
		oldFilter->Release();
	}
	return 0;
}

// Done with the filter, close it.
__declspec(dllexport) int __stdcall Revoke()
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
	ComPtr<IMessageFilter> oldFilter;
	CoRegisterMessageFilter(nullptr, &oldFilter);
	return 0;
}

