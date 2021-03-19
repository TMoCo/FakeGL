COMP 5822 Foundations of Modelling and rendering

FakeGL, a software implementation of the OpenGL pipeline in C++ to help understand the rendering pipeline.

To compile on Linux

module add qt/5.13.0
qmake -project QT+=opengl
qmake
make

Models are stored in obj format, textures in PPM P3 format (no compression, ASCII RGB values)

! uses C++11 !

To compile on OSX:
Use Homebrew to install qt

qmake -project QT+=opengl
qmake
make

To compile on Windows:
Unfortunately, the official OpenGL on Windows was locked at GL 1.1.  Many many hacks exist, and they all disagree.
Just to make it worse, the Qt response to this is clumsy.  Net result: there is no easy way to get this compiling on Windows.
I will aim to update these instructions at a later date.

To run on Linux:

./FakeGLRenderWindowRelease ../path_to/model.obj ../path_to/texture.ppm

To run on OSX:
./FakeGLRenderWindowRelease.app/Contents/MacOS/FakeGLRenderWindowRelease  ../path_to/model.obj ../path_to/texture.ppm

In the example below, OpenGL is rendered on the left and FakeGL is rendered on the right, uncanny!
![FakeGL in action](https://media.giphy.com/media/OoNGvtadFlSEZoIlY4/giphy.gif)
![FakeGL in action](https://media.giphy.com/media/Kjqm3je1ysKnWyAVW1/giphy.gif)
