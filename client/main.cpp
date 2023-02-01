#include <conio.h>
#include <list>
#include "Client.h"
#include "../Packet.h"

#define IP "127.0.0.1"		// サーバーIPアドレス
#define PORT 8080			// ポート番号、クライアントプログラムと合わせる

int main()
{
	CClient client;
	int recvsize;
	char data[MAX_PACKET];

	std::list<Player>	playerlist;
	std::list<Player>::iterator it;
	Player				*pMe = NULL;

	if(!client.Connect(IP,PORT)){
		printf("サーバへの接続に失敗しました\n");
		return -1;
	}

	while(true)
	{
		// 受信処理
		RECVSTATUS status = client.Recv(data,sizeof(data),&recvsize);
		if(status == RECV_SUCCESSED){
			Player player;			
			memcpy(&player,data,sizeof(Player));
			switch(player.cmd)
			{
			case CMD_LOGIN:
				// ログインしたのは自分
				if(pMe == NULL){
					playerlist.push_back(player);
					printf("サーバーにログインしました Socket = %d " 
						"x = %d y = %d HP = %d\n",
						player.sock,player.x,player.y,player.HP);
					std::list<Player>::iterator it = playerlist.end();
					it--;
					pMe = &(*it);
				}
				// ログインしたのは自分以外
				else
				{
					playerlist.push_back(player);
					printf("Socket = %d がログインしてきました " 
						"x = %d y = %d HP = %d\n",
						player.sock,player.x,player.y,player.HP);
				}
				break;
			case CMD_LOGOUT:
				printf("Socket = %d がログアウトしました \n",player.sock);					
				for(it = playerlist.begin();it != playerlist.end();){
					if( it->sock == player.sock){
						it = playerlist.erase(it);
						continue;
					}
					++it;
				}
				break;
			case CMD_MOVE:
				
				printf("Socket = %d x = %d y = %d HP = %d\n",
					player.sock,player.x,player.y,player.HP);
				for(it = playerlist.begin();it != playerlist.end();	++it){
					if( it->sock == player.sock){
						*it = player;
					}
				}
			
				break;
			}
		}
		// サーバから切断された
		else if(status == RECV_FAILED){
			printf("サーバから切断されました\n");
			break;
		}

		// 入力時
		if(_kbhit())
		{
			if(pMe != NULL)
			{
				int c = _getch();
				bool isMove = false;
				// 移動
				switch(c)
				{
				case 'w':
					pMe->cmd = CMD_MOVEUP;
					isMove = true;
					break;
				case 's':
					pMe->cmd = CMD_MOVEDOWN;
					isMove = true;
					break;
				case 'a':
					pMe->cmd = CMD_MOVELEFT;
					isMove = true;
					break;
				case 'd':
					pMe->cmd = CMD_MOVERIGHT;
					isMove = true;
					break;
				}

				if(isMove){
					memcpy(data,pMe,sizeof(Player));
					client.Send(data,sizeof(data));
				}
			}
		}
	}


	return 0;
}
