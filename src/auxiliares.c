/**O Documento auxiliares.c, contém todas as funções auxiliares do programa.
 * Autor: David Vasques
 */

#include "projeto.h"

/**
 * @brief Verifica se uma data é válida.
 * 
 * @param dia Dia do mês.
 * @param mes Mês do ano.
 * @param ano Ano.
 * @return int Retorna 1 se a data for válida, 0 caso contrário.
 */
int data_valida(int dia, int mes, int ano) {
    // Array com o número de dias de cada mês (anos não bissextos)
    int dias_mes[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Verifica se o ano é bissexto e ajusta o número de dias de fevereiro
    if (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0)) {
        dias_mes[1] = 29;
    }

    // Verifica se o mês está dentro do intervalo válido
    if (mes < 1 || mes > 12) {
        return 0;
    }

    // Verifica se o dia está dentro do intervalo válido para o mês
    if (dia < 1 || dia > dias_mes[mes - 1]) {
        return 0;
    }
    return 1;
}

/**
 * @brief Compara uma data com a data atual do sistema.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param dia Dia do mês.
 * @param mes Mês do ano.
 * @param ano Ano.
 * @return int Retorna 1 se a data for igual ou posterior à data atual, 0 caso contrário.
 */
int compara_data(Sys *sys, int dia, int mes, int ano) {
    // Verifica se o ano é anterior ao ano atual
    if (ano < sys->data_atual.ano || 
        // Verifica se o mês é anterior ao mês atual no mesmo ano
        (ano == sys->data_atual.ano && mes < sys->data_atual.mes) ||
        // Verifica se o dia é anterior ao dia atual no mesmo mês e ano
        (ano == sys->data_atual.ano && mes == sys->data_atual.mes && dia < sys->data_atual.dia)) {
        return 0;
    }
    return 1;
}

/**
 * @brief Valida as informações de um lote.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param nome_lote Nome do lote.
 * @param nome_vacc Nome da vacina.
 * @param dia Dia de validade.
 * @param mes Mês de validade.
 * @param ano Ano de validade.
 * @param doses Número de doses no lote.
 * @return int Retorna 1 se as informações forem válidas, 0 caso contrário.
 */
int info_lote_valida(Sys *sys, char *nome_lote, char *nome_vacc, int dia, int mes, int ano, int doses) {
    int i;

    // Verifica se o nome do lote é muito longo ou contém caracteres inválidos
    if (strlen(nome_lote) >= MAXLOTE_NOME || !eh_hex_valido(nome_lote))
        return imprime_msg(sys, EBATCHPT, EBATCH), 0;

    // Verifica se o nome da vacina é muito longo ou contém espaços
    if (strlen(nome_vacc) >= NOME_VACC_MAX || strchr(nome_vacc, ' '))
        return imprime_msg(sys, ENAMEPT, ENAME), 0;

    // Verifica se o número de doses é válido
    if (doses <= 0)
        return imprime_msg(sys, EQUANTITYPT, EQUANTITY), 0;

    // Verifica se a data é válida e se é igual ou posterior à data atual
    if (!data_valida(dia, mes, ano) || !compara_data(sys, dia, mes, ano))
        return imprime_msg(sys, EDATEPT, EDATE), 0;

    // Verifica se o número máximo de lotes foi atingido
    if (sys->num_lotes >= MAXLOTE)
        return imprime_msg(sys, EMAXVACCINEPT, EMAXVACCINE), 0;

    // Verifica se o nome do lote já existe no sistema
    for (i = 0; i < sys->num_lotes; i++)
        if (!strcmp(nome_lote, sys->lotes[i].nome_lote))
            return imprime_msg(sys, EDUPBATCHNUMPT, EDUPBATCHNUM), 0;

    return 1;
}

/**
 * @brief Inicializa um lote com as informações fornecidas.
 * 
 * @param l Ponteiro para a estrutura do lote.
 * @param nome_lote Nome do lote.
 * @param nome_vacc Nome da vacina.
 * @param dia Dia de validade.
 * @param mes Mês de validade.
 * @param ano Ano de validade.
 * @param doses Número de doses no lote.
 */
void inicializa_lote(Lote *l, char *nome_lote, char *nome_vacc, int dia, int mes, int ano, int doses) {
    // Inicializa a estrutura do lote com zeros
    memset(l, 0, sizeof(Lote));
    // Copia o nome do lote para a estrutura
    strcpy(l->nome_lote, nome_lote);
    // Copia o nome da vacina para a estrutura
    strcpy(l->nome_vacc, nome_vacc);
    // Define o número de doses no lote
    l->num_doses = doses;
    // Define a data de validade do lote
    l->data.dia = dia;
    l->data.mes = mes;
    l->data.ano = ano;
}

/**
 * @brief Imprime uma mensagem de erro ou informação.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param pt Mensagem em português.
 * @param en Mensagem em inglês.
 * @return int Retorna 0 sempre.
 */
int imprime_msg(Sys *sys, const char *pt, const char *en) {
    // Imprime a mensagem em português ou inglês, dependendo da configuração do sistema
    puts(sys->pt ? pt : en);
    return 0;
}

/**
 * @brief Verifica se uma string é um valor hexadecimal válido.
 * 
 * @param str String a ser verificada.
 * @return int Retorna 1 se for válida, 0 caso contrário.
 */
int eh_hex_valido(char *str) {
    // Percorre cada caractere da string
    for (int i = 0; str[i] != '\0'; i++) {
        // Verifica se o caractere não é um dígito hexadecimal ou se é uma letra minúscula
        if (!isxdigit(str[i]) || islower(str[i])) {
            return 0; // Retorna 0 se não for válido
        }
    }
    return 1; // Retorna 1 se todos os caracteres forem válidos
}

/**
 * @brief Ordena os lotes do sistema por data e nome.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 */
void ordena_lote(Sys *sys) {
    // Percorre os lotes para ordenação
    for (int i = 0; i < sys->num_lotes - 1; i++) {
        for (int j = i + 1; j < sys->num_lotes; j++) {
            // Compara as datas dos lotes
            if (sys->lotes[i].data.ano > sys->lotes[j].data.ano ||
                (sys->lotes[i].data.ano == sys->lotes[j].data.ano &&
                 sys->lotes[i].data.mes > sys->lotes[j].data.mes) ||
                (sys->lotes[i].data.ano == sys->lotes[j].data.ano &&
                 sys->lotes[i].data.mes == sys->lotes[j].data.mes &&
                 sys->lotes[i].data.dia > sys->lotes[j].data.dia)) {
                
                // Troca os lotes de posição
                Lote temp = sys->lotes[i];
                sys->lotes[i] = sys->lotes[j];
                sys->lotes[j] = temp;
            }
            // Se as datas forem iguais, ordena por nome
            else if (sys->lotes[i].data.ano == sys->lotes[j].data.ano &&
                     sys->lotes[i].data.mes == sys->lotes[j].data.mes &&
                     sys->lotes[i].data.dia == sys->lotes[j].data.dia) {
                
                if (strcmp(sys->lotes[i].nome_lote, sys->lotes[j].nome_lote) > 0) {
                    // Troca os lotes de posição
                    Lote temp = sys->lotes[i];
                    sys->lotes[i] = sys->lotes[j];
                    sys->lotes[j] = temp;
                }
            }
        }
    }
}

/**
 * @brief Imprime as informações de um lote.
 * 
 * @param lote Ponteiro para a estrutura do lote.
 */
void imprime_lote(Lote *lote) {
    // Imprime os detalhes do lote no formato especificado
    printf("%s %s %02d-%02d-%04d %d %d\n",
           lote->nome_vacc, lote->nome_lote,
           lote->data.dia, lote->data.mes, lote->data.ano,
           lote->num_doses, lote->num_doses_app);
}

/**
 * @brief Libera a memória alocada para o sistema.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 */
void libera_sys(Sys *sys) {
    // Libera a memória alocada para informações de inoculação
    if (sys->inoc_info) {
        for (int i = 0; i < sys->num_inoc; i++) {
            free(sys->inoc_info[i].nome_utente); // Libera o nome do utente
        }
        free(sys->inoc_info); // Libera o array de inoculações
        sys->inoc_info = NULL;
    }

    // Libera a memória alocada para os lotes
    if (sys->lotes) {
        free(sys->lotes);
        sys->lotes = NULL;
    }
}

/**
 * @brief Valida se um lote é utilizável para uma vacina específica.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param lote Ponteiro para a estrutura do lote.
 * @param nome_vacc Nome da vacina.
 * @return int Retorna 1 se o lote for válido, 0 caso contrário.
 */
int valida_lote(Sys *sys, Lote *lote, char *nome_vacc) {
    // Verifica se o lote corresponde à vacina, possui doses disponíveis e está dentro da validade
    return strcmp(lote->nome_vacc, nome_vacc) == 0 && lote->num_doses > 0 &&
           (lote->data.ano > sys->data_atual.ano ||
            (lote->data.ano == sys->data_atual.ano && lote->data.mes > sys->data_atual.mes) ||
            (lote->data.ano == sys->data_atual.ano && lote->data.mes == sys->data_atual.mes && lote->data.dia >= sys->data_atual.dia));
}

/**
 * @brief Aloca uma nova inoculação no sistema.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param nome_utente Nome do utente.
 * @param lote Ponteiro para a estrutura do lote.
 * @return int Retorna 1 se a alocação for bem-sucedida, 0 caso contrário.
 */
int aloca_inoculacao(Sys *sys, char *nome_utente, Lote *lote) {
    // Realoca memória para armazenar uma nova inoculação
    Inoc *temp_inoc = realloc(sys->inoc_info, (sys->num_inoc + 1) * sizeof(Inoc));
    if (temp_inoc == NULL) {
        return 0; // Retorna 0 se a realocação falhar
    }
    sys->inoc_info = temp_inoc;
    memset(&sys->inoc_info[sys->num_inoc], 0, sizeof(Inoc)); // Inicializa a nova inoculação com zeros
    sys->inoc_info[sys->num_inoc].nome_utente = strdup(nome_utente); // Duplica o nome do utente
    if (sys->inoc_info[sys->num_inoc].nome_utente == NULL) {
        return 0; // Retorna 0 se a duplicação falhar
    }
    sys->inoc_info[sys->num_inoc].lote_info = lote; // Associa o lote à inoculação
    sys->inoc_info[sys->num_inoc].app_data = sys->data_atual; // Define a data de aplicação
    sys->num_inoc++; // Incrementa o número de inoculações
    return 1; // Retorna 1 se bem-sucedido
}

/**
 * @brief Analisa e separa os nomes de utente e vacina de uma string.
 * 
 * @param c String de entrada.
 * @param nome_utente Nome do utente (saída).
 * @param nome_vacc Nome da vacina (saída).
 * @param offset Posição inicial para análise na string.
 */
void analisa_nomes(char *c, char *nome_utente, char *nome_vacc, int offset) {
    // Verifica se o nome do utente está entre aspas
    if (c[offset] == '"') {
        sscanf(c + offset + 1, "%[^\"] \"%s", nome_utente, nome_vacc); // Lê o nome do utente e da vacina
    } else {
        sscanf(c + offset, "%s %s", nome_utente, nome_vacc); // Lê os nomes diretamente
    }
}

/**
 * @brief Verifica se uma data já existe no sistema.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param data Ponteiro para a estrutura da data.
 * @return int Retorna 1 se a data existir, 0 caso contrário.
 */
int existe_data(Sys *sys, Data *data) {
    // Percorre as inoculações para verificar se a data já existe
    for (int i = 0; i < sys->num_inoc; i++) {
        Data a = sys->inoc_info[i].app_data;
        if (a.dia == data->dia && a.mes == data->mes && a.ano == data->ano)
            return 1; // Retorna 1 se a data for encontrada
    }
    return 0; // Retorna 0 se a data não existir
}

/**
 * @brief Verifica se um lote já existe no sistema.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param nome_lote Nome do lote.
 * @return int Retorna 1 se o lote existir, 0 caso contrário.
 */
int existe_lote(Sys *sys, char *nome_lote) {
    // Percorre os lotes para verificar se o nome já existe
    for (int i = 0; i < sys->num_lotes; i++)
        if (!strcmp(sys->lotes[i].nome_lote, nome_lote))
            return 1; // Retorna 1 se o lote for encontrado
    return 0; // Retorna 0 se o lote não existir
}













