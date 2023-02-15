#ifndef _SERVER_H_
#define _SERVER_H_

//WSAStartup	�J�n
//socket		�\�P�b�g�쐬
//bind			�A�h���X�ƃo�C���h
//listen		���X���\�P�b�g��
//ioctlsocket	���X���\�P�b�g�񓯊���
//accept		�\�P�b�g�ڑ�
//ioctlsocket	�ڑ��\�P�b�g�񓯊���
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

// �T�[�o�[�N���X
class CServer
{
private:
	SOCKET				m_ServerSocket;	// �T�[�o���\�P�b�g
	struct sockaddr_in	m_ServerAddr;	// �T�[�o�A�h���X

public:
	// �R���X�g���N�^
	CServer();
	// �f�X�g���N�^
	virtual ~CServer();

	//-------------------------------------------------------//
	//	�҂��󂯏��
	//
	//	�����FPORT	�҂��󂯃|�[�g
	//				�N���C�A���g�����炱�̃|�[�g�ɐڑ�����
	//-------------------------------------------------------//
	void Listen(u_short PORT);
	//-------------------------------------------------------//
	//	�ڑ�
	//
	//	�����F	pDstSocket	�ڑ���\�P�b�g
	//			pDstAddr	�ڑ���A�h���X
	//
	//	�߂�l�Ftrue	�ڑ���������
	//			false	�ڑ����Ȃ�����
	//-------------------------------------------------------//
	bool Accept(SOCKET* pDstSocket,struct sockaddr_in* pDstAddr);
	
	// ��M
	RECVSTATUS Recv(SOCKET DstSocket,char* pData,int DataSize,int *pRecvSize);
	// ���M
	bool Send(SOCKET DstSocket,char* pData,int DataSize);
	// �ؒf
	void Close(SOCKET DstSocket);
};

#endif