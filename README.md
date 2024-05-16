## Simple DPLL sat solver implementation.

### Build ###
```
    mkdir build
    cmake -B build -DCMAKE_BUILD_TYPE=release
    make -C build
```

### Testing ###
Testing is implemented using **picosat** sat solver and **python** scripts, so they are needed to be installed.
Once they are, just simply run this if you are linux user:
``` 
python3 test.py
```
or this if you are Windows user:
```
python test.py 
```

#### NB
If build directory is different from "build", script should be changed.
