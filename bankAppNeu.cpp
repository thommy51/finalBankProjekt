#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>

using namespace std;

string pin;
string validatePin;
string kontoNr;
string bankBalance;
string recipient;
string amount;

int kontoStart();
int depositMoney(string& s);
int withdrawMoney(string& s);
int getBalance(void* NotUsed, int argc, char** argv, char** azColName);
int balanceQuery();
int transferMoney(string& r, string& b);
int checkKontoNr(string& s);
int getPin(void* NotUsed, int argc, char** argv, char** azColName);
int checkPin(string& s);
int createNewBankAccount(string& ktnr, string& pw);

//PATH TO DATABASE-FILE
const char* dbPath = "C:\\Users\\ThoBr\\source\\databases\\bank.db";

int main()
{
    int choice;
    string createKontoNr;
    string createPin;



    do
    {
        system("cls");
        cout << "1. Einloggen\n";
        cout << "2. Registrieren\n";
        cout << "3. Beenden\n\n";
        cin >> choice;

        if (choice == 1)
        {

            cout << "Geben Sie ihre Kontonr ein: "; cin >> kontoNr;
            checkKontoNr(kontoNr);

            cout << "\nGeben Sie ihre Pin ein: "; cin >> pin;
            checkPin(pin);

        }
        if (choice == 2)
        {
            system("cls");
            cout << "Vergeben Sie eine Kontonr: "; cin >> createKontoNr;
            cout << "Waehlen Sie eine Pin: "; cin >> createPin;

            createNewBankAccount(createKontoNr, createPin);
            system("cls");
            cout << "Registrierung war erfolgreich\n\n";
        }
    } while (choice != 3);



    return 0;
}
int createNewBankAccount(string& ktnr, string& pw) {

    sqlite3* db;
    char* zErrMsg = 0;
    string rc;

    rc = sqlite3_open(dbPath, &db);
    string query = "INSERT INTO user (kontonr, pin) VALUES (" + ktnr + "," + pw + ");";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &zErrMsg);

    sqlite3_close(db);

    return 0;

}
int getPin(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        validatePin = argv[i];
    }
    return 0;
}
int checkKontoNr(string& s) {
    sqlite3* db;
    char* zErrMsg = 0;
    string rc;

    rc = sqlite3_open(dbPath, &db);
    string query = "SELECT pin from user WHERE kontonr = " + s;
    rc = sqlite3_exec(db, query.c_str(), getPin, NULL, &zErrMsg);

    sqlite3_close(db);

    return 0;
}
int checkPin(string& s) {

    if (validatePin == pin)
    {
        system("cls");
        kontoStart();
    }
    else {

        cout << "Die Einlogdaten stimmen nicht!\n\n";
        system("pause");
        exit(0);
    };

    return 0;
}
int kontoStart() {

    int choice;
    string putMoney;
    string takeMoney;

    do {
        system("cls");
        balanceQuery();
        cout << "\nIhr Kontostand betraegt: " << bankBalance;

        cout << "\n\n 1. Moechten Sie Geld einzahlen ?";
        cout << "\n 2. Moechten Sie Geld auszahlen ?";
        cout << "\n 3. Geld ueberweisen.\n\n";
        cout << "\n 4. Um zu verlassen.\n\n";
        cin >> choice;

        if (choice == 1)
        {
            system("cls");

            cout << "Wie viel Geld moechten Sie einzahlen ?: "; cin >> putMoney;

            depositMoney(putMoney);

        }
        else if (choice == 2) {
            system("cls");

            cout << "Wie viel Geld moechten Sie ausbezahlen ?: "; cin >> takeMoney;


            if (takeMoney > bankBalance)
            {
                cout << "\n-- Sie haben nicht genug Guthaben. --\n\n"; system("pause");
            }
            else {
                withdrawMoney(takeMoney);
            }

        }
        else if (choice == 3) {
            system("cls");

            cout << ("Bitte geben Sie die Kontonr des Empfaengers ein: "); cin >> recipient;
            cout << ("Bitte geben Sie den Betrag ein: "); cin >> amount;

            if (amount > bankBalance)
            {
                cout << "\n-- Sie haben nicht genug Guthaben. --\n\n"; system("pause");
            }
            else {
                transferMoney(recipient, amount);
            }

        }
    } while (choice != 4);
    return 0;
}
int balanceQuery() {

    sqlite3* db;
    string rc;

    rc = sqlite3_open(dbPath, &db);

    string query = "SELECT guthaben FROM user WHERE kontonr =" + kontoNr;

    rc = sqlite3_exec(db, query.c_str(), getBalance, NULL, NULL);

    sqlite3_close(db);

    return 0;

}
int getBalance(void* NotUsed, int argc, char** argv, char** azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        bankBalance = argv[i];
        return 0;
    }
}

int depositMoney(string& s) {
    sqlite3* db;
    string rc;

    rc = sqlite3_open(dbPath, &db);

    string query = "UPDATE user SET guthaben = ( guthaben +" + s + ") WHERE kontonr = " + kontoNr;

    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);

    sqlite3_close(db);

    return 0;
}
int withdrawMoney(string& s) {
    sqlite3* db;
    string rc;

    rc = sqlite3_open(dbPath, &db);

    string query = "UPDATE user SET guthaben = ( guthaben -" + s + ") WHERE kontonr = " + kontoNr;

    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);

    sqlite3_close(db);

    return 0;
}
int transferMoney(string& r, string& b) {
    sqlite3* db;
    string rc;

    rc = sqlite3_open(dbPath, &db);

    string query = "UPDATE user SET guthaben = (guthaben - " + b + ") WHERE kontonr = " + kontoNr + "; UPDATE user SET guthaben = (guthaben + " + b + ") WHERE kontonr = " + r + ";";

    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, NULL);

    sqlite3_close(db);

    return 0;
}