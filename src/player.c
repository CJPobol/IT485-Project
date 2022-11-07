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
    
    //ent->model = gf3d_model_load("cube");
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position,position);
    ent->rotation.x = -M_PI;
    
    for (int i = 0; i < 10; i++)
        ent->itemOwned[i] = 0;

    return ent;
}

void wallClimb(Entity* self)
{
    if (self->itemOwned[3])
        if (self->position.x >= 100 || self->position.x <= -100 || self->position.y >= 100 || self->position.y <= -100)
            self->position.z += 0.4;
}

int interacting = 0;

void interact1(Entity* self)
{
    interacting = 1;
    self->itemOwned[0] = 1;
}

void interact2(Entity* self)
{
    interacting = 1;
    self->itemOwned[1] = 1;
}

void interact3(Entity* self)
{
    interacting = 1;
    self->itemOwned[2] = 1;
}

void interact4(Entity* self)
{
    interacting = 1;
    self->itemOwned[3] = 1;
}

void interact5(Entity* self)
{
    interacting = 1;
    self->itemOwned[4] = 1;
}

int crouching = 0;
int slowFall = 0;
int inAir = 0;


void player_think(Entity *self)
{
    slowFall = 0;
    crouching = 0;
    int radians = self->rotation.z;
    Vector3D forward;
    Vector3D right;
    Vector3D up;
    const Uint8 * keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    float magnitude = 0.001;
    

    vector3d_angle_vectors(self->rotation, &forward, &right, &up);
    vector3d_set_magnitude(&forward, magnitude);
    vector3d_set_angle_by_radians(&forward, radians + GFC_HALF_PI);
    vector3d_set_magnitude(&right, magnitude);
    vector3d_set_angle_by_radians(&right, radians);
    vector3d_set_magnitude(&up,magnitude);

    if (keys[SDL_SCANCODE_W])
    {   
        vector3d_add(self->position,self->position,forward);
        wallClimb(self);
    }
    if (keys[SDL_SCANCODE_S])
    {
        vector3d_add(self->position,self->position,-forward);
        wallClimb(self);
    }
    if (keys[SDL_SCANCODE_D])
    {
        vector3d_add(self->position,self->position,right);
        wallClimb(self);
    }
    if (keys[SDL_SCANCODE_A])    
    {
        vector3d_add(self->position,self->position,-right);
        wallClimb(self);
    }

    //jump
    if (keys[SDL_SCANCODE_SPACE])
    {
        if (inAir == 0)
        {
            if (self->itemOwned[2])
                self->position.z = 100;
            else
                self->position.z = 50;
            inAir = 1;
        }
    }
    

    if (keys[SDL_SCANCODE_Z])
    {
        if (self->itemOwned[7]) 
        {
            self->position.z = -10;
            crouching = 1;
            vector3d_set_magnitude(&forward, magnitude / 10);
            vector3d_set_magnitude(&right, magnitude / 10);
        }
    }
    
    if (keys[SDL_SCANCODE_UP])self->rotation.x -= 0.01;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x += 0.01;

    if (keys[SDL_SCANCODE_RIGHT])
    {
        self->rotation.z -= 0.01;
        //radians -= 0.0050;
    }
    if (keys[SDL_SCANCODE_LEFT])
    {
        self->rotation.z += 0.01;
        //radians += 0.0050;
    }

    if (keys[SDL_SCANCODE_C])
    {
        if (self->itemOwned[9])
        {
            self->position.x = forward.x * 100;
            self->position.y = forward.y * 100;
        }
        
    }

    if (keys[SDL_SCANCODE_V])
    {
        if (self->itemOwned[6])
            slowFall = 1;
    }

    if (keys[SDL_SCANCODE_E])
    {
        if (interacting == 0)
        {
            
            if (self->nearNPC == 1)
                interact1(self);
            else if (self->nearNPC == 2)
                interact2(self);
            else if (self->nearNPC == 3)
                interact3(self);
            else if (self->nearNPC == 4)
                interact4(self);
            else if (self->nearNPC == 5)
                interact5(self);            
        }        
    }
}

void player_update(Entity *self)
{
    if (!self)return;

    //creates "gravity"
    if (slowFall == 0)
        self->position.z -= 0.2;
    else
        self->position.z -= 0.02;

    //creates "floor" to game world
    if (crouching == 1)
    {
        if (self->position.z <= -10)
        {
            self->position.z = -10;
            inAir = 0;
        }
            
    }
    else
        if (self->position.z <= 0)
        {
            self->position.z = 0;
            inAir = 0;
        }


    //creates "walls" to game world
    
    if (self->position.x > 100)
        self->position.x = 100;
    if (self->position.x < -100)
        self->position.x = -100;
    if (self->position.y > 100)
        self->position.y = 100;
    if (self->position.y < -100)
        self->position.y = -100;

    



    
    gf3d_camera_set_position(self->position);
    gf3d_camera_set_rotation(self->rotation);
}

/*eol@eof*/
