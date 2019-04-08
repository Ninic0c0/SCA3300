# SCA3300-D01 3-axis Industrial Accelerometer and Inclinometer with Digital SPI Interface Library [![Awesome](https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://github.com/sindresorhus/awesome)
> This repository contains a minimal driver to use Murata SCA3300-D01 3-axis device.

![Murata SCA3300-D01](https://brightcove04pmdo-a.akamaihd.net/4741948346001/4741948346001_5393517021001_5199040810001-vs.jpg)

## Information

First of all this library is really light. The code is probbaly not perfect but works. Feel free to fork it :) 

This project is based on Meson/Ninja build system  
More information Menson/Ninja : http://mesonbuild.com/  

You will find 3 sub folders.  
Once contains the library, once an Hello World and the last one some unit tests.

As the project is base on Meson you can find configuration files as:  
[Master Meson file](./meson.build)  
[Binary Meson file](./src/example/meson.build)  
[Librairie Meson file ](./src/libsca3300/meson.build)  

## Download sources
```sh
[nicolas:Trash]% git clone ssh://***********************************ninja.git
Cloning into 'sca3300'...
remote: Counting objects: 21, done.
remote: Compressing objects: 100% (19/19), done.
remote: Total 21 (delta 4), reused 0 (delta 0)
Receiving objects: 100% (21/21), 11.27 KiB | 0 bytes/s, done.
Resolving deltas: 100% (4/4), done.
Checking connectivity... done.
[nicolas:Trash]% cd SCA3300 
```

## Build 

### Native compilation

```sh
[nicolas:sca3300]% mkdir build && meson build
```

### Cross compilation ( for Raspberry pi 3 )
```sh
[nicolas:lib]% mkdir buildcross
[nicolas:lib]% meson buildcross --cross-file cross_file.txt
```

*Note: As with Make you have access to the clean command:*
```sh
[nicolas:lib]% ninja -C buildcross clean
ninja: Entering directory `buildcross'
[1/1] 'Cleaning'
Cleaning... 7 files.
```


### Build static library and example binary
```sh
[nicolas:lib]% ninja -C buildcross                                                          
ninja: Entering directory `buildcross'
[4/4] Linking target src/example/sca3300-exe
```

## Documentation
```sh
[nicolas:sca3300]% doxygen Doxyfile
Searching for include files...
Searching for example files...
Searching for images...
Searching for dot files...
Searching for msc files...
Searching for dia files...
Searching for files to exclude
Searching INPUT for files to process...
Searching for files in directory ~/workspace/murata/sca3300/lib/src
Searching for files in directory ~/workspace/murata/sca3300/lib/src/example
Searching for files in directory ~/workspace/murata/sca3300/lib/src/libsca3300
Reading and parsing tag files
Parsing files
Preprocessing ~/workspace/murata/sca3300/lib/src/example/example.cpp...
Parsing file ~/workspace/murata/sca3300/lib/src/example/example.cpp...
Preprocessing ~/workspace/murata/sca3300/lib/src/libsca3300/sca3300.cpp...
Parsing file ~/workspace/murata/sca3300/lib/src/libsca3300/sca3300.cpp...
Preprocessing ~/workspace/murata/sca3300/lib/src/libsca3300/sca3300.h...
Parsing file ~/workspace/murata/sca3300/lib/src/libsca3300/sca3300.h...
Preprocessing ~/workspace/murata/sca3300/lib/src/libsca3300/sca3300def.h...
Parsing file ~/workspace/murata/sca3300/lib/src/libsca3300/sca3300def.h...
finished...
```

### Take a look to the documention.
```sh
[nicolas:sca3300]% firefox doc/html/index.html 
```

# Example (build the static lib and hello world for Rapberry pi 3)

*Build:*
```sh
[nicolas:lib]% ninja -C buildcross
ninja: Entering directory `buildcross'
[3/3] Linking target src/example/sca3300-exe
```


*Send binary to the raspberry pi:* 
```sh
[nicolas:lib]% scp -o IdentitiesOnly=yes buildcross/src/example/sca3300-exe pi@1.2.3.4:/home/pi
pi@1.2.3.4's password: 
sca3300-exe                                                100%  285KB 285.0KB/s   00:00    
```
*Establish SSH connection:* 

```sh
[nicolas:lib]% ssh -o IdentitiesOnly=yes pi@1.2.3.4
Linux raspberrypi 4.14.79-v7+ #1159 SMP Sun Nov 4 17:50:20 GMT 2018 armv7l

The programs included with the Debian GNU/Linux system are free software;
the exact distribution terms for each program are described in the
individual files in /usr/share/doc/*/copyright.

Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.
Last login: Wed Apr  3 11:15:00 2019 from 10.2.161.92

SSH is enabled and the default password for the 'pi' user has not been changed.
This is a security risk - please login as the 'pi' user and type 'passwd' to set a new password.

pi@raspberrypi:~ $ ./sca3300-exe 
[OK] Change Mode done.
[OKAY] sca3300 getting probed successfully.
Temperature: 24.0458 °C | raw: 5617
Temperature: 24.0987 °C | raw: 5618
```

# Sources

[Ninja](https://ninja-build.org/)  
[Meson](http://mesonbuild.com/)  
[Wiki de Quentin Gliech](https://git.unistra.fr/ihm/ihm-projet/wikis/meson)  
[Un petit tour des systemes de build](https://linuxfr.org/users/julien_jorge/journaux/un-petit-tour-des-systemes-de-build)  
