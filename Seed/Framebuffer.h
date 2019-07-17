#pragma once

class Framebuffer
{
public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    void AddDepthRenderbuffer();
    void ChangeSize(int width, int height);

    void Bind() const;
    void AttachTexture(GLuint attachment, GLuint textureTarget, GLuint texture, GLuint mipLevel = 0) const;
    void Unbind() const;
    void BlitDepthBufferToDefaultFramebuffer();

private:
    int width, height;

    GLuint fbo;
    GLuint rbo = 0;
};

