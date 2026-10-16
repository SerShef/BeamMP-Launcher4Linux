# BeamMP-Launcher

The launcher is the way we communitcate to outside the game, it does a few automated actions such as but not limited to: downloading the mod, launching the game, and create a connection to a server.

## [Getting started](https://docs.beammp.com/game/getting-started/)


### What's this for(k)?

BeamMP-Launcher searches for BeamNG.drive installed by steam, this means it won't work with any installation outside of steam.

This fork adds a GamePath variable to Launcher.config, so that the user can specify where their game is installed.

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

### vcpkg built in 

Considering serving vcpkg in this repository as a submodule so end users would not need to install `vcpkg` separately; it is technically feasible and would make builds self-contained, but it adds a very large amount of tracking overhead to the source tree, so for now vcpkg remains an documented dependency and a `-bin`  release may be available as a separate option. Contributions splitting into a self-contained build are welcome.



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
