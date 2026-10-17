# BeamMP-Launcher

The launcher is the way we communitcate to outside the game, it does a few automated actions such as but not limited to: downloading the mod, launching the game, and create a connection to a server.

## [Getting started](https://docs.beammp.com/game/getting-started/)


### What's this for(k)?

BeamMP-Launcher searches for BeamNG.drive installed by steam, this means it won't work with any installation outside of steam.

This fork adds a GamePath variable to Launcher.config, and a CLI switch, so that the user can specify where their game is installed.

The alternative is symlinking your installation to a steamlibrary directory, and adding the appid to libraryfolders.vdf every time steam rewrites it.

### How does it f(ing)work?

BeamMP launches, `ConfigInit` parses `GamePath` (with `~` expansion) into the `GamePathOverride` global before `LegitimacyCheck()` runs. 
`LegitimacyCheck()` honours it first, on every OS: if it points at a directory containing `integrity.json`, that directory is used as `GameDir` with a trailing separator and detection returns immediately. 
If it is set, but invalid, the launcher exits with one clear line instead of `integrity.json` filesystem failure. If `GamePath` is empty or missing, behavior is exactly the original code path.


Example config:

```json

{
    "Port": 4444,
    "Build": "Default",
    "CachingDirectory": "./Resources",
    "GamePath": "/home/driver/games/BeamNG.drive/"
}

```

### Prebuilt vcpkg included

Binaries under [releases](https://github.com/SerShef/BeamMP-Launcher4Linux/releases) and [AUR](https://aur.archlinux.org/packages/beammp-launcher4linux) include vcpkg as a prebuilt binary, so no package or build dependency. 

### Build

Linux:
```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Windows (tested with wine):
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build build --config Release --parallel
```

macOS (run via CrossOver/Wine - not officially supported by developers and I'm unable to test) _Open an issue if there are failures building_:
```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=arm64-osx
cmake --build build --config Release --parallel
```

BeamNG.drive is not supported on macOS but can be run using CrossOver/Whisky (Wine).

Set `GamePath` in `Launcher.cfg` for BeamMP to work, same as Linux.

### Launcher.cfg location

- AUR install: `~/.local/share/beammp/Launcher.cfg` (the wrapper runs from there)
- Prebuilt binary: the directory you run it from (current working directory)

### Install on Arch Linux (AUR):

```sh
yay -S beammp-launcher4linux
# or
paru -S beammp-launcher4linux
# or
git clone https://aur.archlinux.org/beammp-launcher4linux.git
cd beammp-launcher4linux
makepkg -si
```

## License

BeamMP Launcher, a launcher for the BeamMP mod for BeamNG.drive
Copyright (C) 2024 BeamMP Ltd., BeamMP team and contributors.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
