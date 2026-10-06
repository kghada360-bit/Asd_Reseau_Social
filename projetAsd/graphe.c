#include "graphe.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
graphePondere creerGraphe(int V) {
    graphePondere g = (graphePondere)malloc(sizeof(*g));
    g->V = V;
    g->utilisateurs = (ELEMENT*)malloc(V * sizeof(ELEMENT));
    g->adjList = (Node*)malloc(V * sizeof(Node));

    for (int i = 0; i < V; i++) {
        g->utilisateurs[i] = NULL;
        g->adjList[i] = NULL;
    }

    return g;
}

int ajouterUtilisateur(graphePondere g, ELEMENT user) {
    if (user->id >= g->V) {
        printf(" ID invalide (trop grand).\n");
        return 0;
    }
    if (g->utilisateurs[user->id] != NULL) {
        printf(" Un utilisateur avec cet ID existe déjà.\n");
        return 0;
    }
    g->utilisateurs[user->id] = user;
    return 1;
}

int ajouterRelation(graphePondere g, int src, int dest, int poids) {
    Node newNode = (Node)malloc(sizeof(structNode));
    newNode->user = g->utilisateurs[dest];
    newNode->weight = poids;
    newNode->next = g->adjList[src];
    g->adjList[src] = newNode;
    return 1;
}


void afficherGraphe(graphePondere g) {
    for (int i = 0; i < g->V; i++) {
        if (g->utilisateurs[i] != NULL) {
            printf("%s (%d) -> ", g->utilisateurs[i]->nom, g->utilisateurs[i]->id);
            Node temp = g->adjList[i];
            while (temp) {
                printf("%s (%d, poids=%d) -> ", temp->user->nom, temp->user->id, temp->weight);
                temp = temp->next;
            }
            printf("NULL\n");
        }
    }
}

int supprimerUtilisateur(graphePondere g, int idUser) {
    if (g->utilisateurs[idUser] == NULL) return 0;
    free(g->utilisateurs[idUser]);
    g->utilisateurs[idUser] = NULL;
    Node temp = g->adjList[idUser];
    while (temp) {
        Node toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }
    g->adjList[idUser] = NULL;

    return 1;
}

int supprimerRelation(graphePondere g, int src, int dest) {
    Node* temp = &g->adjList[src];
    while (*temp) {
        if ((*temp)->user->id == dest) {
            Node toDelete = *temp;
            *temp = (*temp)->next;
            free(toDelete);
            return 1;
        }
        temp = &((*temp)->next);
    }
    return 0;
}

void detruireGraphe(graphePondere g) {
    for (int i = 0; i < g->V; i++) {
        supprimerUtilisateur(g, i);
    }
    free(g->utilisateurs);
    free(g->adjList);
    free(g);
}

void sauvegarderGraphe(graphePondere g, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erreur d'ouverture du fichier !\n");
        return;
    }
    for (int i = 0; i < g->V; i++) {
        if (g->utilisateurs[i] != NULL) {
            fprintf(file, "U %d %s %d\n", g->utilisateurs[i]->id, g->utilisateurs[i]->nom, g->utilisateurs[i]->age);
        }
    }
    for (int i = 0; i < g->V; i++) {
        Node temp = g->adjList[i];
        while (temp) {
            fprintf(file, "R %d %d %d\n", i, temp->user->id, temp->weight);
            temp = temp->next;
        }
    }

    fclose(file);
}

graphePondere chargerGraphe(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur d'ouverture du fichier !\n");
        return NULL;
    }

    int V = 0;
    char type;
    while (fscanf(file, " %c", &type) != EOF) {
        if (type == 'U') V++;
}

 rewind(file);
    graphePondere g = creerGraphe(V);

     int id, age, src, dest, poids;
    char nom[50];

    while (fscanf(file, " %c", &type) != EOF) {
        if (type == 'U') {
            fscanf(file, "%d %s %d", &id, nom, &age);
            ELEMENT user = creerUtilisateur(id, nom, age);
             ajouterUtilisateur(g, user);
        } else if (type == 'R') {
            fscanf(file, "%d %d %d", &src, &dest, &poids);
            ajouterRelation(g, src, dest, poids);
        }
    }

    fclose(file);
    return g;
}
void dfs(graphePondere g, int idUser, int* visite) {
    visite[idUser] = 1;
    printf("%s ", g->utilisateurs[idUser]->nom);

    Node temp = g->adjList[idUser];
    while (temp) {
        if (!visite[temp->user->id]) {
            dfs(g, temp->user->id, visite);
        }
        temp = temp->next;
    }
}

void bfs(graphePondere g, int idUser) {
    bool visite[g->V];
    for (int i = 0; i < g->V; i++) visite[i] = false;

    int queue[g->V], front = 0, rear = 0;
    queue[rear++] = idUser;
    visite[idUser] = true;

    while (front < rear) {
        int u = queue[front++];
        printf("%s ", g->utilisateurs[u]->nom);

        Node temp = g->adjList[u];
        while (temp) {
            if (!visite[temp->user->id]) {
                queue[rear++] = temp->user->id;
                visite[temp->user->id] = true;
            }
            temp = temp->next;
        }
    }
}


void dijkstra(graphePondere g, int src_id) {
    int dist[g->V];
    bool visite[g->V];

    for (int i = 0; i < g->V; i++) {
        dist[i] = INT_MAX;
        visite[i] = false;
    }
    dist[src_id] = 0;

    for (int count = 0; count < g->V - 1; count++) {
        int min = INT_MAX, u;
        for (int v = 0; v < g->V; v++)
            if (!visite[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }

        visite[u] = true;

        Node temp = g->adjList[u];
        while (temp) {
            if (!visite[temp->user->id] && dist[u] + temp->weight < dist[temp->user->id]) {
                dist[temp->user->id] = dist[u] + temp->weight;
            }
            temp = temp->next;
        }
    }

    for (int i = 0; i < g->V; i++) {
        printf("Distance de %s à %s: %d\n", g->utilisateurs[src_id]->nom, g->utilisateurs[i]->nom, dist[i]);
    }
}
