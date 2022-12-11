[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![linux](https://github.com/jd28/arclight/actions/workflows/linux.yml/badge.svg)](https://github.com/jd28/arclight/actions?query=workflow%3Alinux)
[![macos](https://github.com/jd28/arclight/actions/workflows/macos.yml/badge.svg)](https://github.com/jd28/arclight/actions?query=workflow%3Amacos)
[![windows](https://github.com/jd28/arclight/actions/workflows/windows.yml/badge.svg)](https://github.com/jd28/arclight/actions?query=workflow%3Awindows)

# Arclight

### The Goal

This project is to ask: if one was to start today, what would the NWN Toolset be?  What technologies would it use, what would be its features, its goals, and philosophy.

It accepts from the beginning, maybe there *shouldn't* even be a Toolset and that functionality should be just one other mode of the game (i.e. a devclient.exe, analogous to the dmclient.exe).  But that's something only [Beamdog](https://www.beamdog.com) could do.

### The Philosophy

Unlike other projects of this sort, instead of starting work on some massive project and filling in bits and pieces, this starts with small discrete utilities that have the modern features one expects and the widgets/functionality of which can later be welded together to create something that is more than the sum of its parts.

### Platforms

- Linux: The latest LTS of Ubuntu, roughly
- macOS:
- Windows: Windows 10 and above, Visual Studio 2019 and above

## erfherder

erfherder is an editor for [BioWare's ERF File Format](docs/bioware_aurora_engine_file_formats/Bioware_Aurora_ERF_Format.pdf).  It's features include:

* Everything in nwhak.exe
* Drag and Drop support.
* Very simple, but effective Fuzzy Search filtering.

![erfherder](screenshots/erfherder-2022-03-27.gif)

## Acknowledgements, Credits, & Inspirations

- [BioWare](https://bioware.com), [Beamdog](https://beamdog.com) - The game itself
- [Qt](https://www.qt.io) - Gui
- [vcpkg](https://github.com/microsoft/vcpkg) - Package Management
- [neveredit](https://github.com/sumpfork/neveredit) - Inspiration
- [NWNExplorer](https://github.com/virusman/nwnexplorer) - Inspiration
- [NWN Java 3d Model Viewer](https://neverwintervault.org/project/nwn1/other/nwn-java-3d-model-viewer) - Inspiration
