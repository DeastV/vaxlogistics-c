/**O Documento projeto.h, contém todas os erros, em português e inglês, constantes e estruturas do programa.
 * Inclui as bibliotecas de C e as Heads das Funções, principais e auxiliares.
 * Autor: David Vasques
 */


#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * @def NOME_VACC_MAX
 * @brief Comprimento máximo para o nome de uma vacina, incluindo o terminador nulo.
 */
#define NOME_VACC_MAX 51
/**
 * @def MAXLOTE
 * @brief Número máximo de doses em um único lote de vacinas.
 */
#define MAXLOTE 1000
/**
 * @def MAXLOTE_NOME
 * @brief Comprimento máximo para o nome de um lote de vacinas, incluindo o terminador nulo.
 */
#define MAXLOTE_NOME 21
/**
 * @def BUFFER
 * @brief Tamanho do buffer usado para entrada ou processamento de dados.
 */
#define BUFFER 65535
/**
 * @def MAX_UTENTE_NOME
 * @brief Comprimento máximo para o nome de um utente, incluindo o terminador nulo.
 */
#define MAX_UTENTE_NOME 201



#define ENOMEMORY "No memory"
#define EMAXVACCINE "too many vaccines"
#define EDUPBATCHNUM "duplicate batch number"
#define ENAME "invalid name"
#define EDATE "invalid date"
#define EBATCH "invalid batch"
#define EQUANTITY "invalid quantity"
#define ESTOCK "no stock"
#define EAVACC "already vaccinated"
#define ENOVACC "no such vaccine"
#define ENOBATCH "no such batch"
#define ENOUSER "no such user"

#define ENOMEMORYPT "sem memória"
#define EMAXVACCINEPT "demasiadas vacinas" 
#define EDUPBATCHNUMPT "número de lote duplicado"
#define ENAMEPT "nome inválido"
#define EDATEPT "data inválida"
#define EBATCHPT "lote inválido"
#define EQUANTITYPT "quantidade inválida"
#define ESTOCKPT "esgotado"
#define EAVACCPT "já vacinado"
#define ENOVACCPT "vacina inexistente"
#define ENOBATCHPT "lote inexistente"
#define ENOUSERPT "utente inexistente"


/**
 * @brief Estrutura que representa uma data.
 * 
 * Contém informações sobre o dia, mês e ano.
 */
typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;

/**
 * @brief Estrutura que representa um lote de vacinas.
 * 
 * Contém informações sobre o nome do lote, a data de criação, 
 * o número de doses disponíveis, o número de doses aplicadas 
 * e o nome da vacina associada.
 */
typedef struct Lote {
    char nome_lote[MAXLOTE_NOME]; /**< Nome do lote de vacinas. */
    Data data;                   /**< Data de criação do lote. */
    int num_doses;               /**< Número total de doses no lote. */
    int num_doses_app;           /**< Número de doses já aplicadas. */
    char nome_vacc[NOME_VACC_MAX]; /**< Nome da vacina associada ao lote. */
} Lote;

/**
 * @brief Estrutura que representa uma inoculação.
 * 
 * Contém informações sobre o nome do utente, o lote de vacinas utilizado 
 * e a data da aplicação.
 */
typedef struct Inoc {
    char *nome_utente; /**< Nome do utente que recebeu a vacina. */
    Lote *lote_info;   /**< Informações sobre o lote de vacinas utilizado. */
    Data app_data;     /**< Data em que a vacina foi aplicada. */
} Inoc;

/**
 * @brief Estrutura que representa o sistema de gestão de vacinas.
 * 
 * Contém informações sobre a data atual do sistema, o número de lotes, 
 * os lotes disponíveis, as informações de inoculação, o número de inoculações 
 * realizadas e um ponteiro auxiliar.
 */
typedef struct {
    Data data_atual;    /**< Data atual do sistema. */
    int num_lotes;      /**< Número total de lotes disponíveis. */
    Lote *lotes;        /**< Ponteiro para os lotes disponíveis. */
    Inoc *inoc_info;    /**< Ponteiro para as informações de inoculação. */
    int num_inoc;       /**< Número total de inoculações realizadas. */
    int pt;             /**< Ponteiro auxiliar para uso interno. */
} Sys;

/** Heads das Funções Auxiliares */

int data_valida(int dia, int mes, int ano);
int compara_data(Sys *sys, int dia, int mes, int ano);
int eh_hex_valido(char *str);
int info_lote_valida(Sys *sys, char *nome_lote, char *nome_vacc, int dia, int mes, int ano, int doses);
void inicializa_lote(Lote *l, char *nome_lote, char *nome_vacc, int dia, int mes, int ano, int doses);
int imprime_msg(Sys *sys, const char *pt, const char *en);
void ordena_lote(Sys *sys);
void imprime_lote(Lote *lote);
void libera_sys(Sys *sys);
int valida_lote(Sys *sys, Lote *lote, char *nome_vacc);
int aloca_inoculacao(Sys *sys, char *nome_utente, Lote *lote);
void analisa_nomes(char *c, char *nome_utente, char *nome_vacc, int offset);
int existe_lote(Sys *sys, char *nome_lote);
int existe_data(Sys *sys, Data *data);

/** Heads das Funções Principais */
void cria_lote(Sys *sys, char *buff);
void altera_data(Sys *sys, char *buff);
void lista_lote(Sys *sys, char *buff);
void aplica_vacina(Sys *sys, char *buff);
void remove_lote(Sys *sys, char *buff);
void lista_aplicacoes(Sys *sys, char *buff);
void remove_inoculacao(Sys *sys, char *buff);

#endif