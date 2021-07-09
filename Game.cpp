#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <time.h>

//#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>

#include <stdio.h>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  schedule_quit_game() - quit game after act()

int max(int a, int b)
{
  if (a > b)
    return a;
  return b;
}
int min(int a, int b)
{
  if (a < b)
    return a;
  return b;
}

struct GameSettings
{
  //bird_pos
  uint32_t bird_pos_x = 384;
  const uint32_t bird_pos_y = 204; //screen move - pos constant

  //bird_speed
  uint32_t  speed = 0;
  const uint32_t acs = 5;
  const uint32_t space_boost = 20; //?

  bool press = false;
  //const float space_freq = 0.5;
  const float space_last = 0.1;
  float space_pr = 0;

  //pipes settingsco
  const uint32_t pipe_dist = 240;
  const uint32_t pipe_space = 240;
  const uint32_t pipe_weight = 100;

  uint32_t pipes_height[4] = { 0 };
  uint32_t pipes_pos[4] = { 0 };

  const uint32_t pipe_speed = 10;

  const uint32_t min_pipe_heigth = 80;
  const uint32_t max_pipe_heigth = 320;

  // game state
  bool start = false;
  bool end = false;

  const uint32_t count_height = 20; // for count drawing

  // pictures
  uint32_t background[SCREEN_HEIGHT][SCREEN_WIDTH] = { 0 };
  uint32_t bird[80][80] = { 0 };

  float time = 0;
  const float freq = 0.04;

  uint32_t count = 0;
} gameSettings;

void draw_bird()
{
  for(int i = 0; i < 80; i++)
    for(int j = 0; j < 80; j++)
    {
      if(gameSettings.bird[i][j] != 0x00ffffff)
        buffer[gameSettings.bird_pos_x - 40 + i][gameSettings.bird_pos_y - 40 + j] = gameSettings.bird[i][j];
    }
}

void draw_pipes()
{
  for (int i = 0; i < 4; i++)
  {
    if (gameSettings.pipes_height[i] != 0)
    {
      for (int j = 0; j <= gameSettings.pipes_height[i]; j++)
      {
        for (int k = max(gameSettings.pipes_pos[i], 0);
                 k < min(gameSettings.pipes_pos[i] + gameSettings.pipe_weight, SCREEN_WIDTH); k++)
        {
          buffer[j][k] = 0xff00ff00;
        }
      }
      for (int j = gameSettings.pipes_height[i] + gameSettings.pipe_space; j < SCREEN_HEIGHT; j++)
      {
        for (int k = max(gameSettings.pipes_pos[i], 0);
                 k < min(gameSettings.pipes_pos[i] + gameSettings.pipe_weight, SCREEN_WIDTH); k++)
        {
          buffer[j][k] = 0xff00ff00;
        }
      }
    }
  }
}

void draw_num(int num, uint32_t pos){
  if(num == 0)
  {
    buffer[gameSettings.count_height][pos] = 0xff000000;
    buffer[gameSettings.count_height][pos+1] = 0xff000000;
    buffer[gameSettings.count_height][pos+2] = 0xff000000;
    buffer[gameSettings.count_height][pos+3] = 0xff000000;
    buffer[gameSettings.count_height][pos+4] = 0xff000000;

    for(int i = 1; i < 9; i++)
    {
      buffer[gameSettings.count_height-i][pos] = 0xff000000;
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }

    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  }
  else if (num == 1)
  {
    for(int i = 0; i < 9; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }
  }
  else if (num == 2)
  {
    buffer[gameSettings.count_height][pos] = 0xff000000;
    buffer[gameSettings.count_height][pos+1] = 0xff000000;
    buffer[gameSettings.count_height][pos+2] = 0xff000000;
    buffer[gameSettings.count_height][pos+3] = 0xff000000;
    buffer[gameSettings.count_height][pos+4] = 0xff000000;

    buffer[gameSettings.count_height-4][pos] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+4] = 0xff000000;

    for(int i = 1; i < 4; i++)
    {
      buffer[gameSettings.count_height-i][pos] = 0xff000000;
    }
    for(int i = 5; i < 8; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }
    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  }
  else if (num == 3)
  {
    buffer[gameSettings.count_height][pos] = 0xff000000;
    buffer[gameSettings.count_height][pos+1] = 0xff000000;
    buffer[gameSettings.count_height][pos+2] = 0xff000000;
    buffer[gameSettings.count_height][pos+3] = 0xff000000;
    buffer[gameSettings.count_height][pos+4] = 0xff000000;

    buffer[gameSettings.count_height-4][pos] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+4] = 0xff000000;

    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;

    for(int i = 0; i < 9; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }
  }
  else if (num == 4)
  {
    for(int i = 0; i < 9; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }
    for(int i = 4; i < 9; i++)
    {
      buffer[gameSettings.count_height-i][pos] = 0xff000000;
    }

    buffer[gameSettings.count_height-4][pos] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+4] = 0xff000000;
  }
  else if (num == 5)
  {
    buffer[gameSettings.count_height][pos] = 0xff000000;
    buffer[gameSettings.count_height][pos+1] = 0xff000000;
    buffer[gameSettings.count_height][pos+2] = 0xff000000;
    buffer[gameSettings.count_height][pos+3] = 0xff000000;
    buffer[gameSettings.count_height][pos+4] = 0xff000000;

    buffer[gameSettings.count_height-4][pos] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+4] = 0xff000000;

    for(int i = 1; i < 4; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }
    for(int i = 5; i < 8; i++)
    {
      buffer[gameSettings.count_height-i][pos] = 0xff000000;
    }
    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  }
  else if (num == 6)
  {
    buffer[gameSettings.count_height][pos] = 0xff000000;
    buffer[gameSettings.count_height][pos+1] = 0xff000000;
    buffer[gameSettings.count_height][pos+2] = 0xff000000;
    buffer[gameSettings.count_height][pos+3] = 0xff000000;
    buffer[gameSettings.count_height][pos+4] = 0xff000000;

    buffer[gameSettings.count_height-4][pos] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+4] = 0xff000000;

    for(int i = 1; i < 4; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }
    for(int i = 1; i < 8; i++)
    {
      buffer[gameSettings.count_height-i][pos] = 0xff000000;
    }
    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  }
  else if (num==7)
  {
    for(int i = 0; i < 9; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }

    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  }
  else if (num == 8)
  {
    buffer[gameSettings.count_height][pos] = 0xff000000;
    buffer[gameSettings.count_height][pos+1] = 0xff000000;
    buffer[gameSettings.count_height][pos+2] = 0xff000000;
    buffer[gameSettings.count_height][pos+3] = 0xff000000;
    buffer[gameSettings.count_height][pos+4] = 0xff000000;

    buffer[gameSettings.count_height-4][pos] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+4] = 0xff000000;

    for(int i = 1; i < 9; i++)
    {
        buffer[gameSettings.count_height-i][pos] = 0xff000000;
        buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }

    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  }
  else if (num == 9)
  {
    buffer[gameSettings.count_height][pos] = 0xff000000;
    buffer[gameSettings.count_height][pos+1] = 0xff000000;
    buffer[gameSettings.count_height][pos+2] = 0xff000000;
    buffer[gameSettings.count_height][pos+3] = 0xff000000;
    buffer[gameSettings.count_height][pos+4] = 0xff000000;

    buffer[gameSettings.count_height-4][pos] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-4][pos+4] = 0xff000000;

    for(int i = 1; i < 9; i++)
    {
      buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
    }
    for(int i = 5; i < 8; i++)
    {
      buffer[gameSettings.count_height-i][pos] = 0xff000000;
    }
    buffer[gameSettings.count_height-8][pos] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+3] = 0xff000000;
    buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  }
}

void draw_start()
{
  uint32_t pos = 50;
  // P
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 4] = 0xff000000;

  pos += 7;
  // R
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 4] = 0xff000000;

  pos += 7;
  // E
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height][pos + 4] = 0xff000000;

  pos += 7;
  // S
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;

  pos += 7;
  // S
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;

  pos += 7;

  pos += 7;
  // S
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;

  pos += 7;
  // P
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 4] = 0xff000000;

  pos += 7;
  // A
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-1][pos] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-2][pos] = 0xff000000;
  buffer[gameSettings.count_height-2][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos] = 0xff000000;
  buffer[gameSettings.count_height-3][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-5][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-6][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+2] = 0xff000000;

  pos += 7;
  // C
  buffer[gameSettings.count_height][pos+1] = 0xff000000;
  buffer[gameSettings.count_height][pos+2] = 0xff000000;
  buffer[gameSettings.count_height][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+4] = 0xff000000;
  for (int i = 2; i <= 6; i++)
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  buffer[gameSettings.count_height-7][pos] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+3] = 0xff000000;

  pos += 7;
  // E
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height][pos + 4] = 0xff000000;
}

void draw_gameover()
{
  uint32_t pos = 50;
  // G
  buffer[gameSettings.count_height][pos+1] = 0xff000000;
  buffer[gameSettings.count_height][pos+2] = 0xff000000;
  buffer[gameSettings.count_height][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-2][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
  for (int i = 2; i <= 6; i++)
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  buffer[gameSettings.count_height-7][pos] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+3] = 0xff000000;

  pos += 7;
  // A
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-1][pos] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-2][pos] = 0xff000000;
  buffer[gameSettings.count_height-2][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos] = 0xff000000;
  buffer[gameSettings.count_height-3][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-5][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-6][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+2] = 0xff000000;

  pos += 7;
  // M
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
    buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
  }
  buffer[gameSettings.count_height-7][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-6][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-5][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-5][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+2] = 0xff000000;

  pos += 7;
  // E
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height][pos + 4] = 0xff000000;

  pos += 7;

  pos += 7;
  // O
  buffer[gameSettings.count_height][pos+1] = 0xff000000;
  buffer[gameSettings.count_height][pos+2] = 0xff000000;
  buffer[gameSettings.count_height][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+4] = 0xff000000;
  for (int i = 2; i <= 6; i++)
  {
    buffer[gameSettings.count_height - i][pos] = 0xff000000;
    buffer[gameSettings.count_height - i][pos+4] = 0xff000000;
  }
  buffer[gameSettings.count_height-7][pos] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+3] = 0xff000000;

  pos += 7;
  // V
  buffer[gameSettings.count_height-8][pos] = 0xff000000;
  buffer[gameSettings.count_height-8][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-7][pos] = 0xff000000;
  buffer[gameSettings.count_height-7][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-6][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-5][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos] = 0xff000000;
  buffer[gameSettings.count_height-3][pos+4] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-2][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+3] = 0xff000000;
  buffer[gameSettings.count_height][pos+2] = 0xff000000;

  pos += 7;
  // E
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height][pos + 4] = 0xff000000;

  pos += 7;
  // R
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 4] = 0xff000000;

  pos += 7;
  // .
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-1][pos] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+1] = 0xff000000;

  pos += 7;
  // P
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 4] = 0xff000000;

  pos += 7;
  // R
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 4] = 0xff000000;

  pos += 7;
  // E
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height][pos + 4] = 0xff000000;

  pos += 7;
  // S
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;

  pos += 7;
  // S
  buffer[gameSettings.count_height][pos] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-5][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;

  pos += 7;

  pos += 7;
  // E
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height][pos + 4] = 0xff000000;

  pos += 7;
  // N
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
    buffer[gameSettings.count_height-i][pos+4] = 0xff000000;
  }
  buffer[gameSettings.count_height-7][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-6][pos+1] = 0xff000000;
  buffer[gameSettings.count_height-5][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-3][pos+2] = 0xff000000;
  buffer[gameSettings.count_height-2][pos+3] = 0xff000000;
  buffer[gameSettings.count_height-1][pos+3] = 0xff000000;

  pos += 7;
  // T
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos+2] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;

  pos += 7;
  // E
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height][pos + 4] = 0xff000000;

  pos += 7;
  // R
  for (int i = 0; i < 9; i++)
  {
    buffer[gameSettings.count_height-i][pos] = 0xff000000;
  }
  buffer[gameSettings.count_height-8][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 1] = 0xff000000;
  buffer[gameSettings.count_height-8][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-4][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-3][pos + 2] = 0xff000000;
  buffer[gameSettings.count_height-7][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-5][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-2][pos + 3] = 0xff000000;
  buffer[gameSettings.count_height-6][pos + 4] = 0xff000000;
  buffer[gameSettings.count_height-1][pos + 4] = 0xff000000;
}

// initialize game data in this function
void initialize()
{
  srand(time(NULL));

  for(int i = 0; i < SCREEN_HEIGHT; i++)
    for(int j = 0; j < SCREEN_WIDTH; j++)
      gameSettings.background[i][j] = 0xffb5ffff;

  for(int i = 0; i < 80; i++)
    for(int j = 0; j < 80; j++)
    {
      if ((i - 40) * (i - 40) + (j - 40) * (j - 40) <= 40*40)
        gameSettings.bird[i][j] = 0xffffff00;
      else
        gameSettings.bird[i][j] = 0x00ffffff;
    }
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();
  if (is_key_pressed(VK_SPACE) && !gameSettings.press)
  {
    gameSettings.start = true;
    gameSettings.speed = -gameSettings.space_boost;
    gameSettings.space_pr += dt;
    gameSettings.press = true;
  }
  else if (is_key_pressed(VK_SPACE) && gameSettings.press && gameSettings.space_pr < gameSettings.space_last)
  {
    gameSettings.speed = -gameSettings.space_boost;
    gameSettings.space_pr += dt;
  }
  else if (!is_key_pressed(VK_SPACE))
  {
    gameSettings.press = false;
    gameSettings.space_pr = 0;
  }

  if (is_key_pressed(VK_RETURN) && gameSettings.end)
  {
    gameSettings.start = false;
    gameSettings.end = false;
    gameSettings.bird_pos_x = 384;
    gameSettings.speed = 0;
    gameSettings.count = 0;
    for (int i = 0; i < 4; i++)
    {
      gameSettings.pipes_pos[i] = 0;
      gameSettings.pipes_height[i] = 0;
    }

    gameSettings.time = 0;
  }

  gameSettings.time += dt;
  if (gameSettings.start && !gameSettings.end && gameSettings.time > gameSettings.freq)
  {
    gameSettings.bird_pos_x += gameSettings.speed;
    gameSettings.speed += gameSettings.acs;

    for (int i = 0; i < 4; i++)
    {
      gameSettings.pipes_pos[i] -= gameSettings.pipe_speed;
    }

    if (gameSettings.bird_pos_x <= 40)
    {
      gameSettings.end = true;
      gameSettings.bird_pos_x = 45;
      gameSettings.speed = 2;
    }
    else if (gameSettings.bird_pos_x >= SCREEN_HEIGHT - 40)
    {
      gameSettings.end = true;
      gameSettings.bird_pos_x = SCREEN_HEIGHT - 40;
    }
    if (gameSettings.pipes_pos[3] + gameSettings.pipe_weight + gameSettings.pipe_dist < SCREEN_WIDTH)
    {
      gameSettings.pipes_height[0] = gameSettings.pipes_height[1];
      gameSettings.pipes_height[1] = gameSettings.pipes_height[2];
      gameSettings.pipes_height[2] = gameSettings.pipes_height[3];
      gameSettings.pipes_height[3] =
          rand() % (SCREEN_HEIGHT - gameSettings.min_pipe_heigth - gameSettings.max_pipe_heigth) + gameSettings.min_pipe_heigth;

      gameSettings.pipes_pos[0] = gameSettings.pipes_pos[1];
      gameSettings.pipes_pos[1] = gameSettings.pipes_pos[2];
      gameSettings.pipes_pos[2] = gameSettings.pipes_pos[3];
      gameSettings.pipes_pos[3] = SCREEN_WIDTH;
    }
    if (gameSettings.pipes_height[1] != 0 && gameSettings.pipes_pos[1] == 64) // костыль потом исправить
    {
      gameSettings.count += 1;
    }
    if (gameSettings.bird_pos_y + 40 > gameSettings.pipes_pos[1] &&
        gameSettings.bird_pos_y < gameSettings.pipes_pos[1])
    {
      if (gameSettings.bird_pos_x < gameSettings.pipes_height[1] ||
          gameSettings.bird_pos_x > gameSettings.pipes_height[1] + gameSettings.pipe_space)
        gameSettings.end = true;
      else if (gameSettings.bird_pos_x < gameSettings.pipes_height[1] + 40 &&
                (gameSettings.bird_pos_x - gameSettings.pipes_height[1]) *
                 (gameSettings.bird_pos_x - gameSettings.pipes_height[1]) +
                (gameSettings.bird_pos_y - gameSettings.pipes_pos[1]) *
                 (gameSettings.bird_pos_y - gameSettings.pipes_pos[1]) < 40 * 40)
        gameSettings.end = true;
      else if (gameSettings.bird_pos_x > gameSettings.pipes_height[1] + gameSettings.pipe_space - 40 &&
                (gameSettings.bird_pos_x - gameSettings.pipes_height[1] - gameSettings.pipe_space) *
                 (gameSettings.bird_pos_x - gameSettings.pipes_height[1] - gameSettings.pipe_space) +
                (gameSettings.bird_pos_y - gameSettings.pipes_pos[1]) *
                 (gameSettings.bird_pos_y - gameSettings.pipes_pos[1]) < 40 * 40)
        gameSettings.end = true;
    }
    else if (gameSettings.bird_pos_y > gameSettings.pipes_pos[1] &&
             gameSettings.bird_pos_y < gameSettings.pipes_pos[1] + gameSettings.pipe_weight)
    {
      if (gameSettings.bird_pos_x < gameSettings.pipes_height[1] + 40 ||
          gameSettings.bird_pos_x > gameSettings.pipes_height[1] + gameSettings.pipe_space - 40)
        gameSettings.end = true;
    }
    else if (gameSettings.bird_pos_y < gameSettings.pipes_pos[1] + gameSettings.pipe_weight + 40 &&
             gameSettings.bird_pos_y > gameSettings.pipes_pos[1] + gameSettings.pipe_weight)
    {
      if (gameSettings.bird_pos_x < gameSettings.pipes_height[1] ||
          gameSettings.bird_pos_x > gameSettings.pipes_height[1] + gameSettings.pipe_space)
        gameSettings.end = true;
      else if (gameSettings.bird_pos_x < gameSettings.pipes_height[1] + 40 &&
                (gameSettings.bird_pos_x - gameSettings.pipes_height[1]) *
                 (gameSettings.bird_pos_x - gameSettings.pipes_height[1]) +
                (gameSettings.bird_pos_y - gameSettings.pipes_pos[1] - gameSettings.pipe_weight) *
                 (gameSettings.bird_pos_y - gameSettings.pipes_pos[1] - gameSettings.pipe_weight) < 40 * 40)
        gameSettings.end = true;
      else if (gameSettings.bird_pos_x > gameSettings.pipes_height[1] + gameSettings.pipe_space - 40 &&
                (gameSettings.bird_pos_x - gameSettings.pipes_height[1] - gameSettings.pipe_space) *
                 (gameSettings.bird_pos_x - gameSettings.pipes_height[1] - gameSettings.pipe_space) +
                (gameSettings.bird_pos_y - gameSettings.pipes_pos[1] - gameSettings.pipe_weight) *
                 (gameSettings.bird_pos_y - gameSettings.pipes_pos[1] - gameSettings.pipe_weight) < 40 * 40)
        gameSettings.end = true;
    }
    gameSettings.time = 0;
  }
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{

  // clear backbuffer
  memcpy(buffer, gameSettings.background, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  if (gameSettings.end)
  {
    draw_pipes();
    draw_bird();
    draw_gameover();
  }

  else if (gameSettings.start)
  {
    draw_pipes();
    draw_bird();
  }

  else
  {
    draw_bird();
    draw_start();
  }

  uint32_t current = 10;
  while (gameSettings.count / current > 0)
  {
    current *= 10;
  }
  current /= 10;
  uint32_t current_count = gameSettings.count;
  uint32_t pos = 462;
  do {
    draw_num(current_count / current, pos);
    current_count = current_count % current;
    current /= 10;
    pos += 12;
  } while (current > 0);

}

// free game data in this function
void finalize()
{
}

