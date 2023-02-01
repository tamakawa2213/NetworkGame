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
	std::list<Player>	playerlist;
	std::vector<SOCKET>	deletearray;
	std::vector<Player>	movearray;
	int	recvsize;
	char data[MAX_PACKET];


	while(true)
	{
		// 接続処理
		if(server.Accept(&dstsock,&dstaddr)){
			printf("accept %s sock = %d\n",inet_ntoa(dstaddr.sin_addr),dstsock);
			
			Player player;
			player.sock = dstsock;
			player.cmd = CMD_LOGIN;
			// 乱数系列の変更 
			srand((unsigned) time(NULL));
			player.x = rand()%100 - 50;
			player.y = rand()%100 - 50;
			player.HP = 100;
			// ログイン情報を接続先に送信
			memcpy(data,&player,sizeof(Player));
			server.Send(dstsock,data,sizeof(data));

			// ログイン情報を他の全クライアントに送信
			std::list<Player>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
			playerlist.push_back(player);
		}

		// 一括受信処理
		std::list<Player>::iterator it;
		deletearray.clear();
		movearray.clear();
		for(it = playerlist.begin();it != playerlist.end();)
		{
			RECVSTATUS status = server.Recv(it->sock,data,sizeof(data),&recvsize);

			switch(status)
			{
			case RECV_SUCCESSED:
				Player player;
				memcpy(&player,data,sizeof(Player));
				switch(player.cmd)
				{
				case CMD_MOVEUP:
					it->y--;
					movearray.push_back(*it);
					break;
				case CMD_MOVEDOWN:
					it->y++;
					movearray.push_back(*it);
					break;
				case CMD_MOVELEFT:
					it->x--;
					movearray.push_back(*it);
					break;
				case CMD_MOVERIGHT:
					it->x++;
					movearray.push_back(*it);
					break;
				}
				printf("Socket = %d x = %d y = %d HP = %d\n",
					player.sock,player.x,player.y,player.HP);
				break;
				// クライアントから切断された
			case RECV_FAILED:
				// リストから削除
				printf("Socket = %d がログアウトしました \n",it->sock);
				server.Close(it->sock);					
				deletearray.push_back(it->sock);
				it = playerlist.erase(it);
				continue;
			}
			++it;
		}

		// ログアウト処理
		for(unsigned int i = 0;i < deletearray.size();++i){
			Player player;
			player.sock = deletearray[i];
			player.cmd = CMD_LOGOUT;
			memcpy(data,&player,sizeof(Player));
			// ログアウト情報を他の全クライアントに送信
			std::list<Player>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
		}

		// 判定処理

		// 一括送信処理
		for(unsigned int i = 0;i < movearray.size();++i)
		{
			movearray[i].cmd = CMD_MOVE;
			memcpy(data,&movearray[i],sizeof(Player));
			// 移動情報を他の全クライアントに送信
			std::list<Player>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
			
		}

	}


	return 0;
}
