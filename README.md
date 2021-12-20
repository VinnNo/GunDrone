# GunDrone
A bullet-hell FPS prototype. So much a prototype that most of the enemies are made of basic shapes and arrows.


* Guns - 4 to be exact, but you can easily make new gun BPs!
* Bullets - Pew, pew! 
* Pickups - Data/EXP, Healing, Energy (for guns), Upgrades (Jumps, health, energy)
* Bouncers - Launch the player in a direction
* Spikes - Ouch
* Energy Shields - Gotta get up close and personal to break them!
* Entities - Characters (walking) and Flyers (flying) with customizable behaviors!
* Enemy Component - Slap one of these on anything with collision, and you've got an enemy!
* PrimativeRotatorComponent – Easily rotates any primitive component
* Wave Spawner – Spawn.. Waves of things.
* Pickup Spawner – Configurable and randomized pickup spawns

## Player Character
The character mechanics are inspired by jump and gun arena plus bullet hell games. Guns are slapped onto springarm sockets with some lag, since there’s no actual character skeletal mesh or animations. This is a decent stand-in for fluidity of motion with minimal effort! The player has has some basic stats to get the gameplay loop kickstarted:
### Jumps
- Max
- Boosted (temporary upgrades)
### Health
- Max
- Boosted (temporary upgrades)
### Energy
- Used by guns, instead of traditional ammo
- Max
- Boosted (temporary upgrades)
### Data / EXP
- For an unimplemented leveling systems
&nbsp; 
## Entity Flyer
This is an entity that makes heavy use of the projectile movement component, with predefined behaviors that can be manipulated inside of derived blueprints, to create unique flying enemies or characters.
&nbsp; 
## Entity Walker
This is a custom Character derived class, used to take advantage of the  ai navigation. Can be plugged into a blackboard or used to create basic behaviors with simple blueprint calls.
&nbsp; 
## Guns
This class is.. A Gun. A derived blueprint of this class just needs a mesh and a defined bullet class to get going. This gun has a few stats to easily make unique guns.
### Fire rate
- Higher numbers = faster fire rates
### Accuracy
- Offsets spread
- Could potentially be upgraded
### Spread 
- How inaccurate the gun starts out as
### Drain Rate
- How much energy the gun uses per shot
### Gravity
-Does the bullets it’s firing have gravity?
&nbsp; 
## Bullets
There’s two bullet blueprints that derive from this class, and this is important to mention. One is an enemy bullet class, intended to damage the player. The other is the player parent, intended to damage anything with a setup enemy component. You can create a new bullet and set the bullet type, though, if you wanna go that route.
### Life Time
- How long the bullet can travel before it dies
- Requires that Use Life Time be checked
### Use Life Time
- Apply the defined Life Time?
### Base Damage
- This is overridden by the gun that spawns the bullet
### Bounce Count
- Track bounce counts and destroy when the max bounces have been reached
### Wait on Destroy
- Should the bullet wait for a specific event to destroy?
- Currently, there’s a bullet that acts as an explosion and waits for the event to finish before being destroyed
### Bullet Type
-None, Player, Enemy, who does this bullet belong to?
&nbsp; 
### Enemy Component
A component that can be added to any class and takes a primitive component with collision to handle damage being dealt. This component also can create a dynamic material to simulate hitflashes with a blueprint defined color, fed into it as a vector.
### Health
- How much bullet damage can this thing take?
### Hitflash Start
- Starting hitflash color
### Hitflash End
- Finishing hitflash color
### Hitflash speed
- How fast the hitflash changes color


