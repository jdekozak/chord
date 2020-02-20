## REQUIREMENTS

* Git
* CMake
* C++11 compiler

## INSTALL
```
git clone <repository>
cd chord
mkdir build
cd build
cmake .. && cmake --build . && ctest -V
ls ../3rdparty/chords-db/src/db/guitar/chords/*/*.js | grep -v index | xargs ./loader/src/loadDatabase
```
