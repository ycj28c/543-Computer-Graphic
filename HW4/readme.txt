CS 543 HW4

Chengjiao Yang 	11/19/2013

==============================================================================================

1.What does this project do

This project 100% doing what the HW4 requirement, and 100% complete it, and 100% can run in Zoolab machines.

==============================================================================================

2.What are the files in zip file doing

Zip file include: 

- Source vs2010 project: Graphics_Starting_Code_4
- Project document:  readme.txt

This project code comes from the ¡°start_code¡± which the professor provide us, I was doing the programming at example1.cpp at this project. In addition, I make a folder called ¡°ply_files¡± for storing the 45 ply format files and ¡°lsys_files¡± for storing the 7 L-system files and env for envmapping.

==============================================================================================

3.What functions does this project have

-Key D: Toggle shadows ON/OFF. When ON, the shadows show up and when OFF the shadows do not show up. 

-Key A: Toggle ON/OFF between a grass texture and stone texture. When ON, the floor is textured with grass and when OFF the floor is textured using the stone texture. 

-key F: Toggle between fog that changes linearly with depth and fog changing exponentially with depth 

-Key T: Toggle reflection ON/OFF. When ON, all PLY objects are drawn with reflection. When OFF, the PLY objects are drawn with no reflection (same as you had in HW3)

-Key V: Toggle refraction ON/OFF. When ON, all PLY objects are drawn with refraction. When OFF, the PLY objects are drawn with no refraction (same as you had in HW3)

-Key K: Toggle drawing PLY files with a mixture of reflection and refraction ON/OFF. Randomly select which PLY objects to make reflective or refractive. Since this choice is randomized, every time you toggle the K key ON, a different set of PLY objects are reflective vs refractive. When toggled ON, all PLY objects are drawn with either reflection or refraction. When OFF, the PLY objects are drawn with no reflection or refraction (same as you had in HW3)

- 'ESC' key Response: The program terminates. 

==============================================================================================

4.How to run the project

Just run GLSLExperiment.sln in vs2010, use Ctrl+F5 to run it, or u can directly run GLSLExperiment.exe at Debug folder.

==============================================================================================

5.TIPS

- its slow, please be patienced.
- my line fog is different, i use ambient, diffuse, specular, so it looks different, but its liner fog!

