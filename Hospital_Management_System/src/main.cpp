#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

// ==================== UTILITY FUNCTIONS ====================
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string getCurrentDateTime()
{
    time_t now = time(0);
    char *dt = ctime(&now);
    string result(dt);
    result.pop_back(); // Remove newline
    return result;
}

// ==================== PATIENT CLASS ====================
class Patient
{
public:
    int patientID;
    string name;
    int age;
    string disease;
    int severity; // 1-10, higher = more urgent
    string admissionDate;
    string contactNumber;
    string bloodGroup;

    Patient() : patientID(0), age(0), severity(0) {}

    Patient(int id, string n, int a, string d, int s, string contact, string blood)
        : patientID(id), name(n), age(a), disease(d), severity(s), contactNumber(contact), bloodGroup(blood)
    {
        admissionDate = getCurrentDateTime();
    }

    void display() const
    {
        cout << "\n╔══════════════════════════════════════════════════════╗\n";
        cout << "  Patient ID      : " << patientID << "\n";
        cout << "  Name            : " << name << "\n";
        cout << "  Age             : " << age << "\n";
        cout << "  Disease         : " << disease << "\n";
        cout << "  Severity Level  : " << severity << "/10\n";
        cout << "  Blood Group     : " << bloodGroup << "\n";
        cout << "  Contact         : " << contactNumber << "\n";
        cout << "  Admission Date  : " << admissionDate << "\n";
        cout << "╚══════════════════════════════════════════════════════╝\n";
    }

    string toFileString() const
    {
        return to_string(patientID) + "|" + name + "|" + to_string(age) + "|" +
               disease + "|" + to_string(severity) + "|" + contactNumber + "|" +
               bloodGroup + "|" + admissionDate;
    }

    static Patient fromFileString(const string &line)
    {
        Patient p;
        stringstream ss(line);
        string item;

        getline(ss, item, '|');
        p.patientID = stoi(item);
        getline(ss, p.name, '|');
        getline(ss, item, '|');
        p.age = stoi(item);
        getline(ss, p.disease, '|');
        getline(ss, item, '|');
        p.severity = stoi(item);
        getline(ss, p.contactNumber, '|');
        getline(ss, p.bloodGroup, '|');
        getline(ss, p.admissionDate, '|');

        return p;
    }
};

// ==================== DOCTOR CLASS ====================
class Doctor
{
public:
    int doctorID;
    string name;
    string specialization;
    string availability;
    int experienceYears;

    Doctor() : doctorID(0), experienceYears(0) {}

    Doctor(int id, string n, string spec, string avail, int exp)
        : doctorID(id), name(n), specialization(spec), availability(avail), experienceYears(exp) {}

    void display() const
    {
        cout << "\n╔══════════════════════════════════════════════════════╗\n";
        cout << "  Doctor ID       : " << doctorID << "\n";
        cout << "  Name            : " << name << "\n";
        cout << "  Specialization  : " << specialization << "\n";
        cout << "  Experience      : " << experienceYears << " years\n";
        cout << "  Availability    : " << availability << "\n";
        cout << "╚══════════════════════════════════════════════════════╝\n";
    }

    string toFileString() const
    {
        return to_string(doctorID) + "|" + name + "|" + specialization + "|" +
               availability + "|" + to_string(experienceYears);
    }

    static Doctor fromFileString(const string &line)
    {
        Doctor d;
        stringstream ss(line);
        string item;

        getline(ss, item, '|');
        d.doctorID = stoi(item);
        getline(ss, d.name, '|');
        getline(ss, d.specialization, '|');
        getline(ss, d.availability, '|');
        getline(ss, item, '|');
        d.experienceYears = stoi(item);

        return d;
    }
};

// ==================== APPOINTMENT CLASS ====================
class Appointment
{
public:
    int appointmentID;
    int patientID;
    int doctorID;
    string dateTime;
    string status; // Scheduled, Completed, Cancelled

    Appointment() : appointmentID(0), patientID(0), doctorID(0) {}

    Appointment(int appID, int pID, int dID, string dt, string stat)
        : appointmentID(appID), patientID(pID), doctorID(dID), dateTime(dt), status(stat) {}

    string toFileString() const
    {
        return to_string(appointmentID) + "|" + to_string(patientID) + "|" +
               to_string(doctorID) + "|" + dateTime + "|" + status;
    }

    static Appointment fromFileString(const string &line)
    {
        Appointment a;
        stringstream ss(line);
        string item;

        getline(ss, item, '|');
        a.appointmentID = stoi(item);
        getline(ss, item, '|');
        a.patientID = stoi(item);
        getline(ss, item, '|');
        a.doctorID = stoi(item);
        getline(ss, a.dateTime, '|');
        getline(ss, a.status, '|');

        return a;
    }
};

// ==================== DOUBLY LINKED LIST NODE ====================
template <typename T>
class DLLNode
{
public:
    T data;
    DLLNode *prev;
    DLLNode *next;

    DLLNode(T d) : data(d), prev(nullptr), next(nullptr) {}
};

// ==================== DOUBLY LINKED LIST ====================
template <typename T>
class DoublyLinkedList
{
private:
    DLLNode<T> *head;
    DLLNode<T> *tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList()
    {
        clear();
    }

    void insertEnd(T data)
    {
        DLLNode<T> *newNode = new DLLNode<T>(data);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    void insertFront(T data)
    {
        DLLNode<T> *newNode = new DLLNode<T>(data);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    bool deleteByValue(int id)
    {
        DLLNode<T> *current = head;
        while (current)
        {
            if (current->data.appointmentID == id)
            {
                if (current == head && current == tail)
                {
                    head = tail = nullptr;
                }
                else if (current == head)
                {
                    head = current->next;
                    head->prev = nullptr;
                }
                else if (current == tail)
                {
                    tail = current->prev;
                    tail->next = nullptr;
                }
                else
                {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                delete current;
                size--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void traverse() const
    {
        if (!head)
        {
            cout << "  No appointments found.\n";
            return;
        }

        DLLNode<T> *current = head;
        int count = 1;
        while (current)
        {
            cout << "\n  [" << count++ << "] ";
            cout << "Appointment ID: " << current->data.appointmentID
                 << " | Patient ID: " << current->data.patientID
                 << " | Doctor ID: " << current->data.doctorID
                 << "\n      Date/Time: " << current->data.dateTime
                 << " | Status: " << current->data.status << "\n";
            current = current->next;
        }
    }

    vector<T> toVector() const
    {
        vector<T> result;
        DLLNode<T> *current = head;
        while (current)
        {
            result.push_back(current->data);
            current = current->next;
        }
        return result;
    }

    void clear()
    {
        while (head)
        {
            DLLNode<T> *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return head == nullptr; }
};

// ==================== BST NODE ====================
class BSTNode
{
public:
    Doctor data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(Doctor d) : data(d), left(nullptr), right(nullptr) {}
};

// ==================== BINARY SEARCH TREE ====================
class BST
{
private:
    BSTNode *root;

    BSTNode *insertHelper(BSTNode *node, Doctor doctor)
    {
        if (!node)
            return new BSTNode(doctor);

        if (doctor.doctorID < node->data.doctorID)
            node->left = insertHelper(node->left, doctor);
        else if (doctor.doctorID > node->data.doctorID)
            node->right = insertHelper(node->right, doctor);

        return node;
    }

    BSTNode *searchHelper(BSTNode *node, int id)
    {
        if (!node || node->data.doctorID == id)
            return node;

        if (id < node->data.doctorID)
            return searchHelper(node->left, id);
        return searchHelper(node->right, id);
    }

    BSTNode *findMin(BSTNode *node)
    {
        while (node && node->left)
            node = node->left;
        return node;
    }

    BSTNode *deleteHelper(BSTNode *node, int id)
    {
        if (!node)
            return nullptr;

        if (id < node->data.doctorID)
            node->left = deleteHelper(node->left, id);
        else if (id > node->data.doctorID)
            node->right = deleteHelper(node->right, id);
        else
        {
            if (!node->left)
            {
                BSTNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                BSTNode *temp = node->left;
                delete node;
                return temp;
            }

            BSTNode *temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteHelper(node->right, temp->data.doctorID);
        }
        return node;
    }

    void inorderHelper(BSTNode *node)
    {
        if (node)
        {
            inorderHelper(node->left);
            node->data.display();
            inorderHelper(node->right);
        }
    }

    void collectDoctors(BSTNode *node, vector<Doctor> &doctors)
    {
        if (node)
        {
            collectDoctors(node->left, doctors);
            doctors.push_back(node->data);
            collectDoctors(node->right, doctors);
        }
    }

    void clearHelper(BSTNode *node)
    {
        if (node)
        {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }

public:
    BST() : root(nullptr) {}

    ~BST()
    {
        clear();
    }

    void insert(Doctor doctor)
    {
        root = insertHelper(root, doctor);
    }

    Doctor *search(int id)
    {
        BSTNode *result = searchHelper(root, id);
        return result ? &(result->data) : nullptr;
    }

    bool deleteDoctor(int id)
    {
        if (!search(id))
            return false;
        root = deleteHelper(root, id);
        return true;
    }

    void inorderTraversal()
    {
        if (!root)
        {
            cout << "  No doctors found.\n";
            return;
        }
        inorderHelper(root);
    }

    vector<Doctor> getAllDoctors()
    {
        vector<Doctor> doctors;
        collectDoctors(root, doctors);
        return doctors;
    }

    void clear()
    {
        clearHelper(root);
        root = nullptr;
    }
};

// ==================== PRIORITY QUEUE (MIN-HEAP) ====================
class EmergencyQueue
{
private:
    struct PatientNode
    {
        Patient patient;

        bool operator<(const PatientNode &other) const
        {
            return patient.severity < other.patient.severity; // Max heap (higher severity first)
        }
    };

    priority_queue<PatientNode> pq;

public:
    void enqueue(Patient p)
    {
        PatientNode node;
        node.patient = p;
        pq.push(node);
    }

    Patient dequeue()
    {
        if (pq.empty())
        {
            throw runtime_error("Emergency queue is empty");
        }
        Patient p = pq.top().patient;
        pq.pop();
        return p;
    }

    bool isEmpty() const
    {
        return pq.empty();
    }

    int size() const
    {
        return pq.size();
    }

    void display() const
    {
        if (pq.empty())
        {
            cout << "  Emergency queue is empty.\n";
            return;
        }

        priority_queue<PatientNode> temp = pq;
        int position = 1;

        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║         EMERGENCY QUEUE (Sorted by Severity)              ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n";

        while (!temp.empty())
        {
            Patient p = temp.top().patient;
            temp.pop();
            cout << "\n  [Priority " << position++ << "] Severity: " << p.severity << "/10\n";
            cout << "    Patient ID: " << p.patientID << " | Name: " << p.name << "\n";
            cout << "    Disease: " << p.disease << "\n";
        }
    }
};

// ==================== HASH TABLE (Chaining) ====================
class HashTable
{
private:
    static const int TABLE_SIZE = 100;
    vector<Patient> *table;

    int hashFunction(int key)
    {
        return key % TABLE_SIZE;
    }

public:
    HashTable()
    {
        table = new vector<Patient>[TABLE_SIZE];
    }

    ~HashTable()
    {
        delete[] table;
    }

    void insert(Patient patient)
    {
        int index = hashFunction(patient.patientID);

        // Check if patient already exists and update
        for (auto &p : table[index])
        {
            if (p.patientID == patient.patientID)
            {
                p = patient;
                return;
            }
        }

        table[index].push_back(patient);
    }

    Patient *search(int patientID)
    {
        int index = hashFunction(patientID);

        for (auto &p : table[index])
        {
            if (p.patientID == patientID)
            {
                return &p;
            }
        }

        return nullptr;
    }

    bool deletePatient(int patientID)
    {
        int index = hashFunction(patientID);

        for (auto it = table[index].begin(); it != table[index].end(); ++it)
        {
            if (it->patientID == patientID)
            {
                table[index].erase(it);
                return true;
            }
        }

        return false;
    }

    void displayAll()
    {
        bool found = false;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            for (const auto &p : table[i])
            {
                p.display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "  No patients found in the system.\n";
        }
    }

    vector<Patient> getAllPatients()
    {
        vector<Patient> patients;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            for (const auto &p : table[i])
            {
                patients.push_back(p);
            }
        }
        return patients;
    }

    void clear()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            table[i].clear();
        }
    }
};

// ==================== GRAPH (Department Network) ====================
class DepartmentGraph
{
private:
    map<string, vector<pair<string, int>>> adjList; // dept -> [(connected_dept, distance)]

public:
    void addDepartment(string dept)
    {
        if (adjList.find(dept) == adjList.end())
        {
            adjList[dept] = vector<pair<string, int>>();
        }
    }

    void addConnection(string dept1, string dept2, int distance)
    {
        addDepartment(dept1);
        addDepartment(dept2);

        adjList[dept1].push_back({dept2, distance});
        adjList[dept2].push_back({dept1, distance}); // Undirected
    }

    void displayGraph()
    {
        if (adjList.empty())
        {
            cout << "  No departments in the network.\n";
            return;
        }

        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║         HOSPITAL DEPARTMENT NETWORK                       ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        for (const auto &dept : adjList)
        {
            cout << "  " << dept.first << " connects to:\n";
            for (const auto &connection : dept.second)
            {
                cout << "    → " << connection.first << " (Distance: " << connection.second << " meters)\n";
            }
            cout << "\n";
        }
    }

    void BFS(string start)
    {
        if (adjList.find(start) == adjList.end())
        {
            cout << "  Department not found.\n";
            return;
        }

        map<string, bool> visited;
        queue<string> q;

        visited[start] = true;
        q.push(start);

        cout << "\n  BFS Traversal from " << start << ":\n  ";

        while (!q.empty())
        {
            string current = q.front();
            q.pop();
            cout << current << " → ";

            for (const auto &neighbor : adjList[current])
            {
                if (!visited[neighbor.first])
                {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        cout << "END\n";
    }

    void DFS(string start)
    {
        if (adjList.find(start) == adjList.end())
        {
            cout << "  Department not found.\n";
            return;
        }

        map<string, bool> visited;
        cout << "\n  DFS Traversal from " << start << ":\n  ";
        DFSHelper(start, visited);
        cout << "END\n";
    }

    void DFSHelper(string node, map<string, bool> &visited)
    {
        visited[node] = true;
        cout << node << " → ";

        for (const auto &neighbor : adjList[node])
        {
            if (!visited[neighbor.first])
            {
                DFSHelper(neighbor.first, visited);
            }
        }
    }

    void dijkstra(string start, string end)
    {
        if (adjList.find(start) == adjList.end() || adjList.find(end) == adjList.end())
        {
            cout << "  Department not found.\n";
            return;
        }

        map<string, int> distance;
        map<string, string> parent;
        set<pair<int, string>> pq;

        for (const auto &dept : adjList)
        {
            distance[dept.first] = INT_MAX;
        }

        distance[start] = 0;
        pq.insert({0, start});

        while (!pq.empty())
        {
            string current = pq.begin()->second;
            pq.erase(pq.begin());

            for (const auto &neighbor : adjList[current])
            {
                string next = neighbor.first;
                int weight = neighbor.second;

                if (distance[current] + weight < distance[next])
                {
                    pq.erase({distance[next], next});
                    distance[next] = distance[current] + weight;
                    parent[next] = current;
                    pq.insert({distance[next], next});
                }
            }
        }

        if (distance[end] == INT_MAX)
        {
            cout << "  No path exists between " << start << " and " << end << "\n";
            return;
        }

        cout << "\n  Shortest path from " << start << " to " << end << ":\n  ";

        vector<string> path;
        string current = end;
        while (current != start)
        {
            path.push_back(current);
            current = parent[current];
        }
        path.push_back(start);

        reverse(path.begin(), path.end());

        for (size_t i = 0; i < path.size(); i++)
        {
            cout << path[i];
            if (i < path.size() - 1)
                cout << " → ";
        }

        cout << "\n  Total distance: " << distance[end] << " meters\n";
    }

    vector<string> getAllDepartments()
    {
        vector<string> depts;
        for (const auto &dept : adjList)
        {
            depts.push_back(dept.first);
        }
        return depts;
    }
};

// ==================== ACTIVITY LOG (Stack) ====================
class ActivityLog
{
private:
    stack<string> logStack;

public:
    void addLog(string activity)
    {
        string logEntry = "[" + getCurrentDateTime() + "] " + activity;
        logStack.push(logEntry);
    }

    void displayLogs()
    {
        if (logStack.empty())
        {
            cout << "  No activity logs found.\n";
            return;
        }

        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                   ACTIVITY LOGS                           ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        stack<string> temp = logStack;
        int count = 1;

        while (!temp.empty())
        {
            cout << "  " << count++ << ". " << temp.top() << "\n";
            temp.pop();
        }
    }

    void saveToFile(const string &filename)
    {
        ofstream file(filename);
        if (!file.is_open())
            return;

        stack<string> temp = logStack;
        while (!temp.empty())
        {
            file << temp.top() << "\n";
            temp.pop();
        }

        file.close();
    }

    void clear()
    {
        while (!logStack.empty())
        {
            logStack.pop();
        }
    }
};

// ==================== HOSPITAL MANAGEMENT SYSTEM ====================
class HospitalManagementSystem
{
private:
    HashTable patientRecords;
    BST doctorRecords;
    DoublyLinkedList<Appointment> appointmentHistory;
    EmergencyQueue emergencyQueue;
    DepartmentGraph departmentNetwork;
    ActivityLog activityLog;
    queue<Patient> regularQueue;

    int nextPatientID;
    int nextDoctorID;
    int nextAppointmentID;

public:
    HospitalManagementSystem() : nextPatientID(1001), nextDoctorID(2001), nextAppointmentID(3001)
    {
        initializeSampleData();
    }

    void initializeSampleData()
    {
        // Sample departments
        departmentNetwork.addDepartment("Emergency");
        departmentNetwork.addDepartment("Cardiology");
        departmentNetwork.addDepartment("Neurology");
        departmentNetwork.addDepartment("Pediatrics");
        departmentNetwork.addDepartment("Radiology");
        departmentNetwork.addDepartment("Surgery");
        departmentNetwork.addDepartment("ICU");

        departmentNetwork.addConnection("Emergency", "ICU", 50);
        departmentNetwork.addConnection("Emergency", "Radiology", 100);
        departmentNetwork.addConnection("Emergency", "Surgery", 150);
        departmentNetwork.addConnection("Cardiology", "ICU", 75);
        departmentNetwork.addConnection("Neurology", "Radiology", 80);
        departmentNetwork.addConnection("Surgery", "ICU", 60);
        departmentNetwork.addConnection("Pediatrics", "Emergency", 120);
        departmentNetwork.addConnection("Radiology", "Surgery", 90);

        activityLog.addLog("System initialized with sample departments");
    }

    // ========== PATIENT MANAGEMENT ==========
    void addPatient()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                     ADD NEW PATIENT                       ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        string name, disease, contact, blood;
        int age, severity;

        cin.ignore();
        cout << "  Enter Patient Name: ";
        getline(cin, name);

        cout << "  Enter Age: ";
        cin >> age;

        cin.ignore();
        cout << "  Enter Disease: ";
        getline(cin, disease);

        cout << "  Enter Severity Level (1-10): ";
        cin >> severity;

        cin.ignore();
        cout << "  Enter Contact Number: ";
        getline(cin, contact);

        cout << "  Enter Blood Group: ";
        getline(cin, blood);

        Patient p(nextPatientID++, name, age, disease, severity, contact, blood);
        patientRecords.insert(p);

        if (severity >= 7)
        {
            emergencyQueue.enqueue(p);
            activityLog.addLog("Emergency patient added: " + name + " (ID: " + to_string(p.patientID) + ")");
        }
        else
        {
            regularQueue.push(p);
            activityLog.addLog("Regular patient added: " + name + " (ID: " + to_string(p.patientID) + ")");
        }

        cout << "\n  ✓ Patient added successfully! Patient ID: " << p.patientID << "\n";
        pause();
    }

    void searchPatient()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                     SEARCH PATIENT                        ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        int id;
        cout << "  Enter Patient ID to search: ";
        cin >> id;

        Patient *p = patientRecords.search(id);

        if (p)
        {
            p->display();
            activityLog.addLog("Patient searched: ID " + to_string(id));
        }
        else
        {
            cout << "\n  ✗ Patient with ID " << id << " not found.\n";
        }

        pause();
    }

    void displayAllPatients()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                   ALL PATIENT RECORDS                     ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n";

        patientRecords.displayAll();
        activityLog.addLog("Displayed all patients");
        pause();
    }

    void deletePatient()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                     DELETE PATIENT                        ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        int id;
        cout << "  Enter Patient ID to delete: ";
        cin >> id;

        if (patientRecords.deletePatient(id))
        {
            cout << "\n  ✓ Patient deleted successfully!\n";
            activityLog.addLog("Patient deleted: ID " + to_string(id));
        }
        else
        {
            cout << "\n  ✗ Patient not found.\n";
        }

        pause();
    }

    // ========== DOCTOR MANAGEMENT ==========
    void addDoctor()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                      ADD NEW DOCTOR                       ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        string name, specialization, availability;
        int experience;

        cin.ignore();
        cout << "  Enter Doctor Name: ";
        getline(cin, name);

        cout << "  Enter Specialization: ";
        getline(cin, specialization);

        cout << "  Enter Availability (e.g., Mon-Fri 9AM-5PM): ";
        getline(cin, availability);

        cout << "  Enter Years of Experience: ";
        cin >> experience;

        Doctor d(nextDoctorID++, name, specialization, availability, experience);
        doctorRecords.insert(d);

        cout << "\n  ✓ Doctor added successfully! Doctor ID: " << d.doctorID << "\n";
        activityLog.addLog("Doctor added: " + name + " (ID: " + to_string(d.doctorID) + ")");
        pause();
    }

    void searchDoctor()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                      SEARCH DOCTOR                        ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        int id;
        cout << "  Enter Doctor ID to search: ";
        cin >> id;

        Doctor *d = doctorRecords.search(id);

        if (d)
        {
            d->display();
            activityLog.addLog("Doctor searched: ID " + to_string(id));
        }
        else
        {
            cout << "\n  ✗ Doctor with ID " << id << " not found.\n";
        }

        pause();
    }

    void displayAllDoctors()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                    ALL DOCTOR RECORDS                     ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n";

        doctorRecords.inorderTraversal();
        activityLog.addLog("Displayed all doctors");
        pause();
    }

    void deleteDoctor()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                      DELETE DOCTOR                        ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        int id;
        cout << "  Enter Doctor ID to delete: ";
        cin >> id;

        if (doctorRecords.deleteDoctor(id))
        {
            cout << "\n  ✓ Doctor deleted successfully!\n";
            activityLog.addLog("Doctor deleted: ID " + to_string(id));
        }
        else
        {
            cout << "\n  ✗ Doctor not found.\n";
        }

        pause();
    }

    // ========== APPOINTMENT MANAGEMENT ==========
    void scheduleAppointment()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                  SCHEDULE APPOINTMENT                     ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        int patientID, doctorID;
        string dateTime;

        cout << "  Enter Patient ID: ";
        cin >> patientID;

        if (!patientRecords.search(patientID))
        {
            cout << "\n  ✗ Patient not found.\n";
            pause();
            return;
        }

        cout << "  Enter Doctor ID: ";
        cin >> doctorID;

        if (!doctorRecords.search(doctorID))
        {
            cout << "\n  ✗ Doctor not found.\n";
            pause();
            return;
        }

        cin.ignore();
        cout << "  Enter Date/Time (e.g., 2025-12-20 10:00 AM): ";
        getline(cin, dateTime);

        Appointment app(nextAppointmentID++, patientID, doctorID, dateTime, "Scheduled");
        appointmentHistory.insertEnd(app);

        cout << "\n  ✓ Appointment scheduled successfully! Appointment ID: " << app.appointmentID << "\n";
        activityLog.addLog("Appointment scheduled: ID " + to_string(app.appointmentID));
        pause();
    }

    void displayAllAppointments()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                   APPOINTMENT HISTORY                     ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n";

        appointmentHistory.traverse();
        activityLog.addLog("Displayed all appointments");
        pause();
    }

    void cancelAppointment()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                   CANCEL APPOINTMENT                      ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        int id;
        cout << "  Enter Appointment ID to cancel: ";
        cin >> id;

        if (appointmentHistory.deleteByValue(id))
        {
            cout << "\n  ✓ Appointment cancelled successfully!\n";
            activityLog.addLog("Appointment cancelled: ID " + to_string(id));
        }
        else
        {
            cout << "\n  ✗ Appointment not found.\n";
        }

        pause();
    }

    // ========== EMERGENCY QUEUE ==========
    void displayEmergencyQueue()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                    EMERGENCY QUEUE                        ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n";

        emergencyQueue.display();
        activityLog.addLog("Viewed emergency queue");
        pause();
    }

    void processEmergencyPatient()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║               PROCESS EMERGENCY PATIENT                   ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        if (emergencyQueue.isEmpty())
        {
            cout << "  No emergency patients in queue.\n";
        }
        else
        {
            Patient p = emergencyQueue.dequeue();
            cout << "  Processing emergency patient:\n";
            p.display();
            activityLog.addLog("Processed emergency patient: " + p.name + " (ID: " + to_string(p.patientID) + ")");
        }

        pause();
    }

    // ========== DEPARTMENT NETWORK ==========
    void displayDepartmentNetwork()
    {
        clearScreen();
        departmentNetwork.displayGraph();
        activityLog.addLog("Viewed department network");
        pause();
    }

    void findShortestPath()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║              FIND SHORTEST PATH (Dijkstra)                ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        string start, end;

        cout << "  Available Departments:\n";
        vector<string> depts = departmentNetwork.getAllDepartments();
        for (const auto &dept : depts)
        {
            cout << "    - " << dept << "\n";
        }

        cin.ignore();
        cout << "\n  Enter starting department: ";
        getline(cin, start);

        cout << "  Enter destination department: ";
        getline(cin, end);

        departmentNetwork.dijkstra(start, end);
        activityLog.addLog("Shortest path calculated: " + start + " to " + end);
        pause();
    }

    void performBFS()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║            BREADTH-FIRST SEARCH (BFS)                     ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        string start;

        cout << "  Available Departments:\n";
        vector<string> depts = departmentNetwork.getAllDepartments();
        for (const auto &dept : depts)
        {
            cout << "    - " << dept << "\n";
        }

        cin.ignore();
        cout << "\n  Enter starting department: ";
        getline(cin, start);

        departmentNetwork.BFS(start);
        activityLog.addLog("BFS traversal from: " + start);
        pause();
    }

    void performDFS()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║             DEPTH-FIRST SEARCH (DFS)                      ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        string start;

        cout << "  Available Departments:\n";
        vector<string> depts = departmentNetwork.getAllDepartments();
        for (const auto &dept : depts)
        {
            cout << "    - " << dept << "\n";
        }

        cin.ignore();
        cout << "\n  Enter starting department: ";
        getline(cin, start);

        departmentNetwork.DFS(start);
        activityLog.addLog("DFS traversal from: " + start);
        pause();
    }

    // ========== ANALYTICS ==========
    void generateAnalytics()
    {
        clearScreen();
        cout << "\n  ╔═══════════════════════════════════════════════════════════╗\n";
        cout << "  ║                  SYSTEM ANALYTICS                         ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

        vector<Patient> patients = patientRecords.getAllPatients();
        vector<Doctor> doctors = doctorRecords.getAllDoctors();

        cout << "  Total Patients: " << patients.size() << "\n";
        cout << "  Total Doctors: " << doctors.size() << "\n";
        cout << "  Total Appointments: " << appointmentHistory.getSize() << "\n";
        cout << "  Emergency Queue Size: " << emergencyQueue.size() << "\n";

        // Disease frequency analysis
        map<string, int> diseaseCount;
        for (const auto &p : patients)
        {
            diseaseCount[p.disease]++;
        }

        cout << "\n  Disease Frequency Analysis:\n";
        for (const auto &dc : diseaseCount)
        {
            cout << "    " << dc.first << ": " << dc.second << " patients\n";
        }

        // Specialization analysis
        map<string, int> specCount;
        for (const auto &d : doctors)
        {
            specCount[d.specialization]++;
        }

        cout << "\n  Doctor Specialization Distribution:\n";
        for (const auto &sc : specCount)
        {
            cout << "    " << sc.first << ": " << sc.second << " doctor(s)\n";
        }

        activityLog.addLog("Generated system analytics");
        pause();
    }

    // ========== ACTIVITY LOG ==========
    void viewActivityLog()
    {
        clearScreen();
        activityLog.displayLogs();
        pause();
    }

    // ========== FILE OPERATIONS ==========
    void saveAllData()
    {
        // Save patients
        ofstream pFile("data/patients.txt");
        vector<Patient> patients = patientRecords.getAllPatients();
        for (const auto &p : patients)
        {
            pFile << p.toFileString() << "\n";
        }
        pFile.close();

        // Save doctors
        ofstream dFile("data/doctors.txt");
        vector<Doctor> doctors = doctorRecords.getAllDoctors();
        for (const auto &d : doctors)
        {
            dFile << d.toFileString() << "\n";
        }
        dFile.close();

        // Save appointments
        ofstream aFile("data/appointments.txt");
        vector<Appointment> appointments = appointmentHistory.toVector();
        for (const auto &a : appointments)
        {
            aFile << a.toFileString() << "\n";
        }
        aFile.close();

        // Save activity log
        activityLog.saveToFile("data/activity_log.txt");

        cout << "\n  ✓ All data saved successfully!\n";
        activityLog.addLog("Data saved to files");
    }

    void loadAllData()
    {
        // Load patients
        ifstream pFile("data/patients.txt");
        if (pFile.is_open())
        {
            string line;
            patientRecords.clear();
            while (getline(pFile, line))
            {
                if (!line.empty())
                {
                    Patient p = Patient::fromFileString(line);
                    patientRecords.insert(p);
                    if (p.patientID >= nextPatientID)
                    {
                        nextPatientID = p.patientID + 1;
                    }
                }
            }
            pFile.close();
        }

        // Load doctors
        ifstream dFile("data/doctors.txt");
        if (dFile.is_open())
        {
            string line;
            doctorRecords.clear();
            while (getline(dFile, line))
            {
                if (!line.empty())
                {
                    Doctor d = Doctor::fromFileString(line);
                    doctorRecords.insert(d);
                    if (d.doctorID >= nextDoctorID)
                    {
                        nextDoctorID = d.doctorID + 1;
                    }
                }
            }
            dFile.close();
        }

        // Load appointments
        ifstream aFile("data/appointments.txt");
        if (aFile.is_open())
        {
            string line;
            appointmentHistory.clear();
            while (getline(aFile, line))
            {
                if (!line.empty())
                {
                    Appointment a = Appointment::fromFileString(line);
                    appointmentHistory.insertEnd(a);
                    if (a.appointmentID >= nextAppointmentID)
                    {
                        nextAppointmentID = a.appointmentID + 1;
                    }
                }
            }
            aFile.close();
        }

        cout << "\n  ✓ All data loaded successfully!\n";
        activityLog.addLog("Data loaded from files");
    }

    // ========== MAIN MENU ==========
    void run()
    {
        loadAllData();

        while (true)
        {
            clearScreen();
            cout << "\n";
            cout << "  ╔═══════════════════════════════════════════════════════════╗\n";
            cout << "  ║                                                           ║\n";
            cout << "  ║        HOSPITAL MANAGEMENT SYSTEM                         ║\n";
            cout << "  ║        Data Structures Project                            ║\n";
            cout << "  ║                                                           ║\n";
            cout << "  ╚═══════════════════════════════════════════════════════════╝\n\n";

            cout << "  ┌───────────────────────────────────────────────────────────┐\n";
            cout << "  │  PATIENT MANAGEMENT (Hash Table)                          │\n";
            cout << "  ├───────────────────────────────────────────────────────────┤\n";
            cout << "  │  1. Add New Patient                                       │\n";
            cout << "  │  2. Search Patient by ID                                  │\n";
            cout << "  │  3. Display All Patients                                  │\n";
            cout << "  │  4. Delete Patient                                        │\n";
            cout << "  └───────────────────────────────────────────────────────────┘\n\n";

            cout << "  ┌───────────────────────────────────────────────────────────┐\n";
            cout << "  │  DOCTOR MANAGEMENT (Binary Search Tree)                   │\n";
            cout << "  ├───────────────────────────────────────────────────────────┤\n";
            cout << "  │  5. Add New Doctor                                        │\n";
            cout << "  │  6. Search Doctor by ID                                   │\n";
            cout << "  │  7. Display All Doctors (Inorder Traversal)               │\n";
            cout << "  │  8. Delete Doctor                                         │\n";
            cout << "  └───────────────────────────────────────────────────────────┘\n\n";

            cout << "  ┌───────────────────────────────────────────────────────────┐\n";
            cout << "  │  APPOINTMENT MANAGEMENT (Doubly Linked List)              │\n";
            cout << "  ├───────────────────────────────────────────────────────────┤\n";
            cout << "  │  9. Schedule Appointment                                  │\n";
            cout << "  │  10. Display All Appointments                             │\n";
            cout << "  │  11. Cancel Appointment                                   │\n";
            cout << "  └───────────────────────────────────────────────────────────┘\n\n";

            cout << "  ┌───────────────────────────────────────────────────────────┐\n";
            cout << "  │  EMERGENCY MANAGEMENT (Priority Queue/Heap)               │\n";
            cout << "  ├───────────────────────────────────────────────────────────┤\n";
            cout << "  │  12. View Emergency Queue                                 │\n";
            cout << "  │  13. Process Next Emergency Patient                       │\n";
            cout << "  └───────────────────────────────────────────────────────────┘\n\n";

            cout << "  ┌───────────────────────────────────────────────────────────┐\n";
            cout << "  │  DEPARTMENT NETWORK (Graphs)                              │\n";
            cout << "  ├───────────────────────────────────────────────────────────┤\n";
            cout << "  │  14. Display Department Network                           │\n";
            cout << "  │  15. Find Shortest Path (Dijkstra)                        │\n";
            cout << "  │  16. BFS Traversal                                        │\n";
            cout << "  │  17. DFS Traversal                                        │\n";
            cout << "  └───────────────────────────────────────────────────────────┘\n\n";

            cout << "  ┌───────────────────────────────────────────────────────────┐\n";
            cout << "  │  SYSTEM FEATURES                                          │\n";
            cout << "  ├───────────────────────────────────────────────────────────┤\n";
            cout << "  │  18. Generate Analytics & Reports                         │\n";
            cout << "  │  19. View Activity Log (Stack)                            │\n";
            cout << "  │  20. Save All Data to Files                               │\n";
            cout << "  │  21. Exit System                                          │\n";
            cout << "  └───────────────────────────────────────────────────────────┘\n\n";

            cout << "  Enter your choice: ";

            int choice;
            cin >> choice;

            switch (choice)
            {
            case 1:
                addPatient();
                break;
            case 2:
                searchPatient();
                break;
            case 3:
                displayAllPatients();
                break;
            case 4:
                deletePatient();
                break;
            case 5:
                addDoctor();
                break;
            case 6:
                searchDoctor();
                break;
            case 7:
                displayAllDoctors();
                break;
            case 8:
                deleteDoctor();
                break;
            case 9:
                scheduleAppointment();
                break;
            case 10:
                displayAllAppointments();
                break;
            case 11:
                cancelAppointment();
                break;
            case 12:
                displayEmergencyQueue();
                break;
            case 13:
                processEmergencyPatient();
                break;
            case 14:
                displayDepartmentNetwork();
                break;
            case 15:
                findShortestPath();
                break;
            case 16:
                performBFS();
                break;
            case 17:
                performDFS();
                break;
            case 18:
                generateAnalytics();
                break;
            case 19:
                viewActivityLog();
                break;
            case 20:
                saveAllData();
                pause();
                break;
            case 21:
                saveAllData();
                cout << "\n  Thank you for using Hospital Management System!\n";
                cout << "  All data has been saved successfully.\n\n";
                return;
            default:
                cout << "\n  Invalid choice! Please try again.\n";
                pause();
            }
        }
    }
};

// ==================== MAIN FUNCTION ====================
int main()
{
    try
    {
        HospitalManagementSystem hms;
        hms.run();
    }
    catch (const exception &e)
    {
        cerr << "\n  ✗ Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}