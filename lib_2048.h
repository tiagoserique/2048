#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>


#define DIMENSAO 6 /* CONTANDO AS BORDAS */
#define DELAY 1

#define ESPACO " "

#define MIN_RGB 300
#define MAX_RGB 800
#define CORES 6
#define COL_VAL_E_COR 100
#define LIN_VAL_E_COR 5


/* gera um numero aleatorio no intervalo [minimo,maximo) */
int geraNumero(int minimo, int maximo);


/* colore o quadrado de acordo com a peça dentro dele */
void pintaQuadrado(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int cores[CORES],
 int indice, int linha, int coluna, int peca);


/* adiciona valores a matriz que relaciona a peça com a cor */
int adicionaValorECor(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR],
int *limiteValorECor, int peca);


/* busca binaria para encontrar a posicao da peça especificada */
int busca(int vetor[COL_VAL_E_COR], int peca, int inicio, int fim);


/* controla o colorimento da peca */
void colore(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, 
int cores[CORES], int peca, int linha, int coluna);


/* coloca 2 ou 4 em uma posicao aleatoria, adicionando uma nova peça */
void adicionaNovaPeca(int grade[DIMENSAO][DIMENSAO]);


/* faz o giro da matriz no sentido anti-horario */
void rodaAGrade(int grade[DIMENSAO][DIMENSAO]);


/* determina ate onde a peça deve ser deslocada */
int ateOndeMover(int linha[DIMENSAO], int coluna, int limite);


/* faz a movimentacao da peça */
int movePecas(int linha[DIMENSAO], int *score);


/* movimenta para a esquerda */
int movePraEsquerda(int grade[DIMENSAO][DIMENSAO], int *score);


/* movimenta para a direita */
int movePraDireita(int grade[DIMENSAO][DIMENSAO], int *score);


/* movimenta para cima */
int movePraCima(int grade[DIMENSAO][DIMENSAO], int *score);


/* movimenta para baixo */
int movePraBaixo(int grade[DIMENSAO][DIMENSAO], int *score);


/* mostra as bordas da grade e o score */
void mostraLayout(int *score);


/* mostra os valores das peças e chama a funcao para o layout*/
void mostraGrade(int grade[DIMENSAO][DIMENSAO],
int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, int cores[CORES],
int *score);


/* verifica se eh possivel fazer soma */
int daPraSomar(int grade[DIMENSAO][DIMENSAO]);


/* verifica se ha zeros na grade */
int temZero(int grade[DIMENSAO][DIMENSAO]);


/* verifica se o jogo acabou */
int acabouOJogo(int grade[DIMENSAO][DIMENSAO]);


/* cria a grade de peças e as bordas */
void criaGrade(int grade[DIMENSAO][DIMENSAO]);


/* mostra a dela de game over */
void telaFimDeJogo(int grade[DIMENSAO][DIMENSAO],
int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, int cores[CORES], 
int *score);


/* incializa a matriz de relacao entre valor da peça e cor */
void inicializaValorECor(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR],
 int *limiteValorECor);


/* faz as configuracoes inciais do jogo */
void inicializaJogo(int grade[DIMENSAO][DIMENSAO],
int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, int cores[CORES], 
int *score);

