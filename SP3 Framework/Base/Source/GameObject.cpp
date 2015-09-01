#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	isSet(false),
	render(true),
	rotate(0),
	normal(0,1,0),
	ID(0),
	timer(0),
	deflect(false),
	shurikenthrow(true)
{
}

GameObject::~GameObject()
{
}

void GameObject::reset()
{
	vel.SetZero();
	pos.SetZero();
	isSet = false;
	render = true;
}