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
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 cam_direction;
	} o;

void main()
	{
	gl_Position=u.projection*u.view*u_model*vec4(in_position, 1.0f);
	
	o.position=(u.view*u_model*vec4(in_position, 1.0f)).xyz;
	o.uv=in_uv;
	o.normal=normalize((u_model*vec4(in_normal, 0.0f)).xyz);
	o.cam_direction=-normalize(o.position);
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
	vec3 direction;
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
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 cam_direction;
	} i;
	
out vec4 out_color;

void main()
	{
	out_color=vec4(0, 0, 0, 1);
	// Ambient
	out_color.rgb+=m.ambient.rgb*m.diffuse.rgb;
	// Diffuse
	out_color.rgb+=m.diffuse*u_color.rgb*l.color.rgb*clamp(dot(l.direction.xyz, i.normal), -1.0, 1.0);
	// Specular
	vec3 spec_reflection=reflect(-l.direction.xyz, i.normal);
	float spec_cosAlpha=clamp(dot(i.cam_direction, spec_reflection), 0.0, 1.0);
	out_color.rgb+=m.specular*pow(spec_cosAlpha, m.specularexp);
	
	//if(out_color.a<1.0f/256.f)
	//	discard;
	
	gl_FragDepth=gl_FragCoord.z;
	}
