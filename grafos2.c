//------------------------------------------------------------------//
// Centro Universitário - Católica de Santa Catarina - Joinville/SC //
// Curso: Engenharia de Software - Segundo período                 //
// Matéria: Estrutura de Dados - Grafos                             //
// Professor: Manfred Heil Junior                                   //
// Alunos: Beatriz Cercal Cachoeira e Miguel Rochar Xavier          //
// Data: 22/11/2026.                                                //
//------------------------------------------------------------------//

//------------------------------------------------------------------//
//                           DECISÕES                               //
// 1- Labirinto: 25 x 71 em char lab[LINHAS][COLUNAS + 1];          //
// 2- Validação: 71 caracteres por linha. Só valem: espaço (normal) //
// ~ (lama), # (parede), E (entrada) e S (saída). Precisa ter um E  //
// e um S. Se for inválido, avisa e não roda as buscas;             //
// 3- Peso: entrar na lama = 3, resto = 1;                          //
// 4- Vizinhos: cima, baixo, esquerda e direita;                    //
// 5- Desempate Dijkstra: menor índice. Pai só troca se achar um    //
// caminho mais barato (em empate, mantém o primeiro);              //
// 6- Saída: (linha, coluna) a partir de 0, contando a borda;       //
// 7- Grafo: Grade implícita. Não guarda os vizinhos, olha          //
// cima, baixo, esquerda e direita na hora. Uma matriz de pesos     //
// teria 1775 x 1775 posições, quase todas vazias.                  //
// 8- Borda: não é conferida na validação (o enunciado não pede).   //
// viz() testa os limites da matriz antes de olhar se é parede,     //
// então nunca lê fora do labirinto, mesmo sem borda de '#'.        //
//------------------------------------------------------------------//

//------------------------------------------------------------------//
//                     DETALHES DO MODELO                           //
// - Vértice: cada casa que não é parede (espaço, ~, E e S).        //
//   A casa (linha, coluna) vira o número linha * COLUNAS + coluna. //
// - Aresta: liga duas casas vizinhas (cima, baixo, esq., dir.)     //
//   que não são parede. Parede não vira vértice nem aresta.        //
// - Etapa 1 (largura): toda aresta vale 1 passo.                   //
// - Etapa 2 (Dijkstra): grafo dirigido, dois arcos por passagem.   //
//   O peso do arco u -> v é o custo de ENTRAR em v (3 se v é lama, //
//   1 caso contrário). Por isso ir para a lama custa 3 e sair dela //
//   custa 1: os pesos não são simétricos.                          //
// - Custo do caminho: soma do custo de entrada de cada casa,       //
//   menos a entrada E (onde já começamos).                         //
// - Passos: um caminho com n passos passa por n + 1 casas.         //
// - Por que largura na etapa 1: sem pesos, a fila visita as casas  //
//   por camadas (0, 1, 2...), então a primeira vez que uma casa é  //
//   alcançada já é pelo menor número de passos.                    //
// - Por que Dijkstra na etapa 2: com a lama, o caminho de menos    //
//   passos pode não ser o mais barato, e a largura ignora pesos.   //
// - Dijkstra serve aqui porque nenhum custo é negativo (1 ou 3).   //
// - Casa inalcançável fica com distância infinita (INF) e nenhum   //
//   caminho é impresso. Se a saída não é alcançável, as duas       //
//   etapas avisam e a comparação não é feita.                      //
// - Mínimo do Dijkstra: busca linear pela casa não finalizada de   //
//   menor distância (1775 casas, é rápido o bastante).             //
//------------------------------------------------------------------//

#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, free

//------------------------------------------------------------------//
//                         CONSTANTES                               //
//------------------------------------------------------------------//
#define LINHAS 25                    // número de linhas do labirinto
#define COLUNAS 71                   // número de colunas do labirinto
#define TOTAL (LINHAS * COLUNAS)     // total de casas (1775)
#define INF 1000000000               // "infinito": casa ainda não alcançada

//------------------------------------------------------------------//
//                         LABIRINTO                                //
// Cada linha é uma string de 71 caracteres. O tamanho COLUNAS + 1  //
// guarda o '\0' do fim, e é ele que permite conferir o tamanho de  //
// cada linha na validação.                                         //
//------------------------------------------------------------------//
char lab[LINHAS][COLUNAS + 1] = {
    "#######################################################################",
    "#E  #                 #         #       #             ~       #       #",
    "###~# ### # ### ### # # ####### ##### ### ######### ##~## ### ####### #",
    "#       # #   # #   #   #        ~~ # ~ ~ #   #  ~  #  ~#   #~  #   # #",
    "# ### ###~### # # ##### # #### ####~##### ### # ##### # ### ### # # # #",
    "#   #~#   #   # #     # #   #~    #~~ # ~   ~   #     # ~ #   # ~ #   #",
    "### #~# ### ### ##### # ### # ### ### ### # ##### ####### # #~####### #",
    "# #   # #  ~  #   #   #     #   # #     # #         #       #     #   #",
    "# # # # ##### ##### ########### # #####~##### ####### ##### # #~# # ###",
    "# # # # #   #    ~#  ~          #     #     # # #     #       #~# ~   #",
    "# # # # # # ### # #####~#### ######## ##### # # # ##### ####### ##### #",
    "#   #  ~# # ~ # # #     #           # #   # #  ~#   # #  ~# #   #  ~ ~#",
    "# ### # # ### # #~# # # ##### ####### # ### # # ### # ### # # #~# #####",
    "#   # #   #~# #~#   # # #     #   ~ # # ~   # #  ~# # ~ # # # # #     #",
    "### # ##### # # ##### # # #####~### # #~##### # # # # ### # # ##### #~#",
    "# # ~       # #~    # #~  #   ~ # #       #     # # # #   # # #   # # #",
    "# # # ####### # ###~# ##### ## ## ####### ####### # #~# ###   # # #~#~#",
    "#   #  ~~ # ~ #   # # #   # #     #     #   #   # # #       #   # # # #",
    "#~### # # # ### # ### #~# # ### # # #~# ### ### # # #### ##~#####   # #",
    "#   # # #   # # #   #   #   #~ ~# # # #   # ~ # # #   #~  # #     # # #",
    "### # # ##### # ### ######### ### # # ####### #~#~### # # # #~##### # #",
    "# #~~ # #    ~  #~  ~     #  ~#     #     #       ~ #~  # # #   #   # #",
    "# # ###   ####### ###~##### # # ###### ## # ############# # ### # #####",
    "#      ~#           #    ~  #   ~    ~   ~   ~~           #   #  ~   S#",
    "#######################################################################"
};

//------------------------------------------------------------------//
//                   VETORES DE DIREÇÃO                             //
// Ordem dos vizinhos: cima, baixo, esquerda, direita (decisão 4).  //
// Somando dl[k] na linha e dc[k] na coluna, chega-se ao vizinho k. //
//------------------------------------------------------------------//
int dl[4] = {-1, 1, 0, 0};
int dc[4] = { 0, 0, -1, 1};

//------------------------------------------------------------------//
//                   VETORES GLOBAIS DAS BUSCAS                     //
// Ficam fora das funções para não ocupar a pilha (stack).          //
//------------------------------------------------------------------//
int distL[TOTAL];   // largura: número de passos da entrada até cada casa
int paiL[TOTAL];    // largura: casa de onde se chegou em cada casa
int distD[TOTAL];   // Dijkstra: menor custo da entrada até cada casa
int paiD[TOTAL];    // Dijkstra: casa de onde se chegou em cada casa
int vis[TOTAL];     // controle de visitados (largura) / finalizados (Dijkstra)
int camL[TOTAL];    // caminho da largura, da entrada até a saída
int camD[TOTAL];    // caminho do Dijkstra, da entrada até a saída

//------------------------------------------------------------------//
// tam: conta o tamanho da linha l do labirinto.                    //
// Para em COLUNAS + 1 para nunca ler fora da linha, mesmo que a    //
// string esteja sem o '\0' (linha com caracteres a mais).          //
//------------------------------------------------------------------//
int tam(int l)
{
    // variaveis
    int n = 0;

    // processamento
    while (n < COLUNAS + 1 && lab[l][n] != '\0')
        n++;

    // saida
    return n;
}

//------------------------------------------------------------------//
// valida: confere o labirinto (decisão 2).                         //
// - cada linha precisa ter exatamente COLUNAS caracteres;          //
// - só valem os caracteres ' ', '~', '#', 'E' e 'S';               //
// - precisa ter exatamente um 'E' e um 'S'.                        //
// Guarda em *ent e *sai o número da casa da entrada e da saída.    //
// Devolve 1 se o labirinto é válido e 0 se não é.                  //
//------------------------------------------------------------------//
int valida(int *ent, int *sai)
{
    // variaveis
    int l, c, n;
    int qtdE = 0, qtdS = 0;   // quantas entradas e saídas apareceram
    int ok = 1;               // começa válido; qualquer erro zera
    char ch;

    // processamento
    for (l = 0; l < LINHAS; l++)
    {
        n = tam(l);
        if (n != COLUNAS)
        {
            printf("Erro: linha %d tem %d caracteres (esperado %d).\n", l, n, COLUNAS);
            ok = 0;
            continue;   // não olha os caracteres de uma linha com tamanho errado
        }
        for (c = 0; c < COLUNAS; c++)
        {
            ch = lab[l][c];
            if (ch == 'E')
            {
                qtdE++;
                *ent = l * COLUNAS + c;   // número da casa da entrada
            }
            else if (ch == 'S')
            {
                qtdS++;
                *sai = l * COLUNAS + c;   // número da casa da saída
            }
            else if (ch != ' ' && ch != '~' && ch != '#')
            {
                printf("Erro: caractere invalido '%c' em (%d, %d).\n", ch, l, c);
                ok = 0;
            }
        }
    }
    if (qtdE != 1)
    {
        printf("Erro: o labirinto tem %d entradas 'E' (precisa ter 1).\n", qtdE);
        ok = 0;
    }
    if (qtdS != 1)
    {
        printf("Erro: o labirinto tem %d saidas 'S' (precisa ter 1).\n", qtdS);
        ok = 0;
    }

    // saida
    return ok;
}

//------------------------------------------------------------------//
// viz: devolve o número do vizinho k da casa v (k = 0..3, na ordem //
// dos vetores dl e dc), ou -1 se ele sai do labirinto ou é parede. //
// É aqui que a grade implícita vira grafo: as arestas não ficam    //
// guardadas, são descobertas na hora (decisão 7).                  //
//------------------------------------------------------------------//
int viz(int v, int k)
{
    // variaveis
    int l = v / COLUNAS;      // linha da casa v
    int c = v % COLUNAS;      // coluna da casa v
    int nl = l + dl[k];       // linha do vizinho
    int nc = c + dc[k];       // coluna do vizinho

    // processamento
    if (nl < 0 || nl >= LINHAS || nc < 0 || nc >= COLUNAS)
        return -1;            // fora do labirinto
    if (lab[nl][nc] == '#')
        return -1;            // parede não é vértice

    // saida
    return nl * COLUNAS + nc;
}

//------------------------------------------------------------------//
// peso: custo de ENTRAR na casa v (decisão 3).                     //
// Lama ('~') custa 3; espaço, E e S custam 1.                      //
// É o peso do arco que chega em v.                                 //
//------------------------------------------------------------------//
int peso(int v)
{
    // processamento e saida
    if (lab[v / COLUNAS][v % COLUNAS] == '~')
        return 3;
    return 1;
}

//------------------------------------------------------------------//
// bfs: busca em largura a partir da casa ini (etapa 1).            //
// Usa uma fila: a primeira casa que entra é a primeira que sai,    //
// então as casas são processadas por camadas de distância (0, 1,   //
// 2, ...). Por isso, a primeira vez que uma casa é alcançada já é  //
// pelo menor número de passos. O vetor vis impede que uma casa     //
// entre duas vezes na fila.                                        //
//------------------------------------------------------------------//
void bfs(int ini)
{
    // variaveis
    int *fila;              // fila em vetor, alocada com malloc
    int ini_f = 0;          // posição de quem sai da fila
    int fim_f = 0;          // posição onde entra o próximo
    int i, k, u, w;

    // entrada
    // cada casa entra no máximo uma vez, então TOTAL posições bastam
    fila = (int *) malloc(TOTAL * sizeof(int));
    if (fila == NULL)
    {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }

    // processamento
    // começa com tudo não visitado, distância infinita e sem pai
    for (i = 0; i < TOTAL; i++)
    {
        vis[i] = 0;
        distL[i] = INF;
        paiL[i] = -1;
    }

    // a entrada está a 0 passos dela mesma e é a primeira da fila
    vis[ini] = 1;
    distL[ini] = 0;
    fila[fim_f++] = ini;

    while (ini_f < fim_f)               // enquanto a fila não está vazia
    {
        u = fila[ini_f++];              // tira a primeira casa da fila
        for (k = 0; k < 4; k++)         // olha os 4 vizinhos
        {
            w = viz(u, k);
            if (w != -1 && !vis[w])     // vizinho livre e ainda não visitado
            {
                vis[w] = 1;             // marca para não entrar de novo
                distL[w] = distL[u] + 1;// um passo a mais que u
                paiL[w] = u;            // chegou em w vindo de u
                fila[fim_f++] = w;      // coloca no fim da fila
            }
        }
    }

    // saida
    free(fila);
}

//------------------------------------------------------------------//
// dijk: algoritmo de Dijkstra a partir da casa ini (etapa 2).      //
// A cada rodada escolhe a casa ainda não finalizada com menor      //
// custo (busca linear; em empate fica a de menor número, decisão   //
// 5), finaliza essa casa e relaxa os arcos que saem dela: se ir    //
// por ela deixa um vizinho mais barato, atualiza custo e pai.      //
// Como os pesos são positivos, o custo de uma casa finalizada não  //
// pode mais diminuir, e é por isso que o Dijkstra funciona aqui.   //
//------------------------------------------------------------------//
void dijk(int ini)
{
    // variaveis
    int i, k, u, w, nd;

    // processamento
    // começa com custo infinito, sem pai e nada finalizado
    for (i = 0; i < TOTAL; i++)
    {
        vis[i] = 0;
        distD[i] = INF;
        paiD[i] = -1;
    }
    distD[ini] = 0;                     // a entrada não conta no custo

    while (1)
    {
        // seleção do mínimo: busca linear pela casa mais barata
        // ainda não finalizada. O "<" estrito faz o empate ficar
        // com a casa de menor número (a primeira encontrada).
        u = -1;
        for (i = 0; i < TOTAL; i++)
            if (!vis[i] && distD[i] < INF && (u == -1 || distD[i] < distD[u]))
                u = i;

        if (u == -1)                    // não sobrou casa alcançável
            break;

        vis[u] = 1;                     // custo de u agora é definitivo

        // relaxamento dos arcos u -> w
        for (k = 0; k < 4; k++)
        {
            w = viz(u, k);
            if (w != -1 && !vis[w])
            {
                nd = distD[u] + peso(w);    // custo indo por u
                if (nd < distD[w])          // só troca se ficar mais barato
                {
                    distD[w] = nd;
                    paiD[w] = u;
                }
            }
        }
    }
}

//------------------------------------------------------------------//
// monta: reconstrói o caminho até a casa fim seguindo os pais.     //
// Os pais levam da saída de volta à entrada, então o caminho é     //
// montado de trás para frente e depois invertido.                  //
// Devolve a quantidade de CASAS do caminho (passos = casas - 1).   //
//------------------------------------------------------------------//
int monta(int *pai, int fim, int *cam)
{
    // variaveis
    int n = 0, i, aux, v;

    // processamento
    // volta da saída até a entrada (a entrada tem pai -1)
    for (v = fim; v != -1; v = pai[v])
        cam[n++] = v;

    // inverte para ficar da entrada até a saída
    for (i = 0; i < n / 2; i++)
    {
        aux = cam[i];
        cam[i] = cam[n - 1 - i];
        cam[n - 1 - i] = aux;
    }

    // saida
    return n;
}

//------------------------------------------------------------------//
// custo: soma o custo de entrada das casas do caminho.             //
// Começa em i = 1 porque a entrada (cam[0]) não conta.             //
//------------------------------------------------------------------//
int custo(int *cam, int n)
{
    // variaveis
    int i, soma = 0;

    // processamento
    for (i = 1; i < n; i++)
        soma += peso(cam[i]);

    // saida
    return soma;
}

//------------------------------------------------------------------//
// lista: imprime as casas do caminho como (linha, coluna),         //
// contando a partir de 0 e incluindo a borda (decisão 6).          //
// Quebra a linha a cada 8 casas para caber na tela.                //
//------------------------------------------------------------------//
void lista(int *cam, int n)
{
    // variaveis
    int i;

    // saida
    for (i = 0; i < n; i++)
    {
        printf("(%d, %d)", cam[i] / COLUNAS, cam[i] % COLUNAS);
        if (i < n - 1)
            printf(" -> ");
        if ((i + 1) % 8 == 0)
            printf("\n");
    }
    printf("\n");
}

//------------------------------------------------------------------//
//                         CORES (opcional)                         //
// Código ANSI: \033[30;46m = letra preta (30) com fundo ciano (46).//
// \033[0m volta para a cor normal. Funciona no Linux, no Mac e     //
// nos terminais do Windows que aceitam ANSI, como o Windows        //
// Terminal.                                                        //
// Num console que não aceita, aparecem os códigos no lugar da cor, //
// mas o caminho continua impresso em (linha, coluna).              //
//------------------------------------------------------------------//
#define SEM_COR  0
#define CINZA    100
#define CIANO    46
#define VERDE    42
#define ROXO     45
#define AMARELO  43
#define BRANCO   47

//------------------------------------------------------------------//
// cor: decide a cor de fundo da casa v em cada desenho.            //
// modo 1 (largura): caminho = verde; casas com distância <= a da   //
//   saída = ciano.                                                 //
// modo 2 (Dijkstra): caminho = amarelo; casas com custo <= o da    //
//   saída = roxo.                                                  //
// Obs.: as buscas percorrem o labirinto inteiro (não param no S).  //
//   Ciano/roxo mostram quem está até a distância/custo da saída,   //
//   não exatamente as casas processadas.                           //
// modo 3 (comparação): só largura = verde, só Dijkstra = amarelo,  //
//   nos dois = branco.                                             //
//------------------------------------------------------------------//
int cor(int modo, int v, int sai, char *noL, char *noD)
{
    // processamento e saida
    if (modo == 1)
    {
        if (noL[v]) return VERDE;
        if (distL[v] <= distL[sai]) return CIANO;
    }
    else if (modo == 2)
    {
        if (noD[v]) return AMARELO;
        if (distD[v] <= distD[sai]) return ROXO;
    }
    else
    {
        if (noL[v] && noD[v]) return BRANCO;
        if (noL[v]) return VERDE;
        if (noD[v]) return AMARELO;
    }
    return SEM_COR;
}

//------------------------------------------------------------------//
// desenha: mostra o labirinto colorido (modo 1, 2 ou 3, ver cor). //
// Parede fica cinza; E e S ficam sem cor. O caractere original     //
// continua aparecendo, então a lama (~) segue visível.             //
//------------------------------------------------------------------//
void desenha(int modo, int sai, int nL, int nD)
{
    // variaveis
    char *noL, *noD;          // 1 = a casa está no caminho
    int i, l, c, v, f;
    char ch;

    // entrada
    noL = (char *) malloc(TOTAL * sizeof(char));
    noD = (char *) malloc(TOTAL * sizeof(char));
    if (noL == NULL || noD == NULL)
    {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }

    // processamento: marca as casas de cada caminho
    for (i = 0; i < TOTAL; i++)
    {
        noL[i] = 0;
        noD[i] = 0;
    }
    for (i = 0; i < nL; i++)
        noL[camL[i]] = 1;
    for (i = 0; i < nD; i++)
        noD[camD[i]] = 1;

    // saida
    for (l = 0; l < LINHAS; l++)
    {
        for (c = 0; c < COLUNAS; c++)
        {
            v = l * COLUNAS + c;
            ch = lab[l][c];

            if (ch == '#')
                f = CINZA;
            else if (ch == 'E' || ch == 'S')
                f = SEM_COR;
            else
                f = cor(modo, v, sai, noL, noD);

            if (f == SEM_COR)
                printf("%c", ch);
            else
                printf("\033[30;%dm%c\033[0m", f, ch);
        }
        printf("\n");
    }
    free(noL);
    free(noD);
}

//------------------------------------------------------------------//
// igual: devolve 1 se os dois caminhos têm exatamente a mesma      //
// sequência de casas, e 0 se não têm.                              //
//------------------------------------------------------------------//
int igual(int *a, int na, int *b, int nb)
{
    // variaveis
    int i;

    // processamento
    if (na != nb)
        return 0;
    for (i = 0; i < na; i++)
        if (a[i] != b[i])
            return 0;

    // saida
    return 1;
}

//------------------------------------------------------------------//
// compara: comparação final entre os dois caminhos (etapa 2, e).   //
// Mostra passos e custo de cada um, diz se são a mesma sequência   //
// de casas e, se forem diferentes com o mesmo custo, avisa.        //
//------------------------------------------------------------------//
void compara(int nL, int cL, int nD, int cD)
{
    // saida
    printf("\n=============== COMPARACAO ===============\n");
    printf("Largura : %d passos, custo %d\n", nL - 1, cL);
    printf("Dijkstra: %d passos, custo %d\n", nD - 1, cD);

    if (igual(camL, nL, camD, nD))
    {
        printf("Os dois caminhos sao a mesma sequencia de casas.\n");
    }
    else
    {
        printf("Os caminhos sao diferentes.\n");
        if (cL == cD)
            printf("Sao caminhos diferentes com o mesmo custo (%d).\n", cL);
        else if (cL > cD)
            printf("O caminho com menos passos (largura) custa mais: %d a mais.\n", cL - cD);
    }
}

//------------------------------------------------------------------//
//                         PRINCIPAL                                //
//------------------------------------------------------------------//
int main(void)
{
    // variaveis
    int ent = -1, sai = -1;     // número da casa da entrada e da saída
    int nL = 0, nD = 0;         // quantidade de casas de cada caminho
    int cL = 0, cD = 0;         // custo de cada caminho


    // entrada
    // o labirinto já está no código (matriz lab); aqui só é validado
    if (!valida(&ent, &sai))
    {
        printf("Labirinto invalido. As buscas nao serao executadas.\n");
        return 1;
    }

    // processamento
    bfs(ent);    // etapa 1: menor número de passos
    dijk(ent);   // etapa 2: menor custo

    // saida
    // ---------- etapa 1: busca em largura ----------
    printf("========== ETAPA 1: BUSCA EM LARGURA ==========\n");
    if (distL[sai] == INF)
    {
        printf("Nao existe caminho da entrada ate a saida.\n");
    }
    else
    {
        nL = monta(paiL, sai, camL);
        cL = custo(camL, nL);
        printf("Caminho com menos passos:\n");
        lista(camL, nL);
        printf("Passos: %d\n", nL - 1);
        printf("Custo : %d\n", cL);
        printf("Ciano = distancia <= a da saida, verde = caminho\n");
        desenha(1, sai, nL, 0);
    }

    // ---------- etapa 2: Dijkstra ----------
    printf("\n============== ETAPA 2: DIJKSTRA ==============\n");
    if (distD[sai] == INF)
    {
        printf("Nao existe caminho da entrada ate a saida.\n");
    }
    else
    {
        nD = monta(paiD, sai, camD);
        cD = custo(camD, nD);
        printf("Caminho de menor custo:\n");
        lista(camD, nD);
        printf("Passos: %d\n", nD - 1);
        printf("Custo : %d\n", cD);
        printf("Roxo = custo <= o da saida, amarelo = caminho\n");
        desenha(2, sai, 0, nD);
    }

    // ---------- comparação (só se a saída foi alcançada) ----------
    if (distL[sai] != INF && distD[sai] != INF)
    {
        compara(nL, cL, nD, cD);
        printf("\nVerde = so largura, amarelo = so Dijkstra, branco = os dois\n");
        desenha(3, sai, nL, nD);
    }
    else
        printf("\nSaida inalcancavel: comparacao nao realizada.\n");

    return 0;
}
