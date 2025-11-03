#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 5

typedef struct{
    char nome[30];
    int id;
} Peca;

typedef struct{
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila; 

typedef struct{
    Peca itens[MAX];
    int topo;
} Pilha;

void gerarPeca(Fila *f, Peca *p){ // Gera uma peça aleatória.
    static int contador_id;
    char *pecas_possiveis[] = {"I", "O", "T", "L"}; // Tipos de peças possíveis.
    int indice = rand() % 4; // Índice aleatório entre 0 e 3.
    
    if(f->total == MAX){
        return;
    }

    strcpy(p->nome, pecas_possiveis[indice]); // Atribui o nome da peça.
    p->id = contador_id;
    contador_id++;
}

int filaCheia(Fila *f){
    return f->total == MAX; // Verifica se a fila está cheia.
}

int filaVazia(Fila *f){
    return f->total == 0; // Verifica se a fila está vazia.
}

int inserirPeca(Fila *f, Peca p){ // Insere uma peça na fila.
    if(f->total == MAX){
        printf("Fila cheia!\n"); // Verifica se a fila está cheia.
        return 0;
    }

    gerarPeca(f, &p); // Gera uma nova peça aleatória.
    f->itens[f->fim] = p; // Insere a peça no final da fila.
    f->fim = (f->fim + 1) % MAX; // Atualiza o índice do fim da fila.
    f->total++; // Incrementa o total de peças na fila.
    return 1;
}

void removerPeca(Fila *f, Peca *p){ // Remove uma peça da fila.
    if(filaVazia(f)){
        printf("Fila vazia!\n"); // Verifica se a fila está vazia.
        return;
    }

    *p = f->itens[f->inicio]; // Obtém a peça do início da fila.
    f->inicio = (f->inicio + 1) % MAX; // Atualiza o índice do início da fila.
    f->total--; // Decrementa o total de peças na fila.
    
    printf("\nPeça '%s' (ID: %d) removida da fila.\n", p->nome, p->id); 
    inserirPeca(f, *p); // Insere uma nova peça na fila aleatoriamente.
}

void mostrarFila(Fila *f){ // Mostra o estado atual da fila.
    printf("\nFila: ");
    for(int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) // Percorre a fila circularmente.
    {
        printf("%s (%d) ", 
        f->itens[idx].nome, 
        f->itens[idx].id); // Mostra o nome e ID de cada peça.
    }
    printf("\n");
}

void inicializarFila(Fila *f){ // Inicializa a fila.
    // Inicializa os índices e total da fila.
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;

    // Preenche a fila com peças iniciais.
    Peca p;
    for(int i = 0; i < MAX; i++){
        inserirPeca(f, p);
    }
}

void inicializarPilha(Pilha *pl){ // Inicializa a pilha.
    pl->topo = -1;
}

void reservarPeca(Fila *f, Peca *p, Pilha *pl){ // Reserva a peça do início da fila na pilha.
    if(pl->topo == 3 - 1){
        printf("\nPilha cheia! Não é possível reservar mais peças.\n\n"); // Caso a pilha esteja cheia.
        return;
    }
    
    // Reserva a peça do início da fila na pilha.
    pl->topo++;
    pl->itens[pl->topo] = f->itens[f->inicio];
    removerPeca(f, p); // Remove a peça da fila e gera uma nova.
    printf("Peça '%s' (ID: %d) reservada na pilha.\n\n", p->nome, p->id);

}

void usarReserva(Pilha *pl, Peca *p){ // Usa a peça do topo da pilha.
    if(pl->topo == -1){
        printf("Pilha vazia! Nenhuma peça reservada.\n"); // Caso a pilha esteja vazia.
        return;
    }

    *p = pl->itens[pl->topo]; // Obtém a peça do topo da pilha.
    pl->topo--; // Remove a peça do topo da pilha.
    printf("\nPeça '%s' (ID: %d) usada da pilha de reservas.\n", p->nome, p->id);
}

void mostrarPilha(Pilha *pl){ // Mostra o estado atual da pilha.
    if(pl->topo == -1){
        printf("\nPilha vazia!\n"); // Caso a pilha esteja vazia.
    } else {
        printf("\nPilha (Topo -> Base): "); // Caso não esteja vazia, mostra o estado atual da pilha.
    }

    for(int i = pl->topo; i >= 0; i--){ // Percorre a pilha do topo para a base e printa o nome e id de cada peça.
        printf("%s (%d) ", 
        pl->itens[i].nome, 
        pl->itens[i].id);
    }
    printf("\n");

 
}

void trocarPeca(Fila *f, Pilha *pl, Peca *p){ // Troca a peça do início da fila com a peça do topo da pilha.
    if(pl->topo == -1){
        printf("\nPilha vazia! Nenhuma peça reservada para trocar.\n");
        return;
    }

    // Cria uma peça temporária para realizar a troca e realiza a troca.
    Peca temp = pl->itens[pl->topo];
    pl->itens[pl->topo] = f->itens[f->inicio];
    f->itens[f->inicio] = temp;

    printf("\nPeça '%s' (ID: %d) trocada com a peça reservada '%s' (ID: %d).\n", 
    f->itens[f->inicio].nome, f->itens[f->inicio].id,
    pl->itens[pl->topo].nome, pl->itens[pl->topo].id);
}

void trocarTresPecas(Fila *f, Pilha *pl){ // Troca as 3 primeiras peças da fila com as 3 peças do topo da pilha.
    if(pl->topo < 2){
        printf("\nPilha não tem peças suficientes para trocar.\n"); // Verifica se a pilha tem pelo menos 3 peças.
        return;
    }

    // Laço de repetição para trocar as 3 primeiras peças da fila com as 3 peças do topo da pilha.
    for(int i = 0; i < 3; i++){
        Peca temp = pl->itens[pl->topo - i];
        pl->itens[pl->topo - i] = f->itens[(f->inicio + i) % MAX];
        f->itens[(f->inicio + i) % MAX] = temp;
    }

    printf("\nAs 3 primeiras peças da fila foram trocadas com as 3 peças do topo da pilha.\n");
}

void menuAcoes(){ // Menu de ações do Tetris.
    Fila f;
    Peca p;
    Pilha pl;

    inicializarPilha(&pl);
    inicializarFila(&f);
    int opcao;

    do{
    printf("Menu de Ações do Tetris:\n");
    printf("1. Jogar peça.\n");
    printf("2. Reservar peça.\n");
    printf("3. Usar peça reservada.\n");
    printf("4. Trocar peça da frente da fila com o topo da pilha.\n");
    printf("5. Trocar os 3 primeiros da fila com as 3 peças da pilha.\n");
    printf("0. Sair\n\n");
    printf("Escolha uma opção: ");
    
    scanf("%d", &opcao);
    getchar(); // limpar buffer

    switch(opcao) {
        case 1:
            removerPeca(&f, &p);
            mostrarFila(&f);
            mostrarPilha(&pl);
            getchar();
            break;
        case 2:
            reservarPeca(&f, &p, &pl);
            mostrarFila(&f);
            mostrarPilha(&pl);
            getchar();
            break;
        case 3:
            usarReserva(&pl, &p);
            mostrarFila(&f);
            mostrarPilha(&pl);
            getchar();
            break;
        case 4:
            trocarPeca(&f, &pl, &p);
            mostrarFila(&f);
            mostrarPilha(&pl);
            getchar();
            break;
        case 5:
            trocarTresPecas(&f, &pl);
            mostrarFila(&f);
            mostrarPilha(&pl);
            getchar();
            break;
        case 0:
            break;

        default:
            printf("Opção inválida. Tente novamente.\n");
    }
    } while(opcao != 0);
}

int main(){
    srand(time(NULL)); // Inicializa o gerador de números aleatórios.
    menuAcoes(); // Chama o menu de ações do Tetris.
    return 0; 
}
