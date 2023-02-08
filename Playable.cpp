#include "Playable.h"
#include "Engine/Input.h"
#include "Engine/Math.h"
#include "Reception.h"

Playable::Playable(GameObject* parent)
	: Player(parent, "Playable"), PrevHit_(false)
{
	Collision_ = new SphereCollider(transform_.position_, 2.2f);
	AddCollider(Collision_);
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

void Playable::Hit()
{
	Reception* pEnemy = (Reception*)FindObject("Reception");
	if (Math::GetDistance(transform_.position_, pEnemy->GetPosition()) < 4.4f)
	{
		if (!PrevHit_)
		{
			XMVECTOR vMove = this->VecMove_;
			XMVECTOR eMove = pEnemy->VecMove_;
			pEnemy->VecMove_ = -pEnemy->VecMove_ / 2 + vMove / 2;
			this->VecMove_ = -this->VecMove_ / 2 + eMove / 2;
		}
	}
	else
	{
		PrevHit_ = false;
	}

	SAFE_RELEASE(pEnemy);
}
