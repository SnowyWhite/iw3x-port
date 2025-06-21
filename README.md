# IW3x-Port

`IW3x-port` is a community-driven project that enhances the [IW4x](https://github.com/iw4x/iw4x-client) multiplayer experience by making it possible to export and convert assets from [Call of Duty: Modern Warfare (2007)](https://store.steampowered.com/app/7940/Call_of_Duty_4_Modern_Warfare_2007).

The built-in `ZoneBuilder` tool of `IW4x` can use the models, materials, zones, and other content exported by `IW3x-port` to convert them into new assets for `IW4x`.

## How To Use

1. Place the `iw3xport.exe` in your `IW3` installation folder, next to the `iw3mp.exe` and `iw3sp.exe`.
2. Start `IW3x-port`, it will open up the console window of `IW3`.
3. Export the assets of your choice by running the commands described below.

![image](https://github.com/user-attachments/assets/bfca7299-aa09-486e-9d19-74918049c231)

### CLI Mode

Another approach is to run `iw3xport.exe` directly in the terminal.
This way, you can specify start parameters, for example to set certain dvars.
Simply open a terminal in the game directoy and run `iw3xport.exe -stdout <commands>`.

![image](https://github.com/user-attachments/assets/9432b402-d2db-495e-bba4-bebaf42772b1)

### Examples

> [!IMPORTANT]
> Before you can export any asset, it first has to be loaded by the game.  
> For that, use the `loadzone <zone_name>` command with the name of a zone that contains the asset.  
> Then you can use the commands for the assets you want to export.

To export the `M14 Sniper`:

```cmd
loadzone scoutsniper
dumpweapon m14_scoped_silencer_woodland
```

To export the head icon of the `Marines Team`:

```cmd
loadzone mp_backlot
dumpmaterial headicon_american
```

Example of how to use it in the terminal, with additional parameters:

```cmd
iw3xport.exe -stdout \
+set iw3x_correct_speculars 1 \
+set iw3x_convert_gsc 1 \
+set iw3x_smodels_fix_method 2 \
+set iw3x_add_care_packages 1 \
+set export_path mods\airlift \
+dumpmap airlift
```

## Commands

| Name                | Description                                                         | Example                                               |
|:--------------------|:------------------------------------------------------------------- |:------------------------------------------------------|
`loadzone`            | Loads the specified Zone                                            | `loadzone mp_bog` |
`materialInfoDump`    | Prints all Material Names with SortKey, Packed Flags and StateFlags | `materialInfoDump` |
`dumpMap`             | Exports the specified Map with help of the commands below           | `dumpMap mp_bog` |
`dumpclipMap_t`       | Exports the ClipMap of a Map                                        | Needs an Example |
`dumpComWorld`        | Exports the ComWorld of a Map                                       | Needs an Example |
`dumpFxEffectDef`     | Exports the Effects of a Map                                        | Needs an Example |
`dumpGameWorld`       | Exports the GameWorld of a Map                                      | Needs an Example |
`dumpGfxImage`        | Exports an Image                                                    | `dumpGfxImage loadscreen_mp_bog` (needs testing...) |
`dumpGfxLightDef`     | Exports the Lights of a map                                         | Needs an Example |
`dumpGfxWorld`        | Exports the GfxWorld of a map                                       | Needs an Example |
`dumpMapEnts`         | Exports the Entities of a map                                       | Needs an Example |
`dumpMaterial`        | Exports the Materials of a map                                      | Loadscreen: `dumpMaterial $levelbriefing`<br>Compass: `dumpMaterial compass_map_bog` |
`dumpTeams`           | Exports all Teams                                                   | `dumpTeams` |
`dumpTechset`         | Exports a TechSet                                                   | Needs an Example |
`dumpRawFile`         | Exports Vision Files, Sun and GSC scripts  of a Map                 | `dumpRawFile sun/map_bog.sun`<br>`dumpRawFile vision/map_bog.vision`<br>`dumpRawFile maps/mp/mp_bog.gsc` |
`dumpSound`           | Exports a Sound                                                     | `dumpsound ambient_village_assault_ext0` (needs testing...) |
`dumpWeapon`          | Exports a Weapon                                                    | `dumpweapon m14_scoped_silencer_woodland` |
`dumpXAnimParts`      | Exports an Animation for a Model                                    | Needs an Example |
`dumpXModel`          | Exports a 3D Model (and all of its dependencies)                    | Needs an Example |


## Additional Dvars

> [!NOTE]
> Each dvar except `export_path` and `iw3x_smodels_fix_method` needs to be enabled by passing `1` to it.  
> Example: `iw3x_raise_ceiling 1`

| Name                                  | Description                                                                                                                                                                                                |
| :------------------------------------ | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `export_path <path>`                  | Path where exported assets will be saved.                                                                                                                                                                  |
| `iw3x_raise_ceiling`                  | Raises the map’s ceiling collision so that high-flying killstreaks (like the UAV or AC-130) don’t fly above it and prevent stinger lock-on.                                                                |
| `iw3x_add_care_packages`              | Adds care package models and collisions to the map, otherwise they can't be delivered in IW4.                                                                                                              |
| `iw3x_correct_speculars`              | Overwrites IW3 speculars with a different color grading that's better suited to IW4's shader model.                                                                                                        |
| `iw3x_smodels_fix_method <1,2>`       | How incompatible IW3 static models will be handled:<br> `1` - Swapped for an empty model<br> `2` - Moved to script_model in entities  so they render as scene entities<br>Otherwise, nothing will be done. |
| `iw3x_replace_turrets_with_miniguns`  | Replaces IW3 turrets with IW4 miniguns.                                                                                                                                                                    |
| `iw3x_extend_culling`                 | Increases the culling distance for static models so that they are visible at greater distances.                                                                                                            |
| `iw3x_convert_gsc`                    | Attempts to automatically convert IW3 GSC scripts & createFX to the IW4 format. When it's disabled, scripts are left as is.                                                                                |


## Asset types

| Number | Name                     | Description                                                                                                                                                                                                                        | Supported |
| :----- | :----------------------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :-------- |
| 0      | XModelPieces             |                                                                                                                                                                                                                                    |           |
| 1      | PhysPreset               | A physical preset is used with XModels to say how a piece of something should behave when fired at or near an explosion, how bouncy it is, etc... Physical properties                                                              | ✔         |
| 2      | XAnimParts               | Animations for a set of bones, often used with XModels too. Somewhat esoteric structure with a looot of data, from which the game reconstructs live animations                                                                     | ✔         |
| 3      | XModel                   | A XModel is an ensemble of surfaces, materials, collisions, skeleton and physical properties that constitute a model for the game - with up to 4 LODs                                                                              | ✔         |
| 4      | Material                 | Materials are sets of textures and parameters to apply said textures onto a surface, aswell as reference to a technique set (rendering passes informations)                                                                        | ✔         |
| 5      | Technique Set            | A technique set is an ensemble of up to 48 techniques (34 for IW3) describing different render passes the game should combine to draw a material.                                                                                  | ✔         |
| 6      | Image                    | While image data is streamed, each image has a corresponding GfxImage Asset which contains metadata about the texture                                                                                                              |           |
| 7      | Sound                    | "Sounds" are in reality sound alias lists, which mean they contain one or more definitions for sounds that should be played (distance, mixing, pitch, etc)                                                                         |           |
| 8      | Sound Curve              | Simple definition files used by sound aliases to define how the game should behave over a distance                                                                                                                                 |           |
| 9      | Loaded Sound             | A wrapper for a sound that was fully loaded in memory by [Miles Sound System](https://www.radgametools.com/miles.htm)                                                                                                              | ✔         |
| 10     | Clipmap (Singleplayer)   | A clipmap contains all the collision from the original [BSP](https://en.wikipedia.org/wiki/Binary_space_partitioning) a level was built from                                                                                       | ✔         |
| 11     | Clipmap Pvs              |                                                                                                                                                                                                                                    | ✔         |
| 12     | Comworld                 | "Common world" contains pretty much only light positions and definitions, for ambient and omnidirectional lights in the level                                                                                                      | ✔         |
| 13     | Gameworld (Singleplayer) | A mostly empty but mandatory preallocated structure to hold the dynamic glass data from FxWorld once the game started - always empty in map files                                                                                  | ✔         |
| 14     | Gameworld (Multiplayer)  |                                                                                                                                                                                                                                    | ✔         |
| 15     | Map Entities             | "MapEnts" contain a single very long string inherited from Q3 which lists all dynamic and scripted entities of the level, and information about the map triggers for gamemodes                                                     | ✔         |
| 16     | GfxWorld                 | "Graphics world" contains all the graphical, visual part from the original BSP a level was built from - this data is linked and sometimes reminiscent to the ClipMap data. It contains most of everything that constitutes a level | ✔         |
| 17     | Light Definition         | Parameters that define a light (color, power, direction)                                                                                                                                                                           |           |
| 18     | Ui Map                   |                                                                                                                                                                                                                                    |           |
| 19     | Font                     | A font contains materials and the glyph table necessary to draw characters on screen                                                                                                                                               |           |
| 20     | Menulist                 | A transient asset that usually doesn't live in memory for too long (?) and contains a list of menus to load, or to unload. E.g. 'ingame menus', 'main menu', etc                                                                   |           |
| 21     | Menu                     | Each menu displayed in the game exists as a singular Menu asset in memory, which itself can reference other menus or share data with other menus from the same zone                                                                |           |
| 22     | Localized Entry          | Smallest struct in the game: a localized entry is just a Key and a Translated value. 'MPUI_BOG' => 'Bog' - and that's one whole localized entry asset                                                                              |           |
| 23     | Weapon                   | Weapons are the biggest structs in the game besides GfxWorld and Clipmap, and contain every single thing a weapon need to work in the game - materials, sounds, animations, ...                                                    | ✔         |
| 24     | SndDriver Globals        |                                                                                                                                                                                                                                    |           |
| 25     | Fx                       | Special effects, used on the map and in weapons and everything else - usually references sounds, materials, etc                                                                                                                    |           |
| 26     | Impact Fx                | A list of FX                                                                                                                                                                                                                       |           |
| 27     | Ai Type                  |                                                                                                                                                                                                                                    |           |
| 28     | Mp Type                  | Might be unused in this form - probably references mptype CSVs during building a level, a mptype being a set of characters to build into a map                                                                                     |           |
| 29     | Character                | Same as Mp Type - usually present as a rawfile (GSC Script) in the final game, this might have been used during level building only                                                                                                |           |
| 30     | XModelAlias              | Same as Character                                                                                                                                                                                                                  |           |
| 31     | Rawfile                  | Rawfiles are all files the game will do case-by-case parsing for - they can be scripts, vision files, anything that usually involve a form of Com_Parse                                                                            | ✔         |
| 32     | Stringtable              |                                                                                                                                                                                                                                    |           |
| 33     | Count                    |                                                                                                                                                                                                                                    |           |
| 33     | String                   |                                                                                                                                                                                                                                    |           |
| 34     | Assetlist                |                                                                                                                                                                                                                                    |           |
