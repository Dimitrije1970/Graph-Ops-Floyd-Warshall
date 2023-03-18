#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

void change_branch(int** graph, int n, int a, int b, int weight) {
    if (a < 1 || a > n || b < 1 || b > n) {
        printf("Lose uneti indeksi cvorova.");
        exit(0);
    }

    graph[a - 1][b - 1] = weight;
}

void print_graph(int** graph, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (graph[i][j] < 0) {
                graph[i][j] = 0;
            }
            printf("%7d ", graph[i][j]);
        }
        printf("\n");
    }
}

void warshall(int** matrix, int** graph, int n) {
    int k, i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n;j++) {
            if (graph[i][j] > 0) {
                matrix[i][j] = 1;
            }
            else {
                matrix[i][j] = 0;
            }
        }
    }

    for (k = 0; k < n; k++) {
        for (i = 0; i < n;i++) {
            if (matrix[i][k] == 1) {
                for (j = 0; j < n;j++) {
                    matrix[i][j] = (matrix[i][j] | matrix[k][j]);
                }
            }
        }
    }
}

int main() {
    int task;
    printf("1) Prvi zadatak.\n");
    printf("2) Drugi zadatak.\n");
    printf("0) Prekini program.\n");
    printf("Unesi redni broj opcije:");
    scanf_s("%d", &task);

    while (1) {
        if (task == 1) {
            int** graph = NULL;
            int choice;
            int n = 0, i, j;

            printf("\n");
            printf("1. Kreiraj prazan graf.\n");
            printf("2. Dodaj cvor u graf.\n");
            printf("3. Izbrisi cvor iz grafa.\n");
            printf("4. Dodaj granu u graf.\n");
            printf("5. Izbrisi granu iz grafa.\n");
            printf("6. Ispisi reprezentaciju grafa.\n");
            printf("7. Izbrisi graf iz memorije.\n");
            printf("0. Glavni meni.\n");
            printf("Unesi redni broj opcije:");
            scanf_s("%d", &choice);

            while (1) {
                if (choice == 1) {
                    printf("Unesi pocetni broj cvorova u grafu:");
                    scanf_s("%d", &n);

                    if (n < 1) {
                        printf("Lose unet broj cvorova.");
                        return 0;
                    }

                    graph = calloc(n, sizeof(int*));
                    for (i = 0; i < n; i++) {
                        graph[i] = calloc(n, sizeof(int));
                    }
                }
                else if (choice == 2) {
                    graph = realloc(graph, (n + 1) * sizeof(int*));
                    graph[n] = calloc(n, sizeof(int));
                    for (i = 0; i < n; i++) {
                        graph[i] = realloc(graph[i], (n + 1) * sizeof(int));
                        graph[i][n] = 0;
                    }
                    n++;
                    printf("Uspesno dodat cvor u graf.\n");
                }
                else if (choice == 3) {
                    int index;
                    printf("Unesi indeks cvora koji se brise iz grafa(indeksiranje pocinje od 1):");
                    scanf_s("%d", &index);

                    index -= 1;

                    if (index >= 0 && index < n) {
                        for (j = 0; j < n; j++) {
                            int* temp;
                            for (i = index + 1; i < n; i++)graph[j][i - 1] = graph[j][i];
                            temp = realloc(graph[j], (n - 1) * sizeof(int));
                            graph[j] = temp;
                        }

                        for (j = index + 1; j < n; j++)graph[j - 1] = graph[j];
                        n--;
                    }

                }
                else if (choice == 4) {
                    printf("Unesi indekse dva cvora izmedju kojih se dodaje grana(indeksiranje pocinje od 1):");
                    int a, b;
                    scanf_s("%d %d", &a, &b);

                    change_branch(graph, n, a, b, 1);
                }
                else if (choice == 5) {
                    printf("Unesi indekse dva cvora izmedju kojih se brise grana(indeksiranje pocinje od 1):");
                    int a, b;
                    scanf_s("%d %d", &a, &b);

                    change_branch(graph, n, a, b, 0);
                }
                else if (choice == 6) {
                    print_graph(graph, n);
                }
                else if (choice == 7) {
                    for (i = 0; i < n; i++) {
                        free(graph[i]);
                    }
                    free(graph);

                    printf("Graf uspesno obrisan.\n");
                    break;
                }
                else if (choice == 0) {
                    break;
                }
                else {
                    printf("Lose unet redni broj opcije.\n");
                }

                printf("Unesi redni broj opcije:");
                scanf_s("%d", &choice);
            }
        }
        else if (task == 2) {
            int** graph = NULL;
            int** warshall_matrix;
            int choice;
            int n = 0, i, j;

            int cycle_index = 0;
            int el_index = 0;
            int cycles[100][20];
            int cycle_len[100];

            int debt[100];
            int debt_index = 0;

            printf("\n");
            printf("1. Unesi graf.\n");
            printf("2. Pronadji i ispisi cikluse u grafu.\n");
            printf("3. Ispisi najvece iznose kompenzacije za svaki ciklus.\n");
            printf("4. Sprovedi kompenzaciju.\n");
            printf("5. Izbrisi graf.\n");
            printf("0. Glavni meni.\n");
            printf("Unesi redni broj opcije:");
            scanf_s("%d", &choice);

            while (1) {
                if (choice == 1) {
                    char file_name[50];
                    printf("Unesi ime datoteke iz koje se cita graf:");
                    scanf("%s", file_name);

                    FILE* input_file = fopen(file_name, "r");
                    if (input_file == NULL) {
                        perror("Greska pri otvaranju fajla");
                        exit(0);
                    }

                    fscanf(input_file, "%d", &n);

                    graph = malloc((n) * sizeof(int*));
                    for (i = 0; i < n; i++) {
                        graph[i] = malloc((n) * sizeof(int));
                    }

                    int temp;
                    for (i = 0; i < n; i++) {
                        for (j = 0; j < n; j++) {
                            fscanf(input_file, "%d", &temp);
                            graph[i][j] = temp;
                        }
                    }
                    fclose(input_file);

                    print_graph(graph, n);
                    printf("\n");
                }
                else if (choice == 2) {
                    warshall_matrix = calloc(n, sizeof(int*));
                    for (i = 0; i < n;i++) {
                        warshall_matrix[i] = calloc(n, sizeof(int));
                    }

                    warshall(warshall_matrix, graph, n);

                    for (i = 0; i < n;i++) {
                        if (warshall_matrix[i][i] == 1) {
                            cycles[cycle_index][el_index++] = (i + 1);
                            cycle_len[cycle_index] = el_index;

                            int current = i;
                            while (j != i) {
                                for (j = 0; j < n;j++) {
                                    if (graph[current][j] > 0 && warshall_matrix[j][j] == 1) {
                                        cycles[cycle_index][el_index++] = (j + 1);
                                        cycle_len[cycle_index] = el_index;

                                        current = j;
                                        break;
                                    }
                                }
                            }

                            cycle_index++;
                            el_index = 0;
                        }
                    }

                    for (i = 0; i < cycle_index;i += (cycle_len[i]-1)) {
                        for (j = 0; j < cycle_len[i];j++) {
                            if (j == cycle_len[i]-1) {
                                printf("%d", cycles[i][j]);
                            }
                            else {
                                printf("%d->", cycles[i][j]);
                            }

                        }
                        printf("\n");
                    }


                }
                else if (choice == 3) {
                    for (i = 0; i < cycle_index;i += (cycle_len[i]-1)) {
                        debt[debt_index] = graph[cycles[i][0]-1][cycles[i][1]-1];
                        for (j = 0; j < (cycle_len[i]-1); j++) {
                            if (graph[cycles[i][j]-1][cycles[i][j+1]-1] < debt[debt_index]) {
                                debt[debt_index] = graph[cycles[i][j]-1][cycles[i][j + 1]-1];
                            }

                        }
                        debt_index++;
                    }

                    for (i = 0; i < debt_index;i++) {
                        printf("%d\n", debt[i]);
                    }
                }
                else if (choice == 4) {
                    int x = 0;
                    for (i = 0; i < cycle_index; i += (cycle_len[i] - 1)) {
                        for (j = 0; j < (cycle_len[i] - 1); j++) {
                            graph[cycles[i][j] - 1][cycles[i][j + 1] - 1] -= debt[x];
                        }
                        x++;
                    }

                    print_graph(graph, n);
                }
                else if (choice == 5) {
                    if (graph == NULL) {
                        printf("Graf uspesno obrisan.\n");
                        break;
                    }

                    for (i = 0; i < n; i++) {
                        free(graph[i]);
                    }
                    free(graph);

                    printf("Graf uspesno obrisan.\n");
                    break;
                }
                else if (choice == 0) {
                    break;
                }
                else {
                    printf("Lose unet redni broj opcije.\n");
                }

                printf("Unesi redni broj opcije:");
                scanf_s("%d", &choice);
            }

        }
        else if (task == 0) {
            break;
        }

        printf("\n");
        printf("1) Prvi zadatak.\n");
        printf("2) Drugi zadatak.\n");
        printf("0) Prekini program.\n");
        printf("Unesi redni broj opcije:");
        scanf_s("%d", &task);

    }

    return 0;
}
