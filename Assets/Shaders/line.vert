#version 450 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec4 a_color;

struct vertex_data{
    vec4 color;
};
out vertex_data vdata;

uniform mat4 CamMatrix;

void main(){
    //gl_Position = proj * view * model * vec4(a_pos,1);
    
    gl_Position = CamMatrix * vec4(a_pos,1);
    //gl_Position = CamMatrix * transform *vec4(a_pos,1);
    //gl_Position = vec4(a_pos,1);
    
    vdata.color = a_color;
}