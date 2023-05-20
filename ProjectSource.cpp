#include<iostream>
#include<string>

using namespace std;

struct Time {
	int day;
	int hour;
};

struct Node {
	string name;
	Time time;
	int id;
	Node *next, *prev;
};

class Reservation {
private:
	Node *head;
	Node* getTail() {
		if (head == NULL) return NULL;
		Node *ptr;
		for (ptr = head; ptr->next != NULL; ptr = ptr->next) {}
		return ptr;
	}
	bool booked[5][4];
public:
	Reservation() {
		head = NULL;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 4; j++) {
				booked[i][j] = false;
				if (i != 4) {
				booked[i][3] = true;
				}
			}
	}
	void printBooked(int day) {
		bool first = true;
		for (int i = 0; i < 4; i++) {
			if (booked[day - 1][i] == false) {
				if (first == false) {
					cout << " or ";
				} else first = false;
				if (day == 5) {
					cout << i + 4;
				} else {
					cout << i + 10;
				}
			}
		}
	}
	Node* findId(int id, bool empty = false) {
		Node* ptr;
		for (ptr = head; ptr != NULL; ptr = ptr->next) {
			if (ptr->id == id) {
				if (ptr->name == "") {
					if (empty == true) {
						break;
					}
				} else {
					break;
				}
			}
		}
		return ptr;
	}
	int getRandom() {
	int rr = (rand()%10)*1000+(rand()%10)*100+(rand()%10)*10+rand()%10;
	if (findId(rr, true) != NULL) getRandom();
	return rr;
	}
	string getName(int id) {
		return findId(id)->name;
	}
	Time getTime(int id) {
		return findId(id)->time;
	}
	int addReservation(string name, Time time) {
		int hour = time.hour;
		if (time.day == 5) hour -= 4; else hour -= 10;
		booked[time.day - 1][hour] = true;

		Node *newNode = new Node;
		newNode->name = name;
		newNode->time = time;
		int rand = getRandom();
		newNode->id = rand;
		newNode->next = NULL;
		if (head == NULL) {
			head = newNode;
			head->prev = NULL;
		} else {
			Node *tail = getTail();
			tail->next = newNode;
			newNode->prev = tail;
		}
			return rand;
	}
	void cancelReservation(int id) {
		Node *ptr;
		ptr = findId(id);
		ptr->name = "";
		int hour = ptr->time.hour;
		if (ptr->time.day == 5) {
			hour -= 4;
		} else {
			hour -= 10;
		}

		booked[ptr->time.day - 1][hour] = false;
		ptr->time.day = -1; // Can be rmeoved
		ptr->time.hour = -1; //
	}
	bool isAvailable(int day, int hour = -1) {
		if (hour == -1) {
			for (int i = 0; i < 4; i++) {
				if (!booked[day - 1][i]) {
					return true;
				}
			}
			return false;
		} else {
			if (day == 5) hour -= 4; else hour -= 10;
		return (!booked[day - 1][hour]);
		}
	}
};

string getDay(Time time) {
	switch (time.day) {
	case 1:
		return "Sun";
	case 2:
		return "Mon";
	case 3:
		return "Tue";
	case 4:
		return "Wed";
	case 5:
		return "Thu";
	default:
		return "Unknown";
	}
}

string getTimeAMPM(Time time) {
	string ampm;
	if (time.day == 5) {
		ampm = "pm";
	} else {
		if (time.hour == 12) {
			ampm = "pm";
		} else {
			ampm = "am";
		}
	}
	return ampm;
}

bool equalsIgnoreCase(string str1, string str2) {
	string nstr1, nstr2;
	for (int i = 0; i < str1.length(); i++) {
		string cstr(1, tolower(str1.at(i)));
		nstr1.append(cstr);
	}
	for (int i = 0; i < str1.length(); i++) {
		string cstr(1, tolower(str2.at(i)));
		nstr2.append(cstr);
	}
	return nstr1 == nstr2;
}

int main() {
	Reservation res;
	while (1) {
	int choice;
	string name, yn;
	cout << "Hello, if you wish to make a reservation, please press 1. If you wish to ask" << endl << "about your reservation, please press 2: ";
	cin >> choice;
	while (choice != 1 && choice != 2) {
		system("cls");
		cout << "Error: Wrong input, please enter the value again" << endl;
		cout << "please press 1. If you wish to ask" << endl << "about your reservation, please press 2: ";
		cin >> choice;
	}
	switch (choice) {
	case 1:
	Time t;
		cout << "please select a day (1-Sun, 2-Mon, 3-Tue, 4-Wed, 5-Thu): ";
		cin >> t.day;
		while (t.day < 1 || t.day > 5) {
			system("cls");
			cout << "Error: Wrong input, please enter the value again" << endl;
			cout << "please select a day (1-Sun, 2-Mon, 3-Tue, 4-Wed, 5-Thu): ";
			cin >> t.day;
		}
		while (!res.isAvailable(t.day)) {
			system("cls");
			cout << "Sorry, this day is not available" << endl;
			cout << "please enter the time: ";
			cin >> t.day;
		}
		cout << "On " + getDay(t) + " you have time slots at "; res.printBooked(t.day);
		cout << ",\nplease enter the time: ";
		cin >> t.hour;
		while (((t.day == 5) && (t.hour > 7 || t.hour < 4)) || ((t.day >= 1 && t.day <= 4) && (t.hour < 10 || t.hour > 12))) {
			system("cls");
			cout << "Sorry, this time is not available" << endl;
			cout << "please enter the time: ";
			cin >> t.hour;
		}
		while (!(res.isAvailable(t.day, t.hour))) {
			system("cls");
			cout << "Sorry, this hour is not available" << endl;
			cout << "please enter the time: ";
			cin >> t.hour;
		}
		cout << "please enter your name: ";
		cin.ignore();
		getline(cin, name);
		system("cls");
		cout << "Thank you for making the reservation. Your id is " << res.addReservation(name, t) << endl;
		break;
	case 2:	
		int id;
		Time fTime;
		cout << "please enter your reservation number: ";
		cin >> id;
		Node *fId;
		while ((fId = res.findId(id)) == NULL) {
			system("cls");
			cout << "that id doesn't exist," << endl;
			cout << "please enter your reservation number: ";
			cin >> id;
		}
		system("cls");
		cout << "Hello " << fId->name << endl;
		fTime = fId->time;
		cout << "Your reservation is on " << getDay(fTime) << " at " << fTime.hour << ":00 " << getTimeAMPM(fTime) << endl;
		cout << "Do you want to cancel it? (Yes/No): ";
		cin >> yn;
		if (equalsIgnoreCase(yn, "yes")) {
			res.cancelReservation(id);
			system("cls");
			cout << "Your reservation is cancelled\nThank you" << endl;
		} // Anything but yes will be considered as no
		break;
	default:
		break;
	}
	}
	system("pause");
	return 0;
}