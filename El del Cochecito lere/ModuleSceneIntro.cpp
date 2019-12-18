#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Color.h"
#include "p2DynArray.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (int i = 0; i < MAX_PILES; i++) {
		mapPiles[i] = nullptr;
	}
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Awake() {

	bool ret = false;
	map_node = LoadMap(map_file);
	if (map_file.empty() == false) {
		ret = true;
	}

	pugi::xml_node pile;
	int pilesAdded = 0;
	int colorIterator = 0;

	for (pile = map_node.child("pile"); pile && ret; pile = pile.next_sibling("pile")) {
		//LOG("%i %i %i", pile.attribute("x").as_int(), pile.attribute("y").as_int(), pile.attribute("z").as_int());
		
		Cube* c = new Cube(1.2f, 1.7f, 1.2f);
		float Yoffset = c->size.y / 2;
		if (colorIterator % 2 == 0) { c->color = Red; }
		else { c->color = White; }
		c->SetPos(pile.attribute("x").as_int(), pile.attribute("y").as_int() + Yoffset, pile.attribute("z").as_int());
		mapPiles[pilesAdded] = c;
		//Cube cnp = *c;
		//App->physics->AddBody(cnp);

		pilesAdded++;
		colorIterator++;
	}

	return ret;
}


// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	p = new Plane(0, 1, 0, 0);
	p->axis = true;

	base = Cube(0.7f, 1, 0.7f);
	float Yoffset = base.size.y / 2;
	base.SetPos(19.5f, Yoffset, -45);
	base.color = Blue;

	bar = Cube(15, 0.7f, 0.7f);
	bar.SetPos(12.3, 1.4f, -45);
	bar.color = Yellow;

	for (int i = 0; i < MAX_PILES; i++) {
		if (mapPiles[i] != nullptr)
		{
			Cube cnp = *mapPiles[i];
			App->physics->AddBody(cnp, 200000);
		}
	}
	
	App->physics->AddBody(base, 200000);
	App->physics->AddBody(bar, 200000);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int i = 0; i < MAX_PILES; i++) {
		if (mapPiles[i] != nullptr) {
			delete mapPiles[i];
			mapPiles[i] = nullptr;
		}
	}
	delete p;

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	for (int i = 0; i < MAX_PILES; i++) {
		if (mapPiles[i] != nullptr) {
			mapPiles[i]->Render();
		}
	}
	base.Render();
	bar.Render();
	p->Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

// ---------------------------------------------
pugi::xml_node ModuleSceneIntro::LoadMap(pugi::xml_document& map_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = map_file.load_file("map.xml");

	if (result == NULL) { LOG("Could not load map xml file map.xml. pugi error: %s", result.description()); }
	else ret = map_file.child("map");

	return ret;
}