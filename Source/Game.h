#pragma once
#include <string>
#include <Engine/OGLGame.h>
#include <fstream>
#include <iostream>


#include "GameObject.h"
#include "Constants.h"
#include "Rect.h"



struct Score
{
	long score = 0;
	std::string initials;
};

struct LevelPosIndex
{
	int block_index = 0;
	int x_index = 0;
	int y_index = 0;
};



/**
*  An OpenGL Game based on ASGE.
*/
class AngryBirdsGame :
	public ASGE::OGLGame
{
public:
	AngryBirdsGame();
	~AngryBirdsGame();
	virtual bool init() override;
	

private:
	void keyHandler(const ASGE::SharedEventData data);
	void clickHandler(const ASGE::SharedEventData data);
	void setupResolution();
	bool loadBackgrounds();
	bool loadGameSprites();
	void setupGrid();
	void levelGen();
	void saveLevelMap();
	void loadLevelMap(int map);

	void renderMainMenu();
	void renderSplash();
	void renderInGame();
	void renderGameOverL();
	void renderGameOverW();
	bool updateHighScores();
	void renderHighScores();
	void renderNewHighScore();

	void newGame();
	void setupLevel(int level);
	void setupProjectiles(rect projectile_platform);
	void setupPigs(int map);
	rect setupPlatforms(int map);
	void projectileCollision();
	void projectileScatterCollision();
	void levelCollision();
	void enemyCollision();
	void releaseBomb(rect projectile);
	void resetBomb();
	void releaseProjectileScatter(rect projectile, vector2 velocity);
	void resetProjectileScatter();
	void bombCollision();
	void boostProjectile(); 
	void resetProjectiles();
	void windBreath();

	void loadFiles();
	void saveHighScores();
	void clearArrays();

	virtual void update(const ASGE::GameTime &) override;
	virtual void render(const ASGE::GameTime &) override;

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	//Add your GameObjects
	GameObject level_layer[NUM_LEVELS];
	GameObject menu_layer;
	GameObject blocks[NUM_BLOCKS];
	GameObject enemies[NUM_ENEMIES];
	GameObject enemy_counter;
	GameObject projectiles[NUM_PROJECTILES];
	GameObject projectiles_scatter[NUM_PROJECTILES_SCATTER];
	GameObject platforms[NUM_PLATFORMS];
	GameObject bomb;
	GameObject slingshot;
	ASGE::Sprite* splash_screen = nullptr;
	rect gameplay_area;
	rect aiming_area;
	int game_state = SPLASH_SCREEN;
	LevelPosIndex level_map[NUM_BLOCKS];


	// grid coordinate arrays
	float grid_X[GRID_SIZE];
	float grid_Y[GRID_SIZE];

	// menu variables
	int menu_option = 0;
	int initial = 0;

	// in game variables
	bool new_game = true;
	int level = 0;
	int projectile = 0;
	long current_score = 0;
	int no_enemies_hit = 0;
	int projectiles_left = 0;
	bool aiming = false;
	bool flying = true;
	bool bomb_active = false;
	bool scatter_active = false;
	bool boost_active = false;
	bool wind_active = false;

	vector2 slingshot_center;

	// high score variables
	Score high_scores[NUM_HIGH_SCORES];
	char new_initial = 'A';
	std::string new_initials = "AAA";
	int high_score_idx_to_update = 0;
};