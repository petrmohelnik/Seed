#pragma once

class Texture
{
public:
    Texture();

    void Load();
    void Unload();

    void BindTexture();

protected:
    friend class FileSystem;
    void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void SetColor(Uint8 r, Uint8 g, Uint8 b);
    void SetColor(Uint8 r1, Uint8 r2);
    void SetColor(Uint8 r);

private:
	GLuint GetInternalFormat();
	GLuint GetFormat();

    std::vector<Uint8> data;
    unsigned int width;
    unsigned int height;
	unsigned int bytesPerPixel;

    bool deleteAfterLoad = true;

    GLuint texture = 0;
};
