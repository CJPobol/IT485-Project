
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
    
    ent->model = gf3d_model_load("cube");
    ent->think = agumon_think;
    ent->update = agumon_update;
    vector3d_copy(ent->position,position);
    return ent;
}

void agumon_update(Entity* self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    vector3d_add(self->position, self->position, self->velocity);
}

void agumon_think(Entity *self)
{
    if (!self)return;
    
    switch (self->state)
    {
        case ES_idle:
            break;
        case ES_move:
            self->velocity.x = gfc_crandom();
            break;
        case ES_dead:
            break;
        case ES_attack:
            break;
    }
}

/*eol@eof*/
