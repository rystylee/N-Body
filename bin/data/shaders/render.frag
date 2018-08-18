#version 430

in float vMass;

out vec4 fragColor;

void main()
{
	fragColor = vec4(clamp(vMass, 0, 1), 0.4, 1.0, 1);
	// fragColor = vec4(1.0, clamp(vMass, 0.5, 1.0), 0.3, 1);
}