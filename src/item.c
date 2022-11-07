
#include "simple_logger.h"
#include "agumon.h"

void item_update(Entity* self);
void item_think(Entity* self);

Entity* item_new(Vector3D position)
{
    Entity* ent = NULL;

    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no agumon for you!");
        return NULL;
    }

    ent->model = gf3d_model_load("cube");
    ent->think = item_think;
    ent->update = item_update;
    vector3d_copy(ent->position, position);

    ent->scale = vector3d(2, 2, 2);
    return ent;
}

void item_update(Entity* self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
}

void item_think(Entity* self)
{
    if (!self)return;

}

/*eol@eof*/
