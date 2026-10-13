#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
struct Request {
    int id;
    int arrival;
    int processing;
};
class CircularQueue {
    Request* arr;
    int capacity;
    int head;
    int count;
    int minCap;
public:
    CircularQueue(int cap) {
        capacity = cap;
        minCap = cap;
        head = 0;
        count = 0;
        arr = new Request[capacity];
    }
    ~CircularQueue() { delete[] arr; }
    bool isEmpty() { return count == 0; }
    bool isFull() { return count == capacity; }
    Request front() { return arr[head]; }
    void resize(int newCap) {
        Request* newArr = new Request[newCap];
        for (int i = 0; i < count; i++)
            newArr[i] = arr[(head + i) % capacity];

        cout << "   [resize] capacity " << capacity << " -> " << newCap << endl;
        delete[] arr;
        arr = newArr;
        capacity = newCap;
        head = 0;
    }
    void enqueue(Request r) {
        if (isFull())
            resize(capacity * 2);
        arr[(head + count) % capacity] = r;
        count++;
        cout << "ENQUEUE R" << r.id << " (arrival=" << r.arrival
             << ", proc=" << r.processing << ")" << endl;
        display();
    }
    Request dequeue() {
        Request r = arr[head];
        head = (head + 1) % capacity;
        count--;
        if (capacity > minCap && count <= capacity / 4)
            resize(capacity / 2);
        cout << "DEQUEUE R" << r.id << endl;
        display();
        return r;
    }
    void display() {
        cout << "   capacity=" << capacity << " size=" << count
             << " head=" << head << " tail=" << (head + count) % capacity << endl;
        cout << "   [";
        for (int i = 0; i < capacity; i++) {
            bool used = false;
            for (int k = 0; k < count; k++)
                if ((head + k) % capacity == i) used = true;

            if (used) cout << " R" << arr[i].id;
            else cout << " --";
        }
        cout << " ]" << endl;
    }
};
void serveRequest(CircularQueue& q, int& clock, int& totalWait, int& processed) {
    Request r = q.dequeue();
    int start = clock;
    if (r.arrival > start) start = r.arrival;
    int wait = start - r.arrival;
    clock = start + r.processing;
    totalWait += wait;
    processed++;

    cout << "   -> R" << r.id << " starts at t=" << start << ", waited " << wait
         << ", finishes at t=" << clock << endl << endl;
}
int main() {
    CircularQueue q(4);
    int clock = 0, totalWait = 0, processed = 0;
    Request reqs[8] = { {1,0,5}, {2,1,3}, {3,2,4}, {4,3,2}, {5,4,6}, {6,5,1}, {7,6,2}, {8,7,3} };
    for (int i = 0; i < 5; i++) {
        q.enqueue(reqs[i]);
        cout << endl;
    }
    serveRequest(q, clock, totalWait, processed);
    serveRequest(q, clock, totalWait, processed);
    for (int i = 5; i < 8; i++) {
        q.enqueue(reqs[i]);
        cout << endl;
    }
    while (!q.isEmpty())
        serveRequest(q, clock, totalWait, processed);
    cout << "Processed requests : " << processed << endl;
    cout << "Total waiting time : " << totalWait << endl;
    cout << fixed << setprecision(2);
    cout << "Average waiting time: " << (double)totalWait / processed << endl;
    return 0;
}