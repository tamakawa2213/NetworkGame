#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <time.h>
#include <list>
#include <vector>
#include "Server.h"
#include "../Packet.h"

#define PORT 8080			// ポート番号、クライアントプログラムと合わせる

int main()
{
	CServer server;
	server.Listen(PORT);
	printf("サーバ待ち受け状態です\nクライアントプログラムを起動してください\n");
	SOCKET dstsock;
	struct sockaddr_in	dstaddr;
	std::list<Character>	playerlist;
	std::vector<SOCKET>	deletearray;
	std::vector<Character>	movearray;
	int	recvsize;
	char data[MAX_PACKET];


	while(true)
	{
		// 接続処理
		if(server.Accept(&dstsock,&dstaddr)){
			printf("accept %s sock = %f\n",inet_ntoa(dstaddr.sin_addr),dstsock);
			
			Character player;
			player.sock = dstsock;
			player.cmd = CMD_LOGIN;
			player.pos = DirectX::XMFLOAT3(0, 0, 0);
			// ログイン情報を接続先に送信
			memcpy(data,&player,sizeof(Character));
			server.Send(dstsock,data,sizeof(data));

			// ログイン情報を他の全クライアントに送信
			std::list<Character>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
			playerlist.push_back(player);
		}

		// 一括受信処理
		std::list<Character>::iterator it;
		deletearray.clear();
		movearray.clear();
		for(it = playerlist.begin();it != playerlist.end();)
		{
			RECVSTATUS status = server.Recv(it->sock,data,sizeof(data),&recvsize);

			switch(status)
			{
			case RECV_SUCCESSED:
				Character player;
				memcpy(&player,data,sizeof(Character));
				movearray.push_back(*it);
				printf("Socket = %llu x = %f y = %f z = %f\n",
					player.sock,player.pos.x,player.pos.y,player.pos.z);
				break;
				// クライアントから切断された
			case RECV_FAILED:
				// リストから削除
				printf("Socket = %llu がログアウトしました \n",it->sock);
				server.Close(it->sock);					
				deletearray.push_back(it->sock);
				it = playerlist.erase(it);
				continue;
			}
			++it;
		}

		// ログアウト処理
		for(unsigned int i = 0;i < deletearray.size();++i){
			Character player;
			player.sock = deletearray[i];
			player.cmd = CMD_LOGOUT;
			memcpy(data,&player,sizeof(Character));
			// ログアウト情報を他の全クライアントに送信
			std::list<Character>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
		}

		// 一括送信処理
		for(unsigned int i = 0;i < movearray.size();++i)
		{
			movearray[i].cmd = CMD_MOVE;
			memcpy(data,&movearray[i],sizeof(Character));
			// 移動情報を他の全クライアントに送信
			std::list<Character>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
		}

	}


	return 0;
}
