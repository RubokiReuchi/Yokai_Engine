#include "DN_SpawnProjectile.h"
#include "Application.h"
#include "TempGameObject.h"
#include "Console.h"

DN_SpawnProjectile::DN_SpawnProjectile(ImVec2 pos, BluePrint* bp) : BP_Node("Spawn Projectile", BP_Node::TYPE::SIMPLE, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin1(bp->unique_id++, "Parent GO", BP_Pin::TYPE::Object, bp);
	ask_pin1.node = this;
	inputs.push_back(ask_pin1);
	BP_Pin ask_pin2(bp->unique_id++, "Speed", BP_Pin::TYPE::Float, bp);
	ask_pin2.node = this;
	inputs.push_back(ask_pin2);
	BP_Pin ask_pin3(bp->unique_id++, "Life Time", BP_Pin::TYPE::Float, bp);
	ask_pin3.node = this;
	inputs.push_back(ask_pin3);
	BP_Pin ask_pin4(bp->unique_id++, "Execute", BP_Pin::TYPE::Executable, bp);
	ask_pin4.node = this;
	inputs.push_back(ask_pin4);
	BP_Pin send_pin(bp->unique_id++, "Projectile", BP_Pin::TYPE::Object, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_SpawnProjectile::DN_SpawnProjectile(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, int ask3_pin_id, int ask4_pin_id, int send_pin_id) : BP_Node("Spawn Projectile", BP_Node::TYPE::SIMPLE, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin1(ask1_pin_id, "Parent GO", BP_Pin::TYPE::Object, bp);
	ask_pin1.node = this;
	inputs.push_back(ask_pin1);
	BP_Pin ask_pin2(ask2_pin_id, "Speed", BP_Pin::TYPE::Float, bp);
	ask_pin2.node = this;
	inputs.push_back(ask_pin2);
	BP_Pin ask_pin3(ask3_pin_id, "Life Time", BP_Pin::TYPE::Float, bp);
	ask_pin3.node = this;
	inputs.push_back(ask_pin3);
	BP_Pin ask_pin4(ask4_pin_id, "Execute", BP_Pin::TYPE::Executable, bp);
	ask_pin4.node = this;
	inputs.push_back(ask_pin4);
	BP_Pin send_pin(send_pin_id, "Projectile", BP_Pin::TYPE::Object, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_SpawnProjectile::~DN_SpawnProjectile()
{

}

void DN_SpawnProjectile::Activate(float dt)
{
	if (inputs[0].IsPinLinked() && inputs[1].IsPinLinked() && inputs[2].IsPinLinked())
	{
		TempGameObject* projectile = new TempGameObject(app->engine_order->rootGameObject, inputs[0].GetOpositePin()->node->info_as_go->transform->GetGlobalTransform(), TempGoType::PROJECTIL, inputs[2].GetOpositePin()->node->info_as_number, inputs[1].GetOpositePin()->node->info_as_number, "Projectile");
		GameObject* sphere = MeshImporter::LoadMesh("Assets/sphere.fbx");
		sphere->SetParent(projectile);
		sphere->name = "Bullet";
	}
	else
	{
		Console::LogInConsole("Missing some Pin Link.");
	}
}