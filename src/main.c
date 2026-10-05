/**O Documento main.c, contém a função principal do programa.
 * Autor: David Vasques
 */

#include "projeto.h"

/**
 * @brief Função principal do programa.
 * 
 * Esta função inicializa o sistema, define a data atual, lê os comandos do usuário e executa as operações correspondentes.
 * O programa continua a executar até que o comando 'q' seja dado.
 * 
 * @param argc Número de argumentos passados para o programa na linha de comando.
 * @param argv Array de strings com os argumentos passados para o programa.
 * 
 * @return 0 se o programa for executado com sucesso.
 */
int main(int argc, char *argv[]) {
    char buff[BUFFER]; /**< Buffer para armazenar a linha de entrada. */
    Sys sys = {0};     /**< Estrutura do sistema, inicializada com valores padrões. */
    sys.data_atual = (Data){1, 1, 2025}; /**< Define a data atual do sistema para 1º de janeiro de 2025. */
    sys.pt = (argc == 2 && strcmp(argv[1], "pt") == 0) ? 1 : 0; /**< Verifica se o idioma é português (se o argumento "pt" for fornecido). */

    sys.lotes = malloc(sizeof(Lote) * 100); /**< Aloca memória para até 100 lotes de vacinas. */
    if (!sys.lotes) { /**< Se a alocação de memória falhar, exibe uma mensagem de erro e sai. */
        imprime_msg(&sys, ENOMEMORYPT, ENOMEMORY);
        return 0;
    }
    sys.num_lotes = 0; /**< Inicializa o número de lotes como 0. */
    sys.inoc_info = NULL; /**< Inicializa a lista de inoculações como vazia. */
    sys.num_inoc = 0; /**< Inicializa o número de inoculações como 0. */

    while (fgets(buff, BUFFER, stdin)) { /**< Lê as entradas do usuário até encontrar o fim da entrada (EOF). */
        switch (buff[0]) {
            case 'q': libera_sys(&sys); return 0; /**< Comando 'q' para sair, liberando a memória. */
            case 'c': cria_lote(&sys, buff); break; /**< Comando 'c' para criar um novo lote. */
            case 't': altera_data(&sys, buff); break; /**< Comando 't' para alterar a data do sistema. */
            case 'l': lista_lote(&sys, buff); break; /**< Comando 'l' para listar os lotes registrados. */
            case 'a': aplica_vacina(&sys, buff); break; /**< Comando 'a' para aplicar uma vacina. */
            case 'r': remove_lote(&sys, buff); break; /**< Comando 'r' para remover um lote de vacinas. */
            case 'u': lista_aplicacoes(&sys, buff); break; /**< Comando 'u' para listar as vacinas aplicadas. */
            case 'd': remove_inoculacao(&sys, buff); break; /**< Comando 'd' para remover uma inoculação de vacina. */
        }
    }

    libera_sys(&sys); /**< Libera a memória alocada antes de sair do programa. */
    return 0;
}
