#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MISSAO      200
#define MAX_JOGADORES   2

// -------------------------------
// Definição da struct Territorio
// -------------------------------
typedef struct {
    char nome[30];
    char cor[10];   // Ex: "vermelho", "azul", "verde" etc.
    int tropas;
} Territorio;

// --------------------------
// Vetor de missões do jogo
// --------------------------
#define TOTAL_MISSOES 5

char* MISSOES_PRE_DEFINIDAS[TOTAL_MISSOES] = {
    "Controlar pelo menos 3 territorios com mais de 5 tropas",
    "Nao pode existir nenhum territorio da cor vermelho",
    "Existir pelo menos 1 territorio com 10 ou mais tropas",
    "Existirem pelo menos 2 territorios da cor azul",
    "Total de tropas no mapa maior que 30"
};

// ---------------------------------
// Protótipos das funções
// ---------------------------------
void lerString(char* destino, int tamanho);
void cadastrarTerritorios(Territorio* mapa, int qtd);
void exibirMapa(Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, int qtdTerritorios, char* missoesJogadores[], int qtdJogadores);

// ---------------------------------------------------------
// Função utilitária para ler strings removendo o '\n'
// ---------------------------------------------------------
void lerString(char* destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len - 1] == '\n') {
            destino[len - 1] = '\0';
        }
    }
}

// ---------------------------------------------------------
// Cadastro dos territórios (usado para todos os níveis)
// ---------------------------------------------------------
void cadastrarTerritorios(Territorio* mapa, int qtd) {
    int i;

    printf("\n=== CADASTRO DE TERRITORIOS ===\n");
    printf("OBS: Para cores, use por exemplo: vermelho, azul, verde...\n\n");

    for (i = 0; i < qtd; i++) {
        printf("Territorio %d\n", i);
        printf("Nome: ");
        lerString(mapa[i].nome, sizeof(mapa[i].nome));

        printf("Cor do exercito: ");
        lerString(mapa[i].cor, sizeof(mapa[i].cor));

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // consome '\n' do scanf

        printf("\n");
    }
}

// ---------------------------------------------------------
// Exibição do mapa de territórios
// ---------------------------------------------------------
void exibirMapa(Territorio* mapa, int qtd) {
    int i;
    printf("\n=== MAPA ATUAL DOS TERRITORIOS ===\n");
    for (i = 0; i < qtd; i++) {
        printf("ID %d | Nome: %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("==================================\n\n");
}

// ---------------------------------------------------------
// Função de ataque entre territórios
// ---------------------------------------------------------
// Regras implementadas:
// - Cada lado rola um dado (1 a 6) com rand()
// - Se atacante > defensor: defensor muda de cor e recebe metade
//   das tropas do atacante (as tropas transferidas saem do atacante)
// - Se atacante <= defensor: atacante perde 1 tropa (se tiver)
// ---------------------------------------------------------
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas < 2) {
        printf("\n[ERRO] O atacante precisa ter pelo menos 2 tropas para atacar.\n");
        return;
    }

    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Nao e possivel atacar um territorio da mesma cor!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n=== ATAQUE ===\n");
    printf("Atacante (%s - %s) rolou: %d\n",
           atacante->nome, atacante->cor, dadoAtacante);
    printf("Defensor (%s - %s) rolou: %d\n",
           defensor->nome, defensor->cor, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\nResultado: O atacante VENCEU a batalha!\n");

        // Metade das tropas do atacante vão para o defensor
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas < 1) tropasTransferidas = 1;

        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas;

        // Defensor muda de cor
        strcpy(defensor->cor, atacante->cor);

        printf("O territorio defensor agora pertence ao atacante (%s).\n", atacante->cor);
        printf("Tropas transferidas: %d\n", tropasTransferidas);
    } else {
        printf("\nResultado: O defensor RESISTIU ao ataque.\n");
        if (atacante->tropas > 0) {
            atacante->tropas -= 1;
            printf("O atacante perdeu 1 tropa. Tropas restantes do atacante: %d\n",
                   atacante->tropas);
        }
    }

    printf("=============================\n\n");
}

// ---------------------------------------------------------
// Atribuição de missão ao jogador
// ---------------------------------------------------------
// Sorteia uma missão e copia para 'destino' usando strcpy.
// ---------------------------------------------------------
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// ---------------------------------------------------------
// Verificação da missão
// ---------------------------------------------------------
// Simplificação: comparamos a string da missão com as missões
// pré-definidas e aplicamos uma lógica para cada uma.
// Retorna 1 se a missão foi cumprida, 0 caso contrário.
// ---------------------------------------------------------
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    int i, cont, soma;

    // Missao 0: "Controlar pelo menos 3 territorios com mais de 5 tropas"
    if (strcmp(missao, MISSOES_PRE_DEFINIDAS[0]) == 0) {
        cont = 0;
        for (i = 0; i < tamanho; i++) {
            if (mapa[i].tropas > 5) {
                cont++;
            }
        }
        if (cont >= 3) {
            return 1;
        }
        return 0;
    }

    // Missao 1: "Nao pode existir nenhum territorio da cor vermelho"
    if (strcmp(missao, MISSOES_PRE_DEFINIDAS[1]) == 0) {
        for (i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0) {
                return 0; // Ainda existe vermelho
            }
        }
        return 1; // Nao ha territorios vermelhos
    }

    // Missao 2: "Existir pelo menos 1 territorio com 10 ou mais tropas"
    if (strcmp(missao, MISSOES_PRE_DEFINIDAS[2]) == 0) {
        for (i = 0; i < tamanho; i++) {
            if (mapa[i].tropas >= 10) {
                return 1;
            }
        }
        return 0;
    }

    // Missao 3: "Existirem pelo menos 2 territorios da cor azul"
    if (strcmp(missao, MISSOES_PRE_DEFINIDAS[3]) == 0) {
        cont = 0;
        for (i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0) {
                cont++;
            }
        }
        if (cont >= 2) {
            return 1;
        }
        return 0;
    }

    // Missao 4: "Total de tropas no mapa maior que 30"
    if (strcmp(missao, MISSOES_PRE_DEFINIDAS[4]) == 0) {
        soma = 0;
        for (i = 0; i < tamanho; i++) {
            soma += mapa[i].tropas;
        }
        if (soma > 30) {
            return 1;
        }
        return 0;
    }

    // Caso alguma missão diferente venha a ser usada
    return 0;
}

// ---------------------------------------------------------
// Liberação de memória dinâmica
// ---------------------------------------------------------
void liberarMemoria(Territorio* mapa, int qtdTerritorios, char* missoesJogadores[], int qtdJogadores) {
    int i;
    if (mapa != NULL) {
        free(mapa);
    }

    for (i = 0; i < qtdJogadores; i++) {
        if (missoesJogadores[i] != NULL) {
            free(missoesJogadores[i]);
        }
    }

    (void)qtdTerritorios; // apenas para evitar warning se não for usado
}

// ---------------------------------------------------------
// Função principal (main)
// ---------------------------------------------------------
int main() {
    srand(time(NULL)); // Garante aleatoriedade para rand()

    int qtdTerritorios;
    Territorio* mapa = NULL;

    int qtdJogadores;
    char* missoesJogadores[MAX_JOGADORES] = { NULL, NULL };
    int i;

    int jogadorAtual;
    int idAtacante, idDefensor;
    char opcao;

    int houveVencedor = 0;
    int vencedor = -1;

    printf("=====================================\n");
    printf("  JOGO WAR - VERSAO SIMPLIFICADA C  \n");
    printf("=====================================\n\n");

    // ---------------------------------------------
    // 1) Definir quantidade de territorios (dinamico)
    // ---------------------------------------------
    printf("Informe o numero de territorios no mapa: ");
    scanf("%d", &qtdTerritorios);
    getchar(); // consome '\n'

    if (qtdTerritorios <= 0) {
        printf("Quantidade invalida. Encerrando.\n");
        return 0;
    }

    // Alocacao dinamica do vetor de territorios
    mapa = (Territorio*)calloc(qtdTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    // ---------------------------------------------
    // 2) Cadastro dos territorios
    // ---------------------------------------------
    cadastrarTerritorios(mapa, qtdTerritorios);

    // Mostrar o mapa apos cadastro (requisito nivel 1)
    exibirMapa(mapa, qtdTerritorios);

    // ---------------------------------------------
    // 3) Definir jogadores e atribuir missoes
    // ---------------------------------------------
    printf("Informe o numero de jogadores (1 ou %d): ", MAX_JOGADORES);
    scanf("%d", &qtdJogadores);
    getchar(); // consome '\n'

    if (qtdJogadores < 1 || qtdJogadores > MAX_JOGADORES) {
        printf("Numero de jogadores invalido. Usando 1 jogador.\n");
        qtdJogadores = 1;
    }

    // Atribuir missão dinamicamente para cada jogador
    for (i = 0; i < qtdJogadores; i++) {
        missoesJogadores[i] = (char*)malloc(MAX_MISSAO * sizeof(char));
        if (missoesJogadores[i] == NULL) {
            printf("Erro ao alocar memoria para missao do jogador %d.\n", i + 1);
            liberarMemoria(mapa, qtdTerritorios, missoesJogadores, qtdJogadores);
            return 1;
        }

        atribuirMissao(missoesJogadores[i], MISSOES_PRE_DEFINIDAS, TOTAL_MISSOES);

        // Exibir a missão apenas uma vez (no início do jogo)
        printf("\nMissao do Jogador %d:\n", i + 1);
        printf("-> %s\n", missoesJogadores[i]);
    }

    // ---------------------------------------------
    // 4) Loop de jogo com ataques
    // ---------------------------------------------
    do {
        exibirMapa(mapa, qtdTerritorios);

        printf("Escolha o jogador que vai atacar (1 a %d): ", qtdJogadores);
        scanf("%d", &jogadorAtual);
        getchar(); // consome '\n'

        if (jogadorAtual < 1 || jogadorAtual > qtdJogadores) {
            printf("Jogador invalido.\n");
            continue;
        }

        printf("Informe o ID do territorio atacante: ");
        scanf("%d", &idAtacante);
        getchar();

        printf("Informe o ID do territorio defensor: ");
        scanf("%d", &idDefensor);
        getchar();

        if (idAtacante < 0 || idAtacante >= qtdTerritorios ||
            idDefensor < 0 || idDefensor >= qtdTerritorios) {
            printf("ID de territorio invalido.\n");
            continue;
        }

        atacar(&mapa[idAtacante], &mapa[idDefensor]);

        // Verificar missoes de todos os jogadores
        for (i = 0; i < qtdJogadores; i++) {
            if (verificarMissao(missoesJogadores[i], mapa, qtdTerritorios)) {
                houveVencedor = 1;
                vencedor = i + 1;
                break;
            }
        }

        if (houveVencedor) {
            printf("\n=================================================\n");
            printf("          JOGADOR %d CUMPRIU SUA MISSAO!         \n", vencedor);
            printf("Missao: %s\n", missoesJogadores[vencedor - 1]);
            printf("                FIM DE JOGO!                     \n");
            printf("=================================================\n\n");
            break;
        }

        printf("Deseja realizar outro ataque? (s/n): ");
        scanf(" %c", &opcao);
        getchar();

    } while (opcao == 's' || opcao == 'S');

    // ---------------------------------------------
    // 5) Liberar memória alocada dinamicamente
    // ---------------------------------------------
    liberarMemoria(mapa, qtdTerritorios, missoesJogadores, qtdJogadores);

    printf("\nMemoria liberada. Programa encerrado.\n");
    return 0;
}
