#include <iostream>
#include <array>
#include <string>

class Character_Stats {
    float Health, Speed;
    int Mana;
public:
    Character_Stats(float Health_, float Speed_, int Mana_) : Health(Health_), Speed(Speed_), Mana(Mana_) {}

};

class Tool {
    std::string Name;
    float Damage, Cooldown;
    int Critical_Chance;
public:
    Tool(const std::string& name_, float Damage_, float Cooldown_, int Critical_Chance_)
        : Name(name_), Damage(Damage_), Cooldown(Cooldown_), Critical_Chance(Critical_Chance_) {}
    float DamageCalculation() {
        int Chance = rand() % 100;
        if (Chance < Critical_Chance) {
            return Damage + 0.5 * Damage;
        }
        return Damage;
    }
};

class Character {
    std::string Name;
    Character_Stats Stats{10, 5, 5 };
    Tool Weapon{"Sword", 3, 0.6, 25};
public:
    explicit Character(const std::string& name_) : Name(name_) {}
    Character(const Character& other)
        : Name(other.Name), Stats(other.Stats), Weapon(other.Weapon) {}
    ~Character() {
        std::cout << Name << " Destroyed\n";
    }
    Character& operator=(const Character& other) {
        Name = other.Name;
        Stats = other.Stats;
        Weapon = other.Weapon;
        return *this;
    }
};

class Enviroment_Object {
    bool isDestructable;
    float Health;
public:
    Enviroment_Object(bool isDestructable_, float Health_) : isDestructable(isDestructable_), Health(Health_) {}
};


int main() {
    srand(time(0));
    // std::cout << "Hello, world!\n";
    // std::array<int, 100> v{};
    // int nr;
    // std::cout << "Introduceți nr: ";
    // /////////////////////////////////////////////////////////////////////////
    // /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
    // /// dați exemple de date de intrare folosind fișierul tastatura.txt
    // /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
    // /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
    // /// De asemenea, trebuie să adăugați în acest fișier date de intrare
    // /// pentru cât mai multe ramuri de execuție.
    // /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
    // /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
    // ///
    // /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
    // /// pentru a simula date introduse de la tastatură.
    // /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
    // ///
    // /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
    // /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
    // /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
    // /// program care merg (și să le evitați pe cele care nu merg).
    // ///
    // /////////////////////////////////////////////////////////////////////////
    // std::cin >> nr;
    // /////////////////////////////////////////////////////////////////////////
    // for(int i = 0; i < nr; ++i) {
    //     std::cout << "v[" << i << "] = ";
    //     std::cin >> v[i];
    // }
    // std::cout << "\n\n";
    // std::cout << "Am citit de la tastatură " << nr << " elemente:\n";
    // for(int i = 0; i < nr; ++i) {
    //     std::cout << "- " << v[i] << "\n";
    // }
    // ///////////////////////////////////////////////////////////////////////////
    // /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
    // /// alt fișier propriu cu ce alt nume doriți.
    // /// Exemplu:
    // /// std::ifstream fis("date.txt");
    // /// for(int i = 0; i < nr2; ++i)
    // ///     fis >> v2[i];
    // ///
    // ///////////////////////////////////////////////////////////////////////////
    return 0;
}
