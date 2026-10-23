#include <iostream>
using namespace std;

class VendingMachine;

class VendingState {
public:
    virtual void insertCoin(VendingMachine* machine) = 0;
    virtual void selectItem(VendingMachine* machine) = 0;
    virtual void dispenseItem(VendingMachine* machine) = 0;
    virtual void refill(VendingMachine* machine) = 0;
    virtual string getStateName() = 0;
};

class NoCoinState: public VendingState {
public:
    NoCoinState();
    void insertCoin(VendingMachine* machine) override;
    void selectItem(VendingMachine* machine) override;
    void dispenseItem(VendingMachine* machine) override;
    void refill(VendingMachine* machine) override;
    string getStateName() override{
        return "NoCoinState";
    }
};

class HasCoinState: public VendingState {
public:
    HasCoinState();
    void insertCoin(VendingMachine* machine) override;
    void selectItem(VendingMachine* machine) override;
    void dispenseItem(VendingMachine* machine) override;
    void refill(VendingMachine* machine) override;
    string getStateName() override{
        return "HasCoinState";
    }
};

class DispenseState: public VendingState {
public:
    DispenseState();
    void insertCoin(VendingMachine* machine) override;
    void selectItem(VendingMachine* machine) override;
    void dispenseItem(VendingMachine* machine) override;
    void refill(VendingMachine* machine) override;
    string getStateName() override{
        return "DispenseState";
    }
};

class SoldOutState: public VendingState {
public:
    SoldOutState();
    void insertCoin(VendingMachine* machine) override;
    void selectItem(VendingMachine* machine) override;
    void dispenseItem(VendingMachine* machine) override;
    void refill(VendingMachine* machine) override;
    string getStateName() override{
        return "SoldOutState";
    }
};

class VendingMachine {
protected:
    VendingState* currentState;
    int itemCount = 0, itemPrice = 0, selectedItem = 0, coins = 0;
public:
    VendingState* noCoinState;
    VendingState* hasCoinState;
    VendingState* dispenseState;
    VendingState* soldOutState;

    VendingMachine(int itemCount, int itemPrice): itemCount(itemCount), itemPrice(itemPrice){
        noCoinState = new NoCoinState();
        hasCoinState = new HasCoinState();
        dispenseState = new DispenseState();
        soldOutState = new SoldOutState();

        if (itemCount > 0) {
            currentState = noCoinState;
        } else {
            currentState = soldOutState;
        }
    }

    VendingState* getState() {
        return currentState;
    }
    void setState(VendingState* state) {
        currentState = state;
    }
    void setItemCount(int count) {
        itemCount = count;
    }
    int getItemCount() {
        return itemCount;
    }
    void setItemPrice(int price) {
        itemPrice = price;
    }
    int getItemPrice() {
        return itemPrice;
    }
    void setCoins(int count) {
        coins = count;
    }
    int getCoins() {
        return coins;
    }

    void insertCoin(){
        currentState->insertCoin(this);
    }

    void selectItem(){
        currentState->selectItem(this);
    }   
    
    void dispenseItem() {
        currentState->dispenseItem(this);
    }

    void refill(){
        currentState->refill(this);
    }

    void printStatus() {
        cout << "--------------------------" << endl;
        cout << "Current State: " << currentState->getStateName() << endl;
        cout << "Items Remaining: " << itemCount << endl;
        cout << "Inserted Coins: " << coins << endl;
        cout << "--------------------------" << endl;
    }

    ~VendingMachine() {
        delete noCoinState;
        delete hasCoinState;
        delete dispenseState;
        delete soldOutState;
    }
};


NoCoinState::NoCoinState(){}

void NoCoinState::insertCoin(VendingMachine* machine){
    machine->setState(machine->hasCoinState);
    machine->setCoins(machine->getCoins() + 1);
}
void NoCoinState::selectItem(VendingMachine* machine) {
    return;
}
void NoCoinState::dispenseItem(VendingMachine* machine) {
    return;
}
void NoCoinState::refill(VendingMachine* machine) {
    return;
}

HasCoinState::HasCoinState(){}

void HasCoinState::insertCoin(VendingMachine* machine){
    machine->setCoins(machine->getCoins() + 1);
}
void HasCoinState::selectItem(VendingMachine* machine) {
    if (machine->getCoins() < machine->getItemPrice()) {
        cout << "Not enough coins inserted." << endl;
        return;
    }
    machine->setState(machine->dispenseState);
}
void HasCoinState::dispenseItem(VendingMachine* machine) {
    machine->setState(machine->dispenseState);
}
void HasCoinState::refill(VendingMachine* machine) {
    return;
}

DispenseState::DispenseState(){}

void DispenseState::insertCoin(VendingMachine* machine){
    return;
}
void DispenseState::selectItem(VendingMachine* machine) {
    return;
}
void DispenseState::dispenseItem(VendingMachine* machine) {
    if (machine->getItemCount() > 0) {
        machine->setItemCount(machine->getItemCount() - 1);
        machine->setCoins(machine->getCoins() - machine->getItemPrice());
        cout << "Dispensing item." << endl;
        if (machine->getItemCount() == 0) {
            machine->setState(machine->soldOutState);
        } else {
            machine->setState(machine->noCoinState);
        }
    } else {
        cout << "No items to dispense." << endl;
        machine->setState(machine->soldOutState);
    }
}
void DispenseState::refill(VendingMachine* machine) {
    return;
}

SoldOutState::SoldOutState(){}

void SoldOutState::insertCoin(VendingMachine* machine){
    machine->setCoins(machine->getCoins() + 1);
}
void SoldOutState::selectItem(VendingMachine* machine) {
    return;
}
void SoldOutState::dispenseItem(VendingMachine* machine) {
    return;
}
void SoldOutState::refill(VendingMachine* machine) {
    machine->setItemCount(machine->getItemCount() + 1);
    machine->setState(machine->noCoinState);
}

int main(){
    // Initialize Vending Machine with 2 items, costing 20 each
    VendingMachine* machine = new VendingMachine(2, 20);

    // Initial Status
    machine->printStatus();

    // 1. Trying to select without coin (should fail/ask for coin)
    machine->selectItem();

    // 2. Insert coin (10)
    machine->insertCoin();

    // 3. Try to select with insufficient funds (10 < 20)
    machine->selectItem();

    // 4. Insert another coin (10), now total 20
    machine->insertCoin();

    // 5. Select item (should move to DispenseState)
    machine->selectItem();

    // 6. Dispense item
    machine->dispenseItem();

    // Final check
    machine->printStatus();

    return 0;
}


