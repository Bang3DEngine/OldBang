#ifndef POSTPROCESSEFFECTSSAO_H
#define POSTPROCESSEFFECTSSAO_H

#include "Bang/Bang.h"
#include "Bang/ShaderProgram.h"
#include "Bang/ResourceHandle.h"
#include "Bang/PostProcessEffect.h"

NAMESPACE_BANG_BEGIN

FORWARD class Framebuffer;

class PostProcessEffectSSAO : public PostProcessEffect
{
    COMPONENT(PostProcessEffectSSAO);

public:
    // Component
    virtual void OnRender(RenderPass renderPass) override;

    void SetSSAORadius(float radius);
    void SetBlurRadius(int blurRadius);
    void SetNumRandomSamples(int numRandomSamples);
    void SetNumRandomRotations(int numRotations);
    void SetFBSize(const Vector2 &fbSize);

    int GetBlurRadius() const;
    float GetSSAORadius() const;
    int GetNumRandomSamples() const;
    int GetNumRandomRotations() const;
    const Vector2 &GetFBSize() const;
    Texture2D* GetSSAOTexture() const;

private:
    float m_ssaoRadius = -1;
    int m_blurRadius = -1;
    int m_numRotations = -1;
    int m_numRandomOffsetsHemisphere = -1;
    Vector2 m_fbSize = Vector2::One;

    Array<Vector3> m_randomHemisphereOffsets;
    RH<Texture2D> m_randomRotationsTexture;

    Framebuffer *m_ssaoFB = nullptr;
    RH<ShaderProgram> p_ssaoShaderProgram;
    RH<ShaderProgram> p_blurShaderProgram;
    RH<ShaderProgram> p_applySSAOShaderProgram;

    void GenerateRandomRotationsTexture(int numRotations);
    static Array<Vector3> GenerateRandomsArray(int numRandoms,
                                               const Vector2 &xRange,
                                               const Vector2 &yRange,
                                               const Vector2 &zRange,
                                               bool normalize);
    static RH<Texture2D> GenerateRandomsTexture(int numRandoms,
                                                const Vector2 &xRange,
                                                const Vector2 &yRange,
                                                const Vector2 &zRange);

	PostProcessEffectSSAO();
	virtual ~PostProcessEffectSSAO();
};

NAMESPACE_BANG_END

#endif // POSTPROCESSEFFECTSSAO_H

