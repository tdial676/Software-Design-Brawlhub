# Log Entries
## Tuesday, 5/17, 10:55am - 11:00am

**WHO:** Stephen

**WHAT:** Added files from physics engine and created 7 new files related to game
            1. brawlhub.c - where game engine is located (in demo)
            2. character.h and character.c - where user functionality is located (in include and library respectively)
            3. computer.h and computer.c - where enemy and possibly ally functionality will be located (in include and library respectively)
            4. key_handler.h and key_handler.c - where the key handling for the separate scenes should be implemented (in include and library respectively)
        
        (All files currently blank though)

**BUGS:** None

**RESOURCES USED:** None

## Wednesday, 5/18, 8:30pm - 9:45pm

**WHO:** Ananya

**WHAT:** Began basic implementation of the enemy and ally functionality within computer.c. Added an enum file for
          different types of enemies and various other listed items

**BUGS:** None

**RESOURCES USED:** My groupmates

## Thursday, 5/19, 3:30am - 8:20am

**WHO:** Thierno, Stephen

**WHAT:** Thierno implemented key methods for WASD keys and for mouse tracking and character rotation. Stephen worked on the character development and added a damaging collision functionality to body and forces.

**BUGS:** SDL mouse notations.

**RESOURCES USED:** SDL library

## Thursday, 5/19, 8:30pm - 11:13pm

**WHO:** Thierno, Stephen, Ananya, Mohamed

**WHAT:** Mohamed worked on getting a skeleton scene. Ananya worked on computer.c or rendering enemies. Thierno worked on finalizing keys inside demo. Stephen helped with finalizing key methods.

**BUGS:** Understanding SDL_Wrapper syntax.

**RESOURCES USED:** SDL library

## Friday, 5/20, 2:00am - 6:10am

**WHO:** Stephen

**WHAT:** Implemented preliminary weapon.h and weapon.c files to be used by computer and character for shooting

**BUGS:** None

**RESOURCES USED:** None

## Sunday, 5/22, 8:00pm - 11:30pm

**WHO:** Mohamed

**WHAT:** Made changes to center the screen around the user by changing SDL wrapper

**BUGS:** None

**RESOURCES USED:** TAs at office hours

## Monday, 5/23, 6:45 - 8:15 pm

**WHO:** Ananya

**WHAT:** Finished implementation of computer.c and computer.h files

**BUGS:** None

**RESOURCES USED:** None

## Monday, 5/23, 9:20pm - 12:03a`m

**WHO:** Stephen, Thierno, Mohamed

**WHAT:** Mohamed worked on the demo scenes (got something to draw and got scne movement). Stephen worked on weapons file. Thierno and Stephen continued implementing character.c.

**BUGS:** None

**RESOURCES USED:** None

## Tuesday, 5/24, 8:30pm - 12:00am

**WHO:** Stephen, Thierno, Mohammed, Ananya

**WHAT:** Mohamed worked on the demo scenes with Stephen and Ananya to implement borders, physics collisions. Thierno and Stephen worked on getting charceter to roatte with mouse. Thierno worked on rendering text for health and kill count. Ananya worked on computer.c with Mohamed.

**BUGS:** The math to get the charecter to correctly rotate. Notation for rendering text.

**RESOURCES USED:** Asked TA on Discord.

## Wednesday, 5/25, 2:30am - 7:30am

**WHO:** Stephen

**WHAT:** Worked on figuring out bug for rotations, and found it; Tried some ways of resolving bug

**BUGS:** Character rotation not working properly; figured out bug but don't know how to resolve yet

**RESOURCES USED:** None

## Wednesday, 5/25, 4:00pm - 5:45pm

**WHO:** Stephen

**WHAT:** Added floor and background to game scene; considered ways to add lifetimes for different bullets

**BUGS:** None

## Wednesday, 5/25, 8:30pm - 12:00am

**WHO:** Mohamed, Thierno, Ananya, Stephen

**WHAT:** Thierno finalized rendering text methods. Mohamed and Thierno worked together on trying to render text on window. Mohamed refactored the demo to add a pause screen with key bindings. Stephen on rotation. Stephen and Ananya worked on computer, specifically enemy shooting.

**BUGS:** Rendering text still doesn't work. Enemies don't seem to be shooting

**RESOURCES USED:** TAs at OH


## Thursday, 5/26, 12:00am - 7:10am

**WHO:** Thierno, Stephen

**WHAT:** Thierno and Stephen worked on getting enemies to move towards player, fire, bullet live time, and damage proccessing. We fixed memory leaks and implemented a background music method however like render text it wont open the file. We plan on working on it in office hours tomorrow.

**BUGS:** music does'nt play.

**RESOURCES USED:** print methods, SDL Notation

## Thursday, 5/26, 8:00pm - 12:15am

**WHO:** Ananya, Mohamed, Stephen

**WHAT:** Ananya worked on implementing spawning waves of enemies; Mohamed worked on making the scene render text in a correct manner; Stephen worked on getting
         the background music to play

**BUGS:** issues with sdl notation for text and music

**RESOURCES USED:** TA office hours and groupmates for clarification on added code

## Friday, 5/27, 12:30am - 4:00am

**WHO:** Stephen

**WHAT:** Implemented the background music; Tweaking enemy stats including health and speed based on type of enemy; Tweaking weapon specs and adding
          mechanism for shotgun to shoot; Made sure pause screen worked as intended

**BUGS:** enemies teleporting due to pause screen not ticking time

**RESOURCES USED:** SDL Notation

## Saturday, 5/28, 12:00am - 3:20am

**WHO:** Stephen

**WHAT:** Implemented a mechanism for randomly spawning obstacles at beginning of game

**BUGS:** Enemy collision does not work with obstacles a lot of the time

**RESOURCES USED:** None


## Monday, 5/30, 9:00pm - 11:30pm

**WHO:** Stephen, Ananya, Mohamed

**WHAT:** Ananya and Stephen finished up waves on enemies and Mohamed worked on and finished up pause screen for the game, and refactored so we could add multiple prints when rendering a scene. Ananya also started working
on sound effects while Stephen started worked on finishing obstacles. 

**BUGS:** Enemy waves didn't work initially when Ananya finished an initial copy, but small debugging made it work. Mohamed also had a couple
small issues to debug as well. 

**RESOURCES USED:** None

## Monday, 5/30, 10:00pm - 4:30am 

**WHO:** Stephen, Thierno

**WHAT:** Stephen and Thierno worked on powerups. Implemented gravity bullet for sniper, and 360 shot for brute. Worked on implemeting a shield fo gunman and fixing color to have translucency. Stephen worked on fixed map with the obstacles. 

**BUGS:** Enemy collision does not work with obstacles a lot of the time

**RESOURCES USED:** None

## Tuesday, 5/31, 8:30pm - 9:45pm

**WHO** Ananya

**WHAT:** Finished up waves and added bosses. Started searching for sound effects

**BUGS:** None

**RESOURCES USED:** None

## Wednesday, 06/01, 7:30pm - 10:02pm 

**WHO:** Thierno

**WHAT:** Thierno implemented xp, coins, and rendered stats.

**BUGS:** None

**RESOURCES USED:** None

## Wednesday, 06/01, 10:45pm - 3:45am 

**WHO:** Thierno, Stephen

**WHAT:** Thierno worked on rendering sprites and Stephen worked on fixing memory leak due to not destroying texture. Stephen implemented overtime healing. Implemented rendering of wave number. Tweaked weapon and computer stats

**BUGS:** Thierno hit a road block gettting png images to draw (will ask TA tomorrow).

**RESOURCES USED:** SDL library

## Wednesday, 06/02, 08:45pm - 12:30am

**WHO:** Thierno, Stephen, Mohamed, Ananya

**WHAT:** Thierno and Mohammed implemented the store as part of the pause scene. Thierno continued working on sprites. Ananya and Stephen worked on sound effectes. Stephen continued with memory leak fixes.

**BUGS:** Rendering images

**RESOURCES USED:** TA, SDL library

## Thursday, 06/02, 12:30am - 3:57am

**WHO:** Thierno, Stephen

**WHAT:** Thierno implemented sprites and got them to render, face mouse, and rotate with the character. Stephen worked on sound boss spawns intervals.

**BUGS:** Rendering different sprites. We tried a switch file method, but it would not return the file name. 

**RESOURCES USED:** SDL library

## Thursday, 06/02, 4:30am - 9:45am

**WHO:** Thierno, Stephen

**WHAT:** Thierno implemented sprites for the enemies and got them to render, face character, and follow the character. 
          Stephen got sound working for character and enemy shooting as well as
          tweaked some stats of the various character types and helped Thierno finally finish rendering of enemy sprites;

**BUGS:** Rendering sprites in correct location. Had issues with the fact the sprites render relative to window as opposed to whole scene

**RESOURCES USED:** SDL library and Stack Overflow

## Friday, 06/03, 8:04pm - 10:45am

**WHO:** Thierno, Stephen, Ananya

**WHAT:** We all commented and refactored code. Stephen reorganized the start screen with Thierno. Thierno added sprites to start to screen and changed pause scne. Stephen cleaned up final presentation and made color changes and last minute tweaks.

**BUGS:** None

**RESOURCES USED:** None

## Sunday, 06/05, 12:00am - 1:30am 

**WHO:** Stephen

**WHAT:** Cleaned up demo file for magic numbers as well as the additional files; refactored main to follow procedural decomposition

**BUGS:** SDL text printing error due to sprintf on stack variables outside of main function

**RESOURCES USED:** C library documentation