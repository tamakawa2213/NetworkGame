#include "GravityInfluence.h"

void GravityInfluence::Falldown()
{
	//ínñ Ç…ê⁄ÇµÇƒÇ¢Ç»Ç¢éûÇÃÇ›
	if (!OnGround_)
	{
		Vertical_ -= GrAccel_;
	}
	else
	{
		Vertical_ = XMVECTOR{ 0,0,0,0 };
	}
}

GravityInfluence::GravityInfluence(GameObject* parent)
	:GravityInfluence(parent, "GravityInfluence")
{
}

GravityInfluence::GravityInfluence(GameObject* parent, std::string name)
	: GameObject(parent, name), OnGround_(true), Vertical_()
{
}

GravityInfluence::~GravityInfluence()
{
}
