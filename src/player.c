#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"


void player_think(Entity *self);
void player_update(Entity *self);

Entity *player_new(Vector3D position)
{
    Entity *ent = NULL;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
    
    //ent->model = gf3d_model_load("dino");
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position,position);
    ent->rotation.x = -M_PI;
    return ent;
}


void player_think(Entity *self)
{
    int radians = self->rotation.z;
    Vector3D forward;
    Vector3D right;
    Vector3D up;
    Vector3D dash;
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    int keyPressed = 0;

    vector3d_angle_vectors(self->rotation, &forward, &right, &up);
    vector3d_set_magnitude(&forward, 0.01);
    vector3d_set_angle_by_radians(&forward, radians);
    vector3d_set_magnitude(&right, 0.01);
    vector3d_set_angle_by_radians(&right, radians - GFC_HALF_PI);
    vector3d_set_magnitude(&up,0.1);
    vector3d_set_magnitude(&dash, 1);
    vector3d_set_angle_by_radians(&dash, radians);

    if (keys[SDL_SCANCODE_W])
    {   
        vector3d_add(self->position,self->position,forward);
    }
    if (keys[SDL_SCANCODE_S])
    {
        vector3d_add(self->position,self->position,-forward);        
    }
    if (keys[SDL_SCANCODE_D])
    {
        vector3d_add(self->position,self->position,right);
    }
    if (keys[SDL_SCANCODE_A])    
    {
        vector3d_add(self->position,self->position,-right);
    }


    if (keys[SDL_SCANCODE_SPACE])self->position.z = 100;
    //else self->position.z = 0;
    if (keys[SDL_SCANCODE_Z])self->position.z = -100;
    //else self->position.z = 0;
    
    if (keys[SDL_SCANCODE_UP])self->rotation.x -= 0.0050;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x += 0.0050;

    if (keys[SDL_SCANCODE_RIGHT])
    {
        self->rotation.z += 0.0050;
        //radians -= 0.0050;
    }
    if (keys[SDL_SCANCODE_LEFT])
    {
        self->rotation.z -= 0.0050;
        //radians += 0.0050;
    }

    if (keys[SDL_SCANCODE_C] && keyPressed == 0)
    {
        vector3d_add(self->position, self->position, dash);
        keyPressed = 1;
    }
}

void player_update(Entity *self)
{
    if (!self)return;

    //creates "gravity"
    self->position.z -= 0.01;

    //creates "floor" to game world
    if (self->position.z <= 0)
        self->position.z = 0;


    //creates "walls" to game world
    
    if (self->position.x >= 100)
        self->position.x = 100;
    if (self->position.x <= -100)
        self->position.x = -100;
    if (self->position.y >= 100)
        self->position.y = 100;
    if (self->position.y <= -100)
        self->position.y = -100;
    
    gf3d_camera_set_position(self->position);
    gf3d_camera_set_rotation(self->rotation);
}

/*eol@eof*/
