#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <stdbool.h>

// FILE:    - estrazione.bin contiene tutte le estrazioni
//          - username_regitro.bin contiene username e password dell'utente username
//          - utenti.bin contiene tutti gli utenti che si sono registrati
//          - registro_ban.txt contiene l'indirizzo ip e il timestamp di chi è stato bannato
//          - usernameV.bin contiene le vittorie dell'utente
//          - usernameG.bin contene le giocate non ancora estratte
//          - usernameGE.bin contiene le giocate estratte

////////////////////////////////GIOCATA//////////////////////////

#define MAX_RUOTE 11
#define MAX_NUMERI 10
#define MAX_IMPORTI 5
#define	DIM_BUFFER 256

struct Giocata {
	bool ruote[MAX_RUOTE] ;	//l'elemento i-esimo del vettore è "true" se la ruota è stata giocata
	int numeri[MAX_NUMERI];
	float importi[MAX_IMPORTI];
	int numeroNumeri;	//cioè quanti numeri ha giocato
};

//mostrerà su 'stringa' la Giocata g
void visualizzaGiocata(struct Giocata g,char* stringa)
{
	int i;
        
        //serve per inizializzare la stringa
        strcpy(stringa,"");
	//visualizzo le ruote scelte
	if(g.ruote[0])
            strcat(stringa,"bari ");
	if(g.ruote[1])
            strcat(stringa,"cagliari ");
	if(g.ruote[2])
            strcat(stringa,"firenze ");
	if(g.ruote[3])
            strcat(stringa,"genova ");
	if(g.ruote[4])
            strcat(stringa,"milano ");
	if(g.ruote[5])
            strcat(stringa,"napoli ");
	if(g.ruote[6])
            strcat(stringa,"palermo ");
	if(g.ruote[7])
            strcat(stringa,"roma ");
	if(g.ruote[8])
            strcat(stringa,"torino ");
	if(g.ruote[9])
            strcat(stringa,"venezia ");
	if(g.ruote[10])
            strcat(stringa,"nazionale ");

	strcat(stringa,"* ");
        
	//visualizzo i numeri scelti
	for(i=0;i<MAX_NUMERI;i++)
	{
        char appoggio[1024];
        if(g.numeri[i]!=0){
		sprintf(appoggio,"%d",g.numeri[i]);
                strcat(stringa,appoggio);
                strcat(stringa," ");
            }    
	}

	strcat(stringa,"* ");
    //visualizzo gli importi per ogni giocata
	for(i=0;i<MAX_IMPORTI;i++)
	{
            char appoggio[1024];
            
            if(g.importi[i] == 0)
                    continue;
            if(i == 0){
                gcvt(g.importi[0],6,appoggio);
                strcat(stringa,appoggio);
                strcat(stringa," estratto ");
            }
            if(i == 1){
                gcvt(g.importi[i],6,appoggio);
                strcat(stringa,appoggio);
                strcat(stringa," ambo ");
            }
            if(i == 2){
                gcvt(g.importi[i],6,appoggio);
                strcat(stringa,appoggio);
                strcat(stringa," terno ");
            }
            if(i == 3){
                gcvt(g.importi[i],6,appoggio);
                strcat(stringa,appoggio);
                strcat(stringa," quaterna ");
            }
            if(i == 4){
                gcvt(g.importi[i],6,appoggio);
                strcat(stringa,appoggio);
                strcat(stringa," cinquina ");
            }
	}
	strcat(stringa,"\n");
}

void inizializzaGiocata(struct Giocata *g)
{
	int i;
	for(i=0;i<MAX_RUOTE;i++)
		g->ruote[i]= false;

	for(i=0;i<MAX_NUMERI;i++)
		g->numeri[i] = 0;

	for(i=0;i<MAX_IMPORTI;i++)
		g->importi[i] = 0;

	g->numeroNumeri=0;
}


///////////////////////////////ESTRAZIONE/////////////////////

struct Estrazione {
	int numeriEstratti[MAX_RUOTE][5];	//per ogni ruota i 5 numeri estratti
	time_t tempoCorrente; 	//ora del'estrazione
};

//ritorna una estrazione di numeri casuali da 1 a 90 senza reinserimento
struct Estrazione estrazione()
{
	struct Estrazione e;
	int i,j,k,a;
	time_t t;
	bool semaforo = true;		

	e.tempoCorrente = time(NULL); //metto il timestamp di quando è avvenuta questa estrazione
	srand((unsigned) time(&t));

	//inizializza la struct estrazione con tutti 0
	for(i=0;i<MAX_RUOTE;i++)
	{
		for(j=0;j<5;j++)
			e.numeriEstratti[i][j] = 0;
	}

	//per ogni ruota
	for(i=0;i<MAX_RUOTE;i++)
	{
		//estraggo 5 numeri casuali senza reinserimento
		for(j=0;j<5;j++)
		{
			while(semaforo)
			{
				a = rand()%90+1;
				if( (a != e.numeriEstratti[i][0]) && (a != e.numeriEstratti[i][1]) && (a != e.numeriEstratti[i][2]) && (a != e.numeriEstratti[i][3]) && (a != e.numeriEstratti[i][4]))
					semaforo = false;	//okei possiamo uscire dal ciclo perché abbiamo estratto un numero inedito	
			}
			e.numeriEstratti[i][j] = a;
			semaforo = true;
		}
	}
	return e;
}

//mostrerà su 'stringa' l'Estrazione e, mostrerà tutte le ruote se ruota = -1, sennò mostrerà la ruota i-esima
void visualizzaEstrazione(struct Estrazione e,char* stringa,int ruota)
{
    //per inizializzare la stringa
    strcpy(stringa,"");
    int i,j;
    
    for(i=0;i<MAX_RUOTE;i++)
    {
        if(i == ruota || ruota == -1){
            if(i==0)
                strcat(stringa,"bari ");
            if(i==1)
                strcat(stringa,"cagliari ");
            if(i==2)
                strcat(stringa,"firenze ");
            if(i==3)
                strcat(stringa,"genova ");
            if(i==4)
                strcat(stringa,"milano ");
            if(i==5)
                strcat(stringa,"napoli ");
            if(i==6)
                strcat(stringa,"palermo ");
            if(i==7)
                strcat(stringa,"roma ");
            if(i==8)
                strcat(stringa,"torino ");
            if(i==9)
                strcat(stringa,"venezia ");
            if(i==10)
                strcat(stringa,"nazionale ");	

            for(j=0;j<5;j++){
                char appoggio[1024];
                sprintf(appoggio,"%d ",e.numeriEstratti[i][j]);
                strcat(stringa,appoggio);
            }
        }
        if(ruota == -1 || i == ruota)
            strcat(stringa,"\n");
    }
    strcat(stringa,"\n");
}

/////////////////////////////VINCITA/////////////////////////
#define vincita1 11.34
#define vincita2 250
#define vincita3 4500
#define vincita4 120000
#define vincita5 6000000 

struct Vincita {
	bool vincente;	//se è true allora la giocata era vincente 
	int numeriVincenti[MAX_RUOTE][MAX_NUMERI];	//per la i esima ruota scrivo i numeri vincenti per quella ruota
	float tipologieVincenti[MAX_RUOTE][MAX_IMPORTI]; //per la i esima ruota scrivo l'ammontare della vincita per ogni tipologia di giocata
	time_t current_time; //ci va copiato l'ora dell'estrazione nella quale ho vinto
};

//mostrerà in 'stringa' la Vincita v
void visualizzaVincita(struct Vincita v,char* stringa)
{
    char s[100];
    int dim;
    time_t t = v.current_time;
    struct tm *tp = localtime (&t);
    int i,j;
    bool ruoteVincenti[11];

    //per inizializzare la stringa
    strcpy(stringa,"");
    strcat(stringa,"Estrazione del ");
 
    //dico l'ora dell'estrazione nella quale ho vinto nel formato richiesto dalle specifiche
    setlocale (LC_ALL, "");
    dim = strftime (s, 100, "%d-%m-%Y ore %H:%M", tp);
    strcat(stringa,s);
    strcat(stringa,"\n");

    //controllo quali ruote sono vincenti
    for(i=0;i<MAX_RUOTE;i++)
    {
            ruoteVincenti[i] = false;
            for(j=0;j<MAX_NUMERI;j++)
            {
                    if(v.numeriVincenti[i][j] != 0)
                    {
                            ruoteVincenti[i] = true;
                            break;
                    }
            }
    }

    for(i=0;i<MAX_RUOTE;i++)
    {
            if(i==0 && ruoteVincenti[i])
                strcat(stringa,"bari ");
            if(i==1 && ruoteVincenti[i])
                strcat(stringa,"cagliari ");
            if(i==2 && ruoteVincenti[i])
                strcat(stringa,"firenze ");
            if(i==3 && ruoteVincenti[i])
                strcat(stringa,"genova ");
            if(i==4 && ruoteVincenti[i])
                strcat(stringa,"milano ");
            if(i==5 && ruoteVincenti[i])
                strcat(stringa,"napoli ");
            if(i==6 && ruoteVincenti[i])
                strcat(stringa,"palermo ");
            if(i==7 && ruoteVincenti[i])
                strcat(stringa,"roma ");
            if(i==8 && ruoteVincenti[i])
                strcat(stringa,"torino ");
            if(i==9 && ruoteVincenti[i])
                strcat(stringa,"venezia ");
            if(i==10 && ruoteVincenti[i])
                strcat(stringa,"nazionale ");
                    
            for(j=0;j<MAX_NUMERI;j++)
            {
                char appoggio[1024];
                if(v.numeriVincenti[i][j]){
                    sprintf(appoggio,"%d ",v.numeriVincenti[i][j]);
                    strcat(stringa,appoggio);
                }
            }

            if(ruoteVincenti[i])
                strcat(stringa,">> ");
           
            for(j=0;j<MAX_IMPORTI;j++)
            {
                char appoggio[1024];
                if(j==0 && v.tipologieVincenti[i][j]!=0){
                    strcat(stringa,"estratto ");
                    gcvt(v.tipologieVincenti[i][j],6,appoggio);
                    strcat(stringa,appoggio);
                    strcat(stringa," ");
                }
                if(j==1 && v.tipologieVincenti[i][j]!=0){
                    strcat(stringa,"ambo ");
                    gcvt(v.tipologieVincenti[i][j],6,appoggio);
                    strcat(stringa,appoggio);
                    strcat(stringa," ");
                }
                if(j==2 && v.tipologieVincenti[i][j]!=0){
                    strcat(stringa,"terno ");
                    gcvt(v.tipologieVincenti[i][j],6,appoggio);
                    strcat(stringa,appoggio);
                    strcat(stringa," ");
                }
                if(j==3 && v.tipologieVincenti[i][j]!=0){
                    strcat(stringa,"quaterna ");
                    gcvt(v.tipologieVincenti[i][j],6,appoggio);
                    strcat(stringa,appoggio);
                    strcat(stringa," ");
                }
                if(j==4 && v.tipologieVincenti[i][j]!=0){
                    strcat(stringa,"cinquina ");
                    gcvt(v.tipologieVincenti[i][j],6,appoggio);
                    strcat(stringa,appoggio);
                    strcat(stringa," ");
                }

            }

            if(ruoteVincenti[i])
                strcat(stringa,"\n");
	}
           
        strcat(stringa,"**************************************************\n");
}


//in v restituisce una vincita con i parametri inizializzati a 0
void inizializzaVincita(struct Vincita *v)
{
	int i,j,k;	
	
        v->vincente = false;
        
	for(i=0;i<MAX_RUOTE;i++)
	{
		for(j=0;j<MAX_NUMERI;j++)
			v->numeriVincenti[i][j] = 0;
		for(k=0;k<MAX_IMPORTI;k++)
			v->tipologieVincenti[i][k] = 0;
	}
}


//ritorna il numero di volte che la giocata è stata vinta, in validaNumero ho i numeri vincenti cioè ho true nell'elemento del vettore corrispondente alla giocata
int controllaVincita(int giocata[], int estrazione[],int tipoGiocata,bool validaNumero[])
{
    int i,j;	
    int numeriAzzeccati=0;

    //conta i numeri azzeccati di giocata nell'estrazione
    for(i=0;i<10;i++)
    {
        for(j=0;j<5;j++)
        {
            if(giocata[i] == estrazione[j])
            {
                validaNumero[i] = true;
                numeriAzzeccati++;
            }
        }
    }

    //se non ho azzeccato nessun numero lo comunico con '0'
    if(numeriAzzeccati == 0)
            return 0;

    //se ho azzeccato dei numeri ma sono minori del tipo giocata allora non ho vinto nulla e azzero il vettore validaNumeri
    if(numeriAzzeccati < tipoGiocata)
    {
            for(i=0;i<10;i++)
                    validaNumero[i] = false;
            return 0;
    }

    //se il tipoGiocata è uguale ai numeri azzeccati allora ho azzeccato 1 giocata soltanto
    if(numeriAzzeccati == tipoGiocata)
            return 1;

    //se sono arrivato qua ho azzeccato più numeri rispetto al tipoGiocata e quindi per sapere quante giocate ho vinto faccio il coefficente binomiale
    return coefficenteBinomiale(numeriAzzeccati,tipoGiocata);

}


int fattoriale(int a)
{
	if(a == 0)
		return 1;
	else
		return a*fattoriale(a-1);
}


int coefficenteBinomiale(int n,int k)
{
	return fattoriale(n)/((fattoriale(k))*fattoriale(n-k));
}


//ricevuta una estrazione ed una giocata restituisce la struct vincita opportunamente riempita avendo confrontato l'estrazione con la giocata
struct Vincita calcolaVincita(struct Estrazione e,struct Giocata g)
{
    //mi dirà quali numeri sono stati estratti fra quelli giocati
    bool validaNumero[10] = {false,false,false,false,false,false,false,false,false,false};
    struct Vincita v;

    //vettore che serve da supporto per prelevare dalla matrice il vettore dei numeri estratti di una determinata ruota
    int vettoreEstrattiSupporto[5];			

    //conterrà il valore delle vincite per ogni giocata
    float premi[MAX_IMPORTI];

    int numeroRuote = 0,i,j,k,a;
    inizializzaVincita(&v);
    
    //ricopio la data e l'ora dell'estrazione e la metto nella Vincita
    v.current_time = e.tempoCorrente;	

    //inizializzo il vettore dei premi
    premi[0] = vincita1;
    premi[1] = vincita2;
    premi[2] = vincita3;
    premi[3] = vincita4;
    premi[4] = vincita5;

    //conto il numero di ruote sulle quali si è giocato
    for(i=0;i<MAX_RUOTE;i++)
    {
        if(g.ruote[i])
            numeroRuote++;
    }

    //divido l'importo di ogni giocata per il numero di ruote su cui è stata fatta la giocata
    for(i=0;i<MAX_IMPORTI;i++)
            g.importi[i] = g.importi[i]/numeroRuote;


    for(i=0;i<MAX_RUOTE;i++)
    {
        //copio l'estrazione sulla ruota i-esima su questo vettore 
        for(k=0;k<5;k++)
            vettoreEstrattiSupporto[k] = e.numeriEstratti[i][k];	

        for(j=0;j<MAX_IMPORTI;j++)
        {
            //se non è stata fatta nessuna giocata vado avanti
            if(g.importi[j] == 0)
                continue;

            //gli do j+1 come tipo giocata avendo esattamente quel valore
            a = controllaVincita(g.numeri,vettoreEstrattiSupporto,j+1,validaNumero);	

            //se almeno una giocata è stata presa allora la giocata è vincente e faccio tutte operazioni
            if(a > 0 && g.ruote[i])	
            {
                v.vincente = true;	

                //utilizzo la validaNumero per inserire nella Vincita i numeriVincenti di questa ruota
                for(k=0;k<MAX_NUMERI;k++)
                {
                    if(validaNumero[k])
                        v.numeriVincenti[i][k] = g.numeri[k];
                }

                //il coefficente binomiale rappresenta il numero di possibili combinazioni del tipo giocata nei numeri che ho giocato
                v.tipologieVincenti[i][j] = g.importi[j]*premi[j]/coefficenteBinomiale(g.numeroNumeri,j+1); 
            }
            //in ogni caso rinizializzo il vettore validaNumero per un nuovo uso
            for(k=0;k<10;k++)
                validaNumero[k] = false;
        }
    }
    return v;
}



struct Utente{
    char username[40];
    char password[40];
};

struct Ban{
    time_t tempoBan;    //indicazione temporale su quando è stato bannato
    char indirizzoIP[32]; //IP del client bannato
};

//crea una stringa alfanumerica di 'len' caratteri casuale
void gen_random(char *s, const int len){
    int i;
    static const char alphanum[] = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    srand(time(0));
    
    for(i=0;i<len;++i)
        s[i] = alphanum[rand()%(sizeof(alphanum) - 1)];
        
    s[len]=0;
}


int main(int argc, char* argv[]){
    int ret, sd, new_sd, len,tentativiFalliti = 0; 
    uint16_t lmsg;
    pid_t pid;
    struct sockaddr_in my_addr, cl_addr;
    int porta = 4242;
    //per default il periodo sono 5 minuti
    int periodoEstrazione = 300;
    
    
    if(argc != 1)
        porta = atoi(argv[1]);
    
    //allora è stato specificato anche il periodo di estrazione
    if(argc > 2)
        periodoEstrazione = atoi(argv[2]);
    
    
    pid = fork();
    
    
    if( pid == 0){
        
        //questo è il processo Estrattore che si occupa anche di controllare che le giocate dei vari utenti siano vincenti o meno
        while(1){
            //faccio l'estrazione
            struct Estrazione e = estrazione();
            FILE* file;
            char nomeFile[1024];
            int m = 1;
            char appoggio[1024];
            
			printf("L'estrazione è stata effettuata\n");

            //inserisco nell'apposito file l'estrazione
            strcpy(nomeFile,"estrazione.bin");
            file = fopen(nomeFile,"ab");
            fwrite(&e,sizeof(struct Estrazione),1,file);
            fclose(file);
                        
            
            file = fopen("utenti.bin","rb");
            //se non esiste ancora il file degli utenti allora non avrebbe senso controllare delle eventuali giocate
            //se invece esiste entro nell'IF sottostante
            if(file != NULL){
                //ora devo controllare per ogni utente se ha delle giocate in sospeso e nel caso metterle tra le vittorie
                while(1){
                    struct Utente utente;
                    FILE* file2;
                    m = fread(&utente,sizeof(struct Utente),1,file);
                    //se sono finiti gli utenti da controllare esco
                    if(m == 0)
                        break;
                    strcpy(nomeFile,utente.username);
                    strcat(nomeFile,"G.bin");
                    file2 = fopen(nomeFile,"rb");
                    //se l'utente non ha ancora effettuato nessuna giocata non faccio nulla
                    if(file2 == NULL)
                        continue;
                    //se invece l'utente ha fatto almeno una giocata controllo
                    while(1){
                        struct Giocata g;
                        struct Vincita v;
                        inizializzaVincita(&v);
                        char appoggio[1024];
                        FILE* file4;
                        
                        m = fread(&g,sizeof(struct Giocata),1,file2);
                        //se sono finite le giocate esco
                        if(m == 0)
                            break;
                        //dato che sto controllando questa giocata la devo aggiungere alfile delle giocate estratte
                        strcpy(nomeFile,utente.username);
                        strcat(nomeFile,"GE.bin");
                        file4 = fopen(nomeFile,"ab");
                        fwrite(&g,sizeof(struct Giocata),1,file4);
                        fclose(file4);
                        
                        
                        //controllo se era una giocata vincente
                        v = calcolaVincita(e,g);
                        //se lo era allora lo scrivo nel file delle vincite dell'utente
                        if(v.vincente){
                            FILE* file3;
                            strcpy(appoggio,utente.username);
                            strcat(appoggio,"V.bin");
                            file3 = fopen(appoggio,"ab");
                            fwrite(&v,sizeof(struct Vincita),1,file3);
                            fclose(file3);
                        } 
                        
                    }
                    fclose(file2);
                    //se arrivo qui ho controllato tutte le giocate quindi posso eliminare il file "usernameG.bin"
                    strcpy(nomeFile,utente.username);
                    strcat(nomeFile,"G.bin");
                    remove(nomeFile);
                }
                fclose(file);
                
            }
            
            //il processo si blocca
            sleep(periodoEstrazione);
        }
               
    } 
    else
    {
        //sono nel processo padre
        /* Creazione socket */
        sd = socket(AF_INET, SOCK_STREAM, 0);
        /* Creazione indirizzo di bind */
        memset(&my_addr, 0, sizeof(my_addr)); // Pulizia 
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(porta);
        my_addr.sin_addr.s_addr = INADDR_ANY;

        printf("Il socket di ascolto è stato creato correttamente\n");
        
        ret = bind(sd, (struct sockaddr*)&my_addr, sizeof(my_addr) );
        ret = listen(sd, 10);
        if(ret < 0){
            perror("Errore in fase di bind: \n");
            exit(-1);
        }

        while(1){
            
            int a = 0,m;
            char app[1024];
            char risposta[1024];
            struct Ban ban;
            struct Utente utenteCorrente;
            FILE* file2;
            len = sizeof(cl_addr);

            // Accetto nuove connessioni
            new_sd = accept(sd, (struct sockaddr*) &cl_addr, &len);


            //controllo il registro dei ban
            strcpy(app,"registro_ban.txt");
            file2 = fopen(app,"r");

            //devo controllare se il suo IP è stato bannato e metto quella condizione
            //nel while perché se non esiste ancora nessun registro_ban vuol dire che non è ancora stato bannato nessuno

            while(file2 != NULL){
                char appoggio[32];
                int len;
                time_t oraCorrente = time(NULL);
                m = fread(&ban,sizeof(struct Ban),1,file2);
                sprintf(appoggio,"%u",cl_addr.sin_addr.s_addr);
                // se l'indirizzo IP coincide e sono passati meno di 1800 secondi allora comunica al client che è stato bannato e che deve aspettare
                if( (!strcmp(ban.indirizzoIP,appoggio))  &&  (difftime(oraCorrente,ban.tempoBan) < 1800 )){ 
                    printf("L'utente è bannato, tentativo di login respinto\n");
                    strcpy(risposta,"Il tuo IP è stato bannato per un ammontare di tempo uguale o inferiore a 30 minuti\n");
                    len = strlen(risposta) + 1;
                    lmsg = htons(len);
                    ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                    ret = send(new_sd,(void*)risposta,len,0);
                    if(ret < 0){
                        perror("Errore in fase di invio:\n");
                        exit(-1);
                    }
                    a = 1; //notifico all'esterno del while che l'utente è stato bannato
                    break;
                }
                //vuol dire che il client non era stato bannato in precedenza
                if(m == 0)
                    break;
            }

            //se il file esisteva va chiuso
            if(file2 != NULL)
                fclose(file2);

            //se l'utente era stato bannato torna alla ricezione connessioni
            if(a == 1)
                continue;

            printf("Nuova connessione accettata\n");
            
            pid = fork();

            if( pid == 0 ){
                //sono nel processo figlio
                int flag;

                close(sd);

                while(1){
                    char buffer[1024];
                    char* comando;
                    char nomeFile[1024]; 
                    char session_id[10];
                    int m;

                    //ricevo il comando
                    ret = recv(new_sd, (void*)&lmsg, sizeof(uint16_t), 0);
					if(ret < 0){
                        perror("Errore in fase di ricezione: \n");
                        continue;
                    }
                    len = ntohs(lmsg); 
                    ret = recv(new_sd, (void*)buffer, len, 0);
                    if(ret < 0){
                        perror("Errore in fase di ricezione: \n");
                        continue;
                    }
                    
                    //estraggo il comando
                    comando = strtok(buffer," ");

                    printf("Comando ricevuto: %s\n",comando);  

                    
                    //tentiamo di fare la close, se ciò non avviene con successo lo comunico al client
                    if(!strcmp(comando,"!esci\n")){
                        char risposta[1024];
                        strcpy(risposta,"Logout eseguito\n");
                        len = strlen(risposta) + 1;
                        lmsg = htons(len);
                        ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                        ret = send(new_sd,(void*)risposta,len,0);
                        if(ret < 0){
                            perror("Errore in fase di invio: \n");
                            exit(-1);
                        }
                        ret = close(new_sd);
                        if(ret < 0){
                            perror("Error: ");
                            strcpy(risposta,"Disconnessione fallita\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                exit(-1);
                            }
                        }
                        printf("Logout eseguito con successo\n");
                        exit(1);
                        
                    } else if(!strcmp(comando,"!signup")){
                        char risposta[1024];
                        struct Utente utente;
                        FILE *file;
                        strcpy(utente.username,strtok(NULL," "));
                        strcpy(utente.password,strtok(NULL," "));

                        //provo ad aprire il file registro dell'utente
                        strcpy(nomeFile,utente.username);
                        strcat(nomeFile,"_registro.bin");
                        file = fopen(nomeFile,"r");

                        //se file è NULL vuol dire che non esisteva il suo file registro e quindi glielo creo
                        if(file == NULL){
                            strcpy(nomeFile,utente.username);
                            strcat(nomeFile,"_registro.bin");
                            file = fopen(nomeFile,"wb");
                            fwrite(&utente,sizeof(struct Utente),1,file);
                            fclose(file);
                        }
                        else{
                            //se invece già esisteva allora lo comunico al client richiedendo un altro username
                            printf("l'username richiesto dall'utente è già esistente\n");
                            fclose(file);
                            strcpy(risposta,"Username esistente\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd, (void*)&lmsg, sizeof(uint16_t),0);
                            ret = send(new_sd, (void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio:\n");
                                continue;
                            }
                            while(1){
                                //ricevo un nuovo tentativo di username
                                struct Utente u;
                                ret = recv(new_sd, (void*)&lmsg,sizeof(uint16_t),0);
                                len = ntohs(lmsg);
                                ret = recv(new_sd,(void*)u.username,len,0);
                                if(ret<0){
                                    perror("Errore in fase di ricezione: \n");
                                    continue;
                                }
                                u.username[len] ='\0';  //serve ad eliminare il \n della fgets nel client
                                strcpy(nomeFile,u.username);
                                strcat(nomeFile,"_registro.bin");
                                file = fopen(nomeFile,"r"); //penso sia uguale ma sarebbe più corretto mettere "rb", quindi prova e vedi
                                if(file == NULL){
                                    //creo il nuovo file registro
                                    strcpy(nomeFile,u.username);
                                    strcat(nomeFile,"_registro.bin");
                                    file = fopen(nomeFile,"wb");
                                    fwrite(&utente,sizeof(struct Utente),1,file);
                                    fclose(file);
                                    break;
                                }
                                else{
                                    printf("L'username richiesto dall'utente è già esistente\n");
                                    fclose(file);
                                    strcpy(risposta,"Username esistente\n");
                                    len = strlen(risposta)+1;
                                    lmsg = htons(len);
                                    ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                                    ret = send(new_sd,(void*)risposta,len,0);
                                    if(ret<0){
                                        perror("Errore in fase di invio:\n");
                                        continue;
                                    }
                                    continue;
                                }

                            }

                        }

                        //aggiungo l'utente al file che racchiude tutti gli utenti
                        strcpy(nomeFile,"utenti.bin");
                        file = fopen(nomeFile,"ab");
                        fwrite(&utente,sizeof(struct Utente),1,file);
                        fclose(file);

                        //se sono uscito dal while vuol dire che ho creato il file registro all'utente al quale comunico l'avvenuta registrazione
                        strcpy(risposta,"Registrato\n");
                        len = strlen(risposta)+1;
                        lmsg = htons(len);
                        ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                        ret = send(new_sd,(void*)risposta,len,0);
                        if(ret < 0){
                            perror("Errore in fase di invio:\n");
                            continue;
                        }
                        printf("Utente registrato\n");
                        continue;

                    } else if(!strcmp(comando,"!login")){
                        char risposta[1024];
                        struct Utente utente1;
                        struct Utente utente2;
                        struct Ban ban;
                        FILE *file;
                        FILE *file2;
                        FILE *file3;
                        char appoggio[1024];
                        int a = 0; //serve per capire se l'utente è stato bannato o meno
                        strcpy(utente1.username,strtok(NULL," "));
                        strcpy(utente1.password,strtok(NULL," "));

                        //provo ad aprire il file registro dell'utente
                        strcpy(nomeFile,utente1.username);
                        strcat(nomeFile,"_registro.bin");
                        file = fopen(nomeFile,"rb");


                        //se file è NULL vuol dire che non esisteva il suo file registro e quindi non esiste un utente con quello username
                        if(file == NULL){
                            printf("Lo username inserito dall'utente non esiste, tentativo respinto\n");
                            tentativiFalliti++;

                            if(tentativiFalliti >= 3)
                                strcpy(risposta,"Il tuo IP è stato bannato per un ammontare di tempo uguale o inferiore a 30 minuti\n");
                            else
                                strcpy(risposta,"Errore nelle credenziali, puoi riprovare\n");

                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio:\n");
                                continue;
                            }
                        } 
                        else 
                        {
                            //se è qui allora il suo file registro esisteva e controllo la password
                            fread(&utente2,sizeof(struct Utente),1,file);
                            fclose(file);
                            //se entra in questo if l'utente ha sbagliato la password
                            if(strcmp(utente2.password,utente1.password) != 0){
                                printf("La password inserita dall'utente è errata, tentativo respinto\n");
                                tentativiFalliti++;

                                if(tentativiFalliti >= 3)
                                    strcpy(risposta,"Il tuo IP è stato bannato per un ammontare di tempo uguale o inferiore a 30 minuti\n");
                                else    
                                    strcpy(risposta,"Errore nelle credenziali, puoi riprovare\n");

                                len = strlen(risposta) + 1;
                                lmsg = htons(len);
                                ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                                ret = send(new_sd,(void*)risposta,len,0);
                                if(ret < 0){
                                    perror("Errore in fase di invio:\n");
                                    continue;
                                }
                            }    
                            else{
                                //se ha azzeccato la password allora si è autenticato e invio il session_id
                                strcpy(risposta,"Autenticazione avvenuta con successo\n");
                                //ricordo i dati dell'utente in utenteCorrente
                                strcpy(utenteCorrente.password,utente1.password);
                                strcpy(utenteCorrente.username,utente1.username);

                                len = strlen(risposta)+1;
                                lmsg = htons(len);
                                ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                                ret = send(new_sd,(void*)risposta,len,0);
                                if(ret < 0){
                                    perror("Errore in fase di invio:\n");
                                    continue;
                                }
                                printf("Utente %s autenticato con successo\n",utente1.username);
                                //genero il session_id
                                gen_random(session_id, 10);
                                //lo invio al client
                                len = strlen(session_id);
                                lmsg = htons(len);
                                ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                                ret = send(new_sd,(void*)session_id,len,0);
                                if(ret < 0){
                                    perror("Errore in fase di invio: \n");
                                    continue;
                                }
                                continue;
                            }

                        }

                        if(tentativiFalliti >= 3){
                            char appoggio[32];
                            //ban.indirizzoIP = cl_addr.sin_addr.s_addr;
                            sprintf(appoggio,"%d",cl_addr.sin_addr.s_addr);
                            strcpy(ban.indirizzoIP,appoggio);
                            ban.tempoBan = time(NULL);
                            strcpy(nomeFile,"registro_ban.txt");
                            // 'a' poiché nel caso non ci fosse si crea, se c'è si aggiunge in fondo al file
                            file3 = fopen(nomeFile,"a");
                            fwrite(&ban,sizeof(struct Ban),1,file3);
                            fclose(file3);
                            //chiudi la connessione
                            break;
                        }



                    } else if(!strcmp(comando,"!invia_giocata")){
                        char risposta[1024];
                        char* token;
                        FILE* file;
                        struct Giocata g;
                        int i;
                        inizializzaGiocata(&g);
                        //ricevo il comando ma con il session_id
                        ret = recv(new_sd, (void*)&lmsg,sizeof(uint16_t),0);
                        len = ntohs(lmsg);
                        ret = recv(new_sd,(void*)buffer,len,0);
                        if(ret < 0){
                            perror("Errore in fase di ricezione: \n");
                            continue;
                        }
                        
                        //estraggo il session_id
                        token = strtok(buffer," ");

                        //se il session_id non è valido lo comunico al client
                        //confronto i primi 10 caratteri poiché il session_id ne ha 10
                        if(strncmp(token,session_id,10)){
                            printf("Comando tentato senza aver effettuato il login\n");
                            strcpy(risposta,"Non puoi richiedere questo comando senza aver effettuato il login\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                    perror("Errore in fase di invio: \n");
                                    continue;
                                }
                            continue;
                        }

                        //ricevo la giocata e la inserisco nell'apposita struct

                        //ignoro il comando
                        token = strtok(NULL," ");

                        //ignoro '-r' mettendolo in token
                        token = strtok(NULL," "); 

                        for(;strcmp(token,"-n");)
                        {
                                token = strtok(NULL," ");	//pesco la stringa

                                if(!strcmp(token,"tutte"))	//aggiorno di conseguenza la giocata con tutte le ruote
                                {
                                        for(i=0;i<11;i++)
                                                g.ruote[i] = true;
                                }
                                if(!strcmp(token,"bari"))
                                        g.ruote[0] = true;
                                if(!strcmp(token,"cagliari"))
                                        g.ruote[1] = true;
                                if(!strcmp(token,"firenze"))
                                        g.ruote[2] = true;
                                if(!strcmp(token,"genova"))
                                        g.ruote[3] = true;
                                if(!strcmp(token,"milano"))
                                        g.ruote[4] = true;
                                if(!strcmp(token,"napoli"))
                                        g.ruote[5] = true;
                                if(!strcmp(token,"palermo"))
                                        g.ruote[6] = true;
                                if(!strcmp(token,"roma"))
                                        g.ruote[7] = true;
                                if(!strcmp(token,"torino"))
                                        g.ruote[8] = true;
                                if(!strcmp(token,"venezia"))
                                        g.ruote[9] = true;
                                if(!strcmp(token,"nazionale"))
                                        g.ruote[10] = true;
                        }

                        token = strtok(NULL," ");	// prendo il primo numero

                        for(i=0;strcmp(token,"-i");i++)
                        {
                                g.numeri[i] = atoi(token);
                                g.numeroNumeri++;
                                token = strtok(NULL," ");
                        }

                        token = strtok(NULL," "); // prendo il primo importo

                        for(i=0;token != NULL;i++)
                        {
                                g.importi[i] = atof(token);
                                token = strtok(NULL," ");
                        }

                        //inserisco la giocata nel file delle giocate dell'utente
                        strcpy(nomeFile,utenteCorrente.username);
                        //così ho usernameG.bin che è il file delle giocate di username
                        strcat(nomeFile,"G.bin");
                        file = fopen(nomeFile,"ab");
                        fwrite(&g,sizeof(struct Giocata),1,file);
                        fclose(file);

                        printf("Giocata effettuata con successo\n");

                        //informo l'utente della giocata
                        strcpy(risposta,"Giocata effettuata con successo\n");
                        len = strlen(risposta) + 1;
                        lmsg = htons(len);
                        ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                        ret = send(new_sd,(void*)risposta,len,0);
                        if(ret < 0){
                            perror("Errore in fase di invio: \n");
                            continue;
                        }

                    } else if(!strcmp(comando,"!vedi_giocate")){
                        char* token;
                        char stringa[1024];
                        char risposta[1024];
                        bool presenzaGiocate;;
                        FILE* file;
                        int i,m;
                        struct Giocata g;

                        //ricevo il comando ma con il session_id
                        ret = recv(new_sd, (void*)&lmsg,sizeof(uint16_t),0);
                        len = ntohs(lmsg);
                        ret = recv(new_sd,(void*)buffer,len,0);
                        if(ret < 0){
                            perror("Errore in fase di ricezione: \n");
                            continue;
                        }
                        
                        //estraggo il session_id
                        token = strtok(buffer," ");

                        //se il session_id non è valido lo comunico al client
                        //confronto i primi 10 caratteri poiché il session_id ne ha 10
                        if(strncmp(token,session_id,10)){
                            printf("Comando tentato senza aver effettuato il login\n");
                            strcpy(risposta,"Non puoi richiedere questo comando senza aver effettuato il login\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                continue;
                            }
                            continue;
                        }

                        //ignoro il comando
                        token = strtok(NULL," ");

                        //estraggo la modalità con la quale l'utente vuole vedere le giocate
                        token = strtok(NULL," ");
                                                
                        strcpy(nomeFile,utenteCorrente.username);
                        
                        //apro il file delle giocate estratte o delle gioate attive a seconda della richiesta dell'utente
                        if(atoi(token) == 0)
                            strcat(nomeFile,"GE.bin");
                        else
                            strcat(nomeFile,"G.bin");

                        file = fopen(nomeFile,"rb");
                        //se il file non esisteva non ho nessuna giocata da mostrare all'utente
                        if(file == NULL)
                            presenzaGiocate = false;
                        else
                            presenzaGiocate = true;


                        //serve per inizializzare stringa
                        strcpy(stringa,"");

                        //scorro tutte le giocate che ha fatto e le farò visualizzare
                        //al client solo se rispettano la condizione da lui richiesta
                        //cioè estratte o meno, informazione che sta in "visualizzaEstratte"
                        //utilizzo come condizione la presenzaGiocate così se non sono presenti
                        //non entra nel ciclo
                        
                        //bisogna cambiare cioè a seconda della modalità scelta dall'utente devo vedere il file giocata coerente!!
                        
                        for(i = 1;presenzaGiocate;){
                            m = fread(&g,sizeof(struct Giocata),1,file);
                            if(m == 0)
                                break;
                            
                            char appoggio[1024];
                            char appoggio2[1024];
                            sprintf(appoggio,"%d)",i);
                            strcat(stringa,appoggio);
                            visualizzaGiocata(g,appoggio2);
                            strcat(stringa,appoggio2);
                            i++;
                        }

                        
                        //do il responso al client a seconda se fossero presenti o meno giocate
                        if(presenzaGiocate){
                            printf("Visualizzazione giocate effettuata con successo\n");
                            fclose(file);
                            len = strlen(stringa);
                            //sotto è in prova
                            //per terminare la stringa
                            stringa[len] = '\0';
                            len++;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)stringa,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                continue;
                            }
                        }else{
                            printf("Nessuna giocata da mostrare\n");
                            strcpy(risposta,"Nessuna giocata da mostrare\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                continue;
                            }
                        }


                    } else if(!strcmp(comando,"!vedi_vincite\n")){
                        char* token;
                        char stringa[4096];
                        char risposta[1024];
                        char app[1024];
                        FILE* file;
                        int i,j,m;
                        struct Vincita v;
                        float vincite[MAX_IMPORTI] = {0,0,0,0,0};

                        //ricevo il comando ma con il session_id
                        ret = recv(new_sd, (void*)&lmsg,sizeof(uint16_t),0);
                        len = ntohs(lmsg);
                        ret = recv(new_sd,(void*)buffer,len,0);
                        if(ret < 0){
                            perror("Errore in fase di ricezione: \n");
                            continue;
                        }
                        
                        //estraggo il session_id
                        token = strtok(buffer," ");

                        //se il session_id non è valido lo comunico al client
                        //confronto i primi 10 caratteri poiché il session_id ne ha 10
                        if(strncmp(token,session_id,10)){
                            printf("Comando tentato senza aver effettuato il login\n");
                            strcpy(risposta,"Non puoi richiedere questo comando senza aver effettuato il login\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                continue;
                            }
                            continue;
                        }
                        
                        strcpy(nomeFile,utenteCorrente.username);
                        strcat(nomeFile,"V.bin");
                        file = fopen(nomeFile,"rb");
                        //se il file non c'è vuol dire che l'utente non ha ancora vinto nulla
                        if(file == NULL){
                            printf("Nessuna vincita da mostrare all'utente\n");
                            strcpy(risposta,"Nessuna vincita da mostrare\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                continue;
                            }
                            continue;
                        }
                        
                        
                        //se invece esiste allora prelevo le vincite e le invio al client
                        while(1){
                            m = fread(&v,sizeof(struct Vincita),1,file);
                            if(m == 0)
                                break;
                            //per fare un consuntivo delle vincite per tipologia di giocata 
                            for(i=0; i<MAX_RUOTE; i++){
                                for(j=0;j<MAX_IMPORTI;j++){
                                    vincite[j] += v.tipologieVincenti[i][j];
                                }
                            }
                            char appoggio[1024];
                            visualizzaVincita(v,appoggio);
                            strcat(stringa,appoggio);
                        }
                        
                        fclose(file);
                        
                        strcat(stringa,"Vincite su ESTRATTO: ");
                        gcvt(vincite[0],6,app);
                        strcat(stringa,app);
                        strcat(stringa,"\n");
                        
                        strcat(stringa,"Vincite su AMBO: ");
                        gcvt(vincite[1],6,app);
                        strcat(stringa,app);
                        strcat(stringa,"\n");
                        
                        strcat(stringa,"Vincite su TERNO: ");
                        gcvt(vincite[2],6,app);
                        strcat(stringa,app);
                        strcat(stringa,"\n");
                        
                        strcat(stringa,"Vincite su QUATERNA: ");
                        gcvt(vincite[3],6,app);
                        strcat(stringa,app);
                        strcat(stringa,"\n");
                        
                        strcat(stringa,"Vincite su CINQUINA: ");
                        gcvt(vincite[4],6,app);
                        strcat(stringa,app);
                        strcat(stringa,"\n");
                        
                        printf("Visualizzazione vincite effettuata con successo\n");
                        len = strlen(stringa);
                        //per terminare la stringa
                        stringa[len] = '\0';
                        len++;
                        lmsg = htons(len);
                        ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                        ret = send(new_sd,(void*)stringa,len,0);
                        if(ret < 0){
                            perror("Errore in fase di invio: \n");
                            continue;
                        }
                        
                    }  else if( (!strcmp(comando,"!vedi_estrazione"))){
                        char* token;
                        char stringa[4096];
                        char risposta[1024];
                        FILE* file;
                        int m;
                        struct Estrazione e;
                        //conterrà il numero di ultime estrazioni richieste dall'utente
                        int ultime;
                        //conterrà la ruota sulla quale vedere l'estrazione richiesta dall'utente, se tutte sarà -1
                        int ruota;
                        //conterrà il numero delle estrazioni presenti sul file "estrazioni.bin"
                        int numeroEstrazioni = 0;
                        int contatore = 0;
                        
                        //ricevo il comando ma con il session_id
                        ret = recv(new_sd, (void*)&lmsg,sizeof(uint16_t),0);
                        len = ntohs(lmsg);
                        ret = recv(new_sd,(void*)buffer,len,0);
                        if(ret < 0){
                            perror("Errore in fase di ricezione: \n");
                            continue;
                        }
                        
                        //estraggo il session_id
                        token = strtok(buffer," ");

                        //se il session_id non è valido lo comunico al client
                        //confronto i primi 10 caratteri poiché il session_id ne ha 10
                        if(strncmp(token,session_id,10)){
                            printf("Comando tentato senza aver effettuato il login\n");
                            strcpy(risposta,"Non puoi richiedere questo comando senza aver effettuato il login\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                continue;
                            }
                            continue;
                        }
                        
                        //ignoro il comando
                        token = strtok(NULL," ");

                        //estraggo il numero di ultime estrazioni che vuole vedere l'utente
                        token = strtok(NULL," ");
                        ultime = atoi(token);
                        
                        token = strtok(NULL," ");
                        if(token == NULL)
                            ruota = -1;
                        else
                        {
                            if(!strcmp(token,"bari\n"))
                                ruota = 0;
                            if(!strcmp(token,"cagliari\n"))
                                ruota = 1;
                            if(!strcmp(token,"firenze\n"))
                                ruota = 2;
                            if(!strcmp(token,"genova\n"))
                                ruota = 3;
                            if(!strcmp(token,"milano\n"))
                                ruota = 4;
                            if(!strcmp(token,"napoli\n"))
                                ruota = 5;
                            if(!strcmp(token,"palermo\n"))
                                ruota = 6;
                            if(!strcmp(token,"roma\n"))
                                ruota = 7;
                            if(!strcmp(token,"torino\n"))
                                ruota = 8;
                            if(!strcmp(token,"venezia\n"))
                                ruota = 9;
                            if(!strcmp(token,"nazionale\n"))
                                ruota = 10;
                        }
                        
                        
                        strcpy(nomeFile,"estrazione.bin");
                        file = fopen(nomeFile,"rb");
                        if(file == NULL){
                            printf("Nessuna estrazione da mostrare al client\n");
                            strcpy(risposta,"Nessuna estrazione da mostrare\n");
                            len = strlen(risposta) + 1;
                            lmsg = htons(len);
                            ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                            ret = send(new_sd,(void*)risposta,len,0);
                            if(ret < 0){
                                perror("Errore in fase di invio: \n");
                                continue;
                            }
                            continue;
                        }
                        
                        //conto quante sono in tutto l'estrazioni per far vedere successivamente solo le ultime
                        while(1){
                            m = fread(&e,sizeof(struct Estrazione),1,file);
                            if(m == 0)
                                break;
                            numeroEstrazioni++;
                        }
                        fclose(file);
                        
                        
                        //se l'utente ha chiesto di vedere più estrazioni di quelle effettivamente presenti 
                        //verranno mostrate tutte le estrazioni presenti
                        if(ultime > numeroEstrazioni)
                            ultime = numeroEstrazioni;
                            
                        
                        file=fopen(nomeFile,"rb");
                        strcpy(stringa,"");
                        while(1){
                            m = fread(&e,sizeof(struct Estrazione),1,file);
                            if(m == 0)
                                break;
                            if( (numeroEstrazioni - ultime) <= contatore ){
                                char appoggio[1024];
                                visualizzaEstrazione(e,appoggio,ruota);
                                strcat(stringa,appoggio);
                            }
                            contatore++;
                        }
                        
                        fclose(file);
                        
                        printf("Visualizzazione estrazione effettuata con successo\n");
                        len = strlen(stringa);
                        //per terminare la stringa
                        stringa[len] = '\0';
                        
                        len++;
                        lmsg = htons(len);
                        ret = send(new_sd,(void*)&lmsg,sizeof(uint16_t),0);
                        ret = send(new_sd,(void*)stringa,len,0);
                        if(ret < 0){
                            perror("Errore in fase di invio: \n");
                            continue;
                        }
                        
                    }
                    
                    
                }

                close(new_sd);
                
                exit(1);
            } 
            else 
            {
                // Processo padre

                close(new_sd);
            }
        }
    }  
}
    
    
    
    
