#include "Serialization.h"
#include "Application.h"
#include "ModuleEngineOrder.h"

void Serialization::YK_SaveScene(std::string first_save)
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

    std::string save_path = "Library/Scenes/";
    if (first_save == "ifyouputthatnameyougotme")
    {
        save_path += app->GetSceneName() + ".ykscene";
    }
    else
    {
        save_path += first_save + ".ykscene";
    }
    json_serialize_to_file_pretty(scene_value, save_path.c_str());
    json_value_free(scene_value);

    Console::LogInConsole(save_path + " saved.");
}

void Serialization::YK_LoadScene(std::string path)
{
    std::string load_path = "Library/Scenes/" + path + ".ykscene";
    JSON_Value* scene_value = json_parse_file(load_path.c_str());

    if (scene_value == NULL)
    {
        Console::LogInConsole("Error loading  " + load_path + " scene.");
        return;
    }

    JSON_Object* scene_object = json_value_get_object(scene_value);
    
    DeSerializeSceneCamera(scene_object);

    JSON_Array* gameobjects_array = json_object_get_array(scene_object, "GameObjects");

    for (size_t i = 0; i < json_array_get_count(gameobjects_array); i++)
    {
        DeSerializeGameObject(gameobjects_array, i);
    }

    app->engine_order->LoadSerializedGO();
    json_value_free(scene_value);

    Console::LogInConsole(load_path + " loaded.");
}

void Serialization::SerializeSceneCamera(JSON_Object* json_object)
{
    JSON_Value* camera_value = json_value_init_object();
    JSON_Object* camera_object = json_value_get_object(camera_value);

    // values
    SetFloat3x4(camera_object, "CameraMatrix", app->camera->sceneCamera.cameraFrustum.WorldMatrix());

    json_object_set_value(json_object, "SceneCamera", camera_value);
}

void Serialization::SerializeGameObject(JSON_Array* json_array, GameObject* go)
{
    JSON_Value* go_value = json_value_init_object();
    JSON_Object* go_object = json_value_get_object(go_value);
    
    // values
    SetString(go_object, "UUID", go->UUID.c_str());
    if (go->parent != NULL) SetString(go_object, "ParentUUID", go->parent->UUID.c_str());
    SetString(go_object, "Name", go->name.c_str());
    SetFloat3(go_object, "Position", go->transform->GetLocalTransform().position);
    SetFloat3(go_object, "Rotation", go->transform->GetLocalTransform().rotation);
    SetFloat3(go_object, "Scale", go->transform->GetLocalTransform().scale);
    SetBool(go_object, "Enabled", go->enabled);
    SetBool(go_object, "Visible", go->visible);
    SetBool(go_object, "VisibleOnEditor", go->visible_on_editor);
    SetString(go_object, "Tag", go->tag.c_str());
    SetBool(go_object, "IsCamera", go->is_camera);
    if (go->GetComponentList().size() > 1) CheckComponents(go_object, go->GetComponentList());
    
    json_array_append_value(json_array, go_value);
}

void Serialization::DeSerializeSceneCamera(JSON_Object* json_object)
{
    JSON_Value* camera_value = json_object_get_value(json_object, "SceneCamera");
    JSON_Object* camera_object = json_value_get_object(camera_value);

    float3x4 matrix = GetFloat3x4(camera_object, "CameraMatrix");
    app->camera->sceneCamera.cameraFrustum.SetWorldMatrix(matrix);
}

void Serialization::DeSerializeGameObject(JSON_Array* json_array, size_t it)
{
    JSON_Object* go_object = json_array_get_object(json_array, it);

    SerializedGO go;
    go.UUID = GetString(go_object, "UUID");
    if (it > 0) go.parentUUID = GetString(go_object, "ParentUUID");
    go.name = GetString(go_object, "Name");
    go.position = GetFloat3(go_object, "Position");
    go.rotation = GetFloat3(go_object, "Rotation");
    go.scale = GetFloat3(go_object, "Scale");
    go.enabled = GetBool(go_object, "Enabled");
    go.visible = GetBool(go_object, "Visible");
    go.visible_on_editor = GetBool(go_object, "VisibleOnEditor");
    go.tag = GetString(go_object, "Tag");
    go.is_camera = GetBool(go_object, "IsCamera");

    JSON_Array* components_array = json_object_get_array(go_object, "Components");

    for (size_t i = 0; i < json_array_get_count(components_array); i++)
    {
        JSON_Object* component_object = json_array_get_object(components_array, i);

        switch (GetInt(component_object, "Type"))
        {
        case 2:
            go.components_type.push_back(2);
            go.components_enabled.push_back(GetBool(component_object, "Enabled"));
            go.show_aabb = GetBool(component_object, "ShowAABB");
            go.show_obb = GetBool(component_object, "ShowOBB");
            go.mesh_path = GetString(component_object, "MeshPath");
            break;
        case 3:
            go.components_type.push_back(3);
            go.components_enabled.push_back(GetBool(component_object, "Enabled"));
            go.selected_texture = GetString(component_object, "SelectedTexturePath");
            break;
        case 4:
            go.components_type.push_back(4);
            go.components_enabled.push_back(GetBool(component_object, "Enabled"));
            go.camera_id = GetInt(component_object, "CameraID");
            go.fov = GetFloat(component_object, "FOV");
            go.camera_range = GetFloat(component_object, "CameraRange");
            go.camera_matrix = GetFloat3x4(component_object, "CameraMatrix");
            break;
        }
    }

    app->engine_order->serialized_go.push_back(go);
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

void Serialization::SetFloat3x4(JSON_Object* json_object, std::string variable, float3x4 value)
{
    JSON_Value* j_value = json_value_init_array();
    JSON_Array* j_array = json_value_get_array(j_value);
    json_array_append_number(j_array, value.v[0][0]);
    json_array_append_number(j_array, value.v[0][1]);
    json_array_append_number(j_array, value.v[0][2]);
    json_array_append_number(j_array, value.v[0][3]);
    json_array_append_number(j_array, value.v[1][0]);
    json_array_append_number(j_array, value.v[1][1]);
    json_array_append_number(j_array, value.v[1][2]);
    json_array_append_number(j_array, value.v[1][3]);
    json_array_append_number(j_array, value.v[2][0]);
    json_array_append_number(j_array, value.v[2][1]);
    json_array_append_number(j_array, value.v[2][2]);
    json_array_append_number(j_array, value.v[2][3]);
    json_object_set_value(json_object, variable.c_str(), j_value);
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

float3x4 Serialization::GetFloat3x4(JSON_Object* json_object, std::string variable)
{
    JSON_Array* j_array = json_object_get_array(json_object, variable.c_str());
    float aux0 = (float)json_array_get_number(j_array, 0);
    float aux1 = (float)json_array_get_number(j_array, 1);
    float aux2 = (float)json_array_get_number(j_array, 2);
    float aux3 = (float)json_array_get_number(j_array, 3);
    float aux4 = (float)json_array_get_number(j_array, 4);
    float aux5 = (float)json_array_get_number(j_array, 5);
    float aux6 = (float)json_array_get_number(j_array, 6);
    float aux7 = (float)json_array_get_number(j_array, 7);
    float aux8 = (float)json_array_get_number(j_array, 8);
    float aux9 = (float)json_array_get_number(j_array, 9);
    float aux10 = (float)json_array_get_number(j_array, 10);
    float aux11 = (float)json_array_get_number(j_array, 11);
    return float3x4(aux0, aux1, aux2, aux3, aux4, aux5, aux6, aux7, aux8, aux9, aux10, aux11);
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
            SetInt(component_object, "Type", (int)components[i]->GetType());
            SetBool(component_object, "Enabled", components[i]->IsEnabled());
            SetBool(component_object, "ShowAABB", dynamic_cast<C_MeshRenderer*>(components[i])->show_aabb);
            SetBool(component_object, "ShowOBB", dynamic_cast<C_MeshRenderer*>(components[i])->show_obb);
            SetString(component_object, "MeshPath", dynamic_cast<C_MeshRenderer*>(components[i])->GetMeshPath());
            break;
        case Component::TYPE::MATERIAL:
            SetInt(component_object, "Type", (int)components[i]->GetType());
            SetBool(component_object, "Enabled", components[i]->IsEnabled());
            SetString(component_object, "SelectedTexturePath", dynamic_cast<C_Material*>(components[i])->selected_texture);
            break;
        case Component::TYPE::CAMERA:
            SetInt(component_object, "Type", (int)components[i]->GetType());
            SetBool(component_object, "Enabled", components[i]->IsEnabled());
            SetInt(component_object, "CameraID", dynamic_cast<C_Camera*>(components[i])->cameraID);
            SetFloat(component_object, "FOV", dynamic_cast<C_Camera*>(components[i])->GetCamera()->GetFOV());
            SetFloat(component_object, "CameraRange", dynamic_cast<C_Camera*>(components[i])->GetCamera()->GetRange());
            SetFloat3x4(component_object, "CameraMatrix", dynamic_cast<C_Camera*>(components[i])->GetCamera()->cameraFrustum.WorldMatrix());
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
    json_value_free(settings_value);
}