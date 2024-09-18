#include <iostream>
#include <raylib.h>

using namespace std;

int player1_score = 0;
int player2_score = 0;
bool isPause = false;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }
    void Update() {
        x += speed_x;
        y += speed_y;
        if(y+radius >= GetScreenHeight() || y-radius <= 0) {
            speed_y *= -1;
        }
        if(x+radius >= GetScreenWidth()) {
            player1_score++;
            ResetBall();
        }
        if(x-radius <= 0) {
            player2_score++;
            ResetBall();
        }
    }
    void ResetBall() {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];

        isPause = true;
    }
};

class Paddle {

protected:
    void LimitMovement() {
        if(y <= 0) {
            y = 0;
        }
        if(y+height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed_y;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }
    void Update() {
        if(IsKeyDown(KEY_UP)) {
            y -= speed_y;
        }
        if(IsKeyDown(KEY_DOWN)) {
            y += speed_y;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle {
public:
    void Update(float ball_y) {
        if(y+height/2 >= ball_y) {
            y -= speed_y;
        }
        if(y+height/2 < ball_y) {
            y += speed_y;
        }
        LimitMovement();
    }
};


Ball ball;
Paddle player;
CpuPaddle cpuPlayer;

int main()
{
    cout << "Starting the game" << endl;
    const int screen_width = 1200;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 10;
    ball.speed_y = 10;

    player.x = screen_width - 30;
    player.y = screen_height / 2 - 60;
    player.width = 20;
    player.height = 120;
    player.speed_y = 12;

    cpuPlayer.x = 10;
    cpuPlayer.y = screen_height / 2 - 60;
    cpuPlayer.width = 20;
    cpuPlayer.height = 120;
    cpuPlayer.speed_y = 8;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        if(!isPause) {

            // Update
            ball.Update();
            player.Update();
            cpuPlayer.Update(ball.y);

            // Collision
            if(CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {player.x, player.y, player.width, player.height}) || 
            CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {cpuPlayer.x, cpuPlayer.y, cpuPlayer.width, cpuPlayer.height})) {
                ball.speed_x *= -1;
            }

            // Draw
            ClearBackground(BLACK);
            DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
            ball.Draw();
            cpuPlayer.Draw();
            player.Draw();
            DrawText(TextFormat("%i", player1_score), screen_width/4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player2_score), 3 * screen_width/4 - 20, 20, 80, WHITE);
        }
        if(isPause) {
            if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
            {
                isPause = false;
            }
            // Draw
            ClearBackground(BLACK);
            DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
            ball.Draw();
            cpuPlayer.Draw();
            player.Draw();
            DrawText(TextFormat("%i", player1_score), screen_width/4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player2_score), 3 * screen_width/4 - 20, 20, 80, WHITE);
        }
        EndDrawing();

    }
    CloseWindow();
    
    
    return 0;
}