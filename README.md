[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![linux](https://github.com/jd28/arclight/actions/workflows/linux.yml/badge.svg)](https://github.com/jd28/arclight/actions?query=workflow%3Alinux)
[![macos](https://github.com/jd28/arclight/actions/workflows/macos.yml/badge.svg)](https://github.com/jd28/arclight/actions?query=workflow%3Amacos)
[![windows](https://github.com/jd28/arclight/actions/workflows/windows.yml/badge.svg)](https://github.com/jd28/arclight/actions?query=workflow%3Awindows)

# Arclight Tools

Tools subproject of something referred to as Arclight

**This is beta software, please, keep backups.**

### Platforms

- Linux: The latest LTS of Ubuntu, roughly
- macOS:
- Windows: Windows 10 and above, Visual Studio 2019 seems to be the latest Qt supported version

## erfherder
**status:** beta

erfherder is an editor for [BioWare's ERF File Format](docs/bioware_aurora_engine_file_formats/Bioware_Aurora_ERF_Format.pdf).  It's features include:

* Everything in nwhak.exe
* Drag and Drop support.
* Very simple, but effective Fuzzy Search filtering.

![erfherder](screenshots/erfherder-2022-03-27.gif)

## texview
**status:** beta

A super mid and basic texture gallery viewer for Bioware DDS, Standard DDS, TGA, and PLT texures.  It can open Key, Erf, and Zip containers.  Screenshot is using NWN:EE main key file circa 36-2.

![texview](screenshots/texi-2024-04-09.gif)

## dlg
**status:** alpha

An editor for DLG conversation files and rollnw DLG json files.  A project of this level of complexity requires a fair bit of polish.. It has most toolset features, the main missing things is:

* Since it exists outside of a module.. completion on module scripts and journal integration.
* Inserting action strings (and corresponding highlights)
* Loading a TLK file and handling dialog nodes with string references.

![dlg](screenshots/dlg-2024-05-01.gif)

## Acknowledgements, Credits, & Inspirations

- [BioWare](https://bioware.com), [Beamdog](https://beamdog.com) - The game itself
- [Qt](https://www.qt.io) - Gui
- [neveredit](https://github.com/sumpfork/neveredit) - Inspiration
- [NWNExplorer](https://github.com/virusman/nwnexplorer) - Inspiration
- [NWN Java 3d Model Viewer](https://neverwintervault.org/project/nwn1/other/nwn-java-3d-model-viewer) - Inspiration
