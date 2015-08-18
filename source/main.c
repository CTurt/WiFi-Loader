#include <stdio.h>

#include <windows.h>
#include <winsock.h>

#ifndef WINVER
	#define WINVER 0x0500
#endif

#define MAKE_IP(a, b, c, d) (((a) << 24) + ((b) << 16) + ((c) << 8) + (d))

#define IP MAKE_IP(192, 168, 0, 14)
#define PORT 9023
#define PAYLOAD "payload.bin"

int main(void) {
	FILE *f;
	char *buffer;
	int length;
	
	int server;
	struct sockaddr_in serverAddress;
	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(IP);
	serverAddress.sin_port = htons(PORT);
	
	connect(server, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	f = fopen(PAYLOAD, "rb");
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	buffer = malloc(length);
	rewind(f);
	fread(buffer, length, 1, f);
	fclose(f);
	
	sendto(server, buffer, length, 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	printf("Sent %d bytes\n", length);
	
	free(buffer);
	
	closesocket(server);
	
	WSACleanup();
	
	return 0;
}
