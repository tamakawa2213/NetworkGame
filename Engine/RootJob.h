#pragma once
#include "GameObject.h"
class RootJob : public GameObject
{
public:
	RootJob(GameObject* parent);
	RootJob(GameObject* parent, const std::string& name);
	~RootJob();

	void Initialize();	//‰Šú‰»
};