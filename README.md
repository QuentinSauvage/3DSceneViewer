# 3DSceneViewer
C++ project made during the 2nd year of Master, allowing to visualize objects and to move around freely.

# Build
To run the program, execute the make command then launch the _tdsv_ file located in the _bin_ folder. The command line expects one or more files with the _.geo_ extension.



.geo files should be structured this way:

- One line with the number _v_ corresponding to the number of vertices of the object.

- V lines, each one describing  a vertex (three coordinates).

- One line with the number _t_ of triangles of the object.

- T lines, each one describing a triangle by giving the index of the three corresponding vertices.

# Controls

- Arrows: Move the camera

- A/D: Rotate the scene around the Y axis.

- W/S: Rotate the scene around the X axis.

- Q/E: Rotate the scene around the Z axis.

- Z/X: Zoom in/out.

- Space: Reset to the initial view.
