/* Vertex shader by BS7 */
#version 460
 
uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform float Time, FrameW, FrameH;
 
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
out vec4 DrawColor;
out vec2 DrawTexCoord;
 
/* Orthographics projection matrix setup function.
 * ARGUMENTS:
 *   - orthographic box side facets coordinates:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
mat4 MatrOrtho( float Left, float Right, float Bottom, float Top, float Near, float Far )
{
  return mat4(vec4(2 / (Right - Left), 0, 0, 0),
              vec4(0, 2 / (Top - Bottom), 0, 0),
              vec4(0, 0, -2 / (Far - Near), 0),
              vec4(-(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1));
} /* End of 'MatrOrtho' function */
 
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;
  DrawTexCoord = InTexCoord;
}