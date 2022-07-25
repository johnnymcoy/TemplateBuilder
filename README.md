# TemplateBuilder

Developed with Unreal Engine 4

Features:

## Done:
+ Add in selection bar to main menu

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
