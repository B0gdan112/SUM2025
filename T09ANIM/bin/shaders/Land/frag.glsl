/* Fragment shader by BS7 */
#version 460
out vec4 OutColor;
 
uniform float Time;
uniform vec3 CamLoc;
uniform bool IsTexture0;  
uniform vec3 Ka, Kd, Ks;
uniform float Ph;

layout(binding = 0) uniform sampler2D Tex;

in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

vec3 Shade( vec3 P, vec3 N )
{
  vec3 LPos = vec3(12, 12, 12);
  vec3 L = normalize(vec3(2, 5, 4));
  vec3 LC = vec3(1, 1, 1); // light color
  vec3 V = normalize(P - CamLoc);
 
  N = faceforward(N, V, N);
  //N = -N;
 
  vec3 color = vec3(0);
 
  // Ambient
  color += Ka;
 
  // Diffuse
  vec3 Diff = Kd;
  if (IsTexture0)
  {
    vec4 tc = texture(Tex, DrawTexCoord);
    Diff = tc.rgb;
  }
  color += LC * Diff * max(0, dot(N, L));
 
  // Specular
  vec3 R = reflect(V, N);
  color += LC * Ks * pow(max(0, dot(R, L)), Ph);
 
  return color;
}
 
void main( void )
{
  OutColor = vec4(Shade(DrawPos, normalize(DrawNormal)), 1);
}