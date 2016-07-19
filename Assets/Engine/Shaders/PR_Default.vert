B_POST_RENDER_VS_DECLARE();

in vec3 B_position_raw_vin;

out vec2 B_uvRaw_vout_fin;

void main()
{
    B_POST_RENDER_VS_INIT_MAIN();

    B_uvRaw_vout_fin = (B_position_raw_vin + 0.5f).xy;
    gl_Position = vec4(B_position_raw_vin * 2.0f, 1);
}
