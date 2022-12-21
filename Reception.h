#pragma once
#include "Player.h"

class Reception : public Player
{
public:
	Reception(GameObject* parent);

	~Reception();

	void SetCommand() override;
};

