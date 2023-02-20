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

// �T�[�o�[�N���X
class Server
{
private:
	SOCKET				m_ServerSocket;	// �T�[�o���\�P�b�g
	struct sockaddr_in	m_ServerAddr;	// �T�[�o�A�h���X

public:
	// �R���X�g���N�^
	Server();
	// �f�X�g���N�^
	virtual ~Server();

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
}