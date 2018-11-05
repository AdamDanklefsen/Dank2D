#include "Game.h"



Game::Game(HWND hwnd) : hwnd(hwnd), gfx(hwnd){}
Game::~Game() {}

void Game::Go() {
	Update();
	Draw();
}

void Game::Update() {

}
void Game::Draw() {

}
