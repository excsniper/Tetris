#include <cstdlib>
#include <iostream>
#include <string>
#include <allegro.h>
#include <fmod.h>
using namespace std;

//speed const
//The bigger the number, the slower the delay
    const int _kb=3;
    const int _grav=25;

//normal const
    const int _x=0,_y=1;
    const int _left=1,_right=2,_down=3;
    const int _field_top=425,_field_left=152;

//timer stuff
    volatile long beat;
    int igrav;
    int ikb;
    int icount;

    void inc_beat()
    {
         beat++;
    }
    END_OF_FUNCTION(inc_beat);

//sum flags
    bool pause=false;
    bool game_over=false;
    bool start_count=false;

//sum globle vars
    int score=0;
    int add_to_score=0;
    bool field[10][20];
    int block[4][2];
    int next_block[4][2];
    int block_centre[2];
    int next_block_centre[2]; 

void r_screen(BITMAP *buffer)
{
     blit(buffer, screen, 0,0,0,0,640,480);
     clear_bitmap(buffer);     
}

void r_field(BITMAP *buffer, BITMAP *block_pic)
{
    for(int y=0;y<20;y++)
    {
        for(int x=0;x<10;x++)
        {
              if (field[x][y])
              {
                 draw_sprite(buffer,block_pic,_field_left+x*20,_field_top-y*20);
              }
        }
    }
}

void r_next_block(BITMAP *buffer, BITMAP *block_pic)
{
    if((next_block[0][_x]==5) && (next_block[1][_x]==5))
    {
        for(int i=0;i<4;i++)
        {
            draw_sprite(buffer,block_pic,378+next_block[i][_x]*20,550-next_block[i][_y]*20);
        }
    }
    else if((next_block[0][_x]==4) && (next_block[2][_x]==4))
    {
        for(int i=0;i<4;i++)
        {
            draw_sprite(buffer,block_pic,388+next_block[i][_x]*20,570-next_block[i][_y]*20);
        }
    }
    else
    {
        for(int i=0;i<4;i++)
        {
            draw_sprite(buffer,block_pic,378+next_block[i][_x]*20,570-next_block[i][_y]*20);
        }
    }
}

void r_block(BITMAP *buffer, BITMAP *block_pic)
{
    for(int i=0;i<4;i++)
    {
        draw_sprite(buffer,block_pic,_field_left+block[i][_x]*20,_field_top-block[i][_y]*20);
    }
}

void r_background(BITMAP *buffer,BITMAP *gamebg)
{
     blit(gamebg, buffer, 0, 0, 0, 0, 640, 480);
}

void clear()
{
     for(int y=0;y<20;y++)
     {
         for(int x=0;x<10;x++)
         {
                 field[x][y]=false;
         }
     }
}

void make_block()
{
     int rand_num = rand() % 7 + 1;     
  
     switch (rand_num)
     {
         case 1:
              next_block[0][_x]=5; next_block[0][_y]=19;
              next_block[1][_x]=5; next_block[1][_y]=18;
              next_block[2][_x]=5; next_block[2][_y]=17;
              next_block[3][_x]=5; next_block[3][_y]=16;
              next_block_centre[_x]=5;next_block_centre[_y]=18;
              break;
         case 2:
              next_block[0][_x]=4; next_block[0][_y]=19;
              next_block[1][_x]=4; next_block[1][_y]=18;
              next_block[2][_x]=5; next_block[2][_y]=18;
              next_block[3][_x]=6; next_block[3][_y]=18;
              next_block_centre[_x]=5;next_block_centre[_y]=18;
              break;
         case 3:
              next_block[0][_x]=6; next_block[0][_y]=19;
              next_block[1][_x]=6; next_block[1][_y]=18;
              next_block[2][_x]=5; next_block[2][_y]=18;
              next_block[3][_x]=4; next_block[3][_y]=18;
              next_block_centre[_x]=5;next_block_centre[_y]=18;
              break;
         case 4:
              next_block[0][_x]=4; next_block[0][_y]=19;
              next_block[1][_x]=5; next_block[1][_y]=19;
              next_block[2][_x]=4; next_block[2][_y]=18;
              next_block[3][_x]=5; next_block[3][_y]=18;
              //dont need centre next_block
              next_block_centre[_x]=-1;next_block_centre[_y]=-1;
              break;
         case 5:
              next_block[0][_x]=4; next_block[0][_y]=18;
              next_block[1][_x]=5; next_block[1][_y]=18;
              next_block[2][_x]=5; next_block[2][_y]=19;
              next_block[3][_x]=6; next_block[3][_y]=19;
              next_block_centre[_x]=5;next_block_centre[_y]=18;
              break;
         case 6:
              next_block[0][_x]=5; next_block[0][_y]=19;
              next_block[1][_x]=4; next_block[1][_y]=18;
              next_block[2][_x]=5; next_block[2][_y]=18;
              next_block[3][_x]=6; next_block[3][_y]=18;
              next_block_centre[_x]=5;next_block_centre[_y]=18;
              break;
         case 7:
              next_block[0][_x]=4; next_block[0][_y]=19;
              next_block[1][_x]=5; next_block[1][_y]=19;
              next_block[2][_x]=5; next_block[2][_y]=18;
              next_block[3][_x]=6; next_block[3][_y]=18;
              next_block_centre[_x]=5;next_block_centre[_y]=18;
     }     
}

void spawn_block()
{
     for(int i=0;i<4;i++)
     {
         block[i][_x]=next_block[i][_x];
         block[i][_y]=next_block[i][_y];
         block_centre[_x]=next_block_centre[_x];
         block_centre[_y]=next_block_centre[_y];
     }
     make_block();     
}

bool check_move(int direction)
{
     if (direction==_left)
     {
         for(int i=0;i<4;i++)
         {
             if (field[ block[i][_x]-1 ][ block[i][_y] ] || ((block[i][_x]-1)<0))
             {
                 return (false);
             }
         }
     }
     else if (direction==_right)
     {
         for(int i=0;i<4;i++)
         {
             if (field[ block[i][_x]+1 ][ block[i][_y] ] || ((block[i][_x]+1)>9))
             {
                 return (false);
             }
         }
     }
     else
     {
         for(int i=0;i<4;i++)
         {
             if (field[ block[i][_x] ][ block[i][_y]-1 ] || ((block[i][_y]-1)<0))
             {
                 return (false);
             }
         }
     }     
     //if func doesn't end with false, it will return true. 
     return (true);
}

void shift_row(int row)
{
     for(int x=0;x<10;x++)
     {
         field[x][row]=false;
     }
     
     for(int y=row+1;y<20;y++)
     {
         for(int x=0;x<10;x++)
         {
             field[x][y-1]=field[x][y];
         }
     }
}

void check_row()
{
    int row=0;
    bool clear;
    do
    {
        clear=true;
        
        for(int y=0;y<20;y++)
        {       
            for(int x=0;x<10;x++)
            {
                if(field[x][y])
                row++;
            }
            
            if(row==10)
            {
                shift_row(y);
                clear=false;
                add_to_score+=100; //add score of 100
            }
            row=0;
        }
    } while(!clear);
}

void set_block()
{
    if (!check_move(_down))
    {
        start_count=true;
        if(icount>15)
        {
            for (int i=0;i<4;i++)
            {
                field[ block[i][_x] ][ block[i][_y] ] = true;
            }
            spawn_block();
            icount=0;
            start_count=false;
            add_to_score+=10; //add 10 to score
            check_row();
        }
    }
    else
    {
        icount=0;
        start_count=false;
    }
}

void move_block(int direction)
{
     if (direction==_left)
     {
         if (check_move(_left))
         {
             for(int i=0;i<4;i++)
             {
                 block[i][_x]--;
             }
             block_centre[_x]--;
         }
     }
     else if (direction==_right)
     {
         if (check_move(_right))
         {
             for(int i=0;i<4;i++)
             {
                 block[i][_x]++;
             }
             block_centre[_x]++;
         }
     }
     else
     {
         if (check_move(_down))
         {         
             for(int i=0;i<4;i++)
             {
                 block[i][_y]--;    
             }
             block_centre[_y]--;
         }
     }
}

void grav()
{
     if (igrav>_grav)
     {
         move_block(_down);
         igrav=0;
     }
}

bool check_roll()
{
    for (int i=0;i<4;i++)
    {
        if (field[ block_centre[_x]-block_centre[_y]+block[i][_y] ][ block_centre[_x]+block_centre[_y]-block[i][_x] ])
        return (false);
        
        if (
               ((block_centre[_x]-block_centre[_y]+block[i][_y])<0) //check left
                                   ||
               ((block_centre[_x]-block_centre[_y]+block[i][_y])>9) //check right
                                   ||
               ((block_centre[_x]+block_centre[_y]-block[i][_x])<0)
           )
        return (false);
    }
    return (true);
}

void roll_block()
{
     if (block_centre[_x]!=-1)
     {         
         if (check_roll())
         {
             for (int i=0;i<4;i++)
             {
                 //dont ask me how this works..
                 //epic math skills ftw..
                 int old_x=block[i][_x];
                 block[i][_x]=block_centre[_x]-block_centre[_y]+block[i][_y];
                 block[i][_y]=block_centre[_x]+block_centre[_y]-old_x;
             }
         }
     }
}

void set_bgm(string num)
{    
    if(num!="0")
    {
        string bgm_path = "data/" + num + ".mp3";
        FSOUND_STREAM *bgm = FSOUND_Stream_Open(bgm_path.c_str(),FSOUND_LOOP_NORMAL,0,0);
        FSOUND_Stream_Play(0,bgm);
    }
    else
    {
        FSOUND_SetPaused(0,true);
    }
}

void check_kb()
{
     if ((ikb>_kb))
     {        
         int key = -1;
         if (keypressed())
         {
             key = readkey() >> 8;
             clear_keybuf();
         }
         if(!pause)
         {
             if (key==KEY_LEFT)
             {
                 move_block(_left);
             }
             else if(key==KEY_RIGHT)
             {
                 move_block(_right);
             }
             else if(key==KEY_DOWN)
             {
                 //don't worry about it.. 
                 //it's all good.. :)
                 icount=15; //set block faster
                 move_block(_down);
             }
             else if(key==KEY_UP)
             {
                 roll_block();
             }
         }
         
         if(key==KEY_0)
         {
             set_bgm("0"); 
         }
         else if(key==KEY_1)
         {
             set_bgm("1"); 
         }
         else if(key==KEY_2)
         {
             set_bgm("2"); 
         }
         else if(key==KEY_P)
         {
              pause=!pause;
         }         
         ikb=0;
     }
}

void add_score()
{
     if(add_to_score>0)
     {
         score++;
         add_to_score--;
     }
}

void check_end()
{
     for(int x=0;x<10;x++)
     {
         if(field[x][19])
         {
             game_over=true;
             break;
         }
     }
}

void inc_frames()
{
    igrav++;
    if(start_count)
    icount++;
}

void r_score(BITMAP *buffer,FONT *score_font)
{
     textprintf_right_ex(buffer, score_font, 520, 105, makecol(135, 165, 110),-1, "%d", score);
}

void init_game()
{
    //init
    srand(time(NULL)); //seed random number func
    allegro_init();
    install_keyboard();
    LOCK_VARIABLE(beat);
    LOCK_FUNCTION(int_beat);
    install_int_ex(inc_beat,BPS_TO_TIMER(60)); //60 is da magic number <:-)
    set_color_depth(32);
    set_gfx_mode(GFX_DIRECTX, 640,480,0,0);
    FSOUND_Init(44100, 1, 0);
}

void logic()
{
    //do logic 60 times a sec
    if (beat>0)
    {
        check_kb();
        ikb++;
        if(!pause)
        {
            grav();
            set_block();
            add_score();
            check_end();
            inc_frames();            
        }
        beat=0;
    }
}

void render(BITMAP *buffer, BITMAP *block_pic, BITMAP *gamebg,FONT *score_font)
{
    r_background(buffer,gamebg);
    r_block(buffer,block_pic);
    r_next_block(buffer,block_pic);
    r_field(buffer,block_pic);
    r_score(buffer,score_font);    
    r_screen(buffer);
}


void start_game()
{
    //reset timer
    beat=0;
    igrav=0;
    ikb=0;
    icount=0;
    
    //reset score
    score=0;
    add_to_score=0;
    
    clear();
    make_block();
    spawn_block();
}
