#include "M_Render.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "EO_Editor.h"

M_Render::M_Render()
{
    // Create the shaders
    basic_shader = new Re_Shader("Assets/shaders/basic.vertex.shader", "Assets/shaders/basic.fragment.shader");
    outline_shader = new Re_Shader("Assets/shaders/outline.vertex.shader", "Assets/shaders/outline.fragment.shader");
}

M_Render::~M_Render()
{
    RELEASE(basic_shader);
}

uint M_Render::InitManageRender(Re_Mesh mesh)
{
    if (initialized) LOG("Init already done");
    initialized = true;
    
    this->total_vertices.insert(total_vertices.begin(), mesh.vertices->begin(), mesh.vertices->end());
    this->total_indices.insert(total_indices.begin(), mesh.indices->begin(), mesh.indices->end());

    CreateMeshBuffers();

    return AddMesh(mesh);
}

void M_Render::Draw()
{
    bool go_selected = false;
    bool go_selected_child = false;
    size_t num_meshes = meshes.size();
    if (!initialized) return;
    if (meshes.empty()) return;
    for (auto &mesh : meshes)
    {
        if (!mesh.second.visible || !mesh.second.in_camera) // mesh renderer with visibility set to false or not in camera frustrum
        {
            num_meshes--;
        }
        else if (app->renderer3D->drawing_scene && !mesh.second.visible_on_editor) // mesh renderer with visibility in editor set to false
        {
            num_meshes--;
        }
        else
        {
            mesh.second.Update();
            model_matrices.push_back(mesh.second.model_matrix); // Insert matrices of each mesh in this M_Render
            texture_ids.push_back(mesh.second.GL_id);
        }
        if (app->renderer3D->drawing_scene && mesh.second.is_outlined)
        {
            go_selected = true;
            if (mesh.second.is_outlined_child) go_selected_child = true;
            selected_total_indices = total_indices;
            selected_model_matrices.push_back(mesh.second.outline_matrix);
        }
    }
    if (num_meshes == 0)
    {
        return; // draw if no mesh is visible
    }

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    // Bind shader with ViewMatrix and ProjectionMatrix
    glUseProgram(basic_shader->program_id);
    glUniformMatrix4fv(glGetUniformLocation(basic_shader->program_id, "view"), 1, GL_FALSE, app->camera->currentDrawingCamera->GetViewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(basic_shader->program_id, "projection"), 1, GL_FALSE, app->camera->currentDrawingCamera->GetProjectionMatrix());

    // Call binded info
    glBindVertexArray(VAO);

    // model matrix bind
    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &model_matrices.front(), model_matrices.size() * sizeof(float4x4));
    glUnmapBuffer(GL_ARRAY_BUFFER);

    // texture id bind
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    void* ptr2 = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr2, &texture_ids.front(), texture_ids.size() * sizeof(float));
    glUnmapBuffer(GL_ARRAY_BUFFER);

    // Bind shader with all binded textures
    for (uint i = 0; i < M_Texture::bindedTextures; i++)
    {
        std::string texture = "textures[" + std::to_string(i) + "]";
        glUniform1i(glGetUniformLocation(basic_shader->program_id, texture.c_str()), i);
    }

    // Draw meshes with texture
    glDrawElementsInstanced(GL_TRIANGLES, total_indices.size(), GL_UNSIGNED_INT, 0, model_matrices.size());
    glBindVertexArray(0);

    // outline
    if (go_selected)
    {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        if (go_selected_child) glDisable(GL_DEPTH_TEST);

        // Bind shader with ViewMatrix and ProjectionMatrix
        glUseProgram(outline_shader->program_id);
        glUniformMatrix4fv(glGetUniformLocation(outline_shader->program_id, "view"), 1, GL_FALSE, app->camera->currentDrawingCamera->GetViewMatrix());
        glUniformMatrix4fv(glGetUniformLocation(outline_shader->program_id, "projection"), 1, GL_FALSE, app->camera->currentDrawingCamera->GetProjectionMatrix());

        // Call binded info
        glBindVertexArray(VAO);

        // model matrix bind
        glBindBuffer(GL_ARRAY_BUFFER, MBO);
        void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(ptr, &selected_model_matrices.front(), selected_model_matrices.size() * sizeof(float4x4));
        glUnmapBuffer(GL_ARRAY_BUFFER);

        // Draw meshes with texture
        glDrawElementsInstanced(GL_TRIANGLES, selected_total_indices.size(), GL_UNSIGNED_INT, 0, selected_model_matrices.size());
        glBindVertexArray(0);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

        selected_total_indices.clear();
        selected_model_matrices.clear();
    }

    // free information
    model_matrices.clear();
    texture_ids.clear();
    M_Texture::UnBindTextures();
}

uint M_Render::AddMesh(Re_Mesh& mesh)
{
    uint meshID = ++id_counter;
    meshes[meshID] = mesh;
    return meshID;
}

std::string M_Render::GetMeshPath()
{

}

void M_Render::CreateMeshBuffers()
{
    // Create Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create Vertex Buffer Object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * total_vertices.size(), &total_vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, normals));
    // vertex texture coords
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, tex_coords));

    // Create Index Buffer Object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * total_indices.size(), &total_indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    // Create Model Matrix Buffer Object
    glGenBuffers(1, &MBO);
    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float4x4) * 1000000, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)0);
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)sizeof(float4));
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 2));
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float4x4), (void*)(sizeof(float4) * 3));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    // Create TextureID Buffer Object
    glGenBuffers(1, &TBO);
    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10000, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);
}