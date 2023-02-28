#include "../PuzzleSolverUtil.h"

struct PasswordData
{
  int min;
  int max;
  char letter;
  string password;
};

class PuzzleSolverOne
{
  private:  
    constexpr static int day = 2;
    constexpr static int part = 1;
    constexpr static int answer = 439;
    constexpr static auto inputFileName = "input.txt";    
    std::chrono::high_resolution_clock::time_point execTimeStart;

    static bool isPwdValidPolicyOne(const PasswordData &pwd)
    {
      int letterCount = std::count(pwd.password.begin(), pwd.password.end(), pwd.letter);
      return letterCount >= pwd.min && letterCount <= pwd.max;
    }

  public:
    vector<string> inputLines;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }

    int parsePasswords(const function<bool (PasswordData)> &isPassValidPolicy)
    {
      const static auto minMaxMidDelimiter = "-";
      const static auto minMaxEndDelimiter = " ";

      int validPasswords = 0;

      for(string pwdLine : this->inputLines)
      {        
        auto minMaxMidDelimiterPos = pwdLine.find(string(minMaxMidDelimiter));
        auto minMaxEndDelimiterPos = pwdLine.find(string(minMaxEndDelimiter));

        PasswordData pwd{std::stoi(pwdLine.substr(0, minMaxMidDelimiterPos)),
                          std::stoi(pwdLine.substr(minMaxMidDelimiterPos + 1, minMaxEndDelimiterPos - (minMaxMidDelimiterPos + 1))),
                          pwdLine[minMaxEndDelimiterPos + 1],
                          pwdLine.substr(minMaxEndDelimiterPos + 4, pwdLine.length() - (minMaxEndDelimiterPos + 4))};  
                
        if(isPassValidPolicy(pwd))
        {
          validPasswords++;
        } 
      }

      return validPasswords;
    }

    void solve()
    {
      this->inputLines = util::loadInputStringLines(string(this->inputFileName));
      int validPasswords = parsePasswords(isPwdValidPolicyOne);
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, validPasswords);
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 2;
    constexpr static int part = 2;
    constexpr static int answer = 584;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne;

    static bool isPwdValidPolicyTwo(const PasswordData &pwd)
    {
      return ((pwd.password[pwd.min - 1] == pwd.letter) + (pwd.password[pwd.max - 1] == pwd.letter)) == 1;
    }
    
  public:

    PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }  

    void solve()
    {
      int validPasswords = this->solverOne.parsePasswords(isPwdValidPolicyTwo);
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, validPasswords);
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