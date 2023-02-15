//WSAStartup	開始
//socket		ソケット作成
//connect		サーバに接続
//ioctlsocket	ソケット非同期化
//recv,send		データ送受信
//closesocket	ソケット閉じる
//WSACleanup	終了

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

// クライアントクラス
class CClient
{
private:
	SOCKET	m_DstSocket;	// 送受信ソケット

public:
	// コンストラクタ
	CClient();
	// デストラクタ
	virtual ~CClient();

	// 接続
	bool Connect(const char* IP,u_short PORT);
	
	// 受信
	RECVSTATUS Recv(char* pData,int DataSize,int *pRecvSize);
	// 送信
	bool Send(char* pData,int DataSize);
};