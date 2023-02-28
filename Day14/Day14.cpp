#include "../PuzzleSolverUtil.h"

struct Instruction
{
  int mask;
  ul address;
  ul value;
};

class PuzzleSolverOne
{
  private:
    constexpr static int day = 14;
    constexpr static int part = 1;
    constexpr static ll answer = 10452688630537;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int maskStart = 7;
    constexpr static auto maskIdentifier = 'a';
    constexpr static auto addressEndChar = ']';
    constexpr static int addressStartPos = 4;
    constexpr static auto maskOneStringEmpty = "000000000000000000000000000";
    constexpr static auto maskZeroStringEmpty = "111111111111111111111111111";
    string maskOneString;
    string maskZeroString;
    vector<string> input;
    unordered_map<ll, ll> memory;    

    void setCurrentMask(string mask)
    {
      this->maskOneString = this->maskOneStringEmpty;
      this->maskZeroString = this->maskZeroStringEmpty;

      for(char c : mask)
      {
        if(c == 'X')
        {
          this->maskOneString.push_back('0');
          this->maskZeroString.push_back('1');
        }
        else if(c == '1')
        {
          this->maskOneString.push_back('1');
          this->maskZeroString.push_back('1');
        }
        else if(c == '0')
        {
          this->maskOneString.push_back('0');
          this->maskZeroString.push_back('0');
        }   
      }
    }

    void parseInstruction(string line)
    {
      int addressLen = line.find(this->addressEndChar);
      string addressString = line.substr(this->addressStartPos, addressLen);
      ul address = std::stoi(addressString);

      string numberString = line.substr(addressLen + this->addressStartPos);
      ul number = std::strtoul(numberString.c_str(), nullptr, 10);

      instructions.emplace_back(this->masks.size()-1, address, number);
    }

    void addToMemory(const Instruction &instruction)
    {
      bitset<63> maskOne{this->maskOneString};
      bitset<63> maskZero{this->maskZeroString};
      bitset<63> numberBit{instruction.value};

      numberBit |= maskOne;
      numberBit &= maskZero;

      this->memory[instruction.address] = numberBit.to_ullong();
    }

    void parseProgram()
    {
      for(string line: this->input)
      {
        if(line[1] == this->maskIdentifier)
        {
          masks.push_back(line.substr(this->maskStart));
          setCurrentMask(masks.back());
        }
        else
        {
          parseInstruction(line);
          addToMemory(instructions.back());
        }
      }    
    }

  public:      
    vector<string> masks;
    vector<Instruction> instructions;

    ll sumMemoryValues(const unordered_map<ll, ll> &memory)
    {
      ll sum = 0;
      for(auto &keyValue : memory)
      {
        sum += keyValue.second;
      }

      return sum;
    }
   
    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    { 
      this->input = util::loadInputStringLines(string(this->inputFileName));
      parseProgram();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, sumMemoryValues(this->memory));       
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 14;
    constexpr static int part = 2;
    constexpr static ll answer = 2881082759597;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int maskStart = 7;
    constexpr static int maskSize = 36;  
    constexpr static auto floatingIdentifier ='X';
    constexpr static auto maskOneStringEmpty = "000000000000000000000000000";
    constexpr static auto maskZeroStringEmpty = "111111111111111111111111111";
    PuzzleSolverOne solverOne;
    string maskOneString;
    string maskZeroString;     
    vector<ll> floatingBits;
    unordered_map<ll, ll> memory;    

    void setCurrentMask(string mask)
    {
      this->maskOneString = this->maskOneStringEmpty;
      this->maskZeroString = this->maskZeroStringEmpty;
      this->floatingBits.clear();
      int floatingPos = this->maskSize - 1;

      for(char c : mask)
      {
        if(c == this->floatingIdentifier)
        {
          this->maskOneString.push_back('0');
          this->maskZeroString.push_back('1');
          this->floatingBits.push_back(floatingPos);
        }
        else if(c == '1')
        {
          this->maskOneString.push_back('1');
          this->maskZeroString.push_back('1');
        }
        else if(c == '0')
        {
          this->maskOneString.push_back('0');
          this->maskZeroString.push_back('1');
        }

        floatingPos--;
      }   
    }

    void addToMemory(Instruction instruction)
    {
      bitset<63> maskOne{maskOneString};
      bitset<63> maskZero{maskZeroString};
      bitset<63> addressBit{instruction.address};  

      addressBit |= maskOne;
      addressBit &= maskZero;

      unordered_set<ll> addresses;
      ll addressCount = 1ull << floatingBits.size();

      //floating bits as a number, for each number to be forced
      for (ll bits = 0; bits < addressCount; bits++) 
      {
        bitset<63> newAddress(addressBit);
        //for each floating bit overlap
        //check position of floating bit in 2^i and apply
        for (ll i = 0; i < floatingBits.size(); i++) 
        {
          newAddress[floatingBits[i]] = (bits & (1u << i)) != 0;        
        }        
        addresses.insert(newAddress.to_ullong());
      }

      for(auto &address : addresses)
      {
        this->memory[address] = instruction.value;
      }
    }

    void parseProgram()
    {
      for(auto &instruction: this->solverOne.instructions)
      {
        setCurrentMask(this->solverOne.masks[instruction.mask]); 
        addToMemory(instruction);
      }    
    }

  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      parseProgram();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->solverOne.sumMemoryValues(this->memory));       
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