#version 450

layout (lines) in;                              // now we can access 2 vertices
layout (triangle_strip, max_vertices = 4) out;  // always (for now) producing 2 triangles (so 4 vertices)

struct vertex_data{
    vec4 color;
};
in vertex_data vdata[];
out vertex_data gdata;

uniform float u_thickness = 0.003;
uniform float u_screen_aspect = 1;            // width/height   x/y 

void main(){
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;

    //vec2 dir    = normalize((p2.xy - p1.xy) * u_viewport_size);
    //vec2 offset = vec2(-dir.y, dir.x) * u_thickness / u_viewport_size;

    vec2 dir    = normalize((p2.xy - p1.xy) * vec2(u_screen_aspect,1));         
    vec2 offset = vec2(-dir.y, dir.x) * u_thickness / vec2(u_screen_aspect,1);

    gl_Position = p1 + vec4(offset.xy, 0.0, 0.0);
    gdata       = vdata[0];
    EmitVertex();
    
    gl_Position = p1 - vec4(offset.xy, 0.0, 0.0);
    gdata       = vdata[0];
    EmitVertex();
    
    gl_Position = p2 + vec4(offset.xy, 0.0, 0.0);
    gdata       = vdata[1];
    EmitVertex();

    gl_Position = p2 - vec4(offset.xy, 0.0, 0.0);
    gdata       = vdata[1];
    EmitVertex();

    EndPrimitive();
}