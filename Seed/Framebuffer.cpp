#include "Framebuffer.h"
#include "Engine.h"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
{
    glGenFramebuffers(1, &fbo);
}

Framebuffer::~Framebuffer()
{
    if(depthRbo != 0)
        glDeleteRenderbuffers(1, &depthRbo);
    if (stencilRbo != 0)
        glDeleteRenderbuffers(1, &stencilRbo);
    glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::AddDepthRenderbuffer()
{
    glGenRenderbuffers(1, &depthRbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::AddStencilRenderbuffer()
{
    glGenRenderbuffers(1, &stencilRbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, stencilRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilRbo);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::SetNoColorAttachment()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void Framebuffer::ChangeSize(int width_, int height_)
{
    if (width == width_ && height == height_)
        return;

    width = width_; height = height_;

    if (depthRbo != 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

        glBindRenderbuffer(GL_FRAMEBUFFER, 0);
    }
    if (stencilRbo != 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, stencilRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glBindRenderbuffer(GL_FRAMEBUFFER, 0);
    }
}

glm::ivec2 Framebuffer::GetSize()
{
    return glm::ivec2(width, height);
}

void Framebuffer::Bind() const
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::AttachTexture(GLuint attachment, GLuint textureTarget, GLuint texture, GLuint mipLevel) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureTarget, texture, mipLevel);
}

void Framebuffer::AttachCubeMapTexture(GLuint attachment, GLuint texture, GLuint mipLevel) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture, mipLevel);
}

void Framebuffer::SetDrawBuffers(std::vector<GLuint> buffers)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glDrawBuffers(static_cast<GLuint>(buffers.size()), &buffers[0]);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Engine::GetWindow().ResetViewport();
}

void Framebuffer::BlitDepthBufferToDefaultFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
