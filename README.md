[![Build Status](https://travis-ci.org/Bang3DEngine/Bang.svg?branch=master)](https://travis-ci.org/Bang3DEngine/Bang)
# Bang
<br/>
<img src="/EngineAssets/BangLogo.png" width="100">
<br/> <br/>
Bang 3D game engine and editor, using C++. <br/> 

# Table of contents
  * [Install](#install)
  * [Execute Bang](#execute-bang)
  * [Dependencies](#dependencies)
    * [Install dependencies in Ubuntu](#install-dependencies-in-ubuntu)
  
# Install
In order to install Bang do the following:
``` Bash
git clone https://github.com/Bang3DEngine/Bang
cd Bang
./scripts/fullCompile.sh EDITOR RELEASE_MODE
```
This will take a few minutes. <br/>
If something fails, check the [Dependencies](#dependencies) section below.

# Execute Bang
Once [installed](#install), you can start Bang using:
```Bash
./bin/Bang
```

# Dependencies
In order to compile Bang you will need:
  - GLEW (**libglew-dev** in Ubuntu).
  - Qt5 development libraries (**qt5-default** in Ubuntu).
  - Freetype development libraries (**libfreetype6-dev** in Ubuntu).
  - OpenAL and ALUT development libraries. (**libopenal-dev** & **libalut-dev** in Ubuntu).

Recommended:  
  - QtCreator to edit the scripts with auto-complete. The editor uses it as the main script editor. (**qtcreator** in Ubuntu).

# Install Dependencies in Ubuntu
To install the needed dependencies in Ubuntu try the following:
``` Bash
sudo apt-get install libglew-dev qt5-default libfreetype6-dev libopenal-dev libalut-dev qtcreator
```
