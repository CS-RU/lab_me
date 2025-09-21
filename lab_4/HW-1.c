#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ฟังก์ชันเปรียบเทียบสำหรับ qsort
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// ฟังก์ชันให้ thread ใช้
void* sort_array(void* arg) {
    int* arr = (int*) arg;
    qsort(arr, 5, sizeof(int), compare);

    /*printf("Thread sorting:  ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");*/

    pthread_exit((void*) arr);  // ส่งผลลัพธ์กลับไปที่ main ด้วยคำสั่ง pthread_exit();
}

int main() {
    pthread_t tid;
    int numbers[5] = {5, 4, 3, 2, 1};
    int* sorted_array;

    printf("Before sorting: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    // สร้าง thread ให้ทำการ sort
    pthread_create(&tid, NULL, sort_array, (void*)numbers);
        
    // รอ thread ทำงานเสร็จ และรับค่าที่ return กลับมา
    pthread_join(tid, (void**)&sorted_array); //เก็บผลลัพธ์ที่ตัวแปร sorted_array

    printf("After sorting:  ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", sorted_array[i]);
    }
    printf("\n");

    return 0;
}
