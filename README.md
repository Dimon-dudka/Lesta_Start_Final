# Final project of Lesta Games Start program

This project is the implementation of the final task on the program "Lesta Games Start". This is a first-person multiplayer shooter inspired by the games "Wolfenstein 3D" and "Serious Sam".

## Technology stack:
- C++ 17
- Unreal Engine 5.3

## Character control
- Character moving - WASD keys
- Lazer weapon choice - key 1 or mouse scroll
- Grenade weapon choice - key 2 or mouse scroll
- Reloading - key R

## Implemented functionality:
### Weapon functionality:
- Laser weapon - a ray coming out of the center of the player's screen, which deals damage to the first actor hit by the ray.
- Grenade weapon - when the shot button is pressed, the player begins to accumulate a "charge", and when the shot button is released, damage is inflicted on all targets within a certain radius from the player.
- Limited ammunition and reloading.
- Weapon choice.

### Actors:
#### Player's charachter:
- Player's character - the main character the player plays for. If a player's character's health drops to 0 or below, they will "die".

#### Enemies:
- Basic enemy - marked in green. A basic enemy always stands in the same place and fires a laser at the player as they approach.
- Guardian enemy - marked in orange. Patrols a given area. If it sees a player, it runs to the location where the player was last seen. If the player disappears from sight, it returns along the path it has traveled. When it approaches the player, it starts firing lasers at the player.
- Bomber enemy - marked in red. As soon as the player sees him, he immediately starts running towards him. He explodes when he approaches.

#### Another actors:
- Box - a cube that will be destroyed when its health points reach zero.
- Spawner - an area where an actor appears when the player interacts with it.
- Door - when the actor approaches the area in front of the door, it begins to open. As soon as the actor leaves this area, the door automatically closes.

### General functionality:
- Health component - implements the management of health points for characters.
- Print HP component - implements the print of health over the actor and its direction towards the local user.
- Destroy Animation - animation of the destruction of actors.
- Basic HUD - output information about the player's health, weapon type, and recharge status.
- The ability to play in multiplayer mode.
- Game restart - the game only restarts when all the players' characters on the current level have died.

## Project features
- There is a short delay in the multiplayer mode. It takes about 5-7 seconds to rest after a hard game before restart.
- Guardian enemy can pass through walls because it can detect the player from different angles and should not get stuck in walls.
- Guardian enemy may incorrectly calculate the vector due to floating point numbers and miss the target point.
- The enemy always chooses the nearest player to attack or pursue.
- Inability to fire or switch weapons while reloading.
- Enemy actor settings require care. An actor cannot be both a grenade (exploding enemy) and a guard at the same time. 
- To activate enemys following component needs in grenade component set "IsGrenade" flag to true