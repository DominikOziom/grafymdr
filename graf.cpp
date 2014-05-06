#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <limits.h>
#include <math.h>
#include <sys/time.h>
#include <fstream>
#include <list>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

//WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
//WWWWW  DEFINICJA KLASY IMPLEMENTUJACEJ WIERZCHOLEK   WWW
//WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
template <typename Typ>
class wierzcholek{
public:
  int key;  // oryginalny klucz wierzcholka
  Typ val;   // przechowywana wartosc
};
//KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
//KKKKK  DEFINICJA KLASY IMPLEMENTUJACEJ KRAWEDZ  KKKKKKKK
//KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
template <typename Typ>
class krawedz{
public:
  int waga;  // wartosc krawedzi
  wierzcholek<Typ> *first;  // wskaznik na pierwszy wierzcholek
  wierzcholek<Typ> *second;     // wskaznik na drugi wierzcholek

  krawedz();  // konstruktor
  ~krawedz(); // destruktor
};
// KONSTRUKTOR
template <typename Typ>
krawedz<Typ>::krawedz(){
  waga=0;
  first=NULL;
  second=NULL;
}
// DESTRUKTOR
template <typename Typ>
krawedz<Typ>::~krawedz(){
  first=NULL;
  second=NULL;
}
// KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
// KKKK DEFINICJA KLASY COMPARE (REGULA SORTOWANIA HEAP) KKKK
// KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
template <typename Typ>
class Compare{
public:
  bool operator()(const krawedz<Typ> x, const krawedz<Typ> y) const
  {
    return x.waga > y.waga;
  } 
};
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MM DEFINICJA KLASY IMPLEMETUJACEJ MACIERZ SASIEDZTWA MM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
template <typename Typ>
class MacierzS{
public:

  krawedz<Typ>** tab;    // tablica dynamiczna dwuwymiarowa (struktura grafu)
  krawedz<Typ>** tab2;    // tablica dynamiczna dwuwymiarowa (struktura grafu)
  wierzcholek<Typ> *Twierzch;   // tablica dynamiczna jednowym. z wartosciami wierzcholkow
  bool *visited;  // tablica z informacja czy dany wierzcholek zostal juz odwiedzony
  int l_wierzch;   // zmienna przechowujaca liczbe wierzcholkow
  int l_kraw;   // zmienna przechowujaca liczbe krawedzi
  vector<krawedz<Typ> > krawedzie1;  // wektor do przechowania posortowanych krawedzi
  vector<krawedz<Typ> > krawedzie2;  // wektor do przechowywania krawedzi w algorytmie Prima
  vector<krawedz<Typ> > drzewo1;  // wektor do przechowania krawedzi tworzacych drzewo (algKruskala)
  vector<krawedz<Typ> > drzewo2;  // wektor do przechowania krawedzi tworzacych drzewo (algPrima)


  MacierzS();  // konstruktor
  ~MacierzS(); // destruktor
  void ZmienWielkosc(int l_w, int l_k);  // zmienia wielkosc tablicy wg zadanych param
  int operator () (int W, int K) const {return tab[W][K];} // przeciaza operator () zwraca wartosc
  int & operator () (int W, int K) {return tab[W][K];} // przeciaza operator () - zwraca referencje
  bool WczytajZPliku(string NazwaPliku); // wczytuje graf z pliku
  bool ZapiszDoPliku(string NazwaPliku); // zapisuje graf do pliku
  void Print();  // drukuje macierz na standardowym wyjsciu
  void GenerujGraf(int l_w, int gestosc);  // generuje losowy graf
  void DFS(int wierzch_start);   // przechodzenie grafu w glab
  bool CzySpojny();   // sprawdza czy graf jest spojny

  void SortujKrawedzie(); //sortuje krawedzie zapisane w wektorze 'krawedzie' poprzez kopcowanie
  void AlgKruskala(); // przeprowadza algorytm kruskala na wektorze 'krawedzie' i wynik zapisuje w 'drzewo1'
  void AlgPrima(); // przeprowadza algorytm prima na macierzy i wynik zapisuje w 'drzewo2'
  void PrzepiszWektorNaMacierz(vector<krawedz<Typ> > W); //przepisuje wybrany wektor do struktury macierz
};

// konstruktor
template <typename Typ>
MacierzS<Typ>::MacierzS(){
  int i,j;
  l_wierzch=0;
  l_kraw=0;
  Twierzch=new wierzcholek<Typ> [l_wierzch]; // alokacja pamieci dla tablicy wierzcholkow
  visited = new bool [l_wierzch];        // alokacja pamieci tablicy odwiedzin

  tab=new krawedz<Typ>* [l_wierzch]; // alokacja pamieci dla pierwszych elementow wierszy
  for (i=0; i<l_wierzch; i++)
    tab[i]=new krawedz<Typ> [l_wierzch];  // alokacja pamieci dla wierszy

  tab2=new krawedz<Typ>* [l_wierzch]; // alokacja pamieci dla pierwszych elementow wierszy
  for (i=0; i<l_wierzch; i++)
    tab2[i]=new krawedz<Typ> [l_wierzch];  // alokacja pamieci dla wierszy

  for (i=0; i<l_wierzch; i++){
    visited[i]=false;         // wyzerowanie tablicy odwiedzin
    for (j=0; j<l_wierzch; j++)
      (tab[i][j]).waga=0;       // wyzerowanie macierzy
  }
}

// destruktor
template <typename Typ>
MacierzS<Typ>::~MacierzS(){
  int i;
  delete Twierzch;
  delete visited;
  for (i=0; i<l_wierzch; i++)
    delete [] tab[i];
  delete tab;

  for (i=0; i<l_wierzch; i++)
    delete [] tab2[i];
  delete tab2;

  l_wierzch=0;
  l_kraw=0;
}
// Zwalnia zajeta pamiec i rezerwuje na nowo o zadanych parametrach
template <typename Typ>
void MacierzS<Typ>::ZmienWielkosc(int l_w, int l_k){
  int i,j;
  delete Twierzch;   // zwolnienie aktualnie zajetej pamieci
  delete visited;
  for (i=0; i<l_wierzch; i++)
    delete [] tab[i];
  delete tab;
  l_wierzch=l_w;
  l_kraw=l_k;
  Twierzch=new wierzcholek<Typ> [l_wierzch]; // alokacja pamieci dla tablicy wierzcholkow
  visited = new bool [l_wierzch];      // alokacja pamieci dla tablicy odwiedzin

  tab=new krawedz<Typ>* [l_wierzch]; // alokacja pamieci dla pierwszych elementow wierszy
  for (i=0; i<l_wierzch; i++)
    tab[i]=new krawedz<Typ> [l_wierzch];  // alokacja pamieci dla wierszy

  tab2=new krawedz<Typ>* [l_wierzch]; // alokacja pamieci dla pierwszych elementow wierszy
  for (i=0; i<l_wierzch; i++)
    tab2[i]=new krawedz<Typ> [l_wierzch];  // alokacja pamieci dla wierszy

  for (i=0; i<l_wierzch; i++){
    visited[i]=false;         // wyzerowanie tablicy odwiedzin
    for (j=0; j<l_wierzch; j++)
      (tab[i][j]).waga=0;       // wyzerowanie macierzy
  }
}
/* Funkcja wczytuje graf z pliku.
 * Najpierw nastepuje proba otworzenia pliku.
 * W przypadku powodzenia pobrane z niego sa: liczba wierzcholkow,
 * liczba krawedzi, a nastepnie cala macierz.
 */
template <typename Typ>
bool MacierzS<Typ>::WczytajZPliku(string NazwaPliku){
  fstream plik(NazwaPliku.c_str(), ios::in);  // otworzenie pliku
  if(!plik.is_open()){
    cerr << "Blad otwarcia pliku" << endl;
    return false;              // niepowodzenie otwarcia pliku o zadanej nazwie
  }
  int i,waga,V1,V2,l_w,l_k;
  plik >> l_w;
  plik >> l_k;

  ZmienWielkosc(l_w,l_k);

  for (i=0; i<l_wierzch; i++){   // wczytanie wartosci wierzcholkow
    plik >> (Twierzch[i]).val;
    (Twierzch[i]).key=i;
  }

  for (i=0; i<l_wierzch; i++){   // wczytanie wartosci wierzcholkow
    cout << (Twierzch[i]).val << endl;
    cout << (Twierzch[i]).key << endl;
  }

  cout << l_kraw << endl;
  for (i=0; i<=(2*l_kraw+1); i++){     // wczytanie krawedzi z ich wagami
    plik >> V1 >> waga >> V2;
    (tab[V1][V2]).waga=(tab[V2][V1]).waga=waga;
    (tab[V1][V2]).first=(&Twierzch[V1]);   // przypisanie wskaznikow na wierzcholki
    (tab[V2][V1]).first=(&Twierzch[V2]);
    (tab[V1][V2]).second=(&Twierzch[V2]);
    (tab[V2][V1]).second=(&Twierzch[V1]);
  }
  return true;
}

/* Funkcja zapisuje graf do pliku.
 * Najpierw nastepuje proba otworzenia pliku.
 * W przypadku powodzenia zapisane do niego sa: liczba wierzcholkow,
 * liczba krawedzi, a nastepnie cala macierz.
 */
template <typename Typ>
bool MacierzS<Typ>::ZapiszDoPliku(string NazwaPliku){
  fstream plik(NazwaPliku.c_str(), ios::out);
  if(!plik.is_open()){
    cerr << "Blad otwarcia pliku" << endl;
    return false;              // niepowodzenie otwarcia pliku o zadanej nazwie
  }
  int w, k;

  plik << l_wierzch << " " << l_kraw << endl;

  for (w=0; w<l_wierzch;w++)      // zapisanie kolejnych wartosci wierzcholkow
    plik << Twierzch[w].val << endl;

  for (w=0; w<l_wierzch; w++){  // 'wybiera' kolejne wiersze
    for (k=0; k<l_wierzch; k++){   // 'wybiera' kolejne komorki wiersza
      plik << w << " " << tab[w][k].waga << " " << k << endl;  // zapisuje nr wierzch, wage, nr wierzch
    }
  }
  return true;
}

// drukuje macierz na standardowym wyjsciu
template <typename Typ>
void MacierzS<Typ>::Print(){
  int w,k;
  int z=0;
  cout << endl;
  for (w=0; w<l_wierzch; w++){
    for (k=0; k<l_wierzch; k++){
      if(tab[w][k].waga!=0){
	cout << setw(12) << tab[w][k].waga << " ";  // druk wag
      }
      else
	cout << setw(12) << z << " ";   // druk zer
    }
    cout << endl;
  }
  /*  // drukowanie tablicy minimalnego drzewa rozpinajacego (trzeba odkomentowac)
      // i uprzednio wlaczyc algorytm prima lub kruskala
    int w,k;
    int z=0;
    cout << endl;
    for (w=0; w<l_wierzch; w++){
    for (k=0; k<l_wierzch; k++){
    if(tab2[w][k].waga!=0){
    cout << setw(12) << tab2[w][k].waga << " ";
    }
    else
    cout << setw(12) << z << " ";
    }
    cout << endl;
    }}*/
}
/* Funkcja generuje losowy graf o zadanej liczbie wierzcholkow
 * i zadanej gestosci.
 */
template <typename Typ>
void MacierzS<Typ>::GenerujGraf(int l_w, int gestosc){
  int i,j;
  srand (time(NULL));
  ZmienWielkosc(l_w,0);
  for (i=0; i<l_wierzch; i++){    // wygenerowanie tablicy wierzcholkow
    Twierzch[i].val=rand()%INT_MAX; // generowanie wartosci wierzcholka
    Twierzch[i].key=i;   // przypisanie klucza
  }
  for (i=0; i<l_wierzch; i++){    // wygenerowanie macierzy
    for (j=i+1; j<l_wierzch; j++){
      if((rand()%100)<=gestosc){   // warunek zachowania gestosci
	(tab[i][j]).waga=(tab[j][i]).waga=rand()%INT_MAX+1;   // losowa waga 1-INT_MAX
	(tab[i][j]).first=&Twierzch[i];   // przypisanie wskaznika na 1 wierzcholek
	(tab[j][i]).first=&Twierzch[j];   // przypisanie wskaznika na 1 wierzcholek
	(tab[i][j]).second=&Twierzch[j];   // przypisanie wskaznika na 2 wierzcholek
	(tab[j][i]).second=&Twierzch[i];   // przypisanie wskaznika na 2 wierzcholek
	l_kraw++;
      }
      else{
	(tab[i][j]).waga=(tab[j][i]).waga=0;          // zero dla zachowania gestosci
      }
    }
  }
}
// ALGORYTM REKURENCYJNY DFS
// przechodzi przez cala macierz zapisujac wierzcholki
// w ktorych juz byl ( tablica bool visited )
template <typename Typ>
void MacierzS<Typ>::DFS(int wierzch_start){
  int i;
  visited[wierzch_start]=true;
  for (i=0; i<l_wierzch; i++)
    if((tab[wierzch_start][i].waga!=0) && !visited[i])
      DFS(i);
}
// CZY SPOJNY
template <typename Typ>
bool MacierzS<Typ>::CzySpojny(){
  int i,odwiedzone=0;
  DFS(0);
  for (i=0; i<l_wierzch; i++)
    if (visited[i]==true)
      odwiedzone++;
  if (odwiedzone==l_wierzch) return true;
  else return false;
}
//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//                                      KRUSKAL

// funkcja sortujaca krawedzie poprzez kopcowanie
template <typename Typ>
void MacierzS<Typ>::SortujKrawedzie()
{
  for (int i=0; i<l_wierzch; i++)    // utworzenie wectora krawedzi
    for (int j=i+1; j<l_wierzch; j++){
      if ( tab[j][i].waga!=0)
        {
	  krawedzie1.push_back(tab[j][i]);
        }
    }

  make_heap (krawedzie1.begin(), krawedzie1.end(), Compare<Typ>()); //def. kopca
 }

template <typename Typ>
void MacierzS<Typ>::AlgKruskala()
{
  vector<int> Z;
  int  j, k, u, v, w;

  for (int i=0; i<l_wierzch; i++) Z.push_back((-1)); // utworzenie wektora kolorow
  k=0;
  j=0;
  while (k<l_wierzch)
    {
      u = krawedzie1[j].first->key;    // przypisanie kluczy sprawdzanej krawedzi
      v = krawedzie1[j].second->key;
      if ( Z[u]==(-1) || Z[v]==(-1) || Z[u]!=Z[v] )  // czy wierzcholki naleza do roznych klastrow
	{                                      // lub czy byly sprawdzane
	  drzewo1.push_back(krawedzie1[j]);   // sprawdzona krawedz bedzie tworzyc drzewo
	  if ( Z[u]==(-1) && Z[v]!=(-1) )   // jesli jeden wierzcholek nalezy do drzewa a drugi nie
	    {
	      w =u;
	      u =v;
	      v =w;
	    }
	  if ( Z[u]==(-1) ) Z[u]=u;   // nadanie kolorow
	  if ( Z[v]==(-1) ) Z[v]=Z[u];
	  else                    // laczenie drzew 
	    {
	      w =Z[v];
	      for (int i=0; i<l_wierzch; i++){
		if ( Z[i]==w ) Z[i]=Z[u];}
	    }
	}
      j++;
      k++;
      //u=0;
      // v=0;
    }
}
//PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
//                                PRIM
template <typename Typ>
void MacierzS<Typ>::AlgPrima()
{
  int w=0, wsk=0;
  unsigned int j;
  int b=1;
  make_heap(krawedzie2.begin(), krawedzie2.end(), Compare<Typ>());


  while(w<l_wierzch){
    for(int i=0; i<l_wierzch; i++){  //zapisanie do wektora wszystkich krawedzi danego wierzcholka 
      if(tab[wsk][i].waga!=0){
	if(drzewo2.size()!=0){
	  for( j=0; j<(drzewo2.size()); j++){  //petla sprawdzajaca czy dana krawedz juz nie wystapila w drzewie	
	    if(tab[wsk][i].waga==drzewo2[j].waga){
	      b=0;	   
	    }	 
	  }//koniec petli sprawdzajacej
	  if(b==1) krawedzie2.push_back(tab[wsk][i]); //dodawanie krawedzi do wektora w ktorym nastapi sortowanie
	  b=1;
	}
	else 
	  krawedzie2.push_back(tab[wsk][i]); //dodawanie krawedzi do wektora  sortujacego     
      }//koniec petli wczytujacej krawedzie
    }
    push_heap(krawedzie2.begin(), krawedzie2.end(), Compare<Typ>()); 
    make_heap(krawedzie2.begin(), krawedzie2.end(), Compare<Typ>()); 
    
    wsk = krawedzie2[1].second->key;   // ustawienie indeksu do znalezienia krawedzi nastepnego wierzcholka   
    drzewo2.push_back(krawedzie2[0]);   
    krawedzie2.erase(krawedzie2.begin(), krawedzie2.begin()+1); //usuniecie z wektora pomocniczego najmniejszej krawedzi 

    w++;
  }
}
//przepisuje wybrany wektor do struktury macierz
template <typename Typ>
void MacierzS<Typ>::PrzepiszWektorNaMacierz(vector<krawedz<Typ> > W){
  int r=W.size();
  int index1, index2, wag, w; 
  for(w=0; w < r; w++){
    index1 = W[w].first->key;
    index2 = W[w].second->key;
    wag = W[w].waga;
    tab2[index1][index2].waga = wag;
    tab2[index2][index1].waga = wag;
  }
}
// LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
// LLLLLLLLLLL    LISTA SASIEDZTWA    LLLLLLLLLLLLLLLLLLLLLLLLLLL
// LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
template <typename Typ>
class ListaS: public vector<list<wierzcholek<Typ> > >{
public:
  krawedz<Typ> *Tkraw;   // tablica dynamiczna jednowym. z krawedziami
  int l_wierzch;   // zmienna przechowujaca liczbe wierzcholkow
  int l_kraw;   // zmienna przechowujaca liczbe krawedzi
  vector<krawedz<Typ> > krawedzie1;  // wektor do przechowania posortowanych krawedzi
  vector<krawedz<Typ> > krawedzie2;  // wektor do przechowywania krawedzi w algorytmie Prima
  vector<krawedz<Typ> > drzewo1;  // wektor do przechowania krawedzi tworzacych drzewo (algKruskala)
  vector<krawedz<Typ> > drzewo2;  // wektor do przechowania krawedzi tworzacych drzewo (algPrima)

  ListaS();  // konstruktor
  ~ListaS(); // destruktor
  void ZmienWielkosc(int l_w, int l_k); // zmiana wielkosci struktur danych
  void DodajWierzcholek(wierzcholek<Typ> W, int ind);  // dodaje wierzcholek do odpowiedniej listy
  void DodajKrawedz(krawedz<Typ> K, int ind);  // dodaje krawedz do tabeli i drugi wierzcholek do listy
  bool WczytajZPliku(string NazwaPliku); // wczytuje graf z pliku
  bool ZapiszDoPliku(string NazwaPliku); // zapisuje graf do pliku
  void Print();  // drukuje liste na standardowym wyjsciu
  void GenerujGraf(int l_w, int gestosc);  // generuje graf losowy

  void SortujKrawedzie(); //sortuje krawedzie zapisane w wektorze 'krawedzie' poprzez kopcowanie
  void AlgKruskala(); // przeprowadza algorytm kruskala na wektorze 'krawedzie' i wynik zapisuje w 'drzewo1'
  void AlgPrima(); // przeprowadza algorytm prima na macierzy i wynik zapisuje w 'drzewo2'
};
// KONSTRUKTOR
template <typename Typ>
ListaS<Typ>::ListaS(){
  l_wierzch=0;
  l_kraw=0;
  Tkraw=new krawedz<Typ> [l_kraw];  // alokacja pamieci dla tablicy krawedzi
}
// DESTRUKTOR
template <typename Typ>
ListaS<Typ>::~ListaS(){
  int i;
  for (i=0; i<l_wierzch; i++)
    delete (*this)[i];
  delete (*this);
  delete Tkraw;    // usuniecie tablicy krawedzi
}
// ZMIENIA MAX WIELKOSC TABLICY I LISTY
template <typename Typ>
void ListaS<Typ>::ZmienWielkosc(int l_w, int l_k){
  int i;
  list<wierzcholek<Typ> > T;
  l_wierzch=l_w;
  l_kraw=l_k;
  Tkraw=new krawedz<Typ> [l_k];
  for (i=0; i<l_w; i++){
    (*this).push_back(T);
  }
}
// DODAJE WIERZCHOLEK
template <typename Typ>
void ListaS<Typ>::DodajWierzcholek(wierzcholek<Typ> W, int ind){
  (*this)[ind].push_back(W);
}
// DODAJE KRAWEDZ
template <typename Typ>
void ListaS<Typ>::DodajKrawedz(krawedz<Typ> K, int ind){
  Tkraw[ind]=K;                            // dodaje krawedz do tabeli krawedzi
  DodajWierzcholek((*this)[K.second->key].front(),K.first->key); // dodaje drugi wierzcholek do listy
}
// WCZYTUJE GRAF Z PLIKU
template <typename Typ>
bool ListaS<Typ>::WczytajZPliku(string NazwaPliku){
  fstream plik(NazwaPliku.c_str(), ios::in);  // otworzenie pliku
  if(!plik.is_open()){
    cerr << "Blad otwarcia pliku" << endl;
    return false;              // niepowodzenie otwarcia pliku o zadanej nazwie
  }
  wierzcholek<Typ> W;
  krawedz<Typ> K;
  int i,wag,wart,V1,V2,l_w,l_k;
  plik >> l_w;
  plik >> l_k;
  ZmienWielkosc(l_w,l_k);

  for (i=0; i<l_w; i++){   // wczytanie wartosci wierzcholkow
    plik >> wart;
    W.val=wart;
    W.key=i;
    DodajWierzcholek(W,i);
  }

  for (i=0; i<l_k; i++){     // wczytanie krawedzi z ich wagami
    plik >> V1 >> wag >> V2;
    K.waga=wag;
    K.first=&(*this)[V1].front();
    K.second=&(*this)[V2].front();
    DodajKrawedz(K,i);
  }
  return true;
}
// ZAPISUJE GRAF DO PLIKU
template <typename Typ>
bool ListaS<Typ>::ZapiszDoPliku(string NazwaPliku){
  fstream plik(NazwaPliku.c_str(), ios::out);
  if(!plik.is_open()){
    cerr << "Blad otwarcia pliku" << endl;
    return false;              // niepowodzenie otwarcia pliku o zadanej nazwie
  }
  int w;
  plik << l_wierzch << " " << l_kraw << endl;

  for (w=0; w<l_wierzch;w++)      // zapisanie kolejnych wartosci wierzcholkow
    plik << (*this)[w].begin()->val << endl;

  for (w=0; w<l_kraw; w++){  // 'wybiera' kolejne krawedzie
    plik << Tkraw[w].first->key<< " " << Tkraw[w].waga << " " << Tkraw[w].second->key << endl;
  }
  return true;
}
// DRUKUJE GRAF
template <typename Typ>
void ListaS<Typ>::Print(){
  int i;
  cout << l_wierzch << " " << l_kraw << endl;
  for (i=0; i<l_wierzch; i++){   // drukowanie wierzcholkow
    cout << (*this)[i].begin()->val << endl;
  }
  for (i=0; i<l_kraw; i++){   // drukowanie wierzcholka - krawedzi - wierzcholka
    cout << Tkraw[i].first->key << " " << Tkraw[i].waga << " " << Tkraw[i].second->key << endl;
  }
}
//GENERUJE GRAF O ZADANEJ L WIERZCHOLKOW I GESTOSCI
template <typename Typ>
void ListaS<Typ>::GenerujGraf(int l_w, int gestosc){
  int i,j,y=0;
  int **tab;

  tab=new int* [l_w]; // alokacja pamieci dla pierwszych elementow wierszy
  for (i=0; i<l_w; i++)
    tab[i]=new int [l_w];  // alokacja pamieci dla wierszy

  wierzcholek<Typ> W;
  krawedz<Typ> K;
  srand (time(NULL));

  for (i=0; i<l_w; i++){    // wygenerowanie macierzy wag
    for (j=i+1; j<l_w; j++){
      if((rand()%100)<=gestosc){
	tab[i][j]=tab[j][i]=rand()%INT_MAX+1;   // losowa waga 1-INT_MAX
	y++;
      }
      else{
	tab[i][j]=tab[j][i]=0;          // zero dla zachowania gestosci
      }
    }
  }

  ZmienWielkosc(l_w,2*y);

  for (i=0; i<l_w; i++){   // wygenerowanie wierzcholkow
    W.val=rand()%INT_MAX;
    W.key=i;
    DodajWierzcholek(W,i);
  }
  y=0;
  for (i=0; i<l_w; i++){   //przeksztalcenie macierzy 2W na tablice 1W
    for (j=0; j<l_w; j++){
      if(tab[i][j]!=0){
	K.waga=tab[i][j];
	K.first=&(*this)[i].front();
	K.second=&(*this)[j].front();
	DodajKrawedz(K,y);
	y++;
      }
    }
  }

  for (i=0; i<l_wierzch; i++)  // usuniecie pomocniczej tablicy
    delete [] tab[i];
  delete tab;
}

// funkcja sortujaca krawedzie poprzez kopcowanie
template <typename Typ>
void ListaS<Typ>::SortujKrawedzie()
{
  for (int i=0; i<l_kraw; i++)
    krawedzie1.push_back(Tkraw[i]);

  make_heap (krawedzie1.begin(), krawedzie1.end(), Compare<Typ>());  
}

//KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKkkKKKKKKKKKKKKKKKKKKKKKKKKKk
//                            KRUSKAL
// analogiczny do tego dla macierzy
template <typename Typ>
void ListaS<Typ>::AlgKruskala()
{
  std::vector<int> Z;
  int  j, k, u, v, w;

  for (int i=0; i<l_wierzch; i++) Z.push_back((-1));

  k=0;
  j=0;
  while (k<l_wierzch)
    {
      u = krawedzie1[j].first->key;
      v = krawedzie1[j].second->key;
      
      if ( Z[u]==(-1) || Z[v]==(-1) || Z[u]!=Z[v] )
	{
	  drzewo1.push_back(krawedzie1[j]);
	  if ( Z[u]==(-1) && Z[v]!=(-1) )
	    {
	      w =u;
	      u =v;
	      v =w;
	    }
	  if ( Z[u]==(-1) ) Z[u]=u;

	  if ( Z[v]==(-1) ) Z[v]=Z[u];
	  else
	    {
	      w =Z[v];
	      for (int i=0; i<l_wierzch; i++){
		if ( Z[i]==w ) Z[i]=Z[u];}
	    }
	}
      j++;
      k++;
      //u=0;
      // v=0;
    }
}
//PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
//                                 PRIM
template <typename Typ>
void ListaS<Typ>::AlgPrima()
{

  int w=0, wsk=0;
  unsigned int j;
  int b=1;
  make_heap(krawedzie2.begin(), krawedzie2.end(), Compare<Typ>());

  while(w<l_wierzch){
    for(int i=0; i<l_kraw; i++){  //zapisanie do wektora wszystkich krawedzi danego wierzcholka 
      if(drzewo2.size()!=0){
	for( j=0; j<drzewo2.size(); j++){  //petla sprawdzajaca czy dana krawedz juz nie wystapila w drzewie	
	  if(Tkraw[i].waga==drzewo2[j].waga){
	    b=0;	   
	  }	 
	}//koniec petli sprawdzajacej
	if(b==1) krawedzie2.push_back(Tkraw[i]); //dodawanie krawedzi do wektora w ktorym nastapi sortowanie
	b=1;
      }
      else 
	for(int h=0; h<l_kraw; h++){
	  if(Tkraw[h].first->key==wsk)
	    krawedzie2.push_back(Tkraw[h]); //dodawanie krawedzi do wektora  sortujacego 
	}
    }//koniec petli wczytujacej krawedzie
    
    make_heap(krawedzie2.begin(), krawedzie2.end(), Compare<Typ>()); 
   
    
    wsk = krawedzie2[1].second->key;   // ustawienie indeksu do znalezienia krawedzi nastepnego wierzcholka   
    drzewo2.push_back(krawedzie2[0]);  
    krawedzie2.erase(krawedzie2.begin(), krawedzie2.begin()+1); //usuniecie z wektora pomocniczego najmniejszej krawedzi

    w++;
  }
}

// WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
// WWWWWWWWWWWWW GLOWNY PROGRAM WWWWWWWWWWWWWWWWWWWWWWW
// WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW

int main()
{
  cout << "Program grafy" << endl << endl;

  double difference, difference1; 
  timeval start, stop, start1,stop1;

  ListaS<int> *L=new ListaS<int>;
  MacierzS<int> *M=new MacierzS<int>;
  //M->WczytajZPliku("plik.txt");
  
   do{
    M->GenerujGraf(2000,50);   // graf dla macierzy S
  }while(!(M->CzySpojny()));
  

   //L->GenerujGraf(100,100);    // graf dla listy S

   /*
    cout <<  "ALGORYTM KRUSKALA DLA MACIERZY" << endl << endl;
    gettimeofday(&start,NULL);
    M->SortujKrawedzie();
    M->AlgKruskala();
    gettimeofday(&stop,NULL);
   */
  
   /*
    cout <<  "ALGORYTM KRUSKALA DLA LISTY" << endl << endl;
    gettimeofday(&start1,NULL);
    L->SortujKrawedzie();
    L->AlgKruskala();
    gettimeofday(&stop1,NULL); 	
   */
  
    cout <<  "ALGORYTM PRIMA DLA MACIERZY" << endl << endl;
    gettimeofday(&start,NULL);
    M->AlgPrima();  
    gettimeofday(&stop,NULL);
  
  /*
    cout <<  "ALGORYTM PRIMA DLA LISTY" << endl << endl;
    gettimeofday(&start1,NULL);
    L->AlgPrima();  
    gettimeofday(&stop1,NULL);
  */
  

  //M->ZapiszDoPliku("plik.txt");

    /*cout << endl;
	  for (int w=0; w<M->l_wierzch; w++){
	    for (int k=0; k<M->l_wierzch; k++){
	      if(M->tab2[w][k].waga!=0){
		cout << setw(12) << M->tab2[w][k].waga << " ";
	      }
	      else
		cout << setw(12) << "0" << " ";
	    }
	    cout << endl;
	    }*/

  cout << endl << "ALGORYTM PRIMA DLA MACIERZY  " << endl;
  difference = (stop.tv_sec - start.tv_sec)*1000.0;
  difference += (stop.tv_usec - start.tv_usec) / 1000.0;
  cout << "czas pracy programu: " << difference << "ms" << endl;
  cout << "##################"<< endl;


  /*
  cout << endl << "ALGORYTM KRUSKALA DLA LISTY 100 " << endl;
  difference1 = (stop1.tv_sec - start1.tv_sec)*1000.0;
  difference1 += (stop1.tv_usec - start1.tv_usec) / 1000.0;
  cout << "czas pracy programu: " << difference1 << "ms" << endl;
  cout << "##################"<< endl; 
  */
 return 0;

}
