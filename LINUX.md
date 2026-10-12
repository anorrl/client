# ANOther Roblox Retake Lol - client (LINUX)

## what is this? 

ok so i hate wine so im trying to get this working on linux (my main system)... im using aya and projectpizza as references to get this working...

here's how to get it working

clone `vcpkg` into this repo, and run `./configure_vcpkg.sh` in its directory on terminal

then run one of these

**this is for accservice**

```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="$PWD/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DACC_BUILD=ON -DSTUDIO_BUILD=OFF
```

**this is for studio***
```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="$PWD/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DACC_BUILD=OFF -DSTUDIO_BUILD=ON
```

**after that... (either)**

a new `build` folder will be created. to begin building, go into that new folder and hit `make`. that should start compiling the stuff, if you make any changes in cmake/vcpkg it will automatically update in `make` without having to run the previous cmake commands.

also expect crackhead shit, i've almost lost my mind with the amount of times they write the same include in many different ways for no fucking reason.

progress: studio "works" but only on x11 and uh ribbon is displaying as black...

