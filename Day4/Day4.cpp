#include "../PuzzleSolverUtil.h"

struct PassportField
{
  string name;
  string content;
};

class Passport 
{
  private:
    vector<PassportField> fields;    
  
    bool byrValidate(string content)
    {
      return std::stoi(content) >= 1920 && std::stoi(content) <= 2002;
    }

    bool iyrValidate(string content)
    {
      return std::stoi(content) >= 2010 && std::stoi(content) <= 2020;
    }

    bool eyrValidate(string content)
    {
      return std::stoi(content) >= 2020 && std::stoi(content) <= 2030;
    }

    bool hgtValidate(string content)
    {      
      if(content[content.size() - 1] == 'm')
      {
        return std::stoi(content) >= 150 && std::stoi(content) <= 193;
      }
      else return std::stoi(content) >= 59 && std::stoi(content) <= 76;      
    }

    bool hclValidate(string content)
    {
      std::regex hclRegex("^#{1}[0-9a-f]{6}$");
      return std::regex_match(content, hclRegex);
    }

    bool eclValidate(string content)
    {
      static const vector<string> eyeColors = {"amb","blu","brn","gry","grn","hzl","oth"}; 
      return std::find(eyeColors.begin(), eyeColors.end(), content) != eyeColors.end();
    }

    bool pidValidate(string content)
    {
      return content.size() == 9;
    }

    public:

      Passport(){};   

      void addField(PassportField passportField)
      {
        if(passportField.name != "cid")
        {
          this->fields.push_back(passportField);
        } 
      }

      int getFieldsSize()
      {
        return this->fields.size();
      }

      bool validate()
      {
        bool valid = true;
        for(const auto &field : this->fields)
        {
          if(field.name == "byr")      valid = valid && byrValidate(field.content);
          else if(field.name== "iyr")  valid = valid && iyrValidate(field.content);
          else if(field.name == "eyr") valid = valid && eyrValidate(field.content);
          else if(field.name == "hgt") valid = valid && hgtValidate(field.content);
          else if(field.name == "hcl") valid = valid && hclValidate(field.content);
          else if(field.name == "ecl") valid = valid && eclValidate(field.content);
          else if(field.name == "pid") valid = valid && pidValidate(field.content); 
        }
        return valid;
      }
};

class PuzzleSolverOne
{
  private:
    constexpr static int day = 4;
    constexpr static int part = 1;
    constexpr static int answer = 245;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static int fieldNameSize = 3;
    constexpr static int requiredFields = 7;
    constexpr static auto fieldDelimiter = " ";
    vector<string> inputLines;

    vector<PassportField> getFieldsInLine(string line)
    {
      vector<PassportField> newFields;

      for(string passportFieldData : util::split(line, string(this->fieldDelimiter)))
      {
        newFields.emplace_back(passportFieldData.substr(0, this->fieldNameSize),
                               passportFieldData.substr(this->fieldNameSize + 1));  
      }

      return newFields;
    }

    void addFields(Passport &newPassport, string line)
    {
      for(PassportField field : getFieldsInLine(line))
      {
        newPassport.addField(field);
      }
    }  

    void checkLatestValid()
    {
      if(this->passports.back().getFieldsSize() != this->requiredFields)
      {
        this->passports.pop_back();
      } 
    }   

    void parsePassports()
    {
      this->passports.emplace_back();

      for(string line : this->inputLines)
      {
        if(line.empty())
        {
          checkLatestValid();
          this->passports.emplace_back();
          continue;
        }
        
        addFields(this->passports.back(), line);
      }
      checkLatestValid(); 
    }

  public:
    vector<Passport> passports;

    PuzzleSolverOne()
    {
      this->execTimeStart = std::chrono::high_resolution_clock::now();
    }    

    void solve()
    {      
      this->inputLines = util::loadInputStringLines(string(this->inputFileName));
      parsePassports();
      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, this->passports.size());     
    }
};

class PuzzleSolverTwo
{
  private:
    constexpr static int day = 4;
    constexpr static int part = 2;
    constexpr static int answer = 133;
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
      int validPassports = 0;

      for(auto &passport : this->solverOne.passports)    
      {
        if(passport.validate())
        {
          validPassports++;
        } 
      }        

      util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, validPassports);        
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