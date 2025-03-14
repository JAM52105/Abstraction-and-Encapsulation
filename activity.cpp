#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <unordered_set>
#include <cctype>

using namespace std;

class Employee {
protected:
    int id;
    string name;

public:
    Employee(int empId, string empName) : id(empId), name(empName) {}

    virtual double calculateSalary() const = 0;
    virtual void displayDetails() const = 0;

    int getId() const { return id; }
    string getName() const { return name; }
};

class FullTimeEmployee : public Employee {
private:
    double fixedSalary;

public:
    FullTimeEmployee(int empId, string empName, double salary)
        : Employee(empId, empName), fixedSalary(salary) {}

    double calculateSalary() const override {
        return fixedSalary;
    }

    void displayDetails() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n"
             << "Fixed Monthly Salary: $" << fixedSalary << "\n\n";
    }
};

class PartTimeEmployee : public Employee {
private:
    double hourlyWage;
    int hoursWorked;

public:
    PartTimeEmployee(int empId, string empName, double wage, int hours)
        : Employee(empId, empName), hourlyWage(wage), hoursWorked(hours) {}

    double calculateSalary() const override {
        return hourlyWage * hoursWorked;
    }

    void displayDetails() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n"
             << "Hourly Wage: $" << hourlyWage << "\n"
             << "Hours Worked: " << hoursWorked << "\n"
             << "Total Salary: $" << calculateSalary() << "\n\n";
    }
};

class ContractualEmployee : public Employee {
private:
    double paymentPerProject;
    int projectsCompleted;

public:
    ContractualEmployee(int empId, string empName, double payment, int projects)
        : Employee(empId, empName), paymentPerProject(payment), projectsCompleted(projects) {}

    double calculateSalary() const override {
        return paymentPerProject * projectsCompleted;
    }

    void displayDetails() const override {
        cout << "Employee: " << name << " (ID: " << id << ")\n"
             << "Contract Payment Per Project: $" << paymentPerProject << "\n"
             << "Projects Completed: " << projectsCompleted << "\n"
             << "Total Salary: $" << calculateSalary() << "\n\n";
    }
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidInt(string prompt) {
    string input;
    cout << prompt;
    cin >> input;

    for (char c : input) {
        if (!isdigit(c)) {
            cout << "Invalid input! Returning to menu...\n";
            return -1;
        }
    }
    return stoi(input);
}

double getValidDouble(string prompt) {
    string input;
    cout << prompt;
    cin >> input;

    bool isValid = true;
    int dotCount = 0;

    for (char c : input) {
        if (!isdigit(c)) {
            if (c == '.' && dotCount == 0) {
                dotCount++;
            } else {
                isValid = false;
                break;
            }
        }
    }

    if (!isValid || input.empty() || input == ".") {
        cout << "Invalid input! Returning to menu...\n";
        return -1;
    }

    return stod(input);
}

int main() {
    vector<Employee*> employees;
    unordered_set<int> idSet;
    int choice;

    do {
        cout << "\n----- Menu -----\n"
             << "1 - Full-time Employee\n"
             << "2 - Part-time Employee\n"
             << "3 - Contractual Employee\n"
             << "4 - Display Payroll Report\n"
             << "5 - Exit\n"
             << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input! Returning to menu...\n";
            clearInput();
            continue;
        }

        if (choice == 1 || choice == 2 || choice == 3) {
            int id;
            string name;

            while (true) {
                id = getValidInt("Enter Employee ID: ");
                if (id == -1) break;
                if (idSet.find(id) == idSet.end()) {
                    idSet.insert(id);
                    break;
                }
                cout << "Duplicate ID! Enter a unique ID.\n";
            }
            if (id == -1) continue;

            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);

            if (choice == 1) {
                double salary = getValidDouble("Enter Fixed Salary: ");
                if (salary == -1) continue;
                employees.push_back(new FullTimeEmployee(id, name, salary));
            }
            else if (choice == 2) {
                double wage = getValidDouble("Enter Hourly Wage: ");
                if (wage == -1) continue;
                int hours = getValidInt("Enter Hours Worked: ");
                if (hours == -1) continue;
                employees.push_back(new PartTimeEmployee(id, name, wage, hours));
            }
            else {
                double payment = getValidDouble("Enter Contract Payment Per Project: ");
                if (payment == -1) continue;
                int projects = getValidInt("Enter Number of Projects: ");
                if (projects == -1) continue;
                employees.push_back(new ContractualEmployee(id, name, payment, projects));
            }

            cout << "Employee added successfully!\n";
        }
        else if (choice == 4) {
            cout << "\n------ Employee Payroll Report ------\n";
            if (employees.empty()) {
                cout << "No employees to display.\n";
            } else {
                for (const auto& emp : employees) {
                    emp->displayDetails();
                }
            }
        }
        else if (choice == 5) {
            cout << "Exiting program...\n";
        }
        else {
            cout << "Invalid choice! Returning to menu...\n";
        }

    } while (choice != 5);

    for (auto emp : employees) {
        delete emp;
    }

    return 0;
}