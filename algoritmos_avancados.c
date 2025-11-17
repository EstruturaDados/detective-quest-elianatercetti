#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 101

/* Estrutura de uma sala (nó da árvore do mapa) */
typedef struct Sala {
    char *nome;
    char *pista; /* string vazia "" se não houver pista */
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* Nó da BST que armazena as pistas coletadas */
typedef struct PistaNode {
    char *pista;
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* Entrada da tabela hash: chave = pista, valor = suspeito */
typedef struct HashEntry {
    char *pista;
    char *suspeito;
    struct HashEntry *prox;
} HashEntry;

HashEntry *tabelaHash[HASH_SIZE] = { NULL };

/* contador auxiliar para criar strings dinamicamente */
char *strdup_local(const char *s) {
    if (s == NULL) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

/* criarSala() – cria dinamicamente um cômodo com ou sem pista */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *s = (Sala*)malloc(sizeof(Sala));
    s->nome = strdup_local(nome);
    s->pista = pista ? strdup_local(pista) : strdup_local("");
    s->esq = NULL;
    s->dir = NULL;
    return s;
}

/* inserirPista() – insere a pista coletada na árvore de pistas (BST) */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return raiz;
    if (raiz == NULL) {
        PistaNode *n = (PistaNode*)malloc(sizeof(PistaNode));
        n->pista = strdup_local(pista);
        n->esq = n->dir = NULL;
        return n;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) raiz->esq = inserirPista(raiz->esq, pista);
    else if (cmp > 0) raiz->dir = inserirPista(raiz->dir, pista);
    /* se igual, já existe; mantemos apenas uma cópia (não insere duplicata) */
    return raiz;
}

/* buscarPistaBST() - retorna 1 se a pista já foi coletada (evita duplicatas) */
int buscarPistaBST(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) return 0;
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) return 1;
    if (cmp < 0) return buscarPistaBST(raiz->esq, pista);
    return buscarPistaBST(raiz->dir, pista);
}

/* inserirNaHash() – insere associação pista/suspeito na tabela hash */
unsigned int hash_func(const char *s) {
    unsigned int h = 0;
    while (*s) {
        h = (h * 31u) + (unsigned char)(*s);
        s++;
    }
    return h % HASH_SIZE;
}

void inserirNaHash(const char *pista, const char *suspeito) {
    if (pista == NULL || strlen(pista) == 0) return;
    unsigned int h = hash_func(pista);
    HashEntry *e = tabelaHash[h];
    /* se já existir a mesma pista, substitui o suspeito (ou mantém) */
    while (e) {
        if (strcmp(e->pista, pista) == 0) {
            /* substitui o suspeito por segurança */
            free(e->suspeito);
            e->suspeito = strdup_local(suspeito);
            return;
        }
        e = e->prox;
    }
    /* cria nova entrada */
    HashEntry *novo = (HashEntry*)malloc(sizeof(HashEntry));
    novo->pista = strdup_local(pista);
    novo->suspeito = strdup_local(suspeito);
    novo->prox = tabelaHash[h];
    tabelaHash[h] = novo;
}

/* encontrarSuspeito() – consulta o suspeito correspondente a uma pista */
char* encontrarSuspeito(const char *pista) {
    if (pista == NULL || strlen(pista) == 0) return NULL;
    unsigned int h = hash_func(pista);
    HashEntry *e = tabelaHash[h];
    while (e) {
        if (strcmp(e->pista, pista) == 0) return e->suspeito;
        e = e->prox;
    }
    return NULL;
}

/* exibirPistas() – imprime a árvore de pistas em ordem alfabética (inorder) */
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esq);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

/* percorrerBST_contarSuspeito() – percorre BST e conta quantas pistas apontam para 'suspeito' */
void percorrerBST_contarSuspeito(PistaNode *raiz, const char *suspeito, int *contador) {
    if (raiz == NULL) return;
    percorrerBST_contarSuspeito(raiz->esq, suspeito, contador);
    char *s = encontrarSuspeito(raiz->pista);
    if (s && strcmp(s, suspeito) == 0) (*contador)++;
    percorrerBST_contarSuspeito(raiz->dir, suspeito, contador);
}

/* explorarSalas() – navega pela árvore e ativa o sistema de pistas */
void explorarSalas(Sala *atual, PistaNode **arvorePistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);
        if (atual->pista && strlen(atual->pista) > 0) {
            /* se ainda nao coletada, adicionar na BST */
            if (!buscarPistaBST(*arvorePistas, atual->pista)) {
                printf("Voce encontrou uma pista: %s\n", atual->pista);
                *arvorePistas = inserirPista(*arvorePistas, atual->pista);
            } else {
                printf("Pista aqui ja coletada anteriormente: %s\n", atual->pista);
            }
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        /* opções de navegação */
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Nao ha caminhos a seguir a partir daqui.\n");
            printf("Escolha 's' para sair da exploracao ou 'b' para voltar (nao implementado): ");
            scanf(" %c", &escolha);
            if (escolha == 's') break;
            else break;
        }

        printf("Ir para esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);
        if (escolha == 'e') {
            if (atual->esq != NULL) atual = atual->esq;
            else printf("Caminho esquerdo inexistente.\n");
        } else if (escolha == 'd') {
            if (atual->dir != NULL) atual = atual->dir;
            else printf("Caminho direito inexistente.\n");
        } else if (escolha == 's') {
            printf("Exploracao encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

/* verificarSuspeitoFinal() – conduz à fase de julgamento final */
void verificarSuspeitoFinal(PistaNode *arvorePistas) {
    if (arvorePistas == NULL) {
        printf("\nNenhuma pista coletada. Impossivel acusar.\n");
        return;
    }

    printf("\nPistas coletadas (em ordem alfabetica):\n");
    exibirPistas(arvorePistas);

    char escolha[100];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    /* consome final de linha e lê string com espaços */
    scanf(" ");
    if (fgets(escolha, sizeof(escolha), stdin) == NULL) return;
    /* remove newline */
    size_t L = strlen(escolha);
    if (L > 0 && escolha[L-1] == '\n') escolha[L-1] = '\0';

    /* normalizar entrada e suspeitos comparados por strcmp exato (caso sensível).
       Poderíamos normalizar para lowercase se desejar matching case-insensitive. */
    int contadorPistas = 0;
    percorrerBST_contarSuspeito(arvorePistas, escolha, &contadorPistas);

    printf("\nPistas que apontam para %s: %d\n", escolha, contadorPistas);
    if (contadorPistas >= 2) {
        printf("Acusacao aceita. Ha PISTAS SUFICIENTES para sustentar a acusacao contra %s.\n", escolha);
    } else {
        printf("Acusacao rejeitada. NAO ha pistas suficientes contra %s.\n", escolha);
    }
}

/* Funcoes auxiliares para liberar memoria */
void liberarPistasBST(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistasBST(raiz->esq);
    liberarPistasBST(raiz->dir);
    free(raiz->pista);
    free(raiz);
}

void liberarSalas(Sala *s) {
    if (!s) return;
    /* libera recursivamente (preorder) */
    liberarSalas(s->esq);
    liberarSalas(s->dir);
    free(s->nome);
    free(s->pista);
    free(s);
}

void liberarHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry *e = tabelaHash[i];
        while (e) {
            HashEntry *tmp = e->prox;
            free(e->pista);
            free(e->suspeito);
            free(e);
            e = tmp;
        }
        tabelaHash[i] = NULL;
    }
}

/* Função principal: monta o mapa, inicializa a tabela hash de pistas->suspeitos e executa o jogo */
int main() {
    /* construir mapa fixo (manualmente) */
    Sala *hall = criarSala("Hall de Entrada", "Chave enferrujada");
    Sala *salaEstar = criarSala("Sala de Estar", "Pegada com lama");
    Sala *cozinha = criarSala("Cozinha", "Papel queimado");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala *jardim = criarSala("Jardim", "");
    Sala *escritorio = criarSala("Escritorio", "Envelope misterioso");
    Sala *porao = criarSala("Porao", "Mancha estranha no chao");

    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = biblioteca;
    salaEstar->dir = jardim;
    cozinha->esq = escritorio;
    cozinha->dir = porao;

    /* montar tabela hash de pistas -> suspeitos (definida manualmente) */
    inserirNaHash("Chave enferrujada", "Sr. Black");
    inserirNaHash("Pegada com lama", "Srta. Green");
    inserirNaHash("Papel queimado", "Sr. White");
    inserirNaHash("Livro rasgado", "Sr. Black");
    inserirNaHash("Envelope misterioso", "Srta. Green");
    inserirNaHash("Mancha estranha no chao", "Sr. White");
    /* pistas sem associação não são inseridas na hash (ou podem apontar para NULL) */

    PistaNode *arvorePistas = NULL;

    printf("Bem-vindo a Detective Quest - Coleta de Pistas\n");
    printf("Explore a mansao a partir do Hall de Entrada.\n");

    explorarSalas(hall, &arvorePistas);

    verificarSuspeitoFinal(arvorePistas);

    /* liberar recursos */
    liberarPistasBST(arvorePistas);
    liberarSalas(hall);
    liberarHash();

    return 0;
}
