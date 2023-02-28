#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:
    constexpr static int day = 9;
    constexpr static int part = 1;
    constexpr static ll answer = 530627549;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int preambleSize = 25;   
    deque<ll> twentyFive;

    bool findSum(int currentNumber)
    {
      bool foundOtherPart = false;
      
      for(ll sumPart : this->twentyFive)
      {
        if(std::find(this->twentyFive.begin(), this->twentyFive.end(), this->inputs[currentNumber] - sumPart) != this->twentyFive.end())
        {
          foundOtherPart = true;
          break;
        }          
      }
      
      return foundOtherPart;
    }

    ll parseNumbers()
    {
      int i = this->preambleSize;
      for(; i < this->inputs.size(); i++)
      {  
        if(!findSum(i)) break;

        this->twentyFive.pop_front();
        this->twentyFive.push_back(this->inputs[i]);
      }
      return this->inputs[i];
    }

  public:       
     vector<ll> inputs;
     ll invalidNumber;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->inputs = util::loadInputLLLines(string(this->inputFileName));
      std::copy(this->inputs.begin(), this->inputs.begin() + this->preambleSize, back_inserter(this->twentyFive));
      invalidNumber = parseNumbers();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, invalidNumber);        
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 9;
    constexpr static int part = 2;
    constexpr static ll answer = 77730285;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;

    ll getEncryptionWeakness(int first, int last)
    {
      const auto begin = this->solverOne.inputs.begin() + first;
      const auto end = this->solverOne.inputs.begin() + last;
      ll min = *std::min_element(begin, end);
      ll max = *std::max_element(begin, end);

      return min + max;
    }

    void sumNumbersUntilResult(int &first, int &last)
    {
      ll sum = 0;
      
      while(sum != this->solverOne.invalidNumber)
      {
        if(sum < this->solverOne.invalidNumber)
        {
          sum += this->solverOne.inputs[last];
          last++;
        }
        else
        {
          sum -= this->solverOne.inputs[first];
          first++;
        }    
      }
    }

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      int first = 0;
      int last = 0;  
      sumNumbersUntilResult(first, last);
      ll encryptionWeakness = getEncryptionWeakness(first, last);
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, encryptionWeakness);      
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