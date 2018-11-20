#define GENERATE_QRCODE_IMPLEMENT 1
#include <Windows.h>
#include <GdiPlus.h>
#include "GdiplusInit.h"
#include "BarcodeGenerator.h"

#include "GenerateQRCode.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "ZXingCore.lib")
#pragma comment(lib, "ZXingGdiPlus.lib")

HANDLE g_qrMutex;


BOOL WINAPI DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_qrMutex = CreateMutex(NULL, FALSE, NULL);
		break;
	case DLL_PROCESS_DETACH:
		CloseHandle(g_qrMutex);
		break;
	}
	return TRUE;
}

DLL_FUNCTION
int __stdcall GenerateQRCode(const wchar_t *content, GENERATED_QR_CODE *pQR_Data)
{
	int rc = -1;
	if (pQR_Data == nullptr)
	{
		return -1;
	}
	if (pQR_Data->data)
	{
		return -1;
	}
	if (WAIT_OBJECT_0 == WaitForSingleObject(g_qrMutex, INFINITE))
	{
		try
		{
			ZXing::GdiplusInit gdiplus_init;
			ZXing::BarcodeGenerator generator("QR_CODE");

			std::shared_ptr<Gdiplus::Bitmap> bmp = generator.generate(content, 0, 0);

			Gdiplus::Rect r;
			r.X = 0;
			r.Y = 0;
			r.Width = bmp->GetWidth();
			r.Height = bmp->GetHeight();

			Gdiplus::BitmapData bmpData;

			Gdiplus::Status sts;
			sts = bmp->LockBits(&r, Gdiplus::ImageLockModeRead, PixelFormat24bppRGB, &bmpData);
			if (sts == Gdiplus::Ok)
			{
				pQR_Data->width = bmpData.Width;
				pQR_Data->height = bmpData.Height;
				pQR_Data->stride = bmpData.Stride;
				pQR_Data->data = new unsigned char[pQR_Data->stride * pQR_Data->height];
				if (pQR_Data->data)
				{
					memcpy(pQR_Data->data, bmpData.Scan0, pQR_Data->stride * pQR_Data->height);
					rc = 0;
				}
				bmp->UnlockBits(&bmpData);
			}
		}
		catch (...)
		{
			rc = -1;
		}
		ReleaseMutex(g_qrMutex);
	}
	return rc;
}

DLL_FUNCTION
int __stdcall ReleaseQRCodeData(GENERATED_QR_CODE *pQR_Data)
{
	int rc = -1;
	if (pQR_Data && pQR_Data->data)
	{
		delete[] pQR_Data->data;
	}
	return rc;
}