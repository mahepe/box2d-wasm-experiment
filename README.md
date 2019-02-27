# A minimal SDL2 sprite example that compiles to both native binary and WASM

What you have here is a mixture of [this
repo](https://github.com/redblobgames/helloworld-sdl2-opengl-emscripten) and
[this gist](https://gist.github.com/armornick/3434362). This repo strives to
provide the intended functionality with minimal extra hassle to serve as an easy
starting point for you to hack on.

(**A small bonus**: We use smart pointers, so you'll also get a working example
for interfacing them with SDL.)

## Build and run

### Install deps

Build on OSX (instructions for other platforms welcome PRs):
```bash
brew install SDL2 SDL2_image emscripten
```

### Build and run native binaries

```
cd build
cmake ..
make
./main
```

### Build and run WASM

```
cd www
emcmake cmake ..
emmake make
python -m http.server 8000
``` 
Then visit http://localhost:8000

## Including a library

1. Put the includes to `./include`
2. Put the library compiled with a native compiler  to `./lib/native`
3. Put the library compiled with emscripten to `./lib/web`
4. Modify `CMakeLists.txt` (There is a commented out example how to include Box2D.)
