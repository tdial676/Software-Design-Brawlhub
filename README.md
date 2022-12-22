# CS3--Software-Design
CS 3 is a practical introduction to designing large programs in a low-level language. Heavy emphasis is placed on documentation, testing, and software architecture. Students will work in teams in two 5-week long projects. In the first half of the course, teams will focus on testing and extensibility. In the second half of the course, teams will use POSIX APIs, as well as their own code from the first five weeks, to develop a large software deliverable. Software engineering topics covered include code reviews, testing and testability, code readability, API design, refactoring, and documentation. For my
final project my group designed a game with the below specs:
# General Gameplay
In our game, we will have a character going around 2d (top-down view) map with a gun shooting enemies. Our game will feature various waves of enemies        that attempt to attack the player. As the player kills more enemies, there will be waves that will feature a higher concentration of enemies and/or more powerful enemies. Every ten waves a boss enemy will show up who will be a more challenging enemy. You lose when your health reaches zero.
# Controls
- WASD for movement as in most standard games
- Space Bar to switch weapons
- Mouse for aiming and shooting(you can only shoot while not moving character)
# Physics
- Collision between character and bullets and enemies and bullets
- Barricades that block bullets
- Gravity for homing bullets
# Game Flow
Character spans at the center of the screen and walks around the canvas shooting enemies that appear randomly from off-screen. The user can switch between two weapons and this switch will be denoted by color changes of the firing mechanism. There will be background music and sound sound effects with each gun fire. The character will have a health bar that will be depleted each time they are hit. If their health reaches zero, game over is printed on the screen and the character is rendered immobile. The player gains health, xp, and currency everytime they kill an enemy. The user can use these to upgrade their square by getting different colors, more health, and different guns and boosts. The game will have a pause button, in which the scene will stop ticking. 
# Graphics
The plan is to ultimately use sprites, but we will start off with polygons for our character and enemy. The character will be a black four sided shape with a small rectangle attached to it to represent the gun. The enemies will be multicolore( color could denote type of enemy) and they will be the same shape as the user with their guns attached the same way as the user. The map will have various polygon obstacles that cannot be destroyed or walked through.
# My Role
I mainly focussed on the the graphics of the character so rendering sprites, implementing coin system, implementing a store and XP. I was also in charge of implementing the controls, background music, powerups, and stats for characters and enemies and varying these stats as the game progressed. As this was a group project, I helped where I could with errors and other implementations and also did not shy away from asking my team for help/clarifications when needed to make sure everything runs as smoothly as possible.
    
# To My Fellow Caltech students
Only look at the solutions (if it's the same assignment) once you have completed the course or submitted the appropriate assignments.
As per the Honor Code: No member of the Caltech community shall take unfair advantage of any other member of the Caltech community.
