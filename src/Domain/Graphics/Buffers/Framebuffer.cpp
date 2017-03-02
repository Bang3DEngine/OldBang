#include "Framebuffer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "GL.h"
#include "Math.h"
#include "Debug.h"
#include "Vector2.h"
#include "RenderTexture.h"

Framebuffer::Framebuffer(int width, int height) : m_width(width),
                                                  m_height(height),
                                                  m_depthAttachmentId(0)
{
    glGenFramebuffers(1, &m_idGL);
}

Framebuffer::~Framebuffer()
{
    //for (auto it : m_attachmentId_To_Texture)
    //{
        // RenderTexture *t = it.second;
        // delete t;
    //}

    if (m_depthAttachmentId != 0)
    {
        glDeleteRenderbuffers(1, &m_depthAttachmentId);
    }

    glDeleteFramebuffers(1, &m_idGL);
}

void Framebuffer::SetAttachment(int attachmentId, RenderTexture *tex)
{
    tex->CreateEmpty(GetWidth(), GetHeight());

    GLuint glAttachment = GL_COLOR_ATTACHMENT0 +
                          m_attachmentId_To_Texture.Size();
    m_attachmentId_To_Texture[attachmentId] = tex;
    m_attachmentId_To_GLAttachment[attachmentId] = glAttachment;

    //Attach it to framebuffer
    Bind();
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, glAttachment,
                           GL_TEXTURE_2D, tex->GetGLId(), 0);
    CheckFramebufferError();
    UnBind();
    //
}

void Framebuffer::CreateDepthRenderbufferAttachment()
{
    Bind(); //TODO:  respect former bindings of renderbuffers

    glGenRenderbuffers(1, &m_depthAttachmentId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttachmentId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                          m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, m_depthAttachmentId);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    CheckFramebufferError();
    UnBind();
}

RenderTexture *Framebuffer::GetColorAttachment(int attachmentId) const
{
    if (!m_attachmentId_To_Texture.ContainsKey(attachmentId))
    {
        return nullptr;
    }
    return m_attachmentId_To_Texture.Get(attachmentId);
}

void Framebuffer::SetAllDrawBuffers() const
{
    Array<int> attachmentIds;
    for (int i = 0; i < m_attachmentId_To_GLAttachment.Size(); ++i)
    {
        attachmentIds.PushBack(i);
    }
    SetDrawBuffers(attachmentIds);
}

void Framebuffer::SetDrawBuffers(const Array<int> &attachmentIds) const
{
    m_currentDrawAttachmentIds = attachmentIds;
    Array<GLenum> drawBuffers;
    for (int attId : attachmentIds)
    {
        drawBuffers.PushBack(m_attachmentId_To_GLAttachment.Get(attId));
    }

    Bind();
    glDrawBuffers(drawBuffers.Size(), &drawBuffers[0]);
    CheckFramebufferError();
    UnBind();
}

void Framebuffer::SetReadBuffer(int attachmentId) const
{
    Bind();
    glReadBuffer(m_attachmentId_To_GLAttachment.Get(attachmentId));
    CheckFramebufferError();
    UnBind();
}

const Array<int> &Framebuffer::GetCurrentDrawAttachmentIds() const
{
    return m_currentDrawAttachmentIds;
}

Color Framebuffer::ReadColor(int x, int y, int attachmentId) const
{
    Bind();
    Color c;
    RenderTexture *t = GetColorAttachment(attachmentId);
    SetReadBuffer(attachmentId);
    glReadPixels(x, y, 1, 1, t->GetGLFormat(), t->GetGLDataType(), &c);
    UnBind();
    return c;
}


void Framebuffer::Resize(int width, int height)
{
    m_width  = Math::Max(width,  1);
    m_height = Math::Max(height, 1);

    for (auto it : m_attachmentId_To_Texture)
    {
        RenderTexture *t = it.second;
        if (t)
        {
            t->Resize(m_width, m_height);
        }
    }

    if (m_depthAttachmentId != 0)
    {
        //TODO:  respect former bindings of renderbuffers
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttachmentId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                              m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

int Framebuffer::GetWidth() const
{
    return m_width;
}

int Framebuffer::GetHeight() const
{
    return m_height;
}

Vector2 Framebuffer::GetSize() const
{
    return Vector2(GetWidth(), GetHeight());
}

void Framebuffer::Clear()
{
    Bind();
    SetAllDrawBuffers();
    GL::ClearDepthBuffer();
    GL::ClearColorBuffer();
    UnBind();
}

void Framebuffer::ClearDepth(float clearDepth)
{
    Bind();
    GL::ClearDepthBuffer(clearDepth);
    UnBind();
}

void Framebuffer::ClearColor(const Color &clearColor)
{
    Bind();
    SetAllDrawBuffers();
    GL::ClearColorBuffer(clearColor);
    UnBind();
}

void Framebuffer::CheckFramebufferError() const
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        String errMsg =
           "There was an error when creating an attachment for a framebuffer.";
        //Debug_Error(errMsg);
    }
}

void Framebuffer::Bind() const
{
    PreBind(GL_FRAMEBUFFER_BINDING);
    glBindFramebuffer(GL_FRAMEBUFFER, m_idGL);
    glViewport(0, 0, m_width, m_height);
}

void Framebuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, PreUnBind(GL_FRAMEBUFFER_BINDING));
}

void Framebuffer::SaveToImage(int attachmentId, const String &filepath,
                              bool invertY) const
{
    Texture *tex = m_attachmentId_To_Texture.Get(attachmentId);
    if (tex)
    {
        SetReadBuffer(attachmentId);
        unsigned int bytesSize = m_width * m_height * 4;
        unsigned char *pixels = new unsigned char[bytesSize];
        glReadPixels(0, 0, m_width, m_height,
                     GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        if (invertY)
        {
            unsigned char *pixelsCpy = new unsigned char[bytesSize];
            memcpy(pixelsCpy, pixels, bytesSize);
            for (int i = 0; i < m_width; ++i)
            {
                for (int j = 0; j < m_height; ++j)
                {
                    int coords    = j * m_width + i;
                    int invCoords = (m_height - j - 1) * m_width + i;
                    coords *= 4; invCoords *= 4;
                    pixels[coords + 0] = pixelsCpy[invCoords + 0];
                    pixels[coords + 1] = pixelsCpy[invCoords + 1];
                    pixels[coords + 2] = pixelsCpy[invCoords + 2];
                    pixels[coords + 3] = pixelsCpy[invCoords + 3];
                }
            }
            delete[] pixelsCpy;
        }
        stbi_write_bmp(filepath.ToCString(), m_width, m_height, 4, pixels);
        delete[] pixels;
    }
}




