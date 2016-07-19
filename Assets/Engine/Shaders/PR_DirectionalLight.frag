B_POST_RENDER_FS_DECLARE();

void main()
{
    B_POST_RENDER_FS_INIT_MAIN();

    if(B_gVertex.receivesLighting > 0.5f)
    {
        float lightDot = dot(B_gVertex.normal, normalize(vec3(1, 1, 1)));
        gl_FragColor = vec4(B_gVertex.diffuseColor * lightDot, 1);
    }
    else
    {
        gl_FragColor = vec4(B_gVertex.diffuseColor, 1);
    }
}
