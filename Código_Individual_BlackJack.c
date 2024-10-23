#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

void titulo() { //função para rodar o titulo
    
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

///////////////////////////////////////////////////////////////////////////////////////////

void gerenciar_carteira_blackjack(float* carteira, float aposta, int resultado) {
    //função para atualizar a carteira depois que acaba o jogo
    
    if (resultado == 1) {
        *carteira += aposta * 3;   
        printf("Seu saldo atual é: R$ %.2f\n", *carteira);
    }
    
    else if (resultado == 2) {
        *carteira -= aposta;  
        printf("Você perdeu a aposta! Seu saldo atual é: R$ %.2f\n", *carteira);
    }
    
    else {
        printf("O saldo não foi mudado: R$ %.2f\n", *carteira);
    }

    // Verifica se a carteira ficou negativa
    if (*carteira < 0) {
        printf("Seu saldo está negativo!\n");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

int menu() { //menu para o usuario escolher qual jogo quer
    
    int jogada;

    printf("Escolha o jogo que quer jogar:\n"); 
    
    printf(" 1- Roleta \n 2- BlackJack \n 3- Caça-Níquel \n 4- Sacar saldo \n  ");
        scanf("%d", &jogada);
    
    return jogada;
}

//////////////////////////////////////////////////////////////////////////////////////////

void gerar_carta(int *carta) { //função que gera cartas aleatorias
    
    *carta = rand() % 13 + 1;
    
    if (*carta > 10) {
        *carta = 10;
    }
    
}

/////////////////////////////////////////////////////////////////////////////////////////////

void imprimir_cartas(int *cartas, int quantidade) { //função que imprime as cartas na tela
    
    for (int i = 0; i < quantidade; i++) {
    
        if (cartas[i] == 1) {
            printf("ÁS  "); //coloquei assim para imprimir corretamente o nome da carta
        } 
        else {
            printf("%d ", cartas[i]);
        }
    }
    
    printf("\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////

int soma_cartas(int *cartas, int quantidade) { //função para somar as cartas
   
    int soma = 0;
    int tem_as = 0;

    for (int i = 0; i < quantidade; i++) {
        soma += cartas[i];

        if (cartas[i] == 1) {
            tem_as++;
        }
    }

    while (tem_as > 0 && soma <= 11) {  
        soma += 10; //aqui ele coloca a regra do as na soma
        tem_as--;
    }

    return soma;
}


////////////////////////////////////////////////////////////////////////////////////////////

void blackjack(float* carteira) { //função principal do blackjack
    
    srand(time(NULL));
    
    #define MAX_CARTAS 10
    float aposta;
    int resultado;
    
    //verifica e recebe a aposta
    do {
        printf("Digite o valor da aposta: R$ ");
        scanf("%f", &aposta);

        if (aposta <= 0) {
            printf("Aposta inválida! O valor deve ser positivo.\n");
        } else if (aposta > *carteira) {
            printf("Aposta inválida! O valor apostado não pode exceder o saldo da carteira.\n");
        }
    } while (aposta <= 0 || aposta > *carteira);

    int cartas_mesa[MAX_CARTAS], cartas_jogador[MAX_CARTAS];
    int qnt_jogador = 0, qnt_mesa = 0;
    int resposta;

    //gera as duas primeiras cartas da mesa e do usuario
    gerar_carta(&cartas_jogador[qnt_jogador++]);
    gerar_carta(&cartas_jogador[qnt_jogador++]);
    
    gerar_carta(&cartas_mesa[qnt_mesa++]);
    gerar_carta(&cartas_mesa[qnt_mesa++]);

    //imprime as cartas do usuario e apenas a primeira carta da mesa
    printf("Suas cartas: \n");
        imprimir_cartas(cartas_jogador, qnt_jogador);
    
    int soma_jogador = soma_cartas(cartas_jogador, qnt_jogador);
    
    printf("Soma das suas cartas: %d\n", soma_jogador);

    printf("//////////////////\n");
    
    printf("Cartas visíveis da mesa: \n");
        imprimir_cartas(cartas_mesa, 1);
    
    int soma_mesa = soma_cartas(cartas_mesa, qnt_mesa);

    printf("//////////////////\n");

    //aqui ele pergunta se o usuario quer receber mais cartas e faz a soma
    while (1) {
        printf("Quer mais uma carta? 1- SIM 2- NAO\n");
            scanf("%d", &resposta);

        if (resposta == 1) {
            gerar_carta(&cartas_jogador[qnt_jogador++]);

            printf("Suas cartas: \n");
                imprimir_cartas(cartas_jogador, qnt_jogador);

            soma_jogador = soma_cartas(cartas_jogador, qnt_jogador);
            
            printf("Soma das suas cartas: %d\n", soma_jogador);
            
            printf("//////////////////\n");
            
            //ve se ainda esta dentro do 21
            if (soma_jogador > 21) {
                printf("Você estourou a sequência! A mesa venceu.\n");
                resultado = 2;  //usado para ver se ganhou ou perdeu a aposta
                
                //arruma o novo saldo da carteira baseado no resultado da jogada
                gerenciar_carteira_blackjack(carteira, aposta, resultado);
                return;  
            }
        } 
        else {
            break;  //quebra o loop se o usuario nã quiser mais cartas
        }
    }

    //a mesa recebe cartas se o jogador receber ou ele bater a soma de 17
    while (soma_mesa < 17 && qnt_mesa < MAX_CARTAS) {
        gerar_carta(&cartas_mesa[qnt_mesa++]);
        
        printf("Dealer recebe uma carta.\n");
        soma_mesa = soma_cartas(cartas_mesa, qnt_mesa);
    }
    
    printf("Cartas da mesa: \n");
        imprimir_cartas(cartas_mesa, qnt_mesa);
    
    printf("Soma da mesa: %d\n", soma_mesa);
    
    printf("//////////////////\n");

    //vê se o usuario ou a mesa ganhou
    if (soma_mesa > 21) {
        printf("A mesa estourou a sequência! Você venceu a aposta\n");
        resultado = 1;
    }
    
    else if (soma_jogador > soma_mesa) {
        printf("Você ganhou a aposta.\n");
        resultado = 1;
    }
    
    else if (soma_jogador < soma_mesa) {
        printf("A mesa venceu a aposta!\n");
        resultado = 2;
    }
    
    else {
        printf("Empate!\n");
        resultado = 0;
    }
    
    //arruma o novo saldo nessa situação
    gerenciar_carteira_blackjack(carteira, aposta, resultado);
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
                blackjack(&carteira); 
                break;
                
            case 3:
                //caca_niquel(&carteira); 
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