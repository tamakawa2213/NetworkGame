#pragma once
#include "Player.h"

//���삷�鑤
class Playable : public Player
{
	void SetCommand() override;

public:
	Playable(GameObject* parent);

	~Playable();
};