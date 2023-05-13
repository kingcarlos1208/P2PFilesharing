#include <iostream>
#include <WinSock2.h>
#include <conio.h>
#include <string>

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

const int PORT = 9000;
const int buffersize = 1024;

char buffer[buffersize];
int Check_received;
int iResult;
COORD coord;
WSADATA wsaData;
sockaddr_in addr;
SOCKET sock;
SOCKET ListenSocket;

bool first = false;

typedef struct {
	string ip;
	string Data;
}Mail;

class Messanger {
private:
	string firstmessage;
	Mail bodymessage;
	char MessageData[1024]=" ";
	

public:
	Messanger();
	void ShowMessage();
	void Home();
	void savedata();
	int SendingMessage();
	void reception();
};

Messanger::Messanger()
{
	firstmessage = "Press the key 'A' to send a message or 'B' to view a message\n";
	std::string ip = "xxx.xxx.xxx";
	std::string Data = "Enter body text here...";
	cout << "Let's start to press the key 'A'\n";
}

void Messanger::Home()
{
	bool isEnter=false;
	system("cls");
	if (first == false)
	{
		cout << "Press the key 'A' to send a message or 'B' to view a message\n";
	}

	while (TRUE)
	{
		if (GetAsyncKeyState('A') & 0x8000) {
			SendingMessage();
			break;
		}
	}

		/*switch (_getch())
		{
		case 'a': SendingMessage(); break;
		case 'b': ShowMessage(); break;
		default: system("cls");  cout << "Unrecognized.";
		}*/
}

void Messanger::savedata()
{

}

int Messanger::SendingMessage()
{
	// �T�[�o�[�ɐڑ����邽�߂̏��̐ݒ�
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr("192.168.2.101");

	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "socket failed: " << WSAGetLastError();
		WSACleanup();
	}

	// �T�[�o�[�ɐڑ�
	iResult = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (iResult == SOCKET_ERROR) {
		cout << "connect failed: " << WSAGetLastError();
		closesocket(sock);
		WSACleanup();
	}

	// ���b�Z�[�W�̓���
	cout << "Enter message: ";
	string message;
	getline(cin, message);

	// ���b�Z�[�W�̑��M
	iResult = send(sock, message.c_str(), message.length(), 0);
	if (iResult == SOCKET_ERROR) {
		cout << "send failed: " << WSAGetLastError();
		closesocket(sock);
		WSACleanup();
	}

	// �\�P�b�g�̃N���[�Y
	closesocket(sock);


	return 0;
}

void Messanger::reception()
{
	
	//�\�P�b�g�̃��X�j���O
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		cout << "listen failed:" << WSAGetLastError();
		closesocket(ListenSocket);
		WSACleanup();
	}

	//�N���C�A���g�̐ڑ��҂�
	SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		cout << "accept failed: " << WSAGetLastError();
		closesocket(ListenSocket);
		WSACleanup();
	}
	//��M����
	char recvbuf[1024];
	int recvbuflen = 1024;
	Check_received = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0) {
		cout << "Received: " << recvbuf;
	}
	else if (iResult == 0) {
		cout << "Connecting closing...";
	}
	else {
		cout << "recv failed: " << WSAGetLastError();
	}
}

void Messanger::ShowMessage()
{
	system("cls");
	if (MessageData != NULL)
	{
		cout << MessageData << "\n";
	}
}

int main()
{	
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed:" << iResult;
	}
	//�\�P�b�g�̍쐬
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		cout << "socket filed: " << WSAGetLastError();
		WSACleanup();
	}

	//�\�P�b�g�̃o�C���h
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	//addr.sin_addr.s_addr = inet_addr("192.168.2.101");  // �R�����g�A�E�g
	addr.sin_addr.s_addr = INADDR_ANY;
	iResult = bind(ListenSocket, (SOCKADDR*)&addr, sizeof(addr));
	
	if (iResult == SOCKET_ERROR)
	{
		cout << "bind failed:" << WSAGetLastError();
		WSACleanup();
	}

	Messanger Ms;
	while (true) {
		if (GetAsyncKeyState('A') & 0x8000) {
			Ms.Home();
			break;
		}
		switch (_getch())
		{
		case 'a': Ms.Home(); break;
		}
		while (TRUE)
		{
			switch (_getch())
			{
			case 'h': Ms.Home();
			}
			Ms.reception();
		}
		WSACleanup();
	}
}
