#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:
    constexpr static int day = 10;
    constexpr static int part = 1;
    constexpr static int answer = 2376;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    int makeChain()
    {
      int diffOne = 0;
      int diffThree = 1;   

      for(int i = 1; i < this->adapters.size(); i++)
      {
        int difference = this->adapters[i] - this->adapters[i - 1];

        if(difference == 1)
        {
          diffOne++;
        }
        else if(difference == 3)
        {
          diffThree++;
        }
      } 

      return diffOne * diffThree;
    }

  public:  
    vector<int> adapters;     

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->adapters = util::loadInputIntLines(string(this->inputFileName));
      this->adapters.push_back(0);  
      std::sort(this->adapters.begin(), this->adapters.end());
      int answerAttempt = makeChain();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, answerAttempt);      
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 10;
    constexpr static int part = 2;
    constexpr static ll answer = 129586085429248;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;
    vector<int> adapters;

    ll calculateArrangements()
    {
      ll arrangements = 1;
      int diffOne = 0;
      int multiplier = 1;

      for(int i = 1; i < this->adapters.size(); i++)
      {
        int difference = this->adapters[i] - this->adapters[i - 1];

        if(difference == 1)
        {
          multiplier += diffOne;
          diffOne++;
        }
        else if(difference == 3) //no other option, so add previous arrangements and reset
        {
          arrangements *= multiplier;
          diffOne = 0;
          multiplier = 1;
        }
      } 
      return arrangements;
    }

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
      this->adapters = solverOne.adapters;
    }    

    void solve()
    {      
      this->adapters.push_back(this->adapters.back() + 3); 
      ll arrangements = calculateArrangements();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, arrangements);    
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