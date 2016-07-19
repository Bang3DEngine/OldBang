B_POST_RENDER_FS_DECLARE();

void main()
{
    B_POST_RENDER_FS_INIT_MAIN();

    gl_FragColor = vec4(B_gVertex.diffuseColor.rgb, 1);
}
