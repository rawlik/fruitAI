#ifndef _GAME_CLASSES_HPP
#define _GAME_CLASSES_HPP

#include <ncurses.h>
#include <vector>
#include <list>
#include <cstdlib>
#include "graph/matrix_graph.hpp"
#include "graph/lists_graph.hpp"


class playground;
class player;
class cursor;
class AI;
class bonus_class;

enum direction_t {
  up, down, left, right
};

enum obj_t {
  wall_t, cursor_t, bonus_t
};


//OBIEKT NA PLANSZY
class game_object {
  friend class playground;
protected:
  playground * pl; //na jakiej planszy jest
  char character;  //jak go drukowac
  int color;
  obj_t type;
  unsigned int x;  //polozenie na planszy
  unsigned int y;
public:
  void draw();
  unsigned int X(){
    return x;
  }
  unsigned int Y(){
    return y;
  }
  obj_t Type(){
    return type;
  }
};

//SCIANA
class wall : public game_object {
  friend class playground;
  wall(playground * Ipl, unsigned int Ix, unsigned int Iy, char Ich, int Icolor);
};

//BONUS
class bonus : public game_object {
  friend class playground;
  int points;
  bonus(playground * pl, unsigned int x, unsigned int y, int points);
public:
  int Points(){
    return points;
  }
};

  
//KURSOR
class cursor : public game_object {
  friend class player;
  player * owner;
  cursor(playground * pl, player * owner, char ch);
public:
  ~cursor();
  void move(direction_t where);
};




//PLANSZA
class playground {
  friend class cursor;
  friend class AI;
  friend class player;

  std::vector< std::vector<game_object *> > objects; //macierz obiektow
  unsigned int x0; //polozenie lewego gornego rogu planszy
  unsigned int y0;
  unsigned int width; //szerokosc planszy
  unsigned int height;
public:
  playground();
  playground(unsigned int width, unsigned int height);
  ~playground();

  void make_null(); //logiczne czyszczenie planszy
  void clear(); //graficzne czyszczenie planszy
  void draw();
  void refresh(bool clean_first = true);
  bool free(int x, int y); //pytanie, czy dane pole jest wolne
  wall*  add_wall(unsigned int x, unsigned int y);
  bonus* add_bonus(void);

  unsigned int X0(){ return x0; }
  unsigned int Y0(){ return y0; }
  unsigned int Width(){ return width; }
  unsigned int Height(){ return height; }  
};




//GRACZ
class player {
  friend class AI;

  static int count; //ile graczy
  int number; //ktory gracz z kolei
  int score; //wynik
  int score_pos; //pozycja drukowania wyniku
  cursor cur; //kursor gracza
  playground * pl;
  const char * name;
public:
  player(playground * pl, const char * name, char ch = ' ');
  void add_score(int n);

  void move(direction_t where){
    cur.move(where);
  }
  cursor * my_cursor(){
    return &cur;
  }
  unsigned int X(){
    return cur.X();
  }
  unsigned int Y(){
    return cur.Y();
  }
};


//AI
class AI {
  player me;
  player * opponent;
  unsigned int next_x;
  unsigned int next_y;
  unsigned int next_intermediary_x;
  unsigned int next_intermediary_y;
  std::list<Vertex> path;

  Vertex ***** intermediary;
  //chodzi o: Vertex * intermediary[COLS][LINES][COLS][LINES];
  int **** dist;
  //int dist[COLS][LINES][COLS][LINES];
  Vertex *** vertices;
  //Vertex * vertices[COLS][LINES];
  GraphAsLists model;

  void eval_path(); //obliczanie optymalnego w danej chwili ruchu
  void dig(Graph & g, int V, std::vector<bool> visited, int dist, int sum, double & best, std::list<Vertex> current_path); //pomocnicza funckja do obliczania sciezki
  unsigned int distance(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
  void create_playground_model(); //obliczanie najkrotszych sciezek na calej planszy
  void loading_screen(double s);
public:
  AI(playground * pl, const char * name, player * opponent);
  ~AI();

  void move();
  void draw_path(); //rysowanie zamiarow

  player * my_player(){
    return &me;
  }
};

#endif
