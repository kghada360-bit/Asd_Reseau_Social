#ifndef GRAPHE_H
#define GRAPHE_H

#include "ELTUSER.h"

typedef struct structNode {
    ELEMENT user;
    int weight;
    struct structNode* next;
} structNode, *Node;

typedef struct {
    int V;
    ELEMENT* utilisateurs;
    Node* adjList;
} *graphePondere;

graphePondere creerGraphe(int V);
void afficherGraphe(graphePondere g);
int ajouterUtilisateur(graphePondere g, ELEMENT user);
int ajouterRelation(graphePondere g, int src, int dest, int poids);
int supprimerUtilisateur(graphePondere g, int idUser);
int supprimerRelation(graphePondere g, int src, int dest);
void detruireGraphe(graphePondere g);
void sauvegarderGraphe(graphePondere g, const char* filename);
graphePondere chargerGraphe(const char* filename);
void dfs(graphePondere g, int idUser, int* visite);
void bfs(graphePondere g, int idUser);
void dijkstra(graphePondere g, int src_id);
#endif
