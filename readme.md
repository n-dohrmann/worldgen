# worldgen image rendering

a worldgen project in the style of Dwarf Fortress ascii art.

currently this project is just getting started. `src/main.c` is just for getting familiar with stb.

## Dependencies
- stb [header files](https://github.com/nothings/stb/tree/master) for image rendering
- cmake

## Tilesets
- Hanuman's 12x12 Curses Tileset, found [here](https://dffd.bay12games.com/file.php?id=318)


## Building
- download a tileset and provide its path
- Then,
```sh
mkdir build
cd build
cmake ..
make
./bin/worldgen
```
