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

    Vector3 resolution;
    double time;
    double timeDelta;
    int framecounter;
    int sampleRate;
    Vector2 mousePosition;

    int resolutionLoc;
    int timeLoc;
    int timeDeltaLoc;
    int frameLoc;
    int mouseLoc;
    int dateLoc;
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
