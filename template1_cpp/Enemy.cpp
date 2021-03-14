#include "Enemy.h"

void Enemy::DrawOfEnemy(Image& screen, Image& floor, Image& monster) {
    for (int y = coords.y; y < coords.y + monster.Height(); ++y)
        for (int x = coords.x; x < coords.x + monster.Width(); ++x)
            screen.PutPixel(x, y, monster.GetPixel(x - coords.x, (y - coords.y)));
}
void Enemy::restoreBackGroundEnemy(Image& screenBuffer, Image& picture) {
	int numY = (int) trunc(old_coords.y / TILE_HEIGHT); // here should trunc all brackets
	int numX = (int) trunc(old_coords.x / TILE_WIDTH); // here should trunc all brackets
	if (old_coords.y % 32 == 0 && old_coords.x % 32 == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
	}
	else if (old_coords.y % 32 == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
	} 
	else if (old_coords.x % 32 == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile(numX * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
	}
	else {

		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile(numX * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
	}
}

void Enemy::drawTile(const int curX, const int curY, Image& screenBuffer, Image& moster) {
	for (int i = 0; i < moster.Width(); ++i)
		for (int j = 0; j < moster.Height(); ++j)
			screenBuffer.PutPixel(curX + i, curY + j, moster.GetPixel(i, j));

}

void Enemy::ProcessInput(MovementDir dir) {
	int move_dist = 1;
	switch (dir) {
	case MovementDir::LEFT:
		old_coords.x = coords.x;
		old_coords.y = coords.y;
		coords.x -= move_dist;
		break;
	case MovementDir::RIGHT:
		old_coords.x = coords.x;
		old_coords.y = coords.y;
		coords.x += move_dist;
		break;
	case MovementDir::UP:
		old_coords.x = coords.x;
		old_coords.y = coords.y;
		coords.y += move_dist;
		break;
	case MovementDir::DOWN:
		old_coords.x = coords.x;
		old_coords.y = coords.y;
		coords.y -= move_dist;
		break;
	default: break;
	}
}
 
