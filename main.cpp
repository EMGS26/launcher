#include <windows.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

const string VBOX = "C:\\PROGRA~1\\Oracle\\VirtualBox\\VBoxManage.exe";
const string VM   = "VM_Kine";
const string IP   = "192.168.0.130";

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
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

    system((VBOX + " startvm \"" + VM + "\" --type headless").c_str());

    cout << "En attente du demarrage";
    for (int i = 0; i < 15; i++) {
        this_thread::sleep_for(chrono::seconds(2));
        cout << "." << flush;
    }
    cout << endl;

    setColor(10);
    cout << "VM demarree - http://" << IP << endl;
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

    system((VBOX + " controlvm \"" + VM + "\" acpipowerbutton").c_str());

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
        setColor(11);
        cout << "IP    : " << IP << endl;
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