#include "Server.h"

// �R���X�g���N�^
Server::Server():m_ServerSocket(INVALID_SOCKET)
{
	// Windows �̏ꍇ
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data);
}

// �f�X�g���N�^
Server::~Server()
{
	// Windows �ł̏I���ݒ�
	WSACleanup();
}

//-------------------------------------------------------//
//	�҂��󂯏��
//
//	�����FPORT	�҂��󂯃|�[�g
//				�N���C�A���g�����炱�̃|�[�g�ɐڑ�����
//-------------------------------------------------------//
void Server::Listen(u_short PORT)
{
	// sockaddr_in �\���̂̃Z�b�g
	memset(&m_ServerAddr, 0, sizeof(m_ServerAddr));
	m_ServerAddr.sin_port = htons(PORT);
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// �\�P�b�g�̐����i�X�g���[���^�j
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
  	// �\�P�b�g�̃o�C���h
	bind(m_ServerSocket, (struct sockaddr *) &m_ServerAddr, sizeof(m_ServerAddr));
  	// �ڑ��̋���
	listen(m_ServerSocket, 5);

	// �����ŁA�m���u���b�L���O�ɐݒ肵�Ă��܂��B
	// val = 0�Ńu���b�L���O���[�h�ɐݒ�ł��܂��B
	// �\�P�b�g�̏����ݒ�̓u���b�L���O���[�h�ł��B
	u_long val=1;
	ioctlsocket(m_ServerSocket, FIONBIO, &val);

}

//-------------------------------------------------------//
//	�ڑ�
//
//	�����F	pDstSocket	�ڑ���\�P�b�g
//			pDstAddr	�ڑ���A�h���X
//
//	�߂�l�Ftrue	�ڑ���������
//			false	�ڑ����Ȃ�����
//-------------------------------------------------------//
bool Server::Accept(SOCKET* pDstSocket,struct sockaddr_in* pDstAddr)
{
	// �N���C�A���g�A�h���X
	int dstAddrSize = sizeof(struct sockaddr_in);
	*pDstSocket = accept(m_ServerSocket, (struct sockaddr *) pDstAddr, &dstAddrSize);
	if(*pDstSocket != INVALID_SOCKET){
		// �ڑ��\�P�b�g��񓯊����[�h�ɂ���
		u_long val=1;
		ioctlsocket(*pDstSocket, FIONBIO, &val);
		return true;
	}

	return false;
}



// ��M
RECVSTATUS Server::Recv(SOCKET DstSocket,char* pData,int DataSize,int *pRecvSize)
{
	
	int n = recv(DstSocket, pData, DataSize, 0);
	if (n < 1) {
		// �f�[�^�����Ă��Ȃ�
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return RECV_STILL;
		// �ؒfor�G���[
		} else {
			return RECV_FAILED;
		}
	} 
	*pRecvSize = n;	// ��M�f�[�^���擾
	return RECV_SUCCESSED;
}
// ���M
bool Server::Send(SOCKET DstSocket,char* pData,int DataSize)
{
	// �p�P�b�g�̑��M
	if(send(DstSocket, pData, DataSize, 0) == SOCKET_ERROR)
		return false;

	return true;
}

// �ؒf
void Server::Close(SOCKET DstSocket)
{
	closesocket(DstSocket);
}