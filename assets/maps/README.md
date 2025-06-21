# How to format a map file

- It doesn't have to be in the assets/maps/ folder. It can be located anywhere on the 
system.
- It has the \*.map extension.
- Images are PNG and sounds are MP3.
- Within the file, the variables come before the maps.
- The variables can be ordered in whichever way. They don't have to respect any 
order.
- Anything written to the right of hash sign ('#') is a comment.
- You can use "null" for an image reference to express "no image".
- The world origin is on the top left corner, and the X axis extends to the 
right, and the Y axis extends downwards.

## Variables

- **IMG**: Give it one JSON file describing images and animations. 
You can have more than one IMG variable. Go explore the img/ folder to 
understand what is expected in this JSON file. Then, in the map file, use the 
image ID indicated in the JSON whenever you want to refer to an image. If you 
use the image path as a reference instead, the game will not know that you mean 
"this image which was in the JSON file I've given you". Such an image would be 
fetched as a still image, and if the JSON file already mentioned it, then the 
image would be present twice in game memory.

- **NO/SO/WE/EA**: North, South, West, and East respectively. "NO" is the same 
as "1NO". The digit represents the wall type, which can be anything from 1 to 
9\. A wall type needs all four cardinals, and the argument to a cardinal is the 
image reference. These variables are the only mandatory ones.

- **G**: The reference to the goal texture, indicated as 'G' in the wall map. 
The map doesn't need a goal. A goal can be crossed if the required amount of 
collectibles has been gathered. If you don't specify a number as a second 
argument, after the texture, then this amount will be 0.

- **Digit + F**: The reference to the floor image for a given type. The floor 
types are used in the floor map.

- **Digit + C**: The reference to the ceiling image for a given type. The 
ceiling types are used in the ceiling map.

- **F**: The color for the floor, that is to say the bottom half of the frame. 
Give it either in RGB (e.g.: 255 255 255) or in hex code (e.g. FFFFFF).

- **C**: The color for the ceiling, that is to say the top half of the frame. 
Give it either in RGB (e.g.: 255 255 255) or in hex code (e.g. FFFFFF).

- **SK**: The reference to the skybox image. Its dimensions need to be based on 
the frame's size. For example, if the frame is 640x360 pixels, the skybox needs 
to have the same height (360) and four times the width (2560).

- **SP**: A sprite. An example is `SP 4.5 20.5 my_sprite collision`. This means 
that the sprite of ID "my_sprite" is located at the position (4.5, 20.5) and 
collides with the player. The tags are optional, that is to say that the sprite 
can be without collision. Another tag which exists is "collectible", and the 
tags can be listed in any order. There can be more than one SP variable.

- **S_MUSIC**: The music.

- **S_AMBIENCE**: The ambience sound.

- **S_WIN**: The sound effect of the positive game over screen.

- **S_LOSE**: The sound effect of the negative game over screen.

- **S_COLLEC**: The sound effect of picking up a collectible item.

- **D**: Door. Indicate its X and Y position, the closed texture, the open 
texture, and an optional orientation. If not specified, the door will be drawn 
like a wall, as a cube. Otherwise, specify "HMID" for "horizontal middle" to 
have the door drawn in the middle of the cell on the X axis, "VMID" for 
"vertical middle" to have it drawn on the Y axis, or a cardinal direction (NO, 
SO, WE or EA) to have it drawn on a specific cardinal face.

- **PT**: Portal. A portal can only be placed where a wall could be, as if on a 
grid. It cannot be in the middle of a cell. Crossing a portal teleports the 
player somewhere else, be it on the same map or another. An example is `PT WE 
31 3 tex_closed tex_open some.map EA 0 4`. This means that a portal is on the 
western wall of the (31, 3) cell, and leads to "some.map" on the eastern wall 
of the (0, 4) cell. The destination cardinal and XY position are optional, if 
they're not given then the start cardinal and XY position of the destination 
map will be used instead. Also, this portal uses the "tex_closed" image, and 
when the player is close enough for the portal to open, "tex_open" is used. 
There can be more than one PT variable.

## Maps

- The first map is mandatory. This is the wall map.
- The second map is the floor map.
- The third map is the ceiling map.
- Separate each map with an empty line. If you want a map to contain an empty 
line, fill it with spaces.
- The maps can contain spaces and digits. A digit refers to a texture type, 
with '0' meaning "none".
- In the wall map, zero is the ground, and anything else is a wall.
- Besides digits, the wall map can have 'D' for "door" and 'G' for "goal".
