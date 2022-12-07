#include "SphereCollider.h"
#include "GameObject.h"

SphereCollider::SphereCollider(XMFLOAT3 position, float Radius) : Radius_(Radius), pGameObject_(nullptr)
{
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::Ishit(SphereCollider* pTarget)
{
	if (this != pTarget)
	{
		float distanceX = this->pGameObject_->GetPosition().x - pTarget->pGameObject_->GetPosition().x;
		float distanceY = this->pGameObject_->GetPosition().y - pTarget->pGameObject_->GetPosition().y;
		float distanceZ = this->pGameObject_->GetPosition().z - pTarget->pGameObject_->GetPosition().z;
		if ((pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2)) < ((double)this->Radius_ + (double)pTarget->GetRadius()))
		{
			return true;
		}
	}
	return false;
}
