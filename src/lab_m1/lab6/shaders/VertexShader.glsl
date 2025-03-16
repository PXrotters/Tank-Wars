#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec3 frag_color;

// Sin and Cos functions
float customSin(float t, float frequency, float amplitude, float phase) {
    return amplitude * sin(frequency * t + phase);
}

float customCos(float t, float frequency, float amplitude, float phase) {
    return amplitude * cos(frequency * t + phase);
}

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position	= v_position;
	frag_normal = v_normal + vec3(customSin(Time, 1.0, 0.5, 0.0), customCos(Time, 1.5, 0.5, 3.14 / 4.0), 1.0);
	frag_texture	= v_texture;
	frag_color		= v_color;

    // TODO(student): Compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position + vec3(customSin(Time, 2.0, 0.2, 0.0), 0.0, customCos(Time, 1.2, 0.2, 3.14 / 2.0)), 1.0);

}
