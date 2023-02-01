#include "Playable.h"
#include "Engine/Input.h"

Playable::Playable(GameObject* parent)
	: Player(parent, "Playable")
{
}

Playable::~Playable()
{
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