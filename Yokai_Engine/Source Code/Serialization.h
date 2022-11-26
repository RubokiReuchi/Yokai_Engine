#pragma once

#include "Globals.h"
#include "GameObject.h"
#include "parson/parson.h"

class GameObject;

class Serialization
{
public:
	static void YK_SaveScene();
	static void SaveSettings();
	static void LoadSettings();

private:
	static void SerializeGameObject(JSON_Array* json_array, GameObject* go);

	static void SetInt(JSON_Object* json_object, std::string variable, int value);
	static void SetString(JSON_Object* json_object, std::string variable, std::string value);
	static void SetFloat3(JSON_Object* json_object, std::string variable, float3 value);
	static void SetQuat(JSON_Object* json_object, std::string variable, Quat value);
	static void SetBool(JSON_Object* json_object, std::string variable, bool value);
	static int GetInt(JSON_Object* json_object, std::string variable);
	static std::string GetString(JSON_Object* json_object, std::string variable);
	static float3 GetFloat3(JSON_Object* json_object, std::string variable);
	static Quat GetQuat(JSON_Object* json_object, std::string variable);
	static bool GetBool(JSON_Object* json_object, std::string variable);

	static void CheckComponents(JSON_Object* json_object, std::vector<Component*> components);

};