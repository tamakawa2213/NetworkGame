#include "RootJob.h"
#include "SceneManager.h"

RootJob::RootJob(GameObject* parent) : GameObject()
{
}

RootJob::RootJob(GameObject* parent, const std::string& name) : GameObject(parent, name)
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{
	Instantiate<SceneManager>(this);
}