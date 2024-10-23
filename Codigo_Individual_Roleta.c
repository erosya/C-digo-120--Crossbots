#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

void titulo() { //função para gerar titulo
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


//////////////////////////////////////////////////////////////////////////////////////////

void gerenciar_carteira_roleta(float* carteira, float aposta, int resultado) {
    //função para atualizar a carteira
    
    if (resultado >= 1 && resultado <= 4) {
        // Ganho
        switch (resultado) { //baseado no tipo de aposta que a pessoa escolheu
            case 1: *carteira += aposta * 35; break;
            case 2: *carteira += aposta * 17; break;
            case 3: *carteira += aposta * 11; break;
            case 4: *carteira += aposta * 8; break;
        }
        printf("Você ganhou! Seu saldo atual é: R$ %.2f\n", *carteira);
    } 
    
    else {
        //tira a perda da carteira
        *carteira -= aposta;
        printf("Você perdeu a aposta! Seu saldo atual é: R$ %.2f\n", *carteira);
    }
    
    if (*carteira < 0) {
        printf("Seu saldo está negativo!\n");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////

void mostrar_numero_sorteado(int n_roleta) {
    //função que pisca o numero na tela
    
    int x, y;
    initscr();
    curs_set(0); //usa a biblioteca ncurses
    noecho();
    getmaxyx(stdscr, y, x);

    mvprintw(y / 2 - 2, (x / 2) - 10, "O número da roleta foi:");
    refresh();
    usleep(1000000);

    for (int i = 0; i < 10; i++) {
        clear();
        mvprintw(y / 2 - 2, (x / 2) - 10, "O número da roleta foi:");
        if (i % 2 == 0) {
            attron(A_BOLD);
            mvprintw(y / 2, (x / 2) - 1, "%d", n_roleta);
            attroff(A_BOLD);
        }
        refresh();
        usleep(400000);
    }

    mvprintw(y / 2 + 2, (x / 2) - 10, "Aperte enter para sair");
    refresh();
    getch();
    endwin();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void roleta(float *carteira) { //função principal da roleta
    
    int n_roleta, tipo_aposta;
    float valor_aposta;
    srand(time(NULL));
    
    n_roleta = rand() % 37;

    printf("\nREGRAS: O jogo de roleta gera um número aleatório e compara com a aposta do jogador.\nSe o número gerado bater com o do jogador, ele é declarado vencedor!\n");

    do { //começa as escolhas de aposta
        printf("Escolha o tipo de aposta:\n");
        printf("1- Aposta Direta: Escolha um número (Paga 35 para 1)\n");
        printf("2- Aposta em dois números (Paga 17 para 1)\n");
        printf("3- Aposta Rua: Escolha três números (Paga 11 para 1)\n");
        printf("4- Aposta de Esquina: Escolha quatro números (Paga 8 para 1)\n");
            scanf("%d", &tipo_aposta);

        if (tipo_aposta < 1 || tipo_aposta > 4) {
            printf("Opção inválida! Por favor, escolha entre 1 e 4.\n");
        }
    } while (tipo_aposta < 1 || tipo_aposta > 4); //acaba a parte de escolha
    
    //recebe e verifica o valor da aposta
    do {
        printf("Digite o valor da aposta: R$ ");
            scanf("%f", &valor_aposta);

        if (valor_aposta < 0) {
            printf("Aposta inválida! O valor da aposta não pode ser negativo.\n");
        } 
        
        else if (valor_aposta > *carteira) {
            printf("Aposta inválida! Você não pode apostar mais do que o saldo disponível.\n");
            printf("Saldo disponível: R$ %.2f\n", *carteira);
        }
    } while (valor_aposta < 0 || valor_aposta > *carteira); //termina a verificação

    //parte das escolhas de aposta
    switch (tipo_aposta) {
        case 1: { //aposta que escolhe apenas um numero
            int aposta;
            do {
                printf("Escolha o número para a aposta (0-36):\n");
                    scanf("%d", &aposta);
                if (aposta < 0 || aposta > 36) {
                    printf("Aposta inválida! Digite um número entre 0 e 36.\n");
                }
            } while (aposta < 0 || aposta > 36);

            mostrar_numero_sorteado(n_roleta); //mostra o numero na tela
            gerenciar_carteira_roleta(carteira, valor_aposta, (aposta == n_roleta) ? 1 : 0); //atualiza a carteira
            break;
        }
        case 2: { //aposta que escolhe dois numeros
            int aposta1, aposta2;
            do {
                printf("Escolha o primeiro número para a aposta (0-36):\n");
                    scanf("%d", &aposta1);
            } while (aposta1 < 0 || aposta1 > 36);

            do {
                printf("Escolha o segundo número para a aposta (0-36):\n");
                scanf("%d", &aposta2);
            } while (aposta2 < 0 || aposta2 > 36);

            mostrar_numero_sorteado(n_roleta); //mostra o numero sorteado
            int resultado = (aposta1 == n_roleta || aposta2 == n_roleta) ? 2 : 0; //atribui o resultado pra comparação
            gerenciar_carteira_roleta(carteira, valor_aposta, resultado); //atualiza a carteira
            break;
        }
        case 3: { //aposta que escolhe tres numeros
            int numeros[3];
            for (int i = 0; i < 3; i++) {
                do {
                    printf("Digite o número %d (entre 0 e 36): ", i + 1);
                        scanf("%d", &numeros[i]);
                } while (numeros[i] < 0 || numeros[i] > 36);
            }

            mostrar_numero_sorteado(n_roleta); //mostra o numero sorteado
            int ganhou = 0;
            //verifica se a pessoa ganhou a aposta
            for (int i = 0; i < 3; i++) {
                if (numeros[i] == n_roleta) {
                    ganhou = 1;
                    break;
                }
            }

            gerenciar_carteira_roleta(carteira, valor_aposta, ganhou ? 3 : 0); //atualiza a carteira
            break;
        }
        case 4: { //aposta que entram quatro numeros
            int numeros[4];
            for (int i = 0; i < 4; i++) {
                do {
                    printf("Digite o número %d (entre 0 e 36): ", i + 1);
                        scanf("%d", &numeros[i]);
                } while (numeros[i] < 0 || numeros[i] > 36);
            }

            mostrar_numero_sorteado(n_roleta); //mostra os numeros sorteados
            
            int ganhou = 0;
            //verifica se o usuario ganhou
            for (int i = 0; i < 4; i++) {
                if (numeros[i] == n_roleta) {
                    ganhou = 1;
                    break;
                }
            }

            gerenciar_carteira_roleta(carteira, valor_aposta, ganhou ? 4 : 0); //atualiza a carteira
            break;
        }
        default:
            printf("Opção inválida!\n");
            break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
                roleta(&carteira);
                break;
                
            case 2: 
                //blackjack(&carteira); 
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
    