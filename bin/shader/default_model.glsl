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
	//o.position=(u.view*u_model*vec4(in_position, 1.0f)).xyz;
	o.normal=(u.view*u_model*vec4(in_normal, 0.0f)).xyz;
	}

/**** Fragment shader ****/
#pragma wtsngine fragment
#version 330 core
	
uniform vec4 u_color;
uniform sampler2D u_texture;
uniform sampler2D u_normal;

//uniform mat4 u_model;

uniform Uniforms
	{
	mat4 view;
	mat4 projection;
	} u;
	
uniform Light
	{
	vec3 ambient;
	vec3 position;
	vec3 color;
	} l;
	
uniform Material
	{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specularexp;
	float transparency;
	} m;
	
in VertexData
	{
	vec2 uv;
	vec3 normal;
	} i;
	
out vec4 out_color;

void main()
	{
	vec3 LDIR=normalize(u.view*vec4(l.position, 1)).xyz;
	out_color=texture(u_texture, i.uv)*vec4(m.diffuse, 1.0f)*u_color;//+vec4(m.ambient, 0.0f);
	out_color.rgb*=max(dot(LDIR, i.normal), 0.0f);
	out_color=vec4(i.normal, 1);
	//out_color.rgb*=max(dot(LDIR, texture(u_normal, i.uv).rgb), 0.0f);
	
	//out_color.a=1;
	
	//if(out_color.a<1.0f/256.f)
	//	discard;
	
	gl_FragDepth=gl_FragCoord.z;
	}
