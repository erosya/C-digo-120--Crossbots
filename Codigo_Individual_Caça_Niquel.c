#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
void titulo() { // Função pra gerar o titulo do programa
    
    
    char *mensagem = "SEJA BEM VINDO A";
    
    char *grafico[] = {
        "    (       )  (   (                  (     ",
        "   (   )\\ ) ( /(  )\\ ))\\ )  (       *   ))\\ )  ",
        "   )\\ (()/( )\\())(()/(()/(( )\\ (  ` )  /(()/(  ",
        " (((_) /(_)|(_)\\  /(_))(_))((_))\\  ( )(_))(_)) ",
        " )\\___(_))   ((_)(_))(_))((_)_((_)(_(_()|_))   ",
        "((/ __| _ \\ / _ \\/ __/ __|| _ ) __|_   _/ __|  ",
        " | (__|   /| (_) \\__ \\__ \\| _ \\ _|  | | \\__ \\  ",
        "  \\___|_|_\\ \\___/|___/___/|___/___| |_| |___/"
    };

    int largura = strlen(grafico[0]);
    int espaco = (largura - strlen(mensagem)) / 2;

    for (int i = 0; i < espaco; i++) {
        printf(" ");
    }
    printf("%s\n", mensagem);

    for (int i = 0; i < 8; i++) {
        printf("%s\n", grafico[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////

int menu() { //função para separar o menu de escolha de jogadas
    int jogada;

    printf("Escolha o jogo que quer jogar:\n"); 
    
    printf(" 1- Roleta \n 2- BlackJack \n 3- Caça-Níquel \n  ");
    
    scanf("%d", &jogada);
    
    return jogada;
}

////////////////////////////////////////////////////////////////////////////////

int* rolar_numeros() { //função para gerar e fazer a animação dos numeros
    #define MINIMO 1
    #define MAXIMO 6
    #define NUMERO_DE_ROLAGENS 3

    int x, y;
    getmaxyx(stdscr, y, x); //pega o tamanho da tela

    int* numeros = (int*) malloc(NUMERO_DE_ROLAGENS * sizeof(int));
    if (numeros == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(1); //aloca memoria para a geração dos numeros
    }
    
    // Loop para cada número a ser gerado
    for (int i = 0; i < NUMERO_DE_ROLAGENS; i++) {
        int rolagem = 0;

        // faz começar a rolagem dos numeros
        while (rolagem < 10) { 
            clear(); 
            
            // imprime otitulo no meio da tela
            int mensagem_x = (x / 2) - ((NUMERO_DE_ROLAGENS * 4) / 2);
            mvprintw(y / 2 - 2, mensagem_x, "Os números são:");

            // imprime os numeros no meio da tela com o ncurses
            for (int j = 0; j < NUMERO_DE_ROLAGENS; j++) {
                int numero_aleatorio = (rand() % (MAXIMO - MINIMO + 1)) + MINIMO;
                mvprintw(y / 2, mensagem_x + (j * 4), "%d", numero_aleatorio); // Ajuste a posição conforme necessário
            }
            
            refresh();
            usleep(100000); 
            rolagem++;
        }

        // gera os numeros do caça niquel
        numeros[i] = (rand() % (MAXIMO - MINIMO + 1)) + MINIMO;
    }

    
    clear(); 
    int mensagem_x = (x / 2) - ((NUMERO_DE_ROLAGENS * 4) / 2);
    mvprintw(y / 2 - 2, mensagem_x, "Os números são:");

    //imprime os numeros no meio da tela
    for (int i = 0; i < NUMERO_DE_ROLAGENS; i++) {
        mvprintw(y / 2, mensagem_x + (i * 4), "%d", numeros[i]); 
        //arruma a posição dos numeros
    }
    
    refresh(); 
    usleep(1000000); 

    mvprintw(y / 2 + 2, (x / 2) - 10, "Aperte enter para sair");
    refresh();
    getch(); 

    return numeros;
}

////////////////////////////////////////////////////////////////////////////////

int verifica_numeros(int *numeros) { //ve se o jogador ganhou a aposta
    if (numeros[0] == numeros[1] && numeros[1] == numeros[2]) {
        if (numeros[0] <= 3) {
            printf("Voce ganhou a aposta! Ganho de 5x\n");
            return 1;
        } 
        
        else {
            printf("Voce ganhou a aposta! Ganho de 10x\n");
            return 2;
        }
    } 
    
    else {
        printf("Voce perdeu a aposta\n");
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////

void gerenciar_carteira_niquel(float* carteira, float aposta, int resultado) {
    //função para adicionar ou tirr valores da carteira
    if (resultado == 1) {
        *carteira += aposta * 5;  
        printf("Seu saldo atual é: R$ %.2f\n", *carteira);
    } 
    
    else if (resultado == 2) {
        *carteira += aposta * 10;  
        printf("Seu saldo atual é: R$ %.2f\n", *carteira);
    } 
    
    else {
        *carteira -= aposta;  
        printf("Você perdeu a aposta! Seu saldo atual é: R$ %.2f\n", *carteira);
    }

    
    if (*carteira < 0) {
        printf("Seu saldo está negativo!\n");
    }
}

////////////////////////////////////////////////////////////////////////////////

void caca_niquel(float* carteira) { //função principal do caça niquel
    float aposta;

    //recebe e verifica a se a aposta é valida
    do {
        printf("Digite o valor da sua aposta: R$ ");
            scanf("%f", &aposta);
        
        if (aposta > *carteira) {
            printf("Aposta inválida! Você não pode apostar mais do que o saldo disponível.\n");
            printf("Saldo disponível: R$ %.2f\n", *carteira);
        } 
        
        else if (aposta <= 0) {
            printf("Aposta inválida! O valor da aposta deve ser maior que zero.\n");
        }
    } while (aposta > *carteira || aposta <= 0);

    initscr(); // começa a biblioteca ncurses
    curs_set(0); 

    srand(time(NULL)); 

    int* numeros = rolar_numeros();  //armazena os numeros gerados

    int resultado = verifica_numeros(numeros);

    endwin(); 

    //atualiza o saldo da carteira se baseando se ganhou ou não
    gerenciar_carteira_niquel(carteira, aposta, resultado);

    free(numeros); // Libera a memória alocada
}

////////////////////////////////////////////////////////////////////////////////

int main() { //função compartilha entre todos os codigos
    float carteira = 0.0;  
    int escolha_jogo;

    printf("\n");
    titulo();

    //recebe e verifica o saldo inicial
    do {
        printf("\n");
        printf("Digite o valor inicial da sua carteira (valor positivo): R$");
            scanf("%f", &carteira);
        
        if (carteira < 0) {
            printf("Valor inválido! Por favor, insira um valor positivo.\n");
        }
    } while (carteira < 0); 

    printf("\n");
    escolha_jogo = menu();
    int retorno_jogo;

    do {
        //verifica se a carteira não esta zerada
        if (carteira <= 0) {
            printf("O seu saldo acabou! Reinicie o código para recarregar.\n");
            exit(0);
        }

        //chama os jogos baseado na escolha da pessoa
        switch (escolha_jogo) { //os cases estão como cometário pois nesse arquivo é só o caça niquel
            case 1:
                //roleta(&carteira);
                break;
                
            case 2: 
                //blackjack(&carteira); 
                break;
                
            case 3:
                caca_niquel(&carteira); 
                break;
                
            case 4:
                //saque_saldo(&carteira);
                exit(0);
                break;
                
            default:
                printf("Opção inválida!\n");
        }

        //verifica a carteira no fim do jogo
        if (carteira <= 0) {
            printf("O seu saldo acabou! Reinicie o código para recarregar.\n");
            exit(0);
        }

        //pergunta se quer repetir o mesmo jogo ou escolher um novo
        do {
            printf("Deseja jogar o mesmo jogo novamente? 1- SIM 2- NÃO \n");
                scanf("%d", &retorno_jogo);

        if (retorno_jogo != 1 && retorno_jogo != 2) {
             printf("Opção inválida. Por favor, digite 1 para SIM ou 2 para NÃO.\n");
    }
} while (retorno_jogo != 1 && retorno_jogo != 2); //quebra o do do inicio do jogo

        if (retorno_jogo == 2) {
            exit(0);
        }

    } while (retorno_jogo == 1 || (retorno_jogo == 2 && escolha_jogo != 0)); 

    return 0;
}