#pragma once
#include "Player.h"

//���삷�鑤
class Playable : public Player
{
	bool PrevHit_;
	XMFLOAT3 enemyPos_;
	void SetCommand() override;
	void Hit() override;
	void NetWork() override;
public:
	Playable(GameObject* parent);

	~Playable();

	XMFLOAT3 GetEnemyPos();
	void SetEnemyPos(XMFLOAT3 pos);
};