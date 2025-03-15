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
    int getId() const { return id; }
    string getName() const { return name; }
    virtual void displayDetails() const = 0;
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
    while (true) {
        cout << prompt;
        cin >> input;

        bool isValid = true;
        for (char c : input) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }

        if (isValid) {
            return stoi(input);
        } else {
            cout << "Invalid input! Please enter a valid positive number.\n";
        }
    }
}

double getValidDouble(string prompt) {
    string input;
    while (true) {
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

        if (isValid && !input.empty() && input != ".") {
            return stod(input);
        } else {
            cout << "Invalid input! Please enter a valid positive amount.\n";
        }
    }
}

int main() {
    vector<Employee*> employees;
    unordered_set<int> idSet;
    string choice;

    do {
        cout << "\n----- Menu -----\n"
             << "1 - Full-time Employee\n"
             << "2 - Part-time Employee\n"
             << "3 - Contractual Employee\n"
             << "4 - Display Payroll Report\n"
             << "5 - Exit\n"
             << "Enter choice: ";
        cin >> choice;

        bool isValid = true;
        for (char c : choice) {
            if (!isdigit(c)) {
                isValid = false;
                break;
            }
        }

        if (!isValid || choice.empty() || stoi(choice) < 1 || stoi(choice) > 5) {
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
            continue;
        }

        int menuChoice = stoi(choice);

        if (menuChoice == 1 || menuChoice == 2 || menuChoice == 3) {
            int id;
            string name;

            while (true) {
                id = getValidInt("Enter Employee ID: ");
                if (idSet.find(id) == idSet.end()) {
                    idSet.insert(id);
                    break;
                }
                cout << "Duplicate ID! Enter a unique ID.\n";
            }

            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);

            if (menuChoice == 1) {
                double salary = getValidDouble("Enter Fixed Salary: ");
                employees.push_back(new FullTimeEmployee(id, name, salary));
            }
            else if (menuChoice == 2) {
                double wage = getValidDouble("Enter Hourly Wage: ");
                int hours = getValidInt("Enter Hours Worked: ");
                employees.push_back(new PartTimeEmployee(id, name, wage, hours));
            }
            else {
                double payment = getValidDouble("Enter Contract Payment Per Project: ");
                int projects = getValidInt("Enter Number of Projects: ");
                employees.push_back(new ContractualEmployee(id, name, payment, projects));
            }

            cout << "Employee added successfully!\n";
        }
        else if (menuChoice == 4) {
            cout << "\n------ Employee Payroll Report ------\n";
            if (employees.empty()) {
                cout << "No employees to display.\n";
            } else {
                for (const auto& emp : employees) {
                    emp->displayDetails();
                }
            }
        }
        else if (menuChoice == 5) {
            cout << "Exiting program...\n";
        }
    } while (stoi(choice) != 5);

    for (auto emp : employees) {
        delete emp;
    }

    return 0;
}
