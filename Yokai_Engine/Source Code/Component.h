#pragma once

class GameObject;

//ERROR: Remove this include from here
//#include"DEJsonSupport.h"
#include<string>

class Component
{
public:

	enum class Type
	{
		None,
		Transform,
		MeshRenderer,
		Material,
	};

	Component(GameObject* game_object);
	virtual ~Component();

	virtual void Enable();
	virtual void Disable();

	virtual void Awake();
	virtual void Update();

#ifndef STANDALONE
	virtual bool OnEditor();
#endif // !STANDALONE

	//Maybe return JSON_Value*?
	//virtual void SaveData(JSON_Object* nObj);
	//virtual void LoadData(DEConfig& nObj);

	inline bool IsActive() {
		return active;
	}

	inline GameObject* GetGameObject() {
		return gameObject;
	}

	inline Component::Type GetType() const {
		return type;
	}
	inline std::string& GetName() {
		return name;
	}

	Type type;

protected:
	//const char* name;
	bool active;
	GameObject* gameObject;
	std::string name;

};