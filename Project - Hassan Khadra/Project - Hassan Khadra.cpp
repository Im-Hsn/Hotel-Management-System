#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<istream>
#include<sstream>
#include<fstream>
#include<string>
#include<cstring>
#include<cmath>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include<cwchar>
#include"md5.h"
#include"md5.cpp"
#include "PDFWriter.cpp"
using namespace std;

struct date {
    int day;
    int month;
    int year;
};

struct room {
    int num;
    string address;
    string type;
    double price;
    string* option;
    date start_date;
    date end_date;
};

struct client {
    int ID;
    string firstname;
    string lastname;
    string password;
    string address;
    string tel;
    string priv;
    room* r;
};
 
void fullscreen() //To launch program in full screen
{
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1c, 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}

void UI() {
    fullscreen();
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 400); //To change UI color
    system("mode 650");
    static CONSOLE_FONT_INFOEX  fontex;
    fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetCurrentConsoleFontEx(hOut, 0, &fontex);
    fontex.FontWeight = 610;
    fontex.dwFontSize.X = 36;
    fontex.dwFontSize.Y = 36;
    SetCurrentConsoleFontEx(hOut, NULL, &fontex); // To change UI font

    cout << "\n\n\n\n\n\n\n\n\t\t\t\t\tWELCOME TO HOTEL\n\n\n\t\t\t\t ";
    system("pause"); //To pause program until a key is pressed
    system("cls"); //To clear the previous output
    
}

bool checkdigit(client c) {
    for (int i = 0; i < c.password.length(); i++) {
        if (isdigit(c.password[i])) return true;
    }return false;
}

bool checkalpha(client c) {
    for (int i = 0; i < c.password.length(); i++) {
        if (isalpha(c.password[i])) return true;
    }return false;
}

bool checkspecial(client c) {
    for (int i = 0; i < c.password.length(); i++) {
        if (isalnum(c.password[i]) == 0 && c.password[i] > 32) return true;
    }return false;
}

bool passcheck(client c) {
    if ((c.password.length() >= 8) &&
        (checkdigit(c)) &&
        (checkalpha(c)) &&
        (checkspecial(c)))return true;
    return false;
}

bool emailcheck(string c) {
    int i = 0, a = c.find("@"), b = c.find(".");
    if ((isalnum(c[0])) && (c.at(c.length() - 1) != '.'))i++; //first char has to be alpha or num and last can't be '.'
    if ((a != string::npos) && (b != string::npos))i++; //to check if '@' and '.' exist

    for (int j = 0; j < c.length(); j++) {
        if (c[j] == '@')a = j;
        else if (c[j] == '.') b = j;
    }if (a < b)i++;   // '.' must be after the '@'

    if (i == 3) return true;
    return false;
}

bool numcheck(client c) {
    int i;
    for (i = 0; i < c.tel.length(); i++) {
        if (isdigit(c.tel[i]) == 0) break;
    }if ((c.tel.length() == 8) && (i == c.tel.length()))return true;
    return false;
}

void userinput(client c) {
    system("cls");
        if (c.address.length() > 0)
        {
            cout << "New account creation in process\n\nFirst name entered:" << c.firstname << "\n\n"
                << "Last name entered:" << c.lastname << "\n\nEmail address entered:" << c.address << "\n\n";
        } 
        if (c.tel.length() > 0) {
            cout << "Phone number entered:" << c.tel << "\n\n";
        }  
}

void type(client& cp) {
    char a = 0;
    do {
        system("cls");
        cout << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   0:   For administrator"
            << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   1:   For client"
            << "\n\t\t\t------------------------------------------------\t\t\t\t";
        a = _getch();
    } while (a < '0' || a> '1');
    if (a == '1')cp.priv = "client";
    else if (a == '0')cp.priv = "admin";
}

bool out(string back) {
    if (back == "0")return true;
    return false;
}

void name(client& c, char* back) {
    do {
        system("cls");
        cout << "New account creation in process (Enter '0' anywhere to exit)\n\n"
            << "First and last name must both contain at least 2 characters\n\n\nPlease enter your first name:";
        getline(cin, c.firstname); if (out(c.firstname)) { *back = '0'; return; }

        cout << endl;
        cout << "Please enter your last name:";
        getline(cin, c.lastname); if (out(c.lastname)) { *back = '0'; return; }
    } while ((c.firstname.length() <= 1) || (c.lastname.length() <= 1));
}

bool ifexist(string e) {
    string str, adr;
    fstream file("client.csv"); //initialising
    while (file) {
        getline(file, str, ',');
        getline(file, str, ',');
        getline(file, str, ',');
        getline(file, str, ',');
        getline(file, adr, ','); //email address
        getline(file, str, '\n');
        if (adr == e)return true;
    }file.close();
    return false;
}

void adr_num(client& c, char* back) {
    int i = 0, j = 0;
    cout << "\nPlease enter your email address:";
    do {
        if (j > 0)cout << "\nAn account associated with this email address already exists\nPlease re-enter your email address:";
        do {
            if (i > 0) cout << "\nInvalid email. Please re-enter your email address:";
            getline(cin, c.address); if (out(c.address)) { *back = '0'; return; }
            userinput(c);
            i++;
        } while (emailcheck(c.address) == false);
        j++; i = 0;
    } while (ifexist(c.address));
    cout << "\nPlease enter your phone number:";
    do {
        if (i > 0)cout << "\nInvalid phone number. Please re-enter your phone number:";
        getline(cin, c.tel); if (out(c.tel)) { *back = '0'; return; }
        userinput(c);
        i++;
    } while (numcheck(c) == 0);
}

void pass(client& c, char* back) {
    int i = 0;
    cout << "\nPlease enter your password (must contain at least 8 characters without spaces, while containing \n"
        << "numbers, letters, and special characters):";
    do {
        if (i >= 1) {
            cout << "Please re-enter your password following the conditions mentioned below\n"
                << "(must contain at least 8 characters without spaces, while containing numbers, letters, and \nspecial characters):";
        }
        getline(cin, c.password); if (out(c.password)) { *back = '0'; return; }
        userinput(c);
        i++;
    } while (passcheck(c) == false);
    cout << "\n\nAccount has been succesfully created";
    Sleep(2500); //To pause program for 2.5 seconds
}

int idcount() {
    string str;
    int ID = -1 , row = 0;
    fstream file("client.csv");
    while (file) {
        file >> ID;
        getline(file, str, '\n'); //to finish the line
        row++;
    }file.close();
    if (ID == -1) return 1;
    else return row;
}

int roomcount() {
    string str;
    int row = 0;
    fstream file("room.csv");
    while (file) {
        getline(file, str, '\n'); //to finish the line
        row++;
    }file.close(); return row - 1;
}

void add(client c) {
    fstream file("client.csv", ios::out | ios::app);
    file << c.ID << "," << c.firstname << "," << c.lastname << "," <<
        md5(c.password) << "," << c.address << "," << c.tel << "," << c.priv << "\n";
    file.close();
}

bool checkf(fstream &f) { //To check if the file is empty
    string* s = new string;
    getline(f, *s, '\n');
    if ((*s).length() < 1) { delete s; return true; }delete s;
    return false;
}

void addroom(room r) {
    fstream file("room.csv", ios::out | ios::app);
    file << r.num << "," << r.address << "," << r.type << "," <<
        "$" << r.price << "," << *(r.option) << "\n";
    file.close();
}

void roomcout(room r, int i, string a, string p) {
    if (i == 0) {
            cout << "\n-------------------------------------------------------------------------------------------------"
            << "\n|  Number   |      Address      |      Type      |   Price   |       Additional features        |"
            << "\n-------------------------------------------------------------------------------------------------";
    }
    cout << "\n|" << setw(6) << r.num << "     |" << setw(12) << r.address << "       |" << setw(11) << r.type << "     |" << setw(7) << p << "    |";
    if (a.length() > 19) cout << setw(32) << a << "  |";
    else cout << setw(19) << a << "\t\t|";
    cout << "\n-------------------------------------------------------------------------------------------------";
}

void availroom() {
    system("cls");
    room r;
    string a, p;
    int i = 0, j = -1;
    fstream file("room.csv");
    if (checkf(file)) { file.close(); cout << "\n\nNo rooms available"; Sleep(2000); return; }
    while (file) {
        if (i > 0)j = r.num;
        file >> r.num; if (r.num == j)break;
        file.ignore(1);
        getline(file, r.address, ',');
        getline(file, r.type, ',');
        getline(file, p, ',');
        getline(file, a, '\n');
        if (i == 0) cout << "\n\t\t\t\t\tHotel rooms:\n\n"; roomcout(r, i, a, p);
        i++;
    }cout << "\n"; file.close(); system("pause");
}

bool roomcheck(int n) {
    string* s = new string;
    int num = -1;
    fstream file("room.csv");
    while (file) {
        file >> num;
        getline(file, *s, '\n'); //to finish the line
        if (num == n) { delete s; return true; }
    }file.close(); delete s;
    return false;
}

void readroom(fstream &f, int &i, room* rp, string *str) {
    char a = 36; // "$" sign
    string* s = new string;
    if (i > 0)i = rp->num;
    f >> rp->num;
    f.ignore(1);
    getline(f, rp->address, ',');
    getline(f, rp->type, ',');
    getline(f, *s, a);
    f >> rp->price;
    f.ignore(1);
    getline(f, *str, '\n');
    delete s;
}

void deleteroom() {
    int n;
    system("cls");
    cout << "\n\nPlease enter the number of the room:";
    cin >> n;
    if (roomcheck(n) == 0) { cout << "\n\nRoom does not exist"; Sleep(2000); return; }
    room r;
    room* rp = &r;
    int i = 0;
    r.option = new string;
    fstream f("room.csv"), file("newroom.csv", ios::out | ios::app);
    while (f) {
        readroom(f, i, rp, r.option);
        if (rp->num == i) break; i = 0; //To break if it repeats on the last row
        if (rp->num != n) {
            file << r.num << "," << r.address << "," << r.type << "," <<
                "$" << r.price << "," << *(r.option) << '\n'; i++;
        }
    }f.close(); file.close(); delete r.option;
    remove("room.csv");
    rename("newroom.csv", "room.csv");
    cout << "\n\nRoom has been successfully deleted";
    Sleep(2000);
}

void modify(room r) {
    int n;
    system("cls");
    cout << "\n\nPlease enter the number of the room:";
    cin >> n;
    if (roomcheck(n) == 0) { cout << "\n\nRoom does not exist"; Sleep(2000); return; }
    room* rp = &r;
    int i = 0, c = -1, j = 0;
    r.option = new string;
    char choice;
    int y = roomcount();
    room* rs = new room[y];
    for (int k = 0; k < y; k++) rs[k].option = new string;
    fstream f("room.csv"), file("new.csv", ios::out | ios::app);
    while (f) {
        readroom(f, i, rp, r.option); c++;
        if (rp->num == n) break;
    }
    f.clear();
    f.seekg(0); //To get back to pos 0 in file
    while (f) {
        readroom(f, i, rp, r.option); if (rp->num == i) break; i = 0; //To prevent the loop from reading the same last room twice
        rs[j].num = r.num; rs[j].address = r.address; rs[j].type = r.type;
        rs[j].price = r.price; *(rs[j].option) = *(r.option); j++; i++;
    }j = 0;
    do {
        system("cls");
        cout << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   1:   To modify room number"
            << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   2:   To modify address"
            << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   3:   To modify type"
            << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   4:   To modify price"
            << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   5:   To modify additional features"
            << "\n\t\t\t------------------------------------------------\t\t\t\t";
        choice = 0;
        choice = _getch();
        cin.ignore(1);
    } while (choice != '1' && choice != '2' && choice != '3' && choice != '4' && choice != '5');
        if (choice == '1') { cout << "\n\nEnter new number:"; cin >> rs[c].num; }
        else if (choice == '2') { cout << "\n\nEnter new address:"; getline(cin, rs[c].address); }
        else if (choice == '3') { cout << "\n\nEnter new type:"; getline(cin, rs[c].type); }
        else if (choice == '4') { cout << "\n\nEnter new price:"; cin >> rs[c].price; }
        else if (choice == '5') { cout << "\n\nEnter new additional features.\n(separated with comma):"; getline(cin, *(rs[c].option)); }
    while (file) {
        if (j == y)break;
        file << rs[j].num << "," << rs[j].address << "," << rs[j].type << "," <<
            "$" << rs[j].price << "," << *(rs[j].option) << '\n'; j++;
    }
    f.close(); file.close(); delete r.option, rs; for (int k = 0; k < y; k++) delete rs[k].option;
    remove("room.csv");
    rename("new.csv", "room.csv");
    cout << "\n\nRoom has been successfully modified";
    Sleep(2000);
}

void roominput(room r, int i) {
    system("cls"); cout << "New room creation in process (Enter '0' anywhere to exit)\n\n";
    if (r.num != NULL) cout << "\n\nRoom number entered:" << r.num;
    if (r.address.length() > 1) {
        cout << "\n\nRoom address entered:" << r.address;
        if (r.type.length() > 1) {
            cout << "\n\nRoom type entered:" << r.type;
            if (r.price > 0) cout << "\n\nRoom price entered:" << "$" << r.price;

            if (((*(r.option)).length() > 1) && (*(r.option)).find(",") == string::npos) {
                cout << "\n\nAdditional room features entered:";
                if (i == 0) cout << *(r.option);
            }
        }
    }
}

void getroom(room& r) {
    int i = 0;
    string opt, s;
    r.option = &opt;
    char a = 0; r.num = NULL; r.price = NULL; r.type.clear(); r.address.clear(); (*(r.option)).clear(); //starting with null to prevent re-reading the second time
    system("cls"); roominput(r, 1); cout << "\n\nEnter room number:";
    do {
        if (i > 0)cout << "\n\nRoom already exists. Please enter another room number:";
        while (1) {
            if (cin >> r.num) break;// valid number        // Loop to check if input is numerical
            else { roominput(r, 1); cout << "\n\nInvalide number, please re-enter room number:"; cin.clear(); while (cin.get() != '\n'); } // empty loop
        } i++; system("cls"); if (r.num == 0)return;
    } while (roomcheck(r.num)); roominput(r, 1); i = 0;
    cin.ignore(INT_MAX, '\n');
    cout << "\n\nEnter room address:";
    do {
        if (i > 0) cout << "\n\nPlease re-enter room address.\n(must contain at least 2 charachters):";
        getline(cin, r.address); if (out(r.address))return; i++; roominput(r, 1);
    } while (r.address.length() < 2); i = 0;
    cout << "\n\nEnter room type:";
    do {
        if (i > 0)cout << "\n\nPlease re-enter room type.\n(must contain at least 2 charachters):";
        getline(cin, r.type); if (out(r.type))return; i++; roominput(r, 1);
    } while (r.type.length() < 2); i = 0;
    cout << "\n\nEnter room price:";
    do {
        if (i > 0) cout << "\n\nPlease re-enter room price:";
        while (1) {
            if (cin >> r.price)break; // valid number        // Loop from Google to check if input is numerical
            else { roominput(r, 1); cout << "\n\nInvalide number, please re-enter room price:"; cin.clear(); while (cin.get() != '\n'); } // empty loop
        } if (r.price == 0)return; roominput(r, 1); i++;
    } while (r.price <= 0); i = 0;
    cout << "\n\nWould you like to add additional features? (Y / N)";
    a = _getch();
    if (a == 'Y' || a == 'y') {
        do {
            a = 0;
            do {
                if ((i > 0) && ((*(r.option)).length() > 1) &&     // to add the additional features together if the number exceeds 1
                    (*(r.option)).find(",") == string::npos) s = *(r.option) + ",";

                cout << "\n\nEnter 1 additional feature\n(Must contain at least 2 characters and cannot contain ','):";
                getline(cin, *(r.option)); if (out(*(r.option)))return; roominput(r, i);
            } while (((*(r.option)).length() < 2) || (*(r.option)).find(",") != string::npos);

            if (i > 0) { *(r.option) = s + *(r.option); cout << *(r.option); s = *(r.option) + ","; } //to put the additional features in *(r.option) and cout them if they exceed 1
            cout << "\n\nDo you want to enter another additional feature? (Y / N)";
            a = _getch(); i++;
        } while (a == 'Y' || a == 'y');
    }
    addroom(r);
    cout << "\n\nRoom has been successfully added";
    Sleep(2500);
}

void compare(string e, string p, char& priv, client *c) {
    system("cls");
    int x, y = (idcount() + 6);
    priv = '0';
    string a;
    string* arr = new string [y];  //Dynamically allocating array of strings to save space
    fstream file("client.csv");
    while (file) { x = 0;
        getline(file, a);
        stringstream s(a);  //to cut the line into different strings using comma as delimiter
        while (getline(s, arr[x], ',')) {
            if ((arr[3] == md5(p)) && (arr[4] == e)) {
                c->ID = stoi(arr[0]); c->firstname = arr[1]; c->lastname = arr[2];
                system("cls");
                if (arr[6] == "client") {
                    cout << "\n\nClient authenticated"; priv = '1';
                }
                else if (arr[6] == "admin") {
                    cout << "\n\nAdministrator authenticated"; priv = '2';
                }
            }
            x++;
        }if (priv == '1' || priv == '2') { Sleep(2000); break; }
    }file.close();
    delete[] arr;
    if (priv == '0') {
        cout << "\n\nUnauthenticated user, email or password does not match";
        Sleep(2000);
    }
}

void close() {
    system("cls");
    cout << "\n\n";
    cout << "\n\t\t\t     ----------------------------------------\n";
    cout << "\t\t\t\t  THANKS FOR USING THIS SOFTWARE";
    cout << "\n\t\t\t     ----------------------------------------\n";
    cout << "\n\n";
    exit(0);}

void emailpass(string &em, string &pa) {
    int i = 0;
    system("cls");
    cout << "\n\nPlease enter your email address:";
    do {
        if (i > 0) { system("cls"); cout << "\n\nInvalid email. Please re-enter your email address:"; }
        getline(cin, em); i++;
    } while (emailcheck(em) == 0); system("cls"); cout << "\n\nEmail entered:" << em;
    cout << "\n\nPlease enter your password:";
    getline(cin, pa);
    system("cls");
}

void admin(room &r) {
    char choice = 0;
    do {
        system("cls");
        cout << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   1:   To add room"
        << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   2:   To delete a room"
        << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   3:   To modify the data of a room"
        << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   4:   To check Hotel rooms"
        << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   5:   To exit program"
        << "\n\t\t\t------------------------------------------------\t\t\t\t";
        choice = _getch(); //function takes single character as an input (doesn't need "Enter" key to be pressed)
        switch (choice)
        {
        case '1':
            getroom(r);
            break;
        case '2':
            deleteroom();
            break;
        case '3':
            modify(r);
            break;
        case '4':
            availroom();
            break;
        case '5':
            close();
            break;
        default:
            system("cls");
            cout << choice << " is not a Valid Choice, please choose again \n\n";
            break;
        }
    } while (choice); //To get back to menu after break
}

void dateinput(room r) {
    system("cls"); cout << "New reservation in process (Enter '0' anywhere to exit)\n\n";
        if (r.start_date.day != NULL) cout << "\n\nStarting day entered:" << r.start_date.day;
        if (r.start_date.month != NULL)cout << "\n\nStarting month entered:" << r.start_date.month;
        if (r.start_date.year != NULL) cout << "\n\nStarting year entered:" << r.start_date.year;
        if (r.end_date.day != NULL) cout << "\n\n\nEnding day entered:" << r.end_date.day;
        if (r.end_date.month != NULL)cout << "\n\nEnding month entered:" << r.end_date.month;
        if (r.end_date.year != NULL) cout << "\n\nEnding year entered:" << r.end_date.year;
}

void checkint(room r, int &n) {
    while (1) {
        if (cin >> n)break; // valid number        // Loop to check if input is numerical
        else { dateinput(r); cout << "\n\nInvalide number, please re-enter:"; cin.clear(); while (cin.get() != '\n'); }
    }
}

void getdate(room& r, char& b) {
    r.start_date.day = NULL; r.start_date.month = NULL; r.start_date.year = NULL;
    r.end_date.day = NULL; r.end_date.month = NULL; r.end_date.year = NULL; dateinput(r);
    cout << "\n\nPlease enter starting day:"; checkint(r, r.start_date.day); if (r.start_date.day == 0) { b = 0; return; } dateinput(r); cin.ignore(INT_MAX, '\n');
    cout << "\n\nPlease enter starting month:"; checkint(r, r.start_date.month); if (r.start_date.month == 0) { b = 0; return; } dateinput(r); cin.ignore(INT_MAX, '\n');
    cout << "\n\nPlease enter starting year:"; checkint(r, r.start_date.year); if (r.start_date.year == 0) { b = 0; return; } dateinput(r); cin.ignore(INT_MAX, '\n');
    cout << "\n\n\nPlease enter ending day:"; checkint(r, r.end_date.day); if (r.end_date.day == 0) { b = 0; return; } dateinput(r); cin.ignore(INT_MAX, '\n');
    cout << "\n\nPlease enter ending month:"; checkint(r, r.end_date.month); if (r.end_date.month == 0) { b = 0; return; } dateinput(r); cin.ignore(INT_MAX, '\n');
    cout << "\n\nPlease enter ending year:"; checkint(r, r.end_date.year); if (r.end_date.year == 0) { b = 0; return; } dateinput(r); cin.ignore(INT_MAX, '\n');
}

void yourroom(room r[], int i) {
    if (i == 0) {
        cout << "\n-------------------------------------------------------------------------------------------------"
            << "\n|  Number   |      Address      |      Type      |   Price   |       Additional features        |"
            << "\n-------------------------------------------------------------------------------------------------";
    }
    cout << "\n|" << setw(6) << r[i].num << "     |" << setw(12) << r[i].address <<
        "       |" << setw(11) << r[i].type << "     |" << setw(7) << r[i].price << "$" << "   |";
    if ((*(r[i].option)).length() > 19) cout << setw(32) << *(r[i].option) << "  |";
    else cout << setw(19) << *(r[i].option) << "\t\t|";
    cout << "\n-------------------------------------------------------------------------------------------------";
}

void toroom(fstream& file, room rs[], room* rp, int num[], int y, int &j) {
    int i = 0, b; file.clear(); file.seekg(0);
    if (num[0] != NULL) while (file) { b = 0;
    readroom(file, i, rp, rp->option); if (rp->num == i) break; i = 0; for (int k = 0; k < y; k++) { if (rp->num == num[k]) { b = 1; break; }
    }if (b == 1) {
        rs[j].num = rp->num; rs[j].address = rp->address; rs[j].type = rp->type;
        rs[j].price = rp->price; *(rs[j].option) = *(rp->option); i++; j++;
    }
    }
    else while(file) {
        readroom(file, i, rp, rp->option); if (rp->num == i) break; i = 0; //To prevent the loop from reading the same last room twice
        rs[j].num = rp->num; rs[j].address = rp->address; rs[j].type = rp->type;
        rs[j].price = rp->price; *(rs[j].option) = *(rp->option); j++; i++;
    }
}

bool rchoice(room r[], client &c, int n) {
    for (int i = 0; i < roomcount(); i++) { // to make sure the user picks a room available for them
        if (n == r[i].num) { c.r->num = r[i].num; c.r->address = r[i].address; c.r->type = r[i].type;
            c.r->price = r[i].price; *(c.r->option) = *(r[i].option); return true; } //the data of the room to is stored in the room structure
    }return false;
}

void roomchoice(room rs[], client &c) {
    int n = 0;
    do { cout << "\nPlease enter room number to be reserved:"; cin >> n; } while (rchoice(rs, c, n) == false);
    fstream f("reservation.csv", ios::out | ios::app);
    f << n << "," << c.ID << "," << c.firstname << "," << c.lastname << "," << c.r->start_date.day << "-" <<
        c.r->start_date.month << "-" << c.r->start_date.year << "," << c.r->end_date.day << "-" << c.r->end_date.month <<
        "-" << c.r->end_date.year << '\n';
    system("cls"); cout << "\n\nRoom has been reserved successfully"; Sleep(2000); f.close();
}

void resroom(client* c) { char* back = new char; *back = '1';
getdate(*c->r, *back); if (*back == 0)return; delete back;
    int y = roomcount(), j = 0, i = 0, b; c->r->option = new string;
    room* rp = c->r; int* num = new int[y]; int* n = new int[y]; num[0] = NULL;
    room* rs = new room[y]; for (int k = 0; k < y; k++) rs[k].option = new string;
    int* start = new int; int* end = new int; int* ye = new int; int* m = new int;
    string* str = new string;
    fstream f("reservation.csv");
    if (checkf(f)) {
        fstream file("room.csv"); if (checkf(file)) { f.close(); file.close();
        delete start, end, ye, m, str; rp->option; for (int k = 0; k < y; k++) delete rs[k].option; delete[]num, rs, n;
        cout << "\n\nNo rooms available"; return; }
        else { toroom(file, rs, rp, num, y, j); file.close(); }
    }
    else {
        *start = rp->start_date.day + (rp->start_date.month * 30) + (rp->start_date.year * 12 * 30); f.clear(); f.seekg(0);
        while (f) {
            f >> n[j];//num
            f.ignore(1); if (n[j] < 0)break;
            getline(f, *str, ','); // user ID
            getline(f, *str, ','); //fname
            getline(f, *str, ','); //lname
            getline(f, *str, ','); //start date
            f >> *end; getline(f, *str, '-');
            f >> *m; getline(f, *str, '-');
            f >> *ye; getline(f, *str, '\n');
            *end += (*ye * 12 * 30) + (*m * 30);
            if (*end <= *start) { num[i] = n[j]; i++; } j++; //saving room numbers in num array
        }fstream file("room.csv"); y = j; j = i; i = 0;
        while (file) {
                b = 0;
                readroom(file, i, rp, rp->option); if (rp->num == i) break; i = 0; i++; //to check if the room file has rooms that are available
                for (int k = 0; k < y; k++) { if (rp->num == n[k]) { b = 1; break; } } if (b == 0) { num[j] = rp->num; j++; }
            }y = j; j = 0; toroom(file, rs, rp, num, y, j); file.close();
    }system("cls"); f.close();
    for (y = 0; y < j; y++) { if (y == 0) cout << "\n\t\t\t\tHotel rooms available for you:\n\n"; yourroom(rs, y); } roomchoice(rs, *c); for (int k = 0; k < roomcount(); k++) delete rs[k].option;
    delete c->r->option, rs, end, start, ye, m, str; delete[]num, n;
}

string tol(string s) {
    string a;
    for (int i = 0; i < s.length(); i++) { //function to convert string to lowercase string
        if (isupper(s[i])) a += tolower(s[i]);
        else a += s[i];
    } return a; }

string zero(string s) {
    for (int i = s.length()-1; i > -1; i--) {
        if (s[i] == '0')s = s.substr(0, i);
        if (s[i] == '.') { s = s.substr(0, i); return s; }
    } }

void PDF(client c, room rn[], room* r, int size, double charge) {
    pdfwrite::PDFWriter pdf; pdf.setWidthHeight(1000, 1000); string e; room* ar = new room[size]; for (int i = 0; i < size; i++)ar[i].option = new string;
    pdf.pdfSetFont(pdfwrite::TIMES_ITALIC, 20); string* s = new string; string name = c.firstname + " " + c.lastname+ " "; room temp; temp.option = new string;
    pdf.pdfSetContent(name); pdf.warpText(100, true); pdf.setXY(450, 800); pdf.pdfAddToPage(); if (!pdf.writeToFile("Invoice.pdf", e));
    if (size < 1) {
        e = "$" + zero(to_string(r->price));
        *s = to_string(r->num) + " " + r->address + " " + r->type + " " + e + " " + *(r->option) + " ";
        pdf.pdfSetContent(*s); pdf.warpText(800, true); pdf.setXY(100, 700); pdf.pdfAddToPage(); if (!pdf.writeToFile("Invoice.pdf", e)); }
    else {
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                if (rn[i].price > rn[j].price)
                {   temp = rn[i];
                    rn[i] = rn[j];
                    rn[j] = temp; } } }
        for (int i = 0; i < size; i++) {
            e = "$" + zero(to_string(rn[i].price));
            *s = to_string(rn[i].num) + " " + rn[i].address + " " + rn[i].type + " " + e + " " + *(rn[i].option) + " "; 
            pdf.pdfSetContent(*s); pdf.warpText(800, true); pdf.setXY(100, 700); pdf.pdfAddToPage(); if (!pdf.writeToFile("Invoice.pdf", e));
        } } *s = "Total charges to be paid: $" + zero(to_string(charge));
    pdf.pdfSetContent(*s); pdf.warpText(100, true); pdf.setXY(450, 800); pdf.pdfAddToPage(); if (!pdf.writeToFile("Invoice.pdf", e));
    delete temp.option, s; for (int i = 0; i < size; i++)delete ar[i].option; delete[]ar;
}

void checkout(room rn[], room* rp, int size, int d[], client c) { string s = to_string(rp->price);
system("cls"); string a = "$" + zero(s), op = *(rp->option); double* charge = new double;
    for (int k = 0; k < size; k++) { if (k == 0) cout << "\n\t\t\t\t\tYour reserved rooms:\n"; yourroom(rn, k); }
    cout << "\n\n\t\t\t\t\tYour cancelled room:\n"; roomcout(*rp, 0, op, a);
    if ((tol(op).find("free cancellation")) != string::npos) { *charge = 0; cout << "\n\nYour room has a free cancellation feature.\nTotal charges:$0"; }
    else {
        *charge = d[0] + (d[1] * 30) + (d[2] * 30 * 12); d[6] = d[3] + (d[4] * 30) + (d[5] * 30 * 12);
        *charge = d[6] - *charge; *charge *= rp->price; *charge += (*charge * 0.11); cout << "\nYour total charges to be paid:$" << *charge; }
    PDF(c, rn, rp, size, *charge);
    cout << "\n\nYour invoice has been generated as a PDF. Thank you."; Sleep(10000); delete charge;
}

void cancelres(client* c) { system("cls");
    fstream f("reservation.csv");
    int y = roomcount(), a = 0, j = 0, n, b, i; c->r->option = new string; int* date = new int[6];
    room* rp = c->r; int* num = new int[y]; string* s = new string; string* st = new string;
    room* rs = new room[y]; for (int k = 0; k < y; k++) rs[k].option = new string;
    string* str = new string;
    while (f) { if (a > 0)a = rp->num;
        f >> rp->num;//num
        f.ignore(1); if (a == rp->num)break; a = 0;
        f >> n; // user ID
        f.ignore(1); if (n == c->ID) { num[j] = rp->num; j++; }
        getline(f, *str, '\n'); a++; //fname
    }n = j; j = 0; f.clear(); f.seekg(0); fstream f1("room.csv"); a = 0;
    while (f1) {
        b = 0;
        readroom(f1, a, rp, rp->option); if (rp->num == a) break; a = 0; for (int k = 0; k < n; k++) if (num[k] == rp->num) b = 1;
        if (b == 1) { rs[j].num = rp->num; rs[j].address = rp->address; rs[j].type = rp->type;
            rs[j].price = rp->price; *(rs[j].option) = *(rp->option); j++; a++; }
    }f1.close(); for (int k = 0; k < j; k++) { if (k == 0) cout << "\n\t\t\t\t      Your reserved rooms:\n\n"; yourroom(rs, k); }
    do { cout << "\nPlease enter room number to be cancelled:"; if (cin >> n) continue; else { cin.clear(); while (cin.get() != '\n'); return; } } while (rchoice(rs, *c, n) == false); a = 0;
    room* rn = new room[j]; for (int k = 0; k < j; k++) rn[k].option = new string; for (int k = 0; k < j; k++) {
        if (rs[k].num != n) {
            rn[a].num = rs[k].num; rn[a].address = rs[k].address; rn[a].type = rs[k].type;
            rn[a].price = rs[k].price; *(rn[a].option) = *(rs[k].option); a++;
        }
    }j = a;
    for (int k = 0; k < y; k++) delete rs[k].option; delete[]rs; //removing the room to be cancelled from the structure
    fstream file("newres.csv", ios::out | ios::app); a = 0;
    while (f) { if (a > 0)a = i; y = f.tellg();
    getline(f, *s, '\n');
    f.seekg(y); f >> i; f.ignore(1); f >> b; f.ignore(1); getline(f, *st, '\n'); if (i == a) break; a = 0; //To break if it repeats on the last row
    if ((i == n) && (b == c->ID)) {
        f.seekg(y); getline(f, *st, ','); getline(f, *st, ','); getline(f, *st, ','); getline(f, *st, ',');
    f >> date[0]; getline(f, *st, '-'); f >> date[1]; getline(f, *st, '-'); f >> date[2]; getline(f, *st, ',');
        f >> date[3]; getline(f, *st, '-'); f >> date[4]; getline(f, *st, '-'); f >> date[5]; getline(f, *st, '\n'); }
    else { file << *s << '\n'; }a++; }f.close(); file.close();
    remove("reservation.csv");
    rename("newres.csv", "reservation.csv"); system("cls");
    cout << "\n\nRoom has been successfully cancelled"; delete[] num; delete str, s;
    Sleep(2000);
    checkout(rn, rp, j, date, *c);
    delete[] rn, date; delete rp->option, rp;
}

void Client(client* c) {
    char choice = 0;
    do {
        system("cls");
        cout << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   1:   To reserve a room"
            << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   2:   To cancel a reservation"
            << "\n\t\t\t------------------------------------------------"
            << "\n\t\t\tPRESS   3:   To exit program"
            << "\n\t\t\t------------------------------------------------\t\t\t\t";
        choice = _getch(); //function takes single character as an input (doesn't need "Enter" key to be pressed)
        switch (choice)
        {
        case '1':
            resroom(c);
            break;
        case '2':
            cancelres(c);
            break;
        case '3':
            close();
            break;
        default:
            system("cls");
            cout << choice << " is not a Valid Choice, please choose again \n\n";
            break;
        }
    } while (choice); //To get back to menu after break
}

void case2(client cptr) {
    char ans = 0;
    do {
        char* back = new char;
        type(cptr);
        cptr.ID = idcount(); //Generated ID
        name(cptr, back); if (*back == '0') { delete back; return; }
        adr_num(cptr, back); if (*back == '0') { delete back; return; }
        pass(cptr, back); if (*back == '0') { delete back; return; }
        add(cptr); system("cls");
        cout << "Would you like to add another user? (Y or N)\n\n";
        ans = _getch();
        system("cls");
    } while (ans == 'y' || ans == 'Y');
}

void menu1() {
    cout << "\n\t\t\t >>>>>>>>>  HOTEL MANAGEMENT SYSTEM  <<<<<<<<<\n\n\n"
        << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   1:   To log in with an existing account"
        << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   2:   To create an account for new user"
        << "\n\t\t\t------------------------------------------------"
        << "\n\t\t\tPRESS   3:   To exit program"
        << "\n\t\t\t------------------------------------------------\t\t\t\t";
}

int main()
{
    UI();
    client* cptr = new client; cptr->r = new room;
    room r;
    char choice = 0, priv;
  string email, passw;
  do {
      system("cls");
      menu1();
      choice = _getch(); //function takes single character as an input (doesn't need "Enter" key to be pressed)
      switch (choice)
      {
      case '1':
          emailpass(email, passw);
          compare(email, passw, priv, cptr);
          if (priv == '2')admin(r);
          else if (priv == '1') Client(cptr);
          break;
      case '2':
          case2(*cptr);
          system("cls");
          break;
      case '3':
          close();
          break;
      default:
          system("cls");
          cout << choice << " is not a Valid Choice, please choose again \n\n";
          break;
      }
  } while (choice);
}