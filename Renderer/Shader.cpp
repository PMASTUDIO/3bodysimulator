//
// Created by Pedro Ganme on 15/05/24.
//

#include "Shader.h"

#include <iostream>
#include <fstream>

static std::string ReadFileAsString(const std::string& filepath)
{
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        result.resize((size_t)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        std::cout << "Current working directory " << std::filesystem::current_path() << std::endl;
        std::cout << "Could not open file " << filepath << std::endl;
    }

    return result;
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

Shader * Shader::LoadFromFile(std::vector<ShaderFile> shaderFiles) {
    Shader* shader = new Shader();
    shader->LoadFromTextFiles(shaderFiles);
    return shader;
}

void Shader::LoadFromTextFiles(std::vector<ShaderFile> shaderFiles) {
    GLuint program = glCreateProgram();
    int glShaderIDIndex = 0;

    std::vector<unsigned int> compiledShadersID;

    for (auto shader_file: shaderFiles) {
        std::string source = ReadFileAsString(shader_file.filePath);
        unsigned int shader = CompileShader(shader_file.type, source);
        glAttachShader(program, shader);

        compiledShadersID.push_back(shader);
    }

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(program);

        for (unsigned int shader_id: compiledShadersID) {
            glDeleteShader(shader_id);
        }

        std::cout << "ERROR: " << infoLog.data() << std::endl;
    }

    for (unsigned int shader_id: compiledShadersID) {
        glDetachShader(program, shader_id);
        glDeleteShader(shader_id);
    }

    m_RendererID = program;
}

unsigned int Shader::CompileShader(GLenum type, const std::string &source) {
    GLuint shader = glCreateShader(type);

    const GLchar* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        glDeleteShader(shader);

        std::cout << "ERROR: " << infoLog.data() << std::endl;
        // HZ_CORE_ASSERT(false, "Shader compilation failure!");
    }

    return shader;
}
