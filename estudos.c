#include <stdio.h>
#include <stdlib.h>

#define LINHAS 25
#define COLUNAS 71
#define TOTAL (LINHAS * COLUNAS)
#define INF 1000000000

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

int dl[4] = {-1, 1, 0, 0};
int dc[4] = { 0, 0, -1, 1};

int distL[TOTAL];
int paiL[TOTAL];
int distD[TOTAL];
int paiD[TOTAL];
int vis[TOTAL];
int camL[TOTAL];
int camD[TOTAL];

int tam(int l)
{
    int n = 0;

    while (n < COLUNAS + 1 && lab[l][n] != '\0')
        n++;

    return n;
}

int valida(int *ent, int *sai)
{
    int l, c, n;
    int qtdE = 0, qtdS = 0;
    int ok = 1;
    char ch;

    for (l = 0; l < LINHAS; l++)
    {
        n = tam(l);
        if (n != COLUNAS)
        {
            printf("Erro: linha %d tem %d caracteres (esperado %d).\n", l, n, COLUNAS);
            ok = 0;
            continue;
        }
        for (c = 0; c < COLUNAS; c++)
        {
            ch = lab[l][c];
            if (ch == 'E')
            {
                qtdE++;
                *ent = l * COLUNAS + c;
            }
            else if (ch == 'S')
            {
                qtdS++;
                *sai = l * COLUNAS + c;
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

    return ok;
}

int viz(int v, int k)
{
    int l = v / COLUNAS;
    int c = v % COLUNAS;
    int nl = l + dl[k];
    int nc = c + dc[k];

    if (nl < 0 || nl >= LINHAS || nc < 0 || nc >= COLUNAS)
        return -1;
    if (lab[nl][nc] == '#')
        return -1;

    return nl * COLUNAS + nc;
}

int peso(int v)
{
    if (lab[v / COLUNAS][v % COLUNAS] == '~')
        return 3;
    return 1;
}

void bfs(int ini)
{
    int *fila;
    int ini_f = 0;
    int fim_f = 0;
    int i, k, u, w;

    fila = (int *) malloc(TOTAL * sizeof(int));
    if (fila == NULL)
    {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }

    for (i = 0; i < TOTAL; i++)
    {
        vis[i] = 0;
        distL[i] = INF;
        paiL[i] = -1;
    }

    vis[ini] = 1;
    distL[ini] = 0;
    fila[fim_f++] = ini;

    while (ini_f < fim_f)
    {
        u = fila[ini_f++];
        for (k = 0; k < 4; k++)
        {
            w = viz(u, k);
            if (w != -1 && !vis[w])
            {
                vis[w] = 1;
                distL[w] = distL[u] + 1;
                paiL[w] = u;
                fila[fim_f++] = w;
            }
        }
    }

    free(fila);
}

void dijk(int ini)
{
    int i, k, u, w, nd;

    for (i = 0; i < TOTAL; i++)
    {
        vis[i] = 0;
        distD[i] = INF;
        paiD[i] = -1;
    }
    distD[ini] = 0;

    while (1)
    {
        u = -1;
        for (i = 0; i < TOTAL; i++)
            if (!vis[i] && distD[i] < INF && (u == -1 || distD[i] < distD[u]))
                u = i;

        if (u == -1)
            break;

        vis[u] = 1;

        for (k = 0; k < 4; k++)
        {
            w = viz(u, k);
            if (w != -1 && !vis[w])
            {
                nd = distD[u] + peso(w);
                if (nd < distD[w])
                {
                    distD[w] = nd;
                    paiD[w] = u;
                }
            }
        }
    }
}

int monta(int *pai, int fim, int *cam)
{
    int n = 0, i, aux, v;

    for (v = fim; v != -1; v = pai[v])
        cam[n++] = v;

    for (i = 0; i < n / 2; i++)
    {
        aux = cam[i];
        cam[i] = cam[n - 1 - i];
        cam[n - 1 - i] = aux;
    }

    return n;
}

int custo(int *cam, int n)
{
    int i, soma = 0;

    for (i = 1; i < n; i++)
        soma += peso(cam[i]);

    return soma;
}

void lista(int *cam, int n)
{
    int i;

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

#define SEM_COR  0
#define CINZA    100
#define CIANO    46
#define VERDE    42
#define ROXO     45
#define AMARELO  43
#define BRANCO   47

int cor(int modo, int v, int sai, char *noL, char *noD)
{
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

void desenha(int modo, int sai, int nL, int nD)
{
    char *noL, *noD;
    int i, l, c, v, f;
    char ch;

    noL = (char *) malloc(TOTAL * sizeof(char));
    noD = (char *) malloc(TOTAL * sizeof(char));
    if (noL == NULL || noD == NULL)
    {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }

    for (i = 0; i < TOTAL; i++)
    {
        noL[i] = 0;
        noD[i] = 0;
    }
    for (i = 0; i < nL; i++)
        noL[camL[i]] = 1;
    for (i = 0; i < nD; i++)
        noD[camD[i]] = 1;

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

int igual(int *a, int na, int *b, int nb)
{
    int i;

    if (na != nb)
        return 0;
    for (i = 0; i < na; i++)
        if (a[i] != b[i])
            return 0;

    return 1;
}

void compara(int nL, int cL, int nD, int cD)
{
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

int main(void)
{
    int ent = -1, sai = -1;
    int nL = 0, nD = 0;
    int cL = 0, cD = 0;

    if (!valida(&ent, &sai))
    {
        printf("Labirinto invalido. As buscas nao serao executadas.\n");
        return 1;
    }

    bfs(ent);
    dijk(ent);

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

    if (distL[sai] != INF && distD[sai] != INF)
    {
        compara(nL, cL, nD, cD);
        printf("\nVerde = so largura, amarelo = so Dijkstra, branco = os dois\n");
        desenha(3, sai, nL, nD);
    }
    else
        printf("\nSaida qpinalcancavel: comparacao nao realizada.\n");

    return 0;
}
