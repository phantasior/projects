#include "../lib/MySQL.h"

#include <iostream>

using std::cout;
using std::endl;

void test1() {
    MySQL db("my_db.db");

    db.Query(R"(
        CREATE TABLE students (
            student_id INT PRIMARY KEY,
            name VARCHAR,
            major VARCHAR,
            is_guy BOOL,
            height DOUBLE
        );
    )");

    db.Query(R"(INSERT INTO students (student_id, name, major, is_guy) VALUES (13, "Oleg", "IT", true, 17.3);)");
    db.Query(R"(INSERT INTO students (student_id, name, major, is_guy) VALUES (23, "Artyom", "Meth", false, 18.3);)");
    db.Query(R"(INSERT INTO students (student_id, name, major, is_guy) VALUES (17, "Ivan", "xui", false, 13.4);)");

    auto q1 = db.Query(R"(SELECT student_id, name, major, is_guy FROM students;)");
    auto q2 = db.Query(R"(SELECT student_id, name, major FROM students;)");

    cout << db.Query(R"(
        SELECT student_id, name, major
        FROM students
        WHERE ((((name != "Oleg") AND (name != "XTWE")) AND (major != "SDFS")) OR (name = "Oleg"));
    )");

    auto q4 = db.Query(R"(DROP TABLE students;)");
}

void test2() {
    MySQL db("mysql.db");
    db.Query(R"(
        CREATE TABLE persons (
            PersonId INT PRIMARY KEY,
            LastName VARCHAR,
            FirstName VARCHAR,
            Age INT
        );
    )");

    db.Query(R"(INSERT INTO persons (PersonId, LastName, FirstName, Age) VALUES (1, "Marchenko", "Artyom", 18);)");
    db.Query(R"(INSERT INTO persons (PersonId, LastName, FirstName, Age) VALUES (2, "Bolt", "Usain", 32);)");
    db.Query(R"(INSERT INTO persons (PersonId, LastName, FirstName, Age) VALUES (3, "Lee", "Stan", 99);)");

    db.Query(R"(
        CREATE TABLE orders (
            OrderID INT PRIMARY KEY,
            OrderNumber INT,
            PersonID INT FOREIGN KEY REFERENCES persons(PersonID)
        );
    )");

    db.Query(R"(INSERT INTO orders (OrderID, OrderNumber, PersonID) VALUES (1, 123123, 3);)");
    db.Query(R"(INSERT INTO orders (OrderID, OrderNumber, PersonID) VALUES (2, 321321, 3);)");
    db.Query(R"(INSERT INTO orders (OrderID, OrderNumber, PersonID) VALUES (3, 123456, 3);)");
    db.Query(R"(INSERT INTO orders (OrderID, OrderNumber, PersonID) VALUES (4, 654321, 3);)");

    db.Query(R"(UPDATE orders  SET OrderID = 100,  OrderNumber = 239, PersonID = 2 WHERE (OrderNumber = 123123);)");
    db.Query(R"(DELETE FROM orders WHERE (OrderID != 3);)");
    cout << db.Query(R"(SELECT * FROM orders)");
}

void test3() {
    MySQL db("join.db");

    cout << db.Query(R"(
        CREATE TABLE Customers (
            CustomerID INT PRIMARY KEY,
            CustomerName VARCHAR,
            Country VARCHAR
        );
    )");

    db.Query(R"(INSERT INTO Customers (CustomerID, CustomerName, Country) VALUES (1, "Oleg", "Russia");)");
    db.Query(R"(INSERT INTO Customers (CustomerID, CustomerName, Country) VALUES (2, "Alex", "USA");)");
    db.Query(R"(INSERT INTO Customers (CustomerID, CustomerName, Country) VALUES (3, "Shan-Chi", "China");)");

    db.Query(R"(
        CREATE TABLE Orders (
            OrderID INT PRIMARY KEY,
            CustomerID INT FOREIGN KEY REFERENCES Customers(CustomerID),
            OrderDate VARCHAR
        );
    )");

    db.Query(R"(INSERT INTO Orders (OrderID, CustomerID, OrderDate) VALUES (10248, 1, "1998-02-13");)");
    db.Query(R"(INSERT INTO Orders (OrderID, CustomerID, OrderDate) VALUES (10249, 2, "1998-02-13");)");
    db.Query(R"(INSERT INTO Orders (OrderID, CustomerID, OrderDate) VALUES (10250, 3, "1912-12-123");)");
    db.Query(R"(INSERT INTO Orders (OrderID, CustomerID, OrderDate) VALUES (10250, 4, "1912-12-123");)");

    cout << db.Query(R"(SELECT * FROM Orders)") << endl << endl;
    cout << db.Query(R"(SELECT * FROM Customers)") << endl << endl;

    cout << db.Query(R"(
        SELECT *
        FROM Orders
        INNER JOIN Customers ON Orders.CustomerID=Customers.CustomerID
    )");
}

int main() {
//    cout << "TEST 1\n";
//    test1();
//    cout << endl << endl;
//    cout << "TEST 2\n";
//    test2();
//    cout << endl << endl;
    cout << "TEST 3\n";
    test3();
}