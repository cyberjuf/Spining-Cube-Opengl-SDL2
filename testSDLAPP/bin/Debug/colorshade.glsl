#shader vertex
#version 330 core
layout(location =0) in vec3 position;
 					in vec3 VertColor;
out vec3 fragColor;
uniform mat4 tranMatrix;
uniform mat4 projection;
void main()
{
    fragColor = VertColor;
    gl_Position = vec4(position,1)*tranMatrix*projection;
    //gl_Position = vec4(position,1)*mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);      
}
#shader fragment
#version 330 core
layout(location =0 ) out vec4 color;
in vec3 fragColor;
void main()
{
    color = vec4(fragColor,1);
}
 
