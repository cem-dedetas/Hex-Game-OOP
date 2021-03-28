/*Nurettin Cem DEDETAS
**171044028
**HW #3
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>

#define ARR_SIZE 200/*Default array size for the trail array*/

using namespace std;

/*Tile types*/
enum tile_t
{
    empty = '.',
    user1 = 'x',
    user2 = 'o',
    comp = 'o',
    winX = 'X',
    winO = 'O'
};

/*Game types*/
enum game_t{
    unkwn = -1,
    pvp = 0,
    pve = 1
};


/*Hex class :*/
class Hex
{
private:
    
    int size,/*Board size*/
        moves;/*total amount of moves made*/
    game_t gameMode;/*0 is pve, 1 is pvp*/
    static int marked;/*Number of marked tiles*/
    
    /*Inner class: Cell*/
    class Cell {
        private:

            int xpos, ypos;/*Coordinates of a cell object*/
            tile_t tile;/*type of tile (x,o)*/
        public:
            /*Constructors for Cell class*/
            Cell(int x, int y, tile_t tiletype);
            Cell();

        /*getters and setters for position data*/
            int getxpos(){return xpos;}
            inline int setxpos(int xpos){return this->xpos = xpos;}

            int getypos(){return ypos;}
            inline int setypos(int ypos){return this->ypos = ypos;}
        /*getter an setter for tyle type*/
            tile_t setTile(tile_t tile){return this->tile = tile;}
            char getTile(){return tile;}

            
    };
    /*Board vector*/
    vector< std ::vector<Cell>> hexCells;

    void saveBoard(string filename);
    void loadBoard(string filename);
    game_t getGameT(){return this->gameMode;}
    bool winCheck(int trail[][2],int * trailIndex, int xPos, int yPos, bool forX);
    void arrayFiller(int arr[][2], int arrsize);
    bool isIncluded(int arr[][2], int x , int y, int arrsize);
    void helper( int arr[][2],int arrsize, char a );
    void compMoves(char * x, int * y);
    int getNumofMoves(){return this->moves;}
    void initCells();
    
    int getMove(char * letter, int * number);
    
public:
    Hex();

    inline int setSize(int a){return this->size = size;}/*Manually setting game size*/
    int getSize(){return size;}/*getter for size variable*/

    void printHex();
    void playGame();
    bool compareGames(Hex otherGame);
    
    /*The class will have a static function that returns 
    *the number of marked (non-empty) cells in all the games. 
    *This is that static function
    */
    static int numOfMarked(){return marked;}
    
    
};



int main()
{
    Hex hex0, hex1, hex2, hex3, hex4;

    hex0.playGame();
    hex1.playGame();
    hex2.playGame();
    hex3.playGame();
    hex4.playGame();

    hex2.compareGames(hex3);
    
    return 0;
}


int Hex :: marked = 0;

/*Defult constructor for Cell class*/
Hex::Hex()
{   
    int temp;
    do
    {
        cout << "\nWhat size the board shall be? :";
        cin >> temp;
        if(temp<5){
            cout << "\nHas to be bigger than(or equal to) 5";
        }
    } while (temp<5);

    this->size = temp;
    this->moves = 0;
    initCells();
}

/*Constructor for Cell class with data given*/
Hex :: Cell :: Cell(int x, int y, tile_t tiletype){
    this->xpos = x;
    this->ypos = y;
    this->tile = tiletype;
}

/*Defult constructor for Cell class*/
Hex :: Cell :: Cell(){
    this->xpos = 0;
    this->ypos = 0;
    this->tile = empty;
}

/*fill the board with '.' */
void Hex :: initCells(){
    vector<Cell> tempRow;
    Cell tempCell;
    
    for (int i = 0; i < size; ++i){
        
        for(int j = 0 ; j< size; ++j){
            tempCell.setxpos(i);
            tempCell.setypos(j);
            tempCell.setTile(empty);
            tempRow.push_back(tempCell);
        };
        hexCells.push_back(tempRow);
        tempRow.clear();
    }
}

/*print board in asked format*/
void Hex :: printHex(){

        for(int a = 'a'; a< ('a'+size); ++a) cout << " " << (char)a ;
        cout<<endl;
        
	    for (int i = 0; i < size; ++i)
	    {
	    	cout << i+1;
	    	for (int k = 0; k <= i ; ++k)
	    		cout << " ";
    
	    	for (int  j = 0; j < size; ++j)
	    	{
	    		cout << hexCells[j][i].getTile();
	    		cout << " ";
	    	}
	    	cout << endl;
	    }
	    return;
    }

/*get move from user*/
int Hex:: getMove(char * letter, int * number){
    string sTurn = (moves % 2 == 0)? "X's turn:" : "O's turn:";
	string temp0, temp1;
	cout << sTurn;

	if((((int)gameMode)==1) && ((moves % 2 == 0)==1)){
		compMoves(letter , number);
		return 1;
	}

	cin >>temp0 >> temp1;
	
	if(temp0 == "SAVE"){
		saveBoard(temp1);
		return 2;
	}
	else if(temp0 == "LOAD"){
		loadBoard(temp1);
		return 3;	
	}
	else {
		stringstream toInt(temp1);
		toInt >> *number;
		*letter = temp0[0];

		if((*letter >= 'A' && *letter <= 'A' + size)) letter -= (int)('a'-'A');

		return ((*letter >= 'a' && *letter <= 'a' + size) && (*number >= 1 && *number <= size));
	}
}

/*Saves the board to ./filename*/
void Hex ::saveBoard(string filename){
    fstream myFile;
	myFile.open(filename, ios::out);
	myFile<< size<<" "<<(int)gameMode<<" "<<moves<<" ";
	for(int i=0; i<size; ++i){
		for (int j = 0; j <size; ++j){
			myFile << (char)hexCells[i][j].getTile();
		}
	}
	myFile.close();
}

/*Loads the board from ./filename*/
void Hex :: loadBoard(string filename){
	fstream myFile;
	char temp;

    int gameTemp;
    
	vector<Cell> tempRow;
    Cell tempCell;

    hexCells.clear();

	myFile.open(filename, ios::in);
	myFile >> size >> gameTemp >> moves;
	myFile.get(temp);
    
    gameMode = (game_t)gameTemp;

    for (int i = 0; i < size; ++i){
        
        for(int j = 0 ; j< size; ++j){
            myFile.get(temp);
            tempCell.setxpos(i);
            tempCell.setypos(j);
            tempCell.setTile((tile_t)temp);
            tempRow.push_back(tempCell);
        };
        hexCells.push_back(tempRow);
        tempRow.clear();
    }
	myFile.close();    
}

/*Generate moves for computer to play*/
void Hex :: compMoves(char * x, int * y){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    /* using nano-seconds instead of seconds */
    srand((time_t)ts.tv_nsec);
  	int r[2];

  	for(int i =0; i<2; ++i)r[i] = rand()%size;
	auto xRand = r[0];
	decltype(*y) yRand = r[1];


	while(hexCells[xRand][yRand].getTile() != empty){
		for(int i =0; i<2; ++i)r[i] = rand()%size;
		xRand = r[0];
		yRand = r[1];
	}
	
	*x = xRand + 'a';
	*y = yRand+1;

	cout << endl <<"Computer's move :"<< *x<<" "<< *y<<endl;
}

/*Main function that game takes place on
** The class should have a function that returns if the game ended(this is that class function)

**The class should have a function named playGame. This function plays the game by asking
    the user the board size first then asks the user to play and the computer plays, etc.
    (Also that function aswell)
*/
void Hex :: playGame(){
    char letter;
	int number, index = 0, tempGameT;
	bool stop = false;
	int trail[ARR_SIZE][2];



	arrayFiller(trail, ARR_SIZE);

	while(!(gameMode == pve || gameMode == pvp)){
		cout << "Vs. Player(0) or Vs. Comp.(1) (Please input 1 or 0):";
		cin >>tempGameT;
        gameMode = (game_t)tempGameT;

		if(!(gameMode == pve || gameMode == pvp)){
			cerr << "\nInvalid Game Type!!"<< endl;
			cout <<"\n(Please input 1 or 0):"<<endl;
		}
	}

	printHex();

	while(!stop){
        int temp;
		temp = getMove(&letter, &number);

        


		while (temp==0){
			cerr << endl << letter << number << "<- this move is invalid.";
			cout << " \n Try again:" << endl;
			temp = getMove(&letter, &number);
		}
	 
		if (temp == 1){
			if(hexCells[letter-'a'][number-1].getTile() == empty){
				if(moves % 2 == 0) hexCells[letter-'a'][number-1].setTile(user1);
            	else hexCells[letter-'a'][number-1].setTile(user2);
                
                ++marked;
				++moves; 
			}

			else cout <<  "Inavlid move!! (there is an \'" << hexCells[letter-'a'][number-1].getTile() << "\' there) try again." << endl;
	
			printHex();
            
        	for (int i = 0; i < size; i++)
        	{
			

        	    if(hexCells[0][i].getTile() == user1)
        	        if(winCheck(trail, &index, 0, i, true)){
						stop = true;
        	        	helper(trail, index, winX);
						printHex();
					}
					arrayFiller(trail, ARR_SIZE);
					
        	        index = 0;
        	    if(hexCells[i][0].getTile() == user2)
        	        if(winCheck(trail, &index, i, 0, false)){
						stop = true;
        	        	helper(trail, index, winO);
						printHex();

					}
        	        arrayFiller(trail, ARR_SIZE);
        	        index = 0;
        	}
        	arrayFiller(trail, ARR_SIZE);
		}

		else if(temp == 2){
			cout<<endl<<"SAVED SUCCESFULY"<<endl;
			printHex();
		}
		else if(temp == 3){
			cout<<endl<<"LOADED SUCCESFULY"<<endl;
			printHex();
		}
	}
	return;    
}

/*Main game logic: when an edge tile is placed , checks a winning path recursively. While in recursive function, 
* holds a list of previously visited tiles so it doesnt go in an infinite loop.
*/
bool Hex :: winCheck(int trail[][2],int * trailIndex, int xPos, int yPos, bool forX){
    trail[*trailIndex][0] = xPos;
    trail[*trailIndex][1] = yPos;
    (*trailIndex)++;
    bool isDone = false;

    if(forX && xPos==size-1){
        cout << "\n'X' WON!!"<<endl;
        return true;
    }
    if(!forX && yPos == size-1){
        cout<< "\n'O' WON!!"<<endl;
        return true;
    }

    if(xPos-1 >= 0){
        if(!isIncluded(trail, xPos-1, yPos, *trailIndex) && ((forX && hexCells[xPos-1][yPos].getTile()==user1) || (!forX && hexCells[xPos-1][yPos].getTile()==user2))){
            isDone = winCheck(trail ,trailIndex,xPos-1,yPos, forX);
        }
	    if(isDone)return true;
    }
    
    if(xPos-1 >= 0 && yPos+1<=size){
        if(!isIncluded(trail, xPos-1, yPos+1, *trailIndex) && ((forX && hexCells[xPos-1][yPos+1].getTile()==user1) || (!forX && hexCells[xPos-1][yPos+1].getTile()==user2))){
            isDone = winCheck(trail ,trailIndex,xPos-1,yPos+1, forX);
        }
	    if(isDone)return true;
    }

    if(yPos+1<=size){
        if(!isIncluded(trail, xPos, yPos+1, *trailIndex) && ((forX && hexCells[xPos][yPos+1].getTile()==user1) || (!forX && hexCells[xPos][yPos+1].getTile()==user2))){
            isDone = winCheck(trail ,trailIndex,xPos,yPos+1, forX);
        }
	    if(isDone)return true;        
    }

    if(yPos-1>=0){
        if(!isIncluded(trail, xPos, yPos-1, *trailIndex) && ((forX && hexCells[xPos][yPos-1].getTile()==user1) || (!forX && hexCells[xPos][yPos-1].getTile()==user2))){
            isDone = winCheck(trail ,trailIndex,xPos,yPos-1, forX);
        }
	    if(isDone)return true;
    }

    
    if(xPos+1<=size){
        if(!isIncluded(trail, xPos+1, yPos, *trailIndex) && ((forX && hexCells[xPos+1][yPos].getTile()==user1) || (!forX && hexCells[xPos+1][yPos].getTile()==user2))){
            isDone = winCheck(trail ,trailIndex,xPos+1,yPos, forX);
        }
	    if(isDone)return true;       
    }
     
    if(xPos+1<=size && yPos-1>=0){
        if(!isIncluded(trail, xPos+1, yPos-1, *trailIndex) && ((forX && hexCells[xPos+1][yPos-1].getTile()==user1) || (!forX && hexCells[xPos+1][yPos-1].getTile()==user2))){
            isDone = winCheck(trail ,trailIndex,xPos+1,yPos-1, forX);
        }
        if(isDone)return true;
    }
    
    return false;
}

/*fills the visited tile array with -1's because array indexes cant be -1 so it acts like empty in comparison*/
void Hex :: arrayFiller(int arr[][2],int arrsize){

	for (int i = 0; i < arrsize; ++i)
	{
		arr[i][0] = arr[i][1] = -1;
	}
	

	return;
}

/*Checks if a tile has been previously visited by winCheck funtion*/
bool Hex :: isIncluded(int arr[][2], int x , int y, int arrsize){
    for(int i = 0; i<arrsize; ++i){
        if(arr[i][0]==x && arr[i][1]==y)return true;
    }
    return false;
}


/*Makes the winning path capital letters*/
void Hex :: helper( int arr[][2],int arrsize, char a ){
    tile_t ch;

    ch = (a == 'X')? winX : winO; 

    for (int i = 0; i < arrsize; i++)
    {
        hexCells[arr[i][0]][arr[i][1]].setTile(ch);
    }
	return;
}

/*
*Compare a given game's marked tile count with the current one's tile count

*The class will have a function that takes another object Hex as parameter and
compares the Hex games. It returns true is the first game has more marked cells for the user,
otherwise it returns false
*/
bool Hex::compareGames(Hex otherGame){
    if(this->moves > otherGame.moves)return true;
    else return false;
}