//
// Created by Pedro Ganme on 15/05/24.
//

#pragma once
#include <vector>

#ifdef WITH_EMSCRIPTEN
#else
    #include <glad/glad.h>
#endif

#include <string>

struct ShaderFile {
    std::string filePath;
    GLenum type;
};

class Shader {
public:
    ~Shader();

    unsigned int GetRendererID() const { return m_RendererID; }

    static Shader* LoadFromFile(std::vector<ShaderFile> shaderFiles);
private:
    Shader() = default;

    void LoadFromTextFiles(std::vector<ShaderFile> shaderFiles);
    unsigned int CompileShader(GLenum type, const std::string& source);
private:
    unsigned int m_RendererID;
};
