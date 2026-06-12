#include <windows.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
using namespace std;

const string VBOX = "C:\\PROGRA~1\\Oracle\\VirtualBox\\VBoxManage.exe";
const string VM   = "VM_Kine";
const string IP   = "192.168.0.130";

void setColor(int c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); }

bool vmIsRunning() {
    FILE* pipe = _popen((VBOX + " list runningvms").c_str(), "r");
    if (!pipe) return false;
    char buf[256]; string out;
    while (fgets(buf, sizeof(buf), pipe)) out += buf;
    _pclose(pipe);
    return out.find(VM) != string::npos;
}

void wait(int steps = 15) {
    for (int i = 0; i < steps; i++) {
        this_thread::sleep_for(chrono::seconds(2));
        cout << "." << flush;
    }
    cout << endl;
}

void startVM() {
    setColor(11); cout << "Demarrage..." << endl; setColor(7);
    system((VBOX + " startvm \"" + VM + "\" --type headless").c_str());
    cout << "Attente"; wait();
    setColor(10); cout << "http://" << IP << endl; setColor(7);
}

void stopVM() {
    setColor(12); cout << "Arret..." << endl; setColor(7);
    system((VBOX + " controlvm \"" + VM + "\" acpipowerbutton").c_str());
    cout << "Attente"; wait();
    setColor(10); cout << "VM arretee." << endl; setColor(7);
}

int main() {
    int choice;
    do {
        system("cls");
        cout << "=== VM_Kine === [";
        if (vmIsRunning()) { setColor(10); cout << "ON";  }
        else               { setColor(12); cout << "OFF"; }
        setColor(7); cout << "]\n\n1. Demarrer\n2. Arreter\n0. Quitter\n\nChoix > ";
        cin >> choice;
        cout << endl;
        if (choice == 1) startVM();
        if (choice == 2) stopVM();
        if (choice != 0) { cout << "\nEntree pour continuer..."; cin.ignore(); cin.get(); }
    } while (choice != 0);
    return 0;
}