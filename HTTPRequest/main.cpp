#include <Windows.h>
#include <WinInet.h>

#include <cstdio>

#pragma comment(lib, "wininet.lib")

int main()
{
	int iErrorLevel = 0;

	HINTERNET hSession = NULL;

	hSession = InternetOpenA("Agent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hSession)
	{
		iErrorLevel = GetLastError();
		printf("InternetOpenA() Error: %d\n", iErrorLevel);
	}

	HINTERNET hConnect = NULL;
	if (!iErrorLevel)
	{
		// hConnect = InternetConnectA(hSession, "127.0.0.1", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL); // for HTTPS
		hConnect = InternetConnectA(hSession, "127.0.0.1", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
		if (!hConnect)
		{
			iErrorLevel = GetLastError();
			printf("InternetConnectA() Error: %d\n", iErrorLevel);
		}
	}

	HINTERNET hRequest = NULL;
	if (!iErrorLevel)
	{
		// hRequest = HttpOpenRequestA(hConnect, "POST", "/", NULL, NULL, NULL, INTERNET_FLAG_SECURE, NULL); // for HTTPS
		hRequest = HttpOpenRequestA(hConnect, "POST", "/", NULL, NULL, NULL, 0, NULL);
		if (!hRequest)
		{
			iErrorLevel = GetLastError();
			printf("HttpOpenRequestA() Error: %d\n", iErrorLevel);
		}
	}

	if (!iErrorLevel)
	{
		if (!HttpSendRequestA(hRequest, NULL, 0, NULL, 0))
		{
			iErrorLevel = GetLastError();
			printf("HttpSendRequestA() Error: %d\n", iErrorLevel);
		}
	}	

	if (!iErrorLevel)
	{
		char aBuffer[BUFSIZ]{};
		DWORD dwBufferSize = BUFSIZ;
		DWORD dwIndex = 0;
		if (!HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE, aBuffer, &dwBufferSize, &dwIndex))
		{
			iErrorLevel = GetLastError();
			printf("HttpQueryInfoA() Error: %d\n", iErrorLevel);
		}
	}

	if (!iErrorLevel)
	{
		char aBuffer[BUFSIZ]{};
		for (;;)
		{
			DWORD dwBytesRead = 0;
			if (!InternetReadFile(hRequest, aBuffer, BUFSIZ, &dwBytesRead))
			{
				iErrorLevel = GetLastError();
				printf("InternetReadFile() Error: %d\n", iErrorLevel);
				break;
			}		

			if (!dwBytesRead)
			{
				break;
			}

			printf("%s", aBuffer);

			ZeroMemory(aBuffer, sizeof(aBuffer));
		}
	}

	if (hRequest)
	{
		if (!InternetCloseHandle(hRequest))
		{
			iErrorLevel = GetLastError();
			printf("InternetCloseHandle(hRequest) Error: %d\n", iErrorLevel);
		}
		else
		{
			hConnect = NULL;
		}
	}

	if (hConnect)
	{
		if (!InternetCloseHandle(hConnect))
		{
			iErrorLevel = GetLastError();
			printf("InternetCloseHandle(hConnect) Error: %d\n", iErrorLevel);
		}
		else
		{
			hConnect = NULL;
		}
	}

	if (hSession)
	{
		if (!InternetCloseHandle(hSession))
		{
			iErrorLevel = GetLastError();
			printf("InternetCloseHandle(hSession) Error: %d\n", iErrorLevel);
		}
		else
		{
			hSession = NULL;
		}		
	}

	return iErrorLevel;
}
