
#include "simple_logger.h"
#include "agumon.h"

void agumon_update(Entity* self);
void agumon_think(Entity *self);

Entity *agumon_new(Vector3D position)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no agumon for you!");
        return NULL;
    }
    
    ent->model = gf3d_model_load("dino");
    ent->think = agumon_think;
    ent->update = agumon_update;
    vector3d_copy(ent->position,position);

    ent->scale = vector3d(2,2,2);
    return ent;
}

void agumon_update(Entity* self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
}

void agumon_think(Entity *self)
{
    if (!self)return;
    
}

/*eol@eof*/
