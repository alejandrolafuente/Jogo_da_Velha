#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

char m[3][3];

typedef struct{
    int lin;
    int col;
    char valor;
} posicao_valor;

typedef struct{
    posicao_valor cel1;
    posicao_valor cel2;
    posicao_valor cel3;
} trio_win;

trio_win vetor[8];

FILE *arq_texto; 

typedef struct{
    int partida;
    char JogVelha[3][3];
    char resultado;
} Partida;

//Partida vetorPartidas[10]; // ate 10 registros

FILE *arq_bin;

//*******************************************************************************
 
void inicializa_velha(){ // inicia matriz m 3 x 3 (global) com caractere  
    int i, j;            // espaço em cada "célula"
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            m[i][j] = ' '; 
}

void imprime_matriz(){
    int i, j;
    system("cls");
    printf("\n");
    for(i=0; i<3; i++){
        for(j=0; j<3; j++)
            printf("%c ",m[i][j]);
        printf("\n"); 
    }
    printf("\n");
}

int menu(){
    int op;
    printf("Escolha seu oponente, 1 para jogar contra pc, 2 contra usuario:\n");
    scanf("%d",&op);
    return op;
}

void escolha_simb(char *jog1, char *jog2){
    printf("Escolha seu caractere, X ou O (letra o)\n");
    scanf("%*c%c", jog1); // VALIDAR!!
    *jog1 = toupper(*jog1);
    if (*jog1 == 'X')
        *jog2 = 'O';
    else
        *jog2 = 'X';
}

void inicializa_estrutura_de_dados(){
    int i;

    for (i=0; i<8; i++){ // inicializa todos as "células" com espaço
        vetor[i].cel1.valor = ' ';
        vetor[i].cel2.valor = ' ';
        vetor[i].cel3.valor = ' ';
    }

    for (i=0; i<3; i++){ // coloca os endereços dos 3 primeiros elementos do vetor  
        vetor[i].cel1.lin = i;
        vetor[i].cel2.lin = i;
        vetor[i].cel3.lin = i;
        
        vetor[i].cel1.col = 0;
        vetor[i].cel2.col = 1;
        vetor[i].cel3.col = 2;
    }

    for (i=0; i<3; i++){ // coloca os endereços de vet[3] a vet[5]  
        vetor[i+3].cel1.col = i;
        vetor[i+3].cel2.col = i;
        vetor[i+3].cel3.col = i;

        vetor[i+3].cel1.lin = 0;
        vetor[i+3].cel2.lin = 1;
        vetor[i+3].cel3.lin = 2;
    }

    i = 0; // estes comandos colocam os valores de vet[6]
    vetor[6].cel1.lin = i;
    vetor[6].cel1.col = i;
    vetor[6].cel2.lin = i+1;
    vetor[6].cel2.col = i+1;
    vetor[6].cel3.lin = i+2;
    vetor[6].cel3.col = i+2;

    i = 0; // estes comandos colocam os valores de vet[7]
    vetor[7].cel1.lin = 0;
    vetor[7].cel1.col = 2;
    vetor[7].cel2.lin = 1;
    vetor[7].cel2.col = 1;
    vetor[7].cel3.lin = 2;
    vetor[7].cel3.col = 0;
}

int jogada_usuario(int lin, int col, char jog){ // aqui preenchemos a matriz com 'X' ou 'O'
    if ( (lin >= 0) && (lin <= 2) && (col >= 0) && (col <=2) && (m[lin][col] == ' ') ){
        m[lin][col] = jog;
        return 0; // retorna zero para jogada válida
    }

    if ( ( (lin < 0) || (lin > 2) ) || ( (col < 0) || (col > 2) ) ){
        return 1; // retorna 1 para posição inválida
    }

    if ( (lin >= 0) && (lin <= 2) && (col >= 0) && (col <=2) && (m[lin][col] != ' ') ){
        return 2; // retorna 2 para posição já preenchida 
    }
}

// percorre nossa estrutura de dados e preenche em cada struct que os valores passa-
// dos na chamada forem iguais. trocar esta abordagem por "instancia" via ponteiros
void preenche_vetor_structs(int l, int c, char jog){
    int i;

    for (i=0 ; i <8 ; i++){

        if ( (vetor[i].cel1.lin == l) && (vetor[i].cel1.col == c))
            vetor[i].cel1.valor = jog; 

        if ( (vetor[i].cel2.lin == l) && (vetor[i].cel2.col == c))
            vetor[i].cel2.valor = jog;

        if ( (vetor[i].cel3.lin == l) && (vetor[i].cel3.col == c))
            vetor[i].cel3.valor = jog;
    }
}
   
int verifica_ganhador(char jog){
    int i = 0, ganhou = 0;
    
    while (!ganhou && (i < 8)){
        if ( (vetor[i].cel1.valor == jog) && (vetor[i].cel2.valor == jog) && (vetor[i].cel3.valor == jog))
            ganhou = 1;
        i = i + 1;
    }

    if (ganhou)
        return 1;
    else
        return 0;
}

int verifica_se_matriz_cheia(){ // percorre m e se esta cheia retorna 1
    int i=0, j=0, achou_vazio = 0;

    while (!achou_vazio && i <3) {
        j = 0;
        while (!achou_vazio && j <3) {
            if(m[i][j] == ' ')
                achou_vazio = 1;
            else
                j++;
        }
        i++;
    }
    
    if (achou_vazio)
        return 0;
    else
        return 1;
}

void jogada_basica(char jog){ // a jogada basica eh na matriz
    int i=0, j=0, alocou=0; //percorre a matriz e preenche o primeiro espaço livre
    
    while ((i < 3) && (!alocou)){
        j = 0;
        while ((j < 3) && (!alocou)){
            if (m[i][j] == ' '){
                m[i][j] = jog;
                alocou = 1;
            }
            else
                j = j + 1;
        } 
        if (!alocou)
            i = i + 1; 
    }
    preenche_vetor_structs(i,j,jog);
}

int jogada_intermediaria(char jog){ // eh no vetor.
// 1. Tenta bloquear caso humano prestes a ganhar
// 2. Se não bloquear, preenche o primeiro vazio

// percorrer o vetor com for ou while
// primeiro verifica qual o primeiro registro que  tem dois 'X' e um vazio
// Se encontrar um registro com dois 'x' preenche o vazio com 'O'
// guarda a posicao deste registro e termina o laço. Já temos nosso l e c. Assim,
// quem terminara de instanciar no VETOR para nos eh preenche_vetor_structs
// ou seja, o pc esta tentando me bloquear
// se nao achar dois 'X' em nenhuma posiçao, preenche o primeiro vazio na matriz!!
    int i=0, bloqueou = 0, conta_diferentes; // esta ultima variavel eh p bloquear
    int auxlin = -1, auxcol;
// em cada elemento do vetor,conta quantos caracteres do oponente têm. se houver dois
// destes e um vazio, innstancia o vazio. guardar lin e col do vazio 
// se houver caracteres iguais ao meu, desconsideramos para esta jogada
    while ((i < 8) && !bloqueou){

        conta_diferentes = 0;
        if ((vetor[i].cel1.valor != jog) && (vetor[i].cel1.valor != ' '))//
            conta_diferentes = conta_diferentes + 1;
        if (vetor[i].cel1.valor == ' '){
            auxlin = vetor[i].cel1.lin;
            auxcol = vetor[i].cel1.col;
        } // verificamos apenas a primeira celula

        if ((vetor[i].cel2.valor != jog) && (vetor[i].cel2.valor != ' '))//
            conta_diferentes = conta_diferentes + 1;
        if (vetor[i].cel2.valor == ' '){
            auxlin = vetor[i].cel2.lin;
            auxcol = vetor[i].cel2.col;
        }

        if ((vetor[i].cel3.valor != jog) && (vetor[i].cel3.valor != ' '))//
            conta_diferentes = conta_diferentes + 1;
        if (vetor[i].cel3.valor == ' '){
            auxlin = vetor[i].cel3.lin;
            auxcol = vetor[i].cel3.col;
        }

        if ( (conta_diferentes == 2) && (auxlin != -1) ){ // bloqueia o primeiro risco de perder que achar
            bloqueou = 1;
            m[auxlin][auxcol] = jog; // instancia matriz
            preenche_vetor_structs(auxlin,auxcol,jog);
        }

        i++;
    } // while ((i < 8) && !bloqueou)

    if(bloqueou){ // se não bloqueou preenche o primeiro vazio na matriz e no vetor
        return 1;
    }
    else
        return 0;
}

int joga_para_ganhar(char jog){ // eh no vetor.

    int i=0, ganhou = 0, conta_iguais; // esta ultima variavel eh p bloquear
    int auxlin = -1, auxcol;

    while ((i < 8) && !ganhou){

        conta_iguais = 0;
        if ((vetor[i].cel1.valor == jog) && (vetor[i].cel1.valor != ' '))//
            conta_iguais = conta_iguais + 1;
        if (vetor[i].cel1.valor == ' '){
            auxlin = vetor[i].cel1.lin;
            auxcol = vetor[i].cel1.col;
        } // verificamos apenas a primeira celula

        if ((vetor[i].cel2.valor == jog) && (vetor[i].cel2.valor != ' '))//
            conta_iguais = conta_iguais + 1;
        if (vetor[i].cel2.valor == ' '){
            auxlin = vetor[i].cel2.lin;
            auxcol = vetor[i].cel2.col;
        }

        if ((vetor[i].cel3.valor == jog) && (vetor[i].cel3.valor != ' '))//
            conta_iguais = conta_iguais + 1;
        if (vetor[i].cel3.valor == ' '){
            auxlin = vetor[i].cel3.lin;
            auxcol = vetor[i].cel3.col;
        }

        if ( (conta_iguais == 2) && (auxlin != -1) ){ // bloqueia o primeiro risco de perder que achar
            ganhou = 1;
            m[auxlin][auxcol] = jog; // instancia matriz
            preenche_vetor_structs(auxlin,auxcol,jog);
        }

        i++;
    } // while ((i < 8) && !bloqueou)

    if(ganhou){ // se não bloqueou preenche o primeiro vazio na matriz e no vetor
        return 1;
    }
    else
        return 0;
}

void jogada_avancada(char jog){
    // 1. Primeiro tenta ganhar
    // 2. Se nao ganhar, verifica se nao tem risco de perder caso haja bloqueia
    // 3. Se nao bloqueou, implementa jogada avançada
    int i, j, k, possibilidade_vitorias, almost_win ;
    int melhor_lin, melhor_col, max_pv=0, max_aw=0, ganhou, bloqueou;

    
    ganhou = joga_para_ganhar(jog);

    if (!ganhou)
        bloqueou = jogada_intermediaria(jog);
    printf("ganhou = %d bloqueou = %d\n",ganhou,bloqueou);
    if (!ganhou && !bloqueou){
        for (i=0; i<3 ; i++) // verifica para cada celula vazia da matriz
            for (j=0 ; j<3; j++)
                if (m[i][j] == ' '){
                    possibilidade_vitorias = 0;
                    almost_win = 0;
                    for (k=0; k<8; k++) {// percorre a estrutura de dados vetor

                        // se a celula da matriz esta presente no registro...
                        if ( ((vetor[k].cel1.lin == i) && (vetor[k].cel1.col == j)) || ((vetor[k].cel2.lin == i) && (vetor[k].cel2.col == j)) || ((vetor[k].cel3.lin == i) && (vetor[k].cel3.col == j)))
                        {
                            // para cada v[i], verifica que cel1.valor, cel2.valor e cel3.valor nao sejam caractere "inimigo"
                            if ( ((vetor[k].cel1.valor == ' ') || (vetor[k].cel1.valor == jog)) && ((vetor[k].cel2.valor == ' ') || (vetor[k].cel2.valor == jog)) && ((vetor[k].cel3.valor == ' ') || (vetor[k].cel3.valor == jog)) ) {
                                possibilidade_vitorias++ ;
                                if ( (vetor[k].cel1.valor == jog) || (vetor[k].cel2.valor == jog) || (vetor[k].cel3.valor == jog))
                                // ou seja, se já houver um elemento amigo neste v[i], se jogarmos aqui estamos a um passo de vitoria
                                    almost_win++ ;
                            }
                        }
                    }

                    if (almost_win > max_aw){
                        max_aw = almost_win ; 
                        melhor_lin = i ;
                        melhor_col = j ;
                    }

                    if (almost_win == max_aw)
                        if (possibilidade_vitorias > max_pv){
                            max_pv = possibilidade_vitorias ; 
                            melhor_lin = i ;
                            melhor_col = j ;
                        }
                } // if (m[i][j] == ' ') => verificou cada celula vazia da matriz

        m[melhor_lin][melhor_col] = jog;
        preenche_vetor_structs(melhor_lin,melhor_col,jog);
    }
}

void jogada_computador(char jog, int nivel){
    int bloqueou;
    
    if (nivel == 1){
        jogada_basica(jog);
    }

    if (nivel == 2){
        bloqueou = jogada_intermediaria(jog); //será no vetor de structs
        if (!bloqueou){
            jogada_basica(jog);
        }
    }

    if (nivel == 3){
        jogada_avancada(jog);
    }
}

void grava_texto(char jog1, char jog2){
    char nomeJogador[15], pc[11] = "Computador",texto[50];

    printf("Digite seu nome:\n");
    scanf("%s",nomeJogador);

    arq_texto = fopen("arquivoTexto.txt", "w");
    strcpy(texto,nomeJogador);
    strcat(texto,"; ");
    strncat(texto,&jog1,1);

    strcat(texto,pc);
    strcat(texto,"; ");
    strncat(texto,&jog2,1);

    int retorno = fputs(texto,arq_texto);
    if(retorno == EOF)
        printf("Erro ao gravar...\n");
    
    fclose(arq_texto);
}

int gravaBinario(char *nome, Partida reg){
    int retorno=0;
    printf("partida %d\n",reg.partida + 1);
    if(reg.partida == 0){ // abre somente quando recebe o 1o registro
        arq_bin = fopen(nome, "wb");
        if(arq_bin == NULL){
            printf("Erro na abertura\n");
            system("pause");
            exit(1);
        }
    }

    retorno = fwrite(&reg,sizeof(Partida), 1, arq_bin);
    return retorno; 
}

void imprime_partidas(int vitorias, int derrotas){
    FILE *arq;
    Partida registro;
    int i,j;

    arq = fopen("arquivoBinario.bin", "rb");
    if(arq == NULL){
        printf("Erro na abertura\n");
        system("pause");
        exit(1);
    } 


    printf("\n");
    while(1){
        fread(&registro,sizeof(registro),1,arq);
        if (feof(arq))
            break;
        printf("partida numero %d:\n",registro.partida+1);
        printf("\n");
        for(i=0; i<3; i++){
            for(j=0; j<3; j++)
                printf("%c ",registro.JogVelha[i][j]);
            printf("\n"); 
        }
        printf("\n");
        printf("resultado = %c\n",registro.resultado);
        printf("\n");
    }
    
    fclose(arq);

    printf("Humano %d X Computador %d\n",vitorias,derrotas);

    if(vitorias == derrotas){
        printf("Empate\n");
    }

    if(vitorias < derrotas){
        printf("O computador ganhou o torneio!\n");
    }

    if(vitorias > derrotas){
        printf("Voce ganhou o torneio!\n");
    }
}

Partida partidaEspecifica(char *nome, int n){
    Partida regi;

    printf("registro que procuramos: %d\n",n);
    arq_bin = fopen(nome, "rb");
    if(arq_bin == NULL){
        printf("Erro na abertura\n");
        system("pause");
        exit(1);
    }

    fseek(arq_bin, (n-1)*sizeof(Partida), SEEK_SET);
    fread(&regi,sizeof(regi),1,arq_bin);

    fclose(arq_bin);

    return regi;
}

//**************************************************************************
void main()
{   
    Partida reg,reg1; 
    int  vitorias=0, derrotas=0;
    int  oponente, jogoaberto, user_move, l, c,resultado,matriz_cheia,nivel;
    char jog1, jog2; // devem fazer parte da funcao main? nao seriam globais?
    char controla_jogos = 'S', aux;
    int  njogos = 0, i,j, confirma_gravacao,npart;
    
    while (controla_jogos == 'S'){
        inicializa_velha(); // 1. inicializa a matriz cheia de espaços 
        oponente = menu();  // 2. Escolhe oponente -> = 1 vs PC e = 2 vs humano
        escolha_simb(&jog1, &jog2); // escolher 'X' ou 'O'
        jogoaberto = 1;       // var. booleana que é true enquanto jogo não foi encerrado 
        inicializa_estrutura_de_dados();
        printf("%c\n", jog2);

        grava_texto(jog1,jog2);

        if (oponente == 1){ //PRIMEIRO RESOLVEMOS O CASO DE JOGO VS PC
            while (jogoaberto){
                printf("Voce esta jogando contra o pc, entre com a linha e coluna de sua jogada:\n");
                do{ //faz a jogada valida e preenche a matriz
                    printf("Entre com a linha\n");
                    scanf("%d", &l);
                    printf("Entre com a coluna\n");
                    scanf("%d", &c);
                    user_move = jogada_usuario(l,c,jog1); // preenche a matriz
                } while(user_move != 0); // sai do laço quando a jogada é valida
            
                preenche_vetor_structs(l,c,jog1); // faz a jogada no vetor,instancia o vetor
                imprime_matriz();
                resultado = verifica_ganhador(jog1); // 1 --> GANHOU  0 --> NAO GANHOU

                if (resultado == 1){
                    printf("Parabens vc ganhou!!!\n\n");
                    vitorias++;
                    aux = jog1;
                    jogoaberto = 0;
                }  // posso ter lotado a matriz e nao  ganhado, tem que verificar!!
                else{ //ou seja, se resultado == 0
                    matriz_cheia =  verifica_se_matriz_cheia();
                    if (matriz_cheia){
                        printf("A velha encheu e voce nao ganhou\n\n");
                        aux = 'V';
                        jogoaberto = 0;
                    }
                }
                // agora faremos a jogada do computador caso jogo aberto
                if (jogoaberto){
                    printf("Escolha o nivel da jogada do pc: 1, 2 ou 3\n");
                    scanf("%d",&nivel);
                    jogada_computador(jog2, nivel); //preenche o vetor aqui dentro!!
                    imprime_matriz();
                    resultado = verifica_ganhador(jog2); // 1 --> GANHOU  0 --> NAO GANHOU
                    
                    if (resultado == 1){
                        printf("O computador ganhou!\n\n");
                        derrotas++;
                        aux = jog2;
                        jogoaberto = 0;
                    }  // posso ter lotado a matriz e nao  ganhado, tem que verificar!!
                    else{ //ou seja, se resultado == 0
                        matriz_cheia =  verifica_se_matriz_cheia();
                        if (matriz_cheia){
                            printf("A velha encheu e voce nao ganhou\n");
                            aux = 'V';
                            jogoaberto = 0;
                        }
                    }
                } // if (jogoaberto) printf("jogo aberto = %d\n", jogoaberto);
                
            } // while (jogoaberto)
        } // if (oponente == 1)
        

        //montamos nossa struct:
        reg.partida = njogos;
        // copiamos as matrizes:
        for(i=0; i<3; i++)
            for(j=0; j<3; j++)
                 reg.JogVelha[i][j] = m[i][j] ; 

        reg.resultado = aux;

        confirma_gravacao = gravaBinario("arquivoBinario.bin",reg);
        if (confirma_gravacao){
            printf("Gravado com sucesso!\n");
        }else
            printf("erro ao gravar");

        njogos = njogos + 1;
        printf("Deseja jogar novamente? S/N...\n");
        do{
            scanf("%c",&controla_jogos);
            controla_jogos = toupper(controla_jogos);
        }while((controla_jogos!='S') && (controla_jogos!='N'));
    } // while (controla_jogos == 'S') ==> Fim das partidas
    fclose(arq_bin);
    printf("Entre com uma partida especifica:\n");
    scanf("%d",&npart);
    reg1 = partidaEspecifica("arquivoBinario.bin",npart);

    printf("partida numero %d:\n",reg1.partida+1);
    printf("\n");
    for(i=0; i<3; i++){
        for(j=0; j<3; j++)
            printf("%c ",reg1.JogVelha[i][j]);
        printf("\n"); 
    }
    printf("\n");
    printf("resultado = %c\n",reg1.resultado);
    printf("\n");

    printf("Vejamos todos os jogos:\n");
    imprime_partidas(vitorias,derrotas);
}
