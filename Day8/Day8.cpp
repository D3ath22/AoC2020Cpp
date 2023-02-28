#include "../PuzzleSolverUtil.h"

struct Instruction
{
  string operation;
  int argument;
  bool visited;
};

class PuzzleSolverOne
{
  private:
    constexpr static int day = 8;
    constexpr static int part = 1;
    constexpr static int answer = 1928;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int opSize = 3;
    constexpr static auto accOp = "acc";
    constexpr static auto jmpOp = "jmp";
    vector<string> inputLines;
    int accumulator = 0;
    int line = 0;

    vector<Instruction> loadInstructions()
    {
      vector<Instruction> instructions;

      for(string instructionLine : this->inputLines)
      {
        string operation = instructionLine.substr(0, this->opSize);
        int argument = std::stoi(instructionLine.substr(this->opSize + 1, instructionLine.size() - (this->opSize + 1)));
        instructions.emplace_back(operation, argument, 0);
      }

      return instructions;
    }

  public:       
    vector<Instruction> instructions;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void nextInstruction(int &line, int &accumulator)
    {
      this->instructions[line].visited = true;
      Instruction instruction = this->instructions[line];      

      if(instruction.operation == this->accOp)
      {
        accumulator += instruction.argument;
      } 
      else if(instruction.operation == this->jmpOp)
      {    
        line += instruction.argument - 1;
      }

      line++;
    }

    void parseInstructions(int &line, int &accumulator)
    {      
      int safety = 0;
      while(safety < this->instructions.size())
      {
        if(line < this->instructions.size() && !this->instructions[line].visited)
        {
          nextInstruction(line, accumulator);
        }      
        else break;

        safety++;
      }
    }

    void solve()
    {      
      this->inputLines = util::loadInputStringLines(string(this->inputFileName));
      this->instructions = loadInstructions();
      parseInstructions(this->line, this->accumulator);
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->accumulator);            
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 8;
    constexpr static int part = 2;
    constexpr static int answer = 1319;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int opSize = 3;
    constexpr static auto accOp = "acc";
    constexpr static auto jmpOp = "jmp";
    constexpr static auto nopOp = "nop";
    PuzzleSolverOne solverOne;
    vector<Instruction> instructions;
    int accumulator = 0;
    int line = 0;

    bool explorePossibleFuture(int possibleLine)
    {
      bool foundEnd = false;
      int possibleAccumulator = this->accumulator;

      this->solverOne.parseInstructions(possibleLine, possibleAccumulator);
     
      if(possibleLine == this->instructions.size())
      {
        this->accumulator = possibleAccumulator;
        foundEnd = true;
      }

      return foundEnd;
    }

    bool nextInstruction()
    {
      bool foundEnd = false;
      this->instructions[this->line].visited = true;
      Instruction instruction = this->instructions[this->line];      

      if(instruction.operation == this->accOp)
      {
        this->accumulator += instruction.argument;
      } 
      else if(instruction.operation == this->jmpOp)
      {    
        foundEnd = explorePossibleFuture(this->line + 1);

        this->line += instruction.argument - 1;
      }
      else if(instruction.operation == this->nopOp)
      {
        foundEnd = explorePossibleFuture(this->line + instruction.argument);
      }

      this->line++;

      return foundEnd;
    }

    void parseInstructions()
    {      
      int safety = 0;
      while(safety < this->instructions.size())
      {
        if(this->line < this->instructions.size() && !this->instructions[this->line].visited)
        {
          if(nextInstruction()) break;  
        }      
        else break;

        safety++;
      }
    }

    void unvisitInstructions()
    {
      for(Instruction &instruction : this->instructions)
      {
        instruction.visited = false;
      }
    }

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->instructions = this->solverOne.instructions;
      unvisitInstructions();
      parseInstructions();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->accumulator);        
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