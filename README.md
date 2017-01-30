# Bang
<br/>
<img src="/EngineAssets/BangLogo.png" width="150">
<br/>
Bang 3D Game Engine and Editor. <br/>
Made with C++, OpenGL, and Qt5 for the Editor.

# Install
In order to install Bang do the following:
``` Bash
git clone https://github.com/Bang3DEngine/Bang
cd Bang
./scripts/fullCompile.sh EDITOR RELEASE_MODE
```
This will take a few minutes. <br/>
If something fails, check the Dependencies section below.

# Execute Bang
Once installed (see above), you can start Bang using:
```Bash
./bin/Bang
```

# Dependencies
You need:
  - OpenGL headers (usually coming by default).
  - GLEW (libglew-dev in Ubuntu).
  - Qt5 development libraries (qt5-default in Ubuntu).
  - Freetype development libraries (libfreetype6-dev in Ubuntu).
  - lbz2 (libbz2-dev in Ubuntu).
  - g++ (usually coming by default).

# Install Dependencies in Ubuntu
``` Bash
sudo apt-get install libglew-dev
sudo apt-get install qt5-default
sudo apt-get install libfreetype6-dev
sudo apt-get install libbz2-dev
```
