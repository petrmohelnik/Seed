#pragma once

class Framebuffer
{
public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    void AddDepthRenderbuffer();
    void AddStencilRenderbuffer();
    void SetNoColorAttachment();
    void ChangeSize(int width, int height);
    glm::ivec2 GetSize();

    void Bind() const;
    void AttachTexture(GLuint attachment, GLuint textureTarget, GLuint texture, GLuint mipLevel = 0) const;
    void AttachCubeMapTexture(GLuint attachment, GLuint texture, GLuint mipLevel = 0) const;
    void SetDrawBuffers(std::vector<GLuint> buffers);
    static void Unbind();
    void BlitDepthBufferToDefaultFramebuffer();

private:
    int width, height;

    GLuint fbo = 0;
	GLuint depthRbo = 0;
	GLuint stencilRbo = 0;
};

