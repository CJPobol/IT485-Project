#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_audio.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_sprite.h"

#include "entity.h"
#include "agumon.h"
#include "player.h"
#include "world.h"

extern int __DEBUG;

int main(int argc,char *argv[])
{
    int done = 0;
    int a;

    Sprite* inv = NULL;
    Sprite* invkey = NULL;
    Sprite* invnote = NULL;
    Sprite* invshoes = NULL;
    Sprite* invspiderweb = NULL;
    Sprite* invdollar = NULL;
    Sprite* invapple = NULL;
    Sprite* invumbrella = NULL;
    Sprite* invbooklet = NULL;
    Sprite* invbox = NULL;
    Sprite* invteleporter = NULL;


    Sprite *mouse = NULL;
    
    int mousex,mousey;
    float mouseFrame = 0;
    World *w;
    
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"--debug") == 0)
        {
            __DEBUG = 1;
        }
    }
    
    init_logger("gf3d.log");    
    gfc_input_init("config/input.cfg");
    slog("gf3d begin");
    gf3d_vgraphics_init("config/setup.cfg");
    gfc_audio_init(10, 2, 1, 1, 1, 1);
    slog_sync();

    entity_system_init(1024);
    
    mouse = gf3d_sprite_load("images/pointer.png",32,32, 16);

    inv = gf3d_sprite_load("images/invtitle.png", 300, 50, 1);
    invkey = gf3d_sprite_load("images/invkey.png", 300, 50, 1);
    invnote = gf3d_sprite_load("images/invnote.png", 300, 50, 1);
    invshoes = gf3d_sprite_load("images/invshoes.png", 300, 50, 1);
    invspiderweb = gf3d_sprite_load("images/invspiderweb.png", 300, 50, 1);
    invdollar = gf3d_sprite_load("images/invdollar.png", 300, 50, 1);
    invapple = gf3d_sprite_load("images/invapple.png", 300, 50, 1);
    invumbrella = gf3d_sprite_load("images/invumbrella.png", 300, 50, 1);
    invbooklet = gf3d_sprite_load("images/invbooklet.png", 300, 50, 1);
    invbox = gf3d_sprite_load("images/invbox.png", 300, 50, 1);
    invteleporter = gf3d_sprite_load("images/invteleporter.png", 300, 50, 1);

    Sprite* menu = gf3d_sprite_load("images/mainmenu_screen.png", 1200, 700, 1);

    Sprite* noteText = gf3d_sprite_load("images/note.png", 2500, 500, 1);

    Sprite* dialogue[7];
        dialogue[0] = gf3d_sprite_load("images/dialogue/0.png", 2500, 500, 1);
        dialogue[1] = gf3d_sprite_load("images/dialogue/1.png", 2500, 500, 1);
        dialogue[2] = gf3d_sprite_load("images/dialogue/2.png", 2500, 500, 1);
        dialogue[3] = gf3d_sprite_load("images/dialogue/3.png", 2500, 500, 1);
        dialogue[4] = gf3d_sprite_load("images/dialogue/4.png", 2500, 500, 1);
        dialogue[5] = gf3d_sprite_load("images/dialogue/5.png", 2500, 500, 1);
        dialogue[6] = gf3d_sprite_load("images/dialogue/6.png", 2500, 500, 1);

    w = world_load("config/testworld.json");

    Entity* wall1 = item_new(vector3d(0, 1000, 0));
    wall1->scale = vector3d(1000, 1, 1000);
    Entity* wall2 = item_new(vector3d(0, -1000, 0));
    wall2->rotation.z = GFC_PI;
    wall2->scale = vector3d(1000, 1, 1000);
    Entity* wall3 = item_new(vector3d(1000, 0, 0));
    wall3->rotation.z = -GFC_HALF_PI;
    wall3->scale = vector3d(1, 1000, 1000);
    Entity* wall4 = item_new(vector3d(-1000, 0, 0));
    wall4->rotation.z = GFC_HALF_PI;
    wall4->scale = vector3d(1, 1000, 1000);
    Entity* floor = item_new(vector3d(0, 0, 0));
    floor->scale = vector3d(1000, 1000, 1);

    
    float interactDist = 50;
    
    Entity* NPC1 = agumon_new(vector3d(0, -850, 60));
    Entity* NPC2 = agumon_new(vector3d(-200, 900, 60));
    Entity* NPC3 = agumon_new(vector3d(850, 700, 60));
    Entity* NPC4 = agumon_new(vector3d(-950, -200, 60));
    Entity* NPC5 = agumon_new(vector3d(850, -600, 60));

    NPC1->rotation.z = M_PI;
    NPC1->scale = vector3d(10, 10, 10);
    NPC2->scale = vector3d(10, 10, 10);
    NPC3->rotation.z = M_PI * 1.5;
    NPC3->scale = vector3d(10, 10, 10);
    NPC4->rotation.z = M_PI / 2;
    NPC4->scale = vector3d(10, 10, 10);
    NPC5->rotation.z = M_PI * 1.5;
    NPC5->scale = vector3d(10, 10, 10);

    Entity* shoes = item_new(vector3d(50, -75, -10));
    Entity* note = item_new(vector3d(-50, 75, -10));
    Entity* umbrella = item_new(vector3d(-50, -75, -10));
    Entity* spiderwebs = item_new(vector3d(50, 75, -10));

    slog_sync();
    gf3d_camera_set_scale(vector3d(1,1,1));
    Entity* player = player_new(vector3d(0,0,60));
    
    // main game loop
    slog("gf3d main loop begin");
    while(!done)
    {
        gfc_input_update();
        SDL_GetMouseState(&mousex,&mousey);
        
        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;
        entity_think_all();
        entity_update_all();
        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        gf3d_vgraphics_render_start();

            //3D draws
                world_draw(w);
                entity_draw_all();
            //2D draws
                gf3d_sprite_draw(mouse,vector2d(mousex,mousey),vector2d(2,2),(Uint32)mouseFrame);

                gf3d_sprite_draw(menu, vector2d(0,700), vector2d(2, 2), (Uint32)mouseFrame);

                gf3d_sprite_draw(inv, vector2d(0,0), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[0])gf3d_sprite_draw(invkey, vector2d(0, 25), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[1])gf3d_sprite_draw(invnote, vector2d(0, 50), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[2])gf3d_sprite_draw(invshoes, vector2d(0, 75), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[3])gf3d_sprite_draw(invspiderweb, vector2d(0, 100), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[4])gf3d_sprite_draw(invdollar, vector2d(0, 125), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[5])gf3d_sprite_draw(invapple, vector2d(0, 150), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[6])gf3d_sprite_draw(invumbrella, vector2d(0, 175), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[7])gf3d_sprite_draw(invbooklet, vector2d(0, 200), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[8])gf3d_sprite_draw(invbox, vector2d(0, 225), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->itemOwned[9])gf3d_sprite_draw(invteleporter, vector2d(0, 250), vector2d(2, 2), (Uint32)mouseFrame);

                if (player->interacting == 1 && player->boxGiven && player->nearNPC == 4) 
                    gf3d_sprite_draw(dialogue[0], vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->interacting == 1 && player->keyGiven && player->nearNPC == 4)
                    gf3d_sprite_draw(dialogue[1], vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->interacting == 1 && player->nearNPC == 2 && !player->itemOwned[5])
                    gf3d_sprite_draw(dialogue[2], vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->interacting == 1 && player->nearNPC == 3 && player->itemOwned[5])
                    gf3d_sprite_draw(dialogue[3], vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->interacting == 1 && player->nearNPC == 1 && player->itemOwned[8])
                    gf3d_sprite_draw(dialogue[4], vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->interacting == 1 && player->nearNPC == 1 && player->itemOwned[0])
                    gf3d_sprite_draw(dialogue[5], vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);
                if (player->interacting && player->nearNPC == 5 && player->itemOwned[7])
                    gf3d_sprite_draw(dialogue[6], vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);
                
                if (player->readingNote && !player->interacting && player->itemOwned[1])
                    gf3d_sprite_draw(noteText, vector2d(0, 500), vector2d(2, 2), (Uint32)mouseFrame);

        gf3d_vgraphics_render_end();

        if (vector3d_distance_between_less_than(player->position, NPC1->position, interactDist))
            player->nearNPC = 1;
        else if (vector3d_distance_between_less_than(player->position, NPC2->position, interactDist))
            player->nearNPC = 2;
        else if (vector3d_distance_between_less_than(player->position, NPC3->position, interactDist))
            player->nearNPC = 3;
        else if (vector3d_distance_between_less_than(player->position, NPC4->position, interactDist))
            player->nearNPC = 4;
        else if (vector3d_distance_between_less_than(player->position, NPC5->position, interactDist))
            player->nearNPC = 5;
        else
            player->nearNPC = 0;

        if (vector3d_distance_between_less_than(player->position, shoes->position, interactDist))
            player->itemOwned[2] = 1;
        if (vector3d_distance_between_less_than(player->position, note->position, interactDist))
            player->itemOwned[1] = 1;
        if (vector3d_distance_between_less_than(player->position, umbrella->position, interactDist))
            player->itemOwned[6] = 1;
        if (vector3d_distance_between_less_than(player->position, spiderwebs->position, interactDist))
            player->itemOwned[3] = 1;
        


        if (gfc_input_command_down("exit"))done = 1; // exit condition
    }    
    
    world_delete(w);
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
