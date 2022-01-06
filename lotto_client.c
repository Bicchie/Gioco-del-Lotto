#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFFER_SIZE 1024

//se riceve 0 nel paramtro stampa tutta la lista dei comandi, sennò solo il comando richiesto
void stampaHelp(int n){
    if(n == 0)
    {
        printf("******************************GIOCO DEL LOTTO ******************************\n");
        printf("Sono disponibili i seguenti comandi:\n");
        printf("1) !help <comando> --> mostra i dettagli di un comando \n");
        printf("2) !signup <username> <password> --> crea un nuovo utente \n");
        printf("3) !login <username> <password> --> autentica un utente \n");
        printf("4) !invia_giocata g --> invia una giocata g al server \n");
        printf("5) !vedi_giocate tipo --> visualizza le giocate precedenti dove tipo = {0,1} \n e permette di visualizzare le giocate passate '0'\n oppure le giocate attive '1' (ancora non estratte \n");
        printf("6) !vedi_estrazione <n> <ruota> --> mostra i numeri sulle ultime n estrazioni \n sulla ruota specificata \n");
        printf("7) !vedi_vincite --> mostra tutte le vincite, l'estrazione in cui sono state realizzate ed un consuntivo \n");
        printf("8) !esci --> termina il client \n");
    }
    if(n == 1)
        printf("!help <comando> --> mostra i dettagli di un comando \n");
    if(n == 2)
        printf("!signup <username> <password> --> crea un nuovo utente \n");
    if(n == 3)
        printf("!login <username> <password> --> autentica un utente \n");
    if(n == 4)
        printf("!invia_giocata g --> invia una giocata g al server \n");
    if(n == 5)
        printf("!vedi_giocate tipo --> visualizza le giocate precedenti dove tipo = {0,1} \n e permette di visualizzare le giocate passate '0'\n oppure le giocate attive '1' (ancora non estratte \n");
    if(n == 6)
        printf("!vedi_estrazione <n> <ruota> --> mostra i numeri sulle ultime n estrazioni \n sulla ruota specificata \n");
    if(n == 7)
        printf("!vedi_vincite --> mostra tutte le vincite, l'estrazione in cui sono state realizzate ed un consuntivo \n");
    if(n == 8)
        printf("!esci --> termina il client \n");
}

// gestisci discorso di avere in input dal terminale porta e ip (semplice)

int main(int argc, char* argv[]){
    int ret, sd, len;
    uint16_t lmsg;
    struct sockaddr_in srv_addr;
    char src[20];
    int porta = 4242;
    
    
    if(argc != 1){
        strcpy(src,argv[1]);
        porta = atoi(argv[2]);
    }
    
    
    /* Creazione socket */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    /* Creazione indirizzo del server */
    memset(&srv_addr, 0, sizeof(srv_addr)); // Pulizia 
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(porta);
    inet_pton(AF_INET, src, &srv_addr.sin_addr);
    
    ret = connect(sd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
    
    if(ret < 0){
        perror("Errore in fase di connessione: \n");
        exit(-1);
    }
    
    //mostro all'utente i comandi disponibili
    stampaHelp(0);
    
    while(1){
        char buffer[BUFFER_SIZE];
        //conterrà tutto l'input digitato dall'utente
        char comandoIntero[BUFFER_SIZE];
        char* token;
        //conterrà il comando da eseguire
        char* comando;
        char session_id[10];
        
        // attendo l'input dell'utente
        fgets(buffer, BUFFER_SIZE, stdin);
     
        strcpy(comandoIntero,buffer);   
        
        //invio l'intero input al server
        len = strlen(buffer) + 1;
        lmsg = htons(len);
        ret = send(sd, (void*) &lmsg, sizeof(uint16_t), 0);
        ret = send(sd, (void*) buffer, len,0);
        
        // estraggo il comando
        comando = strtok(buffer," ");
        
        
        if( (!strcmp(comando,"!help")) || (!strcmp(comando,"!help\n"))){
            //!help senza specificare il comando richiede l'esposizione di tutti i comandi
            token = strtok(NULL," ");
            if(token == NULL){
                stampaHelp(0);
            }
            else{
                //qui invece stampo il manuale del comando richiesto
                if(!strcmp(token,"help\n"))
                stampaHelp(1);
                if(!strcmp(token,"signup\n"))
                    stampaHelp(2);
                if(!strcmp(token,"login\n"))
                    stampaHelp(3);
                if(!strcmp(token,"invia_giocata\n"))
                    stampaHelp(4);
                if(!strcmp(token,"vedi_giocate\n"))
                    stampaHelp(5);
                if(!strcmp(token,"vedi_estrazione\n"))
                    stampaHelp(6);
                if(!strcmp(token,"vedi_vincite\n"))
                    stampaHelp(7);
                if(!strcmp(token,"esci\n"))
                    stampaHelp(8);
            }
        }
        else if(!strcmp(comando, "!esci\n")){
            //ricevo il messaggio dal server
            char appoggio[1024];
            char ricezione[1024];
            ret = recv(sd, (void*)&lmsg, sizeof(uint16_t),0);
            len = ntohs(lmsg);
            ret = recv(sd, (void*)appoggio,len,0);
            if(ret < 0){
                perror("Errore in fase di ricezione: \n");
                exit(-1);
            }
            printf("%s",appoggio);
                    
            //ora aspetto il responso sulla close del server
            //se è avvenuta con successo riceverò 0 sennò riceverò il messaggio di errore
            ret = recv(sd, (void*)&lmsg, sizeof(uint16_t),0);
            if(ret == 0)
                break;
            len = ntohs(lmsg);
            ret = recv(sd, (void*)ricezione,len,0);
            if(ret < 0){
                perror("Errore in fase di ricezione: \n");
                exit(-1);
            }
            //stampo il messaggio di errore ed esco
            printf("%s",ricezione);
            exit(-1);
            
        }
        else if(!strcmp(comando,"!signup")){    
            char appoggio[1024];
            char ricezione[1024];
            // ricevo la risposta del server sull'avvenuta o meno registrazione
            ret = recv(sd, (void*)&lmsg, sizeof(uint16_t), 0);
            len = ntohs(lmsg);
            ret = recv(sd, (void*)appoggio, len, 0);
            if(ret < 0){
                perror("Errore in fase di ricezione: \n");
                exit(-1);
            }
            printf("%s",appoggio);
            
            
            //se è andata a buon fine riceverò registrato e quindi non entrerò nel ciclo
            while(strcmp(appoggio,"Registrato\n")){
                //se è entrato allora l'username esiste già e richiedo un altro username all'utente
                fgets(buffer, BUFFER_SIZE, stdin);
                //invio un altro tentativo di username
                len = strlen(buffer) -1 ; // metto volontariamente il '-1' perché devo togliere il ritorno carrello
                lmsg = htons(len);
                ret = send(sd, (void*) &lmsg, sizeof(uint16_t), 0);
                ret = send(sd, (void*) buffer, len,0);
                if(ret < 0){
                    perror("Errore in fase di invio: \n");
                    exit(-1);
                }
                
                
                //ricevo il responso sul nuovo tentativo
                ret = recv(sd,(void*)&lmsg,sizeof(uint16_t),0);
                len = ntohs(lmsg);
                ret = recv(sd,(void*)ricezione,len,0);
                if(ret < 0){
                    perror("Errore in fase di ricezione: \n");
                    exit(-1);
                }
                printf("%s",ricezione);
            }
                          
        } else if(!strcmp(comando,"!login")){
            char risposta[1024];
            char ricezione[1024];
            //ricevo la risposta dal server sul come è andato il login
            ret = recv(sd,(void*)&lmsg, sizeof(uint16_t),0);
            len = ntohs(lmsg);
            ret = recv(sd,(void*)ricezione,len,0);
            if(ret < 0){
                    perror("Errore in fase di ricezione: \n");
                    exit(-1);
            }
            printf("%s",ricezione);
            
            if(!strcmp(ricezione,"Il tuo IP è stato bannato per un ammontare di tempo uguale o inferiore a 30 minuti\n")){
                continue;
            } else if(!strcmp(ricezione,"Errore nelle credenziali, puoi riprovare\n")){
                continue;
            } else if(!strcmp(ricezione,"Autenticazione avvenuta con successo\n")){
                //ricevo il session id e lo salvo nella omonima variabile
                ret = recv(sd,(void*)&lmsg,sizeof(uint16_t),0);
                len = ntohs(lmsg);
                ret = recv(sd,(void*)risposta,len,0);
                if(ret < 0){
                    perror("Errore in fase di ricezione: \n");
                    exit(-1);
                }
                strncpy(session_id,risposta,10);
                
            }
        } else if(!strcmp(comando,"!invia_giocata")){
            char risposta[1024];
            char ricezione[1024];
            //essendo un comando protetto dal session_id devo inserirlo all'inizio per farlo controllare al server
            strcat(session_id," ");
            strcpy(risposta,session_id);
            strcat(risposta,comandoIntero);
            
            len = strlen(risposta) + 1;
            lmsg = htons(len);
            ret = send(sd,(void*)&lmsg,sizeof(uint16_t),0);
            ret = send(sd,(void*)risposta,len,0);
            if(ret < 0){
                    perror("Errore in fase di invio: \n");
                    exit(-1);
            }
            
            //ricevo il responso
            ret = recv(sd,(void*)&lmsg,sizeof(uint16_t),0);
            len = ntohs(lmsg);
            ret = recv(sd,(void*)ricezione,len,0);
            if(ret < 0){
                    perror("Errore in fase di ricezione: \n");
                    exit(-1);
            }
            
            //mostro il responso all'utente
            printf("%s",ricezione);               
            
        } else if(!strcmp(comando,"!vedi_giocate")){
            char risposta[1024];
            char ricezione[1024];
            //essendo un comando protetto dal session_id devo inserirlo all'inizio per farlo controllare al server
            strcat(session_id," ");
            strcpy(risposta,session_id);
            strcat(risposta,comandoIntero);
            
            //invio il comando con il session_id all'inizio
            len = strlen(risposta) + 1;
            lmsg = htons(len);
            ret = send(sd,(void*)&lmsg,sizeof(uint16_t),0);
            ret = send(sd,(void*)risposta,len,0);
            if(ret < 0){
                    perror("Errore in fase di invio: \n");
                    exit(-1);
            }
            
            //ricevo il responso
            ret = recv(sd,(void*)&lmsg,sizeof(uint16_t),0);
            len = ntohs(lmsg);
            ret = recv(sd,(void*)ricezione,len,0);
            if(ret < 0){
                    perror("Errore in fase di ricezione: \n");
                    exit(-1);
            }
            printf("%s",ricezione);
            
        } else if(!strcmp(comando,"!vedi_vincite\n")){
            char risposta[1024];
            char ricezione[4096];
            //essendo un comando protetto dal session_id devo inserirlo all'inizio per farlo controllare al server
            strcat(session_id," ");
            strcpy(risposta,session_id);
            strcat(risposta,comandoIntero);
            
            //invio il comando con il session_id all'inizio
            len = strlen(risposta) + 1;
            lmsg = htons(len);
            ret = send(sd,(void*)&lmsg,sizeof(uint16_t),0);
            ret = send(sd,(void*)risposta,len,0);
            if(ret < 0){
                    perror("Errore in fase di invio: \n");
                    exit(-1);
            }
            
            //ricevo il responso
            ret = recv(sd,(void*)&lmsg,sizeof(uint16_t),0);
            len = ntohs(lmsg);
            ret = recv(sd,(void*)ricezione,len,0);
            if(ret < 0){
                    perror("Errore in fase di ricezione: \n");
                    exit(-1);
            }
            printf("%s",ricezione);
            
        }else if(!strcmp(comando,"!vedi_estrazione")){
            char risposta[1024];
            char ricezione[4096];
            //essendo un comando protetto dal session_id devo inserirlo all'inizio per farlo controllare al server
            strcat(session_id," ");
            strcpy(risposta,session_id);
            strcat(risposta,comandoIntero);
            
            //invio il comando con il session_id all'inizio
            len = strlen(risposta) + 1;
            lmsg = htons(len);
            ret = send(sd,(void*)&lmsg,sizeof(uint16_t),0);
            ret = send(sd,(void*)risposta,len,0);
            if(ret < 0){
                    perror("Errore in fase di invio: \n");
                    exit(-1);
            }
            
            //ricevo il responso
            ret = recv(sd,(void*)&lmsg,sizeof(uint16_t),0);
            len = ntohs(lmsg);
            ret = recv(sd,(void*)ricezione,len,0);
            if(ret < 0){
                    perror("Errore in fase di ricezione: \n");
                    exit(-1);
            }
            printf("%s",ricezione);
        }
          
    }
    
    close(sd);
    return 0;
}
    
    
    
    
