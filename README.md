# ESD
```bash
    git clone --recursive https://github.com/ron3545/Streamlined-Batch-Exec-GUI.git
    cd Streamlined-Batch-Exec-GUI
    cd Empower-Smart-Deploy
```
## Buildig with cmake

```bash
  mkdir build
  cd build
  cmake ..
```
To produce a Release or Debug. Simply use this
```bash
  cmake --build . --config Release
```
             OR
```bash
  cmake --build . --config Debug
```
To run it. Go inside the build directory->src->Debug->ESD_core.exe

EXE file is located on the:
```bash
    build->src->Release->ESD_core.exe    //for Release version
    build->src->Debug->ESD_core.exe      //for Debug version
```

