#pragma once
#include "Scene.h"
#include "Engine.h"
namespace Scene
{
#pragma region HELPER FUNCTIONS
	bool hasName(std::string &comp, std::string name)
	{
		return comp == name;
	}
	template <class T>
	bool exists(T iter, const char * name)
	{
		return iter->value.HasMember(name);
	}
	template<class T>
	bool isNumber(T iter, const char * name)
	{
		return iter->value[name].IsNumber();
	}
	template<class T>
	bool isString(T iter, const char * name)
	{
		return iter->value[name].IsString();
	}
	template<class T>
	float getFloat(T iter, const char * name)
	{
		return iter->value[name].GetFloat();
	}
	template<class T>
	const char* getString(T iter, const char * name)
	{
		
		return iter->value[name].GetString();
	}

	template <class T>
	void AssignFloat(T iter, const char * name, float &f)
	{
		if (exists(iter, name) && isNumber(iter, name))
		{
			f = getFloat(iter, name);
		}
	}
	template <class T>
	void AssignString(T iter, const char * name, std::string &s)
	{
		if (exists(iter, name) && isString(iter, name))
		{
			s = getString(iter, name);
		}
	}
#pragma endregion

	void load(const char * file)
	{
		//fstream is a file stream class
		////this lets us read data from the file
		//a stream is considered 1 line of a file
		//you can also output things as streams
		//ex: cerr << "some error" << std::end
		//^^output stream^^
		//string entryInFile
		//ss >> entryInFile
		//^^input stream^^
		std::fstream sceneFile(file);
		
		
		//check if scenefile is valid:
		//	file is not empty
		//	fole exists
		if (!sceneFile)
		{
			std::cerr << "Error could not open: " << file << "\"" << std::endl;
			system("Pause");
			exit(EXIT_FAILURE);
		}

		//copy the scenes filestream into string stream
		std::stringstream ss;
		ss << sceneFile.rdbuf();//rdbuf (read buffer) outputs the entire file at once
		sceneFile.close();//close the file as we have all the data stored in  memory

		//we are going to  use rapidjson to parse the json file
		rapidjson::Document document;//create a json document which will hold our data in json format
		if (document.Parse(ss.str().c_str()).HasParseError())
		{
			std::cerr << file << "Parsing Error: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
			system("Pause");
			exit(EXIT_FAILURE);
		}

		//now that we have checked for errors and have parsed the files, we will check that we have entities
		if (!(document.HasMember("Entities") && document["Entities"].IsArray()))
		{
			//if we don't pass this test then we have no entities in this file
			std::cerr << "JSON PARSING ERROR: " << file << "does not appear to have an entity array" << std::endl;
			system("Pause");
			exit(EXIT_FAILURE);
		}

		for (auto entitiesIter = document["Entities"].Begin(); entitiesIter != document["Entities"].End(); ++entitiesIter)
		{
			Entity &e = Entity::create();
			//create and attachn components
			//entitiesIter->MemberBegin(); is equivalent to
			//(*entitiesIter).MemberBegin;
			//-> is used for de-referencing
			for (auto componentIter = entitiesIter->MemberBegin(); componentIter != entitiesIter->MemberEnd(); ++componentIter)
			{
				
				std::string componentTypeName = componentIter->name.GetString();
				if (componentTypeName == "Position")
				{
					Position &p = e.AddComponent<Position>();
					
					if (!e.hasComponent<Transform>())
					{
						Transform &transform = e.AddComponent<Transform>();
						transform.position = &p; //store a reference in our transform, for easy access
					}
					else
					{
						Transform &transform = e.getComponent<Transform>();
						transform.position = &p;
					}

					AssignFloat(componentIter, "x", p.x);
					AssignFloat(componentIter, "y", p.y);
				}
				else if (componentTypeName == "Sprite")
				{
					Sprite &sprite = e.AddComponent<Sprite>();
					
					AssignString(componentIter, "texture", sprite.texture);
					std::cout << componentIter->value["texture"].GetString() << std::endl;
				}
				else if (componentTypeName == "RigidBody")
				{
					RigidBody &rigidBody = e.AddComponent<RigidBody>();
				}
				else if (componentTypeName == "CircleCollider")
				{
					CircleCollider &col = e.AddComponent<CircleCollider>();
					if (componentIter->value.HasMember("radius") && componentIter->value["radius"].IsNumber())
					{
						col.radius = componentIter->value["radius"].GetFloat();
					}
					col.SetOwner(e);
				}
				else if (hasName(componentTypeName, "RectangleCollider"))
				{
					RectangleCollider &col = e.AddComponent<RectangleCollider>();
					AssignFloat(componentIter, "width", col.width);
					AssignFloat(componentIter, "height", col.height);
					col.SetOwner(e);
				}
				else if (hasName(componentTypeName, "ChaiBehaviour"))
				{
					ChaiBehaviour &bhv = e.AddComponent<ChaiBehaviour>();
					BehaviourSystem *behaviourSystem = Engine::getInstance()->getBehaviourSystem();
					std::string className, path;
					AssignString(componentIter, "path", path);
					AssignString(componentIter, "name", className);
					if (!behaviourSystem->hasClass(className))
					{
						behaviourSystem->loadScript(path);
					}
					behaviourSystem->initBehaviour(bhv, className);
				}	
				else if (hasName(componentTypeName, "Transform") && !e.hasComponent<Transform>())
				{
					//we only want one transform per entity
					//in this case we have to check if we have the name (json file) + ensure that we don't have a transform already
					//if we have a transform, do nothing
					Transform &t = e.AddComponent<Transform>();
					if (e.hasComponent<Position>())
					{
						t.position = &e.getComponent<Position>();
					}
					if (e.hasComponent<Rotator>())
					{
						t.rotator = &e.getComponent<Rotator>();
					}
					//if we see this tag, we should assume that we can instantiate it by default
					//to 0,0 and a rotation of 0
					if (exists(componentIter, "Default"))
					{
						t.position = &e.AddComponent<Position>();
						t.rotator = &e.AddComponent<Rotator>();
					}
				}
				else if (hasName(componentTypeName, "Rotator"))
				{
					Rotator &r = e.AddComponent<Rotator>();
					AssignFloat(componentIter, "angle", r.angle);
					Transform &t = e.hasComponent<Transform>() ? e.getComponent<Transform>() : e.AddComponent<Transform>();
					t.rotator = &r;
				}

			}
		}
	}
}