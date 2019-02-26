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
    void SetColor(Uint8 x, Uint8 y, Uint8 z, Uint8 w);
    void SetColor(Uint8 x, Uint8 y, Uint8 z);
    void SetColor(Uint8 x, Uint8 y);
    void SetColor(Uint8 x);

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
