#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"

struct Point
{
    int x;
    int y;
};

enum class MovementDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Player
{
    explicit Player(Point pos = { .x = 10, .y = 10 }) :
        coords(pos), old_coords(coords) {};

    void ProcessInput(MovementDir dir, std::string& chars, bool& flag_stop, bool& flag_fall, bool& flag_exit, bool& flag_final_exit,
        bool& exU, bool& exD, bool& exR, bool& exL);
    void DrawOfPlayer(Image& screen, Image& floor, Image& man);
    char Elem(std::string& str, int a_obst, int b_obst);
    bool CheckCoords(std::string& chars, int obstX, int obstY, int futureX, int futureY);
    void Player::UpdateFlags(bool& local_flag_break, bool& flag_fall, bool& flag_exit, bool& flag_FinalExit,
        std::string& chars, int obstX, int obstY);
    Point GetCoords() { return coords; }
    Point GetOldCoords() { return old_coords; }
    void PutCoords(int x, int y) { coords.x = x; coords.y = y; } 
    //void PutStartCoords(int x, int y) { start_coords.x = x; start_coords.y = y; } //void PutStartCoords(int x, int y, Point coords) { ... }


private:
    //int startX{};
    //int startY{};
    //Point start_coords{ .x = 150, .y = 150 };
    Point coords{ .x = 150, .y = 150 };
    Point old_coords{ .x = 150, .y = 150 };
    //Pixel color{ .r = 255, .g = 0, .b = 0, .a = 1 }; //here, color of game_square
    int move_speed = 1; // here, speed of game_square
};

#endif //MAIN_PLAYER_H
