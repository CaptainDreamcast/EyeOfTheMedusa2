#ifndef GAME_STATE_LOGIC_H
#define GAME_STATE_LOGIC_H

void setGameWon();
void setGameLost();
void setupGameStateLogic(int* winFlag, int* gameOverFlag);
void shutdownGameStateLogic();

#endif
