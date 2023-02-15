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
	std::list<Character>	playerlist;
	std::vector<SOCKET>	deletearray;
	std::vector<Character>	movearray;
	int	recvsize;
	char data[MAX_PACKET];


	while(true)
	{
		// �ڑ�����
		if(server.Accept(&dstsock,&dstaddr)){
			printf("accept %s sock = %f\n",inet_ntoa(dstaddr.sin_addr),dstsock);
			
			Character player;
			player.sock = dstsock;
			player.cmd = CMD_LOGIN;
			player.pos = DirectX::XMFLOAT3(0, 0, 0);
			// ���O�C������ڑ���ɑ��M
			memcpy(data,&player,sizeof(Character));
			server.Send(dstsock,data,sizeof(data));

			// ���O�C�����𑼂̑S�N���C�A���g�ɑ��M
			std::list<Character>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
			playerlist.push_back(player);
		}

		// �ꊇ��M����
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
				// �N���C�A���g����ؒf���ꂽ
			case RECV_FAILED:
				// ���X�g����폜
				printf("Socket = %llu �����O�A�E�g���܂��� \n",it->sock);
				server.Close(it->sock);					
				deletearray.push_back(it->sock);
				it = playerlist.erase(it);
				continue;
			}
			++it;
		}

		// ���O�A�E�g����
		for(unsigned int i = 0;i < deletearray.size();++i){
			Character player;
			player.sock = deletearray[i];
			player.cmd = CMD_LOGOUT;
			memcpy(data,&player,sizeof(Character));
			// ���O�A�E�g���𑼂̑S�N���C�A���g�ɑ��M
			std::list<Character>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
		}

		// �ꊇ���M����
		for(unsigned int i = 0;i < movearray.size();++i)
		{
			movearray[i].cmd = CMD_MOVE;
			memcpy(data,&movearray[i],sizeof(Character));
			// �ړ����𑼂̑S�N���C�A���g�ɑ��M
			std::list<Character>::iterator it;
			for(it = playerlist.begin();it != playerlist.end();++it){
				server.Send(it->sock,data,sizeof(data));
			}
		}

	}


	return 0;
}
