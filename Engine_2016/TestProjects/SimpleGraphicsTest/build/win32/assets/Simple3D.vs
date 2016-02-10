uniform mat4    g_matModelViewProj;

attribute vec4  g_vPositionOS;
attribute vec3  g_vNormalOS;

void main()
{
    vec4 vPositionCS = g_matModelViewProj * g_vPositionOS;

    // Pass everything off to the fragment shader
    gl_Position = vPositionCS;
}