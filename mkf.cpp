#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <user/engine.h>
#include <user/screen.h>
#include <user/resfile.h>
#include <user/graphic.h>
#include <user/keyboard.h>

int main(int argc, char **argv)  {
  U2_Init(argc, argv);
  int inp;
  if(argc < 2)  {
    U2_Exit(0, "USAGE: mkf <fontdir>\n");
    }
  int xpos = 0;
  int ypos = 0;
  Screen *screen = new Screen(640, 480, 32);
  Keyboard *key = new Keyboard;
  char *buf;
  int ctr;
  Graphic *tmpg = NULL;

  int strst = strlen(argv[1]);
  char *fn = new char[strst+20];
  strcpy(fn, argv[1]);

  char tmpbuf[30];
  bzero(tmpbuf, 30);
  char *ind = &fn[strst];
  while(ind != fn && ind[-1] != '\\')  ind--;
  sprintf(tmpbuf, "%s.sgf", ind);
  { NewResFile fonfile(tmpbuf);

    fn[strst] = '/';
    strst++;
    fn[strst] = 0;
    buf = &fn[strst];

    sprintf(buf, "32.bmp%c", 0);
    if(access(fn, R_OK))
      U2_Exit(-1, "Critical Error, Space Graphic (32.bmp) not present!\n");
	
    sprintf(buf, "string.bmp%c", 0);
    if(!access(fn, R_OK)) {
      Graphic ref(fn); int pos=0, xs, yp;
      for(ctr=0; ctr<256; ctr++) {
	tmpg=NULL;
	if(isgraph(ctr)) {
	  if(pos>=ref.xsize) U2_Exit(1, "String overflow at \"%c\"!\n", ctr);
	  for(; pos<ref.xsize; ++pos) {
	    for(yp=0; yp<ref.ysize && ref.image[yp].uc[pos]<1; ++yp);
	    if(yp<ref.ysize) break;
	    }
	  if(pos>=ref.xsize) U2_Exit(1, "String overflow at \"%c\"!\n", ctr);
	  for(xs=0; xs<(ref.xsize-pos); ++xs) {
	    for(yp=0; yp<ref.ysize && ref.image[yp].uc[pos+xs]<1; ++yp);
	    if(yp>=ref.ysize) break;
	    }
	  tmpg=new Graphic(ref.Partial(pos, 0, pos+xs, ref.ysize));
	  pos+=xs;
	  tmpg->Trim();
	  }
	else if(ctr==32) {
	  sprintf(buf, "32.bmp%c", 0);
	  tmpg=new Graphic(fn);
	  }
	fonfile.Add(tmpg);
	if(tmpg != NULL)  delete tmpg;
	tmpg = NULL;
	}      
      }
    else for(ctr=0; ctr<256; ctr++)  {
      if(isgraph(ctr) || ctr == ' ')  {
	FILE *tmpf;
        sprintf(buf, "%d.bmp%c", ctr, 0);
        tmpf = fopen(fn, "r");
        if(tmpf != NULL)  {
	  fclose(tmpf);
	  tmpg = new Graphic(fn);
	  if(ctr != ' ')  tmpg->Trim();
	  if(tmpg->image == NULL)  {
	    delete tmpg;
	    tmpg = NULL;
	    }
	  else  {
	    screen->DrawGraphic(*tmpg, xpos-tmpg->xcenter, ypos-tmpg->ycenter);
	    xpos+=(tmpg->xsize+1);
	    }
	  }
        }
      fonfile.Add(tmpg);
      if(tmpg != NULL)  delete tmpg;
      tmpg = NULL;
      }
    }
  Graphic Curg("cursor.bmp");
  Curg.Trim();

  for(ctr=0; ctr<256; ++ctr)
    screen->SetPaletteEntry(ctr, 255-ctr, 255-ctr, 255-ctr);
  screen->Clear(0xFFFFFFFF);
  screen->SetFont(tmpbuf);
  screen->SetCursor(Curg);
  for(ctr=0; ctr<256; ++ctr) if(isgraph(ctr))
	screen->Printf(0xFFFFFFFF, 0xFF000000, "%c", ctr);
  screen->Print(0xFFFFFFFF, 0xFF000000, "\n\nThis is your font.\nYou Like it?\n");
  screen->RefreshFull();
  screen->Show();
  inp = key->WaitForChar();
  char tmp[2];
  tmp[1] = 0;
  while(inp != CHAR_ESC)  {
    tmp[0] = inp;
    screen->Print(0xFFFFFFFF, 0xFF000000, tmp);
    screen->RefreshFull();
    inp = key->WaitForChar();
    }
  delete key;
  delete screen;
  return 0;
  }
