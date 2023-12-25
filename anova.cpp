#include <bits/stdc++.h>

using namespace std;

void load_experiment_data(vector<vector<vector<int>>> *experiments) {
  ifstream fin("hst.csv");

  if (!fin.is_open()) {
    cout << "File error" << endl;
    throw 400;
  }

  vector<vector<int>> experiment;
  string line, cell, num;
  while (fin.good()) {
    experiment.clear();

    vector<int> temp;
    getline(fin, line);
    stringstream s(line);
    while (getline(s, cell, ',')) {
      temp.clear();

      stringstream scell(cell);
      while (getline(scell, num, ';')) {
        if (isnan(stoi(num))) {
          cout << "Invalid input (not a number)" << endl;
          throw 400;
        }
        temp.push_back(stoi(num));
      }
      experiment.push_back(temp);
    }

    if (experiment.size() != 0) {
      experiments->push_back(experiment);
    }
  }

  fin.close();
}

int main() {
  vector<vector<vector<int>>> experiments;
  load_experiment_data(&experiments);

  /*
    Configuration
  */
  int duration_of_exercise = 210;
  double f = 9.16;
  double alfa = 0.1;

  int a = experiments.size();
  int n = experiments[0].size();

  vector<vector<double>> data;
  for (int i = 0; i < a; i++) {
    vector<double> treatment;
    for (int j = 0; j < n; j++) {
      double sum = 0;
      for (int k = 0; k < experiments[0][0].size() / 2; k++) {
        sum += experiments[i][j][k * 2] - experiments[i][j][k * 2 + 1];
      }
      double fitness_index = duration_of_exercise * 100 / (2 * sum);
      cout << "fitnessIndex: " << fitness_index << endl;
      treatment.push_back(fitness_index);
    }
    data.push_back(treatment);
  }
  cout << endl;

  vector<double> average_of_observations;
  int sum;
  double avg;
  double grand_mean_of_all_observations = 0;
  for (int i = 0; i < a; i++) {
    sum = 0;
    for (int j = 0; j < n; j++) {
      sum += data[i][j];
    }
    avg = sum / n;
    grand_mean_of_all_observations += avg;
    average_of_observations.push_back(avg);
  }

  grand_mean_of_all_observations /= n;

  double SStreatments = 0;
  for (int i = 0; i < a; i++) {
    SStreatments +=
        pow((average_of_observations[i] - grand_mean_of_all_observations), 2);
  }
  SStreatments *= n;

  double SSerror = 0;
  for (int i = 0; i < a; i++) {
    for (int j = 0; j < n; j++) {
      SSerror += pow(data[i][j] - average_of_observations[i], 2);
    }
  }

  double MStreatments = SStreatments / (a - 1);
  double MSerror = SSerror / (a * (n - 1));
  double Fo = MStreatments / MSerror;

  cout << "MSTreatments: " << MStreatments << endl;
  cout << "MSerror: " << MSerror << endl;
  cout << "Fo: " << Fo << endl;
  cout << "a: " << a << ", a-1: " << a - 1 << ", a(n-1): " << a * (n - 1)
       << endl;
  cout << "fa,a-1,a(n-1): " << f << " (alfa = " << alfa << ")" << endl;

  if (Fo > f) {
    cout << "Fo is greater than f. Thus, we reject Ho" << endl;
  } else {
    cout << "Fo is less than f. Thus, we accept Ho" << endl;
  }

  return 0;
}