/* Fragment shader by BS7 */

#version 460
out vec4 OutColor;
 
in vec4 DrawColor;
in vec2 DrawTexCoord;
 
layout(binding = 0) uniform sampler2D Tex;
uniform bool IsTexture0;
 
void main( void )
{
  vec4 tc = texture(Tex, DrawTexCoord);
 
  if (tc.a < 0.30)
    discard;
  OutColor = vec4(tc.rgb, tc.a);
}
