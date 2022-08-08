# TemplateBuilder

Developed with Unreal Engine 4

Features:

## Done:
+ Add in selection bar to main menu
Add in Custom Physical Animation to CustomALS Character
Add in gun impulse to the physical Anim part
BUG- Collision on the custom physical animation doesn't really work when being shot

When taking damage-> health left should be the Alpha of how much the customPhysicalAnimation is applied
Dismemberment component 

AI - -

Add in new animations

Add in reaction Animations?
Fixing up References in Character BP
Trying to make it more flexable and usable if i want to use it in another project

Changed EQS nearest player + find all players from Player_bp to Player controller, need to test if works


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
