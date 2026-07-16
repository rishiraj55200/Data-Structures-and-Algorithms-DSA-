
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>

using namespace std;

/*
===========================================================
                    VEHICLE
===========================================================
Represents a vehicle entering parking lot.
*/
enum class VehicleType {
    TWO_WHEELER,
    FOUR_WHEELER
};

class Vehicle {
public:
    string vehicleNumber;
    VehicleType type;

    Vehicle(string number, VehicleType type)
        : vehicleNumber(number), type(type) {}
};


/*
===========================================================
                 PARKING SPOT
===========================================================
Represents one parking spot.
*/
class ParkingSpot {

private:
    int spotId;
    bool occupied;

public:

    ParkingSpot(int id)
        : spotId(id), occupied(false) {}

    bool isFree() {
        return !occupied;
    }

    void occupy() {
        occupied = true;
    }

    void release() {
        occupied = false;
    }

    int getSpotId() {
        return spotId;
    }
};


/*
===========================================================
      STRATEGY PATTERN FOR SPOT ALLOCATION
===========================================================

Different allocation strategies can be plugged in:
1. Nearest Spot
2. Cheapest Spot
3. Random Spot

Without modifying manager class.
*/
class ParkingSpotLookupStrategy {
public:

    virtual ParkingSpot* findSpot(
        vector<ParkingSpot*>& spots) = 0;

    virtual ~ParkingSpotLookupStrategy() {}
};


/*
Simple strategy:
Return first free spot.
*/
class RandomLookupStrategy
    : public ParkingSpotLookupStrategy {

public:

    ParkingSpot* findSpot(
        vector<ParkingSpot*>& spots) override {

        for(auto spot : spots) {

            if(spot->isFree())
                return spot;
        }

        return nullptr;
    }
};


/*
===========================================================
              PARKING SPOT MANAGER
===========================================================

Responsible for:

1. Parking vehicle
2. Releasing vehicle
3. Managing spots

Thread-safe using mutex.
*/
class ParkingSpotManager {

protected:

    vector<ParkingSpot*> spots;
    ParkingSpotLookupStrategy* strategy;

    mutex mtx;

public:

    ParkingSpotManager(
        vector<ParkingSpot*> spots,
        ParkingSpotLookupStrategy* strategy)
        : spots(spots),
          strategy(strategy) {}

    ParkingSpot* parkVehicle() {

        lock_guard<mutex> lock(mtx);

        ParkingSpot* spot =
            strategy->findSpot(spots);

        if(spot)
            spot->occupy();

        return spot;
    }

    void unParkVehicle(
        ParkingSpot* spot) {

        lock_guard<mutex> lock(mtx);

        spot->release();
    }

    bool hasAvailability() {

        for(auto spot : spots) {

            if(spot->isFree())
                return true;
        }

        return false;
    }
};


/*
===========================================================
                     TICKET
===========================================================

Stores:
1. Vehicle
2. Spot
3. Entry Time

Used during exit and billing.
*/
class Ticket {

public:

    Vehicle* vehicle;
    ParkingSpot* spot;

    chrono::system_clock::time_point entryTime;

    Ticket(
        Vehicle* vehicle,
        ParkingSpot* spot)
        : vehicle(vehicle),
          spot(spot) {

        entryTime =
            chrono::system_clock::now();
    }
};


/*
===========================================================
                 PARKING LEVEL
===========================================================

One floor of parking lot.

Delegates work to manager.
*/
class ParkingLevel {

private:

    ParkingSpotManager* manager;

public:

    ParkingLevel(
        ParkingSpotManager* manager)
        : manager(manager) {}

    ParkingSpot* park() {
        return manager->parkVehicle();
    }

    void unPark(
        ParkingSpot* spot) {

        manager->unParkVehicle(spot);
    }

    bool hasAvailability() {
        return manager->hasAvailability();
    }
};


/*
===========================================================
                PARKING BUILDING
===========================================================

Contains multiple floors.

Responsible for finding
available floor.
*/
class ParkingBuilding {

private:

    vector<ParkingLevel*> levels;

public:

    void addLevel(
        ParkingLevel* level) {

        levels.push_back(level);
    }

    ParkingLevel* getAvailableLevel() {

        for(auto level : levels) {

            if(level->hasAvailability())
                return level;
        }

        return nullptr;
    }
};


/*
===========================================================
                 ENTRANCE GATE
===========================================================

Entry point of parking lot.

Responsibilities:
1. Find floor
2. Allocate spot
3. Generate ticket
*/
class EntranceGate {

public:

    Ticket* enter(
        Vehicle* vehicle,
        ParkingBuilding* building) {

        ParkingLevel* level =
            building->getAvailableLevel();

        if(level == nullptr)
            return nullptr;

        ParkingSpot* spot =
            level->park();

        if(spot == nullptr)
            return nullptr;

        return new Ticket(vehicle, spot);
    }
};


/*
===========================================================
              PRICING STRATEGY
===========================================================

Strategy Pattern.

Different billing methods possible:
1. Hourly
2. Fixed
3. Weekend
4. Dynamic
*/
class PricingStrategy {

public:

    virtual int calculateCost(
        Ticket* ticket) = 0;

    virtual ~PricingStrategy() {}
};


/*
Simple fixed pricing.
*/
class FixedPricingStrategy
    : public PricingStrategy {

public:

    int calculateCost(
        Ticket* ticket) override {

        return 100;
    }
};


/*
===========================================================
              COST COMPUTATION
===========================================================
Delegates billing logic to strategy.
*/
class CostComputation {

private:

    PricingStrategy* strategy;

public:

    CostComputation(
        PricingStrategy* strategy)
        : strategy(strategy) {}

    int compute(
        Ticket* ticket) {

        return strategy
            ->calculateCost(ticket);
    }
};


/*
===========================================================
             PAYMENT STRATEGY
===========================================================

Can support:
1. Cash
2. UPI
3. Card
4. Wallet
*/
class Payment {

public:

    virtual void pay(
        int amount) = 0;

    virtual ~Payment() {}
};


class CashPayment : public Payment {

public:

    void pay(int amount) override {

        cout << "Paid "
             << amount
             << " using Cash\n";
    }
};


class UPIPayment : public Payment {

public:

    void pay(int amount) override {

        cout << "Paid "
             << amount
             << " using UPI\n";
    }
};


/*
===========================================================
                   EXIT GATE
===========================================================

Flow:

1. Calculate bill
2. Take payment
3. Release parking spot
*/
class ExitGate {

private:

    CostComputation* costComputation;
    Payment* payment;

public:

    ExitGate(
        CostComputation* costComputation,
        Payment* payment)
        : costComputation(costComputation),
          payment(payment) {}

    void completeExit(
        Ticket* ticket,
        ParkingLevel* level) {

        int amount =
            costComputation
            ->compute(ticket);

        payment->pay(amount);

        level->unPark(
            ticket->spot);
    }
};


/*
===========================================================
                      MAIN
===========================================================
*/
int main() {

    // Create parking spots

    vector<ParkingSpot*> spots;

    for(int i = 1; i <= 5; i++) {

        spots.push_back(
            new ParkingSpot(i));
    }

    // Spot allocation strategy

    auto lookupStrategy =
        new RandomLookupStrategy();

    // Spot manager

    auto manager =
        new ParkingSpotManager(
            spots,
            lookupStrategy);

    // One parking floor

    auto level =
        new ParkingLevel(manager);

    // Parking building

    ParkingBuilding building;

    building.addLevel(level);

    // Vehicle arrives

    Vehicle car(
        "DL01AB1234",
        VehicleType::FOUR_WHEELER);

    // Entry

    EntranceGate entranceGate;

    Ticket* ticket =
        entranceGate.enter(
            &car,
            &building);

    if(ticket == nullptr) {

        cout << "Parking Full\n";
        return 0;
    }

    cout << "Vehicle Parked\n";

    // Pricing Strategy

    auto pricingStrategy =
        new FixedPricingStrategy();

    CostComputation costComputation(
        pricingStrategy);

    // Payment Strategy

    Payment* payment =
        new UPIPayment();

    // Exit

    ExitGate exitGate(
        &costComputation,
        payment);

    exitGate.completeExit(
        ticket,
        level);

    return 0;
}
```
