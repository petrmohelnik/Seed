#pragma once

class Texture;

class TextureCubeMap
{
public:
    TextureCubeMap();
    ~TextureCubeMap();
    TextureCubeMap(TextureCubeMap&& m) = default;
    TextureCubeMap(const TextureCubeMap& m) = default;
    TextureCubeMap& operator=(const TextureCubeMap& m) = default;
    TextureCubeMap& operator=(TextureCubeMap&& m) = default;

protected:
    friend class FileSystem;
    friend class Skybox;

    void Load();
    void Unload();

    void BindTexture();

private:
    std::vector<std::shared_ptr<Texture>> faces; //right, left, top, bottom, front, back

    bool deleteAfterLoad = true;

    GLuint texture = 0;
};

