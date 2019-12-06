#include<iostream>
#include<stdio.h>
#include<time.h>
#include<vector>
#include<cmath>
#include<map>

using namespace std;

//COSAS	QUE NECESITO
//RECIBIR UN MATRIZ
//CREAR UNA MATRIZ DE ESTADOS
//CREAR UNA MATRIZ DE CALCULOS
//DAR EL RESULTADO EN FORMATO DE UP,DOWN,LEFT AND RIGTH

//COSAS AVANZADAS
//TOMAR UNA IMAGEN Y GENERAR UNA IMAGEN QUE RESALTE OBSTACULOS(RECONTRA YUCA)
//GENERAR UNA MATRIZ A PARTIR DE LA IMAGEN TRATADA

int episodes = 58;
int Maximo(vector<vector<double> > &Q, int a) {
	int maxi = 0;
	for (int i = 0; i < (int)Q[a].size(); ++i) {
		if (Q[a][i] > maxi)maxi = Q[a][i];
	}
	return maxi;
}

int randomAction(vector<vector<int> >& R,int S) {
	vector<int> actions;
	for (int i = 0; i < (int)R[S].size(); ++i) {
		if (R[S][i] > -1)actions.push_back(i);
	}
	int r = rand() % actions.size();
	return actions[r];
}

int indexOfMin(vector<vector<double> >& Q, int a) {
	int mini = 1e9;
	int ind = -1;
	for (int i = 0; i < (int)Q[a].size(); ++i) {
		//cout << Q[a][i] << endl;
		if (Q[a][i]<mini && Q[a][i]>-1) {
			mini = Q[a][i];
			ind = i;
		}
	}
	return ind;
}

void limpiar(vector<vector<double> >& Q, int a) {
	for (int i = 0; i < (int)Q[a].size(); ++i) {
		Q[a][i] = -1;
		Q[i][a] = -1;
	}
	return;
}

int main() {
	srand(time(NULL));
	int ini = 0;
	int fin = 0;
	int r = 0;
	int c = 0;
	cin >> r >> c;
	vector<vector<int> > Enviroment(r, vector<int>(c, 0));
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j) {
			cin >> Enviroment[i][j];
		}
	}
	cin >> ini >> fin;
	vector<vector<int> > R(r * c, vector<int>(c * r, -1));
	int ii = 0;
	map<int, pair<int, int> > Malla;
	for (int i = r-1; i > -1 ; --i) {
		//cout << "i" << endl;
		for (int j = 0; j < c; ++j) {
			if (i - 1 > -1 && !Enviroment[i - 1][j]) { R[ii * c + j][((ii + 1) * c + j)] = ((((ii + 1) * c + j) == fin) ? 100 : 0);  }//UP
			if (i + 1 < r && !Enviroment[i + 1][j]) { R[ii * c + j][((ii - 1) * c + j)] = ((((ii - 1) * c + j) == fin) ? 100 : 0); }//DOWN
			if (j - 1 > -1 && !Enviroment[i][j - 1]) { R[ii * c + j][(ii * c + j) - 1] = ((((ii * c) + j - 1) == fin) ? 100 : 0); }//LEFT
			if (j + 1 < c && !Enviroment[i][j + 1]) { R[ii * c + j][(ii * c + j) + 1] = ((((ii * c) + j + 1) == fin) ? 100 : 0); }//RIGHT
			//cout << ii * c + j << endl;
		}
		++ii;
	}
	int jj = 0;
	for (int i = r - 1; i > -1; --i) {
		for (int j = 0; j < c; ++j) {
			Malla[jj * r + j] = { i,j };
		}
		++jj;
	}
	vector<vector<double> > Q(r * c, vector<double>(c * r, -1));
	/*for (int i = 0; i < r * c; ++i)cout << 0 << ' ' << '\t';
	cout << endl;
	for (int i = 0; i < r * c; ++i) {
		cout << i << ':' << '\t' << ' ';
		for (int j = 0; j < r * c; ++j) {
			cout << R[i][j] << ' '<<'\t';
		}
		cout << endl;
	}*/
	/*for (auto it = Malla.begin(); it != Malla.end(); ++it) {
		cout << it->second.first << ' ' << it->second.second <<'='<<it->first<<endl;
	}*/
	///INICIA ALGORITMO
	cout << "Begin the training\n";
	int S = 0;
	int ra = 0;
	double y = 0.7;
	while (episodes) {
		do { S = rand() % (r * c); } while (Enviroment[Malla[ra].first][Malla[ra].second] != 0);
		//cout << "S inicial " << S << endl;
		do {
			ra = randomAction(R,S);
			if (R[S][ra] == -1)cout << "AHHHHHHHHHHHHHHHHHHHHHHHH\n";
			Q[S][ra] = (double)R[S][ra] + (double)(y * Maximo(Q,ra));
			S = ra;
			//cout << S << endl;
		} while (S != fin);
		--episodes;
	}
	/*for (int i = 0; i < r * c; ++i) {
		for (int j = 0; j < r * c; ++j) {
			cout << Q[i][j] << ' ' << '\t';
		}
		cout << endl;
	}*/
	//Construir camino
	cout << "Empieza construcción del camino" << endl;
	S = ini;
	vector<int> path;
	while ((int)path.size() < (r * c) && S!=-1) {
		path.push_back(S);
		S = indexOfMin(Q, S);
		//cout << S << endl;
		limpiar(Q, path.back());
		//cout << "-------------------------------------------------------------------------------------------\n";
		/*for (int i = 0; i < r * c; ++i) {
			for (int j = 0; j < r * c; ++j) {
				cout << Q[i][j] << ' ' << '\t';
			}
			cout << endl;
		}*/
	}
	for (int i = 0; i < (int)path.size(); ++i) {
		cout << path[i]+1 << ' ';
	}
	vector<unsigned int> ins;
	for (int i = 1; i < (int)path.size(); ++i) {
		if (path[i] - path[i - 1] == 1)ins.push_back(2);
		else if (path[i] - path[i - 1] > 1)ins.push_back(1);
		else if (path[i] - path[i - 1] < 0)ins.push_back(3);
	}
	cout << endl;
	for (int i = 0; i < (int)ins.size(); ++i) {
		cout << ins[i] << ' ';
	}
	return 0;
}