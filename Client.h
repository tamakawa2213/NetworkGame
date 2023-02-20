#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

// ��M���
enum RECVSTATUS
{
	RECV_STILL,		// �f�[�^�����Ă��Ȃ�
	RECV_SUCCESSED,	// ����
	RECV_FAILED		// �ؒfor�G���[
};

// �N���C�A���g�N���X
class Client
{
private:
	SOCKET	m_DstSocket;	// ����M�\�P�b�g

public:
	// �R���X�g���N�^
	Client();
	// �f�X�g���N�^
	virtual ~Client();

	// �ڑ�
	bool Connect(const char* IP,u_short PORT);
	
	// ��M
	RECVSTATUS Recv(char* pData,int DataSize,int *pRecvSize);
	// ���M
	bool Send(char* pData,int DataSize);
};