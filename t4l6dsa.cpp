#include <iostream>
using namespace std;
const int SIZE = 100;
class WaitingRoom {
    int queue[SIZE];
    int front;
    int count;
    int nextId;
public:
    WaitingRoom() {
        front = 0;
        count = 0;
        nextId = 1;
    }
    int RegisterPatient() {
        if (count == SIZE) {
            cout << "Waiting room is full!" << endl;
            return -1;
        }
        int id = nextId++;
        queue[(front + count) % SIZE] = id;
        count++;
        cout << "Registered patient with ID " << id << endl;
        return id;
    }
    int ServePatient() {
        if (count == 0) {
            cout << "No patients waiting." << endl;
            return -1;
        }
        int id = queue[front];
        front = (front + 1) % SIZE;
        count--;
        cout << "Serving patient with ID " << id << endl;
        return id;
    }
    void CancelAll() {
        cout << "Cancelled " << count << " appointment(s). Doctor can go to lunch." << endl;
        front = 0;
        count = 0;
    }
    bool CanDoctorGoHome() {
        return count == 0;
    }
    void ShowAllPatient() {
        if (count == 0) {
            cout << "Waiting list: (empty)" << endl;
            return;
        }
        int temp[SIZE];
        for (int i = 0; i < count; i++)
            temp[i] = queue[(front + i) % SIZE];

        for (int i = 1; i < count; i++) {
            int key = temp[i];
            int j = i - 1;
            while (j >= 0 && temp[j] > key) {
                temp[j + 1] = temp[j];
                j--;
            }
            temp[j + 1] = key;
        }
        cout << "Waiting list (sorted): ";
        for (int i = 0; i < count; i++) {
            cout << temp[i];
            if (i < count - 1) cout << ", ";
        }
        cout << endl;
    }
};
int main() {
    WaitingRoom w;
    cout << "Can doctor go home? " << (w.CanDoctorGoHome() ? "Yes" : "No") << endl << endl;
    for (int i = 0; i < 5; i++)
        w.RegisterPatient();
    w.ShowAllPatient();
    cout << "Can doctor go home? " << (w.CanDoctorGoHome() ? "Yes" : "No") << endl << endl;
    w.ServePatient();
    w.ServePatient();
    w.ShowAllPatient();
    w.RegisterPatient();
    w.RegisterPatient();
    w.ShowAllPatient();
    cout << endl;
    w.CancelAll();
    w.ShowAllPatient();
    cout << "Can doctor go home? " << (w.CanDoctorGoHome() ? "Yes" : "No") << endl << endl;
    w.ServePatient();
    w.RegisterPatient();
    w.ShowAllPatient();
    return 0;
}