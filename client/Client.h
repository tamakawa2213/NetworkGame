//------------------------------------//
//	�N���C�A���g�N���X
//	TCP/IP�ɂ��ڑ�
//
//	�쐬�ҁFDaiki Terai
//	�쐬���F2011/02/05
//	�C�������F
//------------------------------------//

#ifndef _CLIENT_H_
#define _CLIENT_H_

//WSAStartup	�J�n
//socket		�\�P�b�g�쐬
//connect		�T�[�o�ɐڑ�
//ioctlsocket	�\�P�b�g�񓯊���
//recv,send		�f�[�^����M
//closesocket	�\�P�b�g����
//WSACleanup	�I��

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#ifndef _RECVSTATUS_
#define _RECVSTATUS_

// ��M���
enum RECVSTATUS
{
	RECV_STILL,		// �f�[�^�����Ă��Ȃ�
	RECV_SUCCESSED,	// ����
	RECV_FAILED		// �ؒfor�G���[
};

#endif

// �N���C�A���g�N���X
class CClient
{
private:
	SOCKET	m_DstSocket;	// ����M�\�P�b�g

public:
	// �R���X�g���N�^
	CClient();
	// �f�X�g���N�^
	virtual ~CClient();

	// �ڑ�
	bool Connect(const char* IP,u_short PORT);
	
	// ��M
	RECVSTATUS Recv(char* pData,int DataSize,int *pRecvSize);
	// ���M
	bool Send(char* pData,int DataSize);
};

#endif