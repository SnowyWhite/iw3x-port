# IW3x-Port

<img src="assets/iw3.ico" align="right" width="100" height="100">

`IW3x-port` is a community-driven project that aims to enhance the [IW4x](https://github.com/iw4x/iw4x-client) multiplayer experience by enabling the export and conversion of various assets from [Call of Duty: Modern Warfare (2007)](https://store.steampowered.com/app/7940/Call_of_Duty_4_Modern_Warfare_2007).

The built-in `ZoneBuilder` tool from `IW4x` can build new assets based on the models, materials, zones and other assets exported by `IW3x-port`.

## How To Use

1. Place the `iw3xport.exe` in your `IW3` installation folder, next to the `iw3mp.exe` and `iw3sp.exe`.
2. Start `IW3x-port`, it will open up the console window of `IW3`.
3. Run the commands for the assets you wish to export.

> [!IMPORTANT]
> Before you can export any asset, it first has to be loaded by the game.
> Use `loadzone <zone_name>` command first, and then use the commands for assets you want to export.
> For example, to export the `M14 Sniper`:
> `loadzone scoutsniper`
> `dumpweapon m14_scoped_silencer_woodland`
> Another Example:
> `loadzone mp_backlot`
> `dumpmaterial headicon_american`

Another approach is to use it in the terminal.
This way, you can specify start parameters, for example to set certain dvars.
Run `iw3xport.exe -stdout` to redirect the output into the terminal.

Example: `iw3xport.exe -stdout +set iw3x_correct_speculars 1 +set iw3x_convert_gsc 1 +set iw3x_smodels_fix_method 2 +set iw3x_add_care_packages 1 +set export_path <your\export\path\mods\airlift> +dumpmap airlift`

## Commands

| Name                | Description                                                         | Example                                               |
|:--------------------|:------------------------------------------------------------------- |:------------------------------------------------------|
`loadzone`            | Loads the specified Zone                                            | `loadzone mp_bog` |
`materialInfoDump`    | Prints all Material Names with SortKey, Packed Flags and StateFlags | `materialInfoDump` (well...) |
`dumpMap`             | Exports the specified Map with help of the commands below           | `dumpMap mp_bog` |
`dumpclipMap_t`       | Exports the ClipMap of a Map                                        | Needs an Example |
`dumpComWorld`        | Exports the ComWorld of a Map                                       | Needs an Example |
`dumpFxEffectDef`     | Exports the Effects of a Map                                        | Needs an Example |
`dumpGameWorld`       | Exports the GameWorld of a Map                                      | Needs an Example |
`dumpGfxImage`        | Exports an Image                                                    | `dumpGfxImage loadscreen_mp_bog` (needs testing...) |
`dumpGfxLightDef`     | Exports the Lights of a map                                         | Needs an Example |
`dumpGfxWorld`        | Exports the GfxWorld of a map                                       | Needs an Example |
`dumpMapEnts`         | Exports the Entities of a map                                       | Needs an Example |
`dumpMaterial`        | Exports the Materials of a map                                      | Loadscreen: `dumpMaterial $levelbriefing` Compass: `dumpMaterial compass_map_bog` |
`dumpTeams`           | Exports all Teams                                                   | `dumpTeams` |
`dumpTechset`         | Exports a TechSet                                                   | Needs an Example |
`dumpRawFile`         | Exports Vision Files, Sun and GSC scripts  of a Map                 | `dumpRawFile sun/map_bog.sun`, `dumpRawFile vision/map_bog.vision`, `dumpRawFile maps/mp/mp_bog.gsc` |
`dumpSound`           | Exports a Sound                                                     | `dumpsound ambient_village_assault_ext0` (needs testing...) |
`dumpWeapon`          | Exports a Weapon                                                    | `dumpweapon m14_scoped_silencer_woodland` | 
`dumpXAnimParts`      | Exports an Animation for a Model                                    | Needs an Example |
`dumpXModel`          | Exports a 3D Model (and all of its dependencies)                    | Needs an Example |


## Additional Dvars

| Name                                  | Description                               |
|:--------------------------------------|:------------------------------------------|
| `export_path`                         | Path where exported assets will be saved. |
| `iw3x_raise_ceiling`                  | Needs a Description |
| `iw3x_add_care_packages`              | Needs a Description |
| `iw3x_correct_speculars`              | Needs a Description |
| `iw3x_smodels_fix_method`             | Needs a Description |
| `iw3x_replace_turrets_with_miniguns`  | Needs a Description |
| `iw3x_extend_culling`                 | Needs a Description |
| `iw3x_convert_gsc`                    | Needs a Description |


## Supported Assets

- Shader Routings
- Textures
- Materials
- Pixel Shaders
- Shader Passes (techniques)
- Passes Sets (techsets)
- Vertex Shaders
- 3D Models (xmodel)
- To Be Continued...