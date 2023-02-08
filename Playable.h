#pragma once
#include "Player.h"

//‘€ì‚·‚é‘¤
class Playable : public Player
{
	bool PrevHit_;
	void SetCommand() override;
	void Hit() override;
public:
	Playable(GameObject* parent);

	~Playable();
};