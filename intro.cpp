void show_frame(BITMAP *buffer,BITMAP *frame)
{
    int alpha; //0 = show 255 = hide
    
    for (alpha=255;alpha>-1;alpha--)
    {
        set_trans_blender(0, 0, 0, alpha);
        blit(frame, buffer, 0, 0, 0, 0, 640, 480);    
        rectfill(buffer, 0, 0, 640, 480, makecol(0, 0, 0));
        blit(buffer, screen, 0, 0, 0, 0, 640, 480);
        rest(10);
    }
    rest(2000);
    for (alpha=0;alpha<256;alpha++)
    {
        set_trans_blender(0, 0, 0, alpha);
        blit(frame, buffer, 0, 0, 0, 0, 640, 480);    
        rectfill(buffer, 0, 0, 640, 480, makecol(0, 0, 0));
        blit(buffer, screen, 0, 0, 0, 0, 640, 480);
        rest(1);
    }
}
void intro(BITMAP *buffer, BITMAP *ilya ,BITMAP *music, BITMAP *begin)
{
    //set up drawing mode
    drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    
    //wait for game to load
    rest(2000);
    
    //show frames
    show_frame(buffer,ilya);
    show_frame(buffer,music);
    for (int alpha=255;alpha>-1;alpha--)
    {
        set_trans_blender(0, 0, 0, alpha);
        blit(begin, buffer, 0, 0, 0, 0, 640, 480);    
        rectfill(buffer, 0, 0, 640, 480, makecol(0, 0, 0));
        blit(buffer, screen, 0, 0, 0, 0, 640, 480);
        rest(2);
    }
    
    //restore normal drawing mode
    drawing_mode(DRAW_MODE_SOLID,NULL,0,0);
    
    //wait for the ANY KEY
    clear_keybuf();
    readkey();
}
