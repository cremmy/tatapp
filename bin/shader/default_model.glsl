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
	} o;

void main()
	{
	gl_Position=u.projection*u.view*u_model*vec4(in_position, 1.0f);
	
	o.position=(u.view*u_model*vec4(in_position, 1.0f)).xyz;
	o.uv=in_uv;
	o.normal=normalize((u_model*vec4(in_normal, 0.0f)).xyz);
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
	} i;
	
out vec4 out_color;

// http://shdr.bkcore.com/
vec2 blinnPhongDir(vec3 lightDir, float lightInt, float Ka, float Kd, float Ks, float shininess)
	{
	vec3 s = normalize(lightDir);
	vec3 v = normalize(-i.position);
	vec3 n = normalize(i.normal);
	vec3 h = normalize(v+s);
	float diffuse = Ka + Kd * lightInt * max(0.0, dot(n, s));
	float spec =  Ks * pow(max(0.0, dot(n,h)), shininess);
	return vec2(diffuse, spec);
	}

void main()
	{
	out_color=/*texture(u_texture, i.uv)**/vec4(m.diffuse, 1.0f)*u_color;
	//out_color+=vec4(m.ambient, 0);
	//out_color+=vec4(l.ambient, 0);
	out_color.rgb*=max(dot(vec4(l.direction, 0), vec4(i.normal, 0)), 0.2f);
	vec2 bp=blinnPhongDir(
		l.direction,
		1.0,
		1.0,
		1.0,
		1.0,
		1.0
		);
	out_color*=bp.x*bp.y;
	out_color.a=1;
	
	//if(out_color.a<1.0f/256.f)
	//	discard;
	
	gl_FragDepth=gl_FragCoord.z;
	}
