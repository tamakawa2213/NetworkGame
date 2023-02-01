#include <conio.h>
#include <list>
#include "Client.h"
#include "../Packet.h"

#define IP "127.0.0.1"		// �T�[�o�[IP�A�h���X
#define PORT 8080			// �|�[�g�ԍ��A�N���C�A���g�v���O�����ƍ��킹��

int main()
{
	CClient client;
	int recvsize;
	char data[MAX_PACKET];

	std::list<Player>	playerlist;
	std::list<Player>::iterator it;
	Player				*pMe = NULL;

	if(!client.Connect(IP,PORT)){
		printf("�T�[�o�ւ̐ڑ��Ɏ��s���܂���\n");
		return -1;
	}

	while(true)
	{
		// ��M����
		RECVSTATUS status = client.Recv(data,sizeof(data),&recvsize);
		if(status == RECV_SUCCESSED){
			Player player;			
			memcpy(&player,data,sizeof(Player));
			switch(player.cmd)
			{
			case CMD_LOGIN:
				// ���O�C�������͎̂���
				if(pMe == NULL){
					playerlist.push_back(player);
					printf("�T�[�o�[�Ƀ��O�C�����܂��� Socket = %d " 
						"x = %d y = %d HP = %d\n",
						player.sock,player.x,player.y,player.HP);
					std::list<Player>::iterator it = playerlist.end();
					it--;
					pMe = &(*it);
				}
				// ���O�C�������͎̂����ȊO
				else
				{
					playerlist.push_back(player);
					printf("Socket = %d �����O�C�����Ă��܂��� " 
						"x = %d y = %d HP = %d\n",
						player.sock,player.x,player.y,player.HP);
				}
				break;
			case CMD_LOGOUT:
				printf("Socket = %d �����O�A�E�g���܂��� \n",player.sock);					
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
		// �T�[�o����ؒf���ꂽ
		else if(status == RECV_FAILED){
			printf("�T�[�o����ؒf����܂���\n");
			break;
		}

		// ���͎�
		if(_kbhit())
		{
			if(pMe != NULL)
			{
				int c = _getch();
				bool isMove = false;
				// �ړ�
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
