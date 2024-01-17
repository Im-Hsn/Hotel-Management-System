# Hotel Management & Reservation System

## Overview
This C++ program aims to develop a hotel reservation application. It allows users to manage room reservations, handle user authentication, and generate invoices. The application utilizes CSV files as a database and features a modern, user-friendly interface.

## Functionality
1. **User Authentication:**
   - New users are prompted to create an account with unique identifiers.
   - Returning users authenticate using their email address and password.
   - There are various validation checks including proper email entered and password following the password guidelines.

2. **Administrator and Client Roles:**
   - **Administrator:**
     - Add, delete, or modify room data in the "room.csv" file.
   - Administer room-related tasks through the console.
   - Maintain data integrity by checking for room redundancy.
   - **Client:**
     - Reserve a room by specifying the reservation dates.
     - View available rooms on selected dates.
     - Cancel reservations and choose from the client's reserved rooms.

3. **Reservation Handling:**
   - Store room information in the "reservation.csv" file.
   - Calculate the total price, considering a tax of 11%.
   - Generate an invoice in a PDF file, including the user's name and sorted reserved rooms by price.

## Development
- **Structures:**
  - Client Structure: int ID, string firstName, string lastName, string password, string address, string tel, room *r.
  - Room Structure: int num, string address, string type, double price, string *options, date start_date, date end_date.
  - Date Structure: int day, int month, int year.

- **File Handling:**
  - Utilizes CSV files as a database for storage.
  - Implements MD5 encryption for password security.

- **Programming Concepts:**
  - Incorporates functions, procedures, dynamic arrays, and pointers.

## Interface
- Utilizes a visually appealing and modern interface.
- Launches in full screen for an enhanced user experience.
- Eliminates unnecessary prints for a clean presentation.

**Note:** This project enhances the hotel reservation system with a user-friendly interface, better security measures, and efficient data storage using CSV files.
