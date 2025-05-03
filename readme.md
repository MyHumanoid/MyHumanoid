MyHumanoid
==========

MyHumanoid is a modernized fork of MakeHuman 0.9.1

#### Building

Dependencies can be either sourced from the system or by cmake from sourcecode.

| CMake option         | Description                        | Default |
| -------------------- | ---------------------------------- | ------- |
| MH_ENABLE_TRACY      | Enable tracing using tracy         | OFF     |
| MH_USE_SYSTEM_FMT    | Use system provided fmt library    | ON      |
| MH_USE_SYSTEM_GLM    | Use system provided glm library    | ON      |
| MH_USE_SYSTEM_PHYSFS | Use system provided physfs library | ON      |
| MH_USE_SYSTEM_SDL    | Use system provided SDL library    | OFF     |

Configure these by using cmake-gui or on the commandline (cmake -D MH_ENABLE_TRACY=ON ..).

If you want to use system provided libraries you have to install them using the system dependent package manager:

Debian and derivatives e.g. Ubuntu:

```sh
apt install cmake libfmt-dev libglm-dev libphysfs-dev libsdl3-dev
```

#### Original MakeHuman 0.9.1 licenses
~~~~
MakeHuman© code is released under GNU General Public License (GPL v.3)
Copyright© 2001-2008 makehuman.org

MakeHuman© mesh is released under MIT License
Copyright© 2001-2007 makehuman.org
~~~~
[Archive 2008.05.15: http://www.dedalo-3d.com/legal.php](https://web.archive.org/web/20080515074707/http://www.dedalo-3d.com/legal.php)
