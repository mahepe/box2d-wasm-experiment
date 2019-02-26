# A minimal SDL2 sprite example that compiles to both native binary and WASM

What you have here is a mixture of [this
repo](https://github.com/redblobgames/helloworld-sdl2-opengl-emscripten) and
[this gist](https://gist.github.com/armornick/3434362). This repo strives to
provide the intended functionality with minimal extra hassle to serve as an easy
starting point for you to hack on.

(**A small bonus**: We use smart pointers, so you'll also get a working example
for interfacing them with SDL.)

## Build and run

Build on OSX (instructions for other platforms welcome):
```bash
brew install SDL2 SDL2_image emscripten

# To build the native binaries
make

# To build for web
make www
```

Run the native binaries:
```
bin/main
```

Run WASM:
```
cd www
python -m http.server 8000
```
Then visit http://localhost:8000
