#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <time.h>
#include <list>
#include <vector>
#include "Server.h"
#include "../Packet.h"

#define PORT 8080			// �|�[�g�ԍ��A�N���C�A���g�v���O�����ƍ��킹��


int main()
{
	CServer server;
	server.Listen(PORT);
	printf("�T�[�o�҂��󂯏�Ԃł�\n�N���C�A���g�v���O�������N�����Ă�������\n");
	SOCKET dstsock;
	struct sockaddr_in	dstaddr;
	std::list<Player>	playerlist;
	std::vector<SOCKET>	deletearray;
	std::vector<Player>	movearray;
	int	recvsize;
	char data[MAX_PACKET];


	while(true)
	{
		// �ڑ�����
		if(server.Accept(&dstsock,&dstaddr)){
			printf("accept %s sock = %d\n",inet_ntoa(dstaddr.sin_addr),dstsock);
			
			Player player;
			player.sock = dstsock;
			player.cmd = CMD_LOGIN;
			// �����n��̕ύX 
			srand((unsigned) time(NULL));
			player.x = rand()%100 - 50;
			player.y = rand()%100 - 50;
			player.HP = 100;
			// ���O�C������ڑ���ɑ��M
			memcpy(data,&player,sizeof(Player));
			server.Send(dstsock,data,sizeof(data));

			// ���O�C�����𑼂̑S�N���C�A���g�ɑ��M
			std::list<Player>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
			playerlist.push_back(player);
		}

		// �ꊇ��M����
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
				// �N���C�A���g����ؒf���ꂽ
			case RECV_FAILED:
				// ���X�g����폜
				printf("Socket = %d �����O�A�E�g���܂��� \n",it->sock);
				server.Close(it->sock);					
				deletearray.push_back(it->sock);
				it = playerlist.erase(it);
				continue;
			}
			++it;
		}

		// ���O�A�E�g����
		for(unsigned int i = 0;i < deletearray.size();++i){
			Player player;
			player.sock = deletearray[i];
			player.cmd = CMD_LOGOUT;
			memcpy(data,&player,sizeof(Player));
			// ���O�A�E�g���𑼂̑S�N���C�A���g�ɑ��M
			std::list<Player>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
		}

		// ���菈��

		// �ꊇ���M����
		for(unsigned int i = 0;i < movearray.size();++i)
		{
			movearray[i].cmd = CMD_MOVE;
			memcpy(data,&movearray[i],sizeof(Player));
			// �ړ����𑼂̑S�N���C�A���g�ɑ��M
			std::list<Player>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
			
		}

	}


	return 0;
}
