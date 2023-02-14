#version 460 core

out vec4 fragcolor;

in vec4 v_color;
in vec2 v_tex_coord;
in flat int v_texture;

uniform sampler2D m_texture;

uniform float width;
uniform float height;

void main(){
    fragcolor = texture(m_texture,v_tex_coord) * v_color;
    //fragcolor = texture(m_texture,v_tex_coord);
}