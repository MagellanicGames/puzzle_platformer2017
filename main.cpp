//#define RELEASE //if compiling in release, ensure this line is not commented out!
#include <iostream>
#include <sstream>
#include <iomanip>
#ifdef RELEASE
#include <Windows.h>
#endif // RELEASE


#define STB_IMAGE_IMPLEMENTATION
#include "include\stb_image.h"

#include "include\Common.h"
#include "include\MenuGameState.h"
#include "include\Drawing.h"
#include "include\EventQueue.h"
#include "include\LevelStartGameState.h"
#include "include\PlayState.h"
#include "include\PlayState3d.h"
#include "include\EditState.h"
#include "include\OptionState.h"
#include "include\OverworldState.h"

std::string fontPath("Fonts/Munro.ttf");
std::string gameName("PuzzleGame 2017 ");

double versionNumber = 1.0;


ptr<sf::RenderWindow> currentWindow;


std::map<std::string, ptr<Level>> level_store;

ptr<Level> current_level;

void restartLevel(PlayState3d & playState, EditState & editState, OverworldState & overworldState);


std::string versionToString()
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << versionNumber;
	return "v" + ss.str();
}

#ifndef RELEASE

int main()
#else
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)

#endif
{
	Screen::initialise();	
	Screen::setVideoMode(currentWindow, gameName + versionToString(), true);

	SFML_utils::Text::initialise(fontPath);
	Dim::initialise();
	Drawing::initialise();
	Sound::initialise();

	Time time;
	std::cout << sizeof(Level) << "\n";
	std::string level_path = "level1.lvl";
	level_store["level1"] = ptr<Level>(GameState::f_loadLevelFromFile(level_path));
	level_store["level2"] = ptr<Level>(GameState::f_loadLevelFromFile(level_path));
	level_store["level3"] = ptr<Level>(GameState::f_loadLevelFromFile(level_path));
	level_store["level4"] = ptr<Level>(GameState::f_loadLevelFromFile(level_path));
	current_level.reset(new Level(*level_store["level1"].get()));
	ptr<MenuGameState> menuState(new MenuGameState());
	ptr<LevelStartGameState> levelStartScreen;
	PlayState3d playState;
	playState.f_setLevel(current_level.get());	
	EditState editState(current_level->f_levelDimensions());
	editState.f_setLevel(current_level.get());
	OverworldState overworldState;
	OptionState optionState;
	GameState* gameState = menuState.get();
	GameState* lastState = nullptr;

	bool pause = false;

	bool changeWindow = false;


	//Sound::mainTheme.play();
	Sound::mainTheme.setVolume(30);
	
	while (currentWindow->isOpen())
	{			
		
		if (gameState != &editState)//editState is the only state that handles polling of events itself.
		{
			sf::Event event;//polled to stop window from becoming unresponsive
			while (currentWindow->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					currentWindow->close();
			}
		}

		time.restart();				


		gameState->f_run(time.deltaTime, *currentWindow);	
		//WorldTime::update(time.deltaTime);
	
		
		if (SFML_utils::onKeyPress(sf::Keyboard::Key::F1))
		{
			if (gameState != &editState) //change to editstate
			{
				lastState = gameState;
				gameState = &editState;
			}
			else //change to previous state that wasn't editstate
			{
				gameState = lastState;
				lastState = &editState;
				sf::View originalView;
				originalView = currentWindow->getView();
				originalView.setCenter(vecf(Dim::RES_X / 2, Dim::RES_Y / 2));
				currentWindow->setView(originalView);
			}
			
		}

		if (SFML_utils::onKeyPress(sf::Keyboard::F12))
		{
			if (changeWindow)
			{
				Screen::setVideoMode(currentWindow, gameName + versionToString(), true);
				changeWindow = false;
			}
			else
			{
				Screen::setVideoMode(currentWindow, gameName + versionToString(), false);
				changeWindow = true;
			}
			
		}

		while (EventQueue::f_numEvents() > 0)
		{
			Event e = EventQueue::f_fetchEvent();
			switch (e)
			{
				case Event::STARTGAME:
				{
					Sound::mainTheme.stop();
					gameState = &overworldState;
				}
				break;
				case Event::MAINMENU: 
				{
					if (Sound::mainTheme.getStatus() == sf::Sound::Stopped)					
						Sound::mainTheme.play();
										
					std::cout << "Changing to main menu state\n";
					menuState.reset(new MenuGameState());
					gameState = menuState.get();
					break;
				}
				case Event::OPENOPTIONS:gameState = &optionState; break;
				case Event::QUITGAME:currentWindow->close(); break;
				case Event::LEVELSTARTSCREEN:
				{
					Sound::jingle.play();
					levelStartScreen.reset(new LevelStartGameState(nullptr));
					gameState = levelStartScreen.get(); break;
				}
				case Event::STARTLEVEL:  gameState = &playState; break;
				case Event::CHANGERES:Screen::changeResolution(currentWindow,Drawing::m_framebuffer); break;
				case Event::CHANGEFULLSCREEN:std::cout << "Fullscreen switch TODO\n"; break;
				case Event::CHANGEWINDOWED:std::cout << "Windowed switch TODO\n"; break;
				case Event::NEWLEVELADDED: //for loading/adding levels from the console in edit mode.
				{					
					std::tuple<std::string, Level*> newLevel = editState.f_getConsole()->f_getLastNewLevel();//retrieve level
					level_path = std::get<0>(newLevel);

					if (level_store.find(level_path) != level_store.end()) //if it already exists
					{
						level_store.erase(level_path); //delete old version
						level_store[level_path] = ptr<Level>(std::get<1>(newLevel)); //place in new version
						current_level.reset(new Level(*level_store[level_path].get())); //set level to new one
						playState.f_setLevel(current_level.get()); //set states with level
						editState.f_setLevel(current_level.get());
						std::cout << "Level replaced.\n";
					}
					else
					{
						level_store[std::get<0>(newLevel)] = ptr<Level>(std::get<1>(newLevel));//store level
						current_level.reset(new Level(*level_store[level_path].get())); // set level
						playState.f_setLevel(current_level.get()); //set states level
						editState.f_setLevel(current_level.get());
						std::cout << "Level added from file.\n";
					}
				}
				break;
				case Event::RESTARTLEVEL:
				{
					restartLevel(playState,editState,overworldState);
				}
				break;				

				case Event::CHANGELEVEL://change level from a level in the level store
				{
					std::string levelToChangeTo = overworldState.f_currentLevelName();
					if (level_store.find(levelToChangeTo) != level_store.end())
					{
						current_level.reset(new Level(*level_store[levelToChangeTo].get())); // set level
						playState.f_setLevel(current_level.get()); //set states level
						editState.f_setLevel(current_level.get());
					}
					else
					{
						std::cout << "Main: Tried changing to level " << overworldState.f_currentLevelName() << ".  Was not found.\n";
					}

					break;					
				}

				case Event::CHANGEPAR:
				{
					editState.f_getLevel()->f_setPar(editState.f_getConsole()->f_getChangedPar());
					break;
				}

				case Event::LEVELCOMPLETED:
				{
					restartLevel(playState, editState, overworldState);
					overworldState.f_notifyOfLevelCompleted();//sets "levelcompleted" in mapnode to allow movement upon returning to overworld
					gameState = &overworldState;
					break;
				}

				case Event::RETURNOVERWORLD:
				{
					restartLevel(playState, editState, overworldState);
					gameState = &overworldState;
					break;
				}

				case Event::SHOWCOLLISIONTILES:
				{
					for (auto&i : level_store)
					{
						i.second->f_setDrawColliders(true);
					}
					current_level->f_setDrawColliders(true);

				}
				break;
				case Event::HIDECOLLISIONTILES:
				{
					for (auto&i : level_store)
					{
						i.second->f_setDrawColliders(false);
					}
					current_level->f_setDrawColliders(false);
				}
				break;

				case Event::PLAYIMPACTSOUND:
				{
					//if(Sound::impact01.getStatus() != sf::Sound::Playing)
						Sound::impact03.play();
					break;
				}

				case Event::MOVELEFT:current_level->f_recordMove(e); break;
				case Event::MOVERIGHT:current_level->f_recordMove(e); break;
				case Event::MOVEUP:current_level->f_recordMove(e); break;
				case Event::MOVEDOWN:current_level->f_recordMove(e); break;
			}
		}
	}
	
	return 0;
}

void restartLevel(PlayState3d & playState, EditState & editState, OverworldState & overworldState)
{
	if (level_store.find(overworldState.f_currentLevelName()) == level_store.end())
		return;
	current_level.reset(new Level(*level_store[overworldState.f_currentLevelName()])); //copy level again
	playState.f_setLevel(current_level.get()); //set level
	editState.f_setLevel(current_level.get());
}