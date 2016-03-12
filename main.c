#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/*	- Pliki Ÿród³owe w C najlepiej kompilowaæ poleceniem:
*		gcc -O2 -W -Wall -pedantic -std=c99 -o main.o mainc
*		zapobiega to b³êdowi kompilacji "outside C99 mode"
*		i w³¹cza ostrze¿enia widoczne w terminalu.
**/



/*	#define nazwa zamieñ_na - sposób dzia³ania define == zamieni wszystkie fragmentu kodu "nazwa" na "zmieñ_na"	
*	W naszym przypadku fragment kodu "MAX" zostanie zast¹piony przez 16384
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


/* Do zrozumienia dzia³ania (int argc, char* argv[]) bêdziemy musieli wiedzieæ jak dzia³aj¹ tablice i wskaŸniki w C. (Bardzo pomocny artyku³ na wiki https://goo.gl/6PohAA)
 *
 *
 *		1.) WyobraŸmy sobie, ¿e pamiêæ któr¹ dysponujemy jest jak jednowymiarowa oœ, zbudowana z komórek o rozmiarze jednego bajta, te które zapiszemy do pamici oznaczymy jako "O".
 *		Oœ jest pusta dopóki nie zapsizaemy w niej ¿adnych danych a pozycjê dowolnej komórki mo¿emy okreœliæ poprzez podanie odleg³oœci (iloœci komórek) od pocz¹tku osi.
 *
 *		Pusta oœ:
 *
 *		----------------------------------------------------------------------------------------------------------------------->
 *		^												Jednowymiarowa oœ reprezentuj¹ca pamiêæ
 *		a ===> Adres komórki "a" bêdziemy mogli umownie zapisaæ jako 0 (zero).	
 *
 *
 *		2.) Tablicê wyobraŸmy sobie jako u³o¿one obok siebie zmienne, "O" bêdzie symbolizowaæ 1 bajt w pamiêci / 1 komórkê.
 * 
 *		Dla umownej tablicy czterech intów ===> int t[4]			//int na 64-bitowym komputerze to 4 bajty ===> dlatego jeden int to cztery "O"
 *
 *		t[0] t[1] t[2] t[3]
 *		|OOOO|OOOO|OOOO|OOOO|-------------------------------------------------------------------------------------------------->
 *														Jednowymiarowa oœ reprezentuj¹ca pamiêæ
 *
 *			a.) Adres dowolnej zmiennej pobieramy "&"
 *			b.) W naszym modelu adres t[0] to 0(zero)
 *			c.) t[0] zajmuje 4 bajty dlatego nastêpna (t[1]) bêdzie opatrzona adresem 4, bo 0 + 4 = 4
 *			d.) W ³atwy sposób mo¿emy obliczyæ adres dowolnego elementu tablicy, np 3 element tablicy int to (3 * (pamiêæ zajmowana przez jednego inta)) = 3 * 4 = 12
 *			e.) tab jest równoznaczne z tab[0] - adres na pocz¹tek tablicy
 *
 *		3.) WskaŸnik to zmienna w której nie trzymamy tradycyjnej wartoœci ale ADRES, np do innej zmiennej.
 *			a.) "*" (gwiazdka) oznacza "idŸ pod podany adres" lub wskaŸnik
 *				a.1) int *wskaznik - to jest wskaŸnik (zmienna przechowuj¹ca adres)
 *				a.2) printf("%d\n", *x); - idŸ pod adres podany we wskaŸniku x i pobierz wartoœæ
 *			b.) "&" s³u¿y do pobierania adresu, przyk³ad kodu:
 *
 *		1| int a = 37; //Zmienna a pryjmuje wartoœæ 37
 *		2| int *b = &a; //Do wskaŸnika b zapisz adres zmiennej a
 *		3| printf("%d\n", *b); //Wyœwietli 37
 *
 *
 *
 *	int main(int argc, char* argv[]){...} - przekazujemy do maina argc i *argv[]
 *
 *	int atgc - iczba argumentow przekazanych do programu, z czego pierwszym argumentem jest nazwa programu, wiec jej wartosœæ 
 *																		w sytuacji, gdy nie podajemy ¿adnego argumentu w terminalu == 1
 *	char* argv[] - tablica wskaŸników na char
 *		- Czym jest tabica wskaŸników? 
 *			> Najpoœciej rzecz ujmuj¹c, mo¿na to sobie wyobraziæ jako tablicê dwuwymiarow¹ (argv[][])
 *		- Pierwszym argumentem ZAWSZE jest nazwa programu -> jest przechowywana w char[0] 

 *
 * */
int main(int argc, char *argv[]){
	srand(time(NULL));
	int target, source, lbajt;
	
	/*Tworzymy bufor do którego wczytamy dane z pliku wejœciowego.
	* Rozmiar zosta³ tak dobrany aby w tym konkretnym programie wczytaæ ca³y plik do bufora.
	* Poniewa¿ plik jest stosunkowo ma³y (<1kB) tworzymy bufor na 16kB (16684B / 1024 = 16kB), który jest w stanie pomieœciæ ca³y plik.
	**/
	char buf[MAX];
		
	/*	Program do poprawnego dzia³ania wymaga dwóch argumentów: œcie¿ki do pliku wejœciowego lub nazwy pliku, jeœli plik znajduje siê w tym
	*	samym folderze, co nasz program i nazwê pliku do którego zostan¹ zapisane ewentualne dane.
	*	Pierwszy if sprawdza, czy brakuje dwóch argumentów, drugi if sprawdza, czy brakuje tylko jednego.
	*	W tym przypadku pierwszy if jest opcjonalny.
	**/
	if (argc < 2) {
		printf("\nBrak parametrow? Co ja mam z tym niby zrobic?\n\n");
		return 1;
	}
	if (argc < 3) {
		printf("\nJuÅ¼? To Wszystko? O.o Postaraj sie bardzij i podaj mi drugi parametr!\n\n");
		return 1;
	}
	
	/*	Open zwraca deskryptor (int), zapisujemy go do zmiennej "source".
	*	W sytuacji, gdy otwieranie pliku zakoñczy³o siê powodzeniem, wartoœæ zwróconego
	*	dekryptora bêdzie nieujemna, wartoœæ (-1) oznacza b³¹d otwarcia pliku.
	*	
	*	Open przyjmuje dwa parametry: œcie¿kê do pliku (char* path) i odpowiedni¹ flagê - okreœla ona w jakim 
	*	trybie otworzymy plik, flaga O_RDONLY oznacza trym tylko do odczytu (Read Only).	
	**/
	source = open(argv[1], O_RDONLY);

	if (source == -1) { //W sytuacji b³êdu zwracamy komunikat i wychodzimy z programu (return 1 ===> wartoœæ inna ni¿ zero umownie mo¿e oznaczaæ b³¹d)
		printf("Blad 1\n");
		return 1;
	}

	/*	Read w przypadku powodzenia zwraca liczbê zczytanych bajtów, 0 oznacza koniec pliku a wartoœæ ujemna oznacza b³¹d.
	*	
	*	Read jako pierwszy parametr przyjmuje deskryptor otwierania pliku (zapisany w zmiennej source), drugim jest 
	*	bufor do którego bêdziemy wczytywaæ dane a trzecim parametrem jest liczba bajtów, które chcemy wczytaæ.
	**/
	if((lbajt = read(source, buf, MAX)) > 0) {
		
		/*	Poniewa¿ w tym konkretnym zadaniu interesuj¹ nas 4 bajty (15-18) stworzymy sobie pomocnicz¹ zmienn¹ na nie.
		**/
		char temp[4];
		
		/*	Poniewa¿ interesuj¹ nas bajty od 15 do 18, tworzymy pêtlê for i wczytujemy po kolei z buforu buf do tablicy temp;
		**/
		for (int i = 0; i < 4; i++) temp[i] = buf[i+15];

		/*	Obliczanie liczby elementów nie jest wymagane w tym zadaniu, poniewa¿ znamy j¹ z góry (4), natomiast pokazuje uniwersalny
		*	algorytm do oblizcania liczby elementów w danej tablicy.
		*	
		*	sizeof(x) ===> zwróci liczbê bajtów, któr¹ x zajmuje w pamiêci.
		*	
		*	Algorytm obliczania liczby elementów dzieli iloœæ pamiêci zajmowanej przez tablicê przez pamiêæ zajmowan¹ przez jeden elemen.
		*	!!! Nie polecam tworzenia funkcji do zliczania elementów w tablicy, poniewa¿ wartoœæ zajmowanej pamiêci (sizeof()) tablicy 
		*	przekazanej do funkcji zwraca nieprawid³owe wartoœci (najczêœciej 2 razy za du¿e) dok³ady opis zjawiska do znalezienia na stackoverflow
		**/
		int liczbaElementow = sizeof(temp) / sizeof(temp[0]);
		
		if(isChar(temp, liczbaElementow)) { //Sprawdzamy, czy cztery bajty (15-18) s¹ liczb¹ czy znakami.
			printf("Jest Charem\n");
			showTab(temp, liczbaElementow);
		}
		else{
			printf("Podana wartosc jest liczba!\n\n\n");

			for(int i = 0; i < liczbaElementow; i++) {
				
				/*	Zmienna char do przechowywania znaków ASCII wykorzystuje typ int (w³aœciwie char jest intem), dlatego znaj¹c oznaczenia psozczególnych
				*	symboli w kodzie ASCII (wpisaæ w google: ASCII TABLE), mo¿emy losowaæ inty w takim przedziale (48-57), ktory wskazuje tylko na liczby, dla
				*	przyk³adu liczba 2 w kodzie ASCII to 50
				**/
				buf[15+i] = (rand()%10) + 48;
			}

			/* Otwieramy plik do którego odbêdzie siê zapis, O_RDWR oznacza tryb do odczytu i zapisu (Read Write) a oddzielone "|" O_CREAT oznacza, ¿e w przypadku
			* gdyby nasz plik do którego chcemy zapisaæ dane nie istnia³, zostanie automatycznie utworzony
			**/
			target = open(argv[2], O_RDWR | O_CREAT);
			
			/* Deskryptor write zostanie zapisany do zmiennej (int) ss, jeœli jego wartoœæ == -1 oznacza to b³¹d zapisu.
			*
			*	Write jako parametry przyjmuje: œcie¿kê do pliku docelowego, bufor z danymi do zapisu, iloœæ bajtów (w zmiennej typu int), które maj¹ zostaæ zapisane.
			**/
			int ss;
			if (ss = (write(target, buf, lbajt)) == -1){ //Jeœli podczas zapisu powsta³ b³¹d, wyœwietli komunikat i zakoñczy pracê programu.
				printf("Error przy zapisywaniu [%i]\n", ss);
				return 1;
			}

			/* Otwarte pliki musimy zamkn¹c. If sprwdza, czy oba zamkniêcia siê powiod³y. Dla prawid³owego zamkniêcia deskryptor wynosi zero.
			*	Jeœli chocia¿ jeden "close" zwróci inn¹ wartoœæ ni¿ zero zostanie wyœwietlony komunikat o b³êdzie.
			**/
			if(close(source) || close(target)) printf("Blad zamkniecia pliku");
		}
	}
	return 0;
}

void showTab(char inputTab[], int n){ //Pêtla wyœwietlaj¹ca n znaków z tablicy. Dla typu Char w printf u¿ywamy %c
	for (int i = 0; i < n; i++) printf("%c", inputTab[i]);

	printf("\n");
}

/*	Funkcja sprawdzaj¹ca, czy w tablicy znajduje siê chocia¿ jedna wartoœæ nie bêd¹ca liczb¹ (zak³adamy, ¿e tablica nie reprezentuje liczb, jeœli chocia¿ jeden element ni¹ nie jest).
*	
*	Algorytm sprawdzaj¹cy polega na przeszukaniu n elementów w tablicy, jeœli natrafimy na element spoza zakresu cyfr (48-57 w ASCII) od razu zwracamy true i wychodzimy z pêtli, jeœli wszystkie elementy
*	s¹ cyframi, mo¿emy zwróciæ false
**/
_Bool isChar(char in[], int n){
	for(int i = 0; i < n; i++){
		if(in[i] > 57 || in[i] < 48) return true;
	}

	return false;
}