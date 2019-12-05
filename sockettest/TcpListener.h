#pragma once
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE (49152)

class CTcpListner;

typedef void (*MessageRecievedHandler)(CTcpListner* listener, int socketId, std::string msg);

class CTcpListner {
public:
	CTcpListner(std::string ipAddress, int port, MessageRecievedHandler handler);
	
	~CTcpListner();

	//send a message to the spec. client
	void Send(int clientSocket, std::string msg);

	//init winsock
	bool Init();

	//main proc. loop
	void Run();
	void Cleanup();

private:
	std::string m_ipAddress;
	int			m_port;
	MessageRecievedHandler MessageReceived;

	//create SOCK and wait for conn.
	SOCKET CreateSocket();
	SOCKET WaitForConnection(SOCKET listening);
};
