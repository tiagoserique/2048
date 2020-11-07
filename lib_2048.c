#include "lib_2048.h"


int geraNumero(int minimo, int maximo){
    
    return ( rand() % ( maximo - minimo ) ) + minimo;
}


void pintaQuadrado(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int cores[CORES],
 int indice, int linha, int coluna, int peca){

    int par = indice + 1;

    int cor = cores[valorECor[1][indice]];

    init_color(cor, valorECor[2][indice], valorECor[3][indice], valorECor[4][indice]);
    init_pair(par, COLOR_BLACK, cor);
   
    attron(COLOR_PAIR(par)); 
    mvprintw(linha, coluna, "%d", peca);
    attroff(COLOR_PAIR(par));

    return;
}


int adicionaValorECor(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR],
int *limiteValorECor, int peca){

    int indice;

    valorECor[0][*limiteValorECor] = peca;
    valorECor[1][*limiteValorECor] = ( *limiteValorECor % CORES );
    
    for (indice = 2; indice <= 4; indice++)
        valorECor[indice][*limiteValorECor] = geraNumero(MIN_RGB, MAX_RGB + 1);

    (*limiteValorECor)++;

    return *limiteValorECor - 1;
}


int busca(int vetor[COL_VAL_E_COR], int peca, int inicio, int fim){
    
    int meio;

    if ( inicio <= fim ){

        meio = ( inicio + fim )/2;

        if ( peca == vetor[meio] )
            return meio;
        if ( peca < vetor[meio] )
            return busca(vetor, peca, inicio, meio - 1);
        else if ( peca > vetor[meio] )
            return busca(vetor, peca, meio + 1, fim);
    }
    
    return -1;       
}


void colore(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, 
int cores[CORES], int peca, int linha, int coluna){

    int achou = busca(valorECor[0], peca , 0, *limiteValorECor);
    
    if ( achou == -1 )
        achou = adicionaValorECor(valorECor, limiteValorECor, peca);
    
    pintaQuadrado(valorECor, cores, achou, linha, coluna, peca);

    return;
}


void adicionaNovaPeca(int grade[DIMENSAO][DIMENSAO]){

    int linha = geraNumero(1, DIMENSAO - 1);
    int coluna = geraNumero(1, DIMENSAO - 1);
    
    if ( grade[linha][coluna] == 0 )
        grade[linha][coluna] = geraNumero(1, 3) * 2;
    else
        adicionaNovaPeca(grade);
    
    return;
}


void rodaAGrade(int grade[DIMENSAO][DIMENSAO]){

    int tamanho = DIMENSAO;
    int linha, coluna, temp;
    for (linha = 1; linha < tamanho/2; linha++){
        for (coluna = linha; coluna < tamanho - 1 - linha; coluna++){
            temp = grade[linha][coluna]; 
            grade[linha][coluna] = grade[coluna][tamanho - 1 - linha];
            grade[coluna][tamanho - 1 - linha] = grade[tamanho - 1 - linha][tamanho - 1 - coluna];
            grade[tamanho - 1 - linha][tamanho - 1 - coluna] = grade[tamanho - 1 - coluna][linha];
            grade[tamanho - 1 - coluna][linha] = temp;
        }
    }

    return;
}


int ateOndeMover(int linha[DIMENSAO], int coluna, int limite){

    int posicaoPeca;

    if ( coluna > 1 ){
        for(posicaoPeca = coluna - 1; ; posicaoPeca--){
            if ( linha[posicaoPeca] != 0 ) {

                /* checa se deve ser somado ou movido */
                if ( linha[posicaoPeca] != linha[coluna] )
                    return posicaoPeca + 1;
                
                return posicaoPeca;
            } 
            else /* se for 0, verifica se eh a ultima posicao com 0 */
                if ( posicaoPeca == limite )
                    return posicaoPeca;
        }
    }
    else
        return coluna;
}


int movePecas(int linha[DIMENSAO], int *score){

    int moveu = 0;
    int limite = 1;
    int coluna, ondeMover;

    for (coluna = 1; coluna < DIMENSAO - 1; coluna++){
        if ( linha[coluna] != 0 ){
            
            ondeMover = ateOndeMover(linha, coluna, limite);
            if ( ondeMover != coluna ){

                if ( linha[ondeMover] == 0 )
                    linha[ondeMover] = linha[coluna];
                else {
                    /* faz a "soma" e adiciona pontuacao ao score */
                    *score += linha[ondeMover] * 2;
                    linha[ondeMover] *= 2;
                    limite = ondeMover + 1;
                }

                linha[coluna] = 0;
                moveu = 1;
            }
        }
    }

    return moveu;
}


int movePraEsquerda(int grade[DIMENSAO][DIMENSAO], int *score){

    int linha;
    int moveu = 0;

    for (linha = 1; linha < DIMENSAO - 1; linha++)
        if ( movePecas(grade[linha], score) )
            moveu = 1;

    return moveu;
}


int movePraDireita(int grade[DIMENSAO][DIMENSAO], int *score){

    int moveu;

    rodaAGrade(grade);
    rodaAGrade(grade);
    moveu = movePraEsquerda(grade, score);
    rodaAGrade(grade);
    rodaAGrade(grade);

    return moveu;
}


int movePraCima(int grade[DIMENSAO][DIMENSAO], int *score){

    int moveu;

    rodaAGrade(grade);
    moveu = movePraEsquerda(grade, score);
    rodaAGrade(grade);
    rodaAGrade(grade);
    rodaAGrade(grade);

    return moveu;
}


int movePraBaixo(int grade[DIMENSAO][DIMENSAO], int *score){

    int moveu;

    rodaAGrade(grade);
    rodaAGrade(grade);
    rodaAGrade(grade);
    moveu = movePraEsquerda(grade, score);
    rodaAGrade(grade);

    return moveu;
}


void mostraLayout(int *score){

    int coluna, linha, quantidadeColuna, quantidadeLinha;

    quantidadeLinha = DIMENSAO + 2;
    quantidadeColuna = DIMENSAO * 4;

    /* bordas */
    for (coluna = 0; coluna <= quantidadeColuna; coluna++){
        for (linha = 0; linha <= quantidadeLinha; linha++){
            if ( coluna == 0 ){
                if ( linha == 0 ){
                    move(linha, coluna); addch(ACS_ULCORNER);
                }
                else if ( linha == quantidadeLinha ){
                    move(linha, coluna); addch(ACS_LLCORNER);
                }
                else if ( linha % 2 == 0 ){
                    move(linha, coluna); addch(ACS_LTEE);
                }
                else {
                    move(linha, coluna); vline(ACS_VLINE, 1);
                }
            }
            else if ( coluna == quantidadeColuna ){
                if ( linha == 0 ){
                    move(linha, coluna); addch(ACS_URCORNER);
                }
                else if ( linha == quantidadeLinha ){
                    move(linha, coluna); addch(ACS_LRCORNER);
                }
                else if ( linha % 2 == 0 ){
                    move(linha, coluna); addch(ACS_RTEE);
                }
                else {
                    move(linha, coluna); vline(ACS_VLINE, 1);
                }
            }
            else if ( coluna % 6 == 0 ){
                if ( linha == 0 ){
                    move(linha, coluna); addch(ACS_TTEE);
                }
                else if ( linha == quantidadeLinha ){
                    move(linha, coluna); addch(ACS_BTEE);
                }
                else if ( linha % 2 == 0 ){
                    move(linha, coluna); addch(ACS_PLUS);
                }
                else {
                    move(linha, coluna); vline(ACS_VLINE, 1);
                }
            }
            else if ( linha % 2 != 1 ) {
                move(linha, coluna); hline(ACS_HLINE, 1);
            }
        }
    }

    /* score */
    move(2, 30); addch(ACS_ULCORNER);
    move(2, 31); hline(ACS_HLINE, 14);
    move(3, 30); vline(ACS_VLINE, 1);
    move(2, 45); addch(ACS_URCORNER);

    move(4, 30); addch(ACS_LLCORNER);
    move(4, 31); hline(ACS_HLINE, 14);
    move(3, 45); vline(ACS_VLINE, 1);
    move(4, 45); addch(ACS_LRCORNER);

    mvprintw(1, 30, "2048");
    mvprintw(3, 31, "Score: %d", *score);
    mvprintw(5, 30, "Use as setas para mover");
    mvprintw(7, 30, "Feito por Tiago Serique");

    return;
}


void mostraGrade(int grade[DIMENSAO][DIMENSAO],
int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, int cores[CORES],
int *score){ 

    clear();

    int linha, coluna, espacoDeColunas, espacoDeLinhas, peca;
    espacoDeLinhas = 0;

    for (linha = 1; linha < DIMENSAO - 1; linha++){      

        espacoDeColunas = 0;
        for (coluna = 1; coluna < DIMENSAO - 1; coluna++){
            
            peca = grade[linha][coluna];
            if ( peca == 0 )
                mvprintw(linha + espacoDeLinhas, coluna + espacoDeColunas, "%s",
                 ESPACO);
            else
                colore(valorECor, limiteValorECor, cores, peca,
                 linha + espacoDeLinhas, coluna + espacoDeColunas);

            espacoDeColunas += 5;
        }
        espacoDeLinhas += 1;
    }

    mostraLayout(score);

    refresh();

    return;
}


int daPraSomar(int grade[DIMENSAO][DIMENSAO]){

    int linha, coluna;
    for (linha = 1; linha < DIMENSAO - 1; linha++){
        for (coluna = 1; coluna < DIMENSAO - 1; coluna++){
            if ( grade[linha][coluna - 1] == grade[linha][coluna]  /* esquerda */ 
            || grade[linha][coluna + 1] == grade[linha][coluna]    /* direita */
            || grade[linha - 1][coluna] == grade[linha][coluna]    /* cima */
            || grade[linha + 1][coluna] == grade[linha][coluna] )  /* baixo */
                return 1;
        }
    }

    return 0;
}


int temZero(int grade[DIMENSAO][DIMENSAO]){
    
    int linha, coluna;
    for (linha = 1; linha < DIMENSAO - 1; linha++)
        for (coluna = 1; coluna < DIMENSAO - 1; coluna++)
            if (grade[linha][coluna] == 0)
                return 1;
    
    return 0;
}


int acabouOJogo(int grade[DIMENSAO][DIMENSAO]){

    if ( temZero(grade) || daPraSomar(grade) )
        return 0;

    return 1;
}


void criaGrade(int grade[DIMENSAO][DIMENSAO]){

    /* peças */
    int linha, coluna;
    for (linha = 1; linha < DIMENSAO - 1; linha++)
        for (coluna = 0; coluna < DIMENSAO; coluna++)
            grade[linha][coluna] = 0;

    /* bordas */
    for (linha = 0; linha == 0; linha++)
        for (coluna = 0; coluna < DIMENSAO; coluna++)
            grade[linha][coluna] = -1;

    for (linha = DIMENSAO - 1; linha == DIMENSAO - 1; linha++)
        for (coluna = 0; coluna < DIMENSAO; coluna++)
            grade[linha][coluna] = -1;

    for (coluna = 0; coluna == 0; coluna++)
        for (linha = 0; linha < DIMENSAO; linha++)
            grade[linha][coluna] = -1;

    for (coluna = DIMENSAO - 1; coluna == DIMENSAO - 1; coluna++)
        for (linha = 0; linha < DIMENSAO; linha++)
            grade[linha][coluna] = -1;

    return;
}


void telaFimDeJogo(int grade[DIMENSAO][DIMENSAO],
int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, int cores[CORES], 
int *score){ 

    mostraGrade(grade, valorECor, limiteValorECor, cores, score);

    mvprintw(9, 9, "GAME OVER");
    mvprintw(11, 4, "digite q para sair");

    return;
}


void inicializaValorECor(int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR],
 int *limiteValorECor){

    adicionaValorECor(valorECor, limiteValorECor, 2);
    return;
}


void inicializaJogo(int grade[DIMENSAO][DIMENSAO],
int valorECor[LIN_VAL_E_COR][COL_VAL_E_COR], int *limiteValorECor, int cores[CORES], 
int *score){ 

    *limiteValorECor = 0;
    *score = 0;

    srand(time(NULL));

    initscr();

    start_color();
    init_color(COLOR_BLACK, 0, 0, 0);
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    criaGrade(grade);
    inicializaValorECor(valorECor, limiteValorECor);
    adicionaNovaPeca(grade);
    adicionaNovaPeca(grade);
    mostraGrade(grade, valorECor, limiteValorECor, cores, score);
    
    return;
}
