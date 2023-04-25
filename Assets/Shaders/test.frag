#version 450 core

in vec4 v_color;
in vec2 v_tex_coord;

uniform sampler2D texture;

void main(){
    vec4 f_color = texelFetch(texture, ivec2(v_tex_coord),0) * v_color;
    if (f_color.a<0.1f) discard; 
    gl_FragColor = f_color;
}