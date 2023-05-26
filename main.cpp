/*Juego de Nim  Programa realizado para la materia de Inteligencia Artificial
/* El juego consiste en ir retirando palillos de un montón.
/* Pierde el jugador que se ve obligado a retirar el último palillo.
/* Estrategia: Siempre que sea el turno del ordenador, éste jugará si puede.
    de tal modo que queden (MAX_JUGADA+1)*(k-1)+1 palillos en el montón; si
    no le es posible, retira un único palillo del montón.
/* MAX_JUGADA = Número máximo de palillos que puede retirar un jugador del
   montón en una única jugada.
/*MIN_PALILLOS = Número mínimo de palillos para iniciar una jugada
  Se Implementa Algoritmo minimax con poda alfa-beta*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Constantes
#define MAX_JUGADA   3
#define MIN_PALILLOS 5
#define MAX_PROF     10
#define INFINITO     1000000

typedef struct nodo {
    int numPalillos;
    int nivel;
    int jugador;
    struct nodo *hijos[MAX_JUGADA];
} Nodo;

int f(Nodo *u) {
    if (u->numPalillos == 1) {
        if (u->jugador == 0) {
            return INFINITO;
        } else {
            return -INFINITO;
        }
    } else {
        return 0;
    }
}

// Implementación de MINIMAX con poda alfa-beta
int MiniMaxAlphaBeta(Nodo *u, int Prof, int Mano, int alpha, int beta) {
    if (u == NULL || Prof >= MAX_PROF) {
        return f(u);
    }

    if (Mano) { // Turno de Max
        int value = -INFINITO;
        for (int i = 0; i < MAX_JUGADA; i++) {
            if (u->hijos[i] != NULL) {
                int childValue = MiniMaxAlphaBeta(u->hijos[i], Prof + 1, false, alpha, beta);
                value = childValue > value ? childValue : value;
                alpha = alpha > value ? alpha : value;
                if (beta <= alpha) {
                    break; // Poda beta
                }
            }
        }
        return value;
    } else { // Turno de Min
        int value = INFINITO;
        for (int i = 0; i < MAX_JUGADA; i++) {
            if (u->hijos[i] != NULL) {
                int childValue = MiniMaxAlphaBeta(u->hijos[i], Prof + 1, true, alpha, beta);
                value = childValue < value ? childValue : value;
                beta = beta < value ? beta : value;
                if (beta <= alpha) {
                    break; // Poda alfa
                }
            }
        }
        return value;
    }
}

void generaArbol(Nodo *u, int MaxJugada) {
    if (u->nivel >= MAX_PROF || u->numPalillos == 1) {
        return;
    }
    for (int i = 0; i < MaxJugada; i++) {
        Nodo *v = (Nodo *) malloc(sizeof(Nodo));
        v->numPalillos = u->numPalillos - (i + 1);
        v->nivel = u->nivel + 1;
        v->jugador = !(u->jugador);
        for (int j = 0; j < MaxJugada; j++) {
            v->hijos[j] = NULL;
        }
        u->hijos[i] = v;
        // Verificación para la última jugada del jugador humano
        if (v->numPalillos == 1 && v->jugador == 0) {
            return;
        }

        generaArbol(v, MaxJugada);
    }
}

int Estrategia(int NumPalillos, int MaxJugada) {
    Nodo *raiz = (Nodo *) malloc(sizeof(Nodo));
    raiz->numPalillos = NumPalillos;
    raiz->nivel = 0;
    raiz->jugador = 0;
    for (int j = 0; j < MaxJugada; j++) {
        raiz->hijos[j] = NULL;
    }

    generaArbol(raiz, MaxJugada);

    int bestValue = -INFINITO;
    int bestMove = 1; //Jugada mínima 1 palillo
    for (int i = 0; i < MaxJugada; i++) {
        if (raiz->hijos[i] != NULL) {
            int move = i + 1;
            int numPalillos = raiz->numPalillos - move;
            if (numPalillos % (MaxJugada+1) == 1) { // Si el número de palillos restantes es de la forma (MAX_JUGADA+1)*k+1, jugar para que queden (MAX_JUGADA+1)*(k-1)+1 palillos
                return move;
            }
            int value = MiniMaxAlphaBeta(raiz->hijos[i], 0, false, -INFINITO, INFINITO);
            if (value > bestValue) {
                bestValue = value;
                bestMove = move;
            }
        }
    }
    for (int i = 0; i < MaxJugada; i++) {
        if (raiz->hijos[i] != NULL) {
            free(raiz->hijos[i]);
        }
    }

    return bestMove;
}

// Programa principal
int main(void) {
    int MaxJugada;
    int NumPalillos;
    int jugada;
    int turno = 0;

    printf("\nEL JUEGO DE NIM\n\n");

    do {
        printf("Numero de palillos (minimo %d): ", MIN_PALILLOS);
        scanf("%d", &NumPalillos);
    } while (NumPalillos < MIN_PALILLOS);

    MaxJugada = 3;

    printf("\nQuedan %d palillos en el montón.\n", NumPalillos);

    while (NumPalillos > 1) {

        if (turno % 2 == 0) { // Turno del jugador humano
            do {
                printf("¿Cuantos palillos desea retirar del monton (de 1 a %d)? ", MaxJugada);
                scanf("%d", &jugada);
            } while (jugada <= 0 || jugada > MaxJugada || jugada > NumPalillos);

            NumPalillos -= jugada;

            if (NumPalillos < 1) {
                printf("\nJugada incorrecta, podria haber ganado.\n");

            } else if (NumPalillos == 1) {
                printf("\n¡Enhorabuena! Ha ganado la partida.\n");

            } else {
                printf("Quedan %d palillos.\n", NumPalillos);
            }

        } else { // Turno de la IA
            jugada = Estrategia(NumPalillos, MaxJugada);

            printf("Yo retiro %d palillo%s.\n", jugada, jugada == 1 ? "" : "s");

            NumPalillos -= jugada;

            if (NumPalillos == 1) {
                printf("\nLo siento, ha perdido esta partida.\n");
            } else {
                printf("Quedan %d palillos.\n", NumPalillos);
            }
        }

        turno++;
    }

    return 0;
}
