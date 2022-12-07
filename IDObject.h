#pragma once
#include "Engine/GameObject.h"

//GameObjectクラスを拡張したIDObjectクラス
class IDObject : public GameObject
{
	char IDData_;
public:
	IDObject(GameObject* parent);
	IDObject(GameObject* parent, std::string name);
	~IDObject();
	void SetID(char ID);
	char GetID() { return IDData_; }
};