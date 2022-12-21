#include "Reception.h"
#include "Engine/Input.h"

Reception::Reception(GameObject* parent)
	: Player(parent, "Reception")
{
}

Reception::~Reception()
{
}

void Reception::SetCommand()
{
	if (Input::IsKey(DIK_I) && !Input::IsKey(DIK_K))
	{
		Command_ += COMMAND_ADV;
	}

	if (Input::IsKey(DIK_K) && !Input::IsKey(DIK_I))
	{
		Command_ += COMMAND_BACK;
	}

	if (Input::IsKey(DIK_J) && !Input::IsKey(DIK_L))
	{
		Command_ += COMMAND_ROTATE_L;
	}

	if (Input::IsKey(DIK_L) && !Input::IsKey(DIK_J))
	{
		Command_ += COMMAND_ROTATE_R;
	}

	if (Input::IsKey(DIK_RSHIFT) && OnGround_)
	{
		Command_ += COMMAND_JUMP;
	}
}
