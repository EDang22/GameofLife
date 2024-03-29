/*
* Ethan Dang
* 2326380
* edang@chapman.edu
* CPSC 350-02
* Assignment 2
* GameDriver class: gets all information needed before running the simulation
*/
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "GameDriver.h"

using namespace std;

// default constructor
GameDriver::GameDriver()
{
  row = 0;
  col = 0;
  dens = 0.0;
  grid = "";
  mode = ' ';
  output = "";
  oF = false;
  game;
}

// Starts game, asks user for information needed to create game of life object
void GameDriver::startGame()
{
  char pref = ' ';
  cout << "Welcome to Game of Life!" << endl
       << "Would you like to provide a file or have a random configuration? (Enter F for file, R for random, E to exit)" << endl;
  cin >> pref;
  if (cin.fail())
  {
    cin.clear();
    cin.ignore();
    cout << "Error: invalid answer" << endl;
    startGame();
    return;
  }
  else
  {
    if (tolower(pref) == 'f') // if file will be given
    {
       if (!getGrid()) // restarts process if file is not proper format
       {
         cout << endl;
         startGame();
         return;
       }
    }
    else if (tolower(pref) == 'r') // if grid will be randomly generated
    {
      genGrid(); // generates grid
    }
    else if (tolower(pref) == 'e') // if user wishes to exit
    {
      cout << endl << "Goodbye!" << endl;
      return;
    }
    else
    {
      cout << "Error: invalid input" << endl << endl;
      cin.clear();
      cin.ignore();
      startGame();
      return;
    }
    getMode(); // gets mode and method of output
    game = GameofLife(row, col, grid, mode, output, oF);
  }
  return;
}

// Opens file to read flat file configuration with error checks
bool GameDriver::getGrid()
{
  cout << "Enter file path to grid configuration" << endl;
  string path = "";
  cin >> path;
  grid = "";
  ifstream file(path);
  if (file)
  {
    string nextLine = "";
    int numRows = 0;
    int numCol = 0;
    int numLines = 0;
    // reads first 2 lines of file,
    // if proper format,
    // these should be the number of rows and columns
    try
    {
      getline(file, nextLine);
      numRows = stoi(nextLine);
      getline(file, nextLine);
      numCol = stoi(nextLine);
    }
    catch(invalid_argument const &e)
    {
      cout << "Error: improper file format" << endl;
    }
    catch(out_of_range const &e)
    {
      cout << "Error: improper file format" << endl;
    }
    // reads in grid after rows and columns given
    while (getline(file, nextLine))
    {
      ++numLines;
      if (nextLine.length() == numCol)
      {
        grid = grid + nextLine;
      }
      else
      {
        cerr << "Error: File has invalid formatting" << endl;
        return false;
      }
    }
    if (grid.length() != (numCol * numRows))
    {
      cerr << "Error: File had invalid formatting" << endl;
      return false;
    }
    row = numRows;
    col = numCol;
  }
  else
  {
    cerr << "Error: File could not be opened" << endl;
    return false;
  }
  return true;
}

// Gets dimensions and density of grid from user, then generates grid
void GameDriver::genGrid()
{
  grid = "";
  cout << "Enter the number of rows in the grid" << endl;
  cin >> row;
  if (cin.fail() || row <= 1)
  {
    cin.clear();
    cin.ignore();
    cout << "Error: invalid entry" << endl;
    genGrid();
    return;
  }
  else
  {
    cout << "Enter the number of columns in the grid" << endl;
    cin >> col;
    if (cin.fail() || col <= 1)
    {
      cin.clear();
      cin.ignore();
      cout << "Error: invalid entry" << endl;
      genGrid();
      return;
    }
    else
    {
      cout << "Enter the desired population density (greater than 0 and less than or equal to 1)" << endl;
      cin >> dens;
      if (cin.fail() || dens <= 0 || dens > 1)
      {
        cin.clear();
        cin.ignore();
        cout << "Error: invalid entry" << endl;
        genGrid();
        return;
      }
      else
      {
        createGrid(); // generates grid
      }
    }
  }
  return;
}

// Generates grid
void GameDriver::createGrid()
{
  grid = "";
  srand(time(0));
  // population size based on density
  int numAlive = round(dens * (row * col));
  while (grid.length() < (row * col))
  {
    int nextCell = rand() % 2 + 1;
    if (nextCell == 1)
    {
      if (numAlive == 0)
      {
        continue;
      }
      else
      {
        grid = grid + "X";
        --numAlive;
      }
    }
    else if (nextCell = 2)
    {
      if (((row * col) - grid.length()) <= numAlive)
      {
        grid = grid + "X";
        --numAlive;
      }
      else
      {
        grid = grid + "-";
      }
    }
  }
}

// Gets mode and method of output from user
void GameDriver::getMode()
{
  cout << "Enter desired mode (C for Classic, D for Doughnut, M for Mirror)" << endl;
  cin >> mode;
  if (cin.fail() || (tolower(mode) != 'c' && tolower(mode) != 'd' && tolower(mode) != 'm'))
  {
    cin.clear();
    cin.ignore();
    cerr << "Error: invalid input" << endl;
    getMode();
    return;
  }
  else
  {
    cout << "Would you like a pause between generations, to press enter before each generation, or to output to a file? (P for pause, E for enter between generations, O to output to a file)" << endl;
    cin >> output;
    if (output.length() == 1)
    {
      if (tolower(output[0]) == 'e')
      {
        output = "e";
      }
      else if (tolower(output[0]) == 'p')
      {
        output = "p";
      }
      else if (tolower(output[0]) == 'o')
      {
        cout << "Enter output file path" << endl;
        cin >> output;
        ofstream file;
        file.open(output);
        if (file.is_open())
        {
          file.close();
        }
        else
        {
          cerr << "Error: given file path is not valid" << endl;
          getMode();
          return;
        }
        oF = true;
      }
      else
      {
        cerr << "Error: invalid input" << endl;
      }
    }
    else
    {
      cerr << "Error: invalid input" << endl;
      getMode();
      return;
    }
  }
}
