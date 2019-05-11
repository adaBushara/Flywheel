#pragma once
#include "Component.h"
#include <Box2D\Box2D.h>
class Position;

class RigidBody : public Component<RigidBody>
{


public:
	void setPosition(b2Vec2 &v);
	/*void setPosition(Position &p);*/
	void setRotation(float32 w);
	void setKinematic();
	void setStatic();
	void setDynamic();
	bool isKinematic();
	bool isStatic();
	bool isDynamic();
	b2BodyType getBodyType();
	void AddForce(b2Vec2 &force);
	void AddForceAtLocation(b2Vec2 &force, b2Vec2 &location);
	void setVelocity(b2Vec2 &vel);
	void setAngularVelcoity(float32 &w);
	void AddImpulse(b2Vec2 &force);
	void AddImpulseAtLocation(b2Vec2 &force, b2Vec2 &pos);
	void AddTorque(float32 &w);
	void AddAngularImpulse(float32 &w);
	b2Vec2 GetPosition();
	float getAngle()
	{
		return body->GetAngle();
	}
	void SetAngle(float32 _angle)
	{
		body->SetTransform(body->GetPosition(), _angle);
	}
	void SetBody(b2Body *_body);

private:
	b2BodyType bodyType;
	b2Body *body; //the rigid body that we own
};