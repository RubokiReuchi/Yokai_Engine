#include "M_Render.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"

M_Render::M_Render()
{
    basic_shader = new Re_Shader("../Source Code/shaders/basic.vertex.shader", "../Source Code/shaders/basic.fragment.shader");
}

M_Render::~M_Render()
{
    RELEASE(basic_shader);
}

uint M_Render::SetMeshInformation(Re_Mesh& mesh)
{
    if (initialized) LOG("Tried to call M_Render::SetMeshInformation more than once in a single Render Manager instance");
    // Set this RenderManager Mesh information.
    this->total_vertices.insert(total_vertices.begin(), mesh.vertices->begin(), mesh.vertices->end());
    this->total_indices.insert(total_indices.begin(), mesh.indices->begin(), mesh.indices->end());

    CreateBuffers();

    Re_Mesh firstMesh;
    firstMesh.InitAsMeshInformation(mesh.position, mesh.scale);

    mesh.CleanUp(); // Destroy the original vertex and index data (now it is stored inside this render manager).
    initialized = true;

    return AddMesh(firstMesh); // Adds a copy of the original mesh into the mesh list.
}

void M_Render::Draw()
{
    float3 npos, nrot, nscl;
    size_t num_meshes = meshes.size();
    if (!initialized) return; // This is placed here for security reasons. No RenderManager should be created without being initialized.
    if (meshes.empty())
    {
        LOG("A Render Manager is being updated without any meshes!");
        return;
    }
    for (auto &mesh : meshes)
    {
        if (!mesh.second.visible) // mesh renderer with visibility set to false
        {
            num_meshes--;
        }
        else
        {
            mesh.second.Update();
            model_matrices.push_back(mesh.second.model_matrix); // Insert updated matrices
            texture_ids.push_back(mesh.second.OpenGL_texture_id);
            npos = mesh.second.position;
            nrot = mesh.second.rotation;
            nscl = mesh.second.scale;
        }
    }
    if (num_meshes == 0)
    {
        return;
    }

    // Update View and Projection matrices
    //basic_shader->Bind();
    //basic_shader->SetMatFloat4v("view", app->camera->currentDrawingCamera->GetViewMatrix());
    //basic_shader->SetMatFloat4v("projection", app->camera->currentDrawingCamera->GetProjectionMatrix());


    // Draw using Dynamic Geometry
    glBindVertexArray(VAO);

    // Update Model matrices
    /*glBindBuffer(GL_ARRAY_BUFFER, MBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &model_matrices.front(), model_matrices.size() * sizeof(float4x4));
    glUnmapBuffer(GL_ARRAY_BUFFER);*/

    // Update TextureIDs
    /*glBindBuffer(GL_ARRAY_BUFFER, TBO);
    void* ptr2 = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr2, &texture_ids.front(), texture_ids.size() * sizeof(float));
    glUnmapBuffer(GL_ARRAY_BUFFER);*/

    for (uint i = 0; i < M_Texture::bindedTextures; i++)
    {
        basic_shader->SetInt(("textures[" + std::to_string(i) + "]").c_str(), i);
    }

    glPushMatrix();
    glTranslatef(npos.x, npos.y, npos.z);
    glRotatef(nrot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(nrot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(nrot.z, 0.0f, 0.0f, 1.0f);
    glScalef(nscl.x, nscl.y, nscl.z);

    // Draw
    glDrawElementsInstanced(GL_TRIANGLES, total_indices.size(), GL_UNSIGNED_INT, 0, model_matrices.size());

    glPopMatrix();

    glBindVertexArray(0);

    // Reset model matrices.
    model_matrices.clear();
    texture_ids.clear();
    M_Texture::UnBindTextures();
}

uint M_Render::AddMesh(Re_Mesh& mesh)
{
    if (!initialized)
    {
        LOG("Trying to add mesh information into a RenderManager that has not been initialized yet!");
    }
    uint meshID = ++id_counter;
    meshes[meshID] = mesh;
    return meshID;
}

void M_Render::CreateBuffers()
{
    // Create Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * total_vertices.size(), &total_vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
    // vertex texture coords
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    // Create Index Buffer Object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * total_indices.size(), &total_indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Create Model Matrix buffer object
    glGenBuffers(1, &MBO);

    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float4x4) * 1000000, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);

    // You can't pass an entire matrix, so we go row by row.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)sizeof(float4));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 2));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 3));

    // Set instancing interval
    //glVertexAttribDivisor(1, 1);
    //glVertexAttribDivisor(2, 1);
    //glVertexAttribDivisor(3, 1);
    //glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    // Create TextureID buffer object
    glGenBuffers(1, &TBO);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10000, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

    //glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);
}