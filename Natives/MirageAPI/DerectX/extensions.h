#pragma once

#define GetContextDevice() \
DX12Context::s_device == nullptr\
? throw gcnew System::InvalidOperationException("DX12 device not initialized") \
: DX12Context::s_device

#define DX12_CHECK(device_ptr, hr, context_msg) \
do { \
if ((hr) == DXGI_ERROR_DEVICE_REMOVED) { \
HRESULT __reason = (device_ptr)->GetDeviceRemovedReason(); \
throw gcnew System::Exception(context_msg + ": Device removed (" + __reason + ")"); \
} \
else if (FAILED(hr)) { \
throw gcnew System::Exception(context_msg + ": " + (hr)); \
} \
} while(0)
