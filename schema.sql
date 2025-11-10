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

USE flightdb;

INSERT INTO flights (flight_no, src, dst, depart_datetime, arrive_datetime, available_seats, fare) VALUES
('AI101', 'Delhi', 'Mumbai', '2025-11-10 10:30:00', '2025-11-10 12:45:00', 120, 4500),
('AI202', 'Pune', 'Goa', '2025-11-11 14:00:00', '2025-11-11 15:05:00', 100, 3000),
('AI303', 'Chennai', 'Bangalore', '2025-11-12 08:45:00', '2025-11-12 09:50:00', 150, 3500),
('AI404', 'Jaipur', 'Delhi', '2025-11-13 18:30:00', '2025-11-13 19:50:00', 90, 2800),
('AI505', 'Kolkata', 'Hyderabad', '2025-11-14 07:15:00', '2025-11-14 09:30:00', 130, 4100),
('AI606', 'Lucknow', 'Delhi', '2025-11-15 13:00:00', '2025-11-15 14:25:00', 110, 2600);


INSERT INTO flights (flight_no, src, dst, depart_datetime, arrive_datetime, available_seats, fare) VALUES
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

SELECT * FROM flights;