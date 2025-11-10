#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAXQ 3000

MYSQL *connect_db() {
    MYSQL *conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, "localhost", "root", "Mahi@900", "flightdb", 3306, NULL, 0)) {
        printf("Database Connection Failed: %s\n", mysql_error(conn));
        exit(1);
    }
    return conn;
}

void input_str(const char *msg, char *buf, size_t size) {
    printf("%s", msg);
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = 0;
}

int register_user(MYSQL *conn) {
    char uname[50], pass[50];
    input_str("Choose Username: ", uname, sizeof(uname));
    input_str("Choose Password: ", pass, sizeof(pass));

    char q[MAXQ];
    snprintf(q, sizeof(q), "INSERT INTO users (username, password) VALUES ('%s', '%s')", uname, pass);

    if(mysql_query(conn, q)) {
        printf("Register failed: %s\n", mysql_error(conn));
        return -1;
    }

    printf("✅ Registration Successful.\n");
    return 1;
}

int login_user(MYSQL *conn) {
    char uname[50], pass[50];
    input_str("Username: ", uname, sizeof(uname));
    input_str("Password: ", pass, sizeof(pass));

    char q[MAXQ];
    snprintf(q, sizeof(q), "SELECT id FROM users WHERE username='%s' AND password='%s'", uname, pass);

    if(mysql_query(conn, q)) return -1;
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if(row) return atoi(row[0]); 
    else return -1;
}

void show_flights(MYSQL *conn) {
    const char *q =
      "SELECT flight_no, src, dst, depart_datetime, arrive_datetime, available_seats, fare "
      "FROM flights ORDER BY depart_datetime ASC";

    if(mysql_query(conn, q)) {
        printf("Query failed.\n");
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW r;

    printf("\nAvailable Flights:\n");
    printf("FlightNo | From -> To | Depart | Arrive | Seats | Fare\n");
    printf("--------------------------------------------------------------\n");

    while((r = mysql_fetch_row(res))) {
        printf("%s | %s -> %s | %s | %s | %s | %s\n",
               r[0], r[1], r[2], r[3], r[4], r[5], r[6]);
    }

    mysql_free_result(res);
    printf("--------------------------------------------------------------\n\n");
}

int book_flight(MYSQL *conn, int user_id) {
    show_flights(conn);

    char flight_no[50], seat_s[10];
    input_str("Enter Flight Number to book: ", flight_no, sizeof(flight_no));
    input_str("How many seats: ", seat_s, sizeof(seat_s));
    int seats = atoi(seat_s);

    mysql_query(conn, "START TRANSACTION");

    char q[MAXQ];
    snprintf(q, sizeof(q), "SELECT available_seats FROM flights WHERE flight_no='%s' FOR UPDATE", flight_no);
    mysql_query(conn, q);
    MYSQL_RES *res = mysql_store_result(conn);
    if(!res || mysql_num_rows(res) == 0) { mysql_query(conn, "ROLLBACK"); return 0; }
    MYSQL_ROW row = mysql_fetch_row(res);
    int available = atoi(row[0]);
    mysql_free_result(res);

    if(available < seats) { printf("Not enough seats.\n"); mysql_query(conn, "ROLLBACK"); return 0; }

    snprintf(q, sizeof(q), "UPDATE flights SET available_seats = available_seats - %d WHERE flight_no='%s'", seats, flight_no);
    mysql_query(conn, q);

    snprintf(q, sizeof(q), "INSERT INTO bookings (user_id, flight_no, seats) VALUES (%d, '%s', %d)", user_id, flight_no, seats);
    mysql_query(conn, q);

    mysql_query(conn, "COMMIT");
    printf("\n✅ Booking Successful!\n");
    return 1;
}

void show_active(MYSQL *conn, int user_id) {
    char q[MAXQ];
    snprintf(q, sizeof(q),
      "SELECT b.id, b.flight_no, f.src, f.dst, f.depart_datetime, b.seats "
      "FROM bookings b JOIN flights f ON b.flight_no=f.flight_no "
      "WHERE b.user_id=%d AND b.status='booked' ORDER BY f.depart_datetime", user_id);

    mysql_query(conn, q);
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW r;

    printf("\nActive Bookings:\nID | Flight | Route | Depart | Seats\n");
    printf("----------------------------------------\n");
    while((r = mysql_fetch_row(res)))
        printf("%s | %s | %s->%s | %s | %s\n", r[0], r[1], r[2], r[3], r[4], r[5]);
    mysql_free_result(res);
}

void cancel_booking(MYSQL *conn, int user_id) {
    show_active(conn, user_id);

    char id_s[10];
    input_str("Enter Booking ID to Cancel: ", id_s, sizeof(id_s));
    int bid = atoi(id_s);

    mysql_query(conn, "START TRANSACTION");

    char q[MAXQ];
    snprintf(q, sizeof(q), "SELECT flight_no, seats FROM bookings WHERE id=%d AND user_id=%d AND status='booked' FOR UPDATE", bid, user_id);
    mysql_query(conn, q);
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW r = mysql_fetch_row(res);
    if(!r) { mysql_query(conn, "ROLLBACK"); return; }

    char flight_no[50]; strcpy(flight_no, r[0]);
    int seats = atoi(r[1]);
    mysql_free_result(res);

    snprintf(q, sizeof(q), "UPDATE flights SET available_seats = available_seats + %d WHERE flight_no='%s'", seats, flight_no);
    mysql_query(conn, q);

    snprintf(q, sizeof(q), "UPDATE bookings SET status='cancelled' WHERE id=%d", bid);
    mysql_query(conn, q);

    mysql_query(conn, "COMMIT");
    printf("✅ Booking Cancelled.\n");
}

void show_cancelled(MYSQL *conn, int user_id) {
    printf("\nCancelled Bookings:\n");
    mysql_query(conn,
    "SELECT id, flight_no, seats, booking_time FROM bookings WHERE status='cancelled'");
}

void show_history(MYSQL *conn, int user_id) {
    printf("\nPast Travel (Completed Flights):\n");
    mysql_query(conn,
    "SELECT b.id, b.flight_no, b.seats FROM bookings b JOIN flights f ON b.flight_no=f.flight_no WHERE b.status='booked' AND f.depart_datetime < NOW()");
}

int main() {
    MYSQL *conn = connect_db();

    while(1) {
        printf("\n1) Register\n2) Login\n3) Exit\nChoice: ");
        int c; scanf("%d", &c); getchar();

        if(c==1) register_user(conn);
        else if(c==2) {
            int uid = login_user(conn);
            if(uid < 0) { printf("Login Failed.\n"); continue; }

            while(1) {
                printf("\n--- Main Menu ---\n1) List Flights\n2) Book\n3) My Bookings\n4) Cancel\n5) Logout\nChoice: ");
                scanf("%d",&c); getchar();
                if(c==1) show_flights(conn);
                else if(c==2) book_flight(conn, uid);
                else if(c==3) show_active(conn, uid);
                else if(c==4) cancel_booking(conn, uid);
                else break;
            }
        }
        else break;
    }

    mysql_close(conn);
    return 0;
}