#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

void titulo() { //função para gerar o titulo
    
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

/////////////////////////////////////////////////////////////////////////////////////////////

                                   //FUNÇÕOES DA CARTEIRA//

////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////

void saque_saldo(float* carteira) { //função para sacar o saldo da carteira
    
    printf("\n");
    
    printf("Seu saldo final: R$ %.2f \n", *carteira);
    
    printf("\n");
    
    printf("Saldo disponivel para saque: R$???? \n");
    
    printf("\n");
    
    printf("Achou mesmo que ia conseguir sacar alguma coisa?\n");
    
    printf("\n");
    
    sleep(5); //faz ele esperar pra fazer o jumpscare da imagem
    //gera os numeros aleatorios para imrpimir a imagem final
    int imagem = (rand() % 6) + 1;
  
    
    if (imagem == 1 || imagem == 3) {
        imagem1();
    } 
    else if (imagem == 2 || imagem == 4) {
        imagem2();
    } 
    else if (imagem == 3 || imagem == 5) {
        imagem3();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

int menu() { //função para fazer o menu de escolha dos jogos
    int jogada;

    printf("Escolha o jogo que quer jogar:\n"); 
    printf(" 1- Roleta \n 2- BlackJack \n 3- Caça-Níquel \n 4- Sacar saldo \n");
        scanf("%d", &jogada);
    
    return jogada;
}

//////////////////////////////////////////////////////////////////////////////////////////

                               //INICIO DAS FUNÇÕES DA ROLETA//

//////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////

                              //INICIO FUNÇÕES DO BLACKJACK//

////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////

                         //INICIO DAS FUNÇÕES DO CAÇA NIQUEL//

////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////

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



////////////////////////////////////////////////////////////////////////////////////////////


int main() {
    float carteira = 0.0;  // Inicializa a carteira
    int escolha_jogo;

    printf("\n");
    titulo();

    // Valida o saldo inicial
    do {
        printf("\n");
        printf("Digite o valor inicial da sua carteira (valor positivo): R$");
        scanf("%f", &carteira);
        
        if (carteira < 0) {
            printf("Valor inválido! Por favor, insira um valor positivo.\n");
        }
    } while (carteira < 0); // Continua pedindo até que o valor seja positivo

    printf("\n");
    escolha_jogo = menu();
    int retorno_jogo;

    do {
        // Verifica o saldo antes de começar o jogo
        if (carteira <= 0) {
            printf("O seu saldo acabou! Reinicie o código para recarregar.\n");
            exit(0);
        }

        // Joga de acordo com a escolha do usuário
        switch (escolha_jogo) {
            case 1:
                roleta(&carteira);
                break;
                
            case 2: 
                blackjack(&carteira); // Passa o ponteiro corretamente
                break;
                
            case 3:
                caca_niquel(&carteira); // Corrigido para não redeclarar
                break;
                
            case 4:
                saque_saldo(&carteira);
                exit(0);
                break;
                
            default:
                printf("Opção inválida!\n");
        }

        // Verifica o saldo após o jogo
        if (carteira <= 0) {
            printf("O seu saldo acabou! Reinicie o código para recarregar.\n");
            exit(0);
        }

        // Pergunta se o jogador quer repetir o mesmo jogo
        
        do {
            printf("Deseja jogar o mesmo jogo novamente? 1- SIM 2- NÃO \n");
                scanf("%d", &retorno_jogo);

        if (retorno_jogo != 1 && retorno_jogo != 2) {
             printf("Opção inválida. Por favor, digite 1 para SIM ou 2 para NÃO.\n");
    }
} while (retorno_jogo != 1 && retorno_jogo != 2);

        if (retorno_jogo == 2) {
            escolha_jogo = menu(); // Se o jogador escolher não, vai para o menu
        }

    } while (retorno_jogo == 1 || (retorno_jogo == 2 && escolha_jogo != 0)); // Repete o loop enquanto o usuário quiser jogar novamente

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                                        //FUNÇÕES DAS IMAGENS//

////////////////////////////////////////////////////////////////////////////////////////////////////






void imagem1(){
     printf("######################################+++++++++++++++++++++###################################################\n");
    printf("######################################++++++++++++++++++++++++##+++++#########################################\n");
    printf("########################################+++++++++++++++++++++++####+##########################################\n");
    printf("##########################################+++++#+++++++++++++++##########+++##################################\n");
    printf("##########################################++++###++++++++++++++##########++###################################\n");
    printf("##################################################+++#+++++++++###############################################\n");
    printf("#################################################+#####++++++#################################################\n");
    printf("###############################################+++++++##++++++################################################\n");
    printf("###############################################+++++++#++++###################################################\n");
    printf("############################################++++++++++++++####################################################\n");
    printf("################################################+++++++##++###################################+###############\n");
    printf("################################################+++++++++++++#################################+###############\n");
    printf("############################################++++++----------++++##############+########+++++#+################\n");
    printf("##########################################++++------------------+######################+++++##################\n");
    printf("############################################++++++----------------++##################++++++##################\n");
    printf("#############################################+++---------------------+###############+++++####################\n");
    printf("#############################################++++----------------------+#############++#######################\n");
    printf("############################################+++++------------------------#####################################\n");
    printf("#############################################+++++------------------------+##########+########################\n");
    printf("#############################################++++++------------------------+##################################\n");
    printf("#############################################+#+++++++----------------------++################################\n");
    printf("############################################++##+#++++++---------------------++###############################\n");
    printf("#########################################++++++++++++++++---------------------+###############################\n");
    printf("+#######++-+#############################++++++++##+++++++--------------------+#######################++--+###\n");
    printf("+########++-+###########################++++++++++####+-----------------------+#######################+----+##\n");
    printf("+########++-+###########################+++++++#######+--------+###++---------+#######################+----+##\n");
    printf("+#######++--###########################+++++++########++------++++++-++-------+#######################++----##\n");
    printf("++######++-+############################+-++---+++##++-------------------------+######################+-----+#\n");
    printf("+++####+---###############################+++++++####+-------------------------+######################+-----++\n");
    printf("++++++++---##################################++++######+-------++++------------+#######################+------\n");
    printf("+++++++----+++##############################++-+########+------+###+------------++############----++##+++----+\n");
    printf("++##++++-+++#####+--+#######################+--+########+-------+###+------------+###########+---------------+\n");
    printf("++####++++++#####+--#######################+--+######+#+++-------+###+-----------+############+-------------++\n");
    printf("++######++######+-+########################+-+########+++++++-----###+-----------+##############+----------+++\n");
    printf("++++##########+--+########################++++###############+----+###+---------+################++--------+++\n");
    printf("+++++++#######++##########################+++#################++--+###+---------+##################++------+++\n");
    printf("+++++++#####+--###########################+++##################+++####+--------+######################+-----++\n");
    printf("+++++++++++---+###########################++++##################++####+--+----+########################++--++#\n");
    printf("++++++++++----+###########################++-+##+++##################++------+###########################++++#\n");
    printf("#++++++--------+##########################+---+++++##################+--+---+#############################++++\n");
    printf("#####+-----+++--+##########################+--+++++++#+##############---+--+###############################+++\n");
    printf("####+++--+++++---+#########################+--++++++++++++##########+---+-+################################+++\n");
    printf("####+++++++#++----+#########################+-+##+++----++######+###+---++#################################+++\n");
    printf("####++++++++++-----+########################+--##+-----+++######++##+---++++++##########################+++###\n");
    printf("######+++++++-------+########################--###+----++######+-+#++---+++++++###++########+###########++++++\n");
    printf("#########+#+++-------++######################+-####++---+#####+--+++---++##++++++++++++++++##############++++-\n");
    printf("##########++++----------++####################++####++--++###+---++---++#########++++++##################++---\n");
    printf("#############+------------+++########################++++++++----+---+###################################++---\n");
    printf("############++++-------------++#######################+++++--------++####################################++---\n");
    printf("#############+++--+-----------++########################+++-------++####################################+++--+\n");
    printf("#############+++++-------------++###################+++++++------++####################################+++--+#\n");
    printf("##############+++++-------------++###################+++++-------++##############################++###+++---##\n");
    printf("################++++---------+++++++#####################+++-++++++++++##########################+-+##++--+###\n");
    printf("###############++++++--+-------+++++####+#############++++++--+++++++++++########################++#+++--#####\n");
    printf("################++++++++++----+++++##+##+##+##+++++++++++-+------++++++++++++#####################++--++######\n");
    printf("##################++++++##+++-+++++###+####+++#++++++++---++----++--++++++++++################++++++##########\n");
    printf("####################+++++###+++++++####+#####++++++++-++--+++-+++++-+----++++++++###########++++++############\n");
    printf("#####################++++#####++#++##########+++++#+++----+-++++----+----+---+++++######++++++################\n");
    printf("#######################++########+#############+++##++++-++++-++------+++++++++--+++++++++####################\n");

    
}


void imagem2() {
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("-------------------------------------+################++--------------------------------------------\n");
    printf("---------------------------------+##++-----------------+++###+--------------------------------------\n");
    printf("-----------------------------+###+---------------------------++#+-----------------------------------\n");
    printf("-------------------------+##++----------------------------------+##+--------------------------------\n");
    printf("------------------------#+-----------------------------------------+#+------------------------------\n");
    printf("----------------------+#+--------------------------------------------+#++---------------------------\n");
    printf("--------------------+##+-----------------------------------------------+##+-------------------------\n");
    printf("--------------------+#----------------------------------------------------#+------------------------\n");
    printf("--------------------++---------------------------------+-------------------++-----------------------\n");
    printf("-------------------+#----------------------------------++-------------------#+----------------------\n");
    printf("------------------+#-----------------------------------+#--------------------#+---------------------\n");
    printf("------------------++------------------------------------#--------#+----------+#---------------------\n");
    printf("------------------#+------------------------------------#+------#+------------#+--------------------\n");
    printf("-----------------+#-------------------------------------#+------+-----------++++--------------------\n");
    printf("-----------------#-----------------------------+#+-----+#+-----++------------+##+-------------------\n");
    printf("-----------------#------------------------------+##+--+#-------++--------+++---+#+------------------\n");
    printf("-----------------#--------------------------------++#+----+----++-----++#+----+-+#------------------\n");
    printf("----------------+#-----------------+++#########++----+++++#+-++-+-++++++--------+#------------------\n");
    printf("----------------+#---++#######++++-------#################++++++#+#-----+++++++++#+-----------------\n");
    printf("----------------+#---+#+-----++++##########++#++--++#+--++##++#+###++##############+----------------\n");
    printf("-----------------#---##----------------+###++#####++-+++#++############++++++-----##----------------\n");
    printf("-----------------#+--+##+-----------++++###+##########+-+--##------+#######+++---+##+---------------\n");
    printf("-----------------+#+-------------------+###+++++++###++----#+-----+###++####++++-+##+---------------\n");
    printf("------------------++-------------------++##+---++--++#----+#+-----+###+++-+++++++-##+---------------\n");
    printf("-------------------#+-----------------+++########+##+#---+##------+++#+++-++++++--##+---------------\n");
    printf("-------------------+#----------------#+---+++++++#######++++-----++++###############----------------\n");
    printf("--------------------+#+---+#+-+#+---#+----+++++++----------------+#+----++-+++--+++#----------------\n");
    printf("---------------------+#+----++###--++-----++++++----+#------------+##+--++++++--+#+#----------------\n");
    printf("----------------------+#+------+#--++-----+++++----+#+--------------+#+-++++++--++#+----------------\n");
    printf("-----------------------+#-------+--++-----++++---+#+--###+-----------+#--+++++---+#-----------------\n");
    printf("------------------------#+---+++##---------+++--+#+---#+------------+#+--+++++--+#+-----------------\n");
    printf("------------------------#+-----++#+--------++--+#+----+###+-+++--+#+##+---++++--++------------------\n");
    printf("------------------------#+-------+#-------+++-+#++#+------++++-----+++#+--++++-#+-------------------\n");
    printf("------------------------#+----+#####------++++#+#------+##+#++#####++-#+--++++##+-------------------\n");
    printf("------------------------#+--------##------+++#+-#----##+#++#++#++#+##++#+++++#+---------------------\n");
    printf("------------------------#+--------##+-----++##--#+--##################+##+++##----------------------\n");
    printf("------------------------#+--------+-------++#+--++--##################+-+#++#-----------------------\n");
    printf("-----------------------#+--------++--#++-++-++------##################--++-##-----------------------\n");
    printf("----------------------+#+--------+---++++#+--+++++--+################+-++-#+------------------------\n");
    printf("----------------------+#----------------####+++++----+##############--+++##+------------------------\n");
    printf("---------------------+#+----------------+#++#+++-------###########+-++###+--------------------------\n");
    printf("--------------------+#+----------++---------##++++#+---+#######+----+###+---------------------------\n");
    printf("-------------------+#+--------+#++-----------+##++#++------++------####-----------------------------\n");
    printf("------------------+#+--------##----------------+####+-#+#+-+#+###+#------------------------------\n");
    printf("---------------+##+--------+#+------------------++################++--------------------------------\n");
    printf("---------+++##+++++-------++-------------------------++#+--+---+-+++++++++++++###+++++--------------\n");
    printf("---+++++#+------------------------------------------------------------------------------------------\n");
    printf("-----------------------------------------+##++------++++--------------------------------------------\n");
    printf("----------------------------------------------#+--++------------------------------------------------\n");
    printf("----------------------------------------------------------------------------------------------------\n");
}







void imagem3() {
    printf(".. .............................. ............................. ................................ ...\n");
    printf(".. .............................. ............................. ................................ ...\n");
    printf("....................................................................................................\n");
    printf("......... .......... ........................... .......................................... ........\n");
    printf("....................................................................................................\n");
    printf("....................................................................................................\n");
    printf("..................................................................................--................\n");
    printf(".............-+#############+---................................--+##############+-.-++-............\n");
    printf("......-+--###############################-...........-###############################+.-++..........  \n");
    printf("...-++--#####++--.........--++###########+........ ...-+###########+++---.........-+####-.-.........\n");
    printf("...-.-###+...................................... ................................ ....+###..........  \n");
    printf(".....#+-...........-+++----++++++----......................-++++++-++++++-+--............+#-........\n");
    printf(".....................................-+............... ..+-.........................................\n");
    printf("..... .........-#################+-..-##-... ..........-#+..-+##################+...................\n");
    printf(".............+#-...+#####+-##-.+#-....-#+..............+#.....-#+..#######-++....+#-................\n");
    printf("...........-+......+#########-..+#-....#........ . .....+....-#+...#########+..  ..-+.... ......... \n");
    printf(".............+#+++-+########+-...--..........................------+#########++###+.................\n");
    printf("................-------------------.................................................................\n");
    printf("...................... .............................................................................\n");
    printf("....................................................................................................\n");
    printf("....................................................................................................\n");
    printf("....... ....................................................  ...... .......... ....................\n");
    printf("....................................................................................................\n");
    printf("...................... . ........ ..................................................................\n");
    printf("................................................................. .......... .......................\n");
    printf("......................................................................................-+............\n");
    printf(".................. ..................................................................++.............\n");
    printf("....................................................................................++..............\n");
    printf("...................................................... ............................++...............\n");
    printf("..................................................................................++................\n");
    printf(".........................................................................-.......-#.................\n");
    printf("........................................................................#+..... -#-.................\n");
    printf(".............................. ......................................-##........++..................\n");
    printf(".................................................................-+##+.........-#...................\n");
    printf("..........................................................--++###+-......... ..#-...................\n");
    printf("........................#+--------------------+++##########+--................-#....................\n");
    printf("........................+#++++###########++++++----...........................+-....................\n");
    printf(".........................+-....................... ...........................+.....................\n");
    printf(".............................................................................-+............ ........\n");
    printf(".. .......... ................... ............................. ............ --................. ...\n");
    printf(".. .......... ................... ............................. ................................ ...\n");
    printf(".. .......... ................... ............................. ............ ................... ...\n");
    printf("...................................................................... .............................\n");
    printf("...................................................................... .............................\n");
    printf("....................................................................................................\n");
}