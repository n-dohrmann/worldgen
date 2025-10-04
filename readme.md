# worldgen image rendering

a worldgen project in the style of Dwarf Fortress ascii art.

currently this project is just getting started. `src/main.c` is just for getting familiar with stb.

## Dependencies
- stb [header files](https://github.com/nothings/stb/tree/master) for image rendering
- cmake 4.0+

## Tilesets
- Hanuman's 12x12 Curses Tileset, found [here](https://dffd.bay12games.com/file.php?id=318)

## Building
- download a tileset and provide its path in the `TILESET_PATH` definition in
`src/main.c`.
- Then,
```sh
cmake -S . -B build
cmake --build build
./build/apps/worldgen_app
```
