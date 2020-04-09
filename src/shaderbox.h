/**********************************************************************************************
 *
 *   shaderbox - A sandbox tool for learning and experimenting with shaders.
 *
 *   Copyright (c) 2020 Chris Dill
 *
 **********************************************************************************************/

#ifndef SHADERBOX_H
#define SHADERBOX_H

#include "raylib.h"
#include <string.h>

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

#define SHADERBOX_VERSION 0.1
#define MAX_FILENAME_LENGTH 512

typedef struct Shaderbox
{
    Shader shader;
    char fileName[MAX_FILENAME_LENGTH];
    long modTime;
} Shaderbox;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

// Loads a shader in a shaderbox struct storing info for reloading it.
Shaderbox LoadShaderbox(const char* fileName);

// Unload internal shader resources.
void UnloadShaderbox(Shaderbox shaderbox);

// Cache uniforms for the shader type.
void CacheShaderboxUniforms(Shaderbox *shaderbox);

// Replace the existing shader if the new one is valid. Checks against the default shader id.
bool ReplaceShader(Shaderbox *shaderbox, Shader *shader);

// Update per frame uniform values such as delta time, framecounter. 
void UpdateShaderbox(Shaderbox *shaderbox);

// Reloads the current shader if the file is modified.
bool ReloadShaderbox(Shaderbox *shaderbox);

#endif // SHADERBOX_H


/***********************************************************************************
*
*   SHADERBOX IMPLEMENTATION
*
************************************************************************************/

#if defined(SHADERBOX_IMPLEMENTATION)

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------

Vector3 resolution = { 0 };
double time = 0.0;
double timeDelta = 0.0;
int framecounter = 0;
Vector2 mousePosition = { 0 };

static int resolutionLoc = -1;
static int timeLoc = -1;
static int timeDeltaLoc = -1;
static int frameLoc = -1;
static int mouseLoc = -1;
static int dateLoc = -1;
static int sampleRate = -1;

// Loads a shader in a shaderbox struct storing info for reloading it.
Shaderbox LoadShaderbox(const char* fileName)
{
    Shaderbox shaderbox = { 0 };

    // General checks.
    if (TextLength(fileName) > MAX_FILENAME_LENGTH)
    {
        TraceLog(LOG_ERROR, "Filename greater than MAX_FILENAME_LENGTH %i, MAX_FILENAME_LENGTH");
        return shaderbox;
    }

    // Make sure shader file exists.
    if (!FileExists(fileName))
    {
        TraceLog(LOG_ERROR, "Failed to load %s. File does not exist");
        return shaderbox;
    }

    TraceLog(LOG_INFO, "Loading shaderbox %.1f %s", SHADERBOX_VERSION, shaderbox.fileName);

    // Store shader file info.
    TextCopy(shaderbox.fileName, fileName);
    shaderbox.modTime = GetFileModTime(fileName);

    // Store the shader if it loads successfully.
    Shader shader = LoadShader(0, fileName);

    if (ReplaceShader(&shaderbox, &shader))
    {
        CacheShaderboxUniforms(&shaderbox);
    }

    return shaderbox;
}

// Unload internal shader resources.
void UnloadShaderbox(Shaderbox shaderbox)
{
    UnloadShader(shaderbox.shader);
}

// Cache uniforms for the shader type.
void CacheShaderboxUniforms(Shaderbox *shaderbox)
{
    /*
    @TODO determine what the current shader is using.
    Need a way to know if it is a glslSandbox shader, a shadertoy shader etc.
    The filename does not store that info. Maybe check the path and if the shadertoy folder is found.
    Alternatively a config file listing the shaders you want before hand and only allow those?
    Or just allow users to switch between the modes even if it breaks the shader if it is not the same one.
    */

    /*resolutionLoc = GetShaderLocation(shaderbox->shader, "iResolution");
    timeLoc = GetShaderLocation(shaderbox->shader, "iTime");
    timeDeltaLoc = GetShaderLocation(shaderbox->shader, "iTimeDelta");
    timeDeltaLoc = GetShaderLocation(shaderbox->shader, "iFrame");
    mouseLoc = GetShaderLocation(shaderbox->shader, "iMouse");*/

    /*
    resolutionLoc = GetShaderLocation(shaderbox->shader, "iResolution");
    timeLoc = GetShaderLocation(shaderbox->shader, "iTime");
    timeDeltaLoc = GetShaderLocation(shaderbox->shader, "iTimeDelta");
    timeDeltaLoc = GetShaderLocation(shaderbox->shader, "iFrame");
    timeLoc = GetShaderLocation(shaderbox->shader, "u_time");
    mouseLoc = GetShaderLocation(shaderbox->shader, "u_mouse");
    */

    resolutionLoc = GetShaderLocation(shaderbox->shader, "u_resolution");
    timeLoc = GetShaderLocation(shaderbox->shader, "u_time");
    // timeDeltaLoc = GetShaderLocation(shaderbox->shader, "u_timeDelta");
    // timeDeltaLoc = GetShaderLocation(shaderbox->shader, "iFrame");
    mouseLoc = GetShaderLocation(shaderbox->shader, "u_mouse");
}

// Replace the existing shader if the new one is valid. Checks against the default shader id.
bool ReplaceShader(Shaderbox *shaderbox, Shader *shader)
{
    if (shader->id != GetShaderDefault().id)
    {
        if (shaderbox->shader.id != 0)
        {
            UnloadShader(shaderbox->shader);
        }
        shaderbox->shader = *shader;

        return true;
    }
    return false;
}

// Update per frame uniform values such as delta time, framecounter. 
void UpdateShaderbox(Shaderbox *shaderbox)
{
    // Update values
    float foo = GetTime();

    resolution.x = GetScreenWidth();
    resolution.y = GetScreenHeight();

    mousePosition = GetMousePosition();
    mousePosition.x /= resolution.x;
    mousePosition.y /= resolution.y;

    // Send valid uniforms to the shader
    if (timeLoc > -1)
    {
        SetShaderValue(shaderbox->shader, timeLoc, &foo, UNIFORM_FLOAT);
    }

    if (resolutionLoc > -1)
    {
        SetShaderValue(shaderbox->shader, resolutionLoc, &resolution, UNIFORM_VEC2);
    }

    if (mouseLoc > -1)
    {
        SetShaderValue(shaderbox->shader, mouseLoc, &mousePosition, UNIFORM_VEC2);
    }
}

// Reloads the current shader if the file is modified.
bool ReloadShaderbox(Shaderbox *shaderbox)
{
    // Reload if file has been modified
    long newModTime = GetFileModTime(shaderbox->fileName);

    if (newModTime > shaderbox->modTime)
    {
        shaderbox->modTime = newModTime;

        TraceLog(LOG_INFO, "Reloading shaderbox");

        // Replace existing shader if it is valid.
        Shader shader = LoadShader(0, shaderbox->fileName);

        if (ReplaceShader(shaderbox, &shader))
        {
            return true;
        }
    }
    return false;
}

#endif // SHADERBOX_IMPLEMENTATION
