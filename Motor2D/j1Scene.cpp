#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "ent_Player.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Scene.h"

#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");

	bool ret = true;

	for (conf = conf.first_child(); conf != nullptr; conf = conf.next_sibling()) {
		scenes.push_back(conf.attribute("name").as_string());
	}

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	ret = App->map->Load("Iso.tmx");

	CreateEntities();

	App->render->camera = App->render->CameraInitPos();

	return ret;
}

void j1Scene::CreateEntities()
{
	//iterate all objects of tile to find entities

	for (std::list<ColliderObject*>::iterator position = App->map->data.colliders.begin(); position != App->map->data.colliders.end(); position++) {
		if ((*position)->name == "Player") {
			App->entities->CreateEntity(j1Entity::Types::PLAYER, (*position)->coll_x, (*position)->coll_y, (*position)->name);
		}
		else if ((*position)->ent_type == "static") {
			App->entities->CreateEntity(j1Entity::Types::STATIC, (*position)->coll_x, (*position)->coll_y, (*position)->name);
				
		}
		else if ((*position)->name == "NPC") {
			App->entities->CreateEntity(j1Entity::Types::NPC, (*position)->coll_x, (*position)->coll_y, (*position)->name);
		}
		else {
			LOG("There isn't any entity with name %s and type %s", (*position)->name.data(), (*position)->ent_type.data());
		}
	}
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("UpdateScene", Profiler::Color::Red);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 300 * dt;


	//------Debug Keys---------------------
	//Show/hide grid
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->map->draw_grid = !App->map->draw_grid;

	//Change between zoom levels-----------------------------
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		App->win->SetScale(1);

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		App->win->SetScale(2);

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		App->win->SetScale(3);
	//-------------------------------------------------------

	//Show/hide entities quad size and pivot
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		entities_box = !entities_box;

	//-------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		bool change_map = false;
		uint id = -1;

		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
			change_map = true;
			id = 0;
		}
		else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
			change_map = true;
			id = 1;
		}
		else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
			change_map = true;
			id = 2;
		}

		if (change_map) {
			App->map->CleanUp();
			App->entities->CleanUp();

			App->map->Load(scenes[id].data());			
			CreateEntities();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->quit_game = true;


	App->map->Draw();	
	
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return App->entities->CleanUp();
}