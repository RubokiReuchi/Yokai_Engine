#pragma once

#include "Globals.h"
#include "GameObject.h"
#include "Console.h"
#include "parson/parson.h"

class GameObject;
class BluePrint;

struct SerializedNode
{
	int id;
	std::string name;
	std::vector<int> inputs_id;
	std::vector<std::string> inputs_box;
	std::vector<std::string> inputs_vector3_box0;
	std::vector<std::string> inputs_vector3_box1;
	std::vector<std::string> inputs_vector3_box2;
	std::vector<int> inputs_go_ID;
	std::vector<int> outputs_id;
	ImVec2 pos;
};

struct SerializedLink
{
	int id;
	int input_id;
	int output_id;
	float3 color;
};

struct SerializedGO
{
	std::string UUID;
	std::string parentUUID = "-1";
	std::string name;
	float3 position;
	float3 rotation;
	float3 scale;
	bool enabled;
	bool visible;
	bool visible_on_editor;
	std::string tag;
	bool is_camera;

	std::vector<int> components_type;
	std::vector<bool> components_enabled;

	bool show_aabb;
	bool show_obb;
	std::string mesh_path;

	std::string selected_texture;

	int camera_id;
	float fov;
	float camera_range;
	float3x4 camera_matrix;

	int unique_id;
	std::string bp_name;
	std::vector<SerializedNode> nodes;
	std::vector<SerializedLink> links;
};

class Serialization
{
public:
	static void YK_SaveScene(std::string first_save = "ifyouputthatnameyougotme");
	static void YK_LoadScene(std::string path);
	static void SaveSettings();
	static void LoadSettings();

	static void SerializeBlueprint(BluePrint* bp, std::string save_path);
	static void DeSerializeBlueprint(SerializedGO* go, std::string load_path);

private:
	static void SerializeSceneCamera(JSON_Object* json_object);
	static void SerializeGameObject(JSON_Array* json_array, GameObject* go);
	static void DeSerializeSceneCamera(JSON_Object* json_object);
	static void DeSerializeGameObject(JSON_Array* json_array, size_t it);

	static void SetInt(JSON_Object* json_object, std::string variable, int value);
	static void SetString(JSON_Object* json_object, std::string variable, std::string value);
	static void SetFloat(JSON_Object* json_object, std::string variable, float value);
	static void SetFloat3(JSON_Object* json_object, std::string variable, float3 value);
	static void SetQuat(JSON_Object* json_object, std::string variable, Quat value);
	static void SetBool(JSON_Object* json_object, std::string variable, bool value);
	static void SetFloat3x4(JSON_Object* json_object, std::string variable, float3x4 value);
	static int GetInt(JSON_Object* json_object, std::string variable);
	static std::string GetString(JSON_Object* json_object, std::string variable);
	static float GetFloat(JSON_Object* json_object, std::string variable);
	static float3 GetFloat3(JSON_Object* json_object, std::string variable);
	static Quat GetQuat(JSON_Object* json_object, std::string variable);
	static bool GetBool(JSON_Object* json_object, std::string variable);
	static float3x4 GetFloat3x4(JSON_Object* json_object, std::string variable);

	static void CheckComponents(JSON_Object* json_object, std::vector<Component*> components);

	static void SerializeNodes(JSON_Object* json_object, std::vector<BP_Node*> nodes);
	static void SerializeLinks(JSON_Object* json_object, std::vector<BP_Link*> links);
	static void DeSerializeNodes(JSON_Object* json_object, std::vector<SerializedNode>* nodes);
	static void DeSerializeLinks(JSON_Object* json_object, std::vector<SerializedLink>* links);

};