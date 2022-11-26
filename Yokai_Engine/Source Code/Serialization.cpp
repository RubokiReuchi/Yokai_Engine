#include "Serialization.h"
#include "ModuleFile.h"

void Serialization::SerializeGameObject(JSON_Array* json_array, GameObject* go)
{
    JSON_Value* go_value = json_value_init_object();
    JSON_Object* go_object = json_value_get_object(go_value);
    
    // values
    SetString(go_object, "UUID", go->UUID.c_str());
    SetString(go_object, "ParentUUID", go->parent->UUID.c_str());
    SetString(go_object, "Name", go->name.c_str());
    SetFloat3(go_object, "Position", go->transform->GetLocalTransform().position);
    SetFloat3(go_object, "Rotation", go->transform->GetLocalTransform().rotation);
    SetFloat3(go_object, "Scale", go->transform->GetLocalTransform().scale);
    SetBool(go_object, "Enabled", go->enabled);
    SetBool(go_object, "Visible", go->visible);
    SetBool(go_object, "VisibleOnEditor", go->visible_on_editor);
    SetString(go_object, "Tag", go->tag.c_str());
    SetBool(go_object, "IsCamera", go->is_camera);
    SetInt(go_object, "GameObjectID", go->id);
    CheckComponents(go_object, go->GetComponentList());
    
    json_serialize_to_file_pretty(go_value, "Library/Scenes/scene.ykscene");
    json_value_free(go_value);
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

void Serialization::SetQuat(JSON_Object* json_object, std::string variable, Quat value)
{
    JSON_Value* j_value = json_value_init_array();
    JSON_Array* j_array = json_value_get_array(j_value);
    json_array_append_number(j_array, value.x);
    json_array_append_number(j_array, value.y);
    json_array_append_number(j_array, value.z);
    json_array_append_number(j_array, value.w);
    json_object_set_value(json_object, variable.c_str(), j_value);
}

void Serialization::SetBool(JSON_Object* json_object, std::string variable, bool value)
{
    json_object_set_boolean(json_object, variable.c_str(), value);
}

void Serialization::CheckComponents(JSON_Object* json_object, std::vector<Component*> components)
{
    JSON_Value* go_value = json_value_init_array();
    JSON_Array* go_array = json_value_get_array(go_value);

    for (size_t i = 1; i < components.size(); i++) // start at 1 to skip transformation
    {
        JSON_Value* component_value = json_value_init_object();
        JSON_Object* component_object = json_value_get_object(component_value);

        switch (components[i]->GetType())
        {
        case Component::TYPE::MESH_RENDERER:
            SetString(component_object, "Type", std::to_string((int)components[i]->GetType()));
            SetBool(component_object, "Enabled", components[i]->IsEnabled());
            SetBool(component_object, "ShowAABB", dynamic_cast<C_MeshRenderer*>(components[i])->show_aabb);
            SetBool(component_object, "ShowOBB", dynamic_cast<C_MeshRenderer*>(components[i])->show_obb);
            break;
        case Component::TYPE::MATERIAL:
            SetString(component_object, "Type", std::to_string((int)components[i]->GetType()));
            SetBool(component_object, "Enabled", components[i]->IsEnabled());
            SetString(component_object, "SelectedTexturePath", dynamic_cast<C_Material*>(components[i])->selected_texture);
            break;
        case Component::TYPE::CAMERA:
            SetString(component_object, "Type", std::to_string((int)components[i]->GetType()));
            SetBool(component_object, "Enabled", components[i]->IsEnabled());
            SetInt(component_object, "CameraID", dynamic_cast<C_Camera*>(components[i])->cameraID);
            SetQuat(component_object, "OriginalLookingDirection", dynamic_cast<C_Camera*>(components[i])->original_lookingDir);
            SetQuat(component_object, "LookingDirection", dynamic_cast<C_Camera*>(components[i])->lookingDir);
            break;
        default:
            SetString(component_object, "ERROR", "Incorrect component type.");
            break;
        }

        json_array_append_value(go_array, component_value);
    }

    json_object_set_value(json_object, "Components", go_value);
}