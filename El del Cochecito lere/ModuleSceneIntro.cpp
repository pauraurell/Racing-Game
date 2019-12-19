#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "Color.h"
#include "p2DynArray.h"
#include "PhysVehicle3d.h"
#include <math.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (int i = 0; i < MAX_PILES; i++) {
		mapPiles[i] = nullptr;
	}
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Awake()
{

	bool ret = false;
	map_node = LoadMap(map_file);
	if (map_file.empty() == false)
	{
		ret = true;
	}

	pugi::xml_node pile;
	int pilesAdded = 0;
	int colorIterator = 0;

	for (pile = map_node.child("pile"); pile && ret; pile = pile.next_sibling("pile"))
	{
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

	stands1 = new Cube(3, 3, 90);
	Yoffset = stands1->size.y / 2;
	stands1->SetPos(-100, Yoffset, 10);
	stands1->color = Red;

	stands2 = new Cube(3, 3, 90);
	stands2->SetPos(-105, Yoffset + 3, 10);
	stands2->color = Red;

	stands3 = new Cube(3, 3, 90);
	stands3->SetPos(-110, Yoffset + 6, 10);
	stands3->color = Red;

	int initialYpos;
	int cYoffset;
	int rColor;
	int spectCount = 0;
	int spectAdded = 0;
	for (int i = 0; i < MAX_SPECTATORS; i++)
	{
		Cube* c = new Cube(1.2f, 1.7f, 1.2f);
		cYoffset = c->size.y / 2;
		
		rColor = (int)(std::rand() % 6);
		switch (rColor)
		{
			case 0:
				c->color = Blue;
				break;
			case 1:
				c->color = Black;
				break;
			case 2:
				c->color = White;
				break;
			case 3:
				c->color = Yellow;
				break;
			case 4:
				c->color = LightBlue;
				break;
			case 5:
				c->color = Grey;
				break;
			case 6:
				c->color = NavyBlue;
				break;
		}
		initialYpos = (float)(std::rand() % 2);
		if (spectCount < 45) {
			c->SetPos(-100, cYoffset + Yoffset + 2 + initialYpos, 54 - spectCount * 2);
			spectYpos[spectCount] = cYoffset + Yoffset + 2 + initialYpos;
		}
		else if (spectCount >= 45 && spectCount < 90) {
			c->SetPos(-105, cYoffset + Yoffset + 5 + initialYpos, 54 - (spectCount - 45) * 2);
			spectYpos[spectCount] = cYoffset + Yoffset + 2 + initialYpos;
		}
		else if (spectCount >= 90) {
			c->SetPos(-110, cYoffset + Yoffset + 8 + initialYpos, 54 - (spectCount - 90) * 2);
			spectYpos[spectCount] = cYoffset + Yoffset + 2 + initialYpos;
		}
		spectators[spectAdded] = c;
		spectAdded++;
		spectCount++;
	}

	for (int i = 0; i < MAX_PILES; i++)
	{
		if (mapPiles[i] != nullptr)
		{
			Cube cnp = *mapPiles[i];
			App->physics->AddBody(cnp, 200000);
		}
	}
	
	pBase = App->physics->AddBody(base, 200000);
	pBar = App->physics->AddBody(bar, 200000);
	const vec3 a(1, 1, 1);

	//App->physics->AddConstraintHinge(*pBase, *pBar, a, a, a, a, true, true);

	//WIP
	//checkpoint (lap counter)
	//checkpoint = new Cube(3, 2.5, 14);
	//checkpoint->color = Blue;
	//checkpoint->SetPos(25, 1.5, -36.5f);
	LapCounter();
	lap = 0;

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
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		
	}
	
	char title[80];
	sprintf_s(title, "%.1f Km/h  ||  Gear: %i  ||  Laps: %i  || Time: ", App->player->vehicle->GetKmh(), App->player->gear, lap);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (int i = 0; i < MAX_PILES; i++) {
		if (mapPiles[i] != nullptr) {
			mapPiles[i]->Render();
		}
	}
	for (int i = 0; i < MAX_SPECTATORS; i++) {
		if (spectators[i] != nullptr) {
			spectators[i]->Render();
		}
	}
	base.Render();
	bar.Render();
	stands1->Render();
	stands2->Render();
	stands3->Render();
	p->Render();
	checkpoint.Render();

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

void ModuleSceneIntro::LapCounter()
{
	checkpoint.size.Set(3, 2.5, 13);
	checkpoint.SetPos(25, 1.5, -36.5f);
	checkpoint.color = Blue;

	checkpointBody = App->physics->AddBody(checkpoint, this, 0.0f, true);
}

void ModuleSceneIntro::SpectAnimation()
{
	for (int i = 0; i < MAX_SPECTATORS; i++)
	{



		if (i < 45) {
			spectators[i]->SetPos(-100, spectYpos[i] + spectYmov[i], 54 - i * 2);
		}
		else if (i >= 45 && i < 90) {
			spectators[i]->SetPos(-105, spectYpos[i] + spectYmov[i], 54 - (i - 45) * 2);
		}
		else if (i >= 90) {
			spectators[i]->SetPos(-110, spectYpos[i] + spectYmov[i], 54 - (i - 90) * 2);
		}
	}
}

