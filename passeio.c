#include <stdio.h>
#define M 8

int movimento_valido(int board[M][M], int pos_x, int pos_y);  
int movimentos_garantidos(int board[M][M], int mover[2][M],int atual_x, int atual_y);
void norma_euclideana_alt(int possiveis[2][M], int quantidade_possivel);
int todas_casas_preenchidas(int board[M][M]);
void print_solution(int board[M][M], unsigned long int casas, unsigned long int back);
int knights_tour(int board[M][M], int casa_atual,int pos_x, int pos_y, unsigned long int  *casas, unsigned long int *back);
void passeio(int x, int y); 

//esses vetores combinados serao os movimentos do cavalo
int xmove[M]={-1,-2,-2,-1,1,2,2,1};
int ymove[M]={2,1,-1,-2,-2,-1,1,2};    

int movimento_valido(int board[M][M], int pos_x, int pos_y)
{   
    //se a posicao atual esta dentro do tabuleiro e nao foi visitada ainda (ou seja, seu valor é -1)
    if(pos_x < 8 && pos_x >=0 && pos_y < 8 && pos_y >=0 && board[pos_x][pos_y]==-1)
        return 1;

    return 0;
}
 
int movimentos_garantidos(int board[M][M], int mover[2][M],int atual_x, int atual_y)
{
  int aux =0;
  
  for(int i=0;i<M;i++){

    if(movimento_valido(board, atual_x+ xmove[i], atual_y+ ymove[i])){
      mover[0][aux]=atual_x+xmove[i];
      mover[1][aux]=atual_y+ymove[i];
      aux++;
    }

  }
  return aux;
}
    
void norma_euclideana_alt(int possiveis[2][M], int quantidade_possivel)
{
    int inidice_max;
    //salvo a primeira jogada como a melhor jogada (ou seja, mais longe do centro)
    double distancia = (possiveis[0][0] - 4)*(possiveis[0][0] - 4) + (possiveis[1][0]- 4)*(possiveis[1][0] - 4);
    //selection sort para os movimentos de maior norma ficarem no inicio
    for (int i = 0; i < quantidade_possivel-1; i++)
    {
        inidice_max = i;
        for (int j = i+1; j < quantidade_possivel; j++)
        {
          double distancia_atual = (possiveis[0][j] - 4)*(possiveis[0][j] - 4) + (possiveis[1][j] - 4)*(possiveis[1][j] - 4);
          //se eu acho uma norma maior do que a anterior, eu guardo seu indice 
          if (distancia_atual > distancia)
          {
            inidice_max = j;
            distancia = distancia_atual;
          }
        }
        //uso o indice da maior e faço a troca
        int temp = possiveis[0][inidice_max];
        int temp2 = possiveis[1][inidice_max];
        possiveis[0][inidice_max] = possiveis[0][i];
        possiveis[1][inidice_max] = possiveis[1][i];

        possiveis[0][i] = temp;
        possiveis[1][i] = temp2;
    }
}
  
int todas_casas_preenchidas(int board[M][M]){
    //garantir que o tabuleiro esta completo
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board[i][j] == -1)
                return 0;
        }
    }
    return 1;
}

void print_solution(int board[M][M], unsigned long int casas, unsigned long int back)
{
    //imprimir o tabuleiro ao final do passeio
    FILE *arq;
    arq = fopen("saida.txt","a");
    
    for(int m=0;m<8;m++){
        for(int n=0;n<8;n++){
            fprintf(arq,"%.2d ",board[m][n]);
        }
        fprintf(arq,"\n");
    }
    fprintf(arq,"%lu %lu\n", casas, back);

    fclose(arq);

} 

int knights_tour(int board[M][M], int casa_atual,int pos_x, int pos_y, unsigned long int  *casas, unsigned long int *back)
{
    //faço a atribuição pois as casas alcançadas com movimentos validos sao garantidamente validas
    board[pos_x][pos_y] = casa_atual;
    *(casas) += 1;

    //se tabuleiro completo
    if(casa_atual==64 && todas_casas_preenchidas(board))
        return 1;

    int movimentos_possiveis[2][M]; 
    //armazeno os movimentos validos por ordem de MAIOR norma
    int cont = movimentos_garantidos(board, movimentos_possiveis, pos_x, pos_y);
    norma_euclideana_alt(movimentos_possiveis, cont);
    
      for(int try_move=0; try_move < cont; try_move++){
            //recursividade
            if(knights_tour(board, casa_atual+1, movimentos_possiveis[0][try_move], movimentos_possiveis[1][try_move], casas, back))
                return 1;
            else
                *(back) += 1;

        }
    //desmarco as casas que nao tem movimentos possiveis e que nao foram achados tour a partir delas
    //backtracking 
    board[pos_x][pos_y]=-1;
return 0;
}

void passeio(int x, int y) 
{ 
    //posicao pos_inicial= {x,y};
    unsigned long int casas=0, back=0;
    int casa_atual=1;

    int board[8][8]={   
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1}, 
    {-1, -1, -1, -1, -1, -1, -1, -1}
    };
    //passo o endereco de memoria de casas e back 
        if(!knights_tour(board,casa_atual, x-1, y-1, &casas, &back)){
            FILE *arq;
            arq = fopen("output.txt","a");
            //que eu saiba, não existe casa sem soluçao para um board 8x8
            fprintf(arq,"A solução não existe");
            fclose(arq);
        }
        else{
            print_solution(board, casas, back);
        }
}

int main(){
    int x=1, y=1;
    passeio(x,y);
    return 0;
}