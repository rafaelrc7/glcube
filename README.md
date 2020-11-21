# GLCube

This is a simple Cube I did as an exercise. It is rendered in OpenGL and has some keybindings to rotate and scale the cube and to change the rendering mode (Fill, Lines or Points).

## Dependencies
This program depends on:
* [OpenGL 4.3](https://www.khronos.org/opengl/wiki/Getting_Started#Downloading_OpenGL)
* [GLEW 2.1.0](http://glew.sourceforge.net/)
* [GLFW 3.3.2](https://www.glfw.org/)
* [CGLM 0.7.9](https://github.com/recp/cglm)

## Building

Install the dependencies listed above. OpenGL is distributed with your graphics cards drivers (eg. Intel, AMD or NVIDIA). Look for and install the relevant packages.

GLEW, GLFW, CGLM may be downloadable trough your package manager. On Arch GLEW and GLFW can be found on the official repositories while CGLM is available in the AUR. You can also download the source codes and compile them yourself.

With the dependencies sorted out, you may compile the project.
```shell
git clone https://github.com/rafaelrc7/glcube
cd glcube
make
```

## Usage

The program is simple and has quite few controls.
* W,A,S,D - Rotates cube around the X and Y axis.
* R,F - Scales the cube, making it bigger or smaller.
* 1,2,3 - Set glPolygonMode to, respectively, GL_FILL, GL_LINE and GL_POINT.

## License
[MIT](https://choosealicense.com/licenses/mit/)
