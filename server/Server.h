#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

// 受信状態
enum RECVSTATUS
{
	RECV_STILL,		// データが来ていない
	RECV_SUCCESSED,	// 成功
	RECV_FAILED		// 切断orエラー
};

// サーバークラス
class Server
{
private:
	SOCKET				m_ServerSocket;	// サーバ側ソケット
	struct sockaddr_in	m_ServerAddr;	// サーバアドレス

public:
	// コンストラクタ
	Server();
	// デストラクタ
	virtual ~Server();

	//-------------------------------------------------------//
	//	待ち受け状態
	//
	//	引数：PORT	待ち受けポート
	//				クライアント側からこのポートに接続する
	//-------------------------------------------------------//
	void Listen(u_short PORT);
	//-------------------------------------------------------//
	//	接続
	//
	//	引数：	pDstSocket	接続先ソケット
	//			pDstAddr	接続先アドレス
	//
	//	戻り値：true	接続があった
	//			false	接続がなかった
	//-------------------------------------------------------//
	bool Accept(SOCKET* pDstSocket,struct sockaddr_in* pDstAddr);
	
	// 受信
	RECVSTATUS Recv(SOCKET DstSocket,char* pData,int DataSize,int *pRecvSize);
	// 送信
	bool Send(SOCKET DstSocket,char* pData,int DataSize);
	// 切断
	void Close(SOCKET DstSocket);
}