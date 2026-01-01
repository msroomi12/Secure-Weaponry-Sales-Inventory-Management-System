//  Weapon inventory managment system


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;



class Weapon {
protected:
    string maker;
    double price;
    string serial;

public:
    Weapon(string mk, double pr, string sr) : maker(mk), price(pr), serial(sr) {}
    virtual double finalPrice() = 0;
    virtual void show() = 0;
    virtual string getType() = 0;
    string getSerial() { return serial; }
    string getMaker() { return maker; }
    double getPrice() { return price; }
    virtual ~Weapon() {}
};

class CivilianWeapon : public Weapon {
    int magazine;
public:
    CivilianWeapon(string mk, double pr, string sr, int mag)
        : Weapon(mk, pr, sr), magazine(mag) {}
    double finalPrice() { return price * 1.10; }
    void show() {
        cout << "Civilian | Serial: " << serial
             << " | Maker: " << maker
             << " | Magazine: " << magazine
             << " | Price: " << finalPrice() << endl;
    }
    string getType() { return "Civilian"; }
    int getMagazine() { return magazine; }
};

class MilitaryWeapon : public Weapon {
    int clearance;
public:
    MilitaryWeapon(string mk, double pr, string sr, int cl)
        : Weapon(mk, pr, sr), clearance(cl) {}
    double finalPrice() { return price; }
    void show() {
        cout << "Military | Serial: " << serial
             << " | Maker: " << maker
             << " | Clearance: " << clearance
             << " | Price: " << finalPrice() << endl;
    }
    string getType() { return "Military"; }
    int getClearance() { return clearance; }
};

class MeleeWeapon : public Weapon {
    int blade;
public:
    MeleeWeapon(string mk, double pr, string sr, int bl)
        : Weapon(mk, pr, sr), blade(bl) {}
    double finalPrice() { return price * 1.05; }
    void show() {
        cout << "Melee | Serial: " << serial
             << " | Maker: " << maker
             << " | Blade Length: " << blade
             << " | Price: " << finalPrice() << endl;
    }
    string getType() { return "Melee"; }
    int getBlade() { return blade; }
};

class Buyer {
public:
    string name;
    int age;
    string license;
    Buyer(string n = "", int a = 0, string l = "") : name(n), age(a), license(l) {}
};

class InventoryManager {
    vector<Weapon*> weapons;
public:
    ~InventoryManager() {
        for (auto w : weapons) delete w;
    }

    void loadFromFile() {
        ifstream file("weapons.txt");
        if (!file) return;

        string type, maker, serial;
        double price;
        int extra;
        while (file >> type >> maker >> price >> serial >> extra) {
            if (type == "Civilian") weapons.push_back(new CivilianWeapon(maker, price, serial, extra));
            else if (type == "Military") weapons.push_back(new MilitaryWeapon(maker, price, serial, extra));
            else if (type == "Melee") weapons.push_back(new MeleeWeapon(maker, price, serial, extra));
        }
        file.close();
    }

    void saveToFile() {
        ofstream file("weapons.txt");
        for (auto w : weapons) {
            int extra = 0;
            if (w->getType() == "Civilian") extra = dynamic_cast<CivilianWeapon*>(w)->getMagazine();
            else if (w->getType() == "Military") extra = dynamic_cast<MilitaryWeapon*>(w)->getClearance();
            else if (w->getType() == "Melee") extra = dynamic_cast<MeleeWeapon*>(w)->getBlade();
            file << w->getType() << " " << w->getMaker() << " " << w->getPrice() << " " << w->getSerial() << " " << extra << "\n";
        }
        file.close();
    }

    void addWeapon(Weapon* w) {
        weapons.push_back(w);
        saveToFile();
        cout << "Weapon added successfully.\n";
    }

    void showInventory() {
        if (weapons.empty()) { cout << "Inventory empty.\n"; return; }
        for (auto w : weapons) w->show();
    }

    Weapon* findWeapon(string sr) {
        for (auto w : weapons) if (w->getSerial() == sr) return w;
        return nullptr;
    }

    void removeWeapon(string sr) {
        for (int i = 0; i < weapons.size(); i++) {
            if (weapons[i]->getSerial() == sr) {
                delete weapons[i];
                weapons.erase(weapons.begin() + i);
                saveToFile();
                return;
            }
        }
    }
};


class User {
public:
    string username;
    string password;
    string role;
    User(string u = "", string p = "", string r = "") : username(u), password(p), role(r) {}
};

bool login(User &loggedUser) {
    ifstream file("users.txt");
    
    // Check if file exists AND is not empty
    bool fileExists = file.good();
    bool fileIsEmpty = (file.peek() == ifstream::traits_type::eof());

    if (!fileExists || fileIsEmpty) {
        file.close(); // Close read mode to switch to write mode
        ofstream init("users.txt");
        // Format: [Username] [Password] [Role]
        init << "admin admin Admin" << endl; 
        init.close();
        
        // Re-open in read mode
        file.open("users.txt");
    }

    string u, p, fu, fp, fr;
    cout << "\nUsername: ";
    cin >> u; 
    cout << "Password: ";
    cin >> p;

    // Read the file line by line looking for a match
    while (file >> fu >> fp >> fr) {
        if (u == fu && p == fp) { 
            loggedUser = User(fu, fp, fr); 
            return true; 
        }
    }
    return false;
}

void printReceipt(Buyer &buyer, Weapon* w) {
    cout << "\n========================================\n";
    cout << "           SECURE WEAPONRY\n";
    cout << "          PURCHASE RECEIPT\n";
    cout << "========================================\n";
    cout << "Buyer Name      : " << buyer.name << endl;
    cout << "Age             : " << buyer.age << endl;
    cout << "License Number  : " << buyer.license << endl;
    cout << "Weapon Type     : " << w->getType() << endl;
    cout << "Manufacturer    : " << w->getMaker() << endl;

    if (w->getType() == "Civilian") {
        CivilianWeapon* cw = dynamic_cast<CivilianWeapon*>(w);
        cout << "Magazine Size   : " << cw->getMagazine() << endl;
    } else if (w->getType() == "Melee") {
        MeleeWeapon* mw = dynamic_cast<MeleeWeapon*>(w);
        cout << "Blade Length    : " << mw->getBlade() << endl;
    } else if (w->getType() == "Military") {
        MilitaryWeapon* mw = dynamic_cast<MilitaryWeapon*>(w);
        cout << "Clearance Level : " << mw->getClearance() << endl;
    }

    cout << "Final Price     : " << w->finalPrice() << endl;
    cout << "========================================\n";
    cout << "      THANK YOU FOR YOUR PURCHASE!\n";
    cout << "========================================\n";
}


// Main Function

int main() {
    User current;
    cout << "\n=== Secure Weaponry Login ===\n";
    // Default login for admin: admin / admin
    //Default Login for buyer: sohaib / 1234
    if (!login(current)) { cout << "Login failed.\n"; return 0; }

    cout << "Welcome " << current.username << " (" << current.role << ")\n";

    InventoryManager inventory;
    inventory.loadFromFile();

    int choice;
    while (true) {
        cout << "\n1. Add Weapon (Admin)\n2. Show Inventory\n3. Buy Weapon\n4. Exit\nChoice: ";
        cin >> choice;

        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

        if (choice == 1) {
            if (current.role != "Admin") {
                cout << "Access Denied: Admins only.\n";
                continue;
            }
            int t; cout << "1.Civilian 2.Military 3.Melee: "; cin >> t;
            string mk, sr; double pr; int ex;
            cout << "Maker: "; cin >> mk;
            cout << "Price: "; cin >> pr;
            cout << "Serial: "; cin >> sr;
            cout << "Extra Info (Magazine/Clearance/Blade): "; cin >> ex;

            if (t == 1) inventory.addWeapon(new CivilianWeapon(mk, pr, sr, ex));
            else if (t == 2) inventory.addWeapon(new MilitaryWeapon(mk, pr, sr, ex));
            else if (t == 3) inventory.addWeapon(new MeleeWeapon(mk, pr, sr, ex));
        }
        else if (choice == 2) inventory.showInventory();
        else if (choice == 3) {
            string name, license, serial; int age;
            cout << "\nEnter Buyer Name: "; cin.ignore(); getline(cin, name);
            cout << "Enter Age: "; cin >> age; cin.ignore();
            cout << "Enter License ID (Prefix 'MIL' for military): "; getline(cin, license);
            cout << "Enter Weapon Serial to Buy: "; getline(cin, serial);

            Buyer buyer(name, age, license);
            Weapon* w = inventory.findWeapon(serial);
            
            if (!w) { cout << "Weapon not found.\n"; continue; }

            
            bool authorized = true;

            if (age < 18) {
                cout << "ERROR: Buyer is underage.\n";
                authorized = false;
            }
            else if (w->getType() == "Military") {
                // Strict check for military weapons
                if (license.substr(0, 3) != "MIL") {
                    cout << "ERROR: Military weapons require 'MIL' license prefix.\n";
                    authorized = false;
                }
            }

            if (authorized) {
                printReceipt(buyer, w);
                inventory.removeWeapon(serial);
            } else {
                cout << "Transaction Cancelled.\n";
            }
        }
        else if (choice == 4) { cout << "System closed.\n"; break; }
    }

    return 0;
}
