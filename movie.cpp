#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <string>
using namespace std;

class Seat {
public:
    bool reserved;
    string reservedBy;
    Seat() : reserved(false), reservedBy("") {}
};

class Showtime {
public:
    string time;
    vector<vector<Seat> > seats;
    queue<string> waitingList;
    int price;

    Showtime() : time(""), price(0) {} // Default constructor added

    Showtime(string t, int rows, int cols, int p) : time(t), price(p) {
        seats = vector<vector<Seat> >(rows, vector<Seat>(cols));
    }

    void displaySeats() {
        cout << "\nSeat Layout (R = Reserved, A = Available):\n";
        for (int i = 0; i < seats.size(); i++) {
            for (int j = 0; j < seats[i].size(); j++) {
                cout << (seats[i][j].reserved ? "R " : "A ");
            }
            cout << endl;
        }
    }
};

class Movie {
public:
    string name;
    map<string, Showtime> showtimes;

    Movie() {}
    Movie(string n) : name(n) {}
};

class TicketSystem {
private:
    map<string, Movie> movies;
    string adminPassword;

public:
    TicketSystem() {
        adminPassword = "admin123";
    }

    void addMovie(string name) {
        movies[name] = Movie(name);
    }

    void addShowtime(string movie, string time, int rows, int cols, int price) {
        movies[movie].showtimes[time] = Showtime(time, rows, cols, price);
    }

    void bookTicket(string movie, string time, int row, int col, string customerName) {
        Showtime &st = movies[movie].showtimes[time];
        if (!st.seats[row][col].reserved) {
            st.seats[row][col].reserved = true;
            st.seats[row][col].reservedBy = customerName;
            cout << "Ticket booked successfully for " << customerName << "\n";
        } else {
            cout << "Seat already reserved. Adding to waiting list...\n";
            st.waitingList.push(customerName);
        }
    }

    void cancelTicket(string movie, string time, int row, int col) {
        Showtime &st = movies[movie].showtimes[time];
        if (st.seats[row][col].reserved) {
            st.seats[row][col].reserved = false;
            st.seats[row][col].reservedBy = "";
            cout << "Ticket cancelled.\n";
            if (!st.waitingList.empty()) {
                string nextCustomer = st.waitingList.front();
                st.waitingList.pop();
                bookTicket(movie, time, row, col, nextCustomer);
            }
        } else {
            cout << "Seat was not reserved.\n";
        }
    }

    void displaySeats(string movie, string time) {
        movies[movie].showtimes[time].displaySeats();
    }

    void adminMenu() {
        string password;
        cout << "Enter admin password: ";
        cin >> password;
        if (password != adminPassword) {
            cout << "Incorrect password. Access denied.\n";
            return;
        }

        cout << "\nAdmin Menu:\n";
        cout << "1. Change ticket price\n";
        cout << "2. Exit\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            string movie, time;
            int newPrice;
            cout << "Enter movie name: ";
            cin >> movie;
            cout << "Enter showtime: ";
            cin >> time;
            cout << "Enter new ticket price: ";
            cin >> newPrice;
            movies[movie].showtimes[time].price = newPrice;
            cout << "Ticket price updated.\n";
        }
    }
};

int main() {
    TicketSystem system;
    system.addMovie("Avengers");
    system.addShowtime("Avengers", "18:00", 5, 5, 10);

    int choice;
    do {
        cout << "\nMovie Ticket Booking System\n";
        cout << "1. Book Ticket\n2. Cancel Ticket\n3. View Seats\n4. Admin Menu\n5. Exit\n";
        cin >> choice;

        if (choice == 1) {
            string movie, time, name;
            int row, col;
            cout << "Enter movie name: ";
            cin >> movie;
            cout << "Enter showtime: ";
            cin >> time;
            cout << "Enter your name: ";
            cin >> name;
            cout << "Enter row and column: ";
            cin >> row >> col;
            system.bookTicket(movie, time, row, col, name);
        } else if (choice == 2) {
            string movie, time;
            int row, col;
            cout << "Enter movie name: ";
            cin >> movie;
            cout << "Enter showtime: ";
            cin >> time;
            cout << "Enter row and column: ";
            cin >> row >> col;
            system.cancelTicket(movie, time, row, col);
        } else if (choice == 3) {
            string movie, time;
            cout << "Enter movie name: ";
            cin >> movie;
            cout << "Enter showtime: ";
            cin >> time;
            system.displaySeats(movie, time);
        } else if (choice == 4) {
            system.adminMenu();
        }
    } while (choice != 5);

    return 0;
}
