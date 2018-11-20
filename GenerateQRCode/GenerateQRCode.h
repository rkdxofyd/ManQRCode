#pragma once

#ifdef __cplusplus
#if (GENERATE_QRCODE_IMPLEMENT)
#define DLL_FUNCTION extern "C" __declspec(dllexport)
#else
#define DLL_FUNCTION extern "C" __declspec(dllimport)
#endif
#else
#if (GENERATE_QRCODE_IMPLEMENT)
#define DLL_FUNCTION __declspec(dllexport)
#else
#define DLL_FUNCTION __declspec(dllimport)
#endif
#endif

typedef struct _GENERATED_QR_CODE_DATA_
{
	UINT width;
	UINT height;
	UINT stride;
	unsigned char *data;
} GENERATED_QR_CODE;

DLL_FUNCTION
int __stdcall GenerateQRCode(const wchar_t *content, GENERATED_QR_CODE *pQR_Data);

DLL_FUNCTION
int __stdcall ReleaseQRCodeData(GENERATED_QR_CODE *pQR_Data);