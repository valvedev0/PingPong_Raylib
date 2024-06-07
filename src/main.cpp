#include<iostream>
#include <raylib.h>

using namespace std;


//global variables for score
int player_score = 0; 
int cpu_score = 0;

class Ball{
public:
float x, y;
int speed_x, speed_y;
int radius;

void Draw()
{
    DrawCircle(x, y, radius, WHITE);
} 

void Update()
{
    x += speed_x; //here x increses by 7 px on ewach update
    y += speed_y;

    //check for collisions
    if(y + radius >= GetScreenHeight() || y - radius <= 0)
    {
        speed_y *= -1;
    }

    if(x + radius >= GetScreenWidth())  
    {
        cpu_score++;
        ResetBall();
    }
    if(x - radius <= 0)
    {
        player_score++;
        ResetBall();
    }
}

void ResetBall()
{
    x = GetScreenWidth()/2;
    y = GetScreenHeight()/2;

    int randomchoice[2] = {-1,1};
    speed_x *= randomchoice[GetRandomValue(0,1)];
    speed_y *= randomchoice[GetRandomValue(0,1)];
}

};

class Paddle{
protected:
void LimitMovement(){

    if( y + height >= GetScreenHeight())
    {
        y = GetScreenHeight() - height;
    }
    if(y <= 0)
    {
        y = 0;
    }
}

public:

float x, y;
float width, height;
int speed;

void Draw()
{
    DrawRectangle(x, y, width, height, WHITE);
}
void Update()
{
    //implementing input fn
    if(IsKeyDown(KEY_W))
    {
        y -= speed;
    }
    if(IsKeyDown(KEY_S))
    {
        y += speed;
    }
    //Limiting the collsiosn in vertical axis
    LimitMovement();
}

};

class CpuPaddle : public Paddle{
public:
    void Update(int ball_y)
    {
        if( y + height/2 >= ball_y)
        {
            y -= speed;
        }
        if( y + height/2 <= ball_y)
        {
            y += speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main()
{
    cout<<"Window is starting"<<endl;
    const int Screen_Width = 1280;
    const int Screen_Height = 800;
    InitWindow(Screen_Width, Screen_Height, "Ping-Pong");
    SetTargetFPS(60);

    //Defining ball values
    ball.x = Screen_Width/2;
    ball.y = Screen_Height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;
    ball.radius = 20;

    //Defining paddle values, p.s this is representing the right paddle values, left paddle is controlled via CPU
    player.width = 25;
    player.height = 120;
    player.x = Screen_Width - player.width -10;
    player.y = Screen_Height/2 - player.height/2;
    player.speed = 6;

    //defining the cpu values
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = Screen_Height/2 - cpu.height/2;
    cpu.speed = 6; 
    

    //GameLoop
    
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        //sequence is Updating->CheckCollisonClearBG->DrawObject

        //Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //checking for colliosn
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
        }

        ClearBackground(BLACK);

        //Draw Game Objects
        
        ball.Draw();
        cpu.Draw();
        player.Draw();
        
        DrawLine(Screen_Width/2, 0, Screen_Width/2, Screen_Height, WHITE);
        
        //Draw scores
        DrawText(TextFormat("%i", cpu_score), Screen_Width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * Screen_Width/4 - 20, 20, 80, WHITE);
        
       
         
       

        EndDrawing();
    }


    CloseWindow();
    return 0;
}