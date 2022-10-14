#include "ModuleEngineOrder.h"
#include "EO_Editor.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "C_MeshRenderer.h"

ModuleEO::ModuleEO(bool start_enabled) : Module(start_enabled)
{
}

ModuleEO::~ModuleEO()
{
}

bool ModuleEO::Start()
{
    rootGameObject = new GameObject(nullptr, "Root", "None");

    MeshImporter::LoadMesh("Assets/BakerHouse.fbx");
    uint bakerTexture = TextureImporter::ImportTextureSTBI("Assets/Baker_house.png");

    for (auto gameObject : game_objects)
    {
        C_MeshRenderer* c_mr = dynamic_cast<C_MeshRenderer*>(gameObject.second->GetComponent(Component::TYPE::MESH_RENDERER));
        if (c_mr != nullptr)
        {
            c_mr->GetMesh().texture_id = bakerTexture;
        }
    }

    engine_order[(uint)EO_NUM::EDITOR] = new EO_Editor();

    for (int i = 0; i < (uint)EO_NUM::NUM_EO_TYPE; i++)
    {
        if (engine_order[i] && engine_order[i]->IsEnabled())
        {
            engine_order[i]->Start();
        }
    }
    return true;
}

update_status ModuleEO::PreUpdate(float dt)
{
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