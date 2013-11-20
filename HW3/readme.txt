CS 543 HW3

Chengjiao Yang 	10/28/2013

==============================================================================================

1.What does this project do

This project 100% doing what the HW3 requirement (CS 543, fall 2013 Homework 3: Hierarchical 3D Modeling of L-System Trees), and 100% complete it, and 100% can run in Zoolab machines.

==============================================================================================

2.What are the files in zip file doing

Zip file include: 

- Source vs2010 project: Graphics_Starting_Code_3
- Project document:  readme.txt

This project code comes from the ¡°start_code¡± which the professor provide us, I was doing the programming at example1.cpp at this project. In addition, I make a folder called ¡°ply_files¡± for storing the 45 ply format files and ¡°lsys_files¡± for storing the 7 L-system files.

==============================================================================================

3.What functions does this project have

- 'a' key Response: Insert a ¡®a¡¯ tree ( lsys1.txt ) at random position with random trunk color.

- 'b' key Response: Insert a ¡®b¡¯ tree ( lsys2.txt ) at random position with random trunk color.

- 'c' key Response: Insert a ¡®c¡¯ tree ( lsys3.txt ) at random position with random trunk color.

- 'd' key Response: Insert a ¡®d¡¯ tree ( lsys1.txt ) at random position with random trunk color.

- 'e' key Response: Insert a ¡®e¡¯ tree (lsysmyown.txt ) at random position with random trunk color.

- 'f' key Response: draws park including your forest of trees, ground plane and car. There should be 7 trees, 5 components ply and a grey plain.

- 'o' key Response: Inserting of Car and other components. There are five different PLY components.

- 'p' key Response: Drawing of the ground plane.

- 'w' key Response: Clear the screen, press twice please.

- 'ESC' key Response: The program terminates. 

==============================================================================================

4.How to run the project

Just run GLSLExperiment.sln in vs2010, use Ctrl+F5 to run it, or u can directly run GLSLExperiment.exe at Debug folder.

==============================================================================================

5.TIPS

- Tree trunk color is random, the leaf color is green
- ¡®a¡¯ tree and ¡®b¡¯ is slow when loading, please be patienced.
- The trees look ugly, but they are really using polyCylinders.
- It's better to run 'f' function first, then use 'a''b''c''d''e''o' to add other components.

