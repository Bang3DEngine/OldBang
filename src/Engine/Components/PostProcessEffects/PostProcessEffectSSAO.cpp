#include "Bang/PostProcessEffectSSAO.h"

#include "Bang/GL.h"
#include "Bang/Scene.h"
#include "Bang/AARect.h"
#include "Bang/Camera.h"
#include "Bang/GEngine.h"
#include "Bang/GBuffer.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"
#include "Bang/GLUniforms.h"
#include "Bang/Framebuffer.h"
#include "Bang/SceneManager.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ShaderProgramFactory.h"

USING_NAMESPACE_BANG

PostProcessEffectSSAO::PostProcessEffectSSAO()
{
    m_ssaoFB = new Framebuffer();
    m_ssaoFB->CreateAttachment(GL::Attachment::Color0,
                               GL::ColorFormat::RGB10_A2_UByte);

    SetBlurRadius(1);
    SetSSAORadius(1);
    SetNumRandomRotations(8);
    SetNumRandomSamples(16);

    m_ssaoFB->GetAttachmentTexture(GL::Attachment::Color0)->
              SetWrapMode(GL::WrapMode::Repeat);

    p_ssaoShaderProgram = ShaderProgramFactory::Get(
           ShaderProgramFactory::GetPostProcessVertexShaderPath(),
           EPATH("Shaders/SSAO.frag") );

    p_blurShaderProgram = ShaderProgramFactory::Get(
           ShaderProgramFactory::GetPostProcessVertexShaderPath(),
           EPATH("Shaders/SSAOBlur.frag") );

    p_applySSAOShaderProgram = ShaderProgramFactory::Get(
           ShaderProgramFactory::GetPostProcessVertexShaderPath(),
           EPATH("Shaders/SSAOApply.frag") );
}

PostProcessEffectSSAO::~PostProcessEffectSSAO()
{
    delete m_ssaoFB;
}

#include "Bang/Input.h"
void PostProcessEffectSSAO::OnRender(RenderPass renderPass)
{
    Component::OnRender(renderPass);

    if ( MustBeRendered(renderPass) )
    {
        // Save state
        GLId prevSP = GL::GetBoundId( GL::BindTarget::ShaderProgram );
        GLId prevFB = GL::GetBoundId( GL::BindTarget::Framebuffer );
        AARecti prevVP = GL::GetViewportRect();

        // Get references
        GBuffer *gbuffer = GEngine::GetActiveGBuffer();

        // Prepare state
        m_ssaoFB->Bind();
        m_ssaoFB->SetAllDrawBuffers();
        SetFBSize( Vector2(GL::GetViewportSize())  );
        GL::SetViewport(0, 0, GetFBSize().x, GetFBSize().y);

        // First do SSAO
        {
            p_ssaoShaderProgram.Get()->Bind(); // Bind shader

            // Bind
            gbuffer->BindAttachmentsForReading(p_ssaoShaderProgram.Get());

            // Bind random textures and set uniforms
            Vector2 randomRotsUvMult (m_randomRotationsTexture.Get()->GetSize() /
                                      GL::GetViewportSize());
            p_ssaoShaderProgram.Get()->Set("B_SSAORadius", GetSSAORadius());
            p_ssaoShaderProgram.Get()->Set("B_RandomRotationsUvMultiply",
                                           randomRotsUvMult);
            p_ssaoShaderProgram.Get()->Set("B_RandomRotations",
                                           m_randomRotationsTexture.Get());
            p_ssaoShaderProgram.Get()->Set("B_NumRandomOffsets",
                                           GetNumRandomSamples() );
            p_ssaoShaderProgram.Get()->Set("B_RandomHemisphereOffsetsArray",
                                           m_randomHemisphereOffsets);


            GEngine::GetActive()->RenderViewportPlane(); // Render Pass!
        }

        // Then blur
        {
            // Bind program, textures and set uniforms
            p_blurShaderProgram.Get()->Bind();
            gbuffer->BindAttachmentsForReading(p_blurShaderProgram.Get());
            p_blurShaderProgram.Get()->Set("B_BlurRadius", GetBlurRadius());
            p_blurShaderProgram.Get()->Set(GBuffer::GetColorsTexName(),
                                           GetSSAOTexture());
            GEngine::GetActive()->RenderViewportPlane(); // Render blur!
        }

        // Restore previous framebuffer
        GL::Bind( GL::BindTarget::Framebuffer, prevFB );

        // Then apply actual SSAO blending with occlusion calculated above!
        {
            p_applySSAOShaderProgram.Get()->Bind();
            p_applySSAOShaderProgram.Get()->Set("B_SSAOMap", GetSSAOTexture());
            gbuffer->ApplyPass(p_applySSAOShaderProgram.Get(), true);
        }

        // Restore state
        GL::Bind( GL::BindTarget::ShaderProgram, prevSP );
        GL::SetViewport(prevVP);
    }
}

void PostProcessEffectSSAO::SetSSAORadius(float radius)
{
    m_ssaoRadius = radius;
}

void PostProcessEffectSSAO::SetBlurRadius(int blurRadius)
{
    m_blurRadius = blurRadius;
}

void PostProcessEffectSSAO::SetNumRandomSamples(int numRandomSamples)
{
    if (numRandomSamples != GetNumRandomSamples())
    {
        m_numRandomOffsetsHemisphere = numRandomSamples;
        m_randomHemisphereOffsets = GenerateRandomsArray(numRandomSamples,
                                                         Vector2(-1.0f, 1.0f),
                                                         Vector2(-1.0f, 1.0f),
                                                         Vector2( 0.0f, 1.0f),
                                                         true);

        // Scale exponentially close to zero, so that there are more closer
        // samples (and consequently have greater weight)
        for (int i = 0; i < GetNumRandomSamples(); ++i)
        {
            float scale = i / SCAST<float>( GetNumRandomSamples() );
            scale = Math::Lerp(0.1f, 1.0f, scale * scale);
            m_randomHemisphereOffsets[i] *= scale;
            Debug_Log(m_randomHemisphereOffsets[i]);
        }
    }
}

void PostProcessEffectSSAO::SetNumRandomRotations(int numRotations)
{
    if (numRotations != GetNumRandomRotations())
    {
        m_numRotations = numRotations;
        GenerateRandomRotationsTexture( GetNumRandomRotations() );
    }
}

void PostProcessEffectSSAO::SetFBSize(const Vector2 &fbSize)
{
    m_fbSize = fbSize;
    m_ssaoFB->Resize(GetFBSize().x, GetFBSize().y);
}

int PostProcessEffectSSAO::GetBlurRadius() const
{
    return m_blurRadius;
}

float PostProcessEffectSSAO::GetSSAORadius() const
{
    return m_ssaoRadius;
}

int PostProcessEffectSSAO::GetNumRandomRotations() const
{
    return m_numRotations;
}

int PostProcessEffectSSAO::GetNumRandomSamples() const
{
    return m_numRandomOffsetsHemisphere;
}

const Vector2 &PostProcessEffectSSAO::GetFBSize() const
{
    return m_fbSize;
}

Texture2D* PostProcessEffectSSAO::GetSSAOTexture() const
{
    return m_ssaoFB->GetAttachmentTexture(GL::Attachment::Color0);
}

Array<Vector3> PostProcessEffectSSAO::GenerateRandomsArray(int numRandoms,
                                                           const Vector2 &xRange,
                                                           const Vector2 &yRange,
                                                           const Vector2 &zRange,
                                                           bool normalize)
{
    Array<Vector3> randomValues;

    for (int i = 0; i < numRandoms; ++i)
    {
        Vector3 randV (Random::GetRange(xRange[0], xRange[1]),
                       Random::GetRange(yRange[0], yRange[1]),
                       Random::GetRange(zRange[0], zRange[1]));
        if (normalize) { randV = randV.NormalizedSafe(); }
        randomValues.PushBack(randV);
    }

    return randomValues;
}

RH<Texture2D> PostProcessEffectSSAO::GenerateRandomsTexture(int numRandoms,
                                                            const Vector2 &xRange,
                                                            const Vector2 &yRange,
                                                            const Vector2 &zRange)
{
    ASSERT(xRange[0] >= 0.0f && xRange[1] <= 1.0f);
    ASSERT(yRange[0] >= 0.0f && yRange[1] <= 1.0f);
    ASSERT(zRange[0] >= 0.0f && zRange[1] <= 1.0f);

    // Create an image using random values in the specified ranges!
    const int imgSize = Math::Ceil( Math::Sqrt( float(numRandoms) ) );
    ASSERT(imgSize > 0);

    const Array<Vector3> randomValues =
        PostProcessEffectSSAO::GenerateRandomsArray(numRandoms,
                                                    xRange, yRange, zRange,
                                                    true);
    Imageb randomsImg;
    randomsImg.Create(imgSize, imgSize);
    for (int i = 0; i < numRandoms; ++i)
    {
        const int x = i % imgSize;
        const int y = i / imgSize;
        const Vector3 &randValue = randomValues[i]; // Get rand and set color
        randomsImg.SetPixel(x, y, Color(randValue, 1));
    }

    // Now create the texture from the image!
    RH<Texture2D> tex = Resources::Create<Texture2D>();
    tex.Get()->Resize(imgSize, imgSize);
    tex.Get()->Import(randomsImg);
    tex.Get()->SetWrapMode(GL::WrapMode::Repeat);
    tex.Get()->SetFilterMode(GL::FilterMode::Bilinear);
    return tex;
}

void PostProcessEffectSSAO::GenerateRandomRotationsTexture(int numRotations)
{
    // Remap x and y to (0,1) so that we can fit in color.
    // In shader we'll revert this mapping.
    m_randomRotationsTexture =
        PostProcessEffectSSAO::GenerateRandomsTexture(numRotations,
                                                      Vector2(0,1),
                                                      Vector2(0,1),
                                                      Vector2(0,0));
    m_randomRotationsTexture.Get()->ToImage().Export( Path("randomRotationsTexture.png") );
}
