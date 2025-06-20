/* Fragment shader by BS7 */
#version 460
layout(location = 0) out vec4 OutColor;
layout(location = 1) out vec4 OutNormal;
layout(location = 2) out vec4 OutFinal;
 
 
in vec4 DrawColor;
 
void main( void )
{
  OutColor = DrawColor;
  OutNormal = vec4(0);
  OutFinal = vec4(0);
}