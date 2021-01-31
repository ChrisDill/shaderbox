#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables
#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.1415926535897932384626433832795

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

const vec3 color = vec3(.0, .4, .7);
const float intensity = 1.0;

float band(vec2 pos, float amplitude, float frequency)
{
  float wave = amplitude * sin(2.0 * PI * frequency * pos.x + u_time) / 2.;
  float light = clamp(amplitude * frequency * 0.030, 0.001 + 0.001, 5.0) / abs(wave - pos.y + 0.5);
  return light * intensity;
}

void main()
{
  vec2 st = gl_FragCoord.xy / u_resolution.xy;
  st.x *= u_resolution.x / u_resolution.y;

  vec3 color = vec3(0.);
  color = vec3(st.x, st.y, abs(sin(u_time)));

  finalColor = vec4(color, 1.0);
  // finalColor = vec4(0.5, 1, 1, 1);
}
