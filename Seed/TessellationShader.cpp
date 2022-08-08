#include "TessellationShader.h"

TessellationShader::TessellationShader(GLuint program_)
{
    program = program_;
}

void TessellationShader::setup()
{
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glUseProgram(program);
}

void TessellationShader::draw(int count)
{
    glDrawElements(GL_PATCHES, count, GL_UNSIGNED_INT, 0);
}
