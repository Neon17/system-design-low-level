#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

enum class VehicleType {
    CAR,
    MOTORCYCLE
};
enum class SpotType { STANDARD, VIP, EV_CHARGING };

class Vehicle {
    public:
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
        Vehicle* vehicle;
        double chargePerHour;
        double baseRate  = -1;
        double baseHours = -1;
        vector<SpotType> spotTypes; 
        vector<VehicleType> vehicleTypes; 
    public:
        ParkingSpot() : vehicle(nullptr) {}
        
        void parkVehicle(Vehicle* vehicle) {
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
        
        void addSpotType(SpotType type) { spotTypes.push_back(type); }
        void addVehicleType(VehicleType type) { vehicleTypes.push_back(type); }
        
        void removeSpotType(SpotType type) {
            spotTypes.erase(remove(spotTypes.begin(), spotTypes.end(), type), spotTypes.end());
        }
        void removeVehicleType(VehicleType type) {
            vehicleTypes.erase(remove(vehicleTypes.begin(), vehicleTypes.end(), type), vehicleTypes.end());
        }
        
        double getChargePerHour() { return chargePerHour; }
        double getBaseRate() { return baseRate; }
        double getBaseHours() { return baseHours; }
        
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
public:
    ~ParkingLot() {
        for (auto spot : spots) delete spot;
    }

    void addSpot(ParkingSpot* spot) { spots.push_back(spot); }
    
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

class PricingStrategy {
    public:
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
        string startTime;
        string endTime;
        PricingStrategy* pricingStrategy;
        ParkingSpot* spot;
        double totalPrice;
    public:
        Ticket(string start, ParkingSpot* spot, PricingStrategy* strategy) : startTime(start), spot(spot), pricingStrategy(strategy) {}
        
        void setEndTime(string end) { endTime = end; }
        
        void calculatePrice() {
            int hours = stoi(endTime.substr(0, 2)) - stoi(startTime.substr(0, 2));
            int minutes = stoi(endTime.substr(3, 2)) - stoi(startTime.substr(3, 2));
            if (minutes >= 20) hours++;
            totalPrice = pricingStrategy->calculatePrice(hours);
        }
        
        void printTicket() {
            cout << "Ticket Details:" << endl;
            cout << "Start Time: " << startTime << endl;
            cout << "End Time: " << endTime << endl;
            cout << "Total Price: $" << totalPrice << endl;
            cout << endl;
        }
};

int main() {
    unique_ptr<Vehicle> car = make_unique<Car>();
    unique_ptr<Vehicle> motorcycle = make_unique<Motorcycle>();

    unique_ptr<ParkingLot> myLot = make_unique<ParkingLot>();
    
    ParkingSpot* carSpot = new ParkingSpot();
    carSpot->addVehicleType(VehicleType::CAR); 
    
    ParkingSpot* bikeSpot = new ParkingSpot();
    bikeSpot->addVehicleType(VehicleType::MOTORCYCLE); 
    
    myLot->addSpot(carSpot);
    myLot->addSpot(bikeSpot);
    
    ParkingSpot* parkedCarSpot = myLot->parkVehicleInSpot(0, car.get());
    ParkingSpot* parkedBikeSpot = myLot->parkVehicleInSpot(1, motorcycle.get());
    
    parkedCarSpot->setBaseRate(5.0);
    parkedCarSpot->setBaseHours(2);
    parkedCarSpot->setChargePerHour(2.0);
    
    parkedBikeSpot->setBaseRate(3.0);
    parkedBikeSpot->setBaseHours(1);
    parkedBikeSpot->setChargePerHour(1.0);
    
    unique_ptr<PricingStrategy> carAreaPricing = make_unique<ParkingAreaPricing>(parkedCarSpot);
    unique_ptr<PricingStrategy> motorcycleAreaPricing = make_unique<ParkingAreaPricing>(parkedBikeSpot);
    
    unique_ptr<Ticket> carTicket = make_unique<Ticket>("10:00", parkedCarSpot, carAreaPricing.get());
    carTicket->setEndTime("13:30");
    carTicket->calculatePrice();
    carTicket->printTicket();
    
    unique_ptr<Ticket> motorcycleTicket = make_unique<Ticket>("11:00", parkedBikeSpot, motorcycleAreaPricing.get());
    motorcycleTicket->setEndTime("12:15");
    motorcycleTicket->calculatePrice();
    motorcycleTicket->printTicket();

    return 0;
}