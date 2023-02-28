#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:  
    constexpr static int day = 1;
    constexpr static int part = 1;
    constexpr static int answer = 1018336;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    int minExpense = INT_MAX;
    int maxExpense = 0;

    int parseExpenses()
    {
      int answerAttempt = -1;

      for(int expense : this->expenses)
      {
        answerAttempt = sumToGoal(expense);
        if(answerAttempt != -1) break;
      }

      return answerAttempt;
    }    

    int sumToGoal(int currentExpense)
    {
      int found = -1;
      int matchingExpense = this->goal - currentExpense;

      this->minExpense = std::min(this->minExpense, currentExpense);
      this->maxExpense = std::max(this->maxExpense, currentExpense);

      if(matchingExpense >= this->minExpense && matchingExpense <= this->maxExpense)
      {
        auto matchingExpensePos = std::find(this->expenses.begin(), this->expenses.end(), matchingExpense);
        if(matchingExpensePos != this->expenses.end())
        {
          found = makeAnswer({currentExpense, matchingExpense});
        } 
      }     

      return found;  
    }

  public:
    constexpr static int goal = 2020;
    vector<int> expenses;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }

    int makeAnswer(vector<int> foundExpenses)
    {
      int answerAttempt = 1;

      for(auto expense : foundExpenses)
      {
        answerAttempt *= expense;
      }
      
      return answerAttempt;
    }

    void solve()
    {      
      this->expenses = util::loadInputIntLines(string(this->inputFileName));
      int answerAttempt = parseExpenses();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, answerAttempt);
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 1;
    constexpr static int part = 2;
    constexpr static int answer = 288756720;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;    
    vector<int> expenses;

    vector<int> findSumThree()
    {
      auto minExpensePos = std::min_element(this->expenses.begin(), this->expenses.end());
      auto maxExpensePos = std::max_element(this->expenses.begin(), this->expenses.end());
      int minSumTwo = this->solverOne.goal - *maxExpensePos;
      int maxSumTwo = this->solverOne.goal - *minExpensePos;
      int numberOfExpenses = this->expenses.size();

      for (int i = 0; i < numberOfExpenses - 1; i++)
      {
        for (int j = i + 1; j < numberOfExpenses; j++)
        {
          int sumTwo = this->expenses[i] + this->expenses[j];
          
          if(sumTwo <= maxSumTwo && sumTwo >= minSumTwo)
          {
            for (int k = 0; k < numberOfExpenses && k != i && k != j; k++)
            {
              if(sumTwo + this->expenses[k] == this->solverOne.goal)
              {
                return {this->expenses[i], this->expenses[j], this->expenses[k]};
              }
            }
          }
        }
      }
      return {0,0,0};   
    }    

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
      this->expenses = solverOne.expenses;
    }

    void solve()
    {
      int answerAttempt = solverOne.makeAnswer(findSumThree());
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, answerAttempt);
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