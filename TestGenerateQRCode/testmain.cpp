#include <Windows.h>
#include <cstdlib>
#include <cstdio>
#include "GenerateQRCode.h"

#pragma comment(lib, "GenerateQRCode.lib")

int main(int argc, char *argv[])
{
	int rc = 0;
	GENERATED_QR_CODE qr_data;
	memset(&qr_data, 0, sizeof(qr_data));
	rc = GenerateQRCode(L"tttttttttttttttttttt;;;4;;;0;;;100", &qr_data);

	printf("%d\n", rc);
	return 0;
}