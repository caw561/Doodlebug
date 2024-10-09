#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <vector>
using namespace std;

int const grid_size = 400;

class Organism
{
    protected:
    int position;
    int survival_count;
    bool set;

    public:
    Organism():position(0),survival_count(0), set(true){}
    Organism(int position, int survival_count):position(position),survival_count(survival_count),set(false){}
    Organism(int position):position(position),survival_count(0), set(true){};
    virtual ~Organism(){};
    
    virtual int move()=0;
    virtual string get_type()=0;
    int get_survival_count(){return survival_count;};
    virtual void set_position(int new_position)=0;
    virtual void set_survival_count(int new_count)=0;
    void add_one_survival_count(){survival_count+=1;};
    virtual int get_starvation_count()=0;
    virtual void set_starvation_count(int x)=0;
    virtual void add_one_starve_count()=0;
    int get_position(){return position;};
    bool get_set(){return set;};
    void set_set(bool new_set){set = new_set;}
};

class Doodlebug : public Organism
{
    int starvation_count;
    
    public:
    Doodlebug():Organism(),starvation_count(0){};
    Doodlebug(int position):Organism(position), starvation_count(0){}
    int move();
    string get_type() {return "Doodlebug";}
    void set_position(int new_position){position = new_position;}
    void set_survival_count(int new_count){survival_count=new_count;};
    int get_starvation_count(){return starvation_count;};
    void set_starvation_count(int new_starve_count){starvation_count = new_starve_count;};
    void add_one_starve_count(){starvation_count+=1;};
};

class Ant : public Organism
{
    public:
    Ant():Organism(){};
    Ant(int position):Organism(position){}
    int move();
    string get_type() {return "Ant";}
    void set_position(int new_position){position = new_position;}
    void set_survival_count(int new_count){survival_count=new_count;};
    int get_starvation_count(){return -1;};
    void set_starvation_count(int x){};
    void add_one_starve_count(){};
};

class Grid
{
    Organism* grid[grid_size];
    int num_ants;
    int num_DBs;
    int timestep;

    public:
    Grid(){intialize();};
    ~Grid();
    
    void intialize();//intializes grid and places 100 ants and 8 doodlebugs
    void print(); //prints grid
    bool full_grid();//Checks to see if grid is full
    bool add_ant(int position);//adds ants, returns true if ant was added
    bool add_DB(int position);//adds doodlebugs, returns true if doodlebug was added
    void play(){reset_board();play_Doodlebugs();play_Ants();print();};
    void play_Doodlebugs();
    void play_Ants();
    void reset_board();
    int find_empty_position(int current_position);
    int find_ant_position(int DB_position);
};

class World
{
    Grid grid;

    public:
    void play(){grid.play();};
    void print(){grid.print();};
};

int main()
{
    srand(time(NULL));
    World world;
    string input;
    
    cout<<"Press Enter to play. Press anything else to stop"<<endl;
    world.print();

    while(input == ""){
        getline(cin, input);
        world.play();
        cout<<endl;
    }
    
    return 0;
}

void Grid::intialize()
{
    num_ants=0;
    num_DBs=0;
    timestep = 1;
    
    //initialize the  array by setting all pointers to NULL
    for(int i=0; i<grid_size; i++){
        grid[i] = nullptr;
    }

    
    //Add in 100 intial ants
    for(int i=0; i<100; i++){
       if(add_ant(rand()%grid_size)==false)
       {
            i--; //if ant was not place, retry ant i
       }
    }

    //Add in 8 intial doodlebugs
    for(int i=0; i<8; i++){
        if(add_DB(rand()%grid_size)==false)
        {
            i--; //if doodlebug was not placed, retry doodlebug i
        }
    }    


}

void Grid::print()
{
    for(int i=0; i<grid_size; i++)
    {
        if(i%20==0){
            cout<<endl;
        }

        if(grid[i]==nullptr){
            cout<<"-";
        }
        else if(grid[i]->get_type()=="Doodlebug"){
            cout<<'X';
        }
        else{
            cout<<'O';
        }
    }
    cout<<"Ants: "<<num_ants<<" DBs: "<<num_DBs<<endl;
}

bool Grid::add_ant(int position)
{
   //If position is empty, create a new ant and return true, if grid is full or requested position is not empty, return false 
   if(full_grid()==false){
        if(grid[position]==nullptr){
            grid[position]= new Ant(position);
            num_ants++;
            return true;
        }   
   }
    return false;
}

bool Grid::add_DB(int position)
{
    if(full_grid()==false){
        if(grid[position]==nullptr){
            grid[position]= new Doodlebug(position);
            num_DBs++;
            return true;
        }
    }
    return false;
}

bool Grid::full_grid()
{
    if(num_ants+num_DBs == grid_size){
        return true;
    }

    return false;
}

int Doodlebug::move()
{
    //Choose random direction to move
    int direction = rand()%4;
    int new_position = position;

    switch (direction)
    {
    case 0:
        new_position += 20;
        break;
    
    case 1:
        new_position -= 20;
        break;
    case 2:
        new_position +=1;
        if(new_position%20==0){
            new_position = position;
        }
        break;
    case 3:
        if(position%20!=0){
            new_position -=1;
        }
        
        break;
    }

    //checks to see if new position is within grid
    if(new_position>=0&&new_position<grid_size){
        position = new_position;
    }

    return position;
}

int Ant::move()
{
    int direction = rand()%4;
    int new_position = position;

    switch (direction)
    {
    case 0:
        new_position += 20;
        break;
    
    case 1:
        new_position -= 20;
        break;
    case 2:
        new_position +=1;
        if(new_position%20==0){
            new_position = position;
        }
        break;
    case 3:
        if(position%20!=0){
            new_position -=1;
        }
        
        break;
    }

    if(new_position>=0&&new_position<grid_size){
        position = new_position;
    }

    return position;
}

Grid::~Grid()
{
    for(int i=0; i< grid_size; i++){
        if(grid[i]!= nullptr){
            delete grid[i];
        }
    }
}

int Grid::find_ant_position(int DB_position)
{
    //makes a vector of all the near by ants, then randomly choose one ant position to return
    int North = DB_position-20;
    int South = DB_position+20;
    int East = DB_position+1;
    int West = DB_position-1;
    vector<int> ants;
    int num_adjacent_ants =0;
    
    if(North>=0&&North<grid_size&&grid[North]!=nullptr&&grid[North]->get_type()=="Ant"){
        num_adjacent_ants++;
        ants.push_back(North);
    }
    if(East%20!=0&&East>=0&&East<grid_size&&grid[East]!=nullptr&&grid[East]->get_type()=="Ant"){
        num_adjacent_ants++;
        ants.push_back(East);
    }
    if(South>=0&&South<grid_size&&grid[South]!=nullptr&&grid[South]->get_type()=="Ant"){
        num_adjacent_ants++;
        ants.push_back(South);
    }
    if(DB_position%20!=0&&West>=0&&West<grid_size&&grid[West]!=nullptr&&grid[West]->get_type()=="Ant"){
        num_adjacent_ants++;
        ants.push_back(West);
    }
    
    if(num_adjacent_ants==0){
        return -1;
    }
    else{
        return ants[rand()%num_adjacent_ants];
    }
}

void Grid::play_Doodlebugs()
{
    //Move Doodlebug   
    for(int i=0; i<grid_size; i++){
        if(grid[i]!=nullptr&&grid[i]->get_type()=="Doodlebug"&&grid[i]->get_set()==false){
            int new_position = find_ant_position(i);
            if(new_position!=-1){
                num_ants--;
                delete grid[new_position];//eats ant
                grid[new_position] = grid[i];
                grid[i]=nullptr;
                grid[new_position]->set_starvation_count(0);
                grid[new_position]->set_position(new_position);
                grid[new_position]->set_set(true);
            }
            else{
                //if no ants to eat, try to move somewhere else and add one to starvation count
                new_position = grid[i]->move();
                if(grid[new_position]!= nullptr){
                    grid[i]->set_position(i);
                    grid[i]->add_one_starve_count();
                    grid[i]->set_set(true);
                }
                else{
                    grid[new_position] = grid[i];
                    grid[i]=nullptr;
                    grid[new_position]->add_one_starve_count();
                    grid[new_position]->set_position(new_position);
                    grid[new_position]->set_set(true);
                }
            }
        }
    }
    //Breed or starve Doodlebug if possible
    for(int i=0; i<grid_size; i++){
        if(grid[i]!=nullptr&&grid[i]->get_type()=="Doodlebug"){
            int breed_position = find_empty_position(i);    
            if(breed_position!=-1&&grid[i]->get_survival_count()>=8){
                add_DB(breed_position);
                grid[i]->set_survival_count(0); 
            }
            if(grid[i]->get_starvation_count()>=3){
                delete grid[i];
                grid[i] = nullptr;
                num_DBs--;
            }
        }
    }
}

int Grid::find_empty_position(int current_position)
{
    //makes a vector of all empty cell options and randomly choose one to return
    int North = current_position-20;
    int South = current_position+20;
    int East = current_position+1;
    int West = current_position-1;
    vector<int> empty_options;
    int num_empty=0;
    
    if(North>=0&&North<grid_size&&grid[North]==nullptr){
        num_empty++;
        empty_options.push_back(North);
    }
    if(East%20!=0&&East>=0&&East<grid_size&&grid[East]==nullptr){
        num_empty++;
        empty_options.push_back(East);
    }
    if(South>=0&&South<grid_size&&grid[South]==nullptr){
        num_empty++;
        empty_options.push_back(South);
    }
    if(current_position%20!=0&&West>=0&&West<grid_size&&grid[West]==nullptr){
        num_empty++;
        empty_options.push_back(West);
    }
    
    if(num_empty==0){
        return -1;
    }
    else{
        return empty_options[rand()%num_empty];
    }
}

void Grid::play_Ants()
{
    //Move Ants
    for(int i=0; i<grid_size; i++){
        if(grid[i]!=nullptr&&grid[i]->get_type()=="Ant"&&grid[i]->get_set()==false){
            int new_position = grid[i]->move();
            
            if(grid[new_position]!= nullptr){
                grid[i]->set_position(i);
                grid[i]->set_set(true);
            }
            else{
                grid[new_position] = grid[i];
                grid[i]=nullptr;
                grid[new_position]->set_set(true);
            }  
        }
    }
    //attempts to breed if possible
    for(int i=0; i<grid_size; i++){
        if(grid[i]!=nullptr&&grid[i]->get_type()=="Ant"&&grid[i]->get_survival_count()>=3){
            int breed_position = find_empty_position(i);
            if(breed_position!=-1){
                add_ant(breed_position);
                grid[i]->set_survival_count(0); 
            }
        }
    }
}

void Grid::reset_board()
{
    //Adds one to everyone's suvival count and resets set_position to false
    for(int i=0; i<grid_size; i++){
        if(grid[i]!=nullptr){
            grid[i]->add_one_survival_count();
            grid[i]->set_set(false);
        }
    }
}