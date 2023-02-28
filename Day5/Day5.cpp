#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:
    constexpr static int day = 5;
    constexpr static int part = 1;
    constexpr static int answer = 994;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int rowIndexMax = 127;
    constexpr static int rowChars = 7;
    constexpr static char rowLowerHalf = 'F';
    constexpr static int rowMultiplier = 8;
    constexpr static int colIndexMax = 7;
    constexpr static int colChars = 3;
    constexpr static char colLowerHalf = 'L';
    vector<string> inputLines; 

    int parseSeatData(string seatData, const int indexMax, const char lowerHalf)
    {
      int min = 0;
      int max = indexMax;
      int exponent = seatData.size() - 1;

      for(int i = 0; i < seatData.size(); i++)
      {
        int half = std::pow(2, exponent - i);

        if(seatData[i] == lowerHalf)
        {
          max -= half;
        } 
        else
        {
          min += half;
        } 
      }

      return min;
    }

    int makeSeatID(int row, int col)
    {
      return row * this->rowMultiplier + col;
    }

    int parseSeats()
    {
      int maxSeat = 0;

      for(string seatData : this->inputLines) 
      {
        int row = parseSeatData(seatData.substr(0, this->rowChars),this->rowIndexMax,this->rowLowerHalf);
        int col = parseSeatData(seatData.substr(this->rowChars),this->colIndexMax,this->colLowerHalf);
        int seat = makeSeatID(row, col);        
        this->seats.push_back(seat);
        maxSeat = std::max(maxSeat, seat);
      }

      return maxSeat;
    }

  public:       
    vector<int> seats;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->inputLines = util::loadInputStringLines(string(this->inputFileName));
      int maxSeat = parseSeats();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, maxSeat);        
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 5;
    constexpr static int part = 2;
    constexpr static int answer = 741;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;

    int findMissingSeat()
    {
      std::sort(this->solverOne.seats.begin(), this->solverOne.seats.end());
      
      for(int i = 1; i < this->solverOne.seats.size() - 1; i++)
      {
        int expectedSeat = this->solverOne.seats[i - 1] + 1;
        if(this->solverOne.seats[i] != expectedSeat)
        {
          return expectedSeat;
        } 
      }

      return -1;
    }
 
  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      int missingSeat = findMissingSeat();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, missingSeat);         
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