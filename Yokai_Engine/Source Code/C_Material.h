#pragma once

#include "Re_Mesh.h"
#include "Component.h"
#include "C_MeshRenderer.h"

class C_Material : public Component
{
public:
	C_Material(GameObject* gameObject);
	virtual ~C_Material() override;

	void OnEditor();

	void SetTexture(std::string texture);
	std::string GetTexture()
	{
		return selected_texture;
	}

	Re_Mesh& GetMesh();

private:
	C_MeshRenderer* renderer = nullptr;

	std::string selected_texture = "Default";

	friend class Serialization;
};