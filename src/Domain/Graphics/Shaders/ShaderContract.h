#ifndef SHADERCONTRACT_H
#define SHADERCONTRACT_H

//TODO: Add check of all variables beginning with ShaderNames_Prefix, to see
//      if all of them have an existing name. Do this when loading shaders.

#include "String.h"

class ShaderContract
{
private:
    ShaderContract() {}

public:
    const static String \
    ShaderNames_Prefix,

    Attr_Vertex_In_Position_Raw,
    Attr_Vertex_In_Normal_Raw,
    Attr_Vertex_In_Uv_Raw,

    Attr_Vertex_Out_Fragment_In_Vertex_Position_Raw,
    Attr_Vertex_Out_Fragment_In_Vertex_Normal_Raw,
    Attr_Vertex_Out_Fragment_In_Vertex_Uv_Raw,

    Attr_Fragment_Out_GBuffer_In_Position,
    Attr_Fragment_Out_GBuffer_In_Normal,
    Attr_Fragment_Out_GBuffer_In_Uv,
    Attr_Fragment_Out_GBuffer_In_Diffuse,

    Uniform_Position_Camera,

    Uniform_Matrix_Model,
    Uniform_Matrix_Model_Inverse,
    Uniform_Matrix_Normal,
    Uniform_Matrix_Normal_Inverse,
    Uniform_Matrix_View,
    Uniform_Matrix_View_Inverse,
    Uniform_Matrix_Projection,
    Uniform_Matrix_Projection_Inverse,
    Uniform_Matrix_PVM,

    Uniform_Texture_Prefix,

    // When rendering to screen after Deferred Rendering passes.
    // This texture will hold the definitive color values of the rendered scene
    Uniform_Texture_Final_Color,
    Uniform_Material_Diffuse_Color
    ;
};

#endif // SHADERCONTRACT_H
