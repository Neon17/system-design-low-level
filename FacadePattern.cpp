#include <iostream>
using namespace std;

class PowerSupply {
public:
    void providePower(){
        cout<<"Power supply is providing power."<<endl;
    }
};
class CoolingSystem {
public:
    void startFan(){
        cout<<"Fan is starting..."<<endl;
    }
};
class Cpu{
public:
    void startCpu(){
        cout<<"CPU is starting..."<<endl;
    }
};
class Memory {
public:
    void loadMemory(){
        cout<<"Memory is loading..."<<endl;
    }
};
class Bios {
    Cpu cpu;
    Memory memory;
public:
    void boot(){
        cout<<"Bios is booting..."<<endl;
        cpu.startCpu();
        memory.loadMemory();
    }
};
class HardDrive{
public:
    void spinUp(){
        cout<<"Hard drive is spinning up..."<<endl;
    }
};
class Osload {
public:
    void loadOs(){
        cout<<"Operating system is loading..."<<endl;
    }
};
class ComputerFacade {
    PowerSupply powerSupply;
    CoolingSystem coolingSystem;
    Bios bios;
    HardDrive hardDrive;
    Osload osload;

public:
    void start() {
        powerSupply.providePower();
        coolingSystem.startFan();
        bios.boot();
        hardDrive.spinUp();
        osload.loadOs();
    }
};

int main(){
    ComputerFacade computer;
    computer.start();

    return 0;
}

