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
}Race_data; //Versenyadatok strukt�r�ja

typedef struct Racelistelem {
	Race_data race;
	struct Racelistelem *next;
}Racelistelem; //Versenylist�k strukt�r�ja, a f�s�s lista fogai

typedef struct Placelistelem {
	int number;
	char place[20 + 1];
	struct Placelistelem *next;
	Racelistelem *racelist;
}Placelistelem; //Helysz�nlist�k strukt�r�ja, a f�s�s lista gerince

Placelistelem * newplace(int number, char *place) { //�j elem l�trehoz�sa a gerincben
	Placelistelem *new;
	new = (Placelistelem*)malloc(1 * sizeof(Placelistelem));
	new->number = number;
	strcpy(new->place, place);
	new->next = NULL;
	new->racelist = NULL;
	return new;
}

Placelistelem * placeputorder(Placelistelem *head, Placelistelem *new) { //�j elem elhelyez�se a gerincben
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

Placelistelem * placefileread(Placelistelem *head, char *filename) { //Az els� f�jlt beolvas� f�ggv�ny
	FILE *place;
	char c, str[21];
	int num = 0, i = 0;
	place = fopen(filename, "r");
	if (place == NULL) {
		printf("NINCS ILYEN");
		return NULL;
	}
	while (fscanf(place, "%d", &num)==1) {//A verseny sorsz�m�nak beolvas�sa
		fscanf(place, "%c", &c);
		while (fscanf(place, "%c", &c) == 1 && c != '\n') {//A verseny helysz�n�nek beolvas�sa
			str[i] = c;
			i = i + 1;
		}
		str[i] = '\0';
		Placelistelem *new;
		new = newplace(num, str);//�j gerincelem l�trehoz�sa
		head=placeputorder(head, new);// A gerincelem elhelyez�se a list�ban
		num = 0;
		i = 0;
	}
	fclose(place);
	return head; //Val�di listafej visszaad�sa
}

Racelistelem * newraceelem(Race_data newdata) {//�j versenylista elem l�trehoz�sa
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

void raceputorder(Placelistelem *head, Racelistelem *new) { //Az �j versenylista elem elhelyez�se a f�s�s list�ban megkeresve a megfelel� gerincelemet
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

void racefileread(Placelistelem *head, char *filename) {//A versenyf�jl beolvas�sa
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
		printf("NINCS VERSENYF�LJ");
	}
	while (fscanf(racefile, "%d", &race->year) == 1) {//Az �vsz�m beolvas�sa
		printf("\n EV BEOLVASVA");
		fscanf(racefile, "%d", &race->number);//A verseny sorsz�m�nak beolvas�sa
		printf("\n SZAM BEOLVASVA");
		fscanf(racefile, "%d", &race->best_min);//A legjobb id� perceredm�nye
		printf("\n PERC BOLVASVA");
		fscanf(racefile, "%lf", &race->best_sec);//A legjobb id� m�sodperceredm�nye
		printf("\n MASODPERC BEOLVASVA");
		fscanf(racefile, "%c", &c);//a m�sodperc �s a r� k�vetkez� string k�z�tti whitespace �tl�p�se
		while (fscanf(racefile, "%c", &c)==1 && c != ' ') {//A gy�ztes k�dj�nak beolvas�sa
			race->winner[i] = c;
			i = i + 1;
		}
		race->winner[i] = '\0';
		printf("\n WINNER BEOLVASVA");
		i = 0;
		while (fscanf(racefile, "%c", &c)==1 && c != ' ') {//A pol poz�ci�t elnyer� k�dj�nak beolvas�sa
			race->pol[i] = c;
			i = i + 1;
		}
		race->pol[i] = '\0';
		i = 0;
		printf("\n POL BEOLVASVA");
		i = 0;
		while (fscanf(racefile, "%c", &c) == 1 && c != ',') {//A leggyorsabb versenyz� k�dj�nak beolvas�sa
			race->fastest[i] = c;
			i = i + 1;
		}
		printf("\n FASTEST BEOLVASVA");
		Racelistelem *new;
		new = newraceelem(*race);//�j elem l�trehoz�sa
		raceputorder(head, new);//Az elem elhelyez�se a f�s�s list�ban
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
void outwriterace(Racelistelem *head) {//Ellen�rz� f�ggv�ny, amivel ki lehet iratni a versenyist�k elemeit
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
void outwriteplace(Placelistelem* head) {//Ellen�rz� f�ggv�ny, amely minden gerincelemre megh�vja a versenylistaki�r�st
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

Racelistelem** fastest_too(Racelistelem **tomb, int *n) {//A m�sodik, kisebb t�mb�t l�trehoz� f�ggv�ny
	int i, sum = 0;
	for (i = 0; i < *n; i++) {
		if (strcmp(tomb[i]->race.winner, tomb[i]->race.fastest))
			sum = sum + 1;
	}
	Racelistelem **final;
	final = (Racelistelem**)malloc(sum * sizeof(Racelistelem*));
	int finali = 0;
	printf("Az els� t�mb elemsz�ma: %d", *n);
	for (i = 0; i < *n; i++) {
		if (strcmp(tomb[i]->race.winner, tomb[i]->race.fastest)==0) {
			final[finali] = tomb[i];
			finali++;
			printf("SIKER  ");
		}
	}
	free(tomb);
	*n = sum;
	printf("\n A kisebb t�mb elemsz�ma: %d", sum);
	return final;
}
int appliedrace_counter(Placelistelem *head) {//Az els� t�mb l�trehoz�s�hoz kell� f�ggv�ny, amely a t�mb m�ret�t fogja megadni
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
int appliedrace_copy(Racelistelem *head, Racelistelem **tomb, int i, int n) {//Az els� t�mb�t felt�lt� f�ggv�ny
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
void placecopy(Placelistelem *head, Racelistelem **tomb, int *i, int n) {//Az els� t�mb�t felt�lt� f�ggv�ny megh�v�sa minden gerincelemre
	if (head == NULL)
		return; 
	placecopy(head->next, tomb, i, n);
	*i = appliedrace_copy(head->racelist, tomb, *i, n);
}
Racelistelem * search(Placelistelem *head) {//A k�rd�snek megfelel� strukt�ra megkeres�se
	int *n;
	n = (int*)malloc(1 * sizeof(int));
	Racelistelem *final;
	if (n != NULL) {
		*n = appliedrace_counter(head);//Az els� t�mb m�ret�nek megkeres�se
		int *i;
		i = (int*)malloc(1 * sizeof(int));
		if (i != NULL)
			*i = 0;
		Racelistelem **tomb;
		tomb = (Racelistelem**)malloc(*n * sizeof(Racelistelem*));
		placecopy(head, tomb, i, *n);//Az els� t�mb felt�lt�se
		tomb = fastest_too(tomb, n);//Az els� t�mbb�l a m�sodik l�trehoz�sa
		final = tomb[0];
		printf("\n %d", *n);
		for (*i = 0; *i < *n; *i=*i+1) {//A m�sodikt�mbb�l a leggyorsabb megkeres�se
			if (final->race.best_min >= tomb[*i]->race.best_min) {
				if (final->race.best_sec >= tomb[*i]->race.best_sec) {
					final = tomb[*i];
				}
			}
			
		}
		free(n);
		free(i);
		printf("\n felszabad�t�s el�tt");
		free(&tomb[0]);
//Minden, a f�ggv�nyben lefoglalt ter�let felszabad�t�sa
		tomb = NULL;
		n = NULL;
		i = NULL;
		printf("\n Megvan a keresett elem ");
		return final;
	}
	free(n);
	return NULL;
}
void final_print(Racelistelem *final, Placelistelem *head) {//A k�rd�st, �s a v�laszt ki�r� f�ggv�ny
	if (final != NULL) {
		printf("Azok k�z�l a versenyz�k k�z�l, akik legal�bb k�t egym�s ut�ni �vben ugyanazon a p�ly�n rajt-c�l gy�zelmet arattak (mind a pol poz�ci�t, mind a futamot �k nyert�k), ki volt a leggyorsabb azok k�z�l, akik a legjobb id�t is aut�zt�k, �s melyik helysz�nen? ");
		while (final->race.number != head->number) {//A helysz�n ki�r�s�hoz val� keres�s
			head = head->next;
		}
		int i = 0;
		printf("\n A helysz�n: ");
		while (head->place[i] != '\0') {
			printf("%c", head->place[i]);
			i = i + 1;
		}
		printf("\n A keresett versenyz�: ");
		i = 0;
		while (final->race.winner[i] != '\0') {
			printf("%c", final->race.winner[i]);
			i = i + 1;
		}
		printf("\n Az �vsz�m: %d", final->race.year);
	}
}
void deleteracelist(Racelistelem *head) {//A f�s�s lista versenylist�inak a felszabad�t�sa
	if (head == NULL)
		return;
	deleteracelist(head->next);
	free(head);
	head = NULL;
	return;
}
void deleteplacelist(Placelistelem *head) {//A f�s�s lista gerinc�nek a felszabad�t�sa
	if (head == NULL)
		return; 
	deleteplacelist(head->next);
	deleteracelist(head->racelist);//A gerincelem felszabad�t�sa el�tt a hozz� tartoz� versenylista felszabad�t�sa
	free(head);
	head = NULL;
	
}
int main() {
	Placelistelem *head=NULL;
	Racelistelem *final;
	head = placefileread(head, "helyek.txt");//A gerinclista l�trehoz�sa, �s az adatok beolvas�sa a f�jlb�l
	racefileread(head, "verseny.txt"); //A versenylist�k l�trehoz�sa, az adatok beolvas�sa a f�jlb�l �s a list�k elhelyez�se 
	outwriteplace(head); //Ellen�rz� f�ggv�ny a f�s�s lista ki�r�s�hoz
 	final = search(head);//A k�rd�snek megfelel� strukt�ramegkeres�se
	printf("\nvisszat�rt�nk a keresett elemmel");
	final_print(final, head);//Az eredm�nyek ki�r�sa
	deleteplacelist(head); //A f�s�s lista teljes felszabad�t�sa
	getch();
	head = NULL; 
	final = NULL; 
	return 0; 
} 
//it is a test comment for github changes uj
//meg egy komment