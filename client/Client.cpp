#include "Client.h"

// �R���X�g���N�^
CClient::CClient():m_DstSocket(INVALID_SOCKET)
{
	// Windows �̏ꍇ
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data);
}
// �f�X�g���N�^
CClient::~CClient(){
	if(m_DstSocket != INVALID_SOCKET)
		closesocket(m_DstSocket);
	// Windows �ł̃\�P�b�g�̏I��
	WSACleanup();
}

// �ڑ�
bool CClient::Connect(const char* IP,u_short PORT)
{
	// sockaddr_in �\���̂̃Z�b�g
	struct sockaddr_in dstAddr;
	memset(&dstAddr, 0, sizeof(dstAddr));
	dstAddr.sin_port = htons(PORT);
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_addr.s_addr = inet_addr(IP);

	// �\�P�b�g�̐���
	m_DstSocket = socket(AF_INET, SOCK_STREAM, 0);

	// �ڑ�
	if(connect(m_DstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr)) 
		== SOCKET_ERROR){
		return false;
	}
	// �\�P�b�g��񓯊����[�h�ɂ���
	u_long val=1;
	ioctlsocket(m_DstSocket, FIONBIO, &val);
	return true;
}


// ��M
RECVSTATUS CClient::Recv(char* pData,int DataSize,int *pRecvSize)
{
	
	int n = recv(m_DstSocket, pData, DataSize, 0);
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
bool CClient::Send(char* pData,int DataSize)
{
	//�p�P�b�g�̑��M
	if(send(m_DstSocket, pData, DataSize, 0) == SOCKET_ERROR)
		return false;

	return true;
}