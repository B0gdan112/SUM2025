/* Vertex shader by BS7 */
#version 460
 
layout(location = 0) in vec3 InPosition;

out float VertexId;
 
void main( void )
{
  VertexId = gl_VertexID;
  gl_Position = vec4(InPosition, 1);
}
