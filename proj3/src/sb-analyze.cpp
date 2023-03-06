#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <map>
#include "disjoint.h"
#include "disjoint_set.hpp"
using namespace std;
using plank::Disjoint_Set;
//works
#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
  public:
    Superball(int argc, char **argv);
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
};

void analyze_board(Superball *s);

void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < (int)strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if ((int)s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

void analyze_board(Superball *s ){
  DisjointSetByRankWPC *set = new DisjointSetByRankWPC((int)s->board.size());

  for (int i = 0; i < (s->r) * (s->c); i++){
    if (s->board[i] == '*' || s->board[i] == '.')   continue;
    
    //check left
    if (i - 1 >= 0){
      //checlk if at start 
      if ( (tolower( (char)s->board[i] )) == tolower( (char)s->board[i-1] )){
        
        if (set->Find(i-1) == set->Find(i))   continue;
        set->Find( set->Union(set->Find(i), set->Find(i-1)) );
      }
    }

    //check right  
    if (i + 1 < (int)s->board.size()){
      //check if at end
       if (tolower((char)s->board[i]) == tolower((char)s->board[i+1])){
        
        if (set->Find(i+1) == set->Find(i))   continue;
        set->Find( set->Union(set->Find(i), set->Find(i+1)) );
       }
    }

    //check top
    if (i - s->c >= 0){
      //check if top row
      if (tolower((char)s->board[i]) == tolower((char)s->board[i- s->c])){
        if (set->Find(i) == set->Find(i - s->c))  continue;

        set->Find( set->Union(set->Find(i), set->Find(i - s->c)) );
      } 
    }

    //check bottom 
    if (i + s->c < (int)s->board.size()){
      //check if bottom row
      if ((tolower((char)s->board[i])) == tolower((char)s->board[i + s->c])){
        if (set->Find(i) == set->Find(i + s->c))  continue;

        set->Find( set->Union(set->Find(i), set->Find(i + s->c)) );
      }
    }

    //Go through all the index
  }

  for (int i = 0; i < (int)s->board.size(); i++)  set->Find(i);

  //a placeholder for goal since goal only have 0 and 1 
  map<int, int> g;
  map<int, int>::iterator it;
  int score_set = 0;
  for (int i = 0; i < (int)s->goals.size(); i++){
    
    if (s->goals[i])  //if something exist at goal 
    //At the current index which ontain parents and not previous connected to other set 
      if (set->Find(i) != -1 && g.find(set->Find(i)) == g.end())  g.insert(pair<int, int>(set->Find(i), i));

  }

  for (it = g.begin(); it != g.end(); ++it){
    int ct = 0;
    
    //if empty
    if (s->board[it->first] == '*')  continue;
    
    for (int i = 0; i < (int)s->board.size(); i++)
      if (set->Find(i) == it->first)  ct++;

    if (ct >= s->mss){
      if (score_set == 0){
        score_set = 1;
        cout << "Scoring sets: " << endl;
      }

      cout << "   Size: " << ct << "Char: " << (char)s->board[it->first] << "Scoring Cell: " << 
        (it->second) / (s->c) << "," << (it->second) % (s->c) << endl;
    }  

  }

  delete set;


}

int main(int argc, char **argv)
{
  Superball *s;
 
  s = new Superball(argc, argv);
  analyze_board(s);

}


