// Matrices
uniform mat4 B_matrix_model;
uniform mat4 B_matrix_model_inv;
uniform mat4 B_matrix_normal;
uniform mat4 B_matrix_normal_inv;
uniform mat4 B_matrix_view;
uniform mat4 B_matrix_view_inv;
uniform mat4 B_matrix_projection;
uniform mat4 B_matrix_projection_inv;
uniform mat4 B_matrix_pvm;
uniform mat4 B_matrix_pvm_inv;

// Camera
uniform vec3 B_position_camera;

// Editor
uniform bool B_gameObject_isSelected;

// Stencil
uniform bool B_stencilWriteEnabled;
uniform bool B_stencilTestEnabled;

// Material related
uniform bool B_renderer_receivesLighting;
uniform vec4  B_material_diffuse_color;
uniform float B_alphaCuttoff = -1.0f;
uniform float B_material_shininess;
uniform bool B_hasTexture = false;
uniform sampler2D B_texture_0;

// Screen related
uniform vec2 B_screen_size;
#ifdef BANG_FRAGMENT
vec2 B_screen_coord = gl_FragCoord.xy;
vec2 B_screen_coord_norm = B_screen_coord / B_screen_size;
vec2 B_pixel_step = 1.0f / B_screen_size;
#endif

// GBuffer textures
uniform sampler2D B_position_gout_fin;
uniform sampler2D B_normal_gout_fin;
uniform sampler2D B_uv_gout_fin;
uniform sampler2D B_diffuse_gout_fin;
uniform sampler2D B_materialProps_gout_fin;
uniform sampler2D B_depth_gout_fin;
uniform sampler2D B_stencil_gout_fin;
uniform sampler2D B_color_gout_fin;

// Light related
uniform float B_light_range;
uniform vec4  B_light_color;
uniform float B_light_intensity;
uniform vec3  B_light_forward_world;
uniform vec3  B_light_position_world;
