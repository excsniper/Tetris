#include "game.cpp"
#include "intro.cpp"

//main
int main()
{
    //init   
    init_game();
    BITMAP *buffer = create_bitmap(640,480);
    FONT *score_font;
    PALETTE palette;

    //load pics
    BITMAP *block_pic = load_bitmap("data/block.bmp",NULL);
    BITMAP *gamebg = load_bitmap("data/gamebg.bmp",NULL);
    BITMAP *ilya = load_bitmap("data/ilya.bmp",NULL);
    BITMAP *music = load_bitmap("data/music.bmp",NULL);
    BITMAP *begin = load_bitmap("data/begin.bmp",NULL);
    BITMAP *end = load_bitmap("data/end.bmp",NULL);
    score_font = load_font("data/score_font.pcx", palette, NULL);
    
    //start music    
    set_bgm("1");
    
    //run intro
    intro(buffer, ilya, music, begin);    
        
    //start game    
    start_game();
    while(!key[KEY_ESC])
    {
        if (!game_over)
        {
            logic();
            render(buffer, block_pic, gamebg,score_font);
        }
        else
        {            
            //show "game over"
            draw_sprite(screen,end,0,0);
            
            //give it a moment...
            rest(1000);
            
            //wait for the ANY KEY
            clear_keybuf();
            readkey();
            game_over=false; 
            
            //restart game           
            start_game();            
        }            
    }
    
    //free memory
    destroy_bitmap(buffer);
    destroy_bitmap(block_pic);
    destroy_bitmap(ilya);
    destroy_bitmap(music);
    destroy_bitmap(begin);
    destroy_font(score_font);
}
END_OF_MAIN() 
