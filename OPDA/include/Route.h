#pragma once
#include <string>
#include <vector>
class Route
{
private:
    std::string departure, arrival, departureTime, arrivalTime;
    int trainNumber, id;
    double price;
    std::string seats;
    int amountOfSeats;
    int carriages;
public:
    Route(std::string dep, std::string arr, std::string depTime, std::string arrTime, int trNum,
        double price, std::string seats, int amOfSeats, int carriages, int id = 1):
        departure(dep), arrival(arr), departureTime(depTime), arrivalTime(arrTime), trainNumber(trNum),
        price(price), seats(seats), amountOfSeats(amOfSeats), carriages(carriages), id(id){}

    Route(const char* dep, const char* arr, const char* depTime, const char* arrTime, int trNum,
        double price, const char* seats, int amOfSeats, int carriages, int id = 1) :
        departure(dep), arrival(arr), departureTime(depTime), arrivalTime(arrTime), trainNumber(trNum),
        price(price), seats(seats), amountOfSeats(amOfSeats), carriages(carriages), id(id) {}

    std::string getDeparture() const { return departure; }
    std::string getArrival() const { return arrival; }
    std::string getDepartureTime() const { return departureTime; }
    std::string getArrivalTime() const { return arrivalTime; }
    int getTrainNumber() const { return trainNumber; }
    double getPrice() const { return price; } 
    std::string getSeats() const { return seats; }
    int getAmountOfSeats() const { return amountOfSeats; }
    int getCarriages() const { return carriages; }
    int getId() const { return id; }

    void setDeparture(std::string new_dep) { departure = new_dep; }
    void setArrival(std::string new_arr) { arrival = new_arr; }
    void setDepartureTime(std::string new_depTime) { departureTime = new_depTime; }
    void setArrivalTime(std::string new_arrTime) { arrivalTime = new_arrTime; }
    void setTrainNumber(int new_trNum) { trainNumber = new_trNum; }
    void setPrice(double new_price) { price = new_price; }
    void setSeats(std::string new_seats) { seats = new_seats; }
    void setAmountOfSeats(int new_amountOfSeats) { amountOfSeats = new_amountOfSeats; }
    void setAmountOfCarriages(int new_carriages) { carriages = new_carriages; }
    void setId(int new_id) { id = new_id; }
};



