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
    void SetDrawBuffers(std::vector<GLuint> buffers);
    static void Unbind();
    void BlitDepthBufferToDefaultFramebuffer();

private:
    int width, height;

    GLuint fbo;
    GLuint rbo = 0;
};

