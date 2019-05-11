#pragma once
#include "ISystem.h"
#include <fstream>
#include <iostream>
#include "Input.h"
#include <chaiscript.hpp>
#include <utility>
#include "ChaiBehaviour.h"

class BehaviourSystem : System<BehaviourSystem>
{
public:
	BehaviourSystem();

	~BehaviourSystem();

	void Update();

	void loadScript(const std::string &fileLocation);

	void initBehaviour(ChaiBehaviour &c, const std::string &className);
	
	void Start(); //calls all start functions in behaviour systems
	
	bool hasClass(const std::string &name);

private:
	//this is our main interpreter
	static chaiscript::ChaiScript chai;
	std::string fileName; //for now we will only run off one file, in the future we'll make it work with multiple

	void initChaiScript();
	
	//functions that will be used for updating scripts
	void handleAwake();
	void handleStart();
	void handleUpdate();
	void handleCollisionEnter();
	void handleCollisionExit();
	void handleTriggerEnter();
	void handleTriggerExit();
	
	void ExecAwake(ChaiBehaviour &c);
	void ExecStart(ChaiBehaviour &c);
	void ExecUpdate(ChaiBehaviour &c);
	void ExecOnCollisionEnter(ChaiBehaviour &c, Entity &other);
	void ExecOnCollisionExit(ChaiBehaviour &c, Entity &other);
	void ExecOnTriggerEnter(ChaiBehaviour &c, Entity &other);
	void ExecOnTriggerExit(ChaiBehaviour &c, Entity &other);
	void ExecFunc(ChaiBehaviour &c, const std::string &funcName);
	
	template<typename T>
	void ExecFunc_OneArg(ChaiBehaviour &c, const std::string &funcName, T& _arg)
	{
		std::string object_name = c.className + std::to_string(c.getID());
		auto obj = chai.eval(object_name);
		auto func = chai.eval<std::function<void(Boxed_Value &, T& other)> >(funcName);
		func(obj,_arg);
	}


	//has the behaviour system started?
	bool isStarted = false;

	std::unordered_map<std::string, bool> _chaiClasses;
};


 

