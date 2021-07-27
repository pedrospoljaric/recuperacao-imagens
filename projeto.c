#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <locale.h>
#define T 4

typedef struct TItem
{
    char id[20];
}TItem;

typedef struct TNo
{
    TItem item[2*T-1];
    struct TNo *pagina[2*T];
    int nInfo;
}TNo;

typedef struct TArvB
{
    TNo *raiz;
}TArvB;

void InicializarArv(TArvB *arv);
TItem Quebrar(TNo *n, TNo *dir);
void Unir(TNo *n, int indice);
void EmpurrarItens(TNo *n, int indice);
void PuxarItens(TNo *n, int indice);
void Inserir(TArvB *arv, TItem it);
TItem Remover(TNo *no, int indice);
void PrintTree(TNo *no, int h);
TItem ProcurarParaExcluir(TArvB *arv, TItem it);
int StartsWith(const char *a, const char *b);
void Pesquisar(TNo *n, TItem it);

int main()
{
    setlocale(LC_ALL, "Portuguese"); // acentos e afins

    printf("                                                          B\n");
    printf("                                                         BBB\n");
    printf("           _____ __      __ ____   _____   ______       BBBBB\n");
    printf("    /%c    |  __ %c%c %c    / // __ %c |  __ %c |  ____|     BBBBBBB\n", 92, 92, 92, 92, 92, 92);
    printf("   /  %c   | |__) |%c %c  / /| |  | || |__) || |__       BBBBBBBBB\n", 92, 92, 92);
    printf("  / /%c %c  |  _  /  %c %c/ / | |  | ||  _  / |  __|    BBBBBBBBBBBBB\n", 92, 92, 92, 92);
    printf(" / ____ %c | | %c %c   %c  /  | |__| || | %c %c | |____        BBB\n", 92, 92, 92, 92, 92, 92);
    printf("/_/    %c_%c|_|  %c_%c   %c/    %c____/ |_|  %c_%c|______|       BBB\n", 92, 92, 92, 92, 92, 92, 92, 92);

    int i, op;
    struct stat sb; // para verificar se arquivo existe

    TArvB arv;
    InicializarArv(&arv);

    char nome[20], pasta[30];
    TItem it;
    while (1)
    {
        printf("-------------------------------------------------------------------\n");
        printf("\t\t        ..::.. MENU ..::..\n   1 Inserir :: 2 Buscar :: 3 Imprimir :: 4 Remover :: 5 Exibir\n");

        scanf("%d", &op);
        switch (op)
        {
            case 1:
                printf("Exemplo: imagem3.jpg\n");

                printf("-Nome do arquivo: ");
                strcpy(pasta, "_imagens/");
                scanf("%s", nome);
                strcat(pasta, nome); //pasta = '_imagens/oi.jpg'

                if (stat(pasta, &sb) == -1)
                {
                    printf("Arquivo não encontrado.\n");
                }
                else
                {
                    strcpy(it.id, nome);
                    Inserir(&arv, it);
                    printf("-Item inserido.\n");
                }
            break;
            case 2:
                printf("Exemplo: imagem\n");
                printf("-Começo do nome do arquivo: ");

                scanf("%s", nome);
                strcpy(it.id, nome);

                printf("-Resultado(s):\n");
                Pesquisar(arv.raiz, it);
            break;
            case 3:
                PrintTree(arv.raiz, 0);
                printf("\n");
            break;
            case 4:
                printf("Exemplo: imagem3.jpg\n");
                printf("-Nome do arquivo: ");

                scanf("%s", nome);
                strcpy(it.id, nome);

                ProcurarParaExcluir(&arv, it);

                if (arv.raiz != NULL && arv.raiz->nInfo == 0)
                    arv.raiz = arv.raiz->pagina[0];
            break;
            case 5:
                printf("Exemplo: imagem3\n");
                printf("-Nome do arquivo (sem extensão): ");

                scanf("%s", nome);
                strcat(nome, ".jpg");
                strcpy(it.id, nome);

                strcpy(pasta, "_imagens/");
                strcat(pasta, nome);
                printf("-Abrir %s\n", pasta);

                if (stat(pasta, &sb) == -1)
                {
                    printf("Arquivo não encontrado.\n");
                }
                else
                {
                    char st[40];
                    strcpy(st, "start ");
                    strcat(st, pasta); // st = 'start _imagens/oi.jpg'
                    system(st); // abre o arquivo
                }
            break;
            case 0:
                system("cls"); // limpa a tela
                printf("                                                          B\n");
                printf("                                                         BBB\n");
                printf("           _____ __      __ ____   _____   ______       BBBBB\n");
                printf("    /%c    |  __ %c%c %c    / // __ %c |  __ %c |  ____|     BBBBBBB\n", 92, 92, 92, 92, 92, 92);
                printf("   /  %c   | |__) |%c %c  / /| |  | || |__) || |__       BBBBBBBBB\n", 92, 92, 92);
                printf("  / /%c %c  |  _  /  %c %c/ / | |  | ||  _  / |  __|    BBBBBBBBBBBBB\n", 92, 92, 92, 92);
                printf(" / ____ %c | | %c %c   %c  /  | |__| || | %c %c | |____        BBB\n", 92, 92, 92, 92, 92, 92);
                printf("/_/    %c_%c|_|  %c_%c   %c/    %c____/ |_|  %c_%c|______|       BBB\n", 92, 92, 92, 92, 92, 92, 92, 92);
            break;
            default:
                printf("Opção inválida.\n");
            break;
        }
    }

    return 0;
}

void InicializarArv(TArvB *arv)
{
    arv->raiz = NULL;
}

void Pesquisar(TNo *n, TItem it)
{
    if (n != NULL)
    {
        int i;
        for (i = 0; i < n->nInfo; i++) // acessa todos os filhos de n
        {
            if (StartsWith(n->item[i].id, it.id)) // comeca com a palavra pesquisada -> procura por mais no filho a esquerda e printa
            {
                Pesquisar(n->pagina[i], it);
                printf("%s\n", n->item[i].id);
            }
            else if (strcmp(it.id, n->item[i].id) < 0) // se a palavra pesquisada for 'menor' que o item atual, vai par ao filho da esquerda e termina a pesquisa nesse grupo de paginas
            {
                Pesquisar(n->pagina[i], it);
                break;
            }
        }
        if (i == n->nInfo) // chegou no final do no sem descer para alguma pagina -> desce para a ultima
            Pesquisar(n->pagina[i], it);
    }
}

void Inserir(TArvB *arv, TItem it)
{
    int i, j, indice;

    if (arv->raiz == NULL) // um novo no eh alocado quando a arvore esta vazia ou quando a raiz sobe (mais para frente no codigo)
    {
        TNo *novo = (TNo*) malloc(sizeof(TNo));

        novo->item[0] = it;
        novo->nInfo = 1;
        novo->pagina[0] = NULL;
        novo->pagina[1] = NULL;
        arv->raiz = novo;
    }
    else
    {
        if (arv->raiz->nInfo == 2*T-1) // raiz sobe
        {
            TNo *novo = (TNo*) malloc(sizeof(TNo));
            TNo *dir = (TNo*) malloc(sizeof(TNo));

            novo->item[0] = Quebrar(arv->raiz, dir);
            novo->pagina[0] = arv->raiz;
            novo->pagina[1] = dir;
            novo->nInfo = 1;
            arv->raiz = novo;
        }

        TNo *aux = arv->raiz;
        while (aux->pagina[0] != NULL) // todas as paginas dos nos nao-folha tem conteudo em todos os filhos (quero chegar numa folha)
        {
            for (i = 0; i < aux->nInfo; i++) // percorro os itens do no ate saber onde descer (do lado esquerdo de um item maior que o que vai ser inserido)
            {
                if (strcmp(it.id, aux->item[i].id) < 0)
                    break;
            }

            if (i == aux->nInfo) // maior que o ultimo (nInfo-1) elemento do vetor (ou seja, todos)
            {
                if (aux->pagina[aux->nInfo]->nInfo == 2*T-1) // antes de descer, verifica-se se o a pagina ser quebrada
                {
                    TNo *dir = (TNo*) malloc(sizeof(TNo));

                    aux->item[aux->nInfo] = Quebrar(aux->pagina[aux->nInfo], dir);
                    aux->pagina[aux->nInfo+1] = dir;
                    aux->nInfo++;
                }
                if (strcmp(it.id, aux->item[aux->nInfo-1].id) < 0) // destino pode ter sido alterado na quebra da pagina
                    aux = aux->pagina[aux->nInfo-1];
                else
                    aux = aux->pagina[aux->nInfo];
            }
            else // entrou em uma pagina do laod esquerdo de algum item
            {
                if (aux->pagina[i]->nInfo == 2*T-1) // quebra se necessario
                {
                    EmpurrarItens(aux, i);

                    TNo *dir = (TNo*) malloc(sizeof(TNo));

                    aux->item[i] = Quebrar(aux->pagina[i], dir);
                    aux->pagina[i+1] = dir;
                    aux->nInfo++;
                }
                if (strcmp(it.id, aux->item[i].id) < 0) // destino pode ter mudado
                    aux = aux->pagina[i];
                else
                    aux = aux->pagina[i+1];
            }
        }// sai do while -> encontrei folha

        indice = 0; // na folha, quero descobrir em que posicao inserir o item
        while (indice < aux->nInfo && strcmp(it.id, aux->item[indice].id) > 0) indice++;

        if (indice < aux->nInfo) // tenho quer inserir entre dois itens ou no final?
            EmpurrarItens(aux, indice); // empurro todos a minha direita para entrar no meio

        aux->item[indice] = it;
        aux->pagina[indice+1] = NULL;
        aux->nInfo++;
        printf("-Item inserido\n");
    }
}

TItem ProcurarParaExcluir(TArvB *arv, TItem it) // percorre a arvore ate encontrar o item a ser excluido, caso ele exista
{
    int i;
    TNo *aux = arv->raiz;

    while (aux != NULL) // se chegar a null -> item nao existe
    {
        for (i = 0; i < aux->nInfo; i++) // verifica se item esta no noh atual
        {
            if (strcmp(it.id, aux->item[i].id) == 0) // se estiver, remove e ja sai da funcao
            {
                printf("-Item removido.\n");
                return Remover(aux, i);
            }

            if (strcmp(it.id, aux->item[i].id) < 0) // se encontrar alguem maior, nao preciso continuar percorrendo o mesno no e vou descer para uma pagina (i)
                break;
        }

        if (aux->pagina[0] == NULL) break; // se nao encontrei ainda e nao puder descer, item nao existe

        if (i == aux->nInfo) // item a ser excluido eh maior que o ultimo item do no (explicado em inserir)
        {
            if (aux->pagina[aux->nInfo]->nInfo == T-1) // antes de descer, a pagina deve ter pelo menos T itens (explicado melhor no relatorio)
            {
                if (aux->pagina[aux->nInfo-1]->nInfo == T-1) // pagina da esquerda tambem tem T-1
                {
                    Unir(aux, aux->nInfo-1); // item desce, unindo paginas e posicoes sao atualizadas
                    PuxarItens(aux, aux->nInfo-1);
                    aux->nInfo--;
                }
                else // se esquerda tem pelo menos T itens, passo o mais a direita deles para o pai, que desce para complementar o filho a direita
                {
                    TNo *esq = aux->pagina[aux->nInfo-1];
                    TNo *dir = aux->pagina[aux->nInfo];

                    EmpurrarItens(dir, 0);
                    dir->item[0] = aux->item[aux->nInfo-1];
                    aux->item[aux->nInfo-1] = esq->item[esq->nInfo-1];
                    dir->pagina[0] = esq->pagina[esq->nInfo];
                    esq->nInfo--;
                    dir->nInfo++;
                }
            }
            aux = aux->pagina[aux->nInfo]; // despois de arrumar a pagina para ter pelo menos T itens, desco
        }
        else // vou entrar em alguma pagina a esquerda de um item
        {
            if (aux->pagina[i]->nInfo == T-1) // analogo ao caso anterior
            {
                if (aux->pagina[i+1]->nInfo == T-1) // direita tambem tem T-1 itens
                {
                    Unir(aux, i);
                    PuxarItens(aux, i);
                    aux->nInfo--;
                }
                else // mais a esquerda da pagina a direita sobe e item que divide as paginas desce para pagina esquerda
                {
                    TNo *esq = aux->pagina[i];
                    TNo *dir = aux->pagina[i+1];

                    esq->item[esq->nInfo] = aux->item[i];
                    aux->item[i] = dir->item[0];
                    esq->pagina[esq->nInfo+1] = dir->pagina[0];
                    dir->pagina[0] = dir->pagina[1];
                    PuxarItens(dir, 0);
                    dir->nInfo--;
                    esq->nInfo++;
                }
            }
            aux = aux->pagina[i];
        }
    }
    printf("-Item não encontrado.\n");
}

TItem Remover(TNo *no, int indice) // acionada ao encontrar o no a ser removido
{
    TItem it = no->item[indice];
    if (no->pagina[0] == NULL) // se folha, remove (lembrando que para ter chegado nessa funcao, todos os nos no caminho ate a raiz tem pelo menos T itens)
    {
        PuxarItens(no, indice);
        no->nInfo--;
    }
    else // se nao-folha, vou ter que emprestar de um filho, que pode ter que emprestar de um filho dele, etc
    {
        if (no->pagina[indice]->nInfo > T-1) // posso mandar um item do filho da esquerda subir
        {
            no->item[indice] = Remover(no->pagina[indice], no->pagina[indice]->nInfo-1);
        }
        else if (no->pagina[indice+1]->nInfo > T-1) // posso mandar um item do filho da direita subir
        {
            no->item[indice] = Remover(no->pagina[indice+1], 0);
        }
        else // ninguem pode subir ainda -> item desce unindo os dois e o problema de remove-lo eh passado para onde ele foi
        {
            Unir(no, indice);
            PuxarItens(no, indice);
            no->nInfo--;

            TNo *proximo = no->pagina[indice];
            int med = (proximo->nInfo-1)/2;
            return Remover(proximo, med);
        }
    }
    return it;
}

void EmpurrarItens(TNo *n, int indice) // move todos os itens a partir de indice para a direita
{
    int i;
    for (i = n->nInfo-1; i >= indice; i--)
    {
        n->item[i+1] = n->item[i];
        n->pagina[i+2] = n->pagina[i+1];
    }
}

void PuxarItens(TNo *n, int indice) // move todos os itens a partir de indice para a esquerda
{
    int i;
    for (i = indice; i < n->nInfo-1; i++)
    {
        n->item[i] = n->item[i+1];
        n->pagina[i+1] = n->pagina[i+2];
    }
}

TItem Quebrar(TNo *n, TNo *dir) // divide o no n em 2, altera o no direita que foi alocado antes de entrar na funcao e retorna o item que vai subir para o no pai
{
    int med = (n->nInfo-1)/2;
    TItem meio = n->item[med];

    dir->nInfo = T-1;
    dir->pagina[med] = n->pagina[n->nInfo];

    int i;
    for (i = 0; i < med; i++) // copia os itens do final de n para dir
    {
        dir->item[i] = n->item[i + med+1];
        dir->pagina[i] = n->pagina[i + med+1];
    }
    n->nInfo = T-1;

    return meio;
}

void Unir(TNo *n, int indice) // faz o inverso de Quebrar(), manda um item para unir as duas paginas adjacentes a ele (indice e indice+1)
{
    TItem meio = n->item[indice];
    TNo *esq = n->pagina[indice];
    TNo *dir = n->pagina[indice+1];

    esq->item[esq->nInfo] = meio;
    esq->pagina[esq->nInfo+1] = dir->pagina[0];
    int i;
    for (i = esq->nInfo+1; i < 2*T-1; i++) // aumenta o no, unindo as duas paginas e suas subpaginas
    {
        esq->item[i] = dir->item[i-(esq->nInfo+1)];
        esq->pagina[i+1] = dir->pagina[i-(esq->nInfo)];
    }

    esq->nInfo = 2*T-1;
}

void PrintTree(TNo *no, int h) // imprime a arvore organizadamente
{
    if (no != NULL)
    {
        int i;
        for (i = 0; i < 2*h; i++) printf(" ");
        printf("[");
        for (i = 0; i < no->nInfo; i++)
        {
            printf("%s", no->item[i].id);
            if (i != no->nInfo-1) printf(",");
        }
        printf("]");
        if (no->pagina[0] != NULL)
        {
            printf("\n");
            for (i = 0; i <= no->nInfo; i++)
            {
                PrintTree(no->pagina[i], h+1);
            }
            printf("\n");
        }
    }
}

int StartsWith(const char *a, const char *b) // verifica se a comeca com b (usado em Pesquisar()
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}
