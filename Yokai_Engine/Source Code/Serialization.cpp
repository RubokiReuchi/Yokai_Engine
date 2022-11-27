#include "Serialization.h"
#include "Application.h"
#include "ModuleEngineOrder.h"

void Serialization::YK_SaveScene()
{
    JSON_Value* scene_value = json_value_init_object();
    JSON_Object* scene_object = json_value_get_object(scene_value);

    SerializeSceneCamera(scene_object);

    JSON_Value* gameobjects_value = json_value_init_array();
    JSON_Array* gameobjects_array = json_value_get_array(gameobjects_value);

    for (auto& go : app->engine_order->game_objects)
    {
        SerializeGameObject(gameobjects_array, go.second);
    }

    json_object_set_value(scene_object, "GameObjects", gameobjects_value);

    json_serialize_to_file_pretty(scene_value, "Library/Scenes/scene.ykscene");
    json_value_free(scene_value);
}

void Serialization::SerializeSceneCamera(JSON_Object* json_object)
{
    JSON_Value* camera_value = json_value_init_object();
    JSON_Object* camera_object = json_value_get_object(camera_value);

    // values
    SetFloat3(camera_object, "Position", app->camera->sceneCamera.Position);
    SetFloat3(camera_object, "Reference", app->camera->sceneCamera.Reference);
    SetFloat(camera_object, "FOV", app->camera->sceneCamera.GetFOV());

    json_object_set_value(json_object, "SceneCamera", camera_value);
}

void Serialization::SerializeGameObject(JSON_Array* json_array, GameObject* go)
{
    JSON_Value* go_value = json_value_init_object();
    JSON_Object* go_object = json_value_get_object(go_value);
    
    // values
    SetString(go_object, "UUID", go->UUID.c_str());
    if (go->parent != NULL) // excludes root
    {
        SetString(go_object, "ParentUUID", go->parent->UUID.c_str());
    }
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
    if (go->GetComponentList().size() > 1) CheckComponents(go_object, go->GetComponentList());
    
    json_array_append_value(json_array, go_value);
}

void Serialization::SetInt(JSON_Object* json_object, std::string variable, int value)
{
    json_object_set_number(json_object, variable.c_str(), value);
}

void Serialization::SetString(JSON_Object* json_object, std::string variable, std::string value)
{
    json_object_set_string(json_object, variable.c_str(), value.c_str());
}

void Serialization::SetFloat(JSON_Object* json_object, std::string variable, float value)
{
    json_object_set_number(json_object, variable.c_str(), value);
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

int Serialization::GetInt(JSON_Object* json_object, std::string variable)
{
    return (int)json_object_get_number(json_object, variable.c_str());
}

std::string Serialization::GetString(JSON_Object* json_object, std::string variable)
{
    return json_object_get_string(json_object, variable.c_str());
}

float Serialization::GetFloat(JSON_Object* json_object, std::string variable)
{
    return (float)json_object_get_number(json_object, variable.c_str());
}

float3 Serialization::GetFloat3(JSON_Object* json_object, std::string variable)
{
    JSON_Array* j_array = json_object_get_array(json_object, variable.c_str());
    float aux0 = (float)json_array_get_number(j_array, 0);
    float aux1 = (float)json_array_get_number(j_array, 1);
    float aux2 = (float)json_array_get_number(j_array, 2);
    return float3(aux0, aux1, aux2);
}

Quat Serialization::GetQuat(JSON_Object* json_object, std::string variable)
{
    JSON_Array* j_array = json_object_get_array(json_object, variable.c_str());
    float aux0 = (float)json_array_get_number(j_array, 0);
    float aux1 = (float)json_array_get_number(j_array, 1);
    float aux2 = (float)json_array_get_number(j_array, 2);
    float aux3 = (float)json_array_get_number(j_array, 3);
    return Quat(aux0, aux1, aux2, aux3);
}

bool Serialization::GetBool(JSON_Object* json_object, std::string variable)
{
    return json_object_get_boolean(json_object, variable.c_str());
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
            SetFloat(component_object, "FOV", dynamic_cast<C_Camera*>(components[i])->GetCamera()->GetFOV());
            SetFloat(component_object, "CameraRange", dynamic_cast<C_Camera*>(components[i])->GetCamera()->GetRange());
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

void Serialization::SaveSettings()
{
    JSON_Value* settings_value = json_value_init_object();
    JSON_Object* settings_object = json_value_get_object(settings_value);

    // values
    SetInt(settings_object, "FPS", app->fpsCap);
    SetBool(settings_object, "Vsync", app->renderer3D->vsync);
    SetString(settings_object, "WindowSize", (std::to_string(app->window->width) + "x" + std::to_string(app->window->height)).c_str());

    json_serialize_to_file_pretty(settings_value, "Config/settings.ykconfig");
    json_value_free(settings_value);
}

void Serialization::LoadSettings()
{
    JSON_Value* settings_value = json_parse_file("Config/settings.ykconfig");

    if (settings_value == NULL)
    {
        Console::LogInConsole("Error opening 'Config/settings.ykconfig'.");
        return;
    }

    JSON_Object* settings_object = json_value_get_object(settings_value);
    app->fpsCap = GetInt(settings_object, "FPS");
    app->renderer3D->vsync = GetBool(settings_object, "Vsync");
    std::string aux = GetString(settings_object, "WindowSize");

    size_t npos = aux.find_last_of("x") + 1;
    std::string value = aux;
    app->window->height = stoi(value.substr(npos));
    app->window->width = stoi(value.erase(npos, 9));

    SDL_SetWindowSize(app->window->window, app->window->width, app->window->height);
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    SDL_SetWindowPosition(app->window->window, (DM.w - app->window->width) / 2, (DM.h - app->window->height) / 2);
}