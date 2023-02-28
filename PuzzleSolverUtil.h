#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <cmath>
#include <math.h>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <queue>
#include <bitset>
#include <exception>
#include <functional>
#include <compare>
#include <stack>
#include <chrono>
#include <cassert>

using namespace std;

using ul = unsigned long int;
using ll = long long;

namespace util
{
    template<typename T>
    void printVector(vector<T> vector)
    {
      for (auto i: vector) 
      {
        cout << i << "\n";
      }
    }

    ifstream createStream(string inputFileName)
    {
      ifstream inputStream(inputFileName);
      if(!inputStream.good()) throw std::runtime_error("Puzzle Input File Not Found: " + string(inputFileName));
      return inputStream;
    }

    vector<int> loadInputIntLines(string inputFileName)
    {
      vector<int> inputs;

      ifstream inputStream = util::createStream(inputFileName);
      string line;

      while(std::getline(inputStream, line)) 
      {
        inputs.push_back(std::stoi(line));
      }

      inputStream.close();

      return inputs;
    }  

    vector<ll> loadInputLLLines(string inputFileName)
    {
      vector<ll> inputs;

      ifstream inputStream = util::createStream(inputFileName);
      string line;

      while(std::getline(inputStream, line)) 
      {
        inputs.push_back(std::stoull(line));
      }

      inputStream.close();

      return inputs;
    }  

    vector<string> loadInputStringLines(string inputFileName)
    {
      vector<string> inputs;

      ifstream inputStream = util::createStream(inputFileName);
      string line;

      while(std::getline(inputStream, line)) 
        inputs.push_back(line); 

      inputStream.close();

      return inputs;
    }  

    vector<int> findAll(string line, string delimiter)
    {
      vector<int> delimiterPositions;

      int delimiterPos = line.find(delimiter); 
      while(delimiterPos != std::string::npos)
      {
        delimiterPositions.push_back(delimiterPos); 
        delimiterPos = line.find(delimiter, delimiterPos + 1);         
      }

      return delimiterPositions;
    }

    vector<string> split(const string line, string delimiter)
    {
      std::regex regexDelimiter{delimiter};
      std::sregex_token_iterator first(line.begin(), line.end(), regexDelimiter, -1), last;

      return {first, last};      
    }   

    void assertAnswer(int day, int part, std::chrono::high_resolution_clock::time_point execTimeStart, size_t answer, size_t attempt)
    {
      auto execTimeEnd = std::chrono::high_resolution_clock::now();
      auto execTime =  std::chrono::duration_cast<std::chrono::milliseconds>(execTimeEnd - execTimeStart);

      std::cout << "D" << day << "P" << part << " ";

      if(answer == attempt)
      {
        std::cout <<"Correct Answer :> " << attempt << " in " << std::chrono::duration<double>(execTime).count() << "ms" << "\n";
      } 
      else
      {
        std::cout << "Wrong Answer :< " << "Expected " << answer << " , got " << attempt << "\n";
      }       
    }
}