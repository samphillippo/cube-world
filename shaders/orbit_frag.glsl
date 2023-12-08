// ====================================================
#version 330 core

uniform vec3 orbitColor;
uniform sampler2D u_DiffuseMap;

// ======================= IN =========================
in vec3 myNormal; // Import our normal data
in vec2 v_texCoord; // Import our texture coordinates from vertex shader
in vec3 FragPos; // Import the fragment position

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;


void main()
{
    if (orbitColor.x == 0.0 && orbitColor.y == 0.0 && orbitColor.z == 0.0) {
        FragColor = texture(u_DiffuseMap, v_texCoord) * 2.0;
    } else {
        FragColor = (texture(u_DiffuseMap, v_texCoord) + vec4(0.1, 0.1, 0.1, 0.1))* 7.0;
    }
}
// ==================================================================
