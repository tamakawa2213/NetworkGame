#ifndef _PACKET_H_
#define _PACKET_H_

#define MAX_PACKET	1024	// �ő�p�P�b�g

// �R�}���h
enum COMMAND
{
	CMD_LOGIN,		// ���O�C��
	CMD_LOGOUT,		// ���O�A�E�g
	CMD_MOVE,		// �ړ�
	CMD_MOVEUP,		// ��ړ�
	CMD_MOVEDOWN,	// ���ړ�
	CMD_MOVELEFT,	// ���ړ�
	CMD_MOVERIGHT,	// �E�ړ�
	CMD_JUMP		// �W�����v
};

// �v���C���[
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