#include <iostream>
#include<bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;

#define GEN 32
#define HROMOZOM 32

ofstream outdata;

void tokenize(const std::string &s, const char delim,
            std::vector<std::string> &konfig)
{
    std::string::size_type beg = 0;
    for (auto end = 0; (end = s.find(delim, end)) != std::string::npos; ++end)
    {
        beg = end + 1;
    }

    konfig.push_back(s.substr(beg));
}

bool konfigurisi(string konfig_fajl_naziv, std::vector<std::string> &konfig){

     std::ifstream konfig_fajl(konfig_fajl_naziv);;

     string linija;
     if (!konfig_fajl.is_open()) {
        cerr << "Fajl se ne moze otvoriti: "
             << konfig_fajl_naziv << endl;
        return false;
     }

     while (getline(konfig_fajl, linija)){
        tokenize(linija, ':', konfig);
     }

      konfig_fajl.close();
      return true;
}

void popuni(int &prom, string vrednost){

    if(!(vrednost == ""))
        prom = stoi(vrednost);

}

int nadjiRandom(mt19937_64 &rand_gen)
{
    int num = (rand_gen() % 2);
    return num;
}

// Funkcija za generisanje nasumicnog binarnog stringa duzine n
string generisiBinarniString(int n, mt19937_64 &rand_gen)
{
    string s = "";

    for (int i = 0; i < n; i++) {
        s += to_string(rand_gen() % 2);
    }
    return s;
}

double dekodujHromozom(string kodovani_hromozom, int low, int high){

    /// Dekodovanje hromozoma po formuli sa casa
    double p_norm_prim = 0;
    for(int i = 0; i<HROMOZOM; i++){
       p_norm_prim +=(kodovani_hromozom[i] - '0')*pow(2, -(i+1));
    }
    p_norm_prim += pow(2, -HROMOZOM-1);

    p_norm_prim = p_norm_prim*(high-low) + low;

    return p_norm_prim;
}

void dvotackasto_ukrstanje(vector<pair<double, vector<string>>> &populacija, int prvi, int drugi, mt19937_64 &rand_gen){

    vector<string> roditelj1 = populacija[prvi].second;
    vector<string> roditelj2 = populacija[drugi].second;

    vector<string> dete1;
    vector<string> dete2;

    for(int i=0; i<GEN; i++){
       int prva_tacka = rand_gen()%(HROMOZOM);
       int druga_tacka = rand_gen()%(HROMOZOM);
       if(prva_tacka > druga_tacka)
        swap(prva_tacka,druga_tacka);

      string h1;
      string h2;

      h1+= roditelj1[i].substr(0, prva_tacka);
      h1+=roditelj2[i].substr(prva_tacka, druga_tacka-prva_tacka+1);
      h1+=roditelj1[i].substr(druga_tacka+1, HROMOZOM-druga_tacka);

      h2+= roditelj2[i].substr(0, prva_tacka);
      h2+=roditelj1[i].substr(prva_tacka, druga_tacka-prva_tacka+1);
      h2+=roditelj2[i].substr(druga_tacka+1, HROMOZOM-druga_tacka);

      dete1.push_back(h1);
      dete2.push_back(h2);
   }

   populacija.push_back(make_pair(0.0, dete1));
   populacija.push_back(make_pair(0.0, dete2));

}

int ruletskiTocak(vector<pair<double, vector<string>>> vrednosti, int velicina,  mt19937_64 &rand_gen){

    double suma_f = 0.0;
    double max_f = vrednosti[velicina-1].first;

    for (int i=0; i < velicina; i++){
        suma_f += abs(vrednosti[i].first - (max_f + 1));
    }

    // Nasumican broj u intervalu od 0.0 do sume fitnesa
    double random_broj = static_cast <double> (rand_gen()) /( static_cast <double> (rand_gen.max()/suma_f));

    int index_odabranog=0;
    double parcijalna_suma=abs(vrednosti[0].first - (max_f + 1));

    while (random_broj > parcijalna_suma)
    {
        index_odabranog++;
        parcijalna_suma += abs(vrednosti[index_odabranog].first - (max_f + 1));
    }

    return index_odabranog;
}

void mutiraj(vector<pair<double, vector<string>>> &populacioni_set, int index, mt19937_64 &rand_gen){

    double verovatnoca = 0.08;
    for(int i = 0; i<GEN; i++){
          for(int j = 0; j<HROMOZOM; j++){
            double v_b = ((double)rand_gen()/(rand_gen.max()));
            if(v_b<verovatnoca){
                if(populacioni_set[index].second[i][j] == '1')
                    populacioni_set[index].second[i][j] = '0';
                else
                    populacioni_set[index].second[i][j] = '1';
            }
        }
    }

}

bool otvori_fajl(string putanja){

    outdata.open(putanja);
    if( !outdata ) {
      cerr << "Fajl se ne moze otvoriti" << endl;
      return false;
   }

   return true;
}

double funkcija(string tezinski_koef){

    std::stringstream stream;

    stream << "\"C:\\Users\\mijan\\OneDrive\\Desktop\\CPP\\l\""
    << " "
    << tezinski_koef;
    system(stream.str().c_str());

    // citanje vrednosti iz fajla
    std::ifstream fajl;
    string sadrzaj;
    fajl.open("vrednost.txt");
    if ( fajl.is_open() ) {
        fajl >> sadrzaj;
    }
    fajl.close();
    return stof(sadrzaj);

}

int main(int argc, char* argv[])
{
    /// Konfiguracija - podrazumevane vrednosti ukoliko se ne unesu
    int broj_pokretanja = 3; //1
    int velicina_populacije = 50; //2
    int broj_jedinki_za_parenje = 25; //2
    int broj_jedinki_za_mutaciju = 11; //3
    double procenat_odsecanja = 0.3; //4
    int num_param_low = -3; //5
    int num_param_high = 3; //6
    int rand_seed = 100; //7
    string putanja_do_izlazne_datoteke = "izlaz.txt"; //8

    string naziv_konfig_fajla;
    vector<string> konfig;

    if(argc < 2)
        naziv_konfig_fajla = "konfiguracioni_parametri.txt";
    else
        naziv_konfig_fajla = argv[1];

    if(konfigurisi(naziv_konfig_fajla, konfig)){
        /// popunjavanje konfigurisucih parametara
        popuni(broj_pokretanja, konfig[0]);
        popuni(velicina_populacije, konfig[1]);
        popuni(broj_jedinki_za_parenje, konfig[2]);
        popuni(broj_jedinki_za_mutaciju, konfig[3]);
        popuni(num_param_low, konfig[5]);
        popuni(num_param_high, konfig[6]);
        popuni(rand_seed, konfig[7]);

        if(konfig[4] != "")
            procenat_odsecanja = stof(konfig[4]);

        if(konfig[8] != "")
            putanja_do_izlazne_datoteke = konfig[8];
    }

    bool izlazni_fajl = false;
    if(otvori_fajl(putanja_do_izlazne_datoteke)){
        cout<<"Ispis u izlaznu datoteku: "<<putanja_do_izlazne_datoteke<<"\n";
        izlazni_fajl = true;
    }

    mt19937_64 rand_gen(rand_seed);

    for (int pokretanje = 0; pokretanje<broj_pokretanja; pokretanje++){

        string out_start("Pokretanje broj ");
        out_start.append(to_string(pokretanje+1));
        out_start.append("\n");
         if(izlazni_fajl)
                outdata<<out_start;
            else
                cout<<out_start;


        vector<pair<double,vector<string>>> populacioni_set;

        for(int i = 0; i<velicina_populacije; i++){
            vector<string> pomocni;
            for(int j = 0; j<GEN; j++){
                pomocni.push_back(generisiBinarniString(HROMOZOM, rand_gen));
            }
            populacioni_set.push_back(make_pair(0.0, pomocni));
        }

         for(int i = 0; i<velicina_populacije; i++){
            string tezinski_koef;
            for(int j = 0; j<GEN; j++){
                tezinski_koef += to_string(dekodujHromozom(populacioni_set[i].second[j], num_param_low, num_param_high));
                tezinski_koef += " ";
            }
            populacioni_set[i].first = funkcija(tezinski_koef);

         }
        sort(populacioni_set.begin(), populacioni_set.end());

        /// Odavde krecu nase generacije
        for(int g = 0; g<150; g++){
            string out("");
            /// Predselekcija
            /// Procentualno

            for(int i = 1; i<velicina_populacije*procenat_odsecanja; i++){
                populacioni_set.pop_back();
            }

            /// Selekcija za ukrstanje
            /// ruletski tocak

            /// biraju se po dve jedinke za parenje dok se ne dostigne uneti parametar za broj jedinki za parenje
            for(int i = 0; i<broj_jedinki_za_parenje/2;)
            {
                    int roditelj1 = ruletskiTocak(populacioni_set, populacioni_set.size(), rand_gen);
                    int roditelj2 = ruletskiTocak(populacioni_set, populacioni_set.size(), rand_gen);

                    if(roditelj1 != roditelj2){

                        dvotackasto_ukrstanje(populacioni_set, roditelj1, roditelj2, rand_gen);
                        for(int k = populacioni_set.size()-2; k<populacioni_set.size(); k++){
                            string tk = "";
                            for(int j = 0; j<GEN; j++){
                                tk += to_string(dekodujHromozom(populacioni_set[k].second[j], num_param_low, num_param_high));
                                tk += " ";
                            }
                            populacioni_set[k].first = funkcija(tk);
                         }
                         i++;
                    }
            }
            /// sortiramo pre mutacije, da bismo mutirali one koji su najgore prilagodjeni da mozda dobijemo nesto bolje
            sort(populacioni_set.begin(), populacioni_set.end());

            for(int i=populacioni_set.size()-1;i>=populacioni_set.size() - broj_jedinki_za_mutaciju; i--){
                mutiraj(populacioni_set, i, rand_gen);
                string tk = "";
                for(int j = 0; j<GEN; j++){
                    tk += to_string(dekodujHromozom(populacioni_set[i].second[j], num_param_low, num_param_high));
                    tk += " ";
                }
                populacioni_set[i].first = funkcija(tk);
            }

            sort(populacioni_set.begin(), populacioni_set.end());

            int j = populacioni_set.size();
            for(; j>velicina_populacije; j--){
                populacioni_set.pop_back();
            }

            out.append(to_string(g));
            out.append(". ");
            out.append(to_string(populacioni_set[0].first));
            out.append(" ");

            double srednja_vrednost = 0.0;
            for(int i = 0; i<populacioni_set.size(); i++){
                srednja_vrednost += populacioni_set[i].first;
            }
            out.append(to_string(srednja_vrednost/populacioni_set.size()));
            out.append("\n");
            if(izlazni_fajl)
                outdata<<out;
            else
                cout<<out;
        }
        string out_hrom("");
        out_hrom.append("Sastav hromozoma - kodovano\n");
        int br = 0;
        for(int i = 0; i<GEN; i++){
            out_hrom.append(populacioni_set[0].second[i]);
            out_hrom.append(" ");
            br++;
        }
        br = 0;
        out_hrom.append("\nSastav hromozoma - dekodovano\n");
        for(int i = 0; i<GEN; i++){
            out_hrom.append(to_string(dekodujHromozom(populacioni_set[0].second[i], num_param_low, num_param_high)));
            out_hrom.append(" ");
            br++;
        }
        out_hrom.append("\n");
        if(izlazni_fajl)
            outdata<<out_hrom;
        else
            cout<<out_hrom;
    }
    if(izlazni_fajl)
        outdata.close();

    return 0;
}
