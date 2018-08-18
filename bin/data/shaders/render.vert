#version 430

in vec4 position;

out float vMass;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(position.xyz, 1.0);

	vMass = position.w;
	gl_PointSize = clamp(position.w, 0, 2);
}