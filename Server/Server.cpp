// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

DWORD WINAPI ClientThread(LPVOID);

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)& addr, sizeof(addr));
	listen(listener, 5);

	while (1)
	{
		printf("Waiting for new client...\n");
		SOCKET client = accept(listener, NULL, NULL);
		printf("New client accepted: %d\n", client);

		CreateThread(0, 0, ClientThread, &client, 0, 0);
		const char* okMsg = "Press R to record, press S to stop \n";
		send(client, okMsg, strlen(okMsg), 0);
	}

}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;

	char buf[256];
	int ret;

	while (1) {
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			closesocket(client);
			return 0;
		}
		buf[ret] = 0;
		printf("Received: %s\n", buf);
	}
}

