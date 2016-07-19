B_POST_RENDER_VS_DECLARE();

in vec3 B_position_raw_vin; // Plane positions
out vec2 B_uv_raw_vout_fin; // Uvs of the entire plane screen

void main()
{
    B_POST_RENDER_VS_INIT_MAIN();

    B_uv_raw_vout_fin = (B_position_raw_vin + 0.5f).xy; // Get uvs based on the planes coords
    gl_Position = vec4(B_position_raw_vin * 2.0f, 1)

    B_POST_RENDER_VS_END_MAIN();
}
