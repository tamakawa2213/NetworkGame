#pragma once
#include "Player.h"

//���삷�鑤
class Playable : public Player
{
	bool PrevHit_;
	void SetCommand() override;
	void Hit() override;
public:
	Playable(GameObject* parent);

	~Playable();
};