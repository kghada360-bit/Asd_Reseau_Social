#include <stdio.h>
#include <stdlib.h>
#include "graphe.h"
#include <unistd.h> // pour sleep

// Couleurs ANSI
const char *cyan = "\033[1;36m";
const char *blue = "\033[1;34m";
const char *green = "\033[1;32m";
const char *red = "\033[1;31m";
const char *reset = "\033[0m";

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseEtNettoie() {
    printf("\nAppuyez sur Entrée pour revenir au menu...");
    while (getchar() != '\n'); // Consommer les caractères résiduels dans le buffer
    getchar(); // Attendre l'entrée de l'utilisateur
    clearScreen();
}


void print_colored(const char *text, const char *color) {
    printf("%s%s%s", color, text, reset);
}

int main() {
    int choix, id1, id2, poids, nbUtilisateurs;
    char nom[50];
    int age;

    clearScreen();
    printf("====================================\n");
    print_colored("BIENVENUE DANS LE RESEAU SOCIAL \n",cyan);
    printf("====================================\n\n");

    printf("Nombre d'Utilisateurs : ");
    scanf("%d", &nbUtilisateurs);

    graphePondere g = creerGraphe(nbUtilisateurs);

    do {
        print_colored("\n||======================== MENU DU RESEAU SOCIAL =======================||\n", cyan);
        print_colored("||                      1. Ajouter un utilisateur                       ||\n", blue);
        print_colored("||                      2. Ajouter une relation                         ||\n", blue);
        print_colored("||                      3. Afficher le graphe                           ||\n", blue);
        print_colored("||                      4. Supprimer un utilisateur                     ||\n", blue);
        print_colored("||                      5. Supprimer une relation                       ||\n", blue);
        print_colored("||                      6. Sauvegarder le graphe                        ||\n", blue);
        print_colored("||                      7. Charger un graphe                            ||\n", blue);
        print_colored("||                      8. Trouver tous les amis (DFS)                  ||\n", blue);
        print_colored("||                      9. Trouver les amis \xA0 une distance (BFS)        ||\n", blue);
        print_colored("||                     10. Trouver le chemin le plus fort (Dijkstra)    ||\n", blue);
        print_colored("||                     11. Afficher la liste des utilisateurs           ||\n", blue);
        print_colored("||                     12. Quitter                                      ||\n", blue);
        print_colored("Choix : ",cyan);
        scanf("%d", &choix);


        switch (choix) {
            case 1: {
            clearScreen();
            print_colored("\n--- AJOUT D UN UTILISATEUR ---\n", cyan);

            ELEMENT user = (ELEMENT)malloc(sizeof(UtilisateurStruct));
            if (!user) {
                print_colored("Erreur d'allocation\n", red);
                break;
            }


            printf("ID : ");
            while (scanf("%d", &user->id) != 1 || user->id < 0 || user->id >= g->V || g->utilisateurs[user->id] != NULL) {
                print_colored("ID invalide. L ID doit etre entre 0 et %d et ne doit pas exister deja.\n", red);
                printf("ID : ");
                while(getchar() != '\n');
            }

            printf("Nom : ");
            scanf("%s", user->nom);

            printf("Age : ");
            while (scanf("%d", &user->age) != 1 || user->age <= 0) {
                print_colored("Age invalide. Veuillez entrer un age positif.\n", red);
                printf("Age : ");
                while(getchar() != '\n');
            }


            if (ajouterUtilisateur(g, user)) {
                print_colored("\nUtilisateur ajoute avec succes !\n", green);
                printf("-> ID = %d\n", user->id);
                printf("-> Nom = %s\n", user->nom);
                printf("-> Age = %d\n", user->age);
            } else {
                print_colored("Utilisateur non ajoute (ID existant).\n", red);
                free(user);
            }

            pauseEtNettoie();
    break;
}

           case 2: {
                    clearScreen();
                    print_colored("\n--- AJOUT D UNE RELATION ---\n", cyan);
                    printf("ID source : ");
                    while (scanf("%d", &id1) != 1 || id1 < 0 || id1 >= g->V || g->utilisateurs[id1] == NULL) {
                        print_colored("ID source invalide. Veuillez entrer un ID existant.\n", red);
                        printf("ID source : ");
                        while(getchar() != '\n');
                    }

                    printf("ID destination : ");
                    while (scanf("%d", &id2) != 1 || id2 < 0 || id2 >= g->V || g->utilisateurs[id2] == NULL) {
                        print_colored("ID destination invalide. Veuillez entrer un ID existant.\n", red);
                        printf("ID destination : ");
                        while(getchar() != '\n');
                    }

                    printf("Poids de la relation : ");
                    while (scanf("%d", &poids) != 1 || poids <= 0) {
                        print_colored("Poids invalide. Veuillez entrer un poids positif.\n", red);
                        printf("Poids de la relation : ");
                        while(getchar() != '\n');
                    }

                    ajouterRelation(g, id1, id2, poids);
                    print_colored("Relation ajoutee !\n", green);

                    pauseEtNettoie();
                    break;
                }

            case 3:
                clearScreen();
                print_colored("\n--- AFFICHAGE DU GRAPHE ---\n", cyan);
                afficherGraphe(g);
                pauseEtNettoie();
                break;
            case 4:
                clearScreen();
                print_colored("\n--- SUPPRESSION D UN UTILISATEUR ---\n", cyan);
                printf("ID de l utilisateur a supprimer : ");
                scanf("%d", &id1);


                if (id1 >= 0 && id1 < g->V && g->utilisateurs[id1] != NULL) {
                    print_colored("\nUtilisateur a supprimer :\n",red);
                    printf("-> ID = %d\n", g->utilisateurs[id1]->id);
                    printf("-> Nom = %s\n", g->utilisateurs[id1]->nom);
                    printf("-> Age = %d\n", g->utilisateurs[id1]->age);
                } else {
                    print_colored("ID invalide ou utilisateur inexistant.\n", red);
                    pauseEtNettoie();
                    break;
                }


                char confirmation;
                print_colored("\nEtes-vous sur de vouloir supprimer cet utilisateur ? (O/N) : ",red);
                getchar();
                scanf("%c", &confirmation);

                if (confirmation == 'O' || confirmation == 'o') {

                    print_colored("\nGraphe avant la suppression :\n",green);
                    afficherGraphe(g);

                    supprimerUtilisateur(g, id1);


                    print_colored("\nGraphe apres la suppression :\n",green);
                    afficherGraphe(g);

                    print_colored("Utilisateur supprime.\n", green);
                } else {
                    print_colored("Suppression annulee.\n", red);
                }
                pauseEtNettoie();
                break;

            case 5: {
                        clearScreen();
                        print_colored("\n--- SUPPRESSION D UNE RELATION ---\n", cyan);


                        printf("ID source : ");
                        while (scanf("%d", &id1) != 1 || id1 < 0 || id1 >= g->V || g->utilisateurs[id1] == NULL) {
                            print_colored("ID source invalide. Veuillez entrer un ID existant.\n", red);
                            printf("ID source : ");
                            while(getchar() != '\n');
                        }

                        printf("ID destination : ");
                        while (scanf("%d", &id2) != 1 || id2 < 0 || id2 >= g->V || g->utilisateurs[id2] == NULL) {
                            print_colored("ID destination invalide. Veuillez entrer un ID existant.\n", red);
                            printf("ID destination : ");
                            while(getchar() != '\n');
                        }

                        char confirmation;
                        print_colored("Etes-vous sur de vouloir supprimer cette relation (O/N) ? ",red);
                        while (scanf(" %c", &confirmation) != 1 || (confirmation != 'O' && confirmation != 'N')) {
                            print_colored("Reponse invalide. Veuillez entrer 'O' pour Oui ou 'N' pour Non.\n", red);
                            print_colored("Etes-vous sur de vouloir supprimer cette relation (O/N) ? ",red);
                            while(getchar() != '\n');
                        }

                        if (confirmation == 'O') {
                            supprimerRelation(g, id1, id2);
                            print_colored("Relation supprimee.\n", green);
                        } else {
                            print_colored("Suppression annulee.\n", red);
                        }

                        pauseEtNettoie();
                        break;
                    }


            case 6:
                clearScreen();
                 print_colored("\n--- SAUVEGARDE DU GRAPHE ---\n", cyan);
                sauvegarderGraphe(g, "reseau_social.txt");
                print_colored("Graphe sauvegarde dans reseau_social.txt\n", green);
                pauseEtNettoie();
                break;
            case 7:
                clearScreen();
                print_colored("\n--- CHARGEMENT DU GRAPHE ---\n", cyan);
                g = chargerGraphe("reseau_social.txt");
                print_colored("Graphe charge avec succes !\n", green);
                pauseEtNettoie();
                break;
            case 8: {
                clearScreen();
                print_colored("\n--- RECHERCHE DFS (Amis directs et indirects) ---\n", cyan);
                printf("ID de l'utilisateur : ");
                scanf("%d", &id1);
                int* visite = (int*)calloc(g->V, sizeof(int));
                printf("Amis de %s : ", g->utilisateurs[id1]->nom);
                dfs(g, id1, visite);
                printf("\n");
                free(visite);
                pauseEtNettoie();
                break;
            }
            case 9:
                clearScreen();
                print_colored("\n--- RECHERCHE BFS (Amis par distance) ---\n", cyan);
                printf("ID de l'utilisateur : ");
                scanf("%d", &id1);
                printf("Amis a distance : \n");
                bfs(g, id1);
                pauseEtNettoie();
                break;
            case 10:
                clearScreen();
                print_colored("\n--- DIJKSTRA : Connexions les plus solides ---\n", cyan);
                printf("ID de l'utilisateur source : ");
                scanf("%d", &id1);
                dijkstra(g, id1);
                pauseEtNettoie();
                break;
        case 11: {
            clearScreen();
            print_colored("\n--- LISTE DES UTILISATEURS ---\n", cyan);

            int i;
            int utilisateursTrouves = 0;


            for (i = 0; i < g->V; i++) {
                if (g->utilisateurs[i] != NULL) {
                    utilisateursTrouves++;
                    printf("ID : %d\n", g->utilisateurs[i]->id);
                    printf("Nom : %s\n", g->utilisateurs[i]->nom);
                    printf("Age : %d\n", g->utilisateurs[i]->age);
                    printf("-----------------------------\n");
                }
            }

            if (utilisateursTrouves == 0) {
                print_colored("Aucun utilisateur dans le reseau.\n", red);
            } else {
                print_colored("\nFin de la liste des utilisateurs.\n", green);
            }

            pauseEtNettoie();
            break;
        }



            case 12:
                clearScreen();
                print_colored("Merci d'avoir utilise notre programme !\n", green);
                detruireGraphe(g);
                break;
            default:
                print_colored("Choix invalide, veuillez reessayer.\n", red);
                pauseEtNettoie();
        }
    } while (choix != 12);

    return 0;
}
