// miniSMDB.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <stdio.h>
#include <iostream>
#include <string.h>
#pragma warning(disable : 4996) //отключение ошибки об небезопасности fopen в vs studio

/**/


struct iPhone {
    int id;
    char model[30];
    char color[30];
    float price;
    int releaseYear;
    int rom;
};

int tableRead(FILE*, struct iPhone *);
void tablePrint(struct iPhone*, int);
int addRecord(struct iPhone*, int);
int deleteRecord(struct iPhone*, int);
void searchByColumn(struct iPhone*, int);
void sortByColumn(struct iPhone*, int);
void saveDatabase(FILE*, char[], struct iPhone*, int);

int main()
{
    FILE* db = NULL;
    struct iPhone items[10];
    int count = 0, i = 0, choice = -1, yn = 0;
    char source[50];

    printf("Enter database name: ");
    scanf("%s", source);
    strcat(source, ".txt");

    while ((db = fopen(source, "r+")) == NULL) {
        printf("The file could not be found. Create a new file?\n1 - Create a new file\n2 - Try again\n");
        scanf("%d", &yn);
        switch (yn)
        {
        case 1:
            db = fopen(source, "w");
            break;
        case 2:
            printf("Enter database name: ");
            scanf("%s", source);
            strcat(source, ".txt");
            break;
        }
    }

    db = fopen(source, "r+");

    printf("Connecting to the database...\n");

    if (db == NULL) {
        printf("Can't open database file.\n");
    }
    else {
        printf("%s connected.\n", source);
        count = tableRead(db, items);
        while (choice != 6) {
            printf("\nSelect an action by entering a number:\n0 - print table\n1 - add record to database\n2 - delete record from database\n3 - search by column\n4 - sort by column\n5 - save DATABASE\n6 - exit\n\n");
            printf("Input: ");
            scanf_s("%d", &choice);
            switch (choice)
            {
            case 0:
                tablePrint(items, count);
                break;
            case 1:
                count = addRecord(items, count);
                break;
            case 2:
                count = deleteRecord(items, count);
                break;
            case 3:
                searchByColumn(items, count);
                break;
            case 4:
                sortByColumn(items, count);
                break;
            case 5:
                saveDatabase(db, source, items, count);
                break;
            case 6:
                printf("Database has been disabled.\n");
                fclose(db);
                return 0;
            }
        }
    }
    fclose(db);
    return 0;
}

int tableRead(FILE* db, struct iPhone * items) {
    int i = 0;
    while (fscanf(db, "%d %s %s %f %d %d", &(items[i].id), items[i].model, items[i].color, &(items[i].price), &(items[i].releaseYear), &(items[i].rom)) != EOF) {
        i++;
    }
    return i;
}

void tablePrint(struct iPhone* items, int count) {
    int i;
    printf("\t====================================================================\n\t||id  |model               |color     |price(USD)|year  |ROM(GB)  ||\n\t||================================================================||\n");
    for (i = 0; i < count; i++) {
        printf("\t||%-4d|%-20s|%-10s|%-10.2f|%-6d|%-9d||\n\t||----|--------------------|----------|----------|------|---------||\n", items[i].id, items[i].model, items[i].color, items[i].price, items[i].releaseYear, items[i].rom);
    }
    printf("\t====================================================================\n\n");
}

int addRecord(struct iPhone* items, int count) {
    int i = count;
    printf("Fill in the appropriate fields of the table:\n");
    items[i].id = i+1;
    printf("model: ");
    scanf("%s", items[i].model);
    printf("\ncolor: ");
    scanf("%s", items[i].color);
    printf("\nprice: ");
    scanf("%f", &(items[i].price));
    printf("\nyear: ");
    scanf("%d", &(items[i].releaseYear));
    while (items[i].releaseYear < 2007) {
        printf("Release year cannot be less than 2007. Please try again.\n");
        scanf("%d", &(items[i].releaseYear));
    }
    printf("\nROM(GB): ");
    scanf_s("%d", &(items[i].rom));
    while (items[i].rom > 1024) {
        printf("ROM cannot exceed 1024 GB. Please try again.\n");
        scanf("%d", &(items[i].rom));
    }
    printf("\n");
    i++;
    return i;
}

int deleteRecord(struct iPhone* items, int count) {
    int id, i, j;
    printf("Enter the id of the line to be removed: ");
    scanf_s("%d", &id);

    for (i = 0; i < count; i++) {
        if (items[i].id < id) {
            continue;
        }
        if (items[i].id == id) {
            for (j = i; j < count-1; j++) {
                items[j].id = items[j + 1].id;
                strcpy(items[j].model, items[j + 1].model);
                strcpy(items[j].color, items[j + 1].color);
                items[j].price = items[j + 1].price;
                items[j].releaseYear = items[j + 1].releaseYear;
                items[j].rom = items[j + 1].rom;
            }
            count = count - 1;
            printf("Post id %d has been deleted\n", id);
            break;
        }
    }
    return count;
}

void searchByColumn(struct iPhone* items, int count){
    int column, value = 0, i = 0, j = 0, flag = 0, search;
    float search_f;
    char search_c[30];
    printf("\nSelect the column whose value will be searched:\n1 - id\n2 - model\n3 - color\n4 - price\n5 - year\n6 - ROM(GB)\n");
    printf("Input: ");
    scanf("%d", &column);
    if(column < 1 || column > 6) {
        while( flag == 0) {
            printf("\nThe entered value is incorrect. Try again: ");
            scanf("%d", &column);
            if (column >= 1 && column <= 6)
                flag = 1;
        }
    }

    flag = 0;

    if (column == 1 || column == 4 || column == 5 || column == 6) {
        if (column == 4){
            printf("Enter an float search value (x.xx): ");
            scanf("%f", &search_f);
            for (i = 0; i < count; i++) {
                if (items[i].price == search_f) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                printf("\nNo matches found.\n");
                return;
            }
            else {
                printf("\n\tSearching results:\n");
                printf("\t====================================================================\n\t||id  |model               |color     |price(USD)|year  |ROM(GB)  ||\n\t||================================================================||\n");
                for (i = 0; i < count; i++) {
                    if (items[i].price == search_f) {
                        printf("\t||%-4d|%-20s|%-10s|%-10.2f|%-6d|%-9d||\n\t||----|--------------------|----------|----------|------|---------||\n", items[i].id, items[i].model, items[i].color, items[i].price, items[i].releaseYear, items[i].rom);
                    }
                }
                printf("\t====================================================================\n\n");
                return;
            }
        }
        else {
            printf("Enter an integer search value: ");
            scanf("%d", &search);
            for (i = 0; i < count; i++) {
                if (items[i].id == search || items[i].releaseYear == search || items[i].rom == search) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                printf("\nNo matches found.\n");
                return;
            }
            else {
                printf("\n\tSearching results:\n");
                printf("\t====================================================================\n\t||id  |model               |color     |price(USD)|year  |ROM(GB)  ||\n\t||================================================================||\n");
                for (i = 0; i < count; i++) {
                    if (items[i].id == search || items[i].releaseYear == search || items[i].rom == search) {
                        printf("\t||%-4d|%-20s|%-10s|%-10.2f|%-6d|%-9d||\n\t||----|--------------------|----------|----------|------|---------||\n", items[i].id, items[i].model, items[i].color, items[i].price, items[i].releaseYear, items[i].rom);
                    }
                }
                printf("\t====================================================================\n\n");
                return;
            }
            
        }
    }
    if (column == 2 || column == 3) {
        printf("Enter an string search value: ");
        scanf("%s", search_c);
        for (i = 0; i < count; i++) {
            printf("\n\t%d - %d\n", items[i].model, search_c);
            if (strcmp(items[i].model, search_c) == 0 || strcmp(items[i].color, search_c) == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            printf("\nNo matches found.\n");
            return;
        }
        else {
            printf("\n\tSearching results:\n");
            printf("\t====================================================================\n\t||id  |model               |color     |price(USD)|year  |ROM(GB)  ||\n\t||================================================================||\n");
            for (i = 0; i < count; i++) {
                if (strcmp(items[i].model, search_c) == 0 || strcmp(items[i].color, search_c) == 0) {
                    printf("\t||%-4d|%-20s|%-10s|%-10.2f|%-6d|%-9d||\n\t||----|--------------------|----------|----------|------|---------||\n", items[i].id, items[i].model, items[i].color, items[i].price, items[i].releaseYear, items[i].rom);
                }
            }
            printf("\t====================================================================\n\n");
            return;
        }
    }

}

void sortByColumn(struct iPhone* items, int count){
    int i, j, column, flag = 0, res;
    struct iPhone item = items[0];
    struct iPhone bubble;
    printf("\nSelect the column to sort by:\n1 - id\n2 - model\n3 - color\n4 - price\n5 - year\n6 - ROM(GB)\n");
    printf("Input: ");
    scanf("%d", &column);
    if (column < 1 || column > 6) {
        while (flag == 0) {
            printf("\nThe entered value is incorrect. Try again: ");
            scanf("%d", &column);
            if (column >= 1 && column <= 6)
                flag = 1;
        }
    }

    flag = 0;

    if (column == 1 || column == 4 || column == 5 || column == 6) {
        switch (column - 1) {
            case 0:
                for (i = 0; i < count - 1; i++) {
                    for (j = 0; j < count - 1; j++) {
                        if (items[j].id > items[j + 1].id) {
                            bubble = items[j];
                            items[j] = items[j + 1];
                            items[j + 1] = bubble;
                        }
                    }
                }
                break;
            case 3:
                for (i = 0; i < count - 1; i++) {
                    for (j = 0; j < count - 1; j++) {
                        if (items[j].price > items[j + 1].price) {
                            bubble = items[j];
                            items[j] = items[j + 1];
                            items[j + 1] = bubble;
                        }
                    }
                }
                break;
            case 4:
                for (i = 0; i < count - 1; i++) {
                    for (j = 0; j < count - 1; j++) {
                        if (items[j].releaseYear > items[j + 1].releaseYear) {
                            bubble = items[j];
                            items[j] = items[j + 1];
                            items[j + 1] = bubble;
                        }
                    }
                }
                break;
            case 5:
                for (i = 0; i < count - 1; i++) {
                    for (j = 0; j < count - 1; j++) {
                        if (items[j].rom > items[j + 1].rom) {
                            bubble = items[j];
                            items[j] = items[j + 1];
                            items[j + 1] = bubble;
                        }
                    }
                }
                break;
        }
    }
    else{
        switch (column - 1)
        {
            case 1:
                for (i = 0; i < count - 1; i++) {
                    for (j = 0; j < count - 1; j++) {
                        res = strcmp(items[j].model, items[j + 1].model);
                        if (res > 0) {
                            bubble = items[j + 1];
                            items[j + 1] = items[j];
                            items[j] = bubble;
                        }
                    }
                }
                break;
            case 2:
                for (i = 0; i < count - 1; i++) {
                    for (j = 0; j < count - 1; j++) {
                        res = strcmp(items[j].color, items[j + 1].color);
                        if (res > 0) {
                            bubble = items[j + 1];
                            items[j + 1] = items[j];
                            items[j] = bubble;
                        }
                    }
                }
                break;
        }
    }

    printf("\n\tSort results:\n");
    tablePrint(items, count);

    return;
}

void saveDatabase(FILE* db, char source[], struct iPhone* items, int count){
    int choice, i;
    FILE* anotherDb;
    printf("Choose the option you need:\n1 - Save data in current file\n2 - Save data in a new file\n\nInput: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            
            if (remove(source) != -1) {
                printf("File save error 1.\n");
                return;
            }
            else {
                fclose(db);
                if ((db = fopen(source, "w")) == NULL) {
                    printf("File save error 2.\n");
                    return;
                }
                else {
                    for (i = 0; i < count; i++) {
                        fprintf(db, "%-4d %-20s %-10s %-10.2f %-6d %-9d\n", items[i].id, items[i].model, items[i].color, items[i].price, items[i].releaseYear, items[i].rom);
                    }
                    printf("File saved successfully!\n");
                    fclose(db);
                }
            }
            break;
        case 2:
            printf("Enter the file name: ");
            scanf("%s", source);
            strcat(source, ".txt");
            if ((anotherDb = fopen(source, "w")) == NULL) {
                printf("File save error 2.\n");
                return;
            }
            else {
                for (i = 0; i < count; i++) {
                    fprintf(anotherDb, "%-4d %-20s %-10s %-10.2f %-6d %-9d\n", items[i].id, items[i].model, items[i].color, items[i].price, items[i].releaseYear, items[i].rom);
                    printf("File successfully created and saved!\n");
                }
                fclose(anotherDb);
            }
            break;
    }
    return;
}