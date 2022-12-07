#include "IDObject.h"

IDObject::IDObject(GameObject* parent)
	: IDObject(parent, "IDObject")
{
}

IDObject::IDObject(GameObject* parent, std::string name)
	: GameObject(parent, name), IDData_(NULL)
{
}

IDObject::~IDObject()
{
}

void IDObject::SetID(char ID)
{
	IDData_ = ID;
}
