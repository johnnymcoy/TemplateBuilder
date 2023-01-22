# TemplateBuilder
Developed with Unreal Engine 4

# Latest
+ Just added in test Dialouge to get a mission started
+ Added the Dialouge interface to the bp for the player and aI 
+ Added a missionComponent (temp bp) to add to a player to keep track of mission progress

+ NEXT 
+ Add in Dialogue System properly - Almost done
+ Finish added any more parts in c++ - check synty project
+ add in display name to c++, and add the display name from simple shooter to have in multiplayer 
+ Dialogue Component - Has displayName, Icon, Name, and all function for dialouge - also a physical widget icon tooltip "talk to me"
+ check old project to see how i implemented the dialogue, if check, 
  https://gitlab.com/NotYetGames/DlgSystem/-/wikis/Using-with-C++#tldr-c-short-integration-guide
+ 
+ figure out Mission Component 
+ Start with Get Mission from AI, then go to car 
+ then remove all unused menu Items or set to disabled
+ build first level and try as standalone demo
+ then start writing out list of whats next 
+ Create start Mission, Go to car, Find Keys, then Mission complete 

Features:

## Done:
+ Add in selection bar to main menu
+ Add in Custom Physical Animation to CustomALS Character
+ Add in gun impulse to the physical Anim part
+ BUG- Collision on the custom physical animation doesn't really work when being shot

When taking damage-> health left should be the Alpha of how much the customPhysicalAnimation is applied
Dismemberment component 

AI - -

Add in new animations

Add in reaction Animations?
Fixing up References in Character BP
Trying to make it more flexable and usable if i want to use it in another project

Changed EQS nearest player + find all players from Player_bp to Player controller, need to test if works

## MVP:
#### _Minimum viable product_
What is needed to turn this prototype into a working demo
#### Horde Mode
- Game Mode
- Scoring
- Waves
- Enemy Spawning
- Zombie AI
- Connect Menu UI

## To Add:
Component

Character Component - Base class
- ActorComponent-> but with some extra defaults
- Owning Pawn
- Owning Controller
- Owning Player Controller - see shooting component

Visual Effects Spawner 
- Ballistics VFX example 
- Projectile weapons


Pawn Sensing in Character

Testing Map
Similar to ALS testing map 
also has all floors 


## QOL:
#### _Quality of Life_
+ 
+ 
+ 
+ Silenced Weapons, Weapons need to make Noise for AI 
+ List of things that make noise 
+ 

## AI:

+ Shooting Component needs BP function calls, so AI can tell Blackboard they're reloading etc.
+ Add TargetActor to Array, 
+ Forloop over them and find which is closest, or if bTakingDamage
+ Melee Attack in Zombie AI (Refactor it for use with any character)
+ Zombie AI always chooses Player first, change it to Any enemy
+ Go through ALS with interaction for more Insperation for Human AI
+ 
Human
+ Fix When AI is low health
+ add,  when Limb is detached 
+ List of events that will turn a human hostile,
+ What will turn them back 
+ Push back animation
+ fix up decorators and if has weapon task 

Character
+ Fix Death Ragdoll, currently not replicated
+ Physical Animation component not replicating
+ Pawn Sensing
+ Pawn Sense Stimuli 

## Working on:
New Main Menu UI.
+ Use more Blueprint controls for UI, C++ is too bloated
+ On button click, play animation, once completed, use menu swapper
-Delete menu buttons 
-Remove C++ extras, better to have it in BP
Zombie AI
+ Add in Animations
+ Fix up Behavior tree 
+ Add in more function (Walking idle, follow other zombie)


Controller support for Menu.
Aim Assist

Tweaking needed:
Shield Regen 
Health Pickups (Replicate)

Todo: 
Refactor WeaponInterface
Add in all weapons 
Gun FX
Fix up Physical Animation into Seperate component - saves memory
Add in all animation from animation packs
Fix up proper rig, instead of current floating one



Finish enemy AI 
HUD 
Gameover screen 
Pause menu to be the same as Main
2 Simple Horde mode Levels
2 Campain Levels
2 Multiplayer levels, can just be campain levels



Bugs:
Server Player, when starting the game will not have the bottom half of his body replicated.
Client doesn't receive Overlay state when picking up weapon
Full Auto weapons only get the accuracy calculated once, add in a timer and have it stop when stopShooting is called

When in slow-mo, the animations seem to act slightly different, i.e. the two handed pistol holding isn't aligned, but in slow mo is.

UI: 
    Selection Button which is the animated box on the menu, can get "stuck" on buttons, 
    fix: Removed from focus path - disconnecting it will fix it, but only for using mouse, 
    and disconnecting mosue leave will fix for keyboard/controller
    just need to add detection on which the player is using.


Wish List: 

Chat Box: All Chat, Team Chat, Direct Chat.
Emotes. 
Cheat Menu (Halo:CE).
Rework of the Ping system 
