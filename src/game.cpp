#include <ncurses.h>
#include <sstream>
#include <ctime>
#include <cstdlib>

#include "game_classes.hpp"


int main(int argc, char * argv[]){

  if(argc == 2 || argc > 3){
    printf("Skladnia: game [WIDTH HEIGHT]\n\n");
    printf("W przypadku braku podania argumentow plansza jest wielkosci terminala.\n");
    printf("Czas ladownia szybko rosnie z rozmiarami planszy (jak n^3, gdzie n to liczba pol).\n\n");
    printf("Gra polega na zdobywaniu bonusow pojawiajacych sie na planszy. Bonusy sa roznie punktowane.\n");
    printf("Poruszanie kursorem za pomoca strzalek. Kursorem z literka 'C' steruje komputer.\n");
    printf("Plan komputera mozna podejrzec naciskajac spacje (drugi raz aby przywrocic plansze).\n");
    printf("Drukowane sa wtedy numery oznaczajace kolejne bonusy ktore zamierza zebrac komputer\n");

    return 0;
  }

  initscr(); //inicjalizacja ncurses
  cbreak(); //brak buforowania
  keypad(stdscr, TRUE); //dla strzalek
  noecho(); //nie wypisujemy wpisanych znakow
  if(has_colors() == false){
    endwin();
    printf("Your terminal does not support colors. Sorry.\n");
    exit(1);
  }
  srand(time(NULL)); //seed dla randa

  //czytanie argumentow
  unsigned int width = COLS;
  unsigned int height = LINES;
  if(argc == 3){
    width = atoi(argv[1]);
    height = atoi(argv[2]);
    
    width = width > (unsigned int)COLS ? COLS : (width < 41 ? 41 : width);
    height = height > (unsigned int)LINES ? LINES : (height < 9 ? 9 : height);
  }

  //definicje kolorow
  start_color();
  init_pair(1, COLOR_YELLOW, COLOR_RED);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(5, COLOR_BLACK, COLOR_WHITE);
  init_pair(6, COLOR_WHITE, COLOR_BLACK);

  //loading, zeby zapobiec czarnemu ekranowi na poczatku przy bardzo duzych planszach
  mvprintw(LINES/2, COLS/2 - 5, "Loading...");
  refresh();






  //konstruujemy plansze
  playground * pl = new playground(width, height);

  //dodajemy troche scian w sposob tylko troche bardziej cwany niz zupelnie losowy
  for(unsigned int i=0; i < height*width/15; i++){
    //losujemy polozenie sciany
    int x0 = rand() % width;
    int y0 = rand() % height;
    if(pl->free(x0,y0)){
      pl->add_wall(x0, y0);
      //z pewnym prawdopodobienstwem dodajemy sciane obok
      int r = rand();
      for(int limit=0; limit<10; limit++){
	//losujemy kierunek
	if(r%4 == 0){
	  while(double(r)/RAND_MAX > 0.3 && pl->free(x0,y0-1)){
	    pl->add_wall(x0,--y0);
	    i++;
	    r = rand();
	  }
	  break;
	}
	else if(r%4 == 1){
	  while(double(r)/RAND_MAX > 0.3 && pl->free(x0,y0+1)){
	    pl->add_wall(x0,++y0);
	    i++;
	    r = rand();
	  }
	  break;
	}
	else if(r%4 == 2){
	  while(double(r)/RAND_MAX > 0.3 && pl->free(x0-1,y0)){
	    pl->add_wall(--x0,y0);
	    i++;
	    r = rand();
	  }
	  break;
	}
	else if(r%4 == 3){
	  while(double(r)/RAND_MAX > 0.3 && pl->free(x0+1,y0)){
	    pl->add_wall(++x0,y0);
	    i++;
	    r = rand();
	  }
	  break;
	}
	else
	  r = rand();
      }
    }
    else
      i--;
  }

  //konstruujemy gracza i AI
  player player1(pl, "Player", ' ');
  AI comp(pl, "Computer", &player1);

  //dodajemy bonusy na plansze, ich ilosc zalezy od rozmiary planszy
  for(unsigned int i=0; i < height*width/100; i++)
    pl->add_bonus();


  //GLOWNA PETLA
  int ch;
  int who_first; //gdy gracze jednoczesnie wejda na pole z bonusem losowane jest kto dostaje punkty
  while( (ch = getch()) != 'q'){
    if(ch == ' '){
      comp.draw_path();
      while( getch() != ' '){
	;
      }
      pl->refresh();
      continue;
    }
    who_first = rand() % 2;
    if(who_first) 
      comp.move();
    
    switch(ch){
    case KEY_LEFT:
      player1.move(left);
      break;
    case KEY_RIGHT:
      player1.move(right);
      break;
    case KEY_UP:
      player1.move(up);
      break;
    case KEY_DOWN:
      player1.move(down);
      break;
    default:
      break;
    }

    if(!who_first)
      comp.move();

    move(LINES-1, COLS-1); //kursor w rog ekranu
  }


  delete pl;

  endwin(); //zakonczenie ncursec

  return 0;
}
