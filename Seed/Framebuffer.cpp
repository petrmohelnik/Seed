#include "Framebuffer.h"
#include "Engine.h"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height)
{
    glGenFramebuffers(1, &fbo);
}

Framebuffer::~Framebuffer()
{
    if(rbo != 0)
        glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::AddDepthRenderbuffer()
{
    glGenRenderbuffers(1, &rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindRenderbuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::ChangeSize(int width_, int height_)
{
    width = width_; height = height_;

    if (rbo != 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

        glBindRenderbuffer(GL_FRAMEBUFFER, 0);
    }
}

void Framebuffer::Bind() const
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::AttachTexture(GLuint attachment, GLuint textureTarget, GLuint texture, GLuint mipLevel) const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureTarget, texture, mipLevel);
}

void Framebuffer::Unbind() const
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
