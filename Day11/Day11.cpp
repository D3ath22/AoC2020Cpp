#include "../PuzzleSolverUtil.h"

struct SeatPosition
{
  int row;
  int col;
};

class PuzzleSolverOne
{
  private:
    constexpr static int day = 11;
    constexpr static int part = 1;
    constexpr static int answer = 2273;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static char emptySeat = 'L';
    constexpr static char occupiedSeat = '#';
    constexpr static char noSeat = '.';
    const vector<SeatPosition> surroundTransforms = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    int rows;
    int cols; 

    bool isValid(int row, int col)
    {
      return row > -1 && row < rows && col > -1 && col < cols;
    }

    int surroundingOccupied(int i, int j, vector<string> &currSeats)
    {
      int occupiedSeats = 0;

      for(const auto &transform : this->surroundTransforms)
      {
        bool foundEnd = false;
        int multiplier = 1;
        do
        {
          int row = i + multiplier * transform.row;
          int col = j + multiplier * transform.col;
          bool valid = isValid(row, col);
          occupiedSeats += (valid && currSeats[row][col] == this->occupiedSeat);
          foundEnd = this->adjacent || !valid || (valid && currSeats[row][col] != this->noSeat);
          multiplier++;
        } while(!foundEnd);
      }

      return occupiedSeats;
    }

    int changeSeatState(char currentSeatState, vector<string> &currSeats, vector<string> &nextSeats)
    {
      int changes = 0;

      for(int i = 0; i < this->rows; i++)
      {
        for(int j = 0; j < this->cols; j++)
        {
          if(currSeats[i][j] != currentSeatState) continue;
 
          int occupiedSeats = surroundingOccupied(i, j, currSeats);
    
          if(currentSeatState == this->emptySeat && occupiedSeats == 0)
          {
            nextSeats[i][j] = this->occupiedSeat;
            changes++;
          } 
          else if(currentSeatState == this->occupiedSeat && occupiedSeats > this->emptyCondition)
          {
            nextSeats[i][j] = this->emptySeat;
            changes++;
          }       
        }
      } 

      currSeats = nextSeats;

      return changes;
    }

    int simulateSitting()
    {
      int occupied = 0;     
      int iteration = 1;
      int changes = 0;

      do
      {
        if(iteration % 2 != 0)
        {
          changes = changeSeatState(this->emptySeat, this->seats, this->nextSeats); 
          occupied += changes;
        }
        else
        {
          changes = changeSeatState(this->occupiedSeat, this->nextSeats, this->seats);
          occupied -= changes;
        }

        iteration++;

      } while(changes != 0);

      return occupied;
    }

  public:  
    vector<string> input; 
    vector<string> seats;
    vector<string> nextSeats;
    bool adjacent;
    int emptyCondition;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve(bool adjacent = 1, int emptyCondition = 3, int answer = 2273)
    { 
      this->input = util::loadInputStringLines(string(this->inputFileName));
      this->seats = this->input;
      this->nextSeats = this->input;
      this->rows = this->input.size();
      this->cols = this->input[0].size();
      this->adjacent = adjacent;
      this->emptyCondition = emptyCondition;
      int occupied = simulateSitting();
      util::assertAnswer(this->day, this->part, this->execTimeStart, answer, occupied);        
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 11;
    constexpr static int part = 2;
    constexpr static int answer = 2064;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->solverOne.solve(0, 4, this->answer);
    }
};

int main() 
{ 
  try
  {
    PuzzleSolverOne solverOne;
    solverOne.solve();

    PuzzleSolverTwo solverTwo(solverOne);
    solverTwo.solve();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }  
}