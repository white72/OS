#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/*	- Pliki źródłowe w C najlepiej kompilować poleceniem:
*		gcc -O2 -W -Wall -pedantic -std=c99 -o main.o main.c
*		zapobiega to błędowi kompilacji "outside C99 mode"
*		i włącza ostrzeżenia widoczne w terminalu.
**/



/*	#define nazwa zamień_na - sposób działania define == zamieni wszystkie fragmentu kodu "nazwa" na "zmień_na"	
*	W naszym przypadku fragment kodu "MAX" zostanie zastąpiony przez 16384
**/

#define MAX 16384 //Rozmiar naszego bufora(bajty)

void showTab(char[], int);
/*
 * +=====================+
 * |#####C###|####C++####|
 * |  _Bool ==   bool    |
 * +---------------------+
 *
 * */
_Bool isChar(char[], int);


/* Do zrozumienia działania (int argc, char* argv[]) będziemy musieli wiedzieć jak działają tablice i wskaźniki w C. (Bardzo pomocny artykuł na wiki https://goo.gl/6PohAA)
 *
 *
 *		1.) Wyobraźmy sobie, że pamięć którą dysponujemy jest jak jednowymiarowa oś, zbudowana z komórek o rozmiarze jednego bajta, te które zapiszemy do pamici oznaczymy jako "O".
 *		Oś jest pusta dopóki nie zapsizaemy w niej żadnych danych a pozycję dowolnej komórki możemy określić poprzez podanie odległości (ilości komórek) od początku osi.
 *
 *		Pusta oś:
 *
 *		----------------------------------------------------------------------------------------------------------------------->
 *		^												Jednowymiarowa oś reprezentująca pamięć
 *		a ===> Adres komórki "a" będziemy mogli umownie zapisać jako 0 (zero).	
 *
 *
 *		2.) Tablicę wyobraźmy sobie jako ułożone obok siebie zmienne, "O" będzie symbolizować 1 bajt w pamięci / 1 komórkę.
 * 
 *		Dla umownej tablicy czterech intów ===> int t[4]			//int na 64-bitowym komputerze to 4 bajty ===> dlatego jeden int to cztery "O"
 *
 *		t[0] t[1] t[2] t[3]
 *		|OOOO|OOOO|OOOO|OOOO|-------------------------------------------------------------------------------------------------->
 *														Jednowymiarowa oś reprezentująca pamięć
 *
 *			a.) Adres dowolnej zmiennej pobieramy "&"
 *			b.) W naszym modelu adres t[0] to 0(zero)
 *			c.) t[0] zajmuje 4 bajty dlatego następna (t[1]) będzie opatrzona adresem 4, bo 0 + 4 = 4
 *			d.) W łatwy sposób możemy obliczyć adres dowolnego elementu tablicy, np 3 element tablicy int to (3 * (pamięć zajmowana przez jednego inta)) = 3 * 4 = 12
 *			e.) tab jest równoznaczne z tab[0] - adres na początek tablicy
 *
 *		3.) Wskaźnik to zmienna w której nie trzymamy tradycyjnej wartości ale ADRES, np do innej zmiennej.
 *			a.) "*" (gwiazdka) oznacza "idź pod podany adres" lub wskaźnik
 *				a.1) int *wskaznik - to jest wskaźnik (zmienna przechowująca adres)
 *				a.2) printf("%d\n", *x); - idź pod adres podany we wskaźniku x i pobierz wartość
 *			b.) "&" służy do pobierania adresu, przykład kodu:
 *
 *		1| int a = 37; //Zmienna a pryjmuje wartość 37
 *		2| int *b = &a; //Do wskaźnika b zapisz adres zmiennej a
 *		3| printf("%d\n", *b); //Wyświetli 37
 *
 *
 *
 *	int main(int argc, char* argv[]){...} - przekazujemy do maina argc i *argv[]
 *
 *	int atgc - iczba argumentow przekazanych do programu, z czego pierwszym argumentem jest nazwa programu, wiec jej wartosść 
 *																		w sytuacji, gdy nie podajemy żadnego argumentu w terminalu == 1
 *	char* argv[] - tablica wskaźników na char
 *		- Czym jest tabica wskaźników? 
 *			> Najpościej rzecz ujmując, można to sobie wyobrazić jako tablicę dwuwymiarową (argv[][])
 *		- Pierwszym argumentem ZAWSZE jest nazwa programu -> jest przechowywana w char[0] 

 *
 * */
int main(int argc, char *argv[]){
	srand(time(NULL));
	int target, source, lbajt;
	
	/*Tworzymy bufor do którego wczytamy dane z pliku wejściowego.
	* Rozmiar został tak dobrany aby w tym konkretnym programie wczytać cały plik do bufora.
	* Ponieważ plik jest stosunkowo mały (<1kB) tworzymy bufor na 16kB (16684B / 1024 = 16kB), który jest w stanie pomieścić cały plik.
	**/
	char buf[MAX];
		
	/*	Program do poprawnego działania wymaga dwóch argumentów: ścieżki do pliku wejściowego lub nazwy pliku, jeśli plik znajduje się w tym
	*	samym folderze, co nasz program i nazwę pliku do którego zostaną zapisane ewentualne dane.
	*	Pierwszy if sprawdza, czy brakuje dwóch argumentów, drugi if sprawdza, czy brakuje tylko jednego.
	*	W tym przypadku pierwszy if jest opcjonalny.
	**/
	if (argc < 2) {
		printf("\nBrak parametrow? Co ja mam z tym niby zrobic?\n\n");
		return 1;
	}
	if (argc < 3) {
		printf("\nJuĹĽ? To Wszystko? O.o Postaraj sie bardzij i podaj mi drugi parametr!\n\n");
		return 1;
	}
	
	/*	Open zwraca deskryptor (int), zapisujemy go do zmiennej "source".
	*	W sytuacji, gdy otwieranie pliku zakończyło się powodzeniem, wartość zwróconego
	*	dekryptora będzie nieujemna, wartość (-1) oznacza błąd otwarcia pliku.
	*	
	*	Open przyjmuje dwa parametry: ścieżkę do pliku (char* path) i odpowiednią flagę - określa ona w jakim 
	*	trybie otworzymy plik, flaga O_RDONLY oznacza trym tylko do odczytu (Read Only).	
	**/
	source = open(argv[1], O_RDONLY);

	if (source == -1) { //W sytuacji błędu zwracamy komunikat i wychodzimy z programu (return 1 ===> wartość inna niż zero umownie może oznaczać błąd)
		printf("Blad 1\n");
		return 1;
	}

	/*	Read w przypadku powodzenia zwraca liczbę zczytanych bajtów, 0 oznacza koniec pliku a wartość ujemna oznacza błąd.
	*	
	*	Read jako pierwszy parametr przyjmuje deskryptor otwierania pliku (zapisany w zmiennej source), drugim jest 
	*	bufor do którego będziemy wczytywać dane a trzecim parametrem jest liczba bajtów, które chcemy wczytać.
	**/
	if((lbajt = read(source, buf, MAX)) > 0) {
		
		/*	Ponieważ w tym konkretnym zadaniu interesują nas 4 bajty (15-18) stworzymy sobie pomocniczą zmienną na nie.
		**/
		char temp[4];
		
		/*	Ponieważ interesują nas bajty od 15 do 18, tworzymy pętlę for i wczytujemy po kolei z buforu buf do tablicy temp;
		**/
		for (int i = 0; i < 4; i++) temp[i] = buf[i+15];

		/*	Obliczanie liczby elementów nie jest wymagane w tym zadaniu, ponieważ znamy ją z góry (4), natomiast pokazuje uniwersalny
		*	algorytm do oblizcania liczby elementów w danej tablicy.
		*	
		*	sizeof(x) ===> zwróci liczbę bajtów, którą x zajmuje w pamięci.
		*	
		*	Algorytm obliczania liczby elementów dzieli ilość pamięci zajmowanej przez tablicę przez pamięć zajmowaną przez jeden elemen.
		*	!!! Nie polecam tworzenia funkcji do zliczania elementów w tablicy, ponieważ wartość zajmowanej pamięci (sizeof()) tablicy 
		*	przekazanej do funkcji zwraca nieprawidłowe wartości (najczęściej 2 razy za duże) dokłady opis zjawiska do znalezienia na stackoverflow
		**/
		int liczbaElementow = sizeof(temp) / sizeof(temp[0]);
		
		if(isChar(temp, liczbaElementow)) { //Sprawdzamy, czy cztery bajty (15-18) są liczbą czy znakami.
			printf("Jest Charem\n");
			showTab(temp, liczbaElementow);
		}
		else{
			printf("Podana wartosc jest liczba!\n\n\n");

			for(int i = 0; i < liczbaElementow; i++) {
				
				/*	Zmienna char do przechowywania znaków ASCII wykorzystuje typ int (właściwie char jest intem), dlatego znając oznaczenia psozczególnych
				*	symboli w kodzie ASCII (wpisać w google: ASCII TABLE), możemy losować inty w takim przedziale (48-57), ktory wskazuje tylko na liczby, dla
				*	przykładu liczba 2 w kodzie ASCII to 50
				**/
				buf[15+i] = (rand()%10) + 48;
			}

			/* Otwieramy plik do którego odbędzie się zapis, O_RDWR oznacza tryb do odczytu i zapisu (Read Write) a oddzielone "|" O_CREAT oznacza, że w przypadku
			* gdyby nasz plik do którego chcemy zapisać dane nie istniał, zostanie automatycznie utworzony
			**/
			target = open(argv[2], O_RDWR | O_CREAT);
			
			/* Deskryptor write zostanie zapisany do zmiennej (int) ss, jeśli jego wartość == -1 oznacza to błąd zapisu.
			*
			*	Write jako parametry przyjmuje: ścieżkę do pliku docelowego, bufor z danymi do zapisu, ilość bajtów (w zmiennej typu int), które mają zostać zapisane.
			**/
			int ss;
			if (ss = (write(target, buf, lbajt)) == -1){ //Jeśli podczas zapisu powstał błąd, wyświetli komunikat i zakończy pracę programu.
				printf("Error przy zapisywaniu [%i]\n", ss);
				return 1;
			}

			/* Otwarte pliki musimy zamknąc. If sprwdza, czy oba zamknięcia się powiodły. Dla prawidłowego zamknięcia deskryptor wynosi zero.
			*	Jeśli chociaż jeden "close" zwróci inną wartość niż zero zostanie wyświetlony komunikat o błędzie.
			**/
			if(close(source) || close(target)) printf("Blad zamkniecia pliku");
		}
	}
	return 0;
}

void showTab(char inputTab[], int n){ //Pętla wyświetlająca n znaków z tablicy. Dla typu Char w printf używamy %c
	for (int i = 0; i < n; i++) printf("%c", inputTab[i]);

	printf("\n");
}

/*	Funkcja sprawdzająca, czy w tablicy znajduje się chociaż jedna wartość nie będąca liczbą (zakładamy, że tablica nie reprezentuje liczb, jeśli chociaż jeden element nią nie jest).
*	
*	Algorytm sprawdzający polega na przeszukaniu n elementów w tablicy, jeśli natrafimy na element spoza zakresu cyfr (48-57 w ASCII) od razu zwracamy true i wychodzimy z pętli, jeśli wszystkie elementy
*	są cyframi, możemy zwrócić false
**/
_Bool isChar(char in[], int n){
	for(int i = 0; i < n; i++){
		if(in[i] > 57 || in[i] < 48) return true;
	}

	return false;
}
