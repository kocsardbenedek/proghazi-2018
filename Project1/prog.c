#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
typedef struct {
	int year;
	int number;
	char winner[3 + 1];
	char pol[3 + 1];
	int best_min;
	double best_sec;
	char fastest[3 + 1];
}Race_data; //Versenyadatok struktúrája

typedef struct Racelistelem {
	Race_data race;
	struct Racelistelem *next;
}Racelistelem; //Versenylisták struktúrája, a fésûs lista fogai

typedef struct Placelistelem {
	int number;
	char place[20 + 1];
	struct Placelistelem *next;
	Racelistelem *racelist;
}Placelistelem; //Helyszínlisták struktúrája, a fésûs lista gerince

Placelistelem * newplace(int number, char *place) { //Új elem létrehozása a gerincben
	Placelistelem *new;
	new = (Placelistelem*)malloc(1 * sizeof(Placelistelem));
	new->number = number;
	strcpy(new->place, place);
	new->next = NULL;
	new->racelist = NULL;
	return new;
}

Placelistelem * placeputorder(Placelistelem *head, Placelistelem *new) { //Új elem elhelyezése a gerincben
	Placelistelem *actual, *p;
	actual = head;
	if (actual == NULL)
		return new;
	while (actual->next != NULL &&  new->number > actual->next->number){
		actual = actual->next;
	}
	p = actual->next;
	actual->next = new;
	new->next = p;
	return head;
}

Placelistelem * placefileread(Placelistelem *head, char *filename) { //Az elsõ fájlt beolvasó függvény
	FILE *place;
	char c, str[21];
	int num = 0, i = 0;
	place = fopen(filename, "r");
	if (place == NULL) {
		printf("NINCS ILYEN");
		return NULL;
	}
	while (fscanf(place, "%d", &num)==1) {//A verseny sorszámának beolvasása
		fscanf(place, "%c", &c);
		while (fscanf(place, "%c", &c) == 1 && c != '\n') {//A verseny helyszínének beolvasása
			str[i] = c;
			i = i + 1;
		}
		str[i] = '\0';
		Placelistelem *new;
		new = newplace(num, str);//Új gerincelem létrehozása
		head=placeputorder(head, new);// A gerincelem elhelyezése a listában
		num = 0;
		i = 0;
	}
	fclose(place);
	return head; //Valódi listafej visszaadása
}

Racelistelem * newraceelem(Race_data newdata) {//Új versenylista elem létrehozása
	Racelistelem *new;
	new = (Racelistelem*)malloc(1 * sizeof(Racelistelem));
	new->race.best_min = newdata.best_min;
	new->race.best_sec = newdata.best_sec;
	new->race.year = newdata.year;
	new->race.number = newdata.number;
	strcpy(new->race.fastest, newdata.fastest);
	strcpy(new->race.pol, newdata.pol);
	strcpy(new->race.winner, newdata.winner);
	new->next = NULL;
	return new;
}

void raceputorder(Placelistelem *head, Racelistelem *new) { //Az új versenylista elem elhelyezése a fésûs listában megkeresve a megfelelõ gerincelemet
	Racelistelem *actual;
	Placelistelem *actplace = head;
	while (new->race.number != actplace->number && actplace->next!=NULL) {
		actplace = actplace->next;
	}
	actual = actplace->racelist;
	if (actual == NULL) {
		actplace->racelist = new;
		return;
	}
	while (actual->race.year > new->race.year || actual->next != NULL)
		actual = actual->next;
	actual->next = new;
}

void racefileread(Placelistelem *head, char *filename) {//A versenyfájl beolvasása
	FILE *racefile;
	Race_data *race;
	race = (Race_data*)malloc(1 * sizeof(Race_data));
	race->year = 0;
	race->number = 0;
	race->best_min = 0;
	race->best_sec = 0.0;
	int i = 0; 
	char c;
	racefile = fopen(filename, "r");
	if (racefile == NULL){
		printf("NINCS VERSENYFÁLJ");
	}
	while (fscanf(racefile, "%d", &race->year) == 1) {//Az évszám beolvasása
		printf("\n EV BEOLVASVA");
		fscanf(racefile, "%d", &race->number);//A verseny sorszámának beolvasása
		printf("\n SZAM BEOLVASVA");
		fscanf(racefile, "%d", &race->best_min);//A legjobb idõ perceredménye
		printf("\n PERC BOLVASVA");
		fscanf(racefile, "%lf", &race->best_sec);//A legjobb idõ másodperceredménye
		printf("\n MASODPERC BEOLVASVA");
		fscanf(racefile, "%c", &c);//a másodperc és a rá következõ string közötti whitespace átlépése
		while (fscanf(racefile, "%c", &c)==1 && c != ' ') {//A gyõztes kódjának beolvasása
			race->winner[i] = c;
			i = i + 1;
		}
		race->winner[i] = '\0';
		printf("\n WINNER BEOLVASVA");
		i = 0;
		while (fscanf(racefile, "%c", &c)==1 && c != ' ') {//A pol pozíciót elnyerõ kódjának beolvasása
			race->pol[i] = c;
			i = i + 1;
		}
		race->pol[i] = '\0';
		i = 0;
		printf("\n POL BEOLVASVA");
		i = 0;
		while (fscanf(racefile, "%c", &c) == 1 && c != ',') {//A leggyorsabb versenyzõ kódjának beolvasása
			race->fastest[i] = c;
			i = i + 1;
		}
		printf("\n FASTEST BEOLVASVA");
		Racelistelem *new;
		new = newraceelem(*race);//új elem létrehozása
		raceputorder(head, new);//Az elem elhelyezése a fésûs listában
		race->year = 0;
		race->number = 0;
		race->best_min = 0;
		race->best_sec = 0;
		i = 0;
		printf("%d %d ", new->race.number, new->race.year);
		for (i = 0; new->race.winner[i] != '\0'; i++)
			printf("%c", new->race.winner[i]);
		printf(" ");
		i = 0;
		while (new->race.pol[i] != '\0') {
			printf("%c", new->race.pol[i]);
			i = i++;
		}
		printf(" ");
		for (i = 0; new->race.fastest[i] != '\0'; i++)
			printf("%c", new->race.fastest[i]);
		printf(" %d %lf \n", new->race.best_min, new->race.best_sec);
	}
	free(race);
	fclose(racefile);
	return;
}
void outwriterace(Racelistelem *head) {//Ellenõrzõ függvény, amivel ki lehet iratni a versenyisták elemeit
	if (head == NULL)
		return;
	outwriterace(head->next);
	printf("%d %d ", head->race.number, head->race.year);
	int i;
	for (i = 0; head->race.winner[i] != '\0'; i++)
		printf("%c", head->race.winner[i]);
	printf(" ");
	i = 0;
	while (head->race.pol[i] != '\0') {
		printf("%c", head->race.pol[i]);
		i = i++;
	}
	printf(" ");
	for (i = 0; head->race.fastest[i] != '\0'; i++)
		printf("%c", head->race.fastest[i]);
	printf(" %d %lf \n",head->race.best_min, head->race.best_sec);
}
void outwriteplace(Placelistelem* head) {//Ellenõrzõ függvény, amely minden gerincelemre meghívja a versenylistakiírást
	if (head == NULL)
		return;
	outwriteplace(head->next);
	printf("%d", head->number);
	int i;
	for (i = 0; head->place[i] != '\0'; i++)
		printf("%c", head->place[i]);
	printf("\n");
	outwriterace(head->racelist);
}

Racelistelem** fastest_too(Racelistelem **tomb, int *n) {//A második, kisebb tömböt létrehozó függvény
	int i, sum = 0;
	for (i = 0; i < *n; i++) {
		if (strcmp(tomb[i]->race.winner, tomb[i]->race.fastest))
			sum = sum + 1;
	}
	Racelistelem **final;
	final = (Racelistelem**)malloc(sum * sizeof(Racelistelem*));
	int finali = 0;
	printf("Az elsõ tömb elemszáma: %d", *n);
	for (i = 0; i < *n; i++) {
		if (strcmp(tomb[i]->race.winner, tomb[i]->race.fastest)==0) {
			final[finali] = tomb[i];
			finali++;
			printf("SIKER  ");
		}
	}
	free(tomb);
	*n = sum;
	printf("\n A kisebb tömb elemszáma: %d", sum);
	return final;
}
int appliedrace_counter(Placelistelem *head) {//Az elsõ tömb létrehozásához kellõ függvény, amely a tömb méretét fogja megadni
	Placelistelem *actual;
	Racelistelem *act_race;
	int sum = 0;
	for (actual = head; actual != NULL; actual = actual->next) {
		for (act_race = actual->racelist; act_race->next != NULL; act_race = act_race->next) {
			if (strcmp(act_race->race.winner, act_race->next->race.winner) == 0) {
				if (strcmp(act_race->race.pol, act_race->next->race.pol) == 0) {
					if ((act_race->next->race.year - act_race->race.year) <= 1) {
						if (strcmp(act_race->race.pol, act_race->race.winner) == 0)
							sum = sum + 2;
					}
				}
			}
		}
	}
	return sum;
}
int appliedrace_copy(Racelistelem *head, Racelistelem **tomb, int i, int n) {//Az elsõ tömböt feltöltõ függvény
	Racelistelem *act_race;
	if (i < n) {
		for (act_race = head; act_race->next != NULL; act_race = act_race->next) {
			if (strcmp(act_race->race.winner, act_race->next->race.winner) == 0) {
				if (strcmp(act_race->race.pol, act_race->next->race.pol) == 0) {
					if ((act_race->next->race.year - act_race->race.year) <= 1) {
						if (strcmp(act_race->race.pol, act_race->race.winner) == 0) {
							tomb[i] = act_race;
							tomb[i + 1] = act_race->next;
							i = i + 2;
						}
					}
				}
			}
		}
	}
	return i;
}
void placecopy(Placelistelem *head, Racelistelem **tomb, int *i, int n) {//Az elsõ tömböt feltöltõ függvény meghívása minden gerincelemre
	if (head == NULL)
		return; 
	placecopy(head->next, tomb, i, n);
	*i = appliedrace_copy(head->racelist, tomb, *i, n);
}
Racelistelem * search(Placelistelem *head) {//A kérdésnek megfelelõ struktúra megkeresése
	int *n;
	n = (int*)malloc(1 * sizeof(int));
	Racelistelem *final;
	if (n != NULL) {
		*n = appliedrace_counter(head);//Az elsõ tömb méretének megkeresése
		int *i;
		i = (int*)malloc(1 * sizeof(int));
		if (i != NULL)
			*i = 0;
		Racelistelem **tomb;
		tomb = (Racelistelem**)malloc(*n * sizeof(Racelistelem*));
		placecopy(head, tomb, i, *n);//Az elsõ tömb feltöltése
		tomb = fastest_too(tomb, n);//Az elsõ tömbbõl a második létrehozása
		final = tomb[0];
		printf("\n %d", *n);
		for (*i = 0; *i < *n; *i=*i+1) {//A másodiktömbbõl a leggyorsabb megkeresése
			if (final->race.best_min >= tomb[*i]->race.best_min) {
				if (final->race.best_sec >= tomb[*i]->race.best_sec) {
					final = tomb[*i];
				}
			}
			
		}
		free(n);
		free(i);
		printf("\n felszabadítás elõtt");
		free(&tomb[0]);
//Minden, a függvényben lefoglalt terület felszabadítása
		tomb = NULL;
		n = NULL;
		i = NULL;
		printf("\n Megvan a keresett elem ");
		return final;
	}
	free(n);
	return NULL;
}
void final_print(Racelistelem *final, Placelistelem *head) {//A kérdést, és a választ kiíró függvény
	if (final != NULL) {
		printf("Azok közül a versenyzõk közül, akik legalább két egymás utáni évben ugyanazon a pályán rajt-cél gyõzelmet arattak (mind a pol pozíciót, mind a futamot õk nyerték), ki volt a leggyorsabb azok közül, akik a legjobb idõt is autózták, és melyik helyszínen? ");
		while (final->race.number != head->number) {//A helyszín kiírásához való keresés
			head = head->next;
		}
		int i = 0;
		printf("\n A helyszín: ");
		while (head->place[i] != '\0') {
			printf("%c", head->place[i]);
			i = i + 1;
		}
		printf("\n A keresett versenyzõ: ");
		i = 0;
		while (final->race.winner[i] != '\0') {
			printf("%c", final->race.winner[i]);
			i = i + 1;
		}
		printf("\n Az évszám: %d", final->race.year);
	}
}
void deleteracelist(Racelistelem *head) {//A fésûs lista versenylistáinak a felszabadítása
	if (head == NULL)
		return;
	deleteracelist(head->next);
	free(head);
	head = NULL;
	return;
}
void deleteplacelist(Placelistelem *head) {//A fésûs lista gerincének a felszabadítása
	if (head == NULL)
		return; 
	deleteplacelist(head->next);
	deleteracelist(head->racelist);//A gerincelem felszabadítása elõtt a hozzá tartozó versenylista felszabadítása
	free(head);
	head = NULL;
	
}
int main() {
	Placelistelem *head=NULL;
	Racelistelem *final;
	head = placefileread(head, "helyek.txt");//A gerinclista létrehozása, és az adatok beolvasása a fájlból
	racefileread(head, "verseny.txt"); //A versenylisták létrehozása, az adatok beolvasása a fájlból és a listák elhelyezése 
	outwriteplace(head); //Ellenõrzõ függvény a fésûs lista kiírásához
 	final = search(head);//A kérdésnek megfelelõ struktúramegkeresése
	printf("\nvisszatértünk a keresett elemmel");
	final_print(final, head);//Az eredmények kiírása
	deleteplacelist(head); //A fésûs lista teljes felszabadítása
	getch();
	head = NULL; 
	final = NULL; 
	return 0; 
} 
//it is a test comment for github changes uj
//meg egy komment