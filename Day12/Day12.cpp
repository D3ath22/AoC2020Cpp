#include "../PuzzleSolverUtil.h"

struct Instruction
{
  char action;
  int value;
};

class PuzzleSolverOne
{
  private:
    constexpr static int day = 12;
    constexpr static int part = 1;
    constexpr static int answer = 1186;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    const vector<char> directions = {'N','E','S','W','R','L'};
    const vector<int> directionDegrees = {0,90,180,270};
    const vector<vector<int>> actionEffects = {{0,1,0},{0,0,1},{0,-1,0},{0,0,-1},{1,0,0},{-1,0,0}};
    const int circle = 360;
    const int turnDir = 90;   
    const char dirApply = 'F';    
    vector<string> input;    
    vector<int> position; //int directionDegrees; int positionUp; int positionRight;

    void applyActionToPosition(Instruction instruction)
    {
      int directionIndex = 0;
      if(instruction.action == this->dirApply)
      {
        directionIndex = std::find(this->directionDegrees.begin(), this->directionDegrees.end(), this->position[0])
                         - this->directionDegrees.begin();
      }
      else 
      {
        directionIndex = std::find(this->directions.begin(), this->directions.end(), instruction.action)
                         - this->directions.begin();
      }

      vector<int> actionEffect = this->actionEffects[directionIndex];
      std::transform(actionEffect.begin(), actionEffect.end(), 
                     actionEffect.begin(), [&instruction](auto& positionEffect){return positionEffect * instruction.value;});
      std::transform(this->position.begin(), this->position.end(),
                     actionEffect.begin(), this->position.begin(), std::plus<int>());

      if(this->position[0] < 0)
      {
        this->position[0] += this->circle;
      } 
      position[0] %= this->circle;
    }

    void navigate()
    {
      for(const Instruction &instruction : this->instructions)
      {
        applyActionToPosition(instruction); 
      }
    }

    int manhattanDistance()
    {
      return std::abs(this->position[1]) + std::abs(this->position[2]);
    }

  public:  
    vector<Instruction> instructions;   

    vector<Instruction> loadInstructions()
    {
      vector<Instruction> instructions;

      for(string inputLine : this->input)
      {
        instructions.emplace_back(inputLine[0], std::stoi(inputLine.substr(1)));
      }

      return instructions;
    }
   
    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
      this->position = {this->directionDegrees[1], 0, 0};
    }    

    void solve()
    { 
      this->input = util::loadInputStringLines(string(this->inputFileName));
      this->instructions = loadInstructions();
      navigate();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, manhattanDistance());       
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 12;
    constexpr static int part = 2;
    constexpr static int answer = 47806;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne; 
    
    const vector<char> directions = {'N','E','S','W'};
    const vector<vector<int>> actionEffects = {{1,0},{0,1},{-1,0},{0,-1}};
    const vector<int> waypointStart = {1, 10};
    const char dirApply = 'F';    
    const char dirRight = 'R';  
    const char dirLeft = 'L';     
    vector<Instruction> instructions;
    //int positionUp; int positionRight;   
    vector<int> position = {1, 10};
    vector<int> shipPosition = {0, 0};
       
    void applyActionToPosition(Instruction instruction)
    {
      if(instruction.action == this->dirApply)
      {
        this->shipPosition[0] += (instruction.value * (this->position[0]));
        this->shipPosition[1] += (instruction.value * (this->position[1]));
      }
      else if(instruction.action == this->dirRight || instruction.action == this->dirLeft)
      {
        vector<int> newPosition = {0, 0};
        int degree = instruction.value;

        while(degree > 0)
        { 
          if(instruction.action == this->dirRight)
          {
            newPosition[0] = -this->position[1];
            newPosition[1] = this->position[0];       
          }
          else
          {
            newPosition[0] = this->position[1];
            newPosition[1] = -this->position[0];
          }            
          this->position = newPosition; 
          degree -= 90;       
        }
      }
      else
      {      
        int directionIndex = std::find(this->directions.begin(), this->directions.end(), instruction.action)
                             - this->directions.begin();
        vector<int> actionEffect = this->actionEffects[directionIndex];
        std::transform(actionEffect.begin(), actionEffect.end(),
                       actionEffect.begin(), [&instruction](auto& positionEffect){return positionEffect * instruction.value;});
        std::transform(this->position.begin(), this->position.end(),
                       actionEffect.begin(), this->position.begin(), std::plus<int>());
      }
    }

    void navigate()
    {
      for(const Instruction &instruction : this->instructions)
      {
        applyActionToPosition(instruction); 
      }
    }

    int manhattanDistance()
    {
      return std::abs(this->shipPosition[0]) + std::abs(this->shipPosition[1]);
    }

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->position = {1, 10};
      this->shipPosition = {0, 0};
      this->instructions = this->solverOne.instructions;
      navigate();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, manhattanDistance());       
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