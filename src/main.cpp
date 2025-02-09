
#include <array>

#include "raylib.h"
#include<vector>
//#include <array>

//using grid = std::array<std::array<int, 50>, 50>;
const int GRID_SIZE = 60;
const int TILE_SIZE = 15;

struct Posizione {
    int riga;
    int colonna;
};

using Direzione = Posizione;

std::array<Direzione,4> direzioni = {
    Direzione{1,0},
    Direzione{0,-1},
    Direzione{-1,0},
    Direzione{0,1}
};

struct Snake {
    Posizione posizione;
    int direzione; // compreso tra 0 e 4
    std::vector<Posizione> body;
};

void GeneraMela(std::vector<Posizione>& posizioni_mele) {
    auto x = GetRandomValue(0,GRID_SIZE);
    auto y = GetRandomValue(0,GRID_SIZE);
    Posizione posizione;
    posizione.riga = y;
    posizione.colonna = x;
    posizioni_mele.push_back(posizione);

}

void TryEatMela(Snake& snake,std::vector<Posizione>& posizioni_mele) {
    int found = -1;
    for (int i = 0; i < posizioni_mele.size(); i++) {
        if (posizioni_mele[i].riga == snake.posizione.riga && posizioni_mele[i].colonna == snake.posizione.colonna) {
            snake.body.push_back(snake.posizione);
            found = i;
            break;
        }
    }
    if (found >= 0) {
        posizioni_mele.erase(posizioni_mele.begin() + found);
    }
}

void UpdateSnakeBody(Snake& snake) {
    if (snake.body.size() == 0) return;
    snake.body.erase(snake.body.begin());
    snake.body.push_back(snake.posizione);
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 1000;

    std::vector<Posizione> mele;
    Snake snake;
    snake.posizione.riga = GetRandomValue(0,GRID_SIZE);
    snake.posizione.colonna = GetRandomValue(0,GRID_SIZE);
    snake.direzione = 0;
    // GeneraMela(mele);

    int timer = 0;

    //TryEatMela(snake,mele);

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(15);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        timer--;
        if (timer < 0) {
            timer = 10;
            GeneraMela(mele);
        }
        // input
        if (IsKeyDown(KEY_LEFT)) {
            snake.direzione--;
            if (snake.direzione < 0) {
                snake.direzione +=4;
            }
        } else if (IsKeyDown(KEY_RIGHT)) {
            snake.direzione++;
            if (snake.direzione >= 4) {
                snake.direzione -= 4;
            }
        }

        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        snake.posizione.riga += direzioni[snake.direzione].riga;
        snake.posizione.colonna += direzioni[snake.direzione].colonna;

        if (snake.posizione.riga >= GRID_SIZE) {
            snake.posizione.riga -= GRID_SIZE;
        } else if (snake.posizione.riga < 0) {
            snake.posizione.riga += GRID_SIZE;
        }

        if (snake.posizione.colonna >= GRID_SIZE) {
            snake.posizione.colonna -= GRID_SIZE;
        } else if (snake.posizione.colonna < 0) {
            snake.posizione.colonna += GRID_SIZE;
        }



        for (int i = 0; i < snake.body.size(); i++) {
            if (snake.body[i].colonna == snake.posizione.colonna && snake.body[i].riga == snake.posizione.riga) {
                snake.posizione.riga = GetRandomValue(0,GRID_SIZE);
                snake.posizione.colonna = GetRandomValue(0,GRID_SIZE);
                snake.direzione = 0;
                snake.body.clear();
                GeneraMela(mele);
            }
        }

        TryEatMela(snake,mele);
        UpdateSnakeBody(snake);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x < GRID_SIZE; x++) {
            DrawLine(x*TILE_SIZE, 0, x*TILE_SIZE, GRID_SIZE*TILE_SIZE, LIGHTGRAY);
        }

        for (int y = 0; y < GRID_SIZE; y++) {
            DrawLine(0, y*TILE_SIZE, GRID_SIZE*TILE_SIZE, y*TILE_SIZE, LIGHTGRAY);
        }

        //draw snake head
        DrawRectangle(snake.posizione.colonna*TILE_SIZE,snake.posizione.riga*TILE_SIZE,TILE_SIZE,TILE_SIZE,GREEN);
        // draw snake body
        for (int i = 0; i < snake.body.size(); i++) {
            DrawRectangle(snake.body[i].colonna*TILE_SIZE, snake.body[i].riga*TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
        }

        for (auto& posizione : mele) {
            DrawCircle(posizione.colonna*TILE_SIZE+ TILE_SIZE/2,posizione.riga*TILE_SIZE+TILE_SIZE/2,TILE_SIZE/2,RED);
        }


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}