#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
using namespace std;

enum class VehicleType {
    CAR,
    MOTORCYCLE
};
enum class SpotType { STANDARD, VIP, EV_CHARGING };

class Vehicle {
    public:
        virtual ~Vehicle() = default;
        virtual void displayInfo() = 0;
        virtual void park() = 0;
        virtual VehicleType getType() = 0;
};

class Car : public Vehicle {
    public:
        void displayInfo() override { cout << "This is a car." << endl; }
        void park() override { cout << "Car is parked." << endl; }
        VehicleType getType() override { return VehicleType::CAR; }
};

class Motorcycle : public Vehicle {
    public:
        void displayInfo() override { cout << "This is a motorcycle." << endl; }
        void park() override { cout << "Motorcycle is parked." << endl; }
        VehicleType getType() override { return VehicleType::MOTORCYCLE; }
};

class ParkingSpot {
    protected:
        std::mutex mtx;
        Vehicle* vehicle;
        double chargePerHour;
        double baseRate  = -1;
        double baseHours = -1;
        SpotType spotType;
        vector<VehicleType> vehicleTypes; 
        int proximity = 0; // walking distance from entrance
    public:
        ParkingSpot() : vehicle(nullptr) {}
        
        void parkVehicle(Vehicle* vehicle) {
            lock_guard<std::mutex> lock(mtx);
            if (this->vehicle) {
                cout << "Parking spot is already occupied." << endl;
                return;
            }
            if (find(vehicleTypes.begin(), vehicleTypes.end(), vehicle->getType()) == vehicleTypes.end()) {
                cout << "Vehicle type not supported in this parking spot." << endl;
                return;
            }
            this->vehicle = vehicle;
            vehicle->displayInfo();
            vehicle->park();
        }
        
        void setBaseRate(double rate) { baseRate = rate; }
        void setBaseHours(double hours) { baseHours = hours; }
        void setChargePerHour(double charge) { chargePerHour = charge; }
        
        void setSpotType(SpotType type) { spotType = type; }
        void addVehicleType(VehicleType type) { vehicleTypes.push_back(type); }
        
        void removeVehicleType(VehicleType type) {
            vehicleTypes.erase(remove(vehicleTypes.begin(), vehicleTypes.end(), type), vehicleTypes.end());
        }
        
        double getChargePerHour() { return chargePerHour; }
        double getBaseRate() { return baseRate; }
        double getBaseHours() { return baseHours; }
        vector<VehicleType> getVehicleTypes() { return vehicleTypes; }
        SpotType getSpotType() { return spotType; }
        void setProximity(int p) { proximity = p; }
        int getProximity() { return proximity; }

        void getInfo() {
            cout << "Parking Spot Info:" << endl;
            cout << "Base Rate: $" << baseRate << endl;
            cout << "Base Hours: " << baseHours << endl;
            cout << "Charge Per Hour: $" << chargePerHour << endl;
            if (vehicle) {
                cout << "Currently Parked Vehicle: " << (int)vehicle->getType() << endl; // added (int) cast for printing enum
            }
            cout << endl;
        }
        Vehicle* getVehicle() { return vehicle; }
        void freeParkingSpot() { vehicle = nullptr; }
};

class ParkingLot {
protected:
    vector<ParkingSpot*> spots; 
    int proximityToEntrance = 0;
public:
    ~ParkingLot() {
        for (auto spot : spots) delete spot;
    }

    void addSpot(ParkingSpot* spot) { spots.push_back(spot); }
    void setProximityToEntrance(int proximity) { proximityToEntrance = proximity; }
    int getProximityToEntrance() { return proximityToEntrance; }
    
    void displaySpots() {
        for (auto spot : spots) spot->getInfo();
    }
    
    void showAvailableSpots() {
        cout << "Available Parking Spots:" << endl;
        for (int index=0; index < spots.size(); index++) {
            cout << "Index = " << index << endl;
            if (!spots[index]->getVehicle()) {
                spots[index]->getInfo();
            }
        }
    }

    vector<ParkingSpot*> getAvailableSpots() {
        vector<ParkingSpot*> availableSpots;
        for (auto spot : spots) {
            if (!spot->getVehicle()) availableSpots.push_back(spot);
        }
        return availableSpots;
    }
    
    ParkingSpot* getAvailableSpot(int index) {
        if (index >= 0 && index < spots.size() && !spots[index]->getVehicle()) {
            return spots[index];
        }
        return nullptr;
    }
    
    ParkingSpot* parkVehicleInSpot(int index, Vehicle* vehicle) {
        if (index >= 0 && index < spots.size() && !spots[index]->getVehicle()) {
            spots[index]->parkVehicle(vehicle);
            return spots[index];
        } else {
            cout << "Parking spot is not available." << endl;
        }
        return nullptr;
    }
};

class IParkingStrategy {
public:
    virtual ~IParkingStrategy() = default;
    virtual ParkingSpot* selectSpot(ParkingLot* lot, Vehicle* vehicle) = 0;
};

class NearestSpotStrategy : public IParkingStrategy {
public:
    ParkingSpot* selectSpot(ParkingLot* lot, Vehicle* vehicle){
        vector<ParkingSpot*> eligibleSpots;
        vector<pair<int, int>> diffDistances;

        for (ParkingSpot* spot : lot->getAvailableSpots()) {
            
            for (VehicleType type : spot->getVehicleTypes()) {
                if (type == vehicle->getType()) {
                    eligibleSpots.push_back(spot);
                    int distance = abs(spot->getProximity() - lot->getProximityToEntrance());
                    diffDistances.push_back(make_pair(distance, eligibleSpots.size() - 1));
                }
            }
        }
        if (!diffDistances.empty()) {
            sort(diffDistances.begin(), diffDistances.end());
            return eligibleSpots[diffDistances[0].second];
        }
        return nullptr;
    }
};

class PricingStrategy {
    public:
        virtual ~PricingStrategy() = default;
        virtual double calculatePrice(int hours) = 0;
};

class FlatRatePricing : public PricingStrategy {
    private:
        double rate;
    public:
        FlatRatePricing(double r) : rate(r) {}
        double calculatePrice(int hours) override { return rate; }
};

class ParkingAreaPricing: public PricingStrategy {
    protected:
        ParkingSpot* spot;
    public:
        ParkingAreaPricing(ParkingSpot* spot) : spot(spot) {}

        double calculatePrice(int hours) override {
            double baseRate = spot->getBaseRate();
            double baseHours = spot->getBaseHours();
            double chargePerHour = spot->getChargePerHour();
            
            if (hours <= baseHours) {
                return baseRate;
            } else {
                return baseRate + (hours - baseHours) * chargePerHour;
            }
        }
};    

class Ticket {
    private:
        using TimePoint = chrono::system_clock::time_point;
        TimePoint startTime;
        TimePoint endTime;
        bool ended = false;
        PricingStrategy* pricingStrategy;
        ParkingSpot* spot;
        double totalPrice = 0;
    public:
        Ticket(TimePoint start, ParkingSpot* spot, PricingStrategy* strategy)
            : startTime(start), spot(spot), pricingStrategy(strategy) {}
        
        void setEndTime(TimePoint end) { endTime = end; ended = true; }
        
        void calculatePrice() {
            if (!ended) return;
            if (endTime < startTime) { cout << "Invalid ticket: end before start." << endl; return; }
            long long minutes = chrono::duration_cast<chrono::minutes>(endTime - startTime).count();
            int hours = (int)(minutes / 60);
            if (minutes % 60 >= 20) hours++;
            totalPrice = pricingStrategy->calculatePrice(hours);
        }

        static void printTime(TimePoint t) {
            time_t tt = chrono::system_clock::to_time_t(t);
            cout << put_time(localtime(&tt), "%Y-%m-%d %H:%M");
        }
        
        void printTicket() {
            cout << "Ticket Details:" << endl;
            cout << "Start Time: "; printTime(startTime); cout << endl;
            cout << "End Time: "; if (ended) printTime(endTime); cout << endl;
            cout << "Total Price: $" << totalPrice << endl;
            cout << endl;
        }
};

static chrono::system_clock::time_point atTime(int hour, int minute) {
    time_t now = time(nullptr);
    tm lt = *localtime(&now);
    lt.tm_hour = hour;
    lt.tm_min = minute;
    lt.tm_sec = 0;
    return chrono::system_clock::from_time_t(mktime(&lt));
}

int main() {
    unique_ptr<Vehicle> car = make_unique<Car>();
    unique_ptr<Vehicle> motorcycle = make_unique<Motorcycle>();

    unique_ptr<ParkingLot> myLot = make_unique<ParkingLot>();
    unique_ptr<IParkingStrategy> nearestSpotStrategy = make_unique<NearestSpotStrategy>();
    
    ParkingSpot* carSpot = new ParkingSpot();
    carSpot->addVehicleType(VehicleType::CAR); 
    carSpot->setProximity(3);
    
    ParkingSpot* bikeSpot = new ParkingSpot();
    bikeSpot->addVehicleType(VehicleType::MOTORCYCLE); 
    bikeSpot->setProximity(7);
    
    myLot->addSpot(carSpot);
    myLot->addSpot(bikeSpot);
    
    ParkingSpot* parkedCarSpot = nearestSpotStrategy->selectSpot(myLot.get(), car.get());
    ParkingSpot* parkedBikeSpot = nearestSpotStrategy->selectSpot(myLot.get(), motorcycle.get());

    parkedCarSpot->parkVehicle(car.get());
    parkedBikeSpot->parkVehicle(motorcycle.get());    
    
    parkedCarSpot->setBaseRate(5.0);
    parkedCarSpot->setBaseHours(2);
    parkedCarSpot->setChargePerHour(2.0);
    
    parkedBikeSpot->setBaseRate(3.0);
    parkedBikeSpot->setBaseHours(1);
    parkedBikeSpot->setChargePerHour(1.0);
    
    unique_ptr<PricingStrategy> carAreaPricing = make_unique<ParkingAreaPricing>(parkedCarSpot);
    unique_ptr<PricingStrategy> motorcycleAreaPricing = make_unique<ParkingAreaPricing>(parkedBikeSpot);
    
    unique_ptr<Ticket> carTicket = make_unique<Ticket>(atTime(10, 0), parkedCarSpot, carAreaPricing.get());
    carTicket->setEndTime(atTime(13, 30));
    carTicket->calculatePrice();
    carTicket->printTicket();
    
    unique_ptr<Ticket> motorcycleTicket = make_unique<Ticket>(atTime(11, 0), parkedBikeSpot, motorcycleAreaPricing.get());
    motorcycleTicket->setEndTime(atTime(12, 15));
    motorcycleTicket->calculatePrice();
    motorcycleTicket->printTicket();

    return 0;
}