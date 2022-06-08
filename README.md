# TemplateBuilder

Developed with Unreal Engine 4

Features:



## Working on:
New Main Menu UI.
+ Add in selection bar to main menu
+ Use more Blueprint controls for UI, C++ is too bloated
+ On button click, play animation, once completed, use menu swapper

Controller support for Menu.
Aim Assist

Tweaking needed:
Shield Regen 
Health Pickups (Replicate)

Todo: 
Refactor WeaponInterface
Add in all weapons 
Gun FX
Zombie AI
Finish enemy AI 
HUD 
Gameover screen 
Pause menu to be the same as Main



Bugs:
Server Player, when starting the game will not have the bottom half of his body replicated.
Client doesn't receive Overlay state when picking up weapon
Full Auto weapons only get the accuracy calculated once, add in a timer and have it stop when stopShooting is called


Wish List: 
Chat Box: All Chat, Team Chat, Direct Chat.
Emotes. 
Cheat Menu (Halo:CE).
Rework of the Pink system 
