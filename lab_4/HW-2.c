#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define KEY 3  // กำหนด shift สำหรับ Caesar Cipher

// โครงสร้างส่งข้อมูลระหว่าง main กับ thread
typedef struct {
    char encrypted[256]; // ข้อความที่เข้ารหัส
    char decrypted[256]; // ข้อความถอดรหัส
    int key;
} Message;

// ฟังก์ชันถอดรหัส (Thread ทำงาน)
void *decrypt_thread(void *arg) {
    Message *msg = (Message *)arg;
    int len = strlen(msg->encrypted);

    printf("Decoding . . .\n");
    sleep(1);

    for (int i = 0; i < len; i++) {
        char c = msg->encrypted[i];
        if (c >= 'A' && c <= 'Z') {
            msg->decrypted[i] = (c - 'A' - msg->key + 26) % 26 + 'A';
        } else if (c >= 'a' && c <= 'z') {
            msg->decrypted[i] = (c - 'a' - msg->key + 26) % 26 + 'a';
        } else {
            msg->decrypted[i] = c; // ถ้าไม่ใช่ตัวอักษรให้คงไว้
        }
    }
    msg->decrypted[len] = '\0';

    pthread_exit(NULL);
}

// ฟังก์ชันเข้ารหัส (ทำใน main)
void caesar_encrypt(char *text, char *encrypted, int key) {
    int len = strlen(text);

    printf("Encoding . . .\n");
    sleep(1);
    for (int i = 0; i < len; i++) {
        char c = text[i];
        if (c >= 'A' && c <= 'Z') {
            encrypted[i] = (c - 'A' + key) % 26 + 'A';
        } else if (c >= 'a' && c <= 'z') {
            encrypted[i] = (c - 'a' + key) % 26 + 'a';
        } else {
            encrypted[i] = c;
        }
    }
    encrypted[len] = '\0';
}

int main() {
    pthread_t tid;
    Message msg;
    char text[256];

    printf("Enter message: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; // ตัด newline

    // เข้ารหัสใน main
    caesar_encrypt(text, msg.encrypted, KEY);

    printf("Encryption message: %s\n", msg.encrypted);

    msg.key = KEY;

    // สร้าง Thread เพื่อถอดรหัส
    pthread_create(&tid, NULL, decrypt_thread, (void *)&msg);
  

    // รอให้ Thread ทำงานเสร็จ
    pthread_join(tid, NULL);

    // main แสดงผลลัพธ์ที่ได้จาก Thread
    printf("Decryption message: %s\n", msg.decrypted);

    return 0;
}
