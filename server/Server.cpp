#include "Server.h"

// コンストラクタ
Server::Server():m_ServerSocket(INVALID_SOCKET)
{
	// Windows の場合
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data);
}

// デストラクタ
Server::~Server()
{
	// Windows での終了設定
	WSACleanup();
}

//-------------------------------------------------------//
//	待ち受け状態
//
//	引数：PORT	待ち受けポート
//				クライアント側からこのポートに接続する
//-------------------------------------------------------//
void Server::Listen(u_short PORT)
{
	// sockaddr_in 構造体のセット
	memset(&m_ServerAddr, 0, sizeof(m_ServerAddr));
	m_ServerAddr.sin_port = htons(PORT);
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ソケットの生成（ストリーム型）
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
  	// ソケットのバインド
	bind(m_ServerSocket, (struct sockaddr *) &m_ServerAddr, sizeof(m_ServerAddr));
  	// 接続の許可
	listen(m_ServerSocket, 5);

	// ここで、ノンブロッキングに設定しています。
	// val = 0でブロッキングモードに設定できます。
	// ソケットの初期設定はブロッキングモードです。
	u_long val=1;
	ioctlsocket(m_ServerSocket, FIONBIO, &val);

}

//-------------------------------------------------------//
//	接続
//
//	引数：	pDstSocket	接続先ソケット
//			pDstAddr	接続先アドレス
//
//	戻り値：true	接続があった
//			false	接続がなかった
//-------------------------------------------------------//
bool Server::Accept(SOCKET* pDstSocket,struct sockaddr_in* pDstAddr)
{
	// クライアントアドレス
	int dstAddrSize = sizeof(struct sockaddr_in);
	*pDstSocket = accept(m_ServerSocket, (struct sockaddr *) pDstAddr, &dstAddrSize);
	if(*pDstSocket != INVALID_SOCKET){
		// 接続ソケットを非同期モードにする
		u_long val=1;
		ioctlsocket(*pDstSocket, FIONBIO, &val);
		return true;
	}

	return false;
}



// 受信
RECVSTATUS Server::Recv(SOCKET DstSocket,char* pData,int DataSize,int *pRecvSize)
{
	
	int n = recv(DstSocket, pData, DataSize, 0);
	if (n < 1) {
		// データが来ていない
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return RECV_STILL;
		// 切断orエラー
		} else {
			return RECV_FAILED;
		}
	} 
	*pRecvSize = n;	// 受信データ長取得
	return RECV_SUCCESSED;
}
// 送信
bool Server::Send(SOCKET DstSocket,char* pData,int DataSize)
{
	// パケットの送信
	if(send(DstSocket, pData, DataSize, 0) == SOCKET_ERROR)
		return false;

	return true;
}

// 切断
void Server::Close(SOCKET DstSocket)
{
	closesocket(DstSocket);
}