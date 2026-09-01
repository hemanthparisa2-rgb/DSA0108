#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// ---------- CITIZEN ----------
class Citizen {
    int id;
    string name;

public:
    Citizen() : id(0), name("Unknown") {}

    Citizen(int i, string n) : id(i), name(n) {}

    Citizen(const Citizen &c) {
        id = c.id;
        name = c.name;
    }

    int getId() { return id; }
    string getName() { return name; }
};


// ---------- BASE COMPLAINT ----------
class Complaint {
protected:
    int id, severity, affected, urgency, score;
    Citizen citizen;
    string description, status, department;

public:
    static int total;

    Complaint() :
        id(0), severity(1), affected(1), urgency(1),
        score(0), status("Pending"),
        department("Not Assigned") {}

    Complaint(int i, Citizen c, string d,
              int s, int a, int u)
        : id(i), citizen(c), description(d),
          severity(s), affected(a), urgency(u),
          status("Pending"), department("Not Assigned") {
        calculatePriority();
        total++;
    }

    Complaint(const Complaint &c) {
        id = c.id;
        citizen = c.citizen;
        description = c.description;
        severity = c.severity;
        affected = c.affected;
        urgency = c.urgency;
        score = c.score;
        status = c.status;
        department = c.department;
    }

    virtual ~Complaint() {}

    virtual string type() { return "General"; }
    virtual string dept() { return "General Services"; }
    virtual int typeScore() { return 1; }

    void calculatePriority() {
        int impact = (affected >= 50) ? 5 :
                     (affected >= 20) ? 4 :
                     (affected >= 5) ? 2 : 1;

        score = severity + impact + urgency + typeScore();
    }

    string level() {
        if (score >= 15) return "CRITICAL";
        if (score >= 11) return "HIGH";
        if (score >= 7) return "MEDIUM";
        return "LOW";
    }

    void assign() {
        department = dept();
    }

    void update(string s) {
        status = s;
    }

    int getId() { return id; }
    int getScore() { return score; }
    string getStatus() { return status; }

    // Operator overloading
    bool operator>(Complaint &c) {
        return score > c.score;
    }

    friend ostream& operator<<(ostream &out,
                               Complaint &c) {
        out << left << setw(6) << c.id
            << setw(12) << c.type()
            << setw(15) << c.status
            << setw(20) << c.department
            << setw(8) << c.score
            << c.level();
        return out;
    }
};

int Complaint::total = 0;


// ---------- ROAD ----------
class RoadComplaint : public Complaint {
public:
    RoadComplaint(int i, Citizen c, string d,
                   int s, int a, int u)
        : Complaint(i, c, d, s, a, u) {}

    string type() override {
        return "Road";
    }

    string dept() override {
        return "Road Maintenance";
    }

    int typeScore() override {
        return 3;
    }
};


// ---------- WATER ----------
class WaterComplaint : public Complaint {
public:
    WaterComplaint(int i, Citizen c, string d,
                    int s, int a, int u)
        : Complaint(i, c, d, s, a, u) {}

    string type() override {
        return "Water";
    }

    string dept() override {
        return "Water Supply";
    }

    int typeScore() override {
        return 5;
    }
};


// ---------- WASTE ----------
class WasteComplaint : public Complaint {
public:
    WasteComplaint(int i, Citizen c, string d,
                    int s, int a, int u)
        : Complaint(i, c, d, s, a, u) {}

    string type() override {
        return "Waste";
    }

    string dept() override {
        return "Sanitation";
    }

    int typeScore() override {
        return 2;
    }
};


// ---------- FIND COMPLAINT ----------
Complaint* find(vector<Complaint*> &v, int id) {

    for (Complaint *c : v)
        if (c->getId() == id)
            return c;

    return nullptr;
}


// ---------- MAIN ----------
int main() {

    vector<Complaint*> complaints;
    vector<Citizen> citizens;

    int citizenID = 101;
    int complaintID = 1001;
    int choice;

    do {

        cout << "\n\n===== URBAN COMPLAINT SYSTEM =====\n";
        cout << "1. Register Citizen\n";
        cout << "2. Register Complaint\n";
        cout << "3. View Complaints\n";
        cout << "4. Assign Department\n";
        cout << "5. Update Status\n";
        cout << "6. Compare Complaints\n";
        cout << "7. Service Report\n";
        cout << "8. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        // REGISTER CITIZEN
        if (choice == 1) {

            string name;

            cout << "Citizen Name: ";
            cin >> name;

            citizens.push_back(
                Citizen(citizenID++, name));

            cout << "Citizen registered.\n";
        }

        // REGISTER COMPLAINT
        else if (choice == 2) {

            if (citizens.empty()) {
                cout << "Register citizen first.\n";
                continue;
            }

            int cid, type, s, a, u;
            string d;

            cout << "Citizen ID: ";
            cin >> cid;

            Citizen *cptr = nullptr;

            for (Citizen &c : citizens)
                if (c.getId() == cid)
                    cptr = &c;

            if (!cptr) {
                cout << "Citizen not found.\n";
                continue;
            }

            cout << "\n1.Road  2.Water  3.Waste\n";
            cout << "Type: ";
            cin >> type;

            cout << "Description: ";
            cin >> d;

            cout << "Severity (1-5): ";
            cin >> s;

            cout << "Affected people: ";
            cin >> a;

            cout << "Urgency (1-5): ";
            cin >> u;

            Complaint *p = nullptr;

            if (type == 1)
                p = new RoadComplaint(
                    complaintID++, *cptr, d, s, a, u);

            else if (type == 2)
                p = new WaterComplaint(
                    complaintID++, *cptr, d, s, a, u);

            else if (type == 3)
                p = new WasteComplaint(
                    complaintID++, *cptr, d, s, a, u);

            else {
                cout << "Invalid type.\n";
                continue;
            }

            complaints.push_back(p);

            cout << "\nComplaint Registered!";
            cout << "\nPriority Score: "
                 << p->getScore();

            cout << "\nPriority Level: "
                 << p->level() << endl;
        }

        // VIEW
        else if (choice == 3) {

            cout << "\nID    Type        Status"
                 << "         Department           Score Level\n";

            cout << "------------------------------------------------------\n";

            for (Complaint *c : complaints)
                cout << *c << endl;
        }

        // ASSIGN
        else if (choice == 4) {

            int id;
            cout << "Complaint ID: ";
            cin >> id;

            Complaint *c = find(complaints, id);

            if (c) {
                c->assign();
                cout << "Department assigned: "
                     << c->dept() << endl;
            }
            else
                cout << "Complaint not found.\n";
        }

        // STATUS
        else if (choice == 5) {

            int id, s;

            cout << "Complaint ID: ";
            cin >> id;

            Complaint *c = find(complaints, id);

            if (c) {

                cout << "1.Pending  2.In Progress  3.Resolved\n";
                cin >> s;

                if (s == 1) c->update("Pending");
                else if (s == 2) c->update("In Progress");
                else if (s == 3) c->update("Resolved");

                cout << "Status updated.\n";
            }
            else
                cout << "Complaint not found.\n";
        }

        // COMPARE
        else if (choice == 6) {

            int a, b;

            cout << "First Complaint ID: ";
            cin >> a;

            cout << "Second Complaint ID: ";
            cin >> b;

            Complaint *c1 = find(complaints, a);
            Complaint *c2 = find(complaints, b);

            if (c1 && c2) {

                if (*c1 > *c2)
                    cout << a << " has higher priority.\n";

                else if (*c2 > *c1)
                    cout << b << " has higher priority.\n";

                else
                    cout << "Equal priority.\n";
            }
            else
                cout << "Complaint not found.\n";
        }

        // REPORT
        else if (choice == 7) {

            int pending = 0;
            int progress = 0;
            int resolved = 0;

            for (Complaint *c : complaints) {

                if (c->getStatus() == "Pending")
                    pending++;

                else if (c->getStatus() == "In Progress")
                    progress++;

                else if (c->getStatus() == "Resolved")
                    resolved++;
            }

            cout << "\n===== SERVICE REPORT =====\n";

            cout << "Total Complaints : "
                 << Complaint::total << endl;

            cout << "Pending          : "
                 << pending << endl;

            cout << "In Progress      : "
                 << progress << endl;

            cout << "Resolved         : "
                 << resolved << endl;
        }

    } while (choice != 8);


    // Destructor / memory cleanup
    for (Complaint *c : complaints)
        delete c;

    cout << "\nProgram ended successfully.\n";

    return 0;
}