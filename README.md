# ✈️ Flight Booking Management System (C + MySQL)
# KPIT 701(A) -> ABHI RAJ(23BCS13238) , VISHAL KUMAR(23BCS10306) , ANSHUL RANA(23BCS12076)

## 📖 Overview
This is a **terminal-based Flight Booking Management System** built in **C**, using **MySQL** as the backend database.  
It allows users to **register, login, view flights, book tickets, cancel bookings, and view booking history**.

---

## 🧩 Features
✅ User Registration & Login  
✅ View Available Flights  
✅ Book Flights with Seat Management  
✅ View Active Bookings  
✅ Cancel Bookings (Seats are restored)  
✅ Transaction Handling (Commit/Rollback)  
✅ Simple and Interactive CLI Interface  

---

## 🛠️ Requirements
Before running the program, ensure you have:

- **MySQL Server** installed and running  
- **MySQL C Connector** installed  
- **GCC Compiler** (MinGW for Windows / GCC for Linux/Mac)

---

## ⚙️ Database Setup

1. Open your MySQL shell or any MySQL client (like phpMyAdmin or MySQL Workbench).
2. Run the following SQL commands to create and populate the database:

```sql
CREATE DATABASE flightdb;
USE flightdb;

CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE,
    password VARCHAR(50)
);

CREATE TABLE flights (
    flight_no VARCHAR(20) PRIMARY KEY,
    src VARCHAR(50),
    dst VARCHAR(50),
    depart_datetime DATETIME,
    arrive_datetime DATETIME,
    available_seats INT,
    fare DOUBLE
);

CREATE TABLE bookings (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT,
    flight_no VARCHAR(20),
    seats INT,
    status ENUM('booked','cancelled') DEFAULT 'booked',
    booking_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (flight_no) REFERENCES flights(flight_no),
    FOREIGN KEY (user_id) REFERENCES users(id)
);
```

3. Insert flight data:

```sql
INSERT INTO flights (flight_no, src, dst, depart_datetime, arrive_datetime, available_seats, fare) VALUES
('AI101', 'Delhi', 'Mumbai', '2025-11-10 10:30:00', '2025-11-10 12:45:00', 120, 4500),
('AI202', 'Pune', 'Goa', '2025-11-11 14:00:00', '2025-11-11 15:05:00', 100, 3000),
('AI303', 'Chennai', 'Bangalore', '2025-11-12 08:45:00', '2025-11-12 09:50:00', 150, 3500),
('AI404', 'Jaipur', 'Delhi', '2025-11-13 18:30:00', '2025-11-13 19:50:00', 90, 2800),
('AI505', 'Kolkata', 'Hyderabad', '2025-11-14 07:15:00', '2025-11-14 09:30:00', 130, 4100),
('AI606', 'Lucknow', 'Delhi', '2025-11-15 13:00:00', '2025-11-15 14:25:00', 110, 2600),
('AI707', 'Delhi', 'Chennai', '2025-11-16 09:40:00', '2025-11-16 12:20:00', 140, 4800),
('AI808', 'Bangalore', 'Mumbai', '2025-11-17 06:50:00', '2025-11-17 08:55:00', 120, 4200),
('AI909', 'Hyderabad', 'Pune', '2025-11-18 17:10:00', '2025-11-18 18:30:00', 160, 2100),
('AI111', 'Mumbai', 'Ahmedabad', '2025-11-19 15:25:00', '2025-11-19 16:45:00', 90, 2400),
('AI222', 'Chandigarh', 'Delhi', '2025-11-20 11:00:00', '2025-11-20 12:05:00', 100, 2200),
('AI333', 'Goa', 'Bangalore', '2025-11-21 19:30:00', '2025-11-21 21:00:00', 130, 3200),
('AI444', 'Bhopal', 'Hyderabad', '2025-11-22 08:10:00', '2025-11-22 09:45:00', 150, 3500),
('AI555', 'Delhi', 'Varanasi', '2025-11-23 14:20:00', '2025-11-23 15:30:00', 170, 2800),
('AI666', 'Surat', 'Kolkata', '2025-11-24 07:55:00', '2025-11-24 10:45:00', 100, 4500),
('AI777', 'Nagpur', 'Delhi', '2025-11-25 12:10:00', '2025-11-25 13:55:00', 110, 3000),
('AI888', 'Patna', 'Lucknow', '2025-11-26 09:30:00', '2025-11-26 10:45:00', 140, 2100),
('AI999', 'Mumbai', 'Goa', '2025-11-27 18:45:00', '2025-11-27 19:55:00', 200, 2600);
```

---

## 💻 Compilation and Execution

### 🪟 Windows:
```bash
gcc main.c -o flight -IC:\Program Files\MySQL\MySQL Connector C 6.1\include -LC:\Program Files\MySQL\MySQL Connector C 6.1\lib -lmysql
flight.exe
```

### 🐧 Linux/Mac:
```bash
gcc main.c -o flight `mysql_config --cflags --libs`
./flight
```

---

## 🧠 Program Flow
```
1️⃣ Register
2️⃣ Login
     ├── List Flights
     ├── Book Flight
     ├── View My Bookings
     ├── Cancel Booking
     └── Logout
3️⃣ Exit
```

---

## 🧾 Example Output

```
1) Register
2) Login
3) Exit
Choice: 2
Username: abhi
Password: 123

--- Main Menu ---
1) List Flights
2) Book
3) My Bookings
4) Cancel
5) Logout
Choice: 1

Available Flights:
FlightNo | From -> To | Depart | Arrive | Seats | Fare
--------------------------------------------------------------
AI101 | Delhi -> Mumbai | 2025-11-10 10:30:00 | 2025-11-10 12:45:00 | 120 | 4500
...
```

---

## 🧑‍💻 Developer Info
**Author:** Shubham (Roll No: 23BCS13238)  
**Batch:** Sigma Batch Managers, Apna College  
**Database:** MySQL  
**Language:** C (GCC)
