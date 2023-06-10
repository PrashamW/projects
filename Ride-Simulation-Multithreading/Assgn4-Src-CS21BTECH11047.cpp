#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <chrono>
#include <random>
#include <queue>
#include <semaphore.h>

using namespace std::chrono;
using namespace std;

typedef struct pass_input
{
    int passenger_no;
    pthread_t pid;
} Pass_Input;

typedef struct car_input
{
    int car_no;
    pthread_t cid;
} Car_Input;

typedef struct time
{
    long int entry_time;
    long int exit_time;
} Time;

// Global variables:
int P, C, k;
int lambdaP, lambdaC;
int no_of_passengers_available;
sem_t num_passengers;
queue<Car_Input *> car_queue;
sem_t *car_sem;
sem_t carq;
FILE *fp2;
sem_t file_write;
auto start_time = high_resolution_clock::now();
Time *passenger_tour_times;
long int *car_tour_times;
long int passenger_avg_tour_time;
long int car_avg_tour_time;

void *passenger(void *pass_in)
{
    Pass_Input *pass_thread = (Pass_Input *)pass_in;
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> d1((double)1 / double(lambdaP));
    int t1 = (int)d1(gen);

    auto time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(time - start_time);
    passenger_tour_times[pass_thread->passenger_no].entry_time = duration.count();

    sem_wait(&file_write);
    fprintf(fp2, "Passenger %d enters the museum at %ld\n", pass_thread->passenger_no + 1, duration.count());
    sem_post(&file_write);

    for (int i = 0; i < k; i++)
    {
        // wander around the musuem
        usleep(1000 * t1);

        time = high_resolution_clock::now();
        duration = duration_cast<microseconds>(time - start_time);

        sem_wait(&file_write);
        fprintf(fp2, "Passenger %d made a ride request at %ld\n", pass_thread->passenger_no + 1, duration.count());
        sem_post(&file_write);

        // check if cars available

        while (true)
        {
            sem_wait(&carq);
            if (car_queue.empty())
            {
                // car not available in car_queue
                sem_post(&carq);
                continue;
            }
            else
            {
                // car not available in car_queue

                //  pop car queue
                Car_Input *c = car_queue.front();
                car_queue.pop();
                sem_post(&carq);

                sem_wait(&file_write);
                fprintf(fp2, "Car %d accepts Passenger %d's request\n", c->car_no + 1, pass_thread->passenger_no + 1);
                sem_post(&file_write);

                // signal corresponding car thread that was waiting on this particular semaphore to start the ride
                sem_post(&car_sem[c->car_no]);

                time = high_resolution_clock::now();
                duration = duration_cast<microseconds>(time - start_time);

                sem_wait(&file_write);
                fprintf(fp2, "Passenger %d started riding at %ld\n", pass_thread->passenger_no + 1, duration.count());
                sem_post(&file_write);

                // ride simulation
                sem_wait(&file_write);
                fprintf(fp2, "Car %d is riding Passenger %d\n", c->car_no + 1, pass_thread->passenger_no + 1);
                sem_post(&file_write);

                usleep(10000);

                sem_wait(&file_write);
                fprintf(fp2, "Car %d has finished Passenger %d's tour\n", c->car_no + 1, pass_thread->passenger_no + 1);
                sem_post(&file_write);

                time = high_resolution_clock::now();
                duration = duration_cast<microseconds>(time - start_time);

                sem_wait(&file_write);
                fprintf(fp2, "Passenger %d finished riding at %ld\n", pass_thread->passenger_no + 1, duration.count());
                sem_post(&file_write);

                break;
            }
        }
    }

    // decrement no_of_available_passengers
    sem_wait(&num_passengers);
    no_of_passengers_available--;
    sem_post(&num_passengers);

    if (no_of_passengers_available == 0)
    {
        for (int i = 0; i < C; i++)
        {
            if (sem_trywait(&car_sem[i]) == -1)
            {
                sem_post(&car_sem[i]);
            }
        }
    }

    time = high_resolution_clock::now();
    duration = duration_cast<microseconds>(time - start_time);
    passenger_tour_times[pass_thread->passenger_no].exit_time = duration.count();

    sem_wait(&file_write);
    fprintf(fp2, "Passenger %d exit the museum at %ld\n", pass_thread->passenger_no + 1, duration.count());
    sem_post(&file_write);

    pthread_exit(0);
}

void *car(void *car_in)
{
    Car_Input *car_thread = (Car_Input *)car_in;
    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<> d2((double)1 / double(lambdaC));
    exponential_distribution<> d3((double)1 / double((lambdaC + lambdaP) / 2));
    int t2 = (int)d2(gen);
    int t3 = (int)d3(gen);

    while (no_of_passengers_available > 0)
    {
        sem_wait(&car_sem[car_thread->car_no]);
        // car has picked up passenger
        if (no_of_passengers_available == 0)
        {
            break;
        }
        usleep(10000); // ride simulation
        // wait after ride
        usleep(1000 * t2);
        // push itself in car queue
        sem_wait(&carq);
        car_queue.push(car_thread);
        sem_post(&carq);

        car_tour_times[car_thread->car_no] = car_tour_times[car_thread->car_no] + 1000 * t3;
    }

    pthread_exit(0);
}

int main()
{
    FILE *fp1 = fopen("inp-params.txt", "r");
    fscanf(fp1, "%d %d %d %d %d", &P, &C, &lambdaP, &lambdaC, &k);
    fclose(fp1);

    no_of_passengers_available = P;

    Pass_Input **pass_in = (Pass_Input **)malloc(sizeof(Pass_Input *) * P);
    Car_Input **car_in = (Car_Input **)malloc(sizeof(Car_Input *) * C);
    car_sem = (sem_t *)malloc(sizeof(sem_t) * C);
    passenger_tour_times = (Time *)malloc(sizeof(Time) * P);
    car_tour_times = (long int *)malloc(sizeof(long int) * C);

    sem_init(&num_passengers, 0, 1);
    sem_init(&carq, 0, 1);
    sem_init(&file_write, 0, 1);
    for (int i = 0; i < C; i++)
    {
        sem_init(&car_sem[i], 0, 0);
    }

    fp2 = fopen("output.txt", "w");

    // create P passenger threads
    for (int i = 0; i < P; i++)
    {
        pass_in[i] = (Pass_Input *)malloc(sizeof(Pass_Input));
        pass_in[i]->passenger_no = i;
        pthread_create(&pass_in[i]->pid, NULL, passenger, pass_in[i]); // creates the thread and calls passenger function
    }

    // create C car threads
    for (int i = 0; i < C; i++)
    {
        car_in[i] = (Car_Input *)malloc(sizeof(Car_Input));
        car_in[i]->car_no = i;
        car_queue.push(car_in[i]);
        pthread_create(&car_in[i]->cid, NULL, car, car_in[i]); // creates the thread and calls car function
    }

    // wait for passenger threads to exit
    for (int i = 0; i < P; i++)
    {
        pthread_join(pass_in[i]->pid, NULL);
    }

    // wait for car threads to exit
    for (int i = 0; i < C; i++)
    {
        pthread_join(car_in[i]->cid, NULL);
    }

    fprintf(fp2, "\n");

    for (int i = 0; i < P; i++)
    {
        passenger_avg_tour_time = passenger_avg_tour_time + passenger_tour_times[i].exit_time - passenger_tour_times[i].entry_time;
    }
    passenger_avg_tour_time = passenger_avg_tour_time / P;

    fprintf(fp2, "Passenger average tour time: %ld\n", passenger_avg_tour_time);

    for (int i = 0; i < C; i++)
    {
        car_avg_tour_time = car_avg_tour_time + car_tour_times[i];
    }
    car_avg_tour_time = car_avg_tour_time / C;

    fprintf(fp2, "Car average tour time: %ld\n", car_avg_tour_time);

    fclose(fp2);

    sem_destroy(&num_passengers);
    sem_destroy(&carq);
    sem_destroy(&file_write);
    for (int i = 0; i < C; i++)
    {
        sem_destroy(&car_sem[i]);
    }

    return 0;
}