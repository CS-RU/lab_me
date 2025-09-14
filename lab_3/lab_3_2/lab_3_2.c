#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// int sum(int a, int b) {
//     return a + b;
// }

#define _GNU_SOURCE //define เพื่อมาใช้ strdup ไม่งั้นจะ error:Segmentation fault (core dumped)
#define MAX_WORD 500
#define MAX_PREP 500

// เก็บ prepositions
char *prepositions[MAX_PREP];
int prep_count = 0;


// ฟังก์ชันโหลด prepositions จากไฟล์
void load_prepositions(const char *filename) {
    FILE *fp = fopen(filename, "r");
    char buf[128];
    while (fscanf(fp, "%127s", buf) == 1) {
        prepositions[prep_count] = strdup(buf); /*บรรทัดนี้ทำให้เกิด  lab_3_2.c:24:36: warning: incompatible implicit declaration of built-in function ‘strdup’ [-Wbuiltin-declaration-mismatch] และ error Segmentation fault (core dumped)*/
        prep_count++;
    }
    fclose(fp);
}


// ฟังก์ชันนับ prepositions ในไฟล์
void count_prepositions(const char *filename, int counts[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("open lot file"); exit(1); }
    char buf[MAX_WORD];
    while (fscanf(fp, "%99s", buf) == 1) {
        for (int i = 0; i < prep_count; i++) {
            if (strcmp(buf, prepositions[i]) == 0) {
                counts[i]++;
                break;
            }
        }
    }
    fclose(fp);
}

int main() {
    load_prepositions("preposition.txt");

    char *lot_files[3] = {"lot1.txt", "lot2.txt", "lot3.txt"};
    int pipes[3][2];

    // สร้าง pipe
    for (int i = 0; i < 3; i++) {
        if (pipe(pipes[i]) == -1) { perror("pipe"); exit(1); }
    }

    // fork 3 ลูก
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork(); //จะวนเรียก fork 3 รอบ = ลูก 3 คน
        if (pid < 0) { perror("fork"); exit(1); }

        if (pid == 0) {
            // child
            printf("Child %d (PID: %d, Parent PID: %d)\n", (i+1) ,getpid(), getppid());
            //จะเห็นว่าลูกแต่ละตัวจะมี pid ของตัวเอง โดยที่มีแม่คนเดียวกัน

            close(pipes[i][0]); // ปิดอ่าน (ต่อ pipe)
            
            //เขียนผลลัพธ์ ของลูกแต่ละตัวส่งไปให้แม่
            int *counts = calloc(prep_count, sizeof(int));
            count_prepositions(lot_files[i], counts);
            write(pipes[i][1], counts, sizeof(int)*prep_count); 
            //write(pipes[1]เป็น 1 เพราะจะwrite, &counts ตัวแปรที่จะ write, sizeof(int)*prep_count ขนาดของตัวแปรที่ write);
            close(pipes[i][1]);

            free(counts);
            exit(0);
        }
        else {
            close(pipes[i][1]); // parent ปิดเขียน
        }
    }

    // parent อ่านจากทุก pipe
    int *final_counts = calloc(prep_count, sizeof(int));
    for (int i = 0; i < 3; i++) {
        int *temp = calloc(prep_count, sizeof(int));
        read(pipes[i][0], temp, sizeof(int)*prep_count);
        for (int j = 0; j < prep_count; j++) {
            final_counts[j] += temp[j];
        }
        free(temp);
        close(pipes[i][0]);
        wait(NULL);
    }

    // สร้างชื่อไฟล์ output พร้อมเวลา
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char filename[128];
    strftime(filename, sizeof(filename), "output_%Y_%m_%d_%H_%M_%S.csv", t);

    FILE *out = fopen(filename, "w");
    if (!out) { perror("open output"); exit(1); }
    fprintf(out, "WORD|COUNT\n");
    for (int i = 0; i < prep_count; i++) {
        if (final_counts[i] > 0)
            fprintf(out, "%s|%d\n", prepositions[i], final_counts[i]);
    }
    fclose(out);

    printf("✅ Output saved to %s\n", filename);

    // cleanup
    free(final_counts);
    for (int i = 0; i < prep_count; i++) free(prepositions[i]);
    return 0;
}