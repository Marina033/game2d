#ifndef MAIN_ENEMY_H
#define MAIN_ENEMY_H

#include "Player.h"

struct Enemy
{
    explicit Enemy(Point pos = { .x = 10, .y = 10 }) :
        coords(pos), old_coords(coords) {};

    void DrawOfEnemy(Image& screen, Image& floor, Image& monster);
    void restoreBackGroundEnemy(Image& screenBuffer, Image& picture);
    void drawTile(const int curX, const int curY, Image& screenBuffer, Image& moster);
    Point GetCoords() { return coords; }
    Point GetOldCoords() { return old_coords; }
    void PutCoords(int x, int y) { coords.x = x; coords.y = y; } 
    void ProcessInput(MovementDir dir);
    //void PutStartCoords(int x, int y) { start_coords.x = x; start_coords.y = y; } //void PutStartCoords(int x, int y, Point coords) { ... }


private:
    //int startX{};
    //int startY{};
    //Point start_coords{ .x = 150, .y = 150 };
    Point coords{ .x = 150, .y = 150 };
    Point old_coords{ .x = 150, .y = 150 };
    int move_speed = 1; // here, speed of game_square
};

#endif //MAIN_ENEMY_H
