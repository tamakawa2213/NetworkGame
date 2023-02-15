#include "Playable.h"
#include "Engine/Input.h"
#include "Engine/Math.h"
#include "Reception.h"

#define _WINSOCK2API_
#define _WS2IPDEF_
#define _WS2TCPIP_H_
#include "Packet.h"
#include "Client.h"

const char* IP = "127.0.0.1";		// サーバーIPアドレス
u_short PORT = 8080;			// ポート番号、クライアントプログラムと合わせる

CClient client;
int recvsize;
char data[MAX_PACKET];
std::list<Character>	playerlist;
std::list<Character>::iterator it;
Character* pMe = NULL;
Character player;
Character enemy;
SOCKET mySock;
SOCKET enemySock;

Playable::Playable(GameObject* parent)
	: Player(parent, "Playable"), PrevHit_(false)
{
	Collision_ = new SphereCollider(transform_.position_, 2.2f);
	AddCollider(Collision_);

    //接続
	if (!client.Connect(IP, PORT))
	{
        //エラー処理
	}
}

Playable::~Playable()
{
}

XMFLOAT3 Playable::GetEnemyPos()
{
    return enemy.pos;
}

void Playable::SetEnemyPos(XMFLOAT3 pos)
{
    enemy.pos = pos;
}

void Playable::SetCommand()
{
	if (Input::IsKey(DIK_W) && !Input::IsKey(DIK_S))
	{
		Command_ += COMMAND_ADV;
	}

	if (Input::IsKey(DIK_S) && !Input::IsKey(DIK_W))
	{
		Command_ += COMMAND_BACK;
	}

	if (Input::IsKey(DIK_A) && !Input::IsKey(DIK_D))
	{
		Command_ += COMMAND_ROTATE_L;
	}

	if (Input::IsKey(DIK_D) && !Input::IsKey(DIK_A))
	{
		Command_ += COMMAND_ROTATE_R;
	}

	if (Input::IsKeyDown(DIK_SPACE) && OnGround_)
	{
		Command_ += COMMAND_JUMP;
	}
}

void Playable::Hit()
{
	Reception* pEnemy = (Reception*)FindObject("Reception");
	if (Math::GetDistance(transform_.position_, pEnemy->GetPosition()) < 4.4f)
	{
		if (!PrevHit_)
		{
			XMVECTOR vMove = this->VecMove_;
			XMVECTOR eMove = pEnemy->VecMove_;
			pEnemy->VecMove_ = -pEnemy->VecMove_ / 2 + vMove / 2;
			this->VecMove_ = -this->VecMove_ / 2 + eMove / 2;
		}
	}
	else
	{
		PrevHit_ = false;
	}

	SAFE_RELEASE(pEnemy);
}

void Playable::NetWork()
{
    // 受信処理
    RECVSTATUS status = client.Recv(data, sizeof(data), &recvsize);
    if (status == RECV_SUCCESSED)
    {
        memcpy(&player, data, sizeof(Character));
        player.pos = transform_.position_;
        switch (player.cmd)
        {
        case CMD_LOGIN:
            // ログインしたのは自分
            if (pMe == NULL) {
                playerlist.push_back(player);
                std::list<Character>::iterator it = playerlist.end();
                it--;
                pMe = &(*it);
                mySock = player.sock;
            }
            // ログインしたのは自分以外
            else
            {
                playerlist.push_back(player);
                enemySock = player.sock;

            }
            break;
        case CMD_LOGOUT:
            for (it = playerlist.begin(); it != playerlist.end();) {
                if (it->sock == player.sock) {
                    it = playerlist.erase(it);
                    continue;
                }
                ++it;
            }
            break;
        case CMD_MOVE:
            for (it = playerlist.begin(); it != playerlist.end(); ++it)
            {
                if (it->sock == player.sock)
                {
                    *it = player;
                }
                else
                {
                    SetEnemyPos(it->pos);
                }
            }
            break;
        }
    }
    // サーバから切断された
    else if (status == RECV_FAILED)
    {
    }

    //データを送る
    memcpy(data, pMe, sizeof(Character));
    client.Send(data, sizeof(data));
}
