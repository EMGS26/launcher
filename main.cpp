#include <windows.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

const string VBOX = "C:\\PROGRA~1\\Oracle\\VirtualBox\\VBoxManage.exe";
const string VM   = "VM_Kine";

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int color) {
    SetConsoleTextAttribute(console, color);
}

bool vmIsRunning() {
    string cmd = VBOX + " list runningvms";
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) return false;

    char buffer[256];
    string output = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }
    _pclose(pipe);

    return output.find(VM) != string::npos;
}

string getIP() {
    for (int i = 0; i <= 3; i++) {
        string prop = "/VirtualBox/GuestInfo/Net/" + to_string(i) + "/V4/IP";
        string cmd  = VBOX + " guestproperty get \"" + VM + "\" " + prop;

        FILE* pipe = _popen(cmd.c_str(), "r");
        if (!pipe) continue;

        char buffer[256];
        string output = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            output += buffer;
        }
        _pclose(pipe);

        size_t pos = output.find("Value: ");
        if (pos == string::npos) continue;

        string ip = output.substr(pos + 7);
        while (!ip.empty() && (ip.back() == '\n' || ip.back() == '\r' || ip.back() == ' ')) {
            ip.pop_back();
        }
        if (!ip.empty()) return ip;
    }
    return "";
}

void startVM() {
    if (vmIsRunning()) {
        setColor(14);
        cout << "La VM est deja demarree." << endl;
        setColor(7);
        return;
    }

    setColor(11);
    cout << "Demarrage de la VM..." << endl;
    setColor(7);

    string cmd = VBOX + " startvm \"" + VM + "\" --type headless";
    system(cmd.c_str());

    cout << "En attente de l'IP";
    string ip = "";
    for (int i = 0; i < 30; i++) {
        this_thread::sleep_for(chrono::seconds(2));
        cout << "." << flush;
        ip = getIP();
        if (!ip.empty()) break;
    }
    cout << endl;

    if (!ip.empty()) {
        setColor(10);
        cout << "VM demarree -  http://" << ip << endl;
    } else {
        setColor(12);
        cout << "VM demarree mais IP non recuperee." << endl;
    }
    setColor(7);
}

void stopVM() {
    if (!vmIsRunning()) {
        setColor(14);
        cout << "La VM est deja arretee." << endl;
        setColor(7);
        return;
    }

    setColor(12);
    cout << "Arret de la VM..." << endl;
    setColor(7);

    string cmd = VBOX + " controlvm \"" + VM + "\" acpipowerbutton";
    system(cmd.c_str());

    cout << "En attente de l'arret";
    for (int i = 0; i < 15; i++) {
        this_thread::sleep_for(chrono::seconds(2));
        cout << "." << flush;
        if (!vmIsRunning()) break;
    }
    cout << endl;

    if (!vmIsRunning()) {
        setColor(10);
        cout << "VM arretee." << endl;
    } else {
        setColor(12);
        cout << "La VM ne repond pas. Forcer l'arret ? (o/n) : ";
        setColor(7);
        char rep;
        cin >> rep;
        if (rep == 'o' || rep == 'O') {
            system((VBOX + " controlvm \"" + VM + "\" poweroff").c_str());
            setColor(10);
            cout << "VM forcee a l'arret." << endl;
            setColor(7);
        }
    }
}

void showMenu() {
    system("cls");
    cout << "=========================\n";
    cout << "      VM_Kine Launcher\n";
    cout << "=========================\n\n";

    if (vmIsRunning()) {
        setColor(10);
        cout << "Etat : EN COURS D'EXECUTION" << endl;
        string ip = getIP();
        if (!ip.empty()) {
            setColor(11);
            cout << "IP    : " << ip << endl;
        }
    } else {
        setColor(12);
        cout << "Etat : ARRETEE" << endl;
    }
    setColor(7);

    cout << "\n1. Demarrer la VM\n";
    cout << "2. Arreter la VM\n";
    cout << "0. Quitter\n";
    cout << "\nChoix > ";
}

int main() {
    int choice;
    do {
        showMenu();
        cin >> choice;
        cout << endl;

        if (choice == 1) startVM();
        if (choice == 2) stopVM();
        if (choice == 0) cout << "Au revoir." << endl;

        if (choice != 0) {
            cout << "\nAppuyez sur Entree pour continuer...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 0);

    return 0;
}
