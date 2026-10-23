#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Observable;

class Observer {
protected:
    Observable* observable;
public:
    virtual void update(Observable* observable) = 0;
};

class Observable {
protected:
    vector<class Observer*> observers;
public:
    void subscribe(Observer* observer) {
        observers.push_back(observer);
    }
    void unsubscribe(Observer* observer) {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it == observers.end()) return;
        observers.erase(it);
        
    }
    void notifyObservers() {
        for (auto it: observers){
            it->update(this);
        }
    }
    virtual ~Observable() = default;
};

class ConcreteObservable: public Observable {
int temperature = 0;
public:
    int getTemperature(){ return temperature; }

    void setTemperature(int temperature){
        this->temperature = temperature;
        this->notifyObservers();
    }
};

class ConcreteObserver: public Observer {
public:
    void update(Observable* observable){
        cout<<"Something changed, now fetching current state"<<endl;
        this->observable = observable;
        this->currentState();
    }

    void currentState(){
        ConcreteObservable* concreteObservable = this->observable ? dynamic_cast<ConcreteObservable*>(this->observable) : nullptr;
        if (concreteObservable){
            cout<<"Current temperature is: "<<concreteObservable->getTemperature()<<endl;
        }
        cout<<endl;
    }
};

int main() {
    ConcreteObservable* observable = new ConcreteObservable();
    Observer* observer1 = new ConcreteObserver();
    Observer* observer2 = new ConcreteObserver();

    observable->subscribe(observer1);
    observable->subscribe(observer2);

    observable->setTemperature(25); 

    observable->unsubscribe(observer1);
    observable->unsubscribe(observer2);
    delete observer1;
    delete observer2;
    delete observable;

    return 0;
}

