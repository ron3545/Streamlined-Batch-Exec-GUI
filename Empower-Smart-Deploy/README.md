Step 1: Compile the project by running this command: 
```bash
	cmake -S . -B build
```

Step 2: Build the project by running this command:
```bash
	cmake --build build -- /p:CharacterSet=Unicode
```

Step 3: After the comilation and building step is done, go to this directory and run the "*.exe" file:
```bash
	<Project Root Dir>/build/src/Debug
```