/*******************************************************************************************
 *
 *   shaderbox_demo - Demo of the shaderbox sandbox tool for learning and experimenting with shaders.
 *
 *   This example has been created using raylib 3.0 (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h
 *   for details)
 *
 *   Copyright (c) 2020 Chris Dill
 *
 ********************************************************************************************/

#include "raylib.h"
#include <stdio.h>
#include <assert.h>

#define SHADERBOX_IMPLEMENTATION
#include "shaderbox.h"

int main(int argc, char **argv)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Shaderbox");

    Shaderbox shaderbox = LoadShaderbox("resources/shaders/bookofshaders/bookofshaders1.fs");

    bool debug = false;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_F3))
        {
            debug = !debug;
        }

        // Load shader from dropped file
        if (IsFileDropped())
        {
            int count = 0;
            char** files = GetDroppedFiles(&count);

            if (count > 0)
            {
                TraceLog(LOG_INFO, "Reloading shader.");
                shaderbox = LoadShaderbox(files[0]);
            }
            ClearDroppedFiles();
        }

        UpdateShaderbox(&shaderbox);
        
        if (ReloadShaderbox(&shaderbox))
        {
            TraceLog(LOG_INFO, "Reloaded shaderbox.");
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginShaderMode(shaderbox.shader);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
        EndShaderMode();

        if (debug)
        {
            DrawFPS(10, 10);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShaderbox(shaderbox);
    
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
