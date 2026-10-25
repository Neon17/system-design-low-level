#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class TV {
public:
    void on(){
        cout<<"TV is on"<<endl;
    }
    void off() {
        cout<<"TV is off"<<endl;
    }
};

class Fan {
public:
    void on(){
        cout<<"Fan is on"<<endl;
    }
    void off() {
        cout<<"Fan is off"<<endl;
    }
};

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void stop() = 0;
};

class TVCommand: public ICommand {
    TV* tv;
public:
    TVCommand(TV* tv): tv(tv) {}
    void execute() {
        tv->on();
    }
    void stop() {
        tv->off();
    }
};

class FanCommand: public ICommand {
    Fan* fan;
public:
    FanCommand(Fan* fan): fan(fan) {}
    void execute() {
        fan->on();
    }
    void stop() {
        fan->off();
    }
};

class Remote {
    vector<ICommand*> commands;
    vector<char> pressed;
public:
    Remote(vector<ICommand*>& commands){
        for (ICommand* command: commands) {
            this->commands.push_back(command);
            pressed.push_back(0);
        }
    }

    void press(int index){
        if (index < 0 || index >= commands.size()) return;
        if (pressed[index] == 0){
            commands[index]->execute();
            pressed[index] = 1;
        } else {
            commands[index]->stop();
            pressed[index] = 0;
        }
    }
};


int main(){
    TV tv; Fan fan;
    ICommand* tvcommand = new TVCommand(&tv);
    ICommand* fancommand = new FanCommand(&fan);
    vector<ICommand*> commands = {tvcommand, fancommand};

    Remote remote(commands);
    remote.press(0);
    remote.press(1);
    remote.press(1);
    remote.press(0);

    delete(fancommand);
    delete(tvcommand);

    return 0;
}


