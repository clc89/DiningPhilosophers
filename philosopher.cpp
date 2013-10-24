#include <thread>
#include <mutex>
#include <iostream>

const int THINKING =0;
const int HUNGRY = 1;
const int EATING = 2;
const int N = 5;

std::mutex output;

int philosopher_state[N];
std::mutex state_mutex;

std::mutex forks[N];

void think(int i);
void take_forks(int i);
void eat(int i);
void put_forks(int i);

bool test(int i);

void philosopher(int i);

void think(int i)
{
	output.lock();
	std::cout << "I am philosopher " << i << " and I am thinking" << std::endl;
	output.unlock();
}

void take_forks(int i)
{
	std::unique_lock<std::mutex> locker(state_mutex);
	philosopher_state[i] = HUNGRY;
	locker.unlock();
	std::lock(forks[i], forks[(i+1) % N]);
	locker.lock();
	philosopher_state[i] = EATING;
	locker.unlock();
}

void eat(int i)
{
	output.lock();
	std::cout << "I am philosopher " << i << " and I am eating" << std::endl;
	output.unlock();
}

void put_forks(int i)
{
	std::unique_lock<std::mutex> locker(state_mutex);
	philosopher_state[i] = THINKING;
	locker.unlock();
	forks[i].unlock();
	forks[(i+1) % N].unlock();
}

void philosopher(int i)
{
	while (true) {
		think(i);
		take_forks(i);
		eat(i);
		put_forks(i);
	}
}

bool test(int i)
{
	if (philosopher_state[i] == HUNGRY && (philosopher_state[(i+N-1)%N] != EATING && philosopher_state[(i+1)%N] != EATING)) {
		philosopher_state[i] = EATING;
		return true;
	}
	return false;
}

int main()
{

	std::thread p1(philosopher, 0);
	std::thread p2(philosopher, 1);
	std::thread p3(philosopher, 2);
	std::thread p4(philosopher, 3);
	std::thread p5(philosopher, 4);

	p1.join();

	return 0;
}