#include "BasicShader.h"

BasicShader::BasicShader(GLuint program_)
{
    program = program_;
}

void BasicShader::setup()
{
    glUseProgram(program);
}

void BasicShader::draw(int count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
