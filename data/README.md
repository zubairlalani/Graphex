# Graphex

[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)

**Author**: Zubair Lalani - [`zubairl2@illinois.edu`](mailto:example@illinois.edu)

### Overview
 At its core, Graphex is a vector calculus graphing software. 
 
 Features include:
 - 2D & 3D vector field graphing
 - 2D scalar function graphing
 - Divergence, Curl, and Work calculations
 - "Pen Mode" where the user can draw on the field
 - Particle simulations in vector fields
 - 3D camera movement
 
![vector field gif](https://github.com/zubairlalani/Graphex/blob/main/3DVectorFieldSample.gif)

![vector field gif](https://github.com/zubairlalani/Graphex/blob/main/3DVectorFieldSample.gif)

### Dependencies    
Running the simulation will require:
- CMake
- Cinder v0.9.2 and above

### Quickstart
- If you do not have Cinder, download it [here](https://libcinder.org/download)
- Clone the repository using
```git clone https://github.com/uiuc-fa20-cs126/final-project-zubairlalani.git```
- Ensure that the exprtk header file is in the include directory. If not, it can be installed here: https://github.com/ArashPartow/exprtk.git
- Build and run the project from CLion

### Documentation
This codebase follows the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) with Doxygen-style
documentation. Learn more about Doxygen [here](http://www.doxygen.nl/).

### How To Use
- When you run the project, there should be a panel bar from which most of the functionalities can be accessed. 
- If you want to graph a 2D vector field, input the i-component and j-component. Then, click "Draw".
- If you would like to clear the field, press the clear button.
- The arrows can be scaled up or down using the Arrow Scale input. Typically, you would want the scale to be around 
0.1 - 0.5 in order to avoid arrow overlap. If you want to draw the arrows to scale, change it to 1.
- In order to add a particle, specify an "X position" and "Y Position", then click "Add Particle". Alternatively, if you 
click and hold the particle at the top right corner, a particle can be dragged onto the field. Just release the mouse button 
at whatever position you would like.
- To draw on the screen, click "Enter Pen Mode". Then you should be able to use the mouse to draw. Click "Change Pen Color", 
to switch colors. Additionally, the work across a curve you have drawn can be calculated if you click "Calculate Work" after 
drawing a curve and a field. 
- In order to erase all markings on the screen, click "Erase".
- If you are done drawing, click "Exit Pen Mode" to no longer make markings.
- To graph a scalar function, input a single-variable expression next to "y = " or "y2 = ". Then, click "Graph".
- A single graph can be erased by clicking "Undo". Alternatively, all graphs can be erased using "Clear Graphs".
- In order to graph a 3D vector field, click "Toggle 3D". Three dimensional axes should appear. Then, at the top of the panel bar, 
input an i-component, j-component, and k-component. Finally, click "Draw".
