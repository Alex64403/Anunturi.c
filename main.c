#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char title[100];
    char description[500];
} Ad;

void displayMenu() {
    printf("~~~~~~~~~~ Meniu ~~~~~~~~~~\n");
    printf("1. Adaugare anunt\n");
    printf("2. Anunturi\n");
    printf("3. Inchiriaza o masa\n");
    printf("4. Favorite\n");
    printf("5. Exit\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void addAd() {
    Ad ad;
    FILE *file;

    printf("Introduceti detaliile mesei:\n");
    printf("ID: ");
    scanf("%d", &ad.id);
    printf("Nr masa: ");
    getchar();  // Consume the newline character left in the input buffer
    fgets(ad.title, sizeof(ad.title), stdin);
    ad.title[strcspn(ad.title, "\n")] = '\0';  // Remove trailing newline
    printf("Detalii: ");
    fgets(ad.description, sizeof(ad.description), stdin);
    ad.description[strcspn(ad.description, "\n")] = '\0';  // Remove trailing newline

    file = fopen("ads.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d|%s|%s\n", ad.id, ad.title, ad.description);

    fclose(file);

    printf("Anuntul a fost adaugat cu succes!\n");
}

void viewAds() {
    FILE *file;
    Ad ad;
    char line[600];
    int choice;

    file = fopen("ads.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("~~~~~~~~~~ Anunturi ~~~~~~~~~~\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%d|%[^|]|%[^\n]", &ad.id, ad.title, ad.description);
        printf("ID: %d\n", ad.id);
        printf("Nr masa: %s\n", ad.title);
        printf("Detalii: %s\n", ad.description);
        printf("-------------------------\n");
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    fclose(file);

    printf("Introduceti id-ul mesei pe care doriti a o adaugati la favorite (sau 0 pentru a anula): ");
    scanf("%d", &choice);

    if (choice != 0) {
        file = fopen("ads.txt", "r");
        if (file == NULL) {
            printf("Error opening file!\n");
            return;
        }

        int found = 0;

        while (fgets(line, sizeof(line), file) != NULL) {
            sscanf(line, "%d|%[^|]|%[^\n]", &ad.id, ad.title, ad.description);

            if (ad.id == choice) {
                addToFavorites(ad);
                found = 1;
                break;
            }
        }

        fclose(file);

        if (!found) {
            printf("ID invalid. Alege altul.\n");
        }
    }
}

void addToFavorites(Ad ad) {
    FILE *file;
    file = fopen("favorites.txt", "a");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d|%s|%s\n", ad.id, ad.title, ad.description);

    fclose(file);

    printf("Anuntul a fost adaugat la favorite!\n");
}

void viewFavorites() {
    FILE *file;
    Ad ad;
    char line[600];

    file = fopen("favorites.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("~~~~~~~~ Favorite ~~~~~~~~\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%d|%[^|]|%[^\n]", &ad.id, ad.title, ad.description);
        printf("ID: %d\n", ad.id);
        printf("Nr masa: %s\n", ad.title);
        printf("Detalii: %s\n", ad.description);
        printf("-------------------------\n");
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    fclose(file);
}

void rentTable() {
    FILE *file;
    Ad ad;
    char line[600];
    int choice;

    file = fopen("ads.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("~~~~~~~~~~ Anunturi ~~~~~~~~~~\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%d|%[^|]|%[^\n]", &ad.id, ad.title, ad.description);
        printf("ID: %d\n", ad.id);
        printf("Title: %s\n", ad.title);
        printf("Description: %s\n", ad.description);
        printf("-------------------------\n");
    }
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    fclose(file);

    printf("Introduceti id-ul mesei pe care doriti sa o inchiriati: ");
    scanf("%d", &choice);

    file = fopen("ads.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;

    FILE *tempFile;
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening file!\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        sscanf(line, "%d|%[^|]|%[^\n]", &ad.id, ad.title, ad.description);

        if (ad.id != choice) {
            fprintf(tempFile, "%d|%s|%s\n", ad.id, ad.title, ad.description);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("ID invalid. Alege altul.\n");
        remove("temp.txt");
    } else {
        remove("ads.txt");
        rename("temp.txt", "ads.txt");
        printf("Masa cu ID-ul %d a fost inchiriata\n", choice);
    }
}

int main() {
    int option;

    do {
        displayMenu();
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Ai ales optiunea 1 - Adaugati anunt\n");
                addAd();
                break;
            case 2:
                printf("Ai ales optiunea 2 - Vezi anunturile\n");
                viewAds();
                break;
            case 3:
                printf("Ai ales optiunea 3 - Inchiriaza o masa\n");
                rentTable();
                break;
            case 4:
                printf("Ai ales optiunea 4 - Vezi favorite\n");
                viewFavorites();
                break;
            case 5:
                printf("La revedere!\n");
                break;
            default:
                printf("Alegere invalida. Incercati din nou\n");
                break;
        }

        printf("\n");

    } while (option != 5);

    return 0;
}
