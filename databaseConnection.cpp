
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <occi.h>
#include <iomanip>


using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;


int menu(void);
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);
void displayEmployee(Connection* conn, struct Employee emp);
void displayAllEmployees(Connection* conn);
void insertEmployee(Connection* conn, struct Employee employee);
void updateEmployee(Connection* conn, struct Employee employeeID);
void deleteEmployee(Connection* conn, struct Employee employeeID);

struct Employee
{
    int employeeNumber;
    char lastName[50];
    char firstName[50];
    char email[100];
    char phone[50];
    char extension[10];
    char reportsTo[100];
    char jobTitle[50];
    char city[50];

};


int main(void) {

    //OCCI variables
    Environment* env = nullptr;
    Connection* conn = nullptr;
    //USER variables
    string str;
    string usr = "dbs211_213h01";   //this is your login assigned to you
    string pass = "58382296";             //this is your password assigned to you
    string srv = "myoracle12c.senecacollege.ca:1521/oracle12c";
    try {
        env = Environment::createEnvironment(Environment::DEFAULT);
        conn = env->createConnection(usr, pass, srv);
        cout << "Connection is successful!" << endl;
        cout << "Group 10:" << endl;
        cout << "Juan David Rodriguez Castelblanco" << endl;
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    Employee* emp = new Employee();

    int s, flag = 0;

    while (flag != 1) {
        //Selection
        s = menu();

        switch (s) {
        case 1:
        {
            displayEmployee(conn, *emp);
            break;
        }
        case 2:
        {
            displayAllEmployees(conn);
            break;
        }
        case 3:
        {
            //MS3 FINAL
            insertEmployee(conn, *emp);
            break;
        }
        case 4:
        {
            //MS3 FINAL
            updateEmployee(conn, *emp);
            break;
        }
        case 5:
        {
            //MS3 FINAL
            deleteEmployee(conn, *emp);
            break;
        }
        case 0:
        {
            flag = 1;
            break;
        }
        default:
        {

            cout << "Invalid Selection, try again:" << endl;
        }

        }
    }
    return 0;
}

int menu(void) {

    int s = 0;

    //Menu
    cout << "**************************** HR Menu ****************************" << endl;
    cout << "1) Find Employee" << endl;
    cout << "2) Report of Employees" << endl;
    cout << "3) Add Employee" << endl;
    cout << "4) Update Employee" << endl;
    cout << "5) Remove Employee" << endl;
    cout << "0) Exit" << endl;
    cout << endl;
    cout << "Select an option:> ";
    cin >> s;

    cin.ignore();
    if (!cin.fail())
    {
        return s;
    }

}

int findEmployee(Connection* conn, int employeeNumber, Employee* emp) {
    int s = 0;
    string findEmp;

    Statement* stmt = conn->createStatement();

    ResultSet* rs = stmt->executeQuery("SELECT e.employeenumber, e.lastname, e.firstname, e.email, o.phone, e.extension, r.lastname || ' ' || r.firstname, e.jobtitle, o.city FROM employees E  LEFT JOIN offices o on(e.officecode = o.officecode) LEFT JOIN employees r on(e.reportsto = r.employeenumber) WHERE e.employeenumber = " + to_string(employeeNumber));

    //Access the employee dataand save it in the struct
    while (rs->next()) {
        emp->employeeNumber = rs->getInt(1);
        sprintf(emp->lastName, rs->getString(2).c_str());
        sprintf(emp->firstName, rs->getString(3).c_str());
        sprintf(emp->email, rs->getString(4).c_str());
        sprintf(emp->phone, rs->getString(5).c_str());
        sprintf(emp->reportsTo, rs->getString(6).c_str());
        sprintf(emp->extension, rs->getString(7).c_str());
        sprintf(emp->jobTitle, rs->getString(8).c_str());
        sprintf(emp->city, rs->getString(9).c_str());
        s = 1;
    }
    //all open objects should be terminated
    conn->terminateStatement(stmt);
    return s;
}

void displayAllEmployees(Connection* conn) {

    Statement* stmt = conn->createStatement();

    ResultSet* rs = stmt->executeQuery("SELECT e.employeenumber, e.firstname || ' ' || e.lastname, e.email, o.phone, e.extension, m.lastname || ' ' || m.firstname FROM employees E LEFT JOIN offices o on(e.officecode = o.officecode)LEFT JOIN employees m on(e.reportsto = m.employeenumber)ORDER by e.employeenumber");



    cout << endl;
    cout << setw(9) << left << "E";
    cout << setw(23) << left << "Employee Name";
    cout << setw(40) << left << "Email";
    cout << setw(32) << left << "Phone";
    cout << setw(30) << left << "Ext";
    cout << left << "Manager" << endl;

    for (int i = 0; i < 150; i++) {
        cout << "-";
    }
    cout << endl;

    while (rs->next()) {
        //Column variables according to the query
        int id = rs->getInt(1);
        string name = rs->getString(2);
        string email = rs->getString(3);
        string phone = rs->getString(4);
        string ext = rs->getString(5);
        string manager = rs->getString(6);

        // Print all the values to the screen
        cout << endl << setw(9) << left << id << setw(23) << left << name << setw(40) << left << email << setw(32) << left << phone << setw(30) << left << ext << setw(50) << left << manager << endl;

    }
    cout << endl;
    //all open objects should be terminated
    cout.flush();
    conn->terminateStatement(stmt);
}

void displayEmployee(Connection* conn, Employee emp) {
    int findEmp;
    bool flag = false;

    cout << "Enter employee number: ";
    while (!flag) {
        //1 – the entered employee number is the right data type before trying to query the database
        if (!(cin >> findEmp)) {
            cout << "Only digits allowed. Enter employee number: " << endl;
        }
        else
        {
            flag = true;
        }

        cin.clear();
        cin.ignore(1000, '\n');
    }
    cout << endl;

    //2 – the employee number exists in the database before trying to output the values to the console window
    try {
        if (findEmployee(conn, findEmp, &emp) > 0) {
            //if exists show employee data
            cout << "Employee Number = " << emp.employeeNumber << endl;
            cout << "Last Name = " << emp.lastName << endl;
            cout << "First Name = " << emp.firstName << endl;
            cout << "Email = " << emp.email << endl;
            cout << "Phone = " << emp.phone << endl;
            cout << "Reports To = " << emp.reportsTo << endl;
            cout << "Job Title = " << emp.jobTitle << endl;
            cout << "City = " << emp.city << endl;
        }
        else {
            cout << "Employee " << findEmp << " does not exist" << endl;
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

    cout << endl;
    cout.flush();
}
void insertEmployee(Connection* conn, Employee emp) {
    int empid;
    string lName;
    string fName;
    string email;
    int extension, offCode;
    string jobTitle;
    int reportsto = 1;

    bool flag = false;

    cout << "Enter Employee Information" << endl;
    cout << "Employee Number: ";
    cin >> empid;
    cout << "Last Name: ";
    cin >> lName;
    cout << "First Name: ";
    cin >> fName;
    cout << "Email: ";
    cin >> email;
    cout << "Extension: ";
    cin >> extension;
    cout << "Office code: ";
    cin >> offCode;
    cout << "Job Title: ";
    cin >> jobTitle;
    cout << endl;

    try {
        if (findEmployee(conn, empid, &emp) > 0) {
            cout << "An employee with the same employee number exists." << endl;
        }
        else {

            Statement* stmt = conn->createStatement("Insert into employees(employeenumber, lastname, firstname,extension,email, officecode,reportsto, jobtitle) values(" + to_string(empid) + "," + "'" + lName + "'" + "," + "'" + fName + "'" + "," + to_string(extension) + "," + "'" + email + "'" + "," + to_string(offCode) + "," + to_string(reportsto) + "," + "'" + jobTitle + "'" + ")");
            stmt->executeQuery();
            conn->commit();

            conn->terminateStatement(stmt);
            cout << "The new employee is added successfully." << endl;

        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }


    cout << endl;
    cout.flush();

}

void updateEmployee(Connection* conn, Employee empid) {
    int empID;
    string newExtension;
    bool flag = false;
    cout << "Enter employee number: ";
    do {
        if (!(cin >> empID)) {
            cout << "Wrong input! Please try again > " << endl;
        }
        else {
            flag = true;
        }

        cin.clear();
        cin.ignore(1000, '\n');
    } while (!flag);
    cout << endl;

    try {
        if (findEmployee(conn, empID, &empid) > 0) {
            cout << "New Extension: ";
            cin >> newExtension;

            Statement* stmt = conn->createStatement("UPDATE employees SET extension = " + newExtension + " WHERE employeenumber = " + to_string(empID));
            conn->commit();
            cout.flush();
            conn->terminateStatement(stmt);
            cout << "The employee " << empID << " has been updated." << endl;
        }
        else {
            cout << "The employee " << empID << " does not exist." << endl;
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }
    cout << endl;
    cout.flush();
}

void deleteEmployee(Connection* conn, Employee emp) {
    int empID;
    bool flag = false;

    cout << "Enter employee number: ";
    do {
        if (!(cin >> empID)) {
            cout << "Wrong input! Please try again > " << endl;
        }
        else {
            flag = true;
        }

        cin.clear();
        cin.ignore(1000, '\n');
    } while (!flag);

    cout << endl;
    try {
        if (findEmployee(conn, empID, &emp) > 0) {
            Statement* stmt = conn->createStatement("DELETE FROM employees WHERE employeenumber = " + to_string(empID));
            stmt->executeQuery();
            conn->commit();
            conn->terminateStatement(stmt);
            cout << "The employee " << empID << " is deleted." << endl;
        }
        else {
            cout << "The employee " << empID << " does not exist." << endl;
        }

    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

    cout << endl;
    cout.flush();
}

