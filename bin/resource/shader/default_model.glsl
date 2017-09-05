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
	vec3 position;
	vec3 normal;
	} o;

void main()
	{
	gl_Position=u.projection*u.view*u_model*vec4(in_position, 1.0f);
	
	o.uv=in_uv;
	o.position=vec3(u.view*u_model*vec4(in_position, 1.0f));
	o.normal=vec3(normalize(transpose((u.view*u_model))*vec4(in_normal, 0.0f)));
	}

/**** Fragment shader ****/
#pragma wtsngine fragment
#version 330 core

in VertexData
	{
	vec2 uv;
	vec3 position;
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
	vec3 LPOS=vec3(-128, -128, 128);
	vec3 L=normalize(LPOS-i.position);//vec3(2.0f, 1.0f, -0.5f));
	vec3 E=normalize(-i.position);
	vec3 R=normalize(-reflect(L, i.normal));
	
	// Ambient
	vec4 Iamb=vec4(0.1f, 0.1f, 0.1f, 0.1f);
	// Diffuse
	vec4 Idiff=clamp(vec4(0.0f, 1.0f, 0.0f, 0.5f)*max(dot(i.normal, L), 0.0f), 0.0f, 1.0f);
	// Specular
	vec4 Ispec=clamp(vec4(0.0f, 0.0f, 1.0f, 0.5f)*pow(max(dot(R, E), 0.0f), 0.3f/*shininess*/), 0.0f, 1.0f);
	
	out_color=vec4(m.diffuse, 1.0f)+Iamb+Idiff+Ispec;//texture(u_texture, i.uv);

	//out_color.r=0.0f;
	
	//if(out_color.a<1.0f/256.f)
	//	discard;
	
	gl_FragDepth=gl_FragCoord.z;
	}
