#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"

#include "gfc_audio.h"
#include "gfc_input.h"


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
    ent->onMenu = 1;
    ent->balance = 0;
    ent->itempickup = gfc_sound_load("audio/item_pickup.wav", 1, 1);
    
    for (int i = 0; i < 10; i++)
    {
        ent->itemOwned[i] = 0;
        ent->collectedCoin[i] = 0;
    }
        

    return ent;
}

void wallClimb(Entity* self)
{
    if (self->itemOwned[3])
        if (self->position.x >= 1000 || self->position.x <= -1000 || self->position.y >= 1000 || self->position.y <= -1000)
            self->position.z += 0.4;
}

interacting = 0;
boxGiven = 0;
Entity* editingEntity;

void moveEntity(Entity* ent)
{
    editingEntity = ent;
}

void interact1(Entity* self)
{
    self->interacting = 1;
    if (self->boxGiven == 1)
    {
        self->itemOwned[0] = 1;
        gfc_sound_play(self->itempickup, 0, 1, -1, -1);
    }
        
    else
    {
        self->itemOwned[8] = 1;
        gfc_sound_play(self->itempickup, 0, 1, -1, -1);
    }
        
    
}

void interact2(Entity* self)
{
    self->interacting = 1;
    self->balance += 5;
}

void interact3(Entity* self) //this will be the market man
{
    self->interacting = 1;
    self->shopping = 1;   
}

void interact4(Entity* self, Uint8* keys)
{
    self->interacting = 1;
    if (self->itemOwned[8])
    {
        //give mystery box? (Y/N)
        //if (keys[SDL_SCANCODE_Y])
        {
            self->boxGiven = 1;
            self->itemOwned[8] = 0;
        }
    }
    if (self->boxGiven == 1)
    {
        //ask for key
        if (self->itemOwned[0])
        {
            //give key? (Y/N)
            //if (keys[SDL_SCANCODE_Y])
            {
                self->itemOwned[0] = 0;
                self->keyGiven = 1;
                //here take this cool teleporter I found
                //self->itemOwned[9] = 1;
                self->balance += 30;
                slog("Balance: %i", self->balance);
                gfc_sound_play(self->itempickup, 0, 1, -1, -1);
            }
        }
    }
    
}

void interact5(Entity* self)
{
    self->interacting = 1;
    //self->itemOwned[7] = 1;
    self->balance += 10;
    slog("Balance: %i", self->balance);
    gfc_sound_play(self->itempickup, 0, 1, -1, -1);
}

int crouching = 0;
int slowFall = 0;
int inAir = 0;


void player_think(Entity *self)
{
    self->readingNote = 0;
    slowFall = 0;
    crouching = 0;
    int radians = self->rotation.z;
    Vector3D forward;
    Vector3D right;
    Vector3D up;
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    float magnitude = 0.0000000001;
    

    vector3d_angle_vectors(self->rotation, &forward, &right, &up);
    vector3d_set_magnitude(&forward, magnitude);
    vector3d_set_angle_by_radians(&forward, radians + GFC_HALF_PI);
    vector3d_set_magnitude(&right, magnitude);
    vector3d_set_angle_by_radians(&right, radians);
    vector3d_set_magnitude(&up,magnitude);

    if (gfc_input_command_pressed("1"))
    {
        if (self->editing)
            self->selectedToEdit = 1;
        else
        {
            if (self->balance >= 5)
            {
                gfc_sound_play(self->itempickup, 0, 1, -1, -1);
                self->itemOwned[1] = 1;
                self->balance -= 5;
                slog("Balance: %i", self->balance);
            }
        }
    }
    if (gfc_input_command_pressed("2"))
    {
        if (self->editing)
            self->selectedToEdit = 2;
        else
        {
            if (self->balance >= 10)
            {
                gfc_sound_play(self->itempickup, 0, 1, -1, -1);
                self->itemOwned[2] = 1;
                self->balance -= 10;
                slog("Balance: %i", self->balance);
            }
        }
    }
    if (gfc_input_command_pressed("3"))
    {
        if (self->editing)
            self->selectedToEdit = 3;
        else
        {
            if (self->balance >= 5)
            {
                gfc_sound_play(self->itempickup, 0, 1, -1, -1);
                self->itemOwned[5] = 1;
                self->balance -= 5;
                slog("Balance: %i", self->balance);
            }
        }
    }
        
    if (gfc_input_command_pressed("4"))
    {
        if (self->editing)
            self->selectedToEdit = 4;
        else
        {
            if (self->balance >= 10)
            {
                gfc_sound_play(self->itempickup, 0, 1, -1, -1);
                self->itemOwned[6] = 1;
                self->balance -= 10;
                slog("Balance: %i", self->balance);
            }
        }
    }
    if (gfc_input_command_pressed("5"))
    {
        if (self->editing)
            self->selectedToEdit = 5;
        else
        {
            if (self->balance >= 10)
            {
                gfc_sound_play(self->itempickup, 0, 1, -1, -1);
                self->itemOwned[3] = 1;
                self->balance -= 10;
                slog("Balance: %i", self->balance);
            }
        }
    }
    if (gfc_input_command_pressed("6"))
    {
        if (!self->editing && self->balance >= 1)
        {
            gfc_sound_play(self->itempickup, 0, 1, -1, -1);
            self->itemOwned[4] = 1;
            self->balance -= 1;
            slog("Balance: %i", self->balance);
        }
    }
    if (gfc_input_command_pressed("7"))
    {
        if (!self->editing && self->balance >= 20)
        {
            gfc_sound_play(self->itempickup, 0, 1, -1, -1);
            self->itemOwned[9] = 1;
            self->balance -= 20;
            slog("Balance: %i", self->balance);
        }
    }

    if (keys[SDL_SCANCODE_RETURN])
    {
        self->onMenu = 0;
        self->position = vector3d(0, 0, 60);
        self->rotation.x = -GFC_PI;
        self->editing = 0;
    }

    if (keys[SDL_SCANCODE_ESCAPE])
    {
        self->onMenu = 1;
        self->editing = 0;
    }

    if (keys[SDL_SCANCODE_L])
    {
        self->onMenu = 0;
        self->position = vector3d(0, 0, 3000);
        self->rotation.x = -GFC_HALF_PI;
        self->rotation.z = 0;
        self->editing = 1;
    }

    if (keys[SDL_SCANCODE_W] && self->onMenu == 0 && self->editing == 0)
    {  
        if (self->interacting == 0)
        {
            vector3d_add(self->position, self->position, forward);
            wallClimb(self);
        }
    }
    if (keys[SDL_SCANCODE_S] && self->onMenu == 0 && self->editing == 0)
    {
        if (self->interacting == 0)
        {
            vector3d_add(self->position,self->position,-forward);
            wallClimb(self);
        }
    }
    if (keys[SDL_SCANCODE_D] && self->onMenu == 0 && self->editing == 0)
    {
        if (self->interacting == 0)
        {
            vector3d_add(self->position, self->position, right);
            wallClimb(self);
        }
    }
    if (keys[SDL_SCANCODE_A] && self->onMenu == 0 && self->editing == 0)
    {
        if (self->interacting == 0)
        {
            vector3d_add(self->position,self->position,-right);
            wallClimb(self);
        }
    }

    //jump
    if (keys[SDL_SCANCODE_SPACE] && self->onMenu == 0 && self->editing == 0)
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
    

    if (keys[SDL_SCANCODE_Z] && self->onMenu == 0 && self->editing == 0)
    {
        if (self->itemOwned[7]) 
        {
            self->position.z = -10;
            crouching = 1;
            vector3d_set_magnitude(&forward, magnitude / 10);
            vector3d_set_magnitude(&right, magnitude / 10);
        }
    }
    
    if (keys[SDL_SCANCODE_UP] && self->onMenu == 0)
    {
        if (self->editing == 0)
            self->rotation.x -= 0.005;
        else
        {
            editingEntity->position.y += 1;
        }
    }
    if (keys[SDL_SCANCODE_DOWN] && self->onMenu == 0)
    {
        if (self->editing == 0)
            self->rotation.x += 0.005;
        else
        {
            editingEntity->position.y -= 1;
        }
    }

    if (keys[SDL_SCANCODE_RIGHT] && self->onMenu == 0)
    {
        if (self->editing)
            editingEntity->position.x += 1;
        else
            self->rotation.z -= 0.005;
    }
    if (keys[SDL_SCANCODE_LEFT] && self->onMenu == 0)
    {
        if (self->editing)
            editingEntity->position.x -= 1;
        else
            self->rotation.z += 0.005;
        //radians += 0.0050;
    }

    if (keys[SDL_SCANCODE_C] && self->onMenu == 0 && self->editing == 0)
    {
        if (self->itemOwned[9])
        {
            self->position.x = forward.x * 100;
            self->position.y = forward.y * 100;
        }
        
    }

    if (keys[SDL_SCANCODE_V] && self->onMenu == 0 && self->editing == 0)
    {
        if (self->itemOwned[6])
            slowFall = 1;
    }

    if (gfc_input_command_pressed("interact") && self->onMenu == 0 && self->editing == 0)
    {
        if (self->interacting == 0)
        {
            
            if (self->nearNPC == 1)
                interact1(self);
            else if (self->nearNPC == 2)
                interact2(self);
            else if (self->nearNPC == 3)
                interact3(self);
            else if (self->nearNPC == 4)
                interact4(self, keys);
            else if (self->nearNPC == 5)
                interact5(self);            
        }        
    }

    if (keys[SDL_SCANCODE_R] && self->onMenu == 0 && self->editing == 0)
    {
        self->interacting = 0;
        self->shopping = 0;
    }

    if (keys[SDL_SCANCODE_Q] && self->onMenu == 0 && self->editing == 0)
    {
        self->readingNote = 1;
    }
}

void player_update(Entity *self)
{
    if (!self)return;

    //creates "gravity"
    if (self->editing == 0) {
        if (slowFall == 0)
            self->position.z -= 0.2;
        else
            self->position.z -= 0.02;
    }
        

    //creates "floor" to game world
    if (crouching == 1)
    {
        if (self->position.z <= 20)
        {
            self->position.z = 20;
            inAir = 0;
        }
            
    }
    else
        if (self->position.z <= 60)
        {
            self->position.z = 60;
            inAir = 0;
        }


    //creates "walls" to game world
    
    if (self->position.x > 999)
        self->position.x = 999;
    if (self->position.x < -999)
        self->position.x = -999;
    if (self->position.y > 999)
        self->position.y = 999;
    if (self->position.y < -999)
        self->position.y = -999;

    



    
    gf3d_camera_set_position(self->position);
    gf3d_camera_set_rotation(self->rotation);
}

/*eol@eof*/
