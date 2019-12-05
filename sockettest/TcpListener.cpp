#include "TcpListener.h"


CTcpListner::CTcpListner(std::string ipAddress, int port, MessageRecievedHandler handler)
	: m_ipAddress(ipAddress), m_port(port), MessageReceived(handler)
{
	
}

CTcpListner::~CTcpListner() {
	Cleanup();
}

//send a message to the spec. client
void CTcpListner::Send(int clientSocket, std::string msg) {
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

//init winsock
bool CTcpListner::Init() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsInit = WSAStartup(ver, &data);
	return wsInit == 0;
}

//main proc. loop
void CTcpListner::Run() {
	char buffer[MAX_BUFFER_SIZE];
	while (true) {
		//create a list. socket. Wait for client conn. loop receive/send.
		SOCKET listening = CreateSocket();
		if (listening == INVALID_SOCKET) {
			break;
		}
		SOCKET client = WaitForConnection(listening);
		if (client != INVALID_SOCKET) {
			closesocket(listening);
			int bytesReceived = 0;
			do {
				ZeroMemory(buffer, MAX_BUFFER_SIZE);
				bytesReceived = recv(client, buffer,sizeof(buffer),0);
				if (bytesReceived > 0) {
					if (MessageReceived != NULL) {
						MessageReceived(this, client, std::string(buffer, 0, bytesReceived));
					}
				}

			} while (bytesReceived > 0);
			closesocket(client);
		}
	} 
}
void CTcpListner::Cleanup() {
	WSACleanup();
}

//create SOCK and wait for conn.
SOCKET CTcpListner::CreateSocket() {
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening != INVALID_SOCKET) {
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);
		int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOk != SOCKET_ERROR) {
			int ListenOk = listen(listening, SOMAXCONN);
			if (ListenOk == SOCKET_ERROR) {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	return listening;
}
SOCKET CTcpListner::WaitForConnection(SOCKET listening) {
	SOCKET client = accept(listening, NULL, NULL);
	return client;
}

