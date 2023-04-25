#version 450 core

out vec4 fragcolor;

struct vertex_data{
    vec4 color;
};
in vertex_data gdata;

void main(){
    fragcolor = gdata.color;
}