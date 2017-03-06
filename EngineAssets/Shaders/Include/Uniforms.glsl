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
uniform bool  B_material_receivesLighting;
uniform vec4  B_material_diffuse_color;
uniform float B_alphaCutoff = -1.0f;
uniform float B_material_shininess;
uniform bool  B_hasTexture = false;
uniform sampler2D B_texture_0;

// Screen related
uniform vec2 B_screen_size;
uniform vec2 B_buffer_size;

#ifdef BANG_FRAGMENT
vec2  B_screen_pixel_step = 1.0 / B_screen_size;
vec2  B_screen_coord = gl_FragCoord.xy;
vec2  B_screen_coord_norm = B_screen_coord / B_screen_size;

vec2  B_buffer_pixel_step = 1.0 / B_buffer_size;
vec2  B_buffer_coord = gl_FragCoord.xy;
vec2  B_buffer_coord_norm = B_buffer_coord / B_buffer_size;
#endif

// GBuffer textures
uniform sampler2D B_normal_gout_fin;
uniform sampler2D B_diffuse_gout_fin;
uniform sampler2D B_misc_gout_fin;
uniform sampler2D B_color_gout_fin;

// Getters
#ifdef BANG_SP
  #ifdef BANG_FRAGMENT
    #define FRAG_IN_UV_SCREEN()                    B_buffer_coord_norm
    #define FRAG_IN_COLOR()                        (texture2D(B_color_gout_fin,         B_buffer_coord_norm))
    #define FRAG_IN_POSITION_WORLD()               vec3(1,1,1) //(texture2D(B_position_gout_fin,      B_buffer_coord_norm).xyz)
    #define FRAG_IN_NORMAL_WORLD()      normalize(  texture2D(B_normal_gout_fin,        B_buffer_coord_norm).xyz )
    #define FRAG_IN_DIFFUSE_COLOR()                (texture2D(B_diffuse_gout_fin,       B_buffer_coord_norm))
    #define FRAG_IN_RECEIVES_LIGHTING()            (texture2D(B_misc_gout_fin, B_buffer_coord_norm).r > 0.5f)
    #define FRAG_IN_SHININESS()                    (texture2D(B_misc_gout_fin, B_buffer_coord_norm).g)
    #define FRAG_IN_DEPTH()                        (texture2D(B_misc_gout_fin, B_buffer_coord_norm).b)
    #define FRAG_IN_STENCIL()                      (texture2D(B_misc_gout_fin, B_buffer_coord_norm).a)

    vec3 B_ComputeWorldPosition(float depth)
    {
        float x = FRAG_IN_UV_SCREEN().x * 2.0 - 1.0;
        float y = FRAG_IN_UV_SCREEN().y * 2.0 - 1.0;
        float z = depth * 2.0 - 1.0;
        vec4 projectedPos = vec4(x, y, z, 1);
        vec4 worldPos = (B_matrix_projection_inv * projectedPos);
        worldPos = (B_matrix_view_inv * (worldPos/worldPos.w));
        return worldPos.xyz;
    }
  #endif
#endif

#ifdef BANG_G
    #ifdef BANG_VERTEX
        #define VERT_IN_POSITION_OBJECT() B_position_raw_vin
        #define VERT_IN_NORMAL_OBJECT()   B_normal_raw_vin
        #define VERT_IN_UV_OBJECT()       B_uv_raw_vin
    #endif
    #ifdef BANG_FRAGMENT
        #define FRAG_IN_POSITION_WORLD()     B_position_world_vout_fin.xyz
        #define FRAG_IN_NORMAL_WORLD()       B_normal_world_vout_fin
        #define FRAG_IN_UV()                 B_uv_vout_fin
        #define FRAG_IN_DIFFUSE_COLOR()      (texture2D(B_diffuse_gout_fin, B_buffer_coord_norm))
        #define FRAG_IN_DEPTH()              (texture2D(B_misc_gout_fin,    B_buffer_coord_norm).b)
        #define FRAG_IN_STENCIL()            (texture2D(B_misc_gout_fin,    B_buffer_coord_norm).a)
        #define FRAG_IN_COLOR()              (texture2D(B_color_gout_fin,   B_buffer_coord_norm))
    #endif
#endif

// Light related
uniform float B_light_range;
uniform vec4  B_light_color;
uniform float B_light_intensity;
uniform vec3  B_light_forward_world;
uniform vec3  B_light_position_world;
