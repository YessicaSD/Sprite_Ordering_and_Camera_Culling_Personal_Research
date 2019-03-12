#include "ent_Static.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Map.h"

ent_Static::ent_Static(int x, int y, std::string name) :j1Entity(Types::STATIC, x, y, name)
{
	if (name == "tree") {
		type = ent_Static::Type::TREE;
		SetRect(16, 0, 32, 48);
		pivot = { 15, 36 };
	}
	else if (name == "statue") {
		type = ent_Static::Type::STATUE;
		SetRect(0, 48, 112, 160);
		pivot = { 60, 140 };
	}


	//Isometric map
	else if (name == "pilar") {
		type = ent_Static::Type::PILAR;
		SetRect(256, 0, 64, 64);
		pivot = { 32,48 };
	}
	else if (name == "corner") {
		type = ent_Static::Type::CORNER;
		SetRect(64, 0, 64, 64);
		pivot = { 32,48 };
	}
	else if (name == "wall_u_r") {
		type = ent_Static::Type::WALL_UP_RIGHT;
		SetRect(0, 0, 64, 64);
		pivot = { 32,48 };
	}
	else if (name == "wall_u_l") {
		type = ent_Static::Type::WALL_UP_LEFT;
		SetRect(384, 0, 64, 64);
		pivot = { 32,48 };
	}
	else if (name == "wall_d_r") {
		type = ent_Static::Type::WALL_DOWN_RIGHT;
		SetRect(448, 0, 64, 64);
		pivot = { 32,48 };
	}
	else if (name == "wall_d_l") {
		type = ent_Static::Type::WALL_DOWN_LEFT;
		SetRect(192, 0, 64, 64);
		pivot = { 32,48 };
	}
	else if (name == "door") {
		type = ent_Static::Type::DOOR;
		SetRect(128, 0, 64, 64);
		pivot = { 32,48 };
	}

	else {
		LOG("There isn't any type assigned to %s name entity", name.data());
	}

	size = iPoint(frame.w, frame.h);


	if (App->map->data.type == MAPTYPE_ISOMETRIC) {
		position.x /= App->map->data.tile_width * 0.5f;
		position.y /= App->map->data.tile_height;

		iPoint pos = App->map->MapToWorld(position.x + 1, position.y + 1);
		position.create(pos.x, pos.y);
	}

	data.tileset.texture = App->tex->Load(App->map->data.properties.objects_path.data());
}

ent_Static::~ent_Static()
{
}

void ent_Static::Draw()
{
	App->render->Blit(data.tileset.texture, position.x, position.y, &frame);
}

void ent_Static::SetRect(int x, int y, int w, int h)
{
	frame.x = x;
	frame.y = y;
	frame.w = w;
	frame.h = h;
}
