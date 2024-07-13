# Rubik's cube
This project uses OpenGL 4.1 and was written in C++.

## Required Libraries
- glfw (Download from [this page](https://www.glfw.org))
- glad (Download from [this page](https://glad.dav1d.de). Select core profile and check the "Generate a loader" option. After downloading, move glad.c into the src directory)
- glm (Download from [this page](https://github.com/g-truc/glm/tags))

Please edit the installation paths in Makefile as well.

## Camera mode
The project starts in camera mode when you run it. This mode allows you to move around freely. To start solving the cube, press `Command` + `1` to change to solving mode.

### Controls
- `WASD` keys to move forward, backward, and side-to-side
- `Space` to fly upwards, and `Control` to fly downwards

## Solving mode
This mode allows you to perform cube rotations and turns. To move the camera, press `Command` + `1` to change to camera mode.

### Controls
The following keys turn the faces:
- `r` turns the right face
- `l` turns the left face
- `u` turns the top face
- `d` turns the bottom face
- `f` turns the front face
- `b` turns the back face
- `m` turns the middle slice
The face turns are done in a clockwise direction. To turn the faces counterclockwise, hold `Shift` while turning. Hold `Option` while turning to perform a two-slice move.

The following keys rotate the cube:
- `⬆` and `⬇` rotate the cube around the X axis.
- `⬅` and `➡️` rotate the cube around the Y axis.
- `Shift` + `⬅` and `Shift` + `➡️` rotate the cube around the Z axis.

To scramble the cube, press `Command` + `s`.

Press `Esc` to release the cursor

## Running the project
```sh
# build the project
make
# build the project and run it
make run 
```

## Notes
- If the cube is rendered on the bottom left of the screen, go into fullscreen mode and exit fullscreen mode to fix it.
- This project was inspired by Daniel Jackson's [Rubik's cube](https://www.youtube.com/watch?v=2DD-EOMdhX0&pp=ygUTcnViaWsncyBjdWJlIG9wZW5nbA%3D%3D)
