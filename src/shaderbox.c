/**********************************************************************************************
 *
 *   shaderbox - A sandbox tool for learning and experimenting with shaders.
 *
 *   Copyright (c) 2020 Chris Dill
 *
 **********************************************************************************************/

#include "shaderbox.h"

// Loads a shader in a shaderbox struct storing info for reloading it.
Shaderbox LoadShaderbox(const char* fileName)
{
    Shaderbox shaderbox = { 0 };

    shaderbox.resolutionLoc = -1;
    shaderbox.timeLoc = -1;
    shaderbox.timeDeltaLoc = -1;
    shaderbox.frameLoc = -1;
    shaderbox.mouseLoc = -1;
    shaderbox.dateLoc = -1;
    shaderbox.sampleRate = -1;

    // General checks.
    if (TextLength(fileName) > MAX_FILENAME_LENGTH) {
        TraceLog(LOG_ERROR, "Filename greater than MAX_FILENAME_LENGTH %i, MAX_FILENAME_LENGTH");
        return shaderbox;
    }

    // Make sure shader file exists.
    if (!FileExists(fileName)) {
        TraceLog(LOG_ERROR, "Failed to load %s. File does not exist");
        return shaderbox;
    }

    TraceLog(LOG_INFO, "Loading shaderbox %.1f %s", SHADERBOX_VERSION, shaderbox.fileName);

    // Store shader file info.
    TextCopy(shaderbox.fileName, fileName);
    shaderbox.modTime = GetFileModTime(fileName);

    // Store the shader if it loads successfully.
    Shader shader = LoadShader(0, fileName);

    if (ReplaceShader(&shaderbox, &shader)) {
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
void CacheShaderboxUniforms(Shaderbox* shaderbox)
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

    shaderbox->resolutionLoc = GetShaderLocation(shaderbox->shader, "u_resolution");
    shaderbox->timeLoc = GetShaderLocation(shaderbox->shader, "u_time");
    // timeDeltaLoc = GetShaderLocation(shaderbox->shader, "u_timeDelta");
    // timeDeltaLoc = GetShaderLocation(shaderbox->shader, "iFrame");
    shaderbox->mouseLoc = GetShaderLocation(shaderbox->shader, "u_mouse");
}

// Replace the existing shader if the new one is valid. Checks against the default shader id.
bool ReplaceShader(Shaderbox* shaderbox, Shader* shader)
{
    if (shader->id != GetShaderDefault().id) {
        if (shaderbox->shader.id != 0) {
            UnloadShader(shaderbox->shader);
        }
        shaderbox->shader = *shader;

        return true;
    }
    return false;
}

// Update per frame uniform values such as delta time, framecounter.
void UpdateShaderbox(Shaderbox* shaderbox)
{
    // Update values
    float foo = GetTime();

    shaderbox->resolution.x = GetScreenWidth();
    shaderbox->resolution.y = GetScreenHeight();

    shaderbox->mousePosition = GetMousePosition();
    shaderbox->mousePosition.x /= shaderbox->resolution.x;
    shaderbox->mousePosition.y /= shaderbox->resolution.y;

    // Send valid uniforms to the shader
    if (shaderbox->timeLoc > -1) {
        SetShaderValue(shaderbox->shader, shaderbox->timeLoc, &foo, UNIFORM_FLOAT);
    }

    if (shaderbox->resolutionLoc > -1) {
        SetShaderValue(shaderbox->shader, shaderbox->resolutionLoc, &shaderbox->resolution, UNIFORM_VEC2);
    }

    if (shaderbox->mouseLoc > -1) {
        SetShaderValue(shaderbox->shader, shaderbox->mouseLoc, &shaderbox->mousePosition, UNIFORM_VEC2);
    }
}

// Reloads the current shader if the file is modified.
bool ReloadShaderbox(Shaderbox* shaderbox)
{
    // Reload if file has been modified
    long newModTime = GetFileModTime(shaderbox->fileName);

    if (newModTime > shaderbox->modTime) {
        shaderbox->modTime = newModTime;

        TraceLog(LOG_INFO, "Reloading shaderbox");

        // Replace existing shader if it is valid.
        Shader shader = LoadShader(0, shaderbox->fileName);

        if (ReplaceShader(shaderbox, &shader)) {
            return true;
        }
    }
    return false;
}
