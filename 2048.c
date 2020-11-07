#include "lib_2048.h"


int main(){

    int grade[DIMENSAO][DIMENSAO], valorECor[LIN_VAL_E_COR][COL_VAL_E_COR];
    
    /* formato da matriz valorECor (faz a relacao: valor da peça e cor)
        [2, 4, 8, ...]     valor na grade
        [0, 1, 2, ...]     valor do indice do vetor cor
        [r, r, r, ...]     valor do r
        [g, g, g, ...]     valor do g
        [b, b, b, ...]     valor do b   
    */

    int cores[CORES] = {
        COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN
    };

    int seta, moveu, limiteValorECor, score;

    inicializaJogo(grade, valorECor, &limiteValorECor, cores, &score);

    do {
        seta = getch();

        if ( seta == KEY_RIGHT )
            moveu = movePraDireita(grade, &score);
        else if ( seta == KEY_DOWN )
            moveu = movePraBaixo(grade, &score);
        else if ( seta == KEY_UP )
            moveu = movePraCima(grade, &score);
        else if ( seta == KEY_LEFT )
            moveu = movePraEsquerda(grade, &score);
        
        if ( moveu ){
            mostraGrade(grade, valorECor, &limiteValorECor, cores, &score);
            adicionaNovaPeca(grade);
            mostraGrade(grade, valorECor, &limiteValorECor, cores, &score);
        }
    } while ( !acabouOJogo(grade) );

    do {
        telaFimDeJogo(grade, valorECor, &limiteValorECor, cores, &score); 
    } while ( getch() != 'q' );
    
    endwin();

    return 0;
}
