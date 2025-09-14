#include <iostream>
#include <fstream> //ใช้ทั้ง istream, ostream include ครั้งเดียวได้ด้วย <fstream>
#include <sstream> //สำหรับ จัดการข้อมูลที่อยู่ใน string
#include <string>
#include <unistd.h>   // สำหรับ fork(), getpid()
#include <sys/wait.h> // สำหรับ wait()
using namespace std;

struct SalesRecord {
    int orderId;
    string date;
    string region;
    string product;
    double amount;
    string month;
    SalesRecord *next;
};

// สร้าง node ใหม่
SalesRecord* createNode(int orderId, const string date, const string region,
                        const string product, double amount, const string month) {
    SalesRecord *node = new SalesRecord;
    node->orderId = orderId;
    /*#include <cstring> สำหรับ strcpy()
    เวลา assign ต้องเขียนแบบนี้เพราะใช้char
    strcpy(node->date, date.c_str());
    strcpy(node->region, region.c_str());
    strcpy(node->product, product.c_str());
    strcpy(node->month, month.c_str());
    */
    node->date = date;
    node->region = region;
    node->product = product;
    node->amount = amount;
    node->month = month; 
    node->next = nullptr;
    return node;
}

// โหลดข้อมูลจากไฟล์ CSV
void loadCSV(SalesRecord* &head, const string &filename, const string &month) {
    //เปิดไฟล์เพื่ออ่านตามชื่อที่ส่งเข้ามา
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // ข้าม header

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, date, region, product, amountStr;

        getline(ss, idStr, ',');
        getline(ss, date, ',');
        getline(ss, region, ',');
        getline(ss, product, ',');
        getline(ss, amountStr, ',');

        int id = stoi(idStr); //stoi someting to int แปลงidStr -> เป็น int
        double amount = stod(amountStr); //someting to double

        // cout << "id: "<< id << endl;
        // cout << "date: "<< date << endl;
        // cout << "region: "<< region << endl;
        // cout << "product: "<< product<< endl;
        // cout << "amountStr: "<< amountStr << endl;

        SalesRecord *newNode = createNode(id, date, region, product, amount, month);
        newNode->next = head;
        head = newNode;
    }

    file.close();
}

// ข้อ1. หายอดรวมตาม region
double totalByRegion(SalesRecord *head, const string &region) {
    double sum = 0;
    while (head) {
        if (head->region == region) {
            sum += head->amount;
        }
        head = head->next;
    }
    return sum;
}

// ข้อ2. หายอดรวมตาม product
double totalByProduct(SalesRecord *head, const string &product) {
    double sum = 0;
    while (head) {
        if (head->product == product) {
            sum += head->amount;
        }
        head = head->next;
    }
    return sum;
}

// ข้อ3. ค้นหาจาก OrderID
SalesRecord* findByOrderID(SalesRecord *head, int orderId) {
    while (head) {
        if (head->orderId == orderId) {
            return head;
        }
        head = head->next;
    }
    return nullptr;
}

int main() {
    SalesRecord *head = nullptr;

    // โหลดข้อมูลจากทั้ง 3 ไฟล์
    loadCSV(head, "sales_january.csv", "January"); 
    loadCSV(head, "sales_february.csv", "February");
    loadCSV(head, "sales_march.csv", "March");//จะได้ linklist ของ3 เดือน มาต่อกัน

    int choice;
    cout << "1. Total sales by region" << endl;
    cout << "2. Total sales by product" << endl;
    cout << "3. Find record by OrderID" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    pid_t pid = fork();
    if (pid > 0) {
        // parent process
        cout << "Process ID " << getpid() << ": Please wait...." << endl;
        wait(nullptr);

    } else if (pid == 0) {
        // child process
        if (choice == 1) {
            string region;
            cout << "Process ID " << getpid() << ": Please input region: ";
            cin >> region;
            double total = totalByRegion(head, region);
            cout << "Process ID " << getpid() 
                 << ": The total amount of region " << region 
                 << " is " << total << endl;

        } else if (choice == 2) {
            string product;
            cout << "Process ID " << getpid() << ": Please input product name: ";
            cin.ignore(); // ล้าง newline ค้าง
            getline(cin, product);
            double total = totalByProduct(head, product);
            cout << "Process ID " << getpid() 
                 << ": The total amount of \"" << product 
                 << "\" is " << total << endl;

        } else if (choice == 3) {
            int id;
            cout << "Process ID " << getpid() << ": Please input OrderID: ";
            cin >> id;
            SalesRecord *result = findByOrderID(head, id);
            if (result) {
                cout << "Process ID " << getpid() 
                     << ": Found in " << result->month << endl;
                cout << "OrderID = " << result->orderId << endl;
                cout << "Date = " << result->date << endl;
                cout << "Region = " << result->region << endl;
                cout << "Product = " << result->product << endl;
                cout << "Amount = " << result->amount << endl;

            } else {
                cout << "Process ID " << getpid() 
                     << ": OrderID " << id << " not found." << endl;
            }
        }
    }

    return 0;
}