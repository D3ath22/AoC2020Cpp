#include "../PuzzleSolverUtil.h"

struct BusData
{
  int departureEst;
  vector<int> buses;
  vector<int> intervals;
};

class PuzzleSolverOne
{
  private:  
    constexpr static int day = 13;
    constexpr static int part = 1;
    constexpr static int answer = 3882;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    constexpr static auto fieldDelimiter = ",";
    vector<string> input;

    BusData parseInput()
    {
      BusData busData;
      busData.departureEst = std::stoi(this->input[0]);

      int interval = 0;

      for(string entry : util::split(this->input[1], string(this->fieldDelimiter)))
      {
        if(std::isdigit(entry[0]))
        {
          busData.buses.push_back(std::stoi(entry));
          busData.intervals.push_back(interval);
        } 
        interval++;
      }

      return busData;
    }

    int getWaitTime(int bus)
    {
      return (bus * ((std::floor(this->busData.departureEst/bus)) + 1)) - this->busData.departureEst;
    } 

    tuple<int, int> getEarliestBus()
    {
      int earliestBus = 0;
      int waitTime = INT_MAX;

      for(const auto bus : this->busData.buses)
      {
        int newWaitTime = getWaitTime(bus);     

        if(newWaitTime < waitTime)
        {
          earliestBus = bus;
          waitTime = newWaitTime;
        }
      }

      return {earliestBus, waitTime};
    }

    int makeSolution(const tuple<int, int> solution)
    {
      return get<0>(solution) * get<1>(solution);
    }

    public:
      BusData busData; 

      PuzzleSolverOne()
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();
      }           

      void solve()
      {
        this->input = util::loadInputStringLines(string(this->inputFileName));
        this->busData = parseInput();
        tuple<int, int> solution = getEarliestBus();
        util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, makeSolution(solution));   
      }
};

class PuzzleSolverTwo
{
  private:  
    constexpr static int day = 13;
    constexpr static int part = 2;
    constexpr static ll answer = 867295486378319;
    constexpr static auto inputFileName = "input.txt";
    std::chrono::high_resolution_clock::time_point execTimeStart;

    PuzzleSolverOne solverOne; 
    BusData busData;

    int correctDeparture(ll departure, int bus, int interval)
    {
      return (departure + interval) % bus;
    }
    
    ll getDeparture()
    {
      ll departure = 0;   
      ll currIncrease = this->busData.buses[0];

      for(int i = 1; i < this->busData.buses.size(); i++)
      {
        int bus = this->busData.buses[i];
        int interval = this->busData.intervals[i];

        while(correctDeparture(departure, bus, interval) != 0)
        {
          departure += currIncrease;
        }

        currIncrease *= bus;
      }

      return departure;
    }

    public:

      PuzzleSolverTwo(PuzzleSolverOne &solverOne) : solverOne(solverOne)
      {
        this->execTimeStart = std::chrono::high_resolution_clock::now();
        this->busData = solverOne.busData;
      }    

      void solve()
      {
        util::assertAnswer(this->day, this->part, this->execTimeStart, this->answer, getDeparture());  
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