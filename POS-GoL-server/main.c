#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include "pos_sockets/char_buffer.h"
#include "pos_sockets/active_socket.h"
#include "pos_sockets/passive_socket.h"
#include "pole.h"
#include "buffer.h"
#include "pole_stringov.h"

#define ACCEPT_RESPONSE "all:ok"
_Bool server_ma_bezat;

typedef struct thread_data {
    short port;

    STR_POLE* pole_nazvy;
    pthread_mutex_t mutex_subory;
    pthread_cond_t cond_subory; // broadcast treba vyslat
} THREAD_DATA;

void thread_data_init(struct thread_data* data, short port, STR_POLE* pole_nazvy) {
    data->port = port;
    data->pole_nazvy = pole_nazvy;

    pthread_mutex_init(&data->mutex_subory, NULL);
    pthread_cond_init(&data->cond_subory, NULL);
}

void thread_data_destroy(struct thread_data* data) {
    pthread_mutex_destroy(&data->mutex_subory);
    pthread_cond_destroy(&data->cond_subory);

    data->port = 0;
    data->pole_nazvy = NULL;
}

typedef struct klient_vlakno_data {
    int id_klienta;
    ACTIVE_SOCKET * moj_socket;

    pthread_cond_t* cond_druhe_vlakno_start;
    pthread_mutex_t* mutex;

} KLIENT_VLAKNO;

void klient_vlakno_init(KLIENT_VLAKNO * data, ACTIVE_SOCKET* moj_socket, int id_klienta) {
    data->id_klienta = id_klienta;
    data->moj_socket = moj_socket;
    data->cond_druhe_vlakno_start = (pthread_cond_t*) calloc(1, sizeof (pthread_cond_t));
    pthread_cond_init(data->cond_druhe_vlakno_start, NULL);
    data->mutex = (pthread_mutex_t*) calloc(1, sizeof (pthread_mutex_t));
    pthread_mutex_init(data->mutex, NULL);
}

void klient_vlakno_destroy(KLIENT_VLAKNO* data) {
    data->id_klienta = -1;
    data->moj_socket = NULL;
    pthread_cond_destroy(data->cond_druhe_vlakno_start);
    pthread_mutex_destroy(data->mutex);
    free(data->cond_druhe_vlakno_start);
    data->cond_druhe_vlakno_start = NULL;
    free(data->mutex);
    data->mutex = NULL;
}

_Bool skus_deserializovat_klientovu_spravu(struct char_buffer* buf) {
    if (buf) {
        printf(" Obdrzal som spravu >> '%s'\n", buf->data);
        return true;
    }
    return false;
}

/**
    \n SPRACOVANIE DAT OD KLIENTA
 */
_Bool skus_ziskat_data_klienta(struct active_socket* my_sock) {
    _Bool result = false;
    CHAR_BUFFER r_buf;
    char_buffer_init(&r_buf);

    if(active_socket_try_get_read_data(my_sock, &r_buf)) { // ulozi do buffra prvu spravu zo socketu, ktora sa nachadza vo fronte sprav od klienta
        if(r_buf.size > 0) {
            if(active_socket_is_end_message(my_sock, &r_buf)) { // klient chce ukoncit komunikaciu
                printf(" Dostal som ku koncu komunikacie!!\n");
                active_socket_write_end_message(my_sock);
                active_socket_stop_reading(my_sock); // uplne ukoncenie komunikacie
            } else if (skus_deserializovat_klientovu_spravu(&r_buf)) {
                char_buffer_clear(&r_buf);
                char_buffer_append(&r_buf, ACCEPT_RESPONSE, 6);
                active_socket_write_data(my_sock, &r_buf);
                result = true;
            } else {
                printf("Klient poslal spravu v zlom formate\ndata: %s\n", r_buf.data);
            }
        }
    }
    char_buffer_destroy(&r_buf);
    return result;
}

void* ukladaj_spravy_klienta(void* klient_data) {
    KLIENT_VLAKNO data = *(KLIENT_VLAKNO *) klient_data;
    printf(" (info) Klient (%d) je pripojeny!\n", data.id_klienta);

    active_socket_start_reading(data.moj_socket, data.cond_druhe_vlakno_start); // uklada data do struktury | BLOKUJUCA funkcia, cakam kym niekto nezavola, ze koncim
}

void* riad_komunikaciu(void* klient_data) {
    KLIENT_VLAKNO data = *(KLIENT_VLAKNO *) klient_data;

    pthread_mutex_lock(data.mutex);
    if (data.moj_socket != NULL) {
        printf("Komunikacia, pouzivam socket [%d] \n", data.moj_socket->socket_descriptor);
        pthread_cond_wait(data.cond_druhe_vlakno_start, data.mutex);
        pthread_mutex_unlock(data.mutex);

        if (active_socket_is_reading(data.moj_socket)) {
            if (skus_ziskat_data_klienta(data.moj_socket)) { // deserializacia informacie zo socketu
            }
        }
        active_socket_destroy(data.moj_socket);
        free(data.moj_socket); // alokovane v 'vytvor_spojenia_s_klientmi'
        klient_vlakno_destroy(&data);
        return NULL;
    }
    pthread_mutex_unlock(data.mutex);
    return NULL;
}

GENERUJ_POLE(pthread_t*, vlakien);

void* vytvor_spojenia_s_klientmi(void* thread_data) {
    THREAD_DATA *data = (THREAD_DATA *)thread_data;

    POLE_vlakien vlakna_citanie;
    init_pole_vlakien(&vlakna_citanie, 3);

    POLE_vlakien vlakna_riadenie;
    init_pole_vlakien(&vlakna_riadenie, 3);

    PASSIVE_SOCKET p_socket;
    passive_socket_init(&p_socket);
    passive_socket_start_listening(&p_socket, data->port);

    _Bool vlakno_pridane;
    int id = 1;

    while (passive_socket_is_listening(&p_socket)) {

        struct active_socket* act_socket = (struct active_socket*) calloc(1, sizeof (struct active_socket));
        active_socket_init(act_socket);

        passive_socket_wait_for_client(&p_socket, act_socket); // blokuje, kym sa niekto nepripoji

        if (act_socket != NULL)  // dockal som sa noveho klienta
        {
            KLIENT_VLAKNO klient_data;
            klient_vlakno_init(&klient_data, act_socket, id++);
            /* cast 'vlakna_citanie */
            /* dynamicky alokovana pamat(neviem dopredu, kolko klientov mam cakat), POZOR! treba dealokovat vlakno */
            pthread_t* nove_vlakno = (pthread_t*) calloc(1, sizeof (pthread_t));

            if (pridaj_prvok_do_pola_vlakien(nove_vlakno, &vlakna_citanie)) {
                pthread_create(nove_vlakno, NULL, ukladaj_spravy_klienta, &klient_data);
            }
            else {                    // ak sa mi nepodari ho vlozit do pola, nic s nim uz nebudem robit
                free(nove_vlakno);
            }
            /* cast 'vlakna_riadenie */
            nove_vlakno = (pthread_t*) calloc(1, sizeof (pthread_t));

            if (pridaj_prvok_do_pola_vlakien(nove_vlakno, &vlakna_riadenie)) {
                pthread_create(nove_vlakno, NULL, riad_komunikaciu, &klient_data);
            }
            else {                    // ak sa mi nepodari ho vlozit do pola, nic s nim uz nebudem robit
                free(nove_vlakno);
            }
        }

        if (!server_ma_bezat)
            passive_socket_stop_listening(&p_socket);
    }

    passive_socket_stop_listening(&p_socket);
    passive_socket_destroy(&p_socket);

    for (int i = 0; i < vlakna_citanie.offset; ++i) {              // pockanie na ukoncenie vlakien
        pthread_t* vlakno_i;
        spristupni_prvok_pola_vlakien(i, &vlakna_citanie, &vlakno_i);
        pthread_join(*vlakno_i, NULL);
        free(vlakno_i);
    }
    for (int i = 0; i < vlakna_riadenie.offset; ++i) {              // pockanie na ukoncenie vlakien
        pthread_t* vlakno_i;
        spristupni_prvok_pola_vlakien(i, &vlakna_riadenie, &vlakno_i);
        pthread_join(*vlakno_i, NULL);
        free(vlakno_i);
    }

    znic_pole_vlakien(&vlakna_citanie);
    znic_pole_vlakien(&vlakna_riadenie);
    return NULL;
}

void vypni_server(int param) {
    server_ma_bezat = false;
}

int main(int argc, char* argv[]) {

    server_ma_bezat = true;
    signal(SIGINT, vypni_server); // ctrl+c ==> server shutdown

    /* deklaracia */
//    pthread_t th_klienti_pripojenia;
    STR_POLE nazvy_suborov;
    struct thread_data thr_data;
    /* inicializacia */
    init_pole(&nazvy_suborov, 10);

    thread_data_init(&thr_data, 17834, &nazvy_suborov);

//    pthread_create(&th_klienti_pripojenia, NULL, vytvor_spojenia_s_klientmi, &thr_data); // sluzi na ziskavanie dat od klienta
    vytvor_spojenia_s_klientmi(&thr_data);
    /* moze sa pokracovat vo vykonavani mainu */
//    pthread_join(th_klienti_pripojenia, NULL);

    /* upratanie */
    znic_pole(&nazvy_suborov);
    thread_data_destroy(&thr_data);

    printf(" (info) Server sa uspesne vypol.\n");
    return 0;
}
