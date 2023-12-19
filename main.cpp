#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
void cmdArgument(int argc, char *argv[]);
void openFiles();
void fcfs_function();
char *valueF, *valueO;
FILE *infile, *outfile;

enum Scheduling_method{
    None = 0,
    FSFS = 1,
    SJF = 2,
    PRIORITY = 3,
    RR = 4
};
struct Config
{
    Scheduling_method method = Scheduling_method::None;
    int preemptive_mode = 0;
    int time_quantum = 0;
    void setMethod(){
        int opt;
       do{

            system("cls");
            cout << "\tChoose a scheduling method" << endl;
            cout << "1) First Come, First Served Scheduling \n"
                << "2) Shortest-Job-First Scheduling \n"
                << "3) Priority Scheduling \n"
                << "4) Round-Robin Scheduling \n" << endl;
            cout << "Option> ";
            cin >> opt;
        }while(opt <= 0 or opt >= 5);

        switch (opt)
        {
        case 1 :
            method = Scheduling_method::FSFS;
            break;
        case 2 :
            method = Scheduling_method::SJF;
            break;
        case 3 :
            method = Scheduling_method::PRIORITY;
            break;
        case 4 :
            method = Scheduling_method::RR;
            cout << "Enter the time quantum : ";
            cin >> time_quantum;
            break;
        }
    }
    void setPreemptive_mode(){
        int opt;
        do{
            system("cls");
            cout << "Set the preemptive mode (1 : ON, 0 : OFF)" << endl;
            cin >> opt;
        }while(opt != 0 && opt != 1);
        preemptive_mode = opt;
    }
} config;
struct Process{
    int burst_time;
    int arrival_time;
    int priorityy;
};



int main(int argc, char *argv[]){

    
    cmdArgument(argc,argv);
    openFiles();
    int menu_opt = 0;
    
    back :
    do{
        string method = (config.method == 0) ? "None"
                        : (config.method == 1) ? "First Come, First Served Scheduling"
                        : (config.method == 2) ? "Shortest-Job-First Scheduling"
                        : (config.method == 2) ? "Priority Scheduling"
                        : "Round-Robin Scheduling";

        system("cls");
        cout << "\tCPU SCHEDULER SIMILATOR" << endl;
        cout << "1) Scheduling method (" << method << ") \n"
             << "2) Preemptive Mode (" << (config.preemptive_mode == 1 ? "ON" : "OFF") << ") \n"
             << "3) Show result \n"

             << "4) End Program \n" << endl;
        cout << "Option> ";
        cin >> menu_opt;
    }while(menu_opt <= 0 or menu_opt >= 5);

    system("cls");
    switch(menu_opt){
        case 1:
            config.setMethod();
            goto back;
        break;
        case 2:
            config.setPreemptive_mode();
            goto back;
        break;
            
    }
    
    return 0;
}

void cmdArgument(int argc, char *argv[])
{
    int opt;
    int  fflag, oflag;
    

    while ((opt = getopt(argc, argv, "f:o:")) != -1) {
        switch (opt) {
            case 'f':
                fflag = 1;valueF =optarg;
                break;
            case 'o':
                oflag = true;valueO = optarg;
                break;
            default: 
                std::cerr << "Usage: " << argv[0] << " [-f value] [-o value]\n";
                exit(EXIT_FAILURE);
        }
    }

    if (!fflag or !oflag) {
        std::cerr << "Usage: " << argv[0] << " [-f value] [-o value]\n";
        exit(EXIT_FAILURE);
    }

    int i = optind;
    while (i < argc)
    {
        cerr << "No aditionnal argument needed" << endl;
        exit(EXIT_FAILURE);
    }
}

void openFiles()
{
    infile = fopen(valueF,"r");
    if(infile == NULL){
        cerr << "Cannot open the input file" << endl;
        exit(EXIT_FAILURE);
    }

}

void fcfs_function()
{

}
