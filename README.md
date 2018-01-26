# OpenGLProgram
A small program with OpenGL; Program created as an assignment for my Computer Graphics course

How to Compile the Program:
1. Change into the CPSC453A1 directory
2. In terminal, type: make
3. In terminal, type: ./boilerplate.out

How to Operate the Program:
1. Press the left and right arrow keys to alternate between the 3 scenes
2. Press the up and down arrows to increase/decrease between the iterations (max number of iterations allowed is 6; you can change it to another value by changing the maxLayersAllowed variable in main.cpp)

Platform:
Ubuntu 16.4

Compiler:
g++ 5.4.0 20160609


Please note:
When working on this at home, I had to change the openGL core version from 4.1 to 3.3. By doing that, I also had to change the version numbers in fragment.glsl and vertex.glsl from  #410 to #330. 
