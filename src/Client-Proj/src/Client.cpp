#include "Client.h"

Client::Client()
{
	startup();

	resolveAddress();

	connect();
}

void Client::send(const char* buf, const int len) const
{
	int iResult = 0;

	REPEAT_THREE_TIMES(
		{
			iResult = ::send(ConnectSocket, buf, len, 0);
			if (iResult > 0)
				break;
		}
	)
		if (iResult == SOCKET_ERROR) {
			printf("Server Responded with an Error:\n"
				"send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			_exit(ERR);
		}
}

void Client::recv(char* buf, const int len) const
{
	int iResult = 0;
	REPEAT_THREE_TIMES(
		{
			iResult = ::recv(ConnectSocket, buf, len, 0);
			if (iResult > 0)
				break;
		}
	)

	if (iResult == 0) {
			printf("Connection closed\n");
	}
	else if (iResult == SOCKET_ERROR) {
		printf("Server Responded with an Error:\n"
			"recv failed: %d\n", WSAGetLastError());
		_exit(ERR);
	}
}

void Client::startup()
{
	// non fallible code, as it is local
	WSADATA wsaData;
	const int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		_exit(ERR);
	}
}

void Client::resolveAddress()
{
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		_exit(ERR);
	}

	clientService.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(3333);
}

void Client::connect()
{
	int iResult = 0;

	// fallible code, hence we repeat three times
	REPEAT_THREE_TIMES(
		{
			iResult = ::connect(ConnectSocket, 
				reinterpret_cast<SOCKADDR*>(&clientService), 
				sizeof(clientService));
			if (iResult == NOERROR) break;
		}
	)

	// if the fallible code failed we send an error message
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		printf("Server Responded with an Error:\n"
		 "Unable to connect to server: %ld\n", WSAGetLastError());
		WSACleanup();
		_exit(ERR);
	}
}

