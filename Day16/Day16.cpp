#include "../PuzzleSolverUtil.h"

struct Rule
{
  int firstMin;
  int firstMax;
  int secondMin;
  int secondMax;
};

class PuzzleSolverOne
{
  private:  
    constexpr static int day = 16;
    constexpr static int part = 1;
    constexpr static int answer = 26941;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int rulesInputEnd = 20;
    constexpr static int myTicketLine = 22;
    constexpr static int ticketInputBegin = 25;    
    constexpr static auto fieldDelimiter = ",";
    vector<string> ruleLines;
    vector<string> ticketLines;
    int min = INT_MAX;
    int max = 0;

    bool isValueInvalid(int value)
    {
      return value < this->min || value > this->max;
    }

    void findValidFieldRanges()
    {
      for(string ruleLine : this->ruleLines)
      {
        const regex regexContained("((\\w*)-(\\w*) or (\\w*)-(\\w*))");
        smatch matchContained;      
        std::regex_search(ruleLine, matchContained, regexContained);

        this->rules.emplace_back(std::stoi(matchContained.str(2)),
                            std::stoi(matchContained.str(3)),
                            std::stoi(matchContained.str(4)),
                            std::stoi(matchContained.str(5)));

        if(this->rules.back().firstMin < this->min)
        {
          this->min = this->rules.back().firstMin;
        } 
        if(this->rules.back().secondMax > this->max) 
        {
          this->max = this->rules.back().secondMax;
        } 
      
        this->fields.emplace_back(vector<int>());
        this->rulesByFieldPoss.emplace_back(vector<int>());        
        this->rulesByFieldSol.emplace_back(0);
      }
    }
    
    int checkValidity()
    {
      int invalidValues = 0;

      for(string ticketLine : this->ticketLines)
      {
        vector<string> ticketValuesString = util::split(ticketLine, string(this->fieldDelimiter));
        
        vector<int> ticketValues;        
        std::transform(ticketValuesString.begin(), ticketValuesString.end(), 
                        std::back_inserter(ticketValues), [](auto &value){return std::stoi(value);});

        vector<int> tickValuesValidity;
        std::transform(ticketValues.begin(), ticketValues.end(), 
                     std::back_inserter(tickValuesValidity), [this](auto &value){return isValueInvalid(value) ? value : 0;});
        
        int currInvalidSum = std::accumulate(tickValuesValidity.begin(), tickValuesValidity.end(), 0);
        if(currInvalidSum != 0)
        {
          invalidValues += currInvalidSum;
        }
        else
        {
          for(int i = 0; i < ticketValues.size(); i++)
          {
            this->fields[i].push_back(ticketValues[i]);
          }
        }
      }

      return invalidValues;
    }

    public:
      vector<string> input;
      vector<Rule> rules;
      vector<vector<int>> fields;
      vector<string> myTicket;
      vector<vector<int>> rulesByFieldPoss;
      vector<int> rulesByFieldSol;

      PuzzleSolverOne()
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();
      }           

      void solve()
      {
        this->input = util::loadInputStringLines(string(this->inputFileName));
        this->ruleLines = {this->input.begin(), this->input.begin() + this->rulesInputEnd};
        this->ticketLines = {this->input.begin() + this->ticketInputBegin, this->input.end()};
        this->ticketLines.push_back(this->input[this->myTicketLine]);
        this->myTicket = util::split(this->input[this->myTicketLine], string(this->fieldDelimiter));
        findValidFieldRanges();
        int errorRate = checkValidity();
        util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, errorRate);   
      }
};

class PuzzleSolverTwo
{
  private:  
    constexpr static int day = 16;
    constexpr static int part = 2;
    constexpr static ll answer = 634796407951;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int departureFields = 6;
    PuzzleSolverOne solverOne; 
    vector<Rule> rules;
    vector<vector<int>> fields;
    vector<string> myTicket;
    vector<vector<int>> rulesByFieldPoss;
    vector<int> rulesByFieldSol;

    void ruleToFieldValidity()
    {
      for(int ruleIndex = 0; ruleIndex < this->rules.size(); ruleIndex++)
      {
        for(int fieldIndex = 0; fieldIndex < this->fields.size(); fieldIndex++)
        {
          for(int valueIndex = 0; valueIndex < this->fields[fieldIndex].size(); valueIndex++)
          {
            if(this->fields[fieldIndex][valueIndex] > this->rules[ruleIndex].firstMax 
            && this->fields[fieldIndex][valueIndex] < this->rules[ruleIndex].secondMin)
            {
              break;
            }

            if(valueIndex == this->fields[fieldIndex].size() - 1)
            {
              this->rulesByFieldPoss[fieldIndex].push_back(ruleIndex);
            } 
          }      
        }    
      }
    }

    void possRemove(int rule)
    {
      for(auto &p : this->rulesByFieldPoss)
      {
        if(p.size() > 0)
        {
          p.erase(std::remove(p.begin(), p.end(), rule), p.end());
        }
      }
    }

    void narrowDownRules()
    {
      int rulesFound = 0;
      while(rulesFound < this->rules.size())
      {
        for(int i = 0; i < this->rulesByFieldPoss.size(); i++)
        {
          if(this->rulesByFieldPoss[i].size() == 1)
          {
            rulesFound++;
            this->rulesByFieldSol[i] = this->rulesByFieldPoss[i][0];
            possRemove(this->rulesByFieldPoss[i][0]);
          }
        }
      }  
    }

    ll makeSolution()
    {
      ll mult = 1;

      for(int i = 0; i < this->rulesByFieldSol.size(); i++)
      {
        if(this->rulesByFieldSol[i] < this->departureFields)
        {
          mult *= std::stoi(this->myTicket[i]);
        }
      }

      return mult;
    }

    public:

      PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();
        this->rules = solverOne.rules;
        this->fields = solverOne.fields;
        this->myTicket = solverOne.myTicket;
        this->rulesByFieldPoss = solverOne.rulesByFieldPoss;
        this->rulesByFieldSol = solverOne.rulesByFieldSol;
      }    

      void solve()
      {
        ruleToFieldValidity();
        narrowDownRules();
        ll answerAttempt = makeSolution();
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