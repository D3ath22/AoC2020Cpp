#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:  
    constexpr static int day = 15;
    constexpr static int part = 1;
    constexpr static int answer = 492;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int lastTurn = 2020;
    unordered_map<int,int> latestTurn;
    int turn;
    int latestNumber;

    void presetTurns()
    {
      for(int starterNumber : this->input)
      {
        latestTurn[starterNumber] = this->turn;
        this->turn++;
      }  
      this->latestNumber = this->input.back();
    }

    public:
      const vector<int> input = {1,20,8,12,0,14};

      int play(int lastTurn)
      {
        while(this->turn < lastTurn)
        {
          int previousTurn = this->turn - 1;

          if(this->latestTurn.find(this->latestNumber) == this->latestTurn.end())
          {
            this->latestTurn[this->latestNumber] = previousTurn;
            this->latestNumber = 0;      
          }
          else
          {
            int oldNumber = this->latestNumber;
            this->latestNumber = previousTurn - this->latestTurn[this->latestNumber];
            this->latestTurn[oldNumber] = previousTurn;
          }

          this->turn++; 
        }
        return this->latestNumber;
      }

      PuzzleSolverOne()
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();
        this->turn = 1;
      }           

      void solve()
      {
        presetTurns();          
        play(this->lastTurn + 1);       
        util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->latestNumber);   
      }
};

class PuzzleSolverTwo
{
  private:  
    constexpr static int day = 15;
    constexpr static int part = 2;
    constexpr static int answer = 63644;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int lastTurn = 30000000;
    PuzzleSolverOne solverOne; 

    public:

      PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();
      }    

      void solve()
      {
        int latestNumber = this->solverOne.play(this->lastTurn + 1);
        util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, latestNumber);   
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