/**** Vertex shader ****/
#pragma wtsngine vertex
#version 330 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_uv;
layout(location=2) in vec3 in_normal;

uniform mat4 u_model;

uniform Uniforms
	{
	mat4 view;
	mat4 projection;
	} u;
	
uniform Material
	{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specularexp;
	float transparency;
	} m;
	
out VertexData
	{
	vec2 uv;
	vec3 normal;
	} o;

void main()
	{
	gl_Position=u.projection*u.view*u_model*vec4(in_position, 1.0f);
	
	o.uv=in_uv;
	o.normal=in_normal;
	}

/**** Fragment shader ****/
#pragma wtsngine fragment
#version 330 core

in VertexData
	{
	vec2 uv;
	vec3 normal;
	} i;
	
uniform Material
	{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specularexp;
	float transparency;
	} m;
	
uniform sampler2D u_texture;
	
out vec4 out_color;

void main()
	{
	out_color=vec4(m.diffuse, 1.0);//texture(u_texture, i.uv);
	
	if(out_color.a<1.0f/256.f)
		discard;
	
	gl_FragDepth=gl_FragCoord.z;
	}
