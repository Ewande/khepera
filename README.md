# Khepera Simulation System

Simple simulation system dedicated to research in areas such as Evolutionary Robotics and Reinforcement Learning.

## Simulation Server 
Server can be used in two modes:
- local (project files compiled to \*.dll/\*.so shared library - tested on MS Windows and OS X)  
  This mode is useful during research. 
  Compiled library can be imported to Python/C#/C++ and used to control simulation by functions from DllInterface.
- distributed (project files compiled to \*.exe - support for MS Windows only)  
  This mode is useful to test your robot controllers or play with others. Run server with -h option for more help.

## Controller
Use with Simulation Server running in distributed mode. One can select manual or automated steering.  
Currently \*.rcs (produced by Genetic Evolver) and \*.nn files are supported. \*.nn file format: WILL BE PROVIDED SOON 

## Visualiser
Use with Simulation Server running in distributed mode. Provides real-time visual representation of the environment.  

## Map Editor
In progress.
