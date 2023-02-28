#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:
    constexpr static int day = 6;
    constexpr static int part = 1;
    constexpr static int answer = 6297;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    int parseAnyoneAnswers()
    {
      int sumAnswers = 0;
      unordered_set<char> groupAnswers;

      for(string line : this->inputLines)
      {
        if(line.empty())
        {
          sumAnswers += groupAnswers.size();
          groupAnswers.clear();
          continue;
        }
  
        for(char answer : line)
        {
          groupAnswers.insert(answer);
        } 
      }

      return sumAnswers + groupAnswers.size();
    }

  public:    
    vector<string> inputLines;   

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->inputLines = util::loadInputStringLines(string(this->inputFileName));
      int anyoneAnswers = parseAnyoneAnswers();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, anyoneAnswers);        
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 6;
    constexpr static int part = 2;
    constexpr static int answer = 3158;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;
    vector<string> inputLines; 

    int groupEveryoneAnswers(const unordered_map<char, int> &answerOccurences, int groupSize)
    {
      int everyoneAnswers = 0;

      for(const auto &answerOccurence : answerOccurences)   
      {
        if(answerOccurence.second == groupSize)
        {
          everyoneAnswers++;
        } 
      }            

      return everyoneAnswers;
    }

    void insertIndividualAnswers(unordered_map<char, int> &answerOccurences, string line)
    {
      for(char answer : line)
      { 
        auto tryInsert = answerOccurences.insert(pair<char, int>(answer, 1));
        if(tryInsert.second == false)
        {
          answerOccurences[answer] += 1;
        } 
      }
    }

    int parseEveryoneAnswers()
    {
      unordered_map<char, int> answerOccurences;
      int groupSize = 0;
      int everyoneAnswers = 0;

      for(string line : this->inputLines)
      {
        if(line.empty())
        {
          everyoneAnswers += groupEveryoneAnswers(answerOccurences, groupSize);
          answerOccurences.clear();
          groupSize = 0;
        }
        else
        {
          groupSize++;
          insertIndividualAnswers(answerOccurences, line);          
        }  
      }

      return everyoneAnswers + groupEveryoneAnswers(answerOccurences, groupSize);
    }

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
      this->inputLines = solverOne.inputLines;
    }    

    void solve()
    {      
      int everyoneAnswers = parseEveryoneAnswers();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, everyoneAnswers);        
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