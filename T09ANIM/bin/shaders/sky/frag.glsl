/* Fragment shader by BS7 */
#version 460
out vec4 OutColor;
 
uniform float Time;
uniform vec3 CamLoc, CamDir, CamUp, CamRight;
uniform bool IsTexture0;  
uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform float ProjDist, ProjSize, FrameW, FrameH;

layout(binding = 0) uniform sampler2D Tex;

in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;
                                         
void main( void )
{
  float Wp, Hp;

  Wp = Hp = ProjSize;
  if (FrameW > FrameH)
    Wp *= FrameW / FrameH;
  else
    Hp *= FrameH / FrameW;

  
  float xp = (gl_FragCoord.x - FrameW / 2) * Wp / FrameW;
  float yp = (gl_FragCoord.y - FrameH / 2) * Hp / FrameH;

  vec3 D = normalize(CamDir * ProjDist + CamRight * xp + CamUp * yp);
  float theta = acos(-D.y) / acos(-1);
  float phi = atan(D.x, D.z) / (2 * acos(-1));

  vec4 tc = texture(Tex, vec2(phi, theta));
  OutColor = vec4(tc.rgb, 1);
  //OutColor = vec4(D.rgb, 1);
}