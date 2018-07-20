#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <fstream>
#include <SFML/Audio.hpp>

using namespace std;

#define RED_ON_BLUE 1
#define WHITE_ON_BLACK  2
#define BLACK_ON_YELLOW 3
#define YELLOW_ON_BLACK 4
#define WHITE_ON_RED 5
#define WHITE_ON_BLUE 6

void movSnake(int preX,int preY,int currX,int currY, vector<int> &xcoord, vector<int> &ycoord, int &len, bool &test, int &i);
void checkHit(int currX, int currY, bool &test);
void eraseAsSnakeMoves(vector<int> &xcoord, vector<int> &ycoord, int len);
void checkKey(int key, int &currX, int &currY, int &preX, int &preY, vector<int> &xcoord, vector<int> &ycoord, int &len, bool &test, int &i, int &wall, int &wormHole);
void placeWall(int len, int &wall, int &wormhole, vector<int> &xcoord, vector<int> &ycoord);
void addCoords(vector<int> &xcoord, vector<int> &ycoord, int currX, int currY);
void placeFood(int &currX, int &currY, vector<int> &xcoord, vector<int> &ycoord, int &len, int &i, int &wall, int &wormhole);
void makeWall();
int kbinput(int &previous);
bool kbhit();
void initialize();
void endProgram();
int findLongest(vector<string> list);
bool updateCurrent(int key, int howMany, int &current, vector<bool> &selected);
void displayMenu(vector<string> titles, vector<bool> &selected, int &cur, int longest);
void difficulty(vector<string> titles, vector<bool> selected);
void displayEnd(vector<string> play, vector<bool> &selected, int &cur, int longest, vector <int> xcoord);
bool replay(vector<string> titles, vector<bool> selected);

int main(int argc, char *argv[])
{
    int previous;
    bool playback;
    vector<string> titles;
    vector<string>play;
    titles.push_back("Easy");
    titles.push_back("Medium");
    titles.push_back("Hard");
    titles.push_back("Insane");
    play.push_back("Yes");
    play.push_back("No");

//    sf::SoundBuffer buffer;
//    if (!buffer.loadFromFile("/Users/Edward/desktop/wesley.wav"))
//        return -1;
//    sf::Sound sound;
//    sound.setBuffer(buffer);
//    sound.play();
    do{
        int i = 0; //food counter
        int x=0,y=0;
        int wall = 10;
        int wormHole = 13;
        vector<int>xcoord;
        vector<int>ycoord;
        int preY = 40, preX = 12, currY = 40, currX = 12, len = 5; // Snake's head location
        vector<bool> selected;
        bool again = true;
        bool againTwo = true;
        bool test = true;
        int longest, current=0, howMany;
        initialize();


        selected.push_back(false);
        selected.push_back(false);
        selected.push_back(false);
        selected.push_back(false);
        longest = findLongest(titles);
        howMany = titles.size();
        previous=0;
        while(again)
        {
              previous=0;
              displayMenu(titles, selected, current, longest);
              //if(kbinput())
              x=kbinput(previous);
              again = updateCurrent(x, howMany, current, selected);
        }
        if(x){
            difficulty(titles, selected);
            xcoord.push_back(currX);
            ycoord.push_back(currY);
            srand(time(NULL));
            attron(COLOR_PAIR(WHITE_ON_BLACK));
            makeWall();
            placeFood(currX, currY, xcoord, ycoord, len, i, wall, wormHole);
            movSnake(preX, preY, currX, currY, xcoord, ycoord, len, test, i);
        }
        previous=0;
        do
        {
            x=kbinput(previous);
            checkKey(x, currX, currY, preX, preY, xcoord, ycoord, len, test, i, wall, wormHole);
        }while(test);

        vector<bool> selectedTwo;
        selectedTwo.push_back(false);
        selectedTwo.push_back(false);
        int currentTwo=0;
        longest = findLongest(play);
        previous=0;
        while(againTwo)
        {
            previous=0;
            displayEnd(play, selectedTwo, currentTwo, longest, xcoord);
            //if(kbinput())
            y=kbinput(previous);
            againTwo = updateCurrent(y, play.size(), currentTwo, selectedTwo);
        }
        playback = replay(play,selectedTwo);
    } while(playback);
    return EXIT_SUCCESS;
}
bool replay(vector<string> titles, vector<bool> selected){
    int row = 0;
    move(0,0);
    clear();
    if(selected[0])
        return true;
    else
        endProgram();
    refresh();
}

void displayEnd(vector<string> play, vector<bool> &selected, int &cur, int longest, vector <int> xcoord)
{
    clear();
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    mvprintw(4,30,"Play Again?");
    mvprintw(5,30,"Press Space to Continue");
    mvprintw(3,30, "Score: %d", xcoord.size()+1);
    for(unsigned int i = 0; i < play.size(); i++)
    {
        if(i == cur)
           attron(COLOR_PAIR(5));
        mvprintw(7 + i, 30,"%d. %s",i + 1, play[i].c_str());
        attron(COLOR_PAIR(WHITE_ON_BLACK));
    }
    refresh();
}

void movSnake(int preX,int preY,int currX,int currY, vector<int> &xcoord, vector<int> &ycoord, int &len, bool &test,int &i)
{
    attron(COLOR_PAIR(BLACK_ON_YELLOW));
    checkHit(currX, currY, test);
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    mvprintw(1,2, "Score: %d", xcoord.size() /*"X = %d Y = %d attribute = %d",currX, currY, mvinch(currX, currY)*/);
    //clrtoeol();
    attron(COLOR_PAIR(BLACK_ON_YELLOW));
    for(int i = 0; i < xcoord.size(); i++){
        mvprintw(xcoord[i], ycoord[i], " ");
    }
    eraseAsSnakeMoves(xcoord, ycoord, len);
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    refresh();
}

void checkHit(int currX, int currY, bool &test){ //check
    if(COLOR_PAIR(BLACK_ON_YELLOW) == (mvinch(currX,currY)&A_COLOR) || COLOR_PAIR(WHITE_ON_BLUE) == (mvinch(currX,currY)&A_COLOR)){
        test = false;
    }
}

void eraseAsSnakeMoves(vector<int> &xcoord, vector<int> &ycoord, int len){
    while(xcoord.size() > len){
        attron(COLOR_PAIR(WHITE_ON_BLACK));
        mvprintw(xcoord.front(), ycoord.front(), " ");
        xcoord.erase(xcoord.begin(), xcoord.begin()+1);
        ycoord.erase(ycoord.begin(), ycoord.begin()+1);
    }
}


void checkKey(int key,int &currX,int &currY,int &preX,int &preY,vector<int> &xcoord, vector<int> &ycoord, int &len, bool &test, int &i, int &wall, int &wormHole)
{

    switch((int)key)
    {
        case KEY_LEFT : preY = currY--;
                        //if(currY < 1) currY = 79;
                        addCoords(xcoord, ycoord, currX, currY);
                        break;

        case KEY_UP :   preX = currX--;
                        //if(currX < 1) currX = 23;
                        addCoords(xcoord, ycoord, currX, currY);
                        break;

        case KEY_DOWN : preX = currX++;
                        //if(currX > 23) currX = 1;
                        addCoords(xcoord, ycoord, currX, currY);
                        break;

        case KEY_RIGHT : preY = currY++;
                         //if(currY > 79) currY = 1;
                         addCoords(xcoord, ycoord, currX, currY);
                         break;
        case KEY_F(10) :
        case 27        : endProgram();

        default : return;
    }
    placeWall(len, wall, wormHole, xcoord, ycoord);
    placeFood(currX, currY, xcoord, ycoord, len, i, wall, wormHole);
    movSnake(preX, preY, currX, currY, xcoord, ycoord, len, test, i);
}

void placeWall(int len, int &wall, int &wormhole, vector<int> &xcoord, vector<int> &ycoord){
    int wallX = 1, wallY = 1, holeX = 1, holeY = 1, newX = 1, newY = 1;
    wallX = rand() % 21+1;
    wallY = rand() % 77+1;
    holeX = rand() % 21+1;
    holeY = rand() % 77+1;
    if(len > wall){
        attron(COLOR_PAIR(WHITE_ON_BLUE));
        mvprintw(wallX,wallY," ");
        wall+=10;
    }
    if(len > wormhole){
        attron(COLOR_PAIR(RED_ON_BLUE));
        mvprintw(holeX,holeY,"O");
        wormhole += 13;
    }
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    refresh();
}

void addCoords(vector<int> &xcoord, vector<int> &ycoord, int currX, int currY){
    xcoord.push_back(currX);
    ycoord.push_back(currY);
}

void placeFood(int &currX, int &currY, vector<int> &xcoord, vector<int> &ycoord, int &len, int &i, int &wall, int &wormhole)
{
    int counter = 0;
    int foodX = 1, foodY = 1;
    int warpX = 1, warpY = 1;
    static int newFood = 0;
    warpX = rand() % 21+1;
    warpY = rand() % 77+1;
    if(len % 15 == 0 && newFood == 0){
        foodX = rand() % 21+1;
        foodY = rand() % 77+1;
        attron(COLOR_PAIR(WHITE_ON_RED));
        mvprintw(foodX,foodY,"@");
        newFood++;
    }
    if(COLOR_PAIR(WHITE_ON_RED) == (mvinch(currX,currY)&A_COLOR)){
        foodX = rand() % 21+1;
        foodY = rand() % 77+1;
        /*while(((mvinch(foodX,foodY)&A_COLOR) != COLOR_PAIR(WHITE_ON_BLACK))){
            foodX = rand() % 21+1;
            foodY = rand() % 77+1;
        }*/

        attron(COLOR_PAIR(WHITE_ON_RED));
        mvprintw(foodX,foodY,"@");
        len++;
        addCoords(xcoord, ycoord, currX, currY);
        cout << "\a";
        //placeWall(len, wall, wormhole, xcoord, ycoord);
        refresh();
    }
    if(COLOR_PAIR(RED_ON_BLUE) == (mvinch(currX,currY)&A_COLOR)){
        //len+=4;
        currX = warpX;
        currY = warpY;
        addCoords(xcoord, ycoord, currX, currY);
    }
    while(counter < 6 && i < 5)
    {
        foodX = rand() % 21+1;
        foodY = rand() % 77+1;
        /*while(((mvinch(foodX,foodY)&A_COLOR) != COLOR_PAIR(WHITE_ON_BLACK)) && ((mvinch(foodX, foodY) & A_CHARTEXT) != ' ')){
            foodX = rand() % 21+1;
            foodY = rand() % 77+1;
        }*/
        attron(COLOR_PAIR(WHITE_ON_RED));
        mvprintw(foodX,foodY,"@");
        refresh();
        i++; //count initial
    }
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    refresh();
}


void makeWall()
{
    attron(COLOR_PAIR(WHITE_ON_BLUE));
    for(int i = 0; i < 79; i++){
        mvprintw(0,i," ");
    }
    for(int i = 0; i < 24; i++){
        mvprintw(i,0," ");
    }
    for(int i = 0; i < 79; ++i){
        mvprintw(23,i," ");
    }
    for(int i = 0; i < 24; i++){
        mvprintw(i, 78," ");
    }
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    refresh();
}


void initialize()
{
    initscr();      //Initializes ncurses
    cbreak();       //Disables line buffering and erase/kill character-processing
    noecho();       //Turns off echoing of keyboard input
    start_color();  //Turns on the 16 colors
    //timeout(100);   //Says how long to wait, in millisecs, for user input. After that time, program continues
    keypad(stdscr, TRUE); //Enable keypad input
    curs_set(0);    //Sets the cursor to invisible.

    if (!has_colors ()) {
        endwin ();
        fputs ("No colors!", stderr);
        exit (1);
    }

    init_pair (RED_ON_BLUE, COLOR_RED, COLOR_BLUE);
    init_pair (WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair (YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair (BLACK_ON_YELLOW, COLOR_BLACK, COLOR_YELLOW);
    init_pair (WHITE_ON_RED, COLOR_WHITE, COLOR_RED);
    init_pair (WHITE_ON_BLUE, COLOR_WHITE, COLOR_BLUE);
}

int kbinput(int &previous)
{
     //static int previous;
     int ch = getch();
     if(ch == ERR)
         return previous;
     if(ch != previous)
         previous = ch;
     return ch;
}

void endProgram()
{
    erase ();
    refresh ();
    endwin ();
    exit(0);
}

int findLongest(vector<string> list)
{
    unsigned int longest = 0;
    for(unsigned int i = 0; i < list.size(); i++)
      if(longest < list[i].size())
        longest = list[i].size();
    return longest;
}


bool updateCurrent(int key, int howMany, int &curr, vector<bool> &selected)
{
     //int key = getch();
     switch((int)key)
     {
         case 32       : selected[curr] = !selected[curr];
                         return false;
                         break;

         case KEY_UP   : if(--curr < 0)
                           curr = howMany -1;

                         break;

         case KEY_DOWN:  if(++curr >= howMany)
                                curr = 0;

                         break;

         case KEY_F(10): return false;

         case 27       : endProgram();
     }
     return true;
}

void displayMenu(vector<string> titles, vector<bool> &selected, int &cur, int longest)
{
    //int y = 40 - (longest + 3)/2;
    attron(COLOR_PAIR(WHITE_ON_BLACK));
    mvprintw(4,30,"Snake Game");
    mvprintw(5,30,"Choose Difficulty");
    mvprintw(6,30,"Press Space to Continue");
    for(unsigned int i = 0; i < titles.size(); i++)
    {
        /*if(!selected[i])
        {
           attron(COLOR_PAIR(WHITE_ON_BLACK));
           //mvprintw(7 + i, 60,"Unselected");
           clrtoeol();
        }
        else
        {
           attron(COLOR_PAIR(BLACK_ON_YELLOW));
           //mvprintw(7 + i, 60,"Selected");
           clrtoeol();
        }*/
        if(i == cur)
           attron(COLOR_PAIR(5));
        mvprintw(8 + i, 30,"%d. %s",i + 1, titles[i].c_str());
        attron(COLOR_PAIR(WHITE_ON_BLACK));
    }
    refresh();
}

void difficulty(vector<string> titles, vector<bool> selected)
{
     int row = 0;
     move(0,0);
     clear();
     //for(unsigned int i = 0;i < titles.size(); i++)
        //if(selected[i])
           //mvprintw(6+row++, 7,"%s", titles[i].c_str());
     if(selected[0])
         timeout(100);
     else if(selected[1])
         timeout(75);
     else if(selected[2])
         timeout(50);
     else if(selected[3])
         timeout(25);
     refresh();
}
