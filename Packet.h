#ifndef _PACKET_H_
#define _PACKET_H_

#define MAX_PACKET	1024	// 最大パケット

// コマンド
enum COMMAND
{
	CMD_LOGIN,		// ログイン
	CMD_LOGOUT,		// ログアウト
	CMD_MOVE,		// 移動
	CMD_MOVEUP,		// 上移動
	CMD_MOVEDOWN,	// 下移動
	CMD_MOVELEFT,	// 左移動
	CMD_MOVERIGHT,	// 右移動
	CMD_JUMP		// ジャンプ
};

// プレイヤー
struct Player
{
	SOCKET		sock;
	int			cmd;
	int			x,y;
	int			HP;
	static const int dataend = '.';
	bool		isJump;
};

#endif