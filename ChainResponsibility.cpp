#include <iostream>
using namespace std;

class MoneyHandler {
protected:
    int numNotes = 0;
public:
    MoneyHandler* nextHandler;
    virtual int getDenomination() = 0;

    virtual void dispense(int amount){
        int denom = getDenomination();
        int notesNeeded = amount / denom;

        if (notesNeeded > numNotes){
            notesNeeded = numNotes;
            numNotes = 0;
        } else {
            numNotes -= notesNeeded;
        }

        if (notesNeeded > 0){
            cout<<"Dispensing " << notesNeeded << " X Rs. "<<denom<<" notes." << endl;
        }
        int rem = amount - (notesNeeded * denom);
        if (rem > 0){
            if (nextHandler) nextHandler->dispense(rem);
            else cout << "Remaining amount of "<<rem<<" cannot be fulfilled."<<endl;
        }
    }
};

class ThousandHandler: public MoneyHandler {
public:
    ThousandHandler(int numNotes) {this->numNotes = numNotes;}
    int getDenomination() {return 1000;}
};
class FiveHundredHandler: public MoneyHandler {
public:
    FiveHundredHandler(int numNotes){this->numNotes = numNotes;}
    int getDenomination() {return 500;}
};
class TwoHundredHandler: public MoneyHandler {
public:
    TwoHundredHandler(int numNotes){this->numNotes = numNotes;}
    int getDenomination() {return 200;}
};
class HundredHandler: public MoneyHandler {
public:
    HundredHandler(int numNotes){this->numNotes = numNotes;}
    int getDenomination() {return 100;}
};

int main() {
    MoneyHandler* thousandHandler = new ThousandHandler(3);
    MoneyHandler* fivehundredHandler = new FiveHundredHandler(5);
    MoneyHandler* twohundredHandler = new TwoHundredHandler(10);
    MoneyHandler* hundredHandler = new HundredHandler(20);

    thousandHandler->nextHandler = fivehundredHandler;
    fivehundredHandler->nextHandler = twohundredHandler;
    twohundredHandler->nextHandler = hundredHandler;

    int amountToWithdraw = 5700;
    cout<<"Dispensing Amount: Rs. "<<amountToWithdraw<<endl;
    thousandHandler->dispense(amountToWithdraw);

    return 0;
}

