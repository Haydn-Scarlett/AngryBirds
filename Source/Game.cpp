#include <string>
#include <Windows.h>

#include <Engine/Keys.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Sprite.h>

#include "Game.h"

/**
*   @brief   Default Constructor.
*   @details Consider setting the game's width and height
             and even seeding the random number generator.
*/
AngryBirdsGame::AngryBirdsGame()
{
	std::srand(time(NULL));
}

/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
AngryBirdsGame::~AngryBirdsGame()
{
	this->inputs->unregisterCallback(key_callback_id);
	this->inputs->unregisterCallback(mouse_callback_id);
}

/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The keyHandler and clickHandler
			 callback should also be set in the initialise function.
*   @return  True if the game initialised correctly.
*/
bool AngryBirdsGame::init()
{
	setupResolution();
	if (!initAPI())
	{
		return false;
	}

	renderer->setWindowTitle("Angry Birds!");
	renderer->setWindowedMode(ASGE::Renderer::WindowMode::WINDOWED);
	setupResolution();

	// input handling functions
	inputs->use_threads = false;

	key_callback_id = inputs->addCallbackFnc(
		ASGE::E_KEY, &AngryBirdsGame::keyHandler, this);

	mouse_callback_id = inputs->addCallbackFnc(
		ASGE::E_MOUSE_CLICK, &AngryBirdsGame::clickHandler, this);

	clearArrays();
	loadFiles();

	

	gameplay_area.height = game_height * GAMEPLAY_AREA_HEIGHT;
	gameplay_area.length = game_height * GAMEPLAY_AREA_WIDTH;
	gameplay_area.y = game_height * .09f;
	gameplay_area.x = (game_width * 0.5) - (gameplay_area.length * 0.5f);

	setupGrid();
	//levelGen();
	//saveLevelMap();
	if (!loadBackgrounds())
	{
		return false;
	}
	if (!loadGameSprites())
	{
		return false;
	}
	return true;
}

/**
*   @brief   Loads the background and ui sprites
*   @details This function is used to load the sprites for the
background and ui gameobjects.
*   @return  bool
*/
bool AngryBirdsGame::loadBackgrounds()
{
	if (!menu_layer.addSpriteComponent(renderer.get(), "Resources\\Textures\\menu.jpg"))
	{
		return false;
	}

	if (!level_layer[0].addSpriteComponent(renderer.get(), "Resources\\Textures\\lvl1.png"))
	{
		return false;
	}

	if (!level_layer[1].addSpriteComponent(renderer.get(), "Resources\\Textures\\lvl2.png"))
	{
		return false;
	}
	if (!level_layer[2].addSpriteComponent(renderer.get(), "Resources\\Textures\\lvl3.png"))
	{
		return false;
	}
	if (!enemy_counter.addSpriteComponent(renderer.get(), "Resources\\Textures\\kenney_animalpackredux\\PNG\\round\\pig.png"))
	{
		return false;
	}

	splash_screen = renderer->createRawSprite();
	splash_screen->loadTexture(".\\Resources\\Textures\\splash_screen.png");
	splash_screen->width(game_width * 0.5f);
	splash_screen->height(game_height * 0.5f);
	splash_screen->xPos(((float)game_width * 0.5f) -
		(splash_screen->width() * 0.5f));
	splash_screen->yPos(((float)game_height * 0.5f) -
		(splash_screen->height() * 0.5f));

	ASGE::Sprite* menu_layer_sprite = menu_layer.spriteComponent()->getSprite();
	menu_layer_sprite->height((float)game_height);
	menu_layer_sprite->width((float)game_width * 0.7f);
	menu_layer_sprite->yPos(0.f);
	menu_layer_sprite->xPos(0.f);

	for (int i = 0; i < NUM_LEVELS; i++)
	{
		ASGE::Sprite* level_layer_sprite = level_layer[i].spriteComponent()->getSprite();
		level_layer_sprite->height(gameplay_area.height);
		level_layer_sprite->width(gameplay_area.length);
		level_layer_sprite->yPos(gameplay_area.y);
		level_layer_sprite->xPos(gameplay_area.x);
	}

	ASGE::Sprite* enemy_counter_sprite = enemy_counter.spriteComponent()->getSprite();
	enemy_counter_sprite->height(game_height * ENEMY_MEDIUM);
	enemy_counter_sprite->width(game_height * ENEMY_MEDIUM);
	enemy_counter_sprite->yPos(game_height * 0.05f);
	enemy_counter_sprite->xPos(gameplay_area.x);
	
	return true;
}

/**
*   @brief   Loads the gameplay sprites
*   @details This function is used to load the sprites for the 
various gameplay objects.
*   @return  bool
*/
bool AngryBirdsGame::loadGameSprites()
{
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		
		if (i < 2)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Glass elements\\elementGlass012.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < 4)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Glass elements\\elementGlass014.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_THIN);
			block_sprite->width(game_height * BLOCK_LONG);
		}
		else if (i < 7)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Glass elements\\elementGlass021.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_LONG);
			block_sprite->width(game_height * BLOCK_THIN);
		}
		else if (i < 9)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Glass elements\\elementGlass003.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < 10)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Glass elements\\elementGlass005.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < 14)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Wood elements\\elementWood010.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < 20)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Wood elements\\elementWood012.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_THIN);
			block_sprite->width(game_height * BLOCK_LONG);
		}
		else if (i < 24)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Wood elements\\elementWood019.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_LONG);
			block_sprite->width(game_height * BLOCK_THIN);
		}
		else if (i < 28)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Wood elements\\elementWood001.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < 30)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Wood elements\\elementWood000.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < 32)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Stone elements\\elementStone012.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < 36)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Stone elements\\elementStone013.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_THIN);
			block_sprite->width(game_height * BLOCK_LONG);
		}
		else if (i < 39)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Stone elements\\elementStone020.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_LONG);
			block_sprite->width(game_height * BLOCK_THIN);
		}
		else if (i < 40)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Stone elements\\elementStone004.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		else if (i < NUM_BLOCKS)
		{
			if (!blocks[i].addSpriteComponent(renderer.get(),
				"Resources\\Textures\\kenney_physicspack\\PNG\\Explosive elements\\elementExplosive011.png"))
			{
				return false;
			}
			ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
			block_sprite->height(game_height * BLOCK_NORMAL);
			block_sprite->width(game_height * BLOCK_NORMAL);
		}
		blocks[i].setVisible(false);
	}

	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (!enemies[i].addSpriteComponent(renderer.get(), "Resources\\Textures\\kenney_animalpackredux\\PNG\\round\\pig.png"))
		{
			return false;
		}
		ASGE::Sprite* enemy_sprite = enemies[i].spriteComponent()->getSprite();
		if (i < 2)
		{
			enemy_sprite->height(game_height * ENEMY_MEDIUM);
			enemy_sprite->width(game_height * ENEMY_MEDIUM);
		}
		else
		{
			enemy_sprite->height(game_height * ENEMY_SMALL);
			enemy_sprite->width(game_height * ENEMY_SMALL);
		}
		enemies[i].setVisible(false);
	}

	for (int i = 0; i < NUM_PROJECTILES_SCATTER; i++)
	{
		if (!projectiles_scatter[i].addSpriteComponent(renderer.get(),
			"Resources\\Textures\\kenney_animalpackredux\\PNG\\Round\\chick.png"))
		{
			return false;
		}
		projectiles_scatter[i].setVisible(false);
		ASGE::Sprite* projectile = projectiles_scatter[i].spriteComponent()->getSprite();
		projectile->height(gameplay_area.height * (PROJECTILE_SIZE * 0.5f));
		projectile->width(gameplay_area.length * (PROJECTILE_SIZE * 0.5f));
	}

	for (int i = 0; i < NUM_PLATFORMS; i++)
	{
		if (!platforms[i].addSpriteComponent(renderer.get(),
			"Resources\\Textures\\kenney_physicspack\\PNG\\Other\\dirt.png"))
		{
			return false;
		}
		platforms[i].setVisible(false);
		ASGE::Sprite* projectile = projectiles_scatter[i].spriteComponent()->getSprite();
		projectile->height(gameplay_area.height * BLOCK_NORMAL);
		projectile->width(gameplay_area.length * PLATFORM_LONG);
	}

	if (!bomb.addSpriteComponent(renderer.get(),
		"Resources\\Textures\\kenney_physicspack\\PNG\\Other\\coinDiamond.png"))
	{
		return false;
	}
	bomb.setVisible(false);
	ASGE::Sprite* bomb_sprite = bomb.spriteComponent()->getSprite();
	bomb_sprite->height(gameplay_area.height * BOMB_SIZE);
	bomb_sprite->width(gameplay_area.height * BOMB_SIZE);


	if (!projectiles[0].addSpriteComponent(renderer.get(),
		"Resources\\Textures\\kenney_animalpackredux\\PNG\\Round\\duck.png"))
	{
		return false;
	}
	projectiles[0].setVisible(false);
	ASGE::Sprite* projectile = projectiles[0].spriteComponent()->getSprite();
	projectile->height(gameplay_area.height * PROJECTILE_SIZE);
	projectile->width(gameplay_area.length * PROJECTILE_SIZE);

	if (!projectiles[1].addSpriteComponent(renderer.get(),
		"Resources\\Textures\\kenney_animalpackredux\\PNG\\Round\\owl.png"))
	{
		return false;
	}
	projectiles[1].setVisible(false);
	projectile = projectiles[1].spriteComponent()->getSprite();
	projectile->height(gameplay_area.height * PROJECTILE_SIZE);
	projectile->width(gameplay_area.length * PROJECTILE_SIZE);


	if (!projectiles[2].addSpriteComponent(renderer.get(),
		"Resources\\Textures\\kenney_animalpackredux\\PNG\\Round\\penguin.png"))
	{
		return false;
	}
	projectiles[2].setVisible(false);
	projectile = projectiles[2].spriteComponent()->getSprite();
	projectile->height(gameplay_area.height * PROJECTILE_SIZE);
	projectile->width(gameplay_area.length * PROJECTILE_SIZE);

	if (!projectiles[3].addSpriteComponent(renderer.get(),
		"Resources\\Textures\\kenney_animalpackredux\\PNG\\Round\\chick.png"))
	{
		return false;
	}
	projectiles[3].setVisible(false);
	projectile = projectiles[3].spriteComponent()->getSprite();
	projectile->height(gameplay_area.height * (PROJECTILE_SIZE * 0.5f));
	projectile->width(gameplay_area.length * (PROJECTILE_SIZE * 0.5f));


	if (!projectiles[4].addSpriteComponent(renderer.get(),
		"Resources\\Textures\\kenney_animalpackredux\\PNG\\Round\\parrot.png"))
	{
		return false;
	}
	projectiles[4].setVisible(false);
	projectile = projectiles[4].spriteComponent()->getSprite();
	projectile->height(gameplay_area.height * PROJECTILE_SIZE);
	projectile->width(gameplay_area.length * PROJECTILE_SIZE); 
	
	if (!slingshot.addSpriteComponent(renderer.get(),
		"Resources\\Textures\\Slingshot.png"))
	{
		return false;
	}
	slingshot.setVisible(false);
	ASGE::Sprite* slingshot_sprite = slingshot.spriteComponent()->getSprite();
	slingshot_sprite->height(game_height * SLINGSHOT_HEIGHT);
	slingshot_sprite->width(game_height * SLINGSHOT_WIDTH);

	return true;
}

/**
*   @brief   Sets the grid positions for level setup
*   @details This function is designed to create the grid positions for
the level objects 
*   @return  void
*/
void AngryBirdsGame::setupGrid()
{
	float newXpos = gameplay_area.x + (gameplay_area.length * 0.4f);
	float newYpos = gameplay_area.y + (gameplay_area.height * 0.9f);
	for (int i = 0; i < GRID_SIZE; i++)
	{
		grid_X[i] = newXpos;
		grid_Y[i] = newYpos;
		newXpos = newXpos + (game_height * BLOCK_THIN);
		newYpos = newYpos - (game_height * BLOCK_THIN);
	}
}

/**
*   @brief   Generates the level maps
*   @details This function is designed to create the window size, any
aspect ratio scaling factors and safe zones to ensure the
game frames when resolutions are changed in size.
*   @return  void
*/
void AngryBirdsGame::levelGen()
{
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		level_map[i].block_index = i;
		switch (i)
		{

		case 0:
			level_map[i].x_index = 18;
			level_map[i].y_index = 42;
			break;
		case 1:
			level_map[i].x_index = 18;
			level_map[i].y_index = 39;
			break;
		case 2:
			level_map[i].x_index = 30;
			level_map[i].y_index = 16;
			break;
		case 3:
			level_map[i].x_index = 37;
			level_map[i].y_index = 16;
			break;
		case 4:
			level_map[i].x_index = 30;
			level_map[i].y_index = 14;
			break;
		case 5:
			level_map[i].x_index = 36;
			level_map[i].y_index = 14;
			break;
		case 6:
			level_map[i].x_index = 43;
			level_map[i].y_index = 14;
			break;
		case 7:
			level_map[i].x_index = 21;
			level_map[i].y_index = 39;
			break;
		case 8:
			level_map[i].x_index = 12;
			level_map[i].y_index = 26;
			break;
		case 9:
			level_map[i].x_index = 42;
			level_map[i].y_index = 19;
			break;
		case 10:
			level_map[i].x_index = 24;
			level_map[i].y_index = 42;
			break;
		case 11:
			level_map[i].x_index = 24;
			level_map[i].y_index = 39;
			break;
		case 12:
			level_map[i].x_index = 23;
			level_map[i].y_index = 19;
			break;
		case 13:
			level_map[i].x_index = 44;
			level_map[i].y_index = 11;
			break;
		case 14:
			level_map[i].x_index = 15;
			level_map[i].y_index = 43;
			break;
		case 15:
			level_map[i].x_index = 23;
			level_map[i].y_index = 43;
			break;
		case 16:
			level_map[i].x_index = 15;
			level_map[i].y_index = 44;
			break;
		case 17:
			level_map[i].x_index = 23;
			level_map[i].y_index = 44;
			break;
		case 18:
			level_map[i].x_index = 23;
			level_map[i].y_index = 16;
			break;
		case 19:
			level_map[i].x_index = 44;
			level_map[i].y_index = 8;
			break;
		case 20:
			level_map[i].x_index = 14;
			level_map[i].y_index = 43;
			break;
		case 21:
			level_map[i].x_index = 30;
			level_map[i].y_index = 43;
			break;
		case 22:
			level_map[i].x_index = 37;
			level_map[i].y_index = 14;
			break;
		case 23:
			level_map[i].x_index = 22;
			level_map[i].y_index = 22;
			break;
		case 24:
			level_map[i].x_index = 23;
			level_map[i].y_index = 22;
			break;
		case 25:
			level_map[i].x_index = 26;
			level_map[i].y_index = 19;
			break;
		case 26:
			level_map[i].x_index = 44;
			level_map[i].y_index = 14;
			break;
		case 27:
			level_map[i].x_index = 47;
			level_map[i].y_index = 11;
			break;
		case 28:
			level_map[i].x_index = 18;
			level_map[i].y_index = 47;
			break;
		case 29:
			level_map[i].x_index = 24;
			level_map[i].y_index = 47;
			break;
		case 30:
			level_map[i].x_index = 6;
			level_map[i].y_index = 18;
			break;
		case 31:
			level_map[i].x_index = 9;
			level_map[i].y_index = 26;
			break;
		case 32:
			level_map[i].x_index = 9;
			level_map[i].y_index = 23;
			break;
		case 33:
			level_map[i].x_index = 16;
			level_map[i].y_index = 23;
			break;
		case 34:
			level_map[i].x_index = 30;
			level_map[i].y_index = 15;
			break;
		case 35:;
			level_map[i].x_index = 37;
			level_map[i].y_index = 15;
			break;
		case 36:;
			level_map[i].x_index = 9;
			level_map[i].y_index = 22;
			break;
		case 37:
			level_map[i].x_index = 15;
			level_map[i].y_index = 22;
			break;
		case 38:
			level_map[i].x_index = 16;
			level_map[i].y_index = 22;
			break;
		case 39:
			level_map[i].x_index = 9;
			level_map[i].y_index = 29;
			break;
		case 40:
			level_map[i].x_index = 18;
			level_map[i].y_index = 18;
			break;
		}
	}
}

/**
*   @brief   Sets the game window resolution
*   @details This function is designed to create the window size, any 
             aspect ratio scaling factors and safe zones to ensure the 
			 game frames when resolutions are changed in size.
*   @return  void
*/
void AngryBirdsGame::setupResolution()
{
	// how will you calculate the game's resolution?
	// will it scale correctly in full screen? what AR will you use?
	// how will the game be framed in native 16:9 resolutions?
	// here are some abritrary values for you to adjust as you see fit
	// https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
	game_height = GetSystemMetrics(SM_CYSCREEN);
	game_width = GetSystemMetrics(SM_CXSCREEN);
}

/**
*   @brief   Processes any key inputs
*   @details This function is added as a callback to handle the game's
			 keyboard input. For this game, calls to this function
			 are thread safe, so you may alter the game's state as you
			 see fit.
*   @param   data The event data relating to key input.
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::keyHandler(const ASGE::SharedEventData data)
{
	auto key = static_cast<const ASGE::KeyEvent*>(data.get());
	
	if (key->key == ASGE::KEYS::KEY_ESCAPE)
	{
		signalExit();
	}
	
	if (key->key == ASGE::KEYS::KEY_ENTER &&
		key->action == ASGE::KEYS::KEY_PRESSED &&
		key->mods == 0x0004)
	{
		if (renderer->getWindowMode() == ASGE::Renderer::WindowMode::WINDOWED)
		{
			renderer->setWindowedMode(ASGE::Renderer::WindowMode::FULLSCREEN);
		}
		else
		{
			renderer->setWindowedMode(ASGE::Renderer::WindowMode::WINDOWED);
		}
	}
		

	if (key->key == ASGE::KEYS::KEY_SPACE &&
		key->action == ASGE::KEYS::KEY_PRESSED
		&& game_state == IN_GAME && flying)
	{
		switch (projectile)
		{
		case 0:
			if (!boost_active)
			{
				boost_active = true;
				windBreath();
			}
			break;
		case 1:
			if (!bomb_active)
			{
				rect pos = projectiles[projectile].spriteComponent()->getBoundingBox();
				bomb_active = true;
				releaseBomb(pos);
			}
			break;
		case 2:
			if (!boost_active)
			{
				boost_active = true;
				boostProjectile();
			}
			break;
		case 3:
			if (!scatter_active)
			{
				rect pos = projectiles[projectile].spriteComponent()->getBoundingBox();
				vector2 vel = projectiles[projectile].getVelocity();
				scatter_active = true;
				releaseProjectileScatter(pos, vel);
			}
			break;
		case 4:
			break;

		}

	}

	if (key->key == ASGE::KEYS::KEY_UP &&
		key->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (game_state == MAIN_SCREEN)
		{
			menu_option--;
			if (menu_option < ONE_PLAYER)
			{
				menu_option = EXIT_GAME;
			}
		}
		else if (game_state == NEW_HIGH_SCORE)
		{
			new_initial += 1;
			if (new_initial > 'Z')
			{
				new_initial = 'A';
			}
			new_initials[initial] = new_initial;
		}
	}
	if (key->key == ASGE::KEYS::KEY_DOWN &&
		key->action == ASGE::KEYS::KEY_RELEASED)
	{
		if (game_state == MAIN_SCREEN)
		{
			menu_option++;
			if (menu_option > EXIT_GAME)
			{
				menu_option = ONE_PLAYER;
			}
		}
		else if (game_state == NEW_HIGH_SCORE)
		{
			new_initial -= 1;
			if (new_initial < 'A')
			{
				new_initial = 'Z';
			}
			new_initials[initial] = new_initial;
		}
	}

	if (key->key == ASGE::KEYS::KEY_ENTER &&
		key->action == ASGE::KEYS::KEY_RELEASED)
	{   //Main menu
		if (game_state == MAIN_SCREEN)
		{
			if (menu_option == ONE_PLAYER)
			{

				game_state = IN_GAME;
				new_game = true;
			}
			if (menu_option == HIGH_SCORES)
			{
				game_state = HIGH_SCORES_SCREEN;
			}
			if (menu_option == EXIT_GAME)
			{
				signalExit();
			}
		}
		else if (game_state == GAME_OVER_SCREEN)
		{
			if (updateHighScores())
			{
				game_state = NEW_HIGH_SCORE;
			}
			else
			{
				game_state = MAIN_SCREEN;
			}
		}
		else if (game_state == NEW_HIGH_SCORE)
		{
			high_scores[high_score_idx_to_update].initials = new_initials;
			saveHighScores();

			// reset menu variables
			initial = 0;
			new_initial = 'A';
			new_initials = "AAA";
			high_score_idx_to_update = 0;
			game_state = MAIN_SCREEN;
		}
		else if (game_state == HIGH_SCORES_SCREEN)
		{
			game_state = MAIN_SCREEN;
		}

	}

	if (key->key == ASGE::KEYS::KEY_LEFT &&
		key->action == ASGE::KEYS::KEY_PRESSED && 
		game_state == NEW_HIGH_SCORE)
	{
		initial -= 1;
		if (initial < 0)
		{
			initial = 2;
		}
		new_initial = new_initials[initial];
	}
	if (key->key == ASGE::KEYS::KEY_RIGHT &&
		key->action == ASGE::KEYS::KEY_PRESSED && 
		game_state == NEW_HIGH_SCORE)
	{
		initial += 1;
		if (initial > 2)
		{
			initial = 0;
		}
		new_initial = new_initials[initial];
	}
}

/**
*   @brief   Processes any click inputs
*   @details This function is added as a callback to handle the game's
		     mouse button input. For this game, calls to this function
             are thread safe, so you may alter the game's state as you
             see fit.
*   @param   data The event data relating to key input.
*   @see     ClickEvent
*   @return  void
*/
void AngryBirdsGame::clickHandler(const ASGE::SharedEventData data)
{
	auto click = static_cast<const ASGE::ClickEvent*>(data.get());
	double x_pos, y_pos;
	inputs->getCursorPos(x_pos, y_pos);

	rect mousePointer;
	mousePointer.x = x_pos;
	mousePointer.y = y_pos;
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		rect projectile_rect = projectiles[i].spriteComponent()->getBoundingBox();
		if (mousePointer.isInside(projectile_rect) && click->action == 0 &&
			i != projectile && aiming == false && !flying)
		{
			ASGE::Sprite* tempProjectile = projectiles[i].spriteComponent()->getSprite();
			ASGE::Sprite* tempProjectileTwo = projectiles[projectile].spriteComponent()->getSprite();

			float xTemp = tempProjectile->xPos();
			float yTemp = tempProjectile->yPos();
			if (i == 3)
			{
				yTemp -= tempProjectile->height();
			}
			tempProjectile->xPos(tempProjectileTwo->xPos());
			tempProjectile->yPos(tempProjectileTwo->yPos());
			if (projectile == 3)
			{
				yTemp += tempProjectileTwo->height();
			}
			tempProjectileTwo->xPos(xTemp);
			tempProjectileTwo->yPos(yTemp);
			projectile = i;
			i = NUM_PROJECTILES;
		}
		if (mousePointer.isInside(projectile_rect) && click->action == 1 &&
			i == projectile) {
			aiming = true;
		}
	}
	if (aiming)
	{
		if (click->action == 0) {
			aiming = false;
			rect temp_projectile_sprite = projectiles[projectile].spriteComponent()->getBoundingBox();
			projectiles[projectile].setVelocity(slingshot_center.getX() -
				temp_projectile_sprite.x, slingshot_center.getY() - temp_projectile_sprite.y);
			flying = true;
		}
	}
}

/**
*   @brief   Updates the scene
*   @details Prepares the renderer subsystem before drawing the
		     current frame. Once the current frame is has finished
		     the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void AngryBirdsGame::update(const ASGE::GameTime& us)
{
	auto dt_sec = us.delta_time.count() / 1000.0; 
	if (game_state == SPLASH_SCREEN)
	{

		splash_screen->width((float)splash_screen->width() +
			(((float)game_width * 0.5f) * 0.3f)	* (float)(us.delta_time.count() / 1000.f));
		splash_screen->height((float)splash_screen->height() +
			(((float)game_height * 0.5f) * 0.3f)	* (float)(us.delta_time.count() / 1000.f));
		splash_screen->xPos(((float)game_width * 0.5f) -
			(splash_screen->width() * 0.5f));
		splash_screen->yPos(((float)game_height * 0.5f) -
			(splash_screen->height() * 0.5f));
		if (splash_screen->xPos() < 0.f)
		{
			game_state = MAIN_SCREEN;
		}
	}
	else if (game_state == IN_GAME)
	{
		if (new_game)
		{
			newGame();
		}

		double x_pos, y_pos;
		inputs->getCursorPos(x_pos, y_pos);
		if (aiming)
		{
			ASGE::Sprite* temp_projectile_sprite = projectiles[projectile].spriteComponent()->getSprite();
			temp_projectile_sprite->xPos(x_pos);
			temp_projectile_sprite->yPos(y_pos);
			if (temp_projectile_sprite->xPos() < aiming_area.x)
			{
				temp_projectile_sprite->xPos(aiming_area.x);
			}
			if (temp_projectile_sprite->xPos() > aiming_area.x + aiming_area.length)
			{
				temp_projectile_sprite->xPos(aiming_area.x + aiming_area.length);
			}
			if (temp_projectile_sprite->yPos() < aiming_area.y)
			{
				temp_projectile_sprite->yPos(aiming_area.y);
			}
			if (temp_projectile_sprite->yPos() > aiming_area.y + aiming_area.height)
			{
				temp_projectile_sprite->yPos(aiming_area.y + aiming_area.height);
			}
		}
		enemyCollision();
		for (int i = 0; i < NUM_ENEMIES; i++)
		{
			if (enemies[i].getVisible())
			{
				ASGE::Sprite* enemySprite = enemies[i].spriteComponent()->getSprite();
				vector2 enemyVel = enemies[i].getVelocity();
				enemySprite->yPos(enemySprite->yPos() + enemyVel.getY() * 50.f *
					(us.delta_time.count() / 1000.f));

				enemies[i].setVelocity(enemyVel.getX(), (enemyVel.getY() + 20.f * (us.delta_time.count() / 1000.f)));
			}
		}
		if (flying)
		{
			projectileCollision();
			levelCollision();
			vector2 projectile_vel = projectiles[projectile].getVelocity();
			ASGE::Sprite* temp_projectile_sprite = projectiles[projectile].spriteComponent()->getSprite();
			temp_projectile_sprite->yPos((float)temp_projectile_sprite->yPos() +
				((projectile_vel.getY() * 5.f) *
				(us.delta_time.count() / 1000.f)));
			temp_projectile_sprite->xPos((float)temp_projectile_sprite->xPos() +
				((projectile_vel.getX() * 5.f) *
				(us.delta_time.count() / 1000.f)));
			projectiles[projectile].setVelocity(projectile_vel.getX() - 
				(projectile_vel.getX() * (0.05f * (us.delta_time.count() / 1000.f))),
				(projectile_vel.getY() + 8.f * (us.delta_time.count() / 1000.f)));
			temp_projectile_sprite->rotationInRadians(
				temp_projectile_sprite->rotationInRadians() +
				(1 * (us.delta_time.count() / 1000.f)));
			
			for (int i = 0; i < NUM_PROJECTILES_SCATTER; i++)
			{
				if (projectiles_scatter[i].getVisible())
				{
					vector2 projectile_scatter_vel = projectiles_scatter[i].getVelocity();
					ASGE::Sprite* temp_projectile_scatter_sprite =
						projectiles_scatter[i].spriteComponent()->getSprite();
					temp_projectile_scatter_sprite->yPos(
						(float)temp_projectile_scatter_sprite->yPos() +
						((projectile_scatter_vel.getY() * 5.f) *
						(us.delta_time.count() / 1000.f)));
					temp_projectile_scatter_sprite->xPos(
						(float)temp_projectile_scatter_sprite->xPos() +
						((projectile_scatter_vel.getX() * 5.f) *
						(us.delta_time.count() / 1000.f)));
					projectiles[projectile].setVelocity(projectile_scatter_vel.getX() -
						(projectile_scatter_vel.getX() *
						(0.05f * (us.delta_time.count() / 1000.f))),
							(projectile_scatter_vel.getY() + 8.f *
						(us.delta_time.count() / 1000.f)));
					temp_projectile_scatter_sprite->rotationInRadians(
						temp_projectile_scatter_sprite->rotationInRadians() +
						(1 * (us.delta_time.count() / 1000.f)));
				}
			}
		}
		if (bomb.getVisible())
		{
			bombCollision();

			vector2 bomb_vel = bomb.getVelocity();
			ASGE::Sprite* temp_bomb_sprite = bomb.spriteComponent()->getSprite();
			temp_bomb_sprite->yPos((float)temp_bomb_sprite->yPos() +
				((bomb_vel.getY() * 5.f) * (us.delta_time.count() / 1000.f)));
		}

	}

	if (no_enemies_hit == NUM_ENEMIES)
	{
		current_score += projectiles_left * 200;
		int temp = current_score;
		level++;
		if (level < 3)
		{
			newGame();
			current_score = temp;
		}
		else
		{
			level = 0;
			if (updateHighScores())
			{
				game_state = NEW_HIGH_SCORE;
			}
			else
			{
				game_state = GAME_OVER_SCREEN;
			}
		}
		no_enemies_hit = 0;
	}


}

/**
*   @brief   Renders the scene
*   @details Renders all the game objects to the current frame.
Once the current frame is has finished the buffers are
swapped accordingly and the image shown.
*   @return  void
*/
void AngryBirdsGame::render(const ASGE::GameTime &)
{
	renderer->setFont(0);

	if (game_state == SPLASH_SCREEN)
	{
		renderSplash();
	}
	else if (game_state == MAIN_SCREEN)
	{
		renderMainMenu();
	}
	else if (game_state == IN_GAME)
	{
		renderInGame();
	}

	else if (game_state == GAME_OVER_SCREEN)
	{
		if (no_enemies_hit < NUM_ENEMIES)
		{
			renderGameOverL();
		}
		else
		{
			renderGameOverW();
		}
	}
	else if (game_state == NEW_HIGH_SCORE)
	{
		renderNewHighScore();
	}
	else if (game_state == HIGH_SCORES_SCREEN)
	{
		renderHighScores();
	}

}


/**
*   @brief   Splash Screen
*   @details This function is used to render the splash Screen
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::renderSplash()
{
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	renderer->renderSprite(*splash_screen);
}

/**
*   @brief   Main menu
*   @details This function is used todisplay the main menu
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::renderMainMenu()
{

	// Set Background colour
	renderer->setClearColour(ASGE::COLOURS::MIDNIGHTBLUE);
	renderer->renderSprite(*menu_layer.spriteComponent()->getSprite());
	// renders the main menu text
	renderer->renderText(
		"WELCOME TO ANGRY BIRDS \n Press Esc to quit at any time.", game_width * 0.2f,
		game_height * 0.15f, game_height * 0.002f, ASGE::COLOURS::DARKORANGE);

	renderer->renderText(menu_option == 0 ? ">PLAY" : "PLAY", game_width * 0.2f,
		game_height * 0.3f, game_height * 0.002f, ASGE::COLOURS::WHITESMOKE);

	renderer->renderText(menu_option == 1 ? ">HIGH SCORES" : "HIGH SCORES", game_width * 0.2f,
		game_height * 0.4f, game_height * 0.002f, ASGE::COLOURS::WHITESMOKE);

	renderer->renderText(menu_option == 2 ? ">QUIT" : "QUIT", game_width * 0.2f,
		game_height * 0.5f, game_height * 0.002f, ASGE::COLOURS::WHITESMOKE);




}

/**
*   @brief   In Game Screen
*   @details This function is used todisplay the main menu
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::renderInGame()
{
	renderer->renderSprite(*level_layer[level].spriteComponent()->getSprite());
	
	renderer->renderText("Score: ",
		(game_width * 0.60f), (game_height * 0.088f),
		game_height * 0.002f, ASGE::COLOURS::DARKORANGE);
	std::string score_string = std::to_string(current_score);
	renderer->renderText(score_string.c_str(),
		(game_width * 0.73f), (game_height * 0.088f),
		game_height * 0.002f, ASGE::COLOURS::DARKORANGE);

	std::string life_string = std::to_string(NUM_ENEMIES - no_enemies_hit);
	rect enemy_counter_sprite = enemy_counter.spriteComponent()->getBoundingBox();
	renderer->renderText(life_string.c_str(),
		(enemy_counter_sprite.x + (enemy_counter_sprite.length * 1.02f)),
		(enemy_counter_sprite.y + (enemy_counter_sprite.height * 0.95f)),
		game_height * 0.0025f, ASGE::COLOURS::DARKORANGE);


	if (bomb.getVisible() == true)
	{
		renderer->renderSprite(*bomb.spriteComponent()->getSprite());
	}
	for (int i = 0; i < NUM_PROJECTILES_SCATTER; i++)
	{
		if (projectiles_scatter[i].getVisible() == true)
		{
			renderer->renderSprite(*projectiles_scatter[i].spriteComponent()->getSprite());
		}

	}
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		if (projectiles[i].getVisible() == true)
		{
			renderer->renderSprite(*projectiles[i].spriteComponent()->getSprite());
		}

	}

	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if (blocks[i].getVisible() == true)
		{
			renderer->renderSprite(*blocks[i].spriteComponent()->getSprite());
		}

	}
	for (int i = 0; i < NUM_PLATFORMS; i++)
	{
		if (platforms[i].getVisible() == true)
		{
			renderer->renderSprite(*platforms[i].spriteComponent()->getSprite());
		}

	}
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemies[i].getVisible() == true)
		{
			renderer->renderSprite(*enemies[i].spriteComponent()->getSprite());
		}

	}
	if (slingshot.getVisible() == true)
	{
		renderer->renderSprite(*slingshot.spriteComponent()->getSprite());
	}
}

/**
*   @brief   Game Over loss
*   @details This function is used to display the game over screen
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::renderGameOverL()
{
	// Set Background colour
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	// renders the main menu text
	renderer->renderText(
		"GAME OVER out of lives \n Press Enter to return to main menu.",
		game_width * 0.25f, game_height * 0.2f, game_height * 0.002f, ASGE::COLOURS::DARKORANGE);
	renderer->renderText("Final Score: ", (game_width * 0.3f), (game_height * 0.5f),
		game_height * 0.004f, ASGE::COLOURS::WHITESMOKE);
	std::string score_string = std::to_string(current_score);
	renderer->renderText(score_string.c_str(),
		(game_width * 0.7f), (game_height * 0.50f),
		game_height * 0.004f, ASGE::COLOURS::WHITESMOKE);

}

/**
*   @brief   Game Over Win
*   @details This function is used todisplay the the game over screen
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::renderGameOverW()
{
	// Set Background colour
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	// renders the main menu text
	renderer->renderText(
		"CONGRATULATIONS you cleared the game \n Press Enter to return to main menu.", game_width * 0.25f,
		game_height * 0.2f, game_height * 0.002f, ASGE::COLOURS::DARKORANGE);
	renderer->renderText("Final Score: ", (game_width * 0.3f), (game_height * 0.5f),
		game_height * 0.004f, ASGE::COLOURS::WHITESMOKE);
	std::string score_string = std::to_string(current_score);
	renderer->renderText(score_string.c_str(),
		(game_width * 0.7f), (game_height * 0.50f),
		game_height * 0.004f, ASGE::COLOURS::WHITESMOKE);
}

/**
*   @brief   Update high scores
*   @details This function is used to check and update if single player high
score is higher than the current top ten
*   @see     KeyEvent
*   @return  bool
*/
bool AngryBirdsGame::updateHighScores()
{
	bool update_score = false;
	high_score_idx_to_update = 0;
	current_score = (current_score + (projectiles_left * 500));
	for (int i = 9; i > -1; i--)
	{
		if (current_score > high_scores[i].score)
		{
			high_score_idx_to_update = i;
			update_score = true;
		}
	}
	if (update_score)
	{
		for (int i = 9; i > high_score_idx_to_update; i--)
		{
			high_scores[i].score = high_scores[i - 1].score;
			high_scores[i].initials = high_scores[i - 1].initials;
		}
		high_scores[high_score_idx_to_update].score = current_score;

	}
	current_score = 0;
	return update_score;
}

/**
*   @brief   High scores
*   @details This function is used to display the high scores
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::renderHighScores()
{
	// re renders the main menu
	renderMainMenu();


	renderer->renderText("HIGH SCORES", game_width * 0.80f, game_height * 0.15f, game_height * 0.002f,
		ASGE::COLOURS::DARKORANGE);
	// renders the high scores
	int j = 0;
	for (int i = game_height * 0.25f; i < game_height * 0.75f; i = i + game_height * 0.05f)
	{
		renderer->renderText(high_scores[j].initials.c_str(), game_width * 0.82f, i, game_height * 0.002f,
			ASGE::COLOURS::GHOSTWHITE);
		// creates a string with the score appended
		std::string score_str_1 = std::to_string(high_scores[j].score);
		renderer->renderText(score_str_1.c_str(), game_width * 0.87f, i, game_height * 0.002f,
			ASGE::COLOURS::GHOSTWHITE);
		j++;
	}
	renderer->renderText("Press Enter\n to return\n to Main Menu", game_width * 0.8f, game_height * 0.8f,
		game_height * 0.002f, ASGE::COLOURS::GHOSTWHITE);
}

/**
*   @brief   New High score
*   @details This function is used to display the high score and update the
players initials
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::renderNewHighScore()
{

	renderer->renderText("CONGRATULATIONS YOU SCORED A NEW HIGH SCORE",
		game_width * 0.1f, game_height * 0.15f, game_height * 0.003f, ASGE::COLOURS::DARKORANGE);
	int j = 0;
	for (int i = game_height * 0.25f; i < game_height * 0.75f; i = i + game_height * 0.05f)
	{
		renderer->renderText(high_score_idx_to_update == j ?
			new_initials.c_str() : high_scores[j].initials.c_str(),
			game_width * 0.45f, i, game_height * 0.002f, high_score_idx_to_update == j ?
			ASGE::COLOURS::GHOSTWHITE : ASGE::COLOURS::DARKORANGE);
		std::string score_str_1 = std::to_string(high_scores[j].score);
		renderer->renderText(score_str_1.c_str(), game_width * 0.5f, i, game_height * 0.002f,
			high_score_idx_to_update == j ?
			ASGE::COLOURS::GHOSTWHITE : ASGE::COLOURS::DARKORANGE);
		j++;
	}



	renderer->renderText(
		"Use arrow keys to change initials and press Enter when finished", game_width * 0.1f, game_height * 0.8f,
		game_height * 0.002f, ASGE::COLOURS::GHOSTWHITE);
}

/**
*   @brief   New Game
*   @details This function is used to reset a new game
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::newGame()
{
	setupLevel(level);

	// re-initialise bomb and scatter sprites
	resetBomb();
	resetProjectileScatter();
	// re-initialise game variables
	current_score = 0;
	new_game = false;
	flying = false;
	projectile = 0;
	projectiles_left = 5;
	no_enemies_hit = 0;
}

/**
*   @brief   Setup Level 
*   @details This function is used to setup a new level
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::setupLevel(int level)
{
	int map = (rand() % 3);
	rect projectile_platform;
	
	loadLevelMap(map);
	setupPigs(map);
	projectile_platform = setupPlatforms(map);
	setupProjectiles(projectile_platform);
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		ASGE::Sprite* block_sprite = blocks[level_map[i].block_index].spriteComponent()->getSprite();
		block_sprite->xPos(grid_X[level_map[i].x_index]);
		block_sprite->yPos(grid_Y[level_map[i].y_index]);
		blocks[i].setVisible(true);
	}
}

/**
*   @brief   Setup Projectiles
*   @details This function is used to setup the projectile and slingshot positions
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::setupProjectiles(rect projectile_platform)
{

	ASGE::Sprite* slingshot_sprite = slingshot.spriteComponent()->getSprite();
	slingshot_sprite->yPos(projectile_platform.y - slingshot_sprite->height());
	slingshot_sprite->xPos(projectile_platform.x + projectile_platform.length - 
		(slingshot_sprite->width() * 0.6f));
	slingshot.setVisible(true);
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{

		ASGE::Sprite* projectile_sprite = projectiles[i].spriteComponent()->getSprite();
		projectiles[i].setVisible(true);
		if (i == 0)
		{
			projectile_sprite->yPos(slingshot_sprite->yPos() - (projectile_sprite->height() * 0.1f));
			projectile_sprite->xPos(slingshot_sprite->xPos() + (slingshot_sprite->width() *0.4f) - 
				(projectile_sprite->width()*0.5f));
			slingshot_center.setX(projectile_sprite->xPos());
			slingshot_center.setY(projectile_sprite->yPos());
			aiming_area.x = projectile_sprite->xPos() - gameplay_area.length * 0.07f;
			aiming_area.y = projectile_sprite->yPos() - gameplay_area.height * 0.03f;
			aiming_area.height = gameplay_area.height * 0.07f;
			aiming_area.length = gameplay_area.length * 0.07f;

		}
		else
		{
			projectile_sprite->yPos(projectile_platform.y - projectile_sprite->height());
			projectile_sprite->xPos(projectile_platform.x + projectile_platform.length - 
				(((i + 1) * 1.5f) * (game_height * PROJECTILE_SIZE)));
		}

	}
}

/**
*   @brief   Setup pigs
*   @details This function is used to setup the enemy positions
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::setupPigs(int map)
{
	if (level == 0)
	{
		switch (map)
		{
		case 0:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.66f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.6f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.47f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.5f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.69f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.73f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.68f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.505f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.79f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.48f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.48f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.6f));
			enemies[5].setVisible(true);
			break;
		}
		case 1:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.345f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.55f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.46f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.65f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.68f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.72f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.555f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.532f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.76f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.47f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.77f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.66f));
			enemies[5].setVisible(true);
			break;
		}
		case 2:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.725f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.655f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.485f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.57f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.3f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.715f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.335f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.532f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.705f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.475f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.815f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.755f));
			enemies[5].setVisible(true);
			break;
		}
		}
	}
	else if (level == 1)
	{
		switch (map)
		{

		case 0:
		{

			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.635f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.655f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.445f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.525f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.315f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.715f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.605f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.532f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.715f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.495f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.725f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.755f));
			enemies[5].setVisible(true);
			break;
		}

		case 1:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.495f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.725f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.665f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.495f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.325f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.575f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.325f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.675f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.705f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.555f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.755f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.795f));
			enemies[5].setVisible(true);
			break;
		}
		case 2:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.46f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.750f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.6f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.435f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.255f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.575f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.65f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.545f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.74f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.595f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.705f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.795f));
			enemies[5].setVisible(true);


			break;
		}
		}
	}
	else if (level == 2)
	{
		switch (map)
		{
		case 0:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.720f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.592f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.555f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.725f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.69f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.73f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.68f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.505f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.775f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.455f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.455f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.6f));
			enemies[5].setVisible(true);
			break;
		}

		case 1:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.66f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.725f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.37f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.525f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.825f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.73f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.615f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.505f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.805f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.515f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.415f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.675f));
			enemies[5].setVisible(true);
			break;
		}
		case 2:
		{
			ASGE::Sprite* enemy_sprite = enemies[0].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.66f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.725f));
			enemies[0].setVisible(true);

			enemy_sprite = enemies[1].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.305f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.525f));
			enemies[1].setVisible(true);

			enemy_sprite = enemies[2].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.795f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.835f));
			enemies[2].setVisible(true);

			enemy_sprite = enemies[3].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.585f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.535f));
			enemies[3].setVisible(true);

			enemy_sprite = enemies[4].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.655f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.455f));
			enemies[4].setVisible(true);

			enemy_sprite = enemies[5].spriteComponent()->getSprite();
			enemy_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.435f));
			enemy_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.635f));
			enemies[5].setVisible(true);

			break;
		}
		}
	}
}

/**
*   @brief   Setup platforms
*   @details This function is used to setup the platform positions
*   @see     KeyEvent
*   @return  rect
*/
rect AngryBirdsGame::setupPlatforms(int map)
{
	rect projectile_platform;
	if (level == 0)
	{

		ASGE::Sprite* platform_sprite = platforms[0].spriteComponent()->getSprite();
		platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.8f));
		platform_sprite->xPos(gameplay_area.x);
		platforms[0].setVisible(true);
		platform_sprite = platforms[1].spriteComponent()->getSprite();
		platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.8f));
		platform_sprite->xPos(gameplay_area.x + platform_sprite->width());
		platforms[1].setVisible(true);
		projectile_platform = platforms[1].spriteComponent()->getBoundingBox();
		switch (map)
		{
		case 0:
		{
			ASGE::Sprite* platform_sprite = platforms[0].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.8f));
			platform_sprite->xPos(gameplay_area.x);
			platforms[0].setVisible(true);
			platform_sprite = platforms[1].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.8f));
			platform_sprite->xPos(gameplay_area.x + platform_sprite->width());
			platforms[1].setVisible(true);

			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.83f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.43f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.79f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.56f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.73f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.68f));
			platforms[4].setVisible(true);
			break;
		}
		case 1:
		{

			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.78f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.47f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.79f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.58f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.715f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.68f));
			platforms[4].setVisible(true);

			platform_sprite = platforms[5].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.615f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.62f));
			platform_sprite->width(platform_sprite->width() * 0.58f);
			platforms[5].setVisible(true);


			platform_sprite = platforms[6].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.74f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.556f));
			platform_sprite->width(platform_sprite->width() * 0.4f);
			platforms[6].setVisible(true);
			break;
		}
		case 2:
		{



			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.825f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.44f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.825f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.54f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.765f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.62f));
			platforms[4].setVisible(true);


			platform_sprite = platforms[5].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.425f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.48f));
			platforms[5].setVisible(true);

			platform_sprite = platforms[6].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.525f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.66f));
			platforms[6].setVisible(true);

			platform_sprite = platforms[7].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.85f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.72f));
			platforms[7].setVisible(true);
			break;
		}
		}
	}
	else if (level == 1)
	{
		ASGE::Sprite* platform_sprite = platforms[0].spriteComponent()->getSprite();
		platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.72f));
		platform_sprite->xPos(gameplay_area.x);
		platforms[0].setVisible(true);
		platform_sprite = platforms[1].spriteComponent()->getSprite();
		platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.72f));
		platform_sprite->xPos(gameplay_area.x + platform_sprite->width());
		platforms[1].setVisible(true);
		projectile_platform = platforms[1].spriteComponent()->getBoundingBox();
		switch (map)
		{

		case 0:
		{

			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.83f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.46f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.676f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.66f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.89f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.72f));
			platforms[4].setVisible(true);

			platform_sprite = platforms[5].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.505f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.48f));
			platforms[5].setVisible(true);
			break;
		}

		case 1:
		{

			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.788f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.44f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.788f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.54f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.79f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.7f));
			platforms[4].setVisible(true);

			platform_sprite = platforms[5].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.40f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.50f));
			platforms[5].setVisible(true);

			platform_sprite = platforms[6].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.40f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.60f));
			platforms[6].setVisible(true);
			break;
		}
		case 2:
		{
			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.64f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.40f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.816f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.54f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.765f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.71f));
			platforms[4].setVisible(true);


			platform_sprite = platforms[5].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.365f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.54f));
			platforms[5].setVisible(true);
			break;
		}
		}
	}
	else if (level == 2)
	{
		ASGE::Sprite* platform_sprite = platforms[0].spriteComponent()->getSprite();
		platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.775f));
		platform_sprite->xPos(gameplay_area.x);
		platforms[0].setVisible(true);
		platform_sprite = platforms[1].spriteComponent()->getSprite();
		platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.775f));
		platform_sprite->xPos(gameplay_area.x + platform_sprite->width());
		platforms[1].setVisible(true);
		projectile_platform = platforms[1].spriteComponent()->getBoundingBox();
		switch (map)
		{
		case 0:
		{

			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.855f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.43f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.865f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.56f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.715f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.68f));
			platforms[4].setVisible(true);
			break;
		}

		case 1:
		{
			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.826f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.47f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.515f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.48f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.852f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.68f));
			platforms[4].setVisible(true);

			platform_sprite = platforms[5].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.54f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.62f));
			platforms[5].setVisible(true);
			break;
		}
		case 2:
		{
			platform_sprite = platforms[2].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.715f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.44f));
			platforms[2].setVisible(true);

			platform_sprite = platforms[3].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.715f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.54f));
			platforms[3].setVisible(true);

			platform_sprite = platforms[4].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.815f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.65f));
			platforms[4].setVisible(true);


			platform_sprite = platforms[5].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.455f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.48f));
			platforms[5].setVisible(true);

			platform_sprite = platforms[6].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.455f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.58f));
			platforms[6].setVisible(true);

			platform_sprite = platforms[7].spriteComponent()->getSprite();
			platform_sprite->yPos(gameplay_area.y + (gameplay_area.height * 0.815f));
			platform_sprite->xPos(gameplay_area.x + (gameplay_area.length* 0.75f));
			platforms[7].setVisible(true);
			break;
		}
		}
	}
	return(projectile_platform);
}
/**
*   @brief   Projectile Collision
*   @details This function is used to detect collisions projectile
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::projectileScatterCollision()
{
	for (int j = 0; j < NUM_PROJECTILES_SCATTER; j++)
	{
		rect projectile_rect = projectiles_scatter[j].spriteComponent()->getBoundingBox();
		vector2 projectile_vel = projectiles_scatter[j].getVelocity();
		for (int i = 0; i < NUM_PLATFORMS; i++) {
			rect platformRect = platforms[i].spriteComponent()->getBoundingBox();
			if ((projectile_rect.isInside(platformRect) ||
				platformRect.isInside(projectile_rect)) && platforms[i].getVisible())
			{

				vector2 position_projectile(projectile_rect.x, projectile_rect.y);
				vector2 position_platform(platformRect.x, platformRect.y);
				vector2 distance = (position_projectile.subtract(position_platform));
				float dist2 = position_projectile.getDistance(position_platform);

				if (dist2 <= projectile_rect.length)
				{
					vector2 reflection_angle;
					if (distance.getMagnitude() != 0.0f)
					{
						reflection_angle = distance.multiply((
							projectile_rect.length - distance.getMagnitude()) /
							distance.getMagnitude());

					}
					else
					{
						distance.setX(projectile_rect.length);
						distance.setY(0.0f);

						reflection_angle = distance.multiply((projectile_rect.length -
							(projectile_rect.length - 1.0f)) / (projectile_rect.length - 1.0f));
					}

					vector2 v = projectile_vel;
					float vn = v.getScalar(reflection_angle.normalise());

					if (vn <= 0.0f)
					{
						float im1 = 1.f;
						float im2 = 10.f;
						float imp = (-(1.0f + RESTITUTION) * vn) / (im1 + im2);
						vector2 impulse(reflection_angle.multiply(imp));

						projectile_vel = projectile_vel.add(impulse.multiply(im1));
						projectile_vel.setX(0.f - (projectile_vel.getX() * 0.8f));
						projectile_vel.setY(0.f - (projectile_vel.getY() * 0.2f));

						while (dist2 <= (projectile_rect.length + 0.01f))
						{
							position_projectile.setX(position_projectile.getX() + projectile_vel.getX());
							position_projectile.setY(position_projectile.getY() + projectile_vel.getY());
							dist2 = position_projectile.getDistance(position_platform);
						}
					}
					ASGE::Sprite* projectileSprite = projectiles[projectile].spriteComponent()->getSprite();
					projectileSprite->xPos(position_projectile.getX());
					projectileSprite->yPos(position_projectile.getY());
					projectiles[projectile].setVelocity(projectile_vel.getX(), projectile_vel.getY());
				}
			}

		}
		if (projectile_rect.x < gameplay_area.x || projectile_rect.y < gameplay_area.y ||
			projectile_rect.x + projectile_rect.length > gameplay_area.x + gameplay_area.length ||
			projectile_rect.y + projectile_rect.height > gameplay_area.y + gameplay_area.height)
		{
			projectiles_scatter[j].setVisible(false);
		}

		for (int k = 0; k < NUM_BLOCKS; k++)
		{
			rect block = blocks[k].spriteComponent()->getBoundingBox();
			if (blocks[k].getVisible())
			{
				if (projectile_rect.isInside(block))
				{
					current_score += 5;
					blocks[k].setVisible(false);
					if (k < 10)
					{

						vector2 vel = projectiles_scatter[projectile].getVelocity();
						projectiles_scatter[j].setVelocity(vel.getX() - (vel.getX() * .10f),
							vel.getY());
					}
					else if (k < 30)
					{
						current_score += 5;
						vector2 vel = projectiles_scatter[j].getVelocity();
						projectiles_scatter[j].setVelocity(vel.getX() - (vel.getX() * .10f),
							vel.getY());
					}
					else if (k < 40)
					{
						current_score += 10;
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .25f),
							vel.getY());

					}
					else if (k < NUM_BLOCKS)
					{
						current_score += 50;
					}
				}
			}
		}
	}

}

/**
*   @brief   Projectile Collision
*   @details This function is used to detect collisions projectile
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::projectileCollision()
{
	rect projectile_rect = projectiles[projectile].spriteComponent()->getBoundingBox();
	vector2 projectile_vel = projectiles[projectile].getVelocity();
	for (int i = 0; i < NUM_PLATFORMS; i++) {
		rect platformRect = platforms[i].spriteComponent()->getBoundingBox();
		if ((projectile_rect.isInside(platformRect) || 
			platformRect.isInside(projectile_rect)) && platforms[i].getVisible())
		{

			vector2 position_projectile(projectile_rect.x, projectile_rect.y);
			vector2 position_platform(platformRect.x, platformRect.y);
			vector2 distance = (position_projectile.subtract(position_platform));
			float dist2 = position_projectile.getDistance(position_platform);

			if (dist2 <= projectile_rect.length)
			{
				vector2 reflection_angle;
				if (distance.getMagnitude() != 0.0f)
				{
					reflection_angle = distance.multiply((
						projectile_rect.length - distance.getMagnitude()) /
						distance.getMagnitude());

				}
				else
				{
					distance.setX(projectile_rect.length);					
					distance.setY(0.0f);

					reflection_angle = distance.multiply((projectile_rect.length -
						(projectile_rect.length - 1.0f)) / (projectile_rect.length - 1.0f));
				}

				vector2 v = projectile_vel;
				float vn = v.getScalar(reflection_angle.normalise());

				if (vn <= 0.0f)
				{
					float im1 = 1.f;
					float im2 = 10.f;
					float imp = (-(1.0f + RESTITUTION) * vn) / (im1 + im2);
					vector2 impulse(reflection_angle.multiply(imp));

					projectile_vel = projectile_vel.add(impulse.multiply(im1));
					projectile_vel.setX(0.f - (projectile_vel.getX() * 0.8f));
					projectile_vel.setY(0.f - (projectile_vel.getY() * 0.8f));
					
					while (dist2 <= (projectile_rect.length + 0.01f))
					{
						position_projectile.setX(position_projectile.getX() + projectile_vel.getX());
						position_projectile.setY(position_projectile.getY() + projectile_vel.getY());
						dist2 = position_projectile.getDistance(position_platform);
					}
				}
				ASGE::Sprite* projectileSprite = projectiles[projectile].spriteComponent()->getSprite();
				projectileSprite->xPos(position_projectile.getX());
				projectileSprite->yPos(position_projectile.getY());
				projectiles[projectile].setVelocity(projectile_vel.getX(), projectile_vel.getY());
			}
		}

	}
	if (projectile_rect.x < gameplay_area.x || projectile_rect.y < gameplay_area.y ||
		projectile_rect.x + projectile_rect.length > gameplay_area.x + gameplay_area.length ||
		projectile_rect.y + projectile_rect.height > gameplay_area.y + gameplay_area.height)
	{
		projectiles[projectile].setVisible(false);
		projectiles_left--;
		resetProjectiles();
		flying = false;
		if (projectiles_left == 0)
		{
			if (updateHighScores())
			{
				game_state = NEW_HIGH_SCORE;
			}
			else
			{
				game_state = GAME_OVER_SCREEN;
			}
		}
	}
	
}

/**
*   @brief   Level Collision
*   @details This function is used to detect collisions with the level
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::levelCollision()
{
	rect projectile_rect = projectiles[projectile].spriteComponent()->getBoundingBox();
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		rect block = blocks[i].spriteComponent()->getBoundingBox();
		if (blocks[i].getVisible())
		{
			if (projectile_rect.isInside(block))
			{
				current_score += 5;
				blocks[i].setVisible(false);
				if (i < 10)
				{
					if (projectile == 3)
					{
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .05f),
							vel.getY());
					}
					else
					{
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .10f),
							vel.getY());
					}
				}
				else if (i < 30)
				{
					current_score += 5;
					if (projectile == 4)
					{
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .10f),
							vel.getY());

					}
					else if(projectile == 2)
					{
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .05f),
							vel.getY());
					}
					else
					{
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .15f),
							vel.getY());
					}
				}
				else if (i < 40)
				{
					current_score += 10;
					if (projectile < 4)
					{
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .25f),
							vel.getY());

					}
					else
					{
						vector2 vel = projectiles[projectile].getVelocity();
						projectiles[projectile].setVelocity(vel.getX() - (vel.getX() * .10f),
							vel.getY());
					}
				}
				else if (i < NUM_BLOCKS)
				{
					current_score += 50;
				}
			}
		}
	}

}

/**
*   @brief   Enemy Collision
*   @details This function is used to detect collisions with the enemy pigs
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::enemyCollision()
{
	
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemies[i].getVisible())
		{
			float newVelY = 1.f;
			rect enemy_rect = enemies[i].spriteComponent()->getBoundingBox();
			for (int j = 0; j < NUM_BLOCKS; j++)
			{
				if (blocks[j].getVisible())
				{
					rect block = blocks[j].spriteComponent()->getBoundingBox();
					if (enemy_rect.y + enemy_rect.height > block.y && 
						enemy_rect.isBetween(enemy_rect.x + (enemy_rect.length * 0.5f),
							block.x - (enemy_rect.length * 0.5f),
							block.x + block.length + (enemy_rect.length * 0.5f)))
					{
						newVelY = 0.f;
						ASGE::Sprite* enemySprite = enemies[i].spriteComponent()->getSprite();
						enemySprite->yPos(block.y - enemy_rect.height);
					}
				}
			}
			for (int k = 0; k < NUM_PLATFORMS; k++)
			{
				if (platforms[k].getVisible())
				{
					rect platform = platforms[k].spriteComponent()->getBoundingBox();
					if (enemy_rect.isBetween(enemy_rect.y + (enemy_rect.height * 0.5f),
						platform.y - (enemy_rect.height * 0.5f),
						platform.y + platform.height) &&
						enemy_rect.isBetween(enemy_rect.x + (enemy_rect.length * 0.5f),
							platform.x - (enemy_rect.length * 0.5f),
							platform.x + platform.length + (enemy_rect.length * 0.5f)))
					{
						newVelY = 0.f;
						ASGE::Sprite* enemySprite = enemies[i].spriteComponent()->getSprite();
						enemySprite->yPos(platform.y - enemy_rect.height);
					}
				}
			}
			vector2 vel = enemies[i].getVelocity();
			enemies[i].setVelocity(vel.getX(), newVelY);

			rect projectile_rect = projectiles[projectile].spriteComponent()->getBoundingBox();
			if ((projectile_rect.isInside(enemy_rect) || 
				enemy_rect.isInside(projectile_rect)) && enemies[i].getVisible())
			{
				current_score += 150;
				enemies[i].setVisible(false);
				no_enemies_hit++;
			}
			for (int j = 0; j < NUM_PROJECTILES_SCATTER; j++)
			{
				rect projectile_scatter_rect = projectiles_scatter[j].spriteComponent()->getBoundingBox();
				if ((projectile_scatter_rect.isInside(enemy_rect) ||
					enemy_rect.isInside(projectile_scatter_rect)) && enemies[i].getVisible() && projectiles_scatter[j].getVisible())
				{
					current_score += 150;
					enemies[i].setVisible(false);
					no_enemies_hit++;
				}
			}
		}

	}
}

/**
*   @brief   Release Bomb
*   @details This function is used to release a bomb from the
provided projectiles co-ordinates
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::releaseBomb(rect projectile)
{
	ASGE::Sprite* bomb_sprite = bomb.spriteComponent()->getSprite();
	bomb_sprite->yPos(projectile.y);
	bomb_sprite->xPos(projectile.x + ((projectile.length * 0.5f) - (bomb_sprite->width() * 0.5f)));
	bomb_sprite->height(gameplay_area.height * BOMB_SIZE);
	bomb_sprite->width(gameplay_area.height * BOMB_SIZE);
	bomb.setVelocity(0.f, 1.f);
	bomb.setVisible(true);	
}

/**
*   @brief   Reset Bomb
*   @details This function is used to reset a bomb
position and velocity
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::resetBomb()
{
	bomb_active = false;
	// reset sprite position, velocity and visibility
	ASGE::Sprite* bomb_sprite = bomb.spriteComponent()->getSprite();
	bomb_sprite->yPos(0.f);
	bomb_sprite->xPos(0.f);
	bomb_sprite->height(gameplay_area.height * BOMB_SIZE);
	bomb_sprite->width(gameplay_area.height * BOMB_SIZE);
	bomb.setVisible(false);
	bomb.setVelocity(0.f, 0.f);
}

/**
*   @brief   Release Projectile Scatter
*   @details This function is used to reset a scatter shot's
position and velocity
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::releaseProjectileScatter(rect projectile, vector2 velocity)
{
	for (int i = 0; i < NUM_PROJECTILES_SCATTER; i++)
	{
		// reset sprite position, velocity and visibility
		ASGE::Sprite* projectiles_scatter_sprite = projectiles_scatter[i].spriteComponent()->getSprite();
		projectiles_scatter_sprite->yPos(projectile.y);
		projectiles_scatter_sprite->xPos(projectile.x);
		projectiles_scatter_sprite->height(projectile.height);
		projectiles_scatter_sprite->width(projectile.length);
		projectiles_scatter[i].setVisible(true);
		if (i == 1)
		{
			projectiles_scatter[i].setVelocity(velocity.getX(), velocity.getY() - 4.f);
		}
		else
		{
			projectiles_scatter[i].setVelocity(velocity.getX(), velocity.getY() + 4.f);
		}
	}
}

/**
*   @brief   Reset Projectile Scatter
*   @details This function is used to reset a scatter shot's
position and velocity
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::resetProjectileScatter()
{
	for (int i = 0; i < NUM_PROJECTILES_SCATTER; i++)
	{
		// reset sprite position, velocity and visibility
		scatter_active = false;
		ASGE::Sprite* projectiles_scatter_sprite = projectiles_scatter[i].spriteComponent()->getSprite();
		projectiles_scatter_sprite->rotationInRadians(0.f);
		projectiles_scatter_sprite->yPos(0.f);
		projectiles_scatter_sprite->xPos(0.f);
		projectiles_scatter[i].setVisible(false);
		projectiles_scatter[i].setVelocity(0.f, 0.f);
	}
}

/**
*   @brief   Bomb Collision
*   @details This function is used to boost the projectiles velocity
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::bombCollision()
{
	rect explosion;
	explosion.x = 0;
	explosion.y = 0;
	explosion.height = 0;
	explosion.length = 0;
	rect bomb_rect = bomb.spriteComponent()->getBoundingBox();
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemies[i].getVisible())
		{
			rect enemy_rect = enemies[i].spriteComponent()->getBoundingBox();
			if (bomb_rect.isInside(enemy_rect) || enemy_rect.isInside(bomb_rect) && bomb.getVisible())
			{
				bomb.setVisible(false);
				current_score += 150;
				enemies[i].setVisible(false);
				no_enemies_hit++;
				explosion.x = bomb_rect.x - bomb_rect.length * 4.f;
				explosion.y = bomb_rect.y - bomb_rect.length * 4.f;
				explosion.length = bomb_rect.length * 9.f;
				explosion.height = bomb_rect.height * 9.f;
			}
		}
	}
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if (blocks[i].getVisible())
		{
			rect block_rect = blocks[i].spriteComponent()->getBoundingBox();
			if (bomb_rect.isInside(block_rect) || block_rect.isInside(bomb_rect) && bomb.getVisible())
			{
				bomb.setVisible(false);
				if (i < 10)
				{
					current_score += 5;
				}
				else if (i < 30)
				{
					current_score += 10;
				}
				else if (i < 40)
				{
					current_score += 15;
				}			
				blocks[i].setVisible(false);
				explosion.x = bomb_rect.x - bomb_rect.length * 4.f;
				explosion.y = bomb_rect.y - bomb_rect.length * 4.f;
				explosion.length = bomb_rect.length * 9.f;
				explosion.height = bomb_rect.height * 9.f;
			}
		}
	}
	for (int i = 0; i < NUM_PLATFORMS; i++)
	{
		if (platforms[i].getVisible())
		{
			rect platform_rect = platforms[i].spriteComponent()->getBoundingBox();
			if (bomb_rect.isInside(platform_rect) || platform_rect.isInside(bomb_rect) && bomb.getVisible())
			{
				bomb.setVisible(false);
				explosion.x = bomb_rect.x - bomb_rect.length * 4.f;
				explosion.y = bomb_rect.y - bomb_rect.length * 4.f;
				explosion.length = bomb_rect.length * 9.f;
				explosion.height = bomb_rect.height * 9.f;
			}
		}
	}
	if (bomb_rect.y + bomb_rect.height > gameplay_area.y)
	{
		bomb.setVisible(false);
	}
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemies[i].getVisible())
		{
			rect enemy_rect = enemies[i].spriteComponent()->getBoundingBox();
			if (explosion.isInside(enemy_rect) || enemy_rect.isInside(explosion) && bomb.getVisible())
			{
				bomb.setVisible(false);
				current_score += 150;
				enemies[i].setVisible(false);
				no_enemies_hit++;
			}
		}
	}
	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if (blocks[i].getVisible())
		{
			rect block_rect = blocks[i].spriteComponent()->getBoundingBox();
			if (explosion.isInside(block_rect) || block_rect.isInside(explosion) && bomb.getVisible())
			{
				bomb.setVisible(false);
				if (i < 10)
				{
					current_score += 5;
				}
				else if (i < 30)
				{
					current_score += 10;
				}
				else if (i < 40)
				{
					current_score += 15;
				}
				blocks[i].setVisible(false);
			}
		}
	}
}

/**
*   @brief   Boost Projectile 
*   @details This function is used to boost the projectiles velocity
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::boostProjectile()
{
	vector2 projectile_velocity = projectiles[projectile].getVelocity();
	projectiles[projectile].setVelocity(projectile_velocity.getX() * 2.f, projectile_velocity.getY() * 2.f);
}

/**
*   @brief   Load files
*   @details This function is load the high scores and characters files
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::loadFiles()
{
	// load high scores
	std::ifstream inFile_one;
	inFile_one.open("High_scores.txt");
	if (!inFile_one.fail())
	{
		for (int i = 0; i < NUM_HIGH_SCORES; i++)
		{
			std::string score;
			getline(inFile_one, high_scores[i].initials);
			getline(inFile_one, score);
			high_scores[i].score = atoi(score.c_str());
		}
		inFile_one.close();
	}
}

/**
*   @brief   Save files
*   @details This function is load the high scores and characters files
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::saveHighScores()
{
	// save high scores array to file
	std::ofstream outFile;
	outFile.open("High_scores.txt");
	if (!outFile.fail())
	{
		for (int i = 0; i < NUM_HIGH_SCORES; i++)
		{
			outFile << high_scores[i].initials << std::endl;
			outFile << high_scores[i].score << std::endl;
		}
		outFile.close();
	}

}

/**
*   @brief   clear arrays
*   @details This function is used to initialise arrays.
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::clearArrays()
{
	for (int i = 0; i < NUM_HIGH_SCORES; i++)
	{
		high_scores[i].initials = "AAA";
		high_scores[i].score = 0;
	}
}


/**
*   @brief   Save files
*   @details This function is load the high scores and characters files
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::saveLevelMap()
{
	// save high scores array to file
	std::ofstream outFile;
	outFile.open("level_map_8.txt");
	if (!outFile.fail())
	{
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			outFile << level_map[i].block_index << std::endl;
			outFile << level_map[i].x_index << std::endl;
			outFile << level_map[i].y_index << std::endl;
		}
		outFile.close();
	}

}

/**
*   @brief   Load files
*   @details This function is load the high scores and characters files
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::loadLevelMap(int map)
{
	// load high scores
	int map_to_load = map + (level * 3);
	std::ifstream inFile_one;
	std::string level_map_filename;
	level_map_filename = "Resources\\Levels\\level_map_" + std::to_string(map_to_load) + ".txt";
	inFile_one.open(level_map_filename);
	if (!inFile_one.fail())
	{
		for (int i = 0; i < NUM_BLOCKS; i++)
		{
			std::string a, b, c;
			getline(inFile_one, a);
			getline(inFile_one, b);
			getline(inFile_one, c);
			level_map[i].block_index = atoi(a.c_str());
			level_map[i].x_index = atoi(b.c_str());
			level_map[i].y_index = atoi(c.c_str());
		}
		inFile_one.close();
	}
}


/**
*   @brief   Reset Projectiles
*   @details This function is used to reset the projectile and slingshot positions
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::resetProjectiles()
{
	if (bomb_active)
	{
		resetBomb();
	}
	if (scatter_active)
	{
		resetProjectileScatter();
	}
	if (boost_active)
	{
		boost_active = false;
	}
	if (wind_active)
	{
		wind_active = false;
	}
	bool firstVisible = false;
	int numProjectiles = 0;
	rect platform = platforms[0].spriteComponent()->getBoundingBox();
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		if (projectiles[i].getVisible() == true)
		{
			ASGE::Sprite* projectile_sprite = projectiles[i].spriteComponent()->getSprite();
			projectile_sprite->rotationInRadians(0.f);
			if (!firstVisible)
			{
				firstVisible = true;
				projectile_sprite->yPos(slingshot_center.getY());
				projectile_sprite->xPos(slingshot_center.getX());
				numProjectiles++;
				projectile = i;
			}

			else
			{
				projectile_sprite->yPos(platform.y - projectile_sprite->height());
				projectile_sprite->xPos(platform.x + platform.length -
					(((numProjectiles - 1) * 1.5f) * (game_height * PROJECTILE_SIZE)));
				numProjectiles++;
			}
		}
	}
}

/**
*   @brief   Wind Breath
*   @details This function is used to detect collisions with the wind breath
*   @see     KeyEvent
*   @return  void
*/
void AngryBirdsGame::windBreath()
{
	rect wind = projectiles[projectile].spriteComponent()->getBoundingBox();
	wind.x = wind.x + wind.length;
	wind.length = wind.length * 4.f;
	wind.y = wind.y - wind.height;
	wind.height = wind.height * 3.f;
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemies[i].getVisible())
		{
			rect enemy_rect = enemies[i].spriteComponent()->getBoundingBox();

			if (wind.isInside(enemy_rect) ||
				enemy_rect.isInside(wind) && enemies[i].getVisible())
			{
				current_score += 150;
				enemies[i].setVisible(false);
				no_enemies_hit++;
			}
		}

	}


	for (int i = 0; i < NUM_BLOCKS; i++)
	{
		if (blocks[i].getVisible())
		{
			rect block_rect = blocks[i].spriteComponent()->getBoundingBox();
			if (wind.isInside(block_rect) || block_rect.isInside(wind) && bomb.getVisible())
			{
				if (i < 10)
				{
					current_score += 5;
				}
				else if (i < 30)
				{
					current_score += 10;
				}
				else if (i < 40)
				{
					current_score += 15;
				}
				blocks[i].setVisible(false);
			}
		}
	}
}