#include<iostream>
#include<stdlib.h>
#include<time.h>

int main() {

	const int n = 155;
	const int m = 93;
	const int tezKol = 3;
	const int tezVrste = 5;

	int Hvn[m][tezVrste] = {};
	std::cout << "unesite elemente kontrolne matrice u hv obliku:" << std::endl;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < 5; j++) {
			std::cin >> Hvn[i][j];
		}
	}

	int maxIteracija = 100;
	int brojacNeuspesnogDekodovanja = 0;
	int brojKodnihReci = 0;
	int random;
	int c[n] = {}; //pristigla sekvenca
	int cd[n] = {}; //dekodovana sekvenca
	int s[m] = {}; //sindrom
	int sd[m] = {}; //novi sindrom
	int bu[n] = {}; //broj ucestvovanja odredjenog bita u pogresnim jednacinama parnosti

	int bfParam = 0;
	if (tezKol % 2 == 0) {
		bfParam = tezKol / 2;
	}
	else {
		bfParam = tezKol / 2 + 1;
	}

	int verovatnoca = 50;
	//pgdbf algoritam
	srand(time(NULL));
	while (brojacNeuspesnogDekodovanja < 50 || brojKodnihReci < 1000) {

		//unosenje gresnke u sekvencu 
		for (int i = 0; i < n; i++) {
			random = rand() % verovatnoca;
			if (random > 0) {
				c[i] = 0;
			}
			else {
				c[i] = 1;
			}
		}

		//racunanje sindroma
		for (int i = 0; i < m; i++) {
			int zbir = 0;
			for (int j = 0; j < tezVrste; j++) {
				int d = Hvn[i][j] - 1;
				zbir += c[d];
			}
			s[i] = zbir % 2;
		}

		//provera sindroma pristigle kodne reci
		bool sindromOk = true;
		for (int i = 0; i < m; i++) {
			if (s[i] == 1) {
				sindromOk = false;
				break;
			}
		}

		//izjednacavanje nove sekvence sa starom (pre flipovanja odredjenih bita da bi bile iste)
		for (int i = 0; i < n; i++) {
			cd[i] = c[i];
		}

		if (!sindromOk) {

			int brojacIteracije = 1;
			while (brojacIteracije < maxIteracija) { //20 je maksimalan broj iteracija dekodovanja

				//vracanje broja pojavljivanja bita u kriticnim jednacinama na nulu (u slucaju novih iteracija)
				for (int j = 0; j < n; j++) {
					bu[j] = 0;
				}

				//provera broja pojavljivanja odredjenog bita kodne rece u jednacinama koje ne zadovoljavaju parnost
				for (int i = 0; i < m; i++) {
					if (s[i] == 1) {
						for (int j = 0; j < tezVrste; j++) {
							int k = Hvn[i][j] - 1;
							bu[k]++;
							//gbf
							int gbf = (c[k] + cd[k]) % 2;
							bu[k] = bu[k] + gbf;
						}
					}
				}

				//odredjivalje max broja pojavljivanja odredjenog bita (jednog ili vise njih) (u odnosu na ostale bite)
				int max = 0;
				for (int j = 0; j < n; j++) {
					if (bu[j] > max) {
						max = bu[j];
					}
				}

				//flipovanje bita koji se pojavljuju najvise puta u jednacinama koje ne zadovoljavaju parnost
				for (int j = 0; j < n; j++) {
					if (bu[j] == max) {
						//pgdbf 0.7
						if ((rand() % 10) > 2) {
							cd[j]++;
							cd[j] = cd[j] % 2;
						}
					}
				}

				//racunanje novog sindroma nakon flipovanja nekih bita kodne reci
				for (int i = 0; i < m; i++) {
					int zbir = 0;
					for (int j = 0; j < tezVrste; j++) {
						int k = Hvn[i][j] - 1;
						zbir += cd[k];
					}
					sd[i] = zbir % 2;
				}

				//provera novog sindroma
				sindromOk = true;
				for (int i = 0; i < m; i++) {
					if (sd[i] == 1) {
						sindromOk = false;
						break;
					}
				}

				//ako je novi sindrom jednak svim nulama zavrsavamo proces dekodovanja
				if (sindromOk) {
					brojacIteracije = 300;
				}

				//u slucaju sledece iteracije novi sindrom nam postaje stari 
				for (int i = 0; i < m; i++) {
					s[i] = sd[i];
				}

				//u slucaju da novi sindrom nije jednak svim nulama idemo u sledecu iteraciju
				brojacIteracije++;
			}
		}//kraj algoritma za dekodovanje
		if (sindromOk) {
			brojKodnihReci++;
		}
		else {
			brojacNeuspesnogDekodovanja++;
			brojKodnihReci++;
		}
	}
	std::cout << std::endl;
	std::cout << "broj kodnih reci: " << brojKodnihReci << std::endl;
	std::cout << "broj neuspesnog dekodovanja: " << brojacNeuspesnogDekodovanja;
	std::cout << std::endl << "verovatnoca geske:" << (double)brojacNeuspesnogDekodovanja / brojKodnihReci;

	return 0;

}
