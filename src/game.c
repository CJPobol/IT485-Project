#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

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

    
    w = world_load("config/testworld.json");
    
    float interactDist = 15;
    
    Entity* NPC1 = agumon_new(vector3d(0, -85, 0));
    Entity* NPC2 = agumon_new(vector3d(-20, 90, 0));
    Entity* NPC3 = agumon_new(vector3d(85, 70, 0));
    Entity* NPC4 = agumon_new(vector3d(-95, -20, 0));
    Entity* NPC5 = agumon_new(vector3d(85, -60, 0));

    NPC1->rotation.z = M_PI;
    NPC3->rotation.z = M_PI * 1.5;
    NPC4->rotation.z = M_PI / 2;
    NPC5->rotation.z = M_PI * 1.5;


    slog_sync();
    gf3d_camera_set_scale(vector3d(1,1,1));
    Entity* player = player_new(vector3d(0,0,20));
    
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
