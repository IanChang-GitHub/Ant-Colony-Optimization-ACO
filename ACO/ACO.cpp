#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <set>
using namespace std;

class tsp_data //資料建立
{
public:
	int city_num = 51;
	vector<double> jump;
	vector< pair<double, double> > city; //城市座標
	vector< vector<double> > distance; //城市間距離
	vector< vector<double> > expect; //期望值，距離倒數
	vector< vector<double> > pheromone;//城市間費洛蒙

	tsp_data()
	{
		//初始化座標
		double x[52] = { 0,37,49,52,20,40,21,17,31,52,51,42,31,5,12,36,52,27,17,13,57,62,42,16,8,7,27,30,43,58,58,37,38,46,61,62,63,32,45,59,5,10,21,5,30,39,32,25,25,48,56,30 };
		double y[52] = { 0,52,49,64,26,30,47,63,62,33,21,41,32,25,42,16,41,23,33,13,58,42,57,57,52,38,68,48,67,48,27,69,46,10,33,63,69,22,35,15,6,17,10,64,15,10,39,32,55,28,37,40 };
		for (int i = 0;i <= city_num;i++)
			city.push_back(make_pair(x[i], y[i]));

		//初始化城市距離,費洛蒙,期望值
		distance.push_back(jump);
		expect.push_back(jump);
		pheromone.push_back(jump);
		for (int i = 1;i <= city_num;i++)
		{
			vector<double> distance_temp(city_num + 1);
			vector<double> pheromone_temp(city_num + 1);
			vector<double> expect_temp(city_num + 1);
			for (int j = 1;j <= city_num;j++)
			{
				distance_temp[j] = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
				pheromone_temp[j] = 0.0003;
				if (distance_temp[j] != 0)
				{
					expect_temp[j] = 1 / distance_temp[j];
				}
			}
			distance.push_back(distance_temp);
			expect.push_back(expect_temp);
			pheromone.push_back(pheromone_temp);
		}
	}

	double evaluate_distance(vector<int> path) //計算總距離
	{
		int count = path.size();
		double total_distance = 0.0;
		count = count - 1;
		for (int i = 0;i < count; i++) {
			total_distance += distance[path[i]][path[i + 1]];
		}
		total_distance += distance[path[count]][path[0]];
		return total_distance;
	}
};

tsp_data tsp;


class Ant
{
public:
	int city_num = 51;
	vector<int> trail; //紀錄軌跡
	set<int> available;//尚未移動過的城市
	double alpha = 1;
	double beta = 2;
	int start = (int)rand() % 51 + 1;
	Ant()
	{
		trail.push_back(start);//設定起點
		for (int i = 1;i <= city_num;i++)
		{
			available.insert(i);
		}
		available.erase(start);
	}

	void leave_pheromone()
	{
		double total_distance = tsp.evaluate_distance(trail);
		int Q = 100;
		double amount = Q / total_distance;
		int count = trail.size();
		count = count - 1;
		for (int i = 0;i < count; i++) {
			tsp.pheromone[trail[i]][trail[i + 1]] += amount;
		}
		tsp.pheromone[trail[count]][trail[0]];
	}

	double evaluate_next_total_pheromone(int current_city) //計算可到的下一城市費洛蒙總合
	{
		int size = available.size();
		double total_pheromone = 0.0;
		for (set<int>::iterator i = available.begin(); i != available.end(); i++) {
			total_pheromone += (pow(tsp.pheromone[current_city][*i], alpha)) * (pow(tsp.expect[current_city][*i], beta));
		}
		return total_pheromone;
	}

	double move_probability(int i, int j, double next_total_pheromone) //計算前往各城市機率
	{
		double probability = (pow(tsp.pheromone[i][j], alpha)) * (pow(tsp.expect[i][j], beta));
		probability = probability / next_total_pheromone;
		return probability;
	}

	vector<int> update() //更新費洛蒙濃度
	{
		leave_pheromone();
		vector<int> temp = trail;
		vector<int> L;
		L.push_back(1);
		trail = L;
		for (int i = 2;i <= city_num;i++)
		{
			available.insert(i);
		}
		return temp;
	}

	void ant_algo()
	{
		int current_city = trail[trail.size() - 1];//現在所在城市
		double next_total_pheromone = evaluate_next_total_pheromone(current_city);
		bool move = false;
		double current_probability = 0,next_probability; //紀錄最高機率
		int city_choose = 0; //選擇前往城市
		double p;
		double random = (double)rand() / (RAND_MAX + 1);

		for (set<int>::iterator i = available.begin(); i != available.end(); i++)
		{
			p = move_probability(current_city, *i, next_total_pheromone);
			next_probability = current_probability + p;
			if (current_probability < random && random <= next_probability)
			{
				city_choose = *i;
				trail.push_back(*i);
				available.erase(i);
				break;
			}
			else
			{
				current_probability = current_probability + p;
			}
		}
	}

};

int main()
{
	vector<int> path; //拜訪順序
	int city_num = 51; //城市數目
	int ant_num = 51; //螞蟻數目
	vector<Ant> ant;
	double evaporation = 0.1;//揮發比例
	double min_distance = 100000000000;//最小距離
	double temp_distance = 0;
	srand(time(NULL));
		for (int i = 0; i < ant_num; i++) //製造螞蟻
		{
			Ant a;
			ant.push_back(a);
		}

		for (int i = 0;i < ;i++) //
		{
			for (int j = 0; j < (city_num - 1); j++)  //移動軌跡
			{
				for (int k = 0;k < ant_num;k++)
				{
					ant[k].ant_algo();
				}
			}
			
			for (int j = 1;j <= city_num;j++)  //費洛蒙蒸發
			{
				for (int k = 1;k <= city_num;k++)
				{
					tsp.pheromone[j][k] *= (1 - evaporation);
				}
			}
			
			for (int i = 0;i < ant_num;i++)
			{
				vector<int> p = ant[i].update();
				/*
				if (path.size() == 0) {
					path = p;
					min_distance = tsp.evaluate_distance(p);
					continue;
				}
				*/
				temp_distance = tsp.evaluate_distance(p);
				if (temp_distance < min_distance)
				{
					min_distance = temp_distance;
					path = p;
				}
				
			}
			//cout << temp_distance << "  " << min_distance << endl;
		}

	cout << "min_distance: " << min_distance << endl;
	for (int i = 0;i < path.size();i++)
	{
		cout << path[i] << " ";
	}
	cout << endl;
	system("pause");

}