#include "RigidBody.h"
#include "Engine.h"

void RigidBody::setPosition(b2Vec2 & v)
{
	//body->SetTransform(b2Vec2(p.x / physicsScale, p.y / physicsScale), body->GetAngle());
	//body->SetTransform(v,)
	float physicsScale = Engine::getInstance()->physicsScale;
	
	body->SetTransform(b2Vec2(v.x/physicsScale, v.y/physicsScale), body->GetAngle());
}

//void RigidBody::setPosition(Position & p)
//{
//	float physicsScale = Engine::getInstance()->physicsScale;
//
//	body->SetTransform(b2Vec2(p.x / physicsScale, p.y / physicsScale), body->GetAngle());
//	
//}

void RigidBody::setRotation(float32 w)
{
	body->SetTransform(body->GetPosition(), w);
}

void RigidBody::setKinematic()
{
	bodyType = b2BodyType::b2_kinematicBody;
	body->SetType(bodyType);
}

void RigidBody::setStatic()
{
	bodyType = b2BodyType::b2_staticBody;
	body->SetType(bodyType);
}

void RigidBody::setDynamic()
{
	bodyType = b2BodyType::b2_dynamicBody;
	body->SetType(bodyType);
}

bool RigidBody::isKinematic()
{
	return bodyType == b2BodyType::b2_kinematicBody;
}

bool RigidBody::isStatic()
{
	return bodyType == b2BodyType::b2_staticBody;
}

bool RigidBody::isDynamic()
{
	return bodyType == b2BodyType::b2_dynamicBody;
}	

b2BodyType RigidBody::getBodyType()
{
	return bodyType;
}

void RigidBody::AddForce(b2Vec2 &force)
{
	body->ApplyForceToCenter(force, true);
}

void RigidBody::AddForceAtLocation(b2Vec2 &force, b2Vec2 &location)
{
	body->ApplyForce(force,body->GetWorldPoint( location), true);
}

void RigidBody::SetBody(b2Body * _body)
{
	body = _body;
}

void RigidBody::setVelocity(b2Vec2 &vel)
{
	body->SetLinearVelocity(vel);
	
}

void RigidBody::setAngularVelcoity(float32 &w)
{
	body->SetAngularVelocity(-Engine::getInstance()->physicsScale * w);

}

void RigidBody::AddImpulse(b2Vec2 &force)
{
		
	body->ApplyLinearImpulse(-Engine::getInstance()->physicsScale * force, body->GetWorldCenter(), true);
}

void RigidBody::AddImpulseAtLocation(b2Vec2 &force, b2Vec2 &pos)
{

	body->ApplyLinearImpulse(-Engine::getInstance()->physicsScale * force , body->GetWorldPoint(pos), true);
}

void RigidBody::AddTorque(float32 &w)
{

	body->ApplyTorque(w * -Engine::getInstance()->physicsScale, true);
}

void RigidBody::AddAngularImpulse(float32 &w)
{
	body->ApplyAngularImpulse(w * -Engine::getInstance()->physicsScale, true);
}

b2Vec2 RigidBody::GetPosition()
{
	float physicsScale = Engine::getInstance()->physicsScale;
	b2Vec2 pos = body->GetPosition();
	pos *= physicsScale;
	return pos;
}