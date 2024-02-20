#version 450

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec3 interpolatedNormal;

layout(location = 0) out vec4 outColor;

void main(){
	vec3 normalizedNormal = normalize(interpolatedNormal);
	vec3 lightDir = vec3(0.0, 0.0, 1.0);

	float halfLambert = dot(normalizedNormal, lightDir) * 0.5 + 0.5;

	vec3 color = halfLambert * fragColor.rgb;

	outColor = vec4(color, 1.0f);
}