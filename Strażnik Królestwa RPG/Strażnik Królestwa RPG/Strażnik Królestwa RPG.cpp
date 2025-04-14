#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Kody kolorów ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

//GRACZ==========================================================================================================================================================================
class Postac {
public:
    string imie;
    int hp;
    int max_hp;
    int mana;
    int atak;
    int obrona;
    int poziom;
    int xp;
    int mikstury;

    Postac(string imie, int hp, int mana, int atak, int obrona)
        : imie(imie), hp(hp), max_hp(hp), mana(mana), atak(atak), obrona(obrona), poziom(1), xp(0), mikstury(0) {
    }

    void OtrzymaneObrazenia(int obrazenia) {
        hp -= obrazenia;
        if (hp < 0) hp = 0;
    }

    void leczenie() {
        if (mikstury > 0) {
            hp = max_hp;
            mikstury--;
            cout << RED << "Użyłeś mikstury! Twoje HP zostało przywrócone do " << hp << "." << RESET << "\n";
        }
        else {
            cout << RED << "Nie masz więcej mikstur!" << RESET << "\n";
        }
    }

    void ZdobywajXP(int zdobyte_xp) {
        xp += zdobyte_xp;
        while (xp >= 100) {
            xp -= 100;
            poziom++;
            atak += 5;
            obrona += 3;
            max_hp += 15;
            hp = max_hp;
            cout << BLUE << imie << " awansował na poziom " << poziom << "!" << RESET << "\n";
        }
    }

    bool JestZywy() {
        return hp > 0;
    }
};

//POTWOR=========================================================================================================================================================================
class Potwor {
public:
    string imie;
    int hp;
    int atak;
    int obrona;

    Potwor(string imie, int hp, int atak, int obrona)
        : imie(imie), hp(hp), atak(atak), obrona(obrona) {
    }

    void OtrzymaneObrazenia(int obrazenia) {
        hp -= obrazenia;
        if (hp < 0) hp = 0;
    }

    bool Zyje() {
        return hp > 0;
    }
};

//BOSS===========================================================================================================================================================================
class Boss : public Potwor {
public:
    Boss(string imie, int hp, int atak, int obrona)
        : Potwor(imie, hp, atak, obrona) {
    }

    void RozmowaPrzed() {
        cout << MAGENTA << "Boss: \"Myślisz, że możesz mnie pokonać, śmieszny śmiertelniku? Moja moc nie zna granic!\"" << RESET << "\n";
    }

    void RozmowaPo() {
        cout << MAGENTA << "Boss: \"To nie koniec, zrozum! Wrócę, silniejszy niż kiedykolwiek!\"" << RESET << "\n";
    }
};

//WALKA==========================================================================================================================================================================
void walka(Postac& gracz, Potwor& potwor, int numer_walki) {
    srand(time(0));

    if (numer_walki == 4) {
        gracz.mikstury = 1; // Dodanie mikstury leczenia
        cout << "Otrzymałeś miksturę leczenia! Możesz jej użyć w tej walce.\n";
    }

    while (gracz.JestZywy() && potwor.Zyje()) {
        cout << "\n--- " << potwor.imie << " ---\n";
        cout << RED << "HP przeciwnika: " << potwor.hp << RESET << "\n";
        cout << GREEN << "Twoje aktualne HP: " << gracz.hp << RESET << "\n";

        cout << "\nTura gracza:\n";
        cout << "1. Atak\n";
        if (numer_walki == 4 && gracz.mikstury > 0) {
            cout << "2. Użyj mikstury leczenia (przywraca 100% HP)\n";
        }
        cout << "Wybierz akcję: ";

        int wybor;
        cin >> wybor;

        if (wybor == 1) {
            int obrazenia = max(0, gracz.atak - potwor.obrona);
            potwor.OtrzymaneObrazenia(obrazenia);
            cout << BLUE << "Gracz atakuje " << potwor.imie << " zadając " << obrazenia << " obrażeń." << RESET << "\n";
        }
        else if (numer_walki == 4 && wybor == 2 && gracz.mikstury > 0) {
            gracz.leczenie();
        }
        else {
            cout << RED << "Niepoprawny wybór!" << RESET << "\n";
        }

        if (potwor.Zyje()) {
            int obrazenia = max(0, potwor.atak - gracz.obrona);
            gracz.OtrzymaneObrazenia(obrazenia);
            cout << RED << potwor.imie << " atakuje gracza zadając " << obrazenia << " obrażeń." << RESET << "\n";
        }

        system("pause"); // Pauza aby użytkownik zobaczył wynik tury
        system("cls"); // Czyszczenie po turze
    }

    if (gracz.JestZywy()) {
        cout << GREEN << "Gratulacje! Pokonałeś " << potwor.imie << "!" << RESET << "\n";
        gracz.ZdobywajXP(numer_walki * 100);

        if (numer_walki == 4) {
            cout << MAGENTA << "Mag umiera i mówi: \"Jesteś godzien mej księgi wody! Powiedz magiczne zaklęcie:\"\n";
            cout << "bjsfdiklanklgb;fbjfhke (Magiczne kasztany mocy)" << RESET << "\n";
            system("pause");

            gracz.ZdobywajXP(500000); // Gracz dostaje XP na poziom 5000
            gracz.mikstury = 100; // Otrzymuje 100 mikstur
            system("pause");
            system("cls");

            // Dialog po zabiciu Maga
            cout << BLUE << "Gracz: \"Czuję ogromną moc płynącą z tej księgi... Moje ciało i umysł stały się silniejsze!\"\n";
            cout << "Twoje poziomy wzrosły do 5000. Od teraz Twoja moc jest niewyobrażalnie potężna, ale i odpowiedzialność z mocą rośnie!" << RESET << "\n";
            system("pause");
        }
    }
    else {
        cout << RED << "Niestety, zginąłeś..." << RESET << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "");// Polskie znaki

    Postac gracz("Bohater", 100, 50, 20, 10);
    Potwor potwor1("Ork", 50, 25, 5);
    Potwor potwor2("Ogr", 70, 35, 10);
    Potwor potwor3("Szaman", 100, 40, 20);
    Potwor potwor4("Mag", 200, 50, 30);
    Boss boss("Czarny Władca", 3000, 500, 200);

    cout << "Witaj w grze Strażnik Królestwa RPG!\n";
    cout << BLUE << "Twoja postać: " << gracz.imie << RESET << "\n";
    system("pause");

    walka(gracz, potwor1, 1);
    walka(gracz, potwor2, 2);
    walka(gracz, potwor3, 3);
    walka(gracz, potwor4, 4);

    //WALKA_BOSS=================================================================================================================================================================
    system("cls");
    boss.RozmowaPrzed();
    walka(gracz, boss, 5);

    if (gracz.JestZywy()) {
        system("cls");
        cout << "Gratulacje, pokonałeś bossa!\n";
        cout << MAGENTA << "Boss: \"To nie koniec, wrócę silniejszy!\"" << RESET << "\n";
        cout << BLUE << "Gracz: \"Będę CZEKAŁ HAHAHA.\"" << RESET << "\n";
    }
    else {
        cout << RED << "Niestety, zginąłeś..." << RESET << "\n";
    }

    return 0;
}
