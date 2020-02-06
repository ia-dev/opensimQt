This is a very simple example of using QT with a VTK widget. VTK Widget can handle all the visualization of 3D objects example the model geomtery and interact with them using mouse or keyboard. 

 You need to:
 
  1. install Qt5
  2. vtk 8 library (I build it from source but using the binaries should work) 


TODOS: 

1. Run the execuatble file by double click in linux. 
2. Complete the GUI interface design to be similar to opensim-gui
3. Load opensim model instead of a 3D object.
   - load model elements to the table tree.
   - load model geometry based on their positions to the 3D widget
4. Add simulation menu and button to run the simulation.
   - connect to opensim functions.
   - start with simple model that uses visualizer. 
   - load motion file and visualize the result.  
5. Add two visualization modes: 
      1. view: 
      2. edit: changing position or size affect the geometry. 
  
6. Benifit from slicer source code and plugins 
   - either copy some files and code or
   - maybe use SlicerSalt as a start point?

