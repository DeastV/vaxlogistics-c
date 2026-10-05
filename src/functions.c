/**O Documento functions.c, contém todas as funções principais do programa.
 * Autor: David Vasques
 */

#include "projeto.h"

/**
 * @brief Altera a data atual do sistema.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param buff String contendo a nova data no formato "DD-MM-AAAA".
 * 
 * @details Verifica se a data fornecida é válida e se é posterior à data atual.
 *          Caso contrário, imprime uma mensagem de erro.
 */
void altera_data(Sys *sys, char *buff) {
    int dia, mes, ano;

    // Extrai a data do buffer no formato "DD-MM-AAAA"
    sscanf(buff, "%*s %02d-%02d-%04d", &dia, &mes, &ano);

    // Verifica se a data é válida e posterior à data atual
    if (!data_valida(dia, mes, ano) || !compara_data(sys, dia, mes, ano)) {
        imprime_msg(sys, EDATEPT, EDATE);
        return;
    }

    // Atualiza a data no sistema
    sys->data_atual.dia = dia;
    sys->data_atual.mes = mes;
    sys->data_atual.ano = ano;

    // Imprime a nova data
    printf("%02d-%02d-%04d\n", sys->data_atual.dia, sys->data_atual.mes, sys->data_atual.ano);
}

/**
 * @brief Cria um novo lote de vacinas.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param buff String contendo as informações do lote no formato "nome_lote DD-MM-AAAA doses nome_vacc".
 * 
 * @details Verifica se as informações do lote são válidas. Caso sejam, aloca memória
 *          para o novo lote e inicializa-o com os dados fornecidos.
 */
void cria_lote(Sys *sys, char *buff) {
    char nome_lote[1024], nome_vacc[1024];
    int dia, mes, ano;
    int doses;

    // Extrai as informações do lote do buffer
    sscanf(buff, "%*s %1023s %d-%d-%d %d %1023s", nome_lote, &dia, &mes, &ano, &doses, nome_vacc);
    
    // Verifica se as informações do lote são válidas
    if (!info_lote_valida(sys, nome_lote, nome_vacc, dia, mes, ano, doses))
        return;

    // Realoca memória para armazenar o novo lote
    Lote *temp = realloc(sys->lotes, sizeof(Lote) * (sys->num_lotes + 1));
    if (!temp) {
        imprime_msg(sys, ENOMEMORYPT, ENOMEMORY);
        return;
    }

    sys->lotes = temp;

    // Inicializa o novo lote com os dados fornecidos
    inicializa_lote(&sys->lotes[sys->num_lotes], nome_lote, nome_vacc, dia, mes, ano, doses);

    // Imprime o nome do lote criado
    printf("%s\n", sys->lotes[sys->num_lotes++].nome_lote);
}

/**
 * @brief Lista os lotes de vacinas.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param buff String contendo os nomes das vacinas a serem listadas (opcional).
 * 
 * @details Se nenhum nome de vacina for fornecido, lista todos os lotes. Caso contrário,
 *          lista apenas os lotes correspondentes às vacinas especificadas.
 */
void lista_lote(Sys *sys, char *buff) {
    char *nomes_vacc[NOME_VACC_MAX];
    int num_nomes_vacc = 0, imprime_tudo = 1;
    char *token = strtok(buff, " \n");

    // Extrai os nomes das vacinas do buffer
    while ((token = strtok(NULL, " \n")) != NULL) {
        nomes_vacc[num_nomes_vacc++] = token;
        imprime_tudo = 0;
    }

    // Ordena os lotes antes de listar
    ordena_lote(sys);

    // Lista todos os lotes se nenhum nome de vacina foi fornecido
    if (imprime_tudo) {
        for (int i = 0; i < sys->num_lotes; i++) {
            imprime_lote(&sys->lotes[i]);
        }
        return;
    }

    // Lista apenas os lotes correspondentes às vacinas especificadas
    for (int j = 0; j < num_nomes_vacc; j++) {
        int encontrado = 0;
        for (int i = 0; i < sys->num_lotes; i++) {
            if (strcmp(sys->lotes[i].nome_vacc, nomes_vacc[j]) == 0) {
                imprime_lote(&sys->lotes[i]);
                encontrado = 1;
            }
        }
        // Imprime mensagem de erro se a vacina não foi encontrada
        if (!encontrado) {
            printf("%s: ", nomes_vacc[j]);
            imprime_msg(sys, ENOVACCPT, ENOVACC);
        }
    }
}

/**
 * @brief Aplica uma vacina a um utente.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param buff String contendo o nome do utente e o nome da vacina.
 * 
 * @details Verifica se o utente já foi vacinado no mesmo dia com a mesma vacina.
 *          Caso contrário, reduz o número de doses disponíveis no lote e registra a aplicação.
 */
void aplica_vacina(Sys *sys, char *buff) {
    char nome_utente[BUFFER], nome_vacc[NOME_VACC_MAX];
    int offset = 2;

    // Analisa os nomes do utente e da vacina no buffer
    analisa_nomes(buff, nome_utente, nome_vacc, offset);

    // Ordena os lotes antes de aplicar a vacina
    ordena_lote(sys);

    // Verifica se o utente já foi vacinado no mesmo dia com a mesma vacina
    for (int i = 0; i < sys->num_inoc; i++) {
        if (strcmp(sys->inoc_info[i].nome_utente, nome_utente) == 0 &&
            strcmp(sys->inoc_info[i].lote_info->nome_vacc, nome_vacc) == 0 &&
            sys->inoc_info[i].app_data.dia == sys->data_atual.dia &&
            sys->inoc_info[i].app_data.mes == sys->data_atual.mes &&
            sys->inoc_info[i].app_data.ano == sys->data_atual.ano) {
            imprime_msg(sys, EAVACCPT, EAVACC);
            return;
        }
    }

    // Procura um lote válido para aplicar a vacina
    for (int i = 0; i < sys->num_lotes; i++) {
        if (valida_lote(sys, &sys->lotes[i], nome_vacc)) {
            sys->lotes[i].num_doses--;
            sys->lotes[i].num_doses_app++;

            // Registra a aplicação da vacina
            if (!aloca_inoculacao(sys, nome_utente, &sys->lotes[i])) {
                imprime_msg(sys, ENOMEMORYPT, ENOMEMORY);
                return;
            }
            printf("%s\n", sys->lotes[i].nome_lote);
            return;
        }
    }

    // Imprime mensagem de erro se não houver doses disponíveis
    imprime_msg(sys, ESTOCKPT, ESTOCK);
}

/**
 * @brief Remove um lote de vacinas.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param buff String contendo o nome do lote a ser removido.
 * 
 * @details Remove o lote especificado se não houver doses aplicadas. Caso contrário,
 *          zera o número de doses disponíveis no lote.
 */
void remove_lote(Sys *sys, char *buff) {
    char nome_lote[MAXLOTE_NOME];
    int i, j;

    // Extrai o nome do lote do buffer
    sscanf(buff, "%*s %s", nome_lote);

    // Procura o lote pelo nome
    for (i = 0; i < sys->num_lotes; i++) {
        if (strcmp(sys->lotes[i].nome_lote, nome_lote) == 0) {
            printf("%d\n", sys->lotes[i].num_doses_app);

            // Remove o lote se não houver doses aplicadas
            if (sys->lotes[i].num_doses_app == 0) {
                for (j = i; j < sys->num_lotes - 1; j++) {
                    sys->lotes[j] = sys->lotes[j + 1];
                }
                sys->num_lotes--;
                Lote *temp = realloc(sys->lotes, sizeof(Lote) * sys->num_lotes);
                if (temp || sys->num_lotes == 0) sys->lotes = temp;
                return;
            } 
            else {
                // Zera o número de doses disponíveis no lote
                sys->lotes[i].num_doses = 0;
            }
            return;
        }
    }

    // Imprime mensagem de erro se o lote não foi encontrado
    printf("%s: ", nome_lote);
    imprime_msg(sys, ENOBATCHPT, ENOBATCH);
}

/**
 * @brief Lista as aplicações de vacinas.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param buff String contendo o nome do utente (opcional).
 * 
 * @details Se nenhum nome de utente for fornecido, lista todas as aplicações.
 *          Caso contrário, lista apenas as aplicações correspondentes ao utente especificado.
 */
void lista_aplicacoes(Sys *sys, char *buff) {
    char nome_utente[BUFFER];
    int i, encontrado = 0, imprime_tudo = 1, offset = 2;

    // Verifica se o nome do utente está entre aspas
    if (buff[offset] == '"') {
        offset++;
        if (sscanf(buff + offset, "%[^\"]", nome_utente) == 1) imprime_tudo = 0;
    } else if (sscanf(buff + offset, "%s", nome_utente) == 1) imprime_tudo = 0;

    // Lista todas as aplicações se nenhum nome de utente foi fornecido
    if (imprime_tudo) {
        for (i = 0; i < sys->num_inoc; i++)
            printf("%s %s %02d-%02d-%04d\n",
                sys->inoc_info[i].nome_utente,
                sys->inoc_info[i].lote_info->nome_lote,
                sys->inoc_info[i].app_data.dia,
                sys->inoc_info[i].app_data.mes,
                sys->inoc_info[i].app_data.ano);
        return;
    }

    // Lista as aplicações correspondentes ao utente especificado
    for (i = 0; i < sys->num_inoc; i++) {
        Inoc *inoc = &sys->inoc_info[i];
        if (!strcmp(inoc->nome_utente, nome_utente)) {
            printf("%s %s %02d-%02d-%04d\n",
                inoc->nome_utente, inoc->lote_info->nome_lote,
                inoc->app_data.dia, inoc->app_data.mes,
                inoc->app_data.ano);
            encontrado = 1;
        }
    }

    // Imprime mensagem de erro se o utente não foi encontrado
    if (!encontrado) printf("%s: ", nome_utente), imprime_msg(sys, ENOUSERPT, ENOUSER);
}

/**
 * @brief Remove uma aplicação de vacina.
 * 
 * @param sys Ponteiro para a estrutura do sistema.
 * @param buff String contendo o nome do utente, a data e o lote (opcional).
 * 
 * @details Remove as aplicações que correspondem aos critérios fornecidos.
 *          Caso nenhum critério seja atendido, imprime uma mensagem de erro.
 */
void remove_inoculacao(Sys *sys, char *buff) {
    char nome_utente[BUFFER], nome_lote[MAXLOTE_NOME];
    int dia, mes, ano;
    int i, novo_num = 0, removido = 0, encontrou_utente = 0;
    int args = sscanf(buff, "%*s %s %d-%d-%d %s", nome_utente, &dia, &mes, &ano, nome_lote);
    int tem_data = args >= 2, tem_lote = args == 5;
    Data data = {dia, mes, ano};

    // Verifica se a data fornecida é válida
    if (tem_data && !data_valida(dia, mes, ano)) return (void)imprime_msg(sys, EDATEPT, EDATE);

    // Verifica se a data existe no sistema
    if (tem_data && !existe_data(sys, &data)) return (void)imprime_msg(sys, EDATEPT, EDATE);

    // Verifica se o lote existe no sistema
    if (tem_lote && !existe_lote(sys, nome_lote)) {
        printf("%s: ", nome_lote); imprime_msg(sys, ENOBATCHPT, ENOBATCH); return;
    }

    // Remove as aplicações que correspondem aos critérios fornecidos
    for (i = 0; i < sys->num_inoc; i++) {
        Inoc *inoc = &sys->inoc_info[i];
        int match = !strcmp(inoc->nome_utente, nome_utente);
        if (match) encontrou_utente = 1;
        if (match && tem_data) {
            Data d = inoc->app_data;
            match = d.dia == dia && d.mes == mes && d.ano == ano;
        }
        if (match && tem_lote) match = !strcmp(inoc->lote_info->nome_lote, nome_lote);
        if (match) free(inoc->nome_utente), removido++;
        else sys->inoc_info[novo_num++] = *inoc;
    }

    // Atualiza o número de aplicações no sistema
    sys->num_inoc = novo_num;
    Inoc *temp = realloc(sys->inoc_info, sizeof(Inoc) * novo_num);
    if (temp || !novo_num) sys->inoc_info = temp;

    // Imprime mensagem de erro se nenhum critério foi atendido
    if (!removido && !encontrou_utente) printf("%s: ", nome_utente), imprime_msg(sys, ENOUSERPT, ENOUSER);
    else printf("%d\n", removido);
}
