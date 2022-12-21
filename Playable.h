#pragma once
#include "Player.h"

//‘€ì‚·‚é‘¤
class Playable : public Player
{
	void SetCommand() override;

public:
	Playable(GameObject* parent);

	~Playable();
};