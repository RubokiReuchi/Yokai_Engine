#include "ModuleEngineOrder.h"
#include "ModuleInput.h"
#include "EO_Editor.h"
#include "EO_Game.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "C_MeshRenderer.h"

ModuleEO::ModuleEO(bool start_enabled) : Module(start_enabled)
{
}

ModuleEO::~ModuleEO()
{
}

bool ModuleEO::Init()
{
    rootGameObject = new GameObject(nullptr, "Root", "None");
    seed = (uint)time(NULL);
    return true;
}

bool ModuleEO::Start()
{
    engine_order[(uint)EO_NUM::EDITOR] = editor = new EO_Editor();
    engine_order[(uint)EO_NUM::GAME] = game = new EO_Game();

    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->Start();
        }
    }

    // load street to second deliver
    MeshImporter::LoadMesh("Assets/street.fbx");
    uint texture041 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_041.png");
    uint texture040 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_040.png");
    uint texture038_037_034_026_022 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_038_037_034_026_022.tga");
    uint texture036 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_036.tga");
    uint texture033 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_033.tga");
    uint texture032_014 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_032_014.tga");
    uint texture024 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_024.tga");
    uint texture023 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_023.tga");
    uint texture017 = TextureImporter::LoadTexture("Assets/StreetTextures/Building_017.tga");
    uint texture_green = TextureImporter::LoadTexture("Assets/StreetTextures/GreenColor.png");
    uint texture_grey = TextureImporter::LoadTexture("Assets/StreetTextures/GreyColor.png");
    uint texture_lightgrey = TextureImporter::LoadTexture("Assets/StreetTextures/LightGreyColor.png");

    for (auto& gameObject : game_objects)
    {
        C_MeshRenderer* c_mr = dynamic_cast<C_MeshRenderer*>(gameObject.second->GetComponent(Component::TYPE::MESH_RENDERER));
        C_Material* mat = dynamic_cast<C_Material*>(gameObject.second->GetComponent(Component::TYPE::MATERIAL));
        if (c_mr != nullptr)
        {
            if (gameObject.second->name == "City_building_041")
            {
                c_mr->GetMesh().texture_id = (float)texture041;
                mat->SetTexture(c_mr->GetTexture((float)texture041));
            }
            else if (gameObject.second->name == "City_building_040")
            {
                c_mr->GetMesh().texture_id = (float)texture040;
                mat->SetTexture(c_mr->GetTexture((float)texture040));
            }
            else if (gameObject.second->name == "City_building_038" || gameObject.second->name == "City_building_037" 
                || gameObject.second->name == "City_building_034" || gameObject.second->name == "City_building_026"
                || gameObject.second->name == "City_building_022")
            {
                c_mr->GetMesh().texture_id = (float)texture038_037_034_026_022;
                mat->SetTexture(c_mr->GetTexture((float)texture038_037_034_026_022));
            }
            else if (gameObject.second->name == "City_building_036")
            {
                c_mr->GetMesh().texture_id = (float)texture036;
                mat->SetTexture(c_mr->GetTexture((float)texture036));
            }
            else if (gameObject.second->name == "City_building_033")
            {
                c_mr->GetMesh().texture_id = (float)texture033;
                mat->SetTexture(c_mr->GetTexture((float)texture033));
            }
            else if (gameObject.second->name == "City_building_032" || gameObject.second->name == "City_building_014")
            {
                c_mr->GetMesh().texture_id = (float)texture032_014;
                mat->SetTexture(c_mr->GetTexture((float)texture032_014));
            }
            else if (gameObject.second->name == "City_building_024")
            {
                c_mr->GetMesh().texture_id = (float)texture024;
                mat->SetTexture(c_mr->GetTexture((float)texture024));
            }
            else if (gameObject.second->name == "City_building_023")
            {
                c_mr->GetMesh().texture_id = (float)texture023;
                mat->SetTexture(c_mr->GetTexture((float)texture023));
            }
            else if (gameObject.second->name == "City_building_017")
            {
                c_mr->GetMesh().texture_id = (float)texture017;
                mat->SetTexture(c_mr->GetTexture((float)texture017));
            }
            else if (gameObject.second->name == "Plane001")
            {
                c_mr->GetMesh().texture_id = (float)texture_green;
                mat->SetTexture(c_mr->GetTexture((float)texture_green));
            }
            else if (gameObject.second->name == "Object010")
            {
                c_mr->GetMesh().texture_id = (float)texture_lightgrey;
                mat->SetTexture(c_mr->GetTexture((float)texture_lightgrey));
            }
            else if (gameObject.second->name == "Line002")
            {
                c_mr->GetMesh().texture_id = (float)texture_grey;
                mat->SetTexture(c_mr->GetTexture((float)texture_grey));
            }
        }
    }

    return true;
}

update_status ModuleEO::PreUpdate(float dt)
{
    for (size_t i = 0; i < delete_queu.size(); i++)
    {
        RELEASE(delete_queu[i]);
    }

    if (load_scene)
    {
        app->renderer3D->model_render.CleanMap();

        rootGameObject = new GameObject(serialized_go[0]);
        for (size_t i = 1; i < serialized_go.size(); i++)
        {
            GameObject* new_go = new GameObject(serialized_go[i]);
        }

        serialized_go.clear();
        loadedSerializedMeshes.clear();
        load_scene = false;
    }

    if (new_scene)
    {
        app->renderer3D->model_render.CleanMap();
        app->SetSceneName("defaultnameofstartscene");
        new_scene = false;
    }

    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->PreUpdate();
        }
    }
    return UPDATE_CONTINUE;
}

update_status ModuleEO::Update(float dt)
{
    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->Update();
        }
    }
    return UPDATE_CONTINUE;
}

update_status ModuleEO::PostUpdate(float dt)
{
    return UPDATE_CONTINUE;
}

void ModuleEO::DrawEO()
{
    for (int i = 0; i < (uint)EO_NUM::EDITOR; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->PostUpdate();
        }
    }
}

void ModuleEO::DrawEO_Editor()
{
    if (engine_order[(uint)EO_NUM::EDITOR] && engine_order[(uint)EO_NUM::EDITOR]->IsEnabled())
    {
        engine_order[(uint)EO_NUM::EDITOR]->PostUpdate();
    }
}

bool ModuleEO::CleanUp()
{
    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i])
        {
            engine_order[i]->CleanUp();
        }
        RELEASE(engine_order[i]);
    }
    RELEASE(rootGameObject);
    return true;
}

uint ModuleEO::AddGameObject(GameObject* go)
{
    game_objects[id_counter] = go;
    return id_counter++;
}

void ModuleEO::LoadSerializedGO()
{
    rootGameObject->DeleteGameObject();
    app->camera->game_cameras.clear();
    MeshImporter::CleanMaps();
    M_Texture::DeleteTextures();
    load_scene = true;
}

GameObject* ModuleEO::GetGameObjectByUUID(std::string uuid)
{
    for (auto& go : game_objects)
    {
        if (go.second->UUID == uuid)
        {
            return go.second;
        }
    }

    return NULL;
}

void ModuleEO::NewScene()
{
    for (auto& child : rootGameObject->children)
    {
        child->DeleteGameObject();
    }
    app->camera->game_cameras.clear();
    MeshImporter::CleanMaps();
    M_Texture::DeleteTextures();
    new_scene = true;

    GameObject* main_camera = new GameObject(app->engine_order->rootGameObject, "Main Camera", "Camera", true);
    app->camera->InitNewGameCamera(main_camera);
    dynamic_cast<C_Transform*>(main_camera->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(float3(5.0f, 1.5f, 0), float3(1, 1, 1), float3(0, -90, 0));
    C_Camera* cam = dynamic_cast<C_Camera*>(main_camera->GetComponent(Component::TYPE::CAMERA));
    float3x4 start_matrix = float3x4(0.0f, 0.0f, 1.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.5f, -1.0f, 0.0f, 0.0f, 0.0f);
    cam->GetCamera()->cameraFrustum.SetWorldMatrix(start_matrix);
}

std::string ModuleEO::GenerateUUID()
{
    std::string uuid = "";
    char c;
    int r;

    srand(seed++);
    for (int i = 0; i < 16; i++)
    {
        r = rand() % 35;
        if (r < 26) c = 'A' + r;
        else c = char(r + 23);
        uuid += c;
    }

    return uuid;
}