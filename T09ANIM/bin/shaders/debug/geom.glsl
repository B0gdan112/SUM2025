 /* Fragment shader by BS7 */
#version 460
layout(points) in;
layout(line_strip, max_vertices = 100) out;
 
uniform vec3 LightDir;
uniform mat4 MatrShadow, MatrWVP;
 
out vec4 DrawColor;
 
void main( void )
{
  // X axis
  DrawColor = vec4(1, 0, 0, 1);
  gl_Position = MatrWVP * vec4(0, 0, 0, 1);
  EmitVertex();
  gl_Position = MatrWVP * vec4(1111, 0, 0, 0);
  EmitVertex();
  EndPrimitive();
 
  // Y axis
  DrawColor = vec4(0, 1, 0, 1);
  gl_Position = MatrWVP * vec4(0, 0, 0, 1);
  EmitVertex();
  gl_Position = MatrWVP * vec4(0, 1, 0, 0);
  EmitVertex();
  EndPrimitive();
 
  // Z axis
  DrawColor = vec4(0, 0, 1, 1);
  gl_Position = MatrWVP * vec4(0, 0, 0, 1);
  EmitVertex();
  gl_Position = MatrWVP * vec4(0, 0, 1, 0);
  EmitVertex();
  EndPrimitive();
 
  // Light direction
  DrawColor = vec4(1, 1, 1, 1);
  gl_Position = MatrWVP * vec4(LightPos, 1);
  EmitVertex();
  gl_Position = MatrWVP * vec4(LightPos + LightDir * 102, 1);
  EmitVertex();
  EndPrimitive();
 
  // Light frustum
  vec3 p[8] =
  {
    vec3(-1, -1, -1),
    vec3(-1, -1,  1),
    vec3( 1, -1,  1),
    vec3( 1, -1, -1),
 
    vec3(-1,  1, -1),
    vec3(-1,  1,  1),
    vec3( 1,  1,  1),
    vec3( 1,  1, -1),
  };
  int e[] =
  {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
 
    4, 5,
    5, 6,
    6, 7,
    7, 4,
 
    0, 4,
    1, 5,
    2, 6,
    3, 7,
  };
 
  mat4 inm = inverse(MatrShadow);
  for (int i = 0; i < e.length(); i++)
  {
    gl_Position = MatrWVP * inm * vec4(p[e[i]], 1);
    EmitVertex();
    i++;
    gl_Position = MatrWVP * inm * vec4(p[e[i]], 1);
    EmitVertex();
    EndPrimitive();
  }
}