#pragma once

/*! \file Constants.h
@brief   Constants that are used throughout the game.
@details Add any new constants to this file.
*/

/**< The window width. Defines how wide the game window is. */
constexpr float GAMEPLAY_AREA_WIDTH = 1.2f;
/**< The window height. Defines the height of the gameplay area */
constexpr float GAMEPLAY_AREA_HEIGHT = .8f;

/**< Value of PI. */
constexpr float PI = 3.14159265f;
/**< Value of the amount of energy left after a collision. */
constexpr float RESTITUTION = 0.8f;

/**< Defines the size of a single projectile sprite. */
constexpr float PROJECTILE_SIZE = 0.03f;
constexpr float BOMB_SIZE = 0.01f;

/**< Defines the maximum number of game objects for arrays. */
constexpr int NUM_PROJECTILES_SCATTER = 2;
constexpr int NUM_PROJECTILES = 5;
constexpr int NUM_ENEMIES = 6;
constexpr int NUM_BLOCKS = 41;
constexpr int NUM_LEVELS = 3;
constexpr int NUM_PLATFORMS = 10;
constexpr int GRID_SIZE = 150;


/**< Defines the maximum number of High scores. */
constexpr int NUM_HIGH_SCORES = 10;


/**< Defines the size of target sprites. */
constexpr float ENEMY_LARGE = 0.05f;
constexpr float ENEMY_MEDIUM = 0.03f;
constexpr float ENEMY_SMALL = 0.015f;

/**< Defines the size of target sprites. */
constexpr float BLOCK_LONG = 0.07f;
constexpr float PLATFORM_LONG = 0.1f;
constexpr float BLOCK_LARGE = 0.05f;
constexpr float BLOCK_NORMAL = 0.03f;
constexpr float BLOCK_SMALL = 0.01f;
constexpr float BLOCK_THIN = 0.01f;



constexpr float SLINGSHOT_HEIGHT = 0.08f;
constexpr float SLINGSHOT_WIDTH = 0.04;

// defines screen to display options
enum {	SPLASH_SCREEN, MAIN_SCREEN, HIGH_SCORES_SCREEN,
	GAME_OVER_SCREEN, NEW_HIGH_SCORE, IN_GAME};

// defines main menu options
enum { ONE_PLAYER, HIGH_SCORES, EXIT_GAME };