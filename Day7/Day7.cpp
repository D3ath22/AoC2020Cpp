#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:
    constexpr static int day = 7;
    constexpr static int part = 1;
    constexpr static int answer = 155;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    vector<string> inputLines;
    unordered_map<string, vector<string>> rulesReverse;
    unordered_set<string> allContainers;

    void parseContainedRegex(string rule, string container)
    {
      const regex regexContained("([1-9].\\w*.\\w*)");
      smatch matchContained;      
      while(std::regex_search(rule, matchContained, regexContained))
      {
        const int bagSize = std::stoi(matchContained.str(0).substr(0, 1));
        const string contained = matchContained.str(0).substr(2);

        saveRuleContainedMap(contained, container);          
        saveRuleContainerMap(container, contained, bagSize);

        rule = matchContained.suffix();         
      }
    }

    void findContainer(string bag)
    {
      if(this->rulesReverse.find(bag) != this->rulesReverse.end())
      {
        for(string containerBag : this->rulesReverse[bag])
        {
          auto insertion = this->allContainers.insert(containerBag);
          if(insertion.second)
          {
            findContainer(containerBag);    
          }   
        }
      }
    }

    void saveRuleContainedMap(string contained, string container)
    {
      if(this->rulesReverse.find(contained) == rulesReverse.end())
      {
        this->rulesReverse.emplace(contained, vector<string>());
      } 
      this->rulesReverse[contained].push_back(container);
    }

    void saveRuleContainerMap(string container, string contained, int bagSize)
    {
      if(this->rules.find(container) == this->rules.end())
      {
        this->rules.emplace(container, vector<pair<int,string>>());
      }        
      this->rules[container].emplace_back(bagSize,contained);   
    } 

    void parseRules()
    {
      for(string rule : this->inputLines) 
      {  
        if(rule.find(this->endRule) != std::string::npos) continue;

        int containerEndPos = rule.find(this->bagsString) - 1;
        string container = rule.substr(0, containerEndPos);

        parseContainedRegex(rule, container);  
      } 
    }

  public:    
    constexpr static auto myBag = "shiny gold";
    constexpr static auto endRule = "no other bags";
    constexpr static auto bagString = "bag";
    constexpr static auto bagsString = "bags";  
    constexpr static int containedBagsDist = 16;
    constexpr static int nextBagDist = 7;
    constexpr static int bagSizeDist = 2;

    unordered_map<string, vector<pair<int,string>>> rules;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->inputLines = util::loadInputStringLines(string(this->inputFileName));
      parseRules();   
      findContainer(string(this->myBag));
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->allContainers.size());     
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 7;
    constexpr static int part = 2;
    constexpr static int answer = 54803;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;

    int findContainedBags(string bag)
    {
      unordered_map<string, vector<pair<int,string>>> rules = this->solverOne.rules;
      int containedBags = 0;

      if(rules.find(bag) != rules.end())
      {
         for(auto contained: rules[bag])
         {
           containedBags += contained.first * (findContainedBags(contained.second) + 1); 
         }          
      }      

      return containedBags;
    }

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    { 
      int containedBags = findContainedBags(string(this->solverOne.myBag));
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, containedBags);        
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