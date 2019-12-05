#include <iostream>
#include <string>
#include "TcpListener.h"
using namespace std;

void Listener_MessageReceived(CTcpListner* listner, int client, string msg);

void main() {
	CTcpListner server("192.168.21.85", 60000, Listener_MessageReceived);
	if (server.Init()) {
		server.Run();
	}
	else {
		server.~CTcpListner();
	}

}

void Listener_MessageReceived(CTcpListner* listner, int client, string msg) {
	
	listner->Send(client, msg);

}