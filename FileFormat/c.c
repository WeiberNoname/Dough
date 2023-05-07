#include <stdio.h>
#include <stdlib.h>

struct Employee {
    char name[50];
    int age;
    float salary;
};

struct FileHeader {
    int version;
    int numRecords;
};

void writeEmployee(FILE* fp, const struct Employee* emp) {
    fwrite(emp, sizeof(struct Employee), 1, fp);
}

void writeEmployeeArray(FILE* fp, const struct Employee* empArray, int numRecords) {
    int i;
    for (i = 0; i < numRecords; i++) {
        writeEmployee(fp, &empArray[i]);
    }
}

void readEmployee(FILE* fp, struct Employee* emp) {
    fread(emp, sizeof(struct Employee), 1, fp);
}

void readEmployeeArray(FILE* fp, struct Employee* empArray, int numRecords) {
    int i;
    for (i = 0; i < numRecords; i++) {
        readEmployee(fp, &empArray[i]);
    }
}

void createEmployeeFile(const char* filename, const struct Employee* empArray, int numRecords) {
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    struct FileHeader header = {1, numRecords};
    fwrite(&header, sizeof(struct FileHeader), 1, fp);
    writeEmployeeArray(fp, empArray, numRecords);
    fclose(fp);
}

void readEmployeeFile(const char* filename, struct Employee* empArray, int* numRecords) {
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    struct FileHeader header;
    fread(&header, sizeof(struct FileHeader), 1, fp);
    *numRecords = header.numRecords;
    readEmployeeArray(fp, empArray, *numRecords);
    fclose(fp);
}

int main() {
    const char* filename = "employees.bin";

    struct Employee employees[] = {
        {"John Doe", 25, 50000.0f},
        {"Jane Smith", 30, 60000.0f},
        {"Bob Johnson", 35, 70000.0f}
    };

    int numEmployees = sizeof(employees) / sizeof(struct Employee);

    // create a new file with employee data
    createEmployeeFile(filename, employees, numEmployees);

    // read employee data from the file
    struct Employee* readEmployees = malloc(numEmployees * sizeof(struct Employee));
    int readNumEmployees;
    readEmployeeFile(filename, readEmployees, &readNumEmployees);

    // print the employee data
    printf("Read %d employees from file:\n", readNumEmployees);
    int i;
    for (i = 0; i < readNumEmployees; i++) {
        printf("Employee %d: name=%s, age=%d, salary=%.2f\n",
            i, readEmployees[i].name, readEmployees[i].age, readEmployees[i].salary);
    }

    free(readEmployees);
    return 0;
}
