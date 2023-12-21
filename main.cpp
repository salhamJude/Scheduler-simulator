#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;
void cmdArgument(int argc, char *argv[]);
void openFiles();
struct node* createNode(struct Process process);
struct node* insertBack(struct node* header,struct Process process);
struct node* deleteFront(struct node* header);
void swap(struct node* a, struct node* b);
void readProcess();
void fcfs_function();
int is_empty(struct node *header);

char *valueF, *valueO;
FILE *infile, *outfile;
struct node* l_header;
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
    int priority;
};
struct node{
    struct Process data;
    struct node *next;
};



int main(int argc, char *argv[]){

    
    cmdArgument(argc,argv);
    openFiles();
    readProcess();
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
node *createNode(Process process)
{
    struct node* temp = NULL;
    temp = (struct node*)malloc(sizeof(node));
    temp->data = process;
    temp->next = NULL;
    return temp;
}
node *insertBack(node *header, Process process)
{
    struct node* temp = createNode(process);
    struct node* headerTemp = header;
    if(header == NULL){
        header = temp;
        return header;
    }
    while (headerTemp->next != NULL)
    {
        headerTemp = headerTemp->next;
    }
    
    headerTemp->next = temp;
    return header;
}
node *deleteFront(node *header)
{
    struct node* temp;
    if(header == NULL)
        return header;
    temp = header;
    header = header->next;
    free(temp);
    return header;
}
void swap(node *a, node *b)
{

    return;
}
void readProcess()
{
    l_header = NULL;

    char processData[100];
    int ds[3];
    int i=0;
    while (fgets(processData, sizeof(processData), infile) != NULL) {
        printf("%s", processData);
        i=0;
        char *data = strtok(processData,":");
         while (data != NULL) {
            printf("Token: %s\n", data);
            data = strtok(NULL, ":");
            ds[i] = atoi(data);
            i++;
        }
        struct Process process;
        process.burst_time = ds[0];
        process.arrival_time = ds[1];
        process.priority = ds[2];

        l_header = insertBack(l_header,process);
    }
    fclose(infile);
    struct node* temp = l_header;
    while (temp->next != nullptr)
    {
        cout << temp->data.burst_time << ":" << temp->data.arrival_time << ":" << temp->data.priority << endl;
        temp = temp->next;
    }

   
    while (l_header != nullptr){
        l_header = deleteFront(l_header);
    }

}
void fcfs_function()
{
    struct Process processes;

}
int is_empty(struct node *header){
    if(header == NULL)
        return 1;
    else 
        return 0;
}