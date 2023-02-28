#include "../PuzzleSolverUtil.h"

class PuzzleSolverOne
{
  private:  
    constexpr static int day = 17;
    constexpr static int part = 1;
    constexpr static int answer = 223;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int cycles = 6;
    constexpr static auto activeCube = '#';
    vector<string> input;
    vector<vector<vector<bool>>> grid; // z y x
    vector<vector<vector<bool>>> nextGrid;

    int initialSize;
    int yxSize;
    int zSize;    

    int cycle;
    int beginI;
    int endIz;
    int endIyx;

    int activeCubes;

    void loadInput()
    {
      int i = 0;
      for(int y = beginI; y < endIyx; y++)
      {
        int lineI = 0;
        string line = input[i];
        for(int x = beginI; x < endIyx; x++)
        {
          grid[beginI][y][x] = (line[lineI] == activeCube);
          lineI++;
        }   
        i++;     
      }

      nextGrid = grid; 

      print(nextGrid[beginI]);    
    }

    void boot()
    {
      for(int i = 1; i < cycles + 1; i++)
      {
        grid = nextGrid;

        cycle++;
        beginI--;
        endIz++;
        endIyx++;

        std::cout << "cycle = " << i << "\n";
        nextCycle();        
      }
    }

    void nextCycle()
    {
      for(int z = beginI; z < endIz; z++)
      {
        for(int y = beginI; y < endIyx; y++)
        {
          for(int x = beginI; x < endIyx; x++)
          {
            int neighbors = activeNeighbors(z, y, x);
            if(grid[z][y][x] && neighbors != 2 && neighbors != 3) nextGrid[z][y][x] = 0;
            if(!grid[z][y][x] && neighbors == 3) nextGrid[z][y][x] = 1;
            if(cycle == cycles && nextGrid[z][y][x]) activeCubes++;
          }
        }
        std::cout << "z = " << z - cycles << "\n";
        print(nextGrid[z]);
      }
    }

    int activeNeighbors(int z, int y, int x)
    {
      int neighbors = -grid[z][y][x];

      for(int iz = max(z - 1, 0); iz < min(z + 1, zSize - 1) + 1 ; iz++)
      {
        for(int iy = max(y - 1, 0); iy < min(y + 1, yxSize - 1) + 1 ; iy++)
        {
          for(int ix = max(x - 1, 0); ix < min(x + 1, yxSize - 1) + 1 ; ix++)
          {
            neighbors += grid[iz][iy][ix];
          }
        }
      }      
      return neighbors;
    }

    public:

      
      PuzzleSolverOne()
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();    
        this->initialSize = 8;
        this->yxSize = (cycles * 2) + initialSize;
        this->zSize = (cycles * 2) + 1;
        this->grid = vector<vector<vector<bool>>>(zSize, vector<vector<bool>>(yxSize, vector<bool>(yxSize)));  
        this->cycle = 0;
        this->beginI = cycles;
        this->endIz = 1 + cycles;
        this->endIyx = cycles + initialSize;
        this->activeCubes = 0;
      }       

      void print(vector<vector<bool>> &zGrid)
      {
        std::cout << "\n";
        for(auto yRow : zGrid)
        {
          for(auto x : yRow)
          {
            std::cout << x << " ";
          }
          std::cout << "\n";
        }
        std::cout << "\n";
      }    

      void solve()
      {
        this->input = util::loadInputStringLines(string(this->inputFileName));
        loadInput();
        boot(); 
        util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->activeCubes);    
      }
};

class PuzzleSolverTwo
{
  private:  
    constexpr static int day = 17;
    constexpr static int part = 2;
    constexpr static int answer = 1884;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int cycles = 6;
    constexpr static auto activeCube = '#';
    PuzzleSolverOne solverOne; 
    vector<string> input;
    vector<vector<vector<vector<bool>>>> grid; // w z y x
    vector<vector<vector<vector<bool>>>> nextGrid;

    int initialSize;
    int yxSize;
    int zSize;    

    int cycle;
    int beginI;
    int endIz;
    int endIyx;

    int activeCubes;

    void loadInput()
    {
      int i = 0;

      for(int y = beginI; y < endIyx; y++)
      {
        string line = input[i];

        int lineI = 0;
        for(int x = beginI; x < endIyx; x++)
        {
          grid[beginI][beginI][y][x] = (line[lineI] == activeCube);
          lineI++;
        }      
        i++;  
      }

      nextGrid = grid; 

      print(nextGrid[beginI][beginI]);    
    }

    void boot()
    {
      for(int i = 1; i < cycles + 1; i++)
      {
        grid = nextGrid;

        cycle++;
        beginI--;
        endIz++;
        endIyx++;

        std::cout << "cycle = " << i << "\n";
        nextCycle();        
      }
    }

    void nextCycle()
    {
      for(int w = beginI; w < endIz; w++)
      {
        for(int z = beginI; z < endIz; z++)
        {
          for(int y = beginI; y < endIyx; y++)
          {
            for(int x = beginI; x < endIyx; x++)
            {
              int neighbors = activeNeighbors(w, z, y, x);
              if(grid[w][z][y][x] && neighbors != 2 && neighbors != 3) nextGrid[w][z][y][x] = 0;
              if(!grid[w][z][y][x] && neighbors == 3) nextGrid[w][z][y][x] = 1;
              if(cycle == cycles && nextGrid[w][z][y][x]) activeCubes++;
            }
          }
        }
      }
    }

    int activeNeighbors(int w, int z, int y, int x)
    {
      int neighbors = -grid[w][z][y][x];

      for(int iw = max(w - 1, 0); iw < min(w + 1, zSize - 1) + 1 ; iw++)
      {
        for(int iz = max(z - 1, 0); iz < min(z + 1, zSize - 1) + 1 ; iz++)
        {
          for(int iy = max(y - 1, 0); iy < min(y + 1, yxSize - 1) + 1 ; iy++)
          {
            for(int ix = max(x - 1, 0); ix < min(x + 1, yxSize - 1) + 1 ; ix++)
            {
              neighbors += grid[iw][iz][iy][ix];
            }
          }
        }  
      }    
      return neighbors;
    }

    void print(vector<vector<bool>> &zGrid)
    {
      std::cout << "\n";
      for(auto yRow : zGrid)
      {
        for(auto x : yRow)
        {
          std::cout << x << " ";
        }
        std::cout << "\n";
      }
      std::cout << "\n";
    }

    public:

      PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();
        this->initialSize = 8;
        this->yxSize = (cycles * 2) + initialSize;
        this->zSize = (cycles * 2) + 1;
        this->grid = vector<vector<vector<vector<bool>>>>(zSize ,vector<vector<vector<bool>>>(zSize, vector<vector<bool>>(yxSize, vector<bool>(yxSize))));  
        this->cycle = 0;
        this->beginI = cycles;
        this->endIz = 1 + cycles;
        this->endIyx = cycles + initialSize;
        this->activeCubes = 0;
      }    

      void solve()
      {
        this->input = util::loadInputStringLines(string(this->inputFileName));
        loadInput();
        boot(); 
        util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->activeCubes);   
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