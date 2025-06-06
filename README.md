# Cimmerian

*Cimmerian* is a pseudo 3D game, or raycasting game, written in C89. It means 
that a 2D map is translated into 3D. Examples of such games are Wolfenstein 3D 
or Doom.  

![Raycasting](./screenshots/screenshot_raycasting.png)

The libraries in use are:
- naturally the C standard library,
- OpenGL (Open Graphics Library) for the graphics,
- GLFW (OpenGL Framework) for the windowing and input handling,
- and OpenAL (Open Audio Library) for the audio.

I've decided to specialize in Unreal Engine, so why am I even working on a low 
level project? Because it's fun. Moving on... No seriously, I've always wanted 
to work on such a project, so I'm happy to nibble at this from time to time. 
The aim isn't to make it a commercial project, although that's not out of the 
question neither. The purpose is to experiment and scratch my low level itch.  

## Levels

- maps/bonus/test/\*:
The Test level is where I develop features.  

- maps/bonus/olafur/\* (recommended entry "menu2.map"):
The Olafur level is a museum where each painting leads to another mini-world. 
Besides the "Cimmerian" mini-world, which stems from my test level, the choice 
of elevator music, and an easter egg accessible from the elevator, the level 
has been entirely designed by Hellchauffo ([Figma](https://www.figma.com/proto/
UshKwYvv5vY0WgejO4WXR2/Workflow?node-id=82-974) and [GitHub](https://github.com/
Lucie-Kin)), a friend of mine. She is behind the concept, the map layouts, the 
sprites, the textures, the presence of music and the choice of tracks. This has 
been quite the undertaking and I thank her for this work.  

## Commands

Move forward: W or UP arrow key  
Move backward: S or DOWN arrow key  
Rotate left: A or LEFT arrow key  
Rotate right: D or RIGHT arrow key  
Move left: Q  
Move right: E  
Speed: Press SHIFT to run, release to walk  

Quit: ESC  
Backspace: Reset player's position and rotation  
Toggle fullscreen: F11  
Toggle FPS counter: F12  
