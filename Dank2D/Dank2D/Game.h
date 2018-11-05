#pragma once
#include "Graphics.h"

class Game {
public:
	Game(HWND hwnd);
	~Game();
	void Go();
	void Update();
	void Draw();

private:
	HWND hwnd;
	Graphics gfx;
};

