# A Box2D SDL2 WASM experiment

A small example of using Box2D and SDL2 in a WASM project. See it [in
action](https://mahepe.github.io/box2d-wasm-experiment/), also see my [blog
post](https://mahepe.github.io/home/posts/wasm-experiment) about it.

## Preparing

Install deps:
```bash
brew install SDL2 SDL2_image emscripten
```

Clone this repo and the submodules.
```
git clone --recursive git@github.com:mahepe/box2d-wasm-experiment.git
```

Build native Box2D:
```
# Run in ./lib/native/
cmake Box2D-cmake/
make
```

Build WASM Box2D:
```
# Run in ./lib/web/
emcmake cmake Box2D-cmake/
emmake make
```

## Build and run

Native
```
mkdir build
cd build
cmake ..
make
./main
```

WASM
```
mkdir www
cd www
emcmake cmake ..
emmake make
python -m http.server 8000
``` 
Then visit http://localhost:8000
