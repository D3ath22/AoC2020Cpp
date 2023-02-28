#include "../PuzzleSolverUtil.h"

struct Slope
{
  int xInc;
  int yInc;
  
  bool operator==(const Slope &other) const 
  {
      return this->xInc == other.xInc && this->yInc == other.yInc;
  }
};

class PuzzleSolverOne
{
  private:
    constexpr static int day = 3;
    constexpr static int part = 1;
    constexpr static int answer = 247;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static auto tree = '#';    
    
  public:
    vector<string> map;
    int trees;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }

    int countTreesbySlope(const Slope &slope)
    {
      int slopeTrees = 0;
      
      const int rowSize = this->map.size();
      const int columnSize = this->map[0].size();

      int x = slope.xInc;      
      for(int y = slope.yInc; y < rowSize; y += slope.yInc)
      {
        if(this->map[y][x] == this->tree)
        {
          slopeTrees++;
        } 

        x = (x + slope.xInc) % columnSize;
      }

      return slopeTrees;
    }

    void solve()
    {
      this->map = util::loadInputStringLines(string(this->inputFileName));
      this->trees = countTreesbySlope(Slope{3, 1});
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->trees);     
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 3;
    constexpr static int part = 2;
    constexpr static ll answer = 2983070376;
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
      ll treesMult = solverOne.trees;
      static const vector<Slope> slopes = {{1,1},{5,1},{7,1},{1,2}};      

      for(Slope slope : slopes)
      {
        treesMult *= solverOne.countTreesbySlope(slope);
      }        
 
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, treesMult);
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