# Olafur

[Link to the video (YouTube)](https://www.youtube.com/watch?v=WFRCX2qkSP4)  
[Link to the download page (Itch.io)](https://lycorisbellua.itch.io/olafur)  

![Game](./screenshots/main.png)

## Explanation

*Olafur* is a grid-based pseudo 3D game running solely on the CPU and written 
in C. It means that a 2D map is translated into 3D using a technique called 
raycasting. Rays are casted from the camera until walls are hit, which tells us 
how far away they are. Knowing the distance allows us to draw the walls: far 
makes them small, and close makes them big. An example of such a game is 
Wolfenstein 3D.  

![Raycasting](./screenshots/raycasting.png)

The libraries in use are:
- the C standard library,
- OpenGL (Open Graphics Library) for the graphics,
- GLFW (OpenGL Framework) for the windowing and input handling,
- and OpenAL (Open Audio Library) for the audio.

I intend to specialize in Unreal Engine, so why was I even working on a low 
level project? Because it's fun. Moving on... No seriously, I've always wanted 
to work on such a project, so it was a joy to nibble at this from time to time. 
The aim wasn't to make it a commercial project. It was to experiment and 
scratch my low level itch.  

Now, I am ready to move on from this one project. That is not to say I won't 
ever go back to low level dev someday. I might embark on another raycasting 
project with more complex techniques, inspired from Doom or even Duke Nukem. 
I'm pretty stoked about discovering other techniques, although this is not part 
of my current priorities. In any case, this decision led to a name change. The 
project used to be called "Cimmerian" and is now "Olafur", to be the same as 
the demo level. This leaves the name of "Cimmerian" available for this 
potential future project.  

## Levels

Give a specific \*.map file as argument to the program, or don't give any to 
default to "assets/maps/olafur/mus.map".  

- assets/maps/test/\*:  
The Test level is where I develop features.  

- assets/maps/olafur/\* (recommended entry "mus.map"):  
The Olafur level is a museum where each painting leads to another mini-world. 
It has been designed by Hellchauffo 
([Figma](https://www.figma.com/proto/UshKwYvv5vY0WgejO4WXR2/Workflow?node-id=82-974) 
and [GitHub](https://github.com/Lucie-Kin)), a friend of mine. She is behind 
the concept, the map layouts, the sprites, the textures and the choice of music 
tracks. This has been quite the undertaking and I thank her for this work. This 
level was designed in the context of a school project we worked on together, a 
grid-based pseudo 3D game as well, and she allowed me to keep the level for my 
personal project. This comes in quite handy, as I now have a level to showcase 
what the game is capable of. As for my contribution to the level, I've picked 
the elevator music, the sound effects, and added an easter egg accessible from 
the elevator (and through your keyboard).  

## Commands

Move forward: W or UP arrow key  
Move backward: S or DOWN arrow key  
Rotate left: A or LEFT arrow key  
Rotate right: D or RIGHT arrow key  
Move left: Q  
Move right: E  
Speed: Press SHIFT to run, release to walk  
Zoom minimap in and out: Scroll  

Quit: X window button, Alt+F4 or ESC  
Backspace: Reset player's position and rotation  
Toggle fullscreen: F11  
Toggle FPS counter: F12  

## More Screenshots

![Level DGN](./screenshots/dgn.png)
![Level AFG](./screenshots/afg.png)
![Level PUL](./screenshots/pul.png)
![Level BOX](./screenshots/box.png)
![Level BRN](./screenshots/brn.png)
![Level HOL](./screenshots/hol.png)
![Level ELV](./screenshots/elv.png)
![Level COR](./screenshots/cor.png)

