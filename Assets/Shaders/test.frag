#version 460 core

out vec4 fragcolor;

in vec3 v_color;
in vec2 v_tex_coord;

uniform sampler2D m_texture;

uniform float width;
uniform float height;

void main(){
    //fragcolor = texture(m_texture,v_tex_coord) * vec4(v_color,1);
    fragcolor = texture(m_texture,v_tex_coord);
}