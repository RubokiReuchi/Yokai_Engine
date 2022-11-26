#include "Serialization.h"
#include "ModuleFile.h"

void Serialization::SerializeGameObject(JSON_Array* json_array, GameObject* go)
{
    JSON_Value* root_value = json_value_init_object();
    JSON_Object* root_object = json_value_get_object(root_value);
    
    // values
    SetString(root_object, "UUID", go->UUID.c_str());
    SetString(root_object, "ParentUUID", go->parent->UUID.c_str());
    SetString(root_object, "Name", go->name.c_str());
    SetFloat3(root_object, "Position", go->transform->GetLocalTransform().position);
    SetFloat3(root_object, "Rotation", go->transform->GetLocalTransform().rotation);
    SetFloat3(root_object, "Scale", go->transform->GetLocalTransform().scale);
    SetBool(root_object, "Enabled", go->enabled);
    SetBool(root_object, "Visible", go->visible);
    SetBool(root_object, "VisibleOnEditor", go->visible_on_editor);
    SetString(root_object, "Tag", go->tag.c_str());
    SetBool(root_object, "IsCamera", go->is_camera);
    SetInt(root_object, "GameObjectID", go->id);
    
    json_serialize_to_file_pretty(root_value, "Library/Scenes/scene.ykscene");
    json_value_free(root_value);
}

void Serialization::SetInt(JSON_Object* json_object, std::string variable, int value)
{
    json_object_set_number(json_object, variable.c_str(), value);
}

void Serialization::SetString(JSON_Object* json_object, std::string variable, std::string value)
{
    json_object_set_string(json_object, variable.c_str(), value.c_str());
}

void Serialization::SetFloat3(JSON_Object* json_object, std::string variable, float3 value)
{
    JSON_Value* j_value = json_value_init_array();
    JSON_Array* j_array = json_value_get_array(j_value);
    json_array_append_number(j_array, value[0]);
    json_array_append_number(j_array, value[1]);
    json_array_append_number(j_array, value[2]);
    json_object_set_value(json_object, variable.c_str(), j_value);
}

void Serialization::SetBool(JSON_Object* json_object, std::string variable, bool value)
{
    json_object_set_boolean(json_object, variable.c_str(), value);
}

void Serialization::CheckComponents(JSON_Object* json_object, std::vector<Component*> components)
{

}