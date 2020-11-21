#include <GL/glew.h>

#include <stdlib.h>
#include <stdio.h>

#include "shader.h"

unsigned int createShader(const char *vsFile, const char *fsFile)
{
	unsigned int sp, vs, fs;
	char *srcBuffer;

	srcBuffer = readShader(vsFile);
	vs = compileShader(GL_VERTEX_SHADER, srcBuffer);
	free(srcBuffer);

	srcBuffer = readShader(fsFile);
	fs = compileShader(GL_FRAGMENT_SHADER, srcBuffer);
	free(srcBuffer);

	sp = linkShader(vs, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return sp;
}

char *readShader(const char *fileName)
{
	FILE *handle;
	char *source;
	long int srcLen;

	handle = fopen(fileName, "rb");
	if (!handle) {
		fprintf(stderr, "Failed to open %s shader source file\n", fileName);
		return NULL;
	}

	fseek(handle, 0L, SEEK_END);
	srcLen = ftell(handle);
	fseek(handle, 0L, SEEK_SET);

	source = (char*)malloc(srcLen+1);
	if (!source) {
		fprintf(stderr, "Failed to dynamically allocate memory\n");
		return NULL;
	}

	fread(source, srcLen, 1, handle);
	fclose(handle);

	source[srcLen] = '\0';

	return source;
}

unsigned int compileShader(unsigned int type, const char *source)
{
	int ret, logLen;
	char *log;
	unsigned int shaderID = glCreateShader(type);

	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
		log = (char*)malloc(logLen);

		glGetShaderInfoLog(shaderID, logLen, NULL, log);
		fprintf(stderr, "[**ERROR** %s SHADER COMPILATION]:\n\t%s\n",
				(type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), log);

		free(log);
		glDeleteShader(shaderID);

		return 0;
	}

	return shaderID;
}
unsigned int linkShader(unsigned int vertexShader, unsigned int fragmentShader)
{
	int ret, logLen;
	char *log;
	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ret);
	if (!ret) {
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLen);
		log = (char*)malloc(logLen);

		glGetProgramInfoLog(shaderProgram, logLen, NULL, log);
		fprintf(stderr, "[**ERROR** SHADER LINKAGE]:\n\t%s\n", log);

		free(log);
		glDeleteProgram(shaderProgram);

		return 0;
	}

	return shaderProgram;
}

void setUniformb(unsigned int ID, const char *name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}
void setUniformi(unsigned int ID, const char *name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name), value);
}
void setUniformf(unsigned int ID, const char *name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name), value);
}

