#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
	for (pile = map_node.child("pile"); pile && ret; pile = pile.next_sibling("pile")) {
		LOG("%i %i %i", pile.attribute("x").as_int(), pile.attribute("y").as_int(), pile.attribute("z").as_int());
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

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	Cube c(1, 4, 1);
	c.color = Color(255, 0, 0, 255);
	c.SetPos(0, 2, 0);
	c.Render();

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