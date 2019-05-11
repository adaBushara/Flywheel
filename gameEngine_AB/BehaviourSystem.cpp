#include "BehaviourSystem.h"
#include "Position.h"
#include "RigidBody.h"
#include "Entity.h"
#include "Position.h"
#include "Scene.h"
#include "CircleCollider.h"
#include "Engine.h"
#include "Input.h"
#include "PhysicsSystem.h"
#include "Sprite.h"
#include "RenderingSystem.h"
#include "RectangleCollider.h"
#include "ChaiBehaviour.h"
#include "Engine.h"

using namespace chaiscript;
ChaiScript BehaviourSystem::chai;


//want to register our parsing types here
REGISTER_PARSE_TYPE(Entity);
REGISTER_PARSE_TYPE(Position);
REGISTER_PARSE_TYPE(Engine);
REGISTER_PARSE_TYPE(RigidBody);
REGISTER_PARSE_TYPE(Sprite);
REGISTER_PARSE_TYPE(CircleCollider);
REGISTER_PARSE_TYPE(RectangleCollider);
REGISTER_PARSE_TYPE(ChaiBehaviour);
REGISTER_PARSE_TYPE(PhysicsSystem);

#define CREATE_GET_COMPONENT(X) (std::string("get").append(#X))
#define CREATE_ADD_COMPONENT(X) (std::string("add").append(#X))
#define CREATE_REMOVE_COMPONENT(X) (std::string("remove").append(#X))
#define CREATE_GET_COMPONENTS(X) (std::string("get").append(#X)).append("s")
#define CREATE_HAS_COMPONENT(X) (std::string("has").append(#X) )



#define REGISTERCOMPONENT(X) \
chai.add(chaiscript::fun(&Entity::getComponent<X>), CREATE_GET_COMPONENT(X)); \
chai.add(chaiscript::fun(&Entity::AddComponent<X>), CREATE_ADD_COMPONENT(X)); \
chai.add(chaiscript::fun(&Entity::RemoveComponent<X>), CREATE_REMOVE_COMPONENT(X)); \
chai.add(chaiscript::fun(&Entity::getComponent<X>), CREATE_GET_COMPONENTS(X)); \
chai.add(chaiscript::fun(&X::getOwner), "getOwner"); \
chai.add(chaiscript::fun(&Entity::hasComponent<X>), CREATE_HAS_COMPONENT(X)); \
std::cout << CREATE_REMOVE_COMPONENT(X) << std::endl\

static std::string tmpName; //used for macro hackery

#define REGISTER_CLASS_FUNCTION(X) \
tmpName = #X; \
chai.add(chaiscript::fun(&X),tmpName.substr(tmpName.find_last_of(":")+1, tmpName.size())); \


/*
this does not work
reason: macro breaks when it comes to the type (for some reason)
Register Component Macro works instead. 
Does basic Registration of all functions that components have
*/
template<typename T>
void RegisterComponent(chaiscript::ChaiScript &chai)
{
	chai.add(chaiscript::fun(&Entity::getComponent<T>), CREATE_GET_COMPONENT(#T));
	chai.add(chaiscript::fun(&Entity::AddComponent<T>), CREATE_ADD_COMPONENT(#T));
	chai.add(chaiscript::fun(&Entity::RemoveComponent<T>), CREATE_REMOVE_COMPONENT(#T));
	std::cout << CREATE_REMOVE_COMPONENT(#T) << std::endl;
}

template<typename T>
void RegisterClass(chaiscript::ChaiScript &chai)
{
	chai.add(chaiscript::user_type<T>(), TypeParseTraits<T>::name);
}


BehaviourSystem::BehaviourSystem()
{
	componentMask = Position::getMask();
	initChaiScript();

}

void BehaviourSystem::loadScript(const std::string &fileLocation)
{
	chai.eval_file(fileLocation);
}

void BehaviourSystem::initBehaviour (ChaiBehaviour &c, const std::string &className)
{
	

	c.className = className;
	std::string name = c.className + std::to_string(c.getID());
	//chai.add_global(chaiscript::var(c), name);
	//build script that we want
	
	std::string scriptFunc = "global &" + name  + " = " + c.className + "( ); ";
	std::string entName = "Entity" + std::to_string(c.getOwner().getID());
	std::string entAssign =  name + ".owner" + " := " + entName + ";";
	std::string idAssign = name + ".id " + " =" + std::to_string(c.getID()) + ";";
	chai.eval(scriptFunc); //eval the function
	Entity *e = &c.getOwner();	
	chai.add_global(chaiscript::var(*e) , entName);
	Boxed_Value &thing = chai.eval(name);
	chai.eval(entAssign);
	chai.eval(idAssign);	
	
	
	ExecAwake(c);
	if (isStarted)
	{
		ExecStart(c);
	}

}

//start is called after all objects are created
void BehaviourSystem::Start()
{
	handleStart();
	isStarted = true;
}

void BehaviourSystem::initChaiScript()
{

	//register our classes
	RegisterClass<Sprite>(chai);
	RegisterClass<Position>(chai);
	RegisterClass<Entity>(chai);
	RegisterClass<CircleCollider>(chai);
	RegisterClass<RectangleCollider>(chai);
	RegisterClass<ChaiBehaviour>(chai);
	


	REGISTER_CLASS_FUNCTION(Entity::getID);
	REGISTER_CLASS_FUNCTION(Position::x);
	REGISTER_CLASS_FUNCTION(Position::y);
	REGISTER_CLASS_FUNCTION(Sprite::height);
	REGISTER_CLASS_FUNCTION(ChaiBehaviour::getID);

	REGISTER_CLASS_FUNCTION(RigidBody::AddAngularImpulse);
	REGISTER_CLASS_FUNCTION(RigidBody::AddForce);
	REGISTER_CLASS_FUNCTION(RigidBody::AddForceAtLocation);
	REGISTER_CLASS_FUNCTION(RigidBody::AddImpulse);
	REGISTER_CLASS_FUNCTION(RigidBody::AddImpulseAtLocation);
	REGISTER_CLASS_FUNCTION(RigidBody::AddTorque);
	REGISTER_CLASS_FUNCTION(RigidBody::getID);
	REGISTER_CLASS_FUNCTION(RigidBody::GetPosition);
	REGISTER_CLASS_FUNCTION(RigidBody::isDynamic);
	REGISTER_CLASS_FUNCTION(RigidBody::isStatic);
	REGISTER_CLASS_FUNCTION(RigidBody::isKinematic);
	REGISTER_CLASS_FUNCTION(RigidBody::setKinematic);
	REGISTER_CLASS_FUNCTION(RigidBody::setDynamic);
	REGISTER_CLASS_FUNCTION(RigidBody::setStatic);
	REGISTER_CLASS_FUNCTION(RigidBody::setAngularVelcoity);
	REGISTER_CLASS_FUNCTION(RigidBody::setVelocity);
	REGISTER_CLASS_FUNCTION(RigidBody::setPosition);
	REGISTER_CLASS_FUNCTION(RigidBody::setRotation);


	
	chai.add(chaiscript::user_type<b2Vec2>(), "Vector2");
	chai.add(chaiscript::constructor<b2Vec2()>(), "Vector2");
	chai.add(chaiscript::constructor<b2Vec2(float32, float32) >(), "Vector2");
	REGISTER_CLASS_FUNCTION(b2Vec2::Length);
	REGISTER_CLASS_FUNCTION(b2Vec2::LengthSquared);
	REGISTER_CLASS_FUNCTION(b2Vec2::Normalize);
	REGISTER_CLASS_FUNCTION(b2Vec2::operator-);
	REGISTER_CLASS_FUNCTION(b2Vec2::operator*=);
	REGISTER_CLASS_FUNCTION(b2Vec2::operator+=);
	REGISTER_CLASS_FUNCTION(b2Vec2::operator-=);
	REGISTER_CLASS_FUNCTION(b2Vec2::Set);
	REGISTER_CLASS_FUNCTION(b2Vec2::SetZero);
	REGISTER_CLASS_FUNCTION(b2Vec2::Skew);
	REGISTER_CLASS_FUNCTION(b2Vec2::x);
	REGISTER_CLASS_FUNCTION(b2Vec2::y);

	REGISTERCOMPONENT(Sprite);
	REGISTERCOMPONENT(RigidBody);
	REGISTERCOMPONENT(Position);
	REGISTERCOMPONENT(CircleCollider);
	REGISTERCOMPONENT(RectangleCollider);

	
	//Engine & Systems Registration
	RegisterClass<PhysicsSystem>(chai);	
	RegisterClass<Engine>(chai);
	
	REGISTER_CLASS_FUNCTION(PhysicsSystem::setGravity);
	REGISTER_CLASS_FUNCTION(PhysicsSystem::getGravity);

	/*Engine *eng = Engine::getInstance();
	PhysicsSystem *phys = Engine::getInstance()->getPhysicsSystem();*/
	///*chai.add_global_const(chaiscript::var(*eng),"GEngine");*/
	//chai.add_global(chaiscript::var(phys), "PhysSystem");
	
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::A), "KEYS_A");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::B), "KEYS_B");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::C), "KEYS_C");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::D), "KEYS_D");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::E), "KEYS_E");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F), "KEYS_F");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::G), "KEYS_G");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::H), "KEYS_H");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::I), "KEYS_I");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::J), "KEYS_J");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::K), "KEYS_K");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::L), "KEYS_L");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::M), "KEYS_M");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::N), "KEYS_N");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::O), "KEYS_O");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::P), "KEYS_P");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::Q), "KEYS_Q");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::R), "KEYS_R");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::S), "KEYS_S");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::T), "KEYS_T");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::U), "KEYS_U");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::V), "KEYS_V");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::W), "KEYS_W");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::X), "KEYS_X");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::Y), "KEYS_Y");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::Z), "KEYS_Z");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_1), "KEYS_NUM_1");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_2), "KEYS_NUM_2");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_3), "KEYS_NUM_3");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_4), "KEYS_NUM_4");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_5), "KEYS_NUM_5");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_6), "KEYS_NUM_6");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_7), "KEYS_NUM_7");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_8), "KEYS_NUM_8");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_9), "KEYS_NUM_9");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUM_0), "KEYS_NUM_0");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RETURN), "KEYS_RETURN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::ESCAPE), "KEYS_ESCAPE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::BACKSPACE), "KEYS_BACKSPACE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::TAB), "KEYS_TAB");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::SPACE), "KEYS_SPACE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::MINUS), "KEYS_MINUS");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::EQUALS), "KEYS_EQUALS");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LEFTBRACKET), "KEYS_LEFTBRACKET");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RIGHTBRACKET), "KEYS_RIGHTBRACKET");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::BACKSLASH), "KEYS_BACKSLASH");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NONUSHASH), "KEYS_NONUSHASH");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::SEMICOLON), "KEYS_SEMICOLON");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::APOSTROPHE), "KEYS_APOSTROPHE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::GRAVE), "KEYS_GRAVE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::COMMA), "KEYS_COMMA");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::PERIOD), "KEYS_PERIOD");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::SLASH), "KEYS_SLASH");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CAPSLOCK), "KEYS_CAPSLOCK");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F1), "KEYS_F1");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F2), "KEYS_F2");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F3), "KEYS_F3");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F4), "KEYS_F4");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F5), "KEYS_F5");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F6), "KEYS_F6");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F7), "KEYS_F7");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F8), "KEYS_F8");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F9), "KEYS_F9");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F10), "KEYS_F10");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F11), "KEYS_F11");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F12), "KEYS_F12");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::PRINTSCREEN), "KEYS_PRINTSCREEN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::SCROLLLOCK), "KEYS_SCROLLLOCK");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::PAUSE), "KEYS_PAUSE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INSERT), "KEYS_INSERT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::HOME), "KEYS_HOME");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::PAGEUP), "KEYS_PAGEUP");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::END), "KEYS_END");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::PAGEDOWN), "KEYS_PAGEDOWN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RIGHT), "KEYS_RIGHT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LEFT), "KEYS_LEFT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::DOWN), "KEYS_DOWN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::UP), "KEYS_UP");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NUMLOCKCLEAR), "KEYS_NUMLOCKCLEAR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_DIVIDE), "KEYS_KP_DIVIDE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MULTIPLY), "KEYS_KP_MULTIPLY");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MINUS), "KEYS_KP_MINUS");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_PLUS), "KEYS_KP_PLUS");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_ENTER), "KEYS_KP_ENTER");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_1), "KEYS_KP_1");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_2), "KEYS_KP_2");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_3), "KEYS_KP_3");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_4), "KEYS_KP_4");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_5), "KEYS_KP_5");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_6), "KEYS_KP_6");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_7), "KEYS_KP_7");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_8), "KEYS_KP_8");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_9), "KEYS_KP_9");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_0), "KEYS_KP_0");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_PERIOD), "KEYS_KP_PERIOD");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::NONUSBACKSLASH), "KEYS_NONUSBACKSLASH");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::APPLICATION), "KEYS_APPLICATION");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::POWER), "KEYS_POWER");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_EQUALS), "KEYS_KP_EQUALS");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F13), "KEYS_F13");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F14), "KEYS_F14");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F15), "KEYS_F15");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F16), "KEYS_F16");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F17), "KEYS_F17");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F18), "KEYS_F18");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F19), "KEYS_F19");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F20), "KEYS_F20");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F21), "KEYS_F21");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F22), "KEYS_F22");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F23), "KEYS_F23");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::F24), "KEYS_F24");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::EXECUTE), "KEYS_EXECUTE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::HELP), "KEYS_HELP");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::MENU), "KEYS_MENU");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::SELECT), "KEYS_SELECT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::STOP), "KEYS_STOP");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::AGAIN), "KEYS_AGAIN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::UNDO), "KEYS_UNDO");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CUT), "KEYS_CUT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::COPY), "KEYS_COPY");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::PASTE), "KEYS_PASTE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::FIND), "KEYS_FIND");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::MUTE), "KEYS_MUTE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::VOLUMEUP), "KEYS_VOLUMEUP");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::VOLUMEDOWN), "KEYS_VOLUMEDOWN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_COMMA), "KEYS_KP_COMMA");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_EQUALSAS400), "KEYS_KP_EQUALSAS400");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL1), "KEYS_INTERNATIONAL1");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL2), "KEYS_INTERNATIONAL2");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL3), "KEYS_INTERNATIONAL3");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL4), "KEYS_INTERNATIONAL4");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL5), "KEYS_INTERNATIONAL5");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL6), "KEYS_INTERNATIONAL6");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL7), "KEYS_INTERNATIONAL7");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL8), "KEYS_INTERNATIONAL8");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::INTERNATIONAL9), "KEYS_INTERNATIONAL9");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG1), "KEYS_LANG1");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG2), "KEYS_LANG2");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG3), "KEYS_LANG3");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG4), "KEYS_LANG4");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG5), "KEYS_LANG5");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG6), "KEYS_LANG6");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG7), "KEYS_LANG7");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG8), "KEYS_LANG8");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LANG9), "KEYS_LANG9");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::ALTERASE), "KEYS_ALTERASE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::SYSREQ), "KEYS_SYSREQ");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CANCEL), "KEYS_CANCEL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CLEAR), "KEYS_CLEAR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::PRIOR), "KEYS_PRIOR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RETURN2), "KEYS_RETURN2");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::SEPARATOR), "KEYS_SEPARATOR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::OPER), "KEYS_OPER");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CLEARAGAIN), "KEYS_CLEARAGAIN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CRSEL), "KEYS_CRSEL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::EXSEL), "KEYS_EXSEL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_00), "KEYS_KP_00");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_000), "KEYS_KP_000");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::THOUSANDSSEPARATOR), "KEYS_THOUSANDSSEPARATOR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::DECIMALSEPARATOR), "KEYS_DECIMALSEPARATOR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CURRENCYUNIT), "KEYS_CURRENCYUNIT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::CURRENCYSUBUNIT), "KEYS_CURRENCYSUBUNIT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_LEFTPAREN), "KEYS_KP_LEFTPAREN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_RIGHTPAREN), "KEYS_KP_RIGHTPAREN");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_LEFTBRACE), "KEYS_KP_LEFTBRACE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_RIGHTBRACE), "KEYS_KP_RIGHTBRACE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_TAB), "KEYS_KP_TAB");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_BACKSPACE), "KEYS_KP_BACKSPACE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_A), "KEYS_KP_A");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_B), "KEYS_KP_B");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_C), "KEYS_KP_C");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_D), "KEYS_KP_D");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_E), "KEYS_KP_E");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_F), "KEYS_KP_F");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_XOR), "KEYS_KP_XOR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_POWER), "KEYS_KP_POWER");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_PERCENT), "KEYS_KP_PERCENT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_LESS), "KEYS_KP_LESS");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_GREATER), "KEYS_KP_GREATER");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_AMPERSAND), "KEYS_KP_AMPERSAND");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_DBLAMPERSAND), "KEYS_KP_DBLAMPERSAND");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_VERTICALBAR), "KEYS_KP_VERTICALBAR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_DBLVERTICALBAR), "KEYS_KP_DBLVERTICALBAR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_COLON), "KEYS_KP_COLON");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_HASH), "KEYS_KP_HASH");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_SPACE), "KEYS_KP_SPACE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_AT), "KEYS_KP_AT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_EXCLAM), "KEYS_KP_EXCLAM");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MEMSTORE), "KEYS_KP_MEMSTORE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MEMRECALL), "KEYS_KP_MEMRECALL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MEMCLEAR), "KEYS_KP_MEMCLEAR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MEMADD), "KEYS_KP_MEMADD");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MEMSUBTRACT), "KEYS_KP_MEMSUBTRACT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MEMMULTIPLY), "KEYS_KP_MEMMULTIPLY");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_MEMDIVIDE), "KEYS_KP_MEMDIVIDE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_PLUSMINUS), "KEYS_KP_PLUSMINUS");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_CLEAR), "KEYS_KP_CLEAR");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_CLEARENTRY), "KEYS_KP_CLEARENTRY");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_BINARY), "KEYS_KP_BINARY");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_OCTAL), "KEYS_KP_OCTAL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_DECIMAL), "KEYS_KP_DECIMAL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::KP_HEXADECIMAL), "KEYS_KP_HEXADECIMAL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LCTRL), "KEYS_LCTRL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LSHIFT), "KEYS_LSHIFT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LALT), "KEYS_LALT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::LGUI), "KEYS_LGUI");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RCTRL), "KEYS_RCTRL");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RSHIFT), "KEYS_RSHIFT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RALT), "KEYS_RALT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)Keys::RGUI), "KEYS_RGUI");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)MouseButtons::MIDDLE), "MOUSE_MIDDLE");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)MouseButtons::RIGHT), "MOUSE_RIGHT");
	chai.add_global_const(chaiscript::const_var<unsigned int>((unsigned int)MouseButtons::LEFT), "MOUSE_LEFT");


	
	//REGISTER_CLASS_FUNCTION(Input::getMousePress);
	REGISTER_CLASS_FUNCTION(Input::getMouseX);
	REGISTER_CLASS_FUNCTION(Input::getMouseY);
	REGISTER_CLASS_FUNCTION(Input::getPressed);

	chai.add(chaiscript::fun(&Input::getMousePress_uint), "getMousePress");
	chai.add(chaiscript::fun(&Input::getPressed_uint), "getPressed");
}

BehaviourSystem::~BehaviourSystem()
{
	
}


void BehaviourSystem::Update()
{
	
	
	handleUpdate();
	handleCollisionEnter();
	handleCollisionExit();
	handleTriggerEnter();
	handleTriggerExit();
	Engine::getInstance()->getPhysicsSystem()->clearCollisions();
	
	

}


void BehaviourSystem::handleAwake()
{
	auto lst = Entity::getEntityList();

	for (size_t i = 0; i < lst.size(); i++)
	{
		if (lst[i].hasComponent<ChaiBehaviour>())
		{
			auto bhv = lst[i].getComponents<ChaiBehaviour>();
			for (size_t j = 0; j < bhv.size(); j++)
			{
				ExecAwake(bhv[j]);
			}
		}
	}
}

void BehaviourSystem::handleStart()
{
	auto lst = Entity::getEntityList();

	for (size_t i = 0; i < lst.size(); i++)
	{
		if (lst[i].hasComponent<ChaiBehaviour>())
		{
			auto bhv = lst[i].getComponents<ChaiBehaviour>();
			for (size_t j = 0; j < bhv.size(); j++)
			{
				ExecStart(bhv[j]);
			}
		}
	}
}

void BehaviourSystem::handleUpdate()
{
	auto lst = Entity::getEntityList();

	for (size_t i = 0; i < lst.size(); i++)
	{
		if (lst[i].hasComponent<ChaiBehaviour>())
		{
			auto bhv = lst[i].getComponents<ChaiBehaviour>();
			for (size_t j = 0; j < bhv.size(); j++)
			{
				ExecUpdate(bhv[j]);
			}
		}
	}
}

void BehaviourSystem::handleCollisionEnter()
{
	PhysicsSystem *phys = Engine::getInstance()->getPhysicsSystem();
	auto _list = phys->getCollisionEnter();
	for (size_t i = 0; i < _list.size(); i++)
	{
		auto tuple = _list[i];
		Entity *a, *b;
		a = std::get<0>(tuple);
		b = std::get<1>(tuple);

		auto compList = a->getComponents<ChaiBehaviour>();

		for (size_t j = 0; j < compList.size(); j++)
		{
			ExecOnCollisionEnter(compList[j], *b);
		}
		
	}

}

void BehaviourSystem::handleCollisionExit()
{
	PhysicsSystem *phys = Engine::getInstance()->getPhysicsSystem();
	auto& _list = phys->getCollisionExit();
	
	for (size_t i = 0; i < _list.size(); i++)
	{
		auto tuple = _list[i];
		Entity *a, *b;
		a = std::get<0>(tuple);
		b = std::get<1>(tuple);

		auto compList = a->getComponents<ChaiBehaviour>();

		for (size_t j = 0; j < compList.size(); j++)
		{
			ExecOnCollisionExit(compList[j], *b);
		}
	}
}

void BehaviourSystem::handleTriggerEnter()
{
	PhysicsSystem *phys = Engine::getInstance()->getPhysicsSystem();
	auto& _list = phys->getTriggerExit();
	
	for (size_t i = 0; i < _list.size(); i++)
	{
		auto tuple = _list[i];
		Entity *a, *b;
		a = std::get<0>(tuple);
		b = std::get<1>(tuple);

		auto compList = a->getComponents<ChaiBehaviour>();

		for (size_t j = 0; j < compList.size(); j++)
		{
			ExecOnTriggerEnter(compList[j], *b);
		}

	}
}

void BehaviourSystem::handleTriggerExit()
{
	PhysicsSystem *phys = Engine::getInstance()->getPhysicsSystem();
	auto& _list = phys->getTriggerExit();
	
	for (size_t i = 0; i < _list.size(); i++)
	{
		auto tuple = _list[i];
		Entity *a, *b;
		a = std::get<0>(tuple);
		b = std::get<1>(tuple);
		auto compList = a->getComponents<ChaiBehaviour>();

		for (size_t j = 0; j < compList.size(); j++)
		{
			ExecOnTriggerExit(compList[j], *b);
		}

	}

}


void BehaviourSystem::ExecAwake(ChaiBehaviour &c)
{
	ExecFunc(c, "Awake");
}

void BehaviourSystem::ExecStart(ChaiBehaviour &c)
{
	ExecFunc(c, "Start");
}

void BehaviourSystem::ExecUpdate(ChaiBehaviour &c)
{
	ExecFunc(c, "Update");
}

void BehaviourSystem::ExecOnCollisionEnter(ChaiBehaviour &c, Entity &other)
{
	ExecFunc_OneArg<Entity>(c, "OnCollisionEnter", other);
}

void BehaviourSystem::ExecOnCollisionExit(ChaiBehaviour &c, Entity &other)
{
	ExecFunc_OneArg<Entity>(c, "OnCollisionExit", other);
}

void BehaviourSystem::ExecOnTriggerEnter(ChaiBehaviour &c, Entity &other)
{
	ExecFunc_OneArg<Entity>(c, "OnTriggerEnter", other);
}

void BehaviourSystem::ExecOnTriggerExit(ChaiBehaviour &c, Entity &other)
{
	ExecFunc_OneArg<Entity>(c, "OnTriggerEnter", other);
}

//helper functions for awake/start/update
void BehaviourSystem::ExecFunc(ChaiBehaviour &c, const std::string &funcName)
{
	std::string object_name = c.className + std::to_string(c.getID());
	auto obj = chai.eval(object_name);
	auto func = chai.eval<std::function<void(Boxed_Value &)> >(funcName);
	
	func(obj);
}

bool BehaviourSystem::hasClass(const std::string &name)
{
	
	return (!_chaiClasses.count(name) < 1);
}


/*
https://mightynotes.wordpress.com/2017/04/10/chaiscript-tutorial-5-calling-chaiscript-functions-from-c-and-vise-versa/
https://github.com/ChaiScript/ChaiScript/commit/185888501011601a2c4aff318939fa0f4f59892c
https://github.com/ChaiScript/ChaiScript/blob/develop/cheatsheet.md
https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/
http://discourse.chaiscript.com/t/call-methods-of-a-script-object-inside-c/88

We need to be able to call Update from anywhere + need to know variable name?

EZ to do

assign a ChaiBehaviour object a name + ID
probably just bhv_type_ID (ID == entity ID)
Start == Initialized in Constructor

Update gets called every frame

Box2D collision information is left for us to figure out
this shouldn't be impossible, but making triggers would be nice

http://www.iforce2d.net/b2dtut/collision-callbacks

*/

/*
TODO:
Transform component
will hold our rotation data (for rendering)

Animation system:
Animator component (will hold sprite data + animations)
animation system updates each one
anim states + all the other stuff
look at galacticat????

*/