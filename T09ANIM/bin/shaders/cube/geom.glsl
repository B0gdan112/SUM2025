 /* Fragment shader by BS7 */
#version 460
out vec4 OutColor;
 
uniform float Time;
                                          
layout(points) in;
layout(triangle_strip, max_vertices = 3) out;
 
out vec2 FDrawTexCoord;

void main( void )
{  
  gl_Position = vec4(-1, 3, 0, 1);
  EmitVertex();
  gl_Position = vec4(-1, -1, 0, 1);
  EmitVertex();
  gl_Position = vec4(3, -1, 0, 1);
  EmitVertex();
  EndPrimitive();
}