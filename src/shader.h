#ifndef _SHADER
#define _SHADER

unsigned int createShader(const char *vsFile, const char *fsFile);

char *readShader(const char *fileName);
unsigned int compileShader(unsigned int type, const char *source);
unsigned int linkShader(unsigned int vertexShader, unsigned int fragmentShader);

void setUniformb(unsigned int ID, const char *name, int value);
void setUniformi(unsigned int ID, const char *name, int value);
void setUniformf(unsigned int ID, const char *name, float value);
#endif
