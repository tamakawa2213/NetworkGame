#pragma once
#include "Transform.h"

class GameObject;

class SphereCollider
{
	float Radius_;				//半径
	GameObject* pGameObject_;	//GameObject型のポインタ
public:
	SphereCollider(XMFLOAT3 position, float Radius);
	~SphereCollider();

	bool Ishit(SphereCollider* pTarget);	//呼び出し元と引数に受け取った相手の当たり判定を実行する

	float GetRadius() { return Radius_; }

	void SetGameObject(GameObject* obj) { pGameObject_ = obj; }
};