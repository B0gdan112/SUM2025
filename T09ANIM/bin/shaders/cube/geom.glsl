 /* Fragment shader by BS7 */
#version 460
 
layout(points) in;
layout(triangle_strip, max_vertices = 7) out;
 
uniform mat4 MatrWVP;
uniform float Time;
uniform vec3 CamRight, CamUp;
 
out vec2 DrawTexCoord;
out vec4 DrawColor;
 
float Size, Angle;
 
in float VertexId[];
 
float L = 0.3, W = 0.30;
 
vec3 Get( float X, float Y )
{
  float a = Angle * acos(-1) / 180;
 
  DrawTexCoord = vec2(X, Y);
  X *= W;
  Y *= L;
  return (CamRight * (cos(a) * X - sin(a) * Y) +
            CamUp * (sin(a) * X + cos(a) * Y)) * Size;
}
 
void main( void )
{
  Size = 0.5;
  float
    x = int(VertexId[0]) % 32,
    z = int(VertexId[0]) / 32;
  float v = VertexId[0];
  vec3 p = vec3(x + 0.30 * sin(v * v), 0, z + 0.30 * sin(v * v)) / 7;
  
    //Angle = maxa * sin(Time * tt + ts);
  gl_Position = vec4(p + Get(-1, 0), 1);
  EmitVertex();
  gl_Position = vec4(p + Get(1, 0), 1);
  EmitVertex();
  gl_Position = vec4(p + Get(-1, 1), 1);
  EmitVertex();
  gl_Position = vec4(p + Get(1, 1), 1);
  EmitVertex();
 
  //Angle += maxa * sin(Time * tt + ts);
  gl_Position = vec4(p + Get(-1, 2), 1);
  EmitVertex();
  gl_Position = vec4(p + Get(1, 2), 1);
  EmitVertex();
 
  //Angle += maxa * sin(Time * tt + ts);
  gl_Position = vec4(p + Get(0, 3), 1);
  EmitVertex();
  EndPrimitive();
}