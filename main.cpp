#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

void cmdArgument(int argc, char *argv[]);
void openFiles();
void writeOutput();
struct node* createNode(struct Process process);
struct node* insertBack(struct node* header,struct Process process);
struct node* deleteFront(struct node* header);
void swap(struct Process& a, struct Process& b);
void readProcess();
void fcfs_function();
void sjf_function();
void priority_function();
void rr_function();
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
    int nb_process = 0;
    string out;
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
    int c = 0;
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
        case 3:
            switch (config.method)
            {
                case Scheduling_method::None :
                    goto back;
                    break;
                case Scheduling_method::FSFS :
                    fcfs_function();
                    break;
                case Scheduling_method::SJF :
                    sjf_function();
                    break;
                case Scheduling_method::PRIORITY :
                    priority_function();
                    break;
                case Scheduling_method::RR:
                    rr_function();
                    break;
                default:
                    goto back;
                    break;
            }
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
void writeOutput()
{
    outfile = fopen(valueO,"w+");
    fprintf(outfile,config.out.c_str());
}
node *createNode(Process process)
{
    struct node* temp = NULL;
    temp = (struct node*)malloc(sizeof(node));
    temp->data.arrival_time = process.arrival_time;
    temp->data.burst_time = process.burst_time;
    temp->data.priority = process.priority;
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
void swap(struct Process& a, struct Process& b)
{
    struct Process t;
    t.arrival_time = a.arrival_time;
    t.burst_time = a.burst_time;
    t.priority = a.priority;

    a.arrival_time = b.arrival_time;
    a.burst_time = b.burst_time;
    a.priority = b.priority;

    b.arrival_time = t.arrival_time;
    b.burst_time = t.burst_time;
    b.priority = t.priority;
}
void readProcess()
{
    l_header = NULL;

    char processData[100];
    int ds[3];
    int i=0, j =0;
    while (fgets(processData, sizeof(processData), infile) != NULL) {
        
        printf("%s", processData);
        i=0;
        string val;
        char *data = strtok(processData,":");
         while (data != NULL) {
            val = data;
            data = strtok(NULL, ":");
            ds[i] = stoi(val);
            i++;
        }
        struct Process process;
        process.burst_time = ds[0];
        process.arrival_time = ds[1];
        process.priority = ds[2];
        if(j == 0){
            l_header = createNode(process);
        }else{
            l_header = insertBack(l_header,process);
        }
        j++;
        config.nb_process++;
    }
    fclose(infile);
    struct node* temp = l_header;
    
   /*
    while (temp != nullptr)
    {
        cout << temp->data.burst_time << ":" << temp->data.arrival_time << ":" << temp->data.priority << endl;
        temp = temp->next;
    }

    while (l_header != nullptr){
        l_header = deleteFront(l_header);
    }
    */
}
void fcfs_function()
{
    config.out = "";
    config.out += "Scheduling Method: First Come First Served\n";
    config.out += "Process Waiting Times:\n";
    cout << "Scheduling Method: First Come First Served\n";
    cout << "Process Waiting Times:\n";
    Process processes [config.nb_process];
    struct node* temp = l_header;
    int i = 0;
    
    while (temp != nullptr)
    {
        processes[i].arrival_time = temp->data.arrival_time;
        processes[i].burst_time = temp->data.burst_time;
        processes[i].priority = temp->data.priority;
        temp = temp->next;
        i++;
    }
    
    int waiting_time[config.nb_process];
    float avg =0.0;

    for(int i = 0; i < config.nb_process; i++){
        waiting_time[i]  = 0;
        for (int j = 0; j < i; j++)
        {
            waiting_time[i] += processes[j].burst_time;
        }
        waiting_time[i] -= processes[i].arrival_time;
        avg += waiting_time[i];
        cout << "P" << i << ":" << waiting_time[i] << "ms\n";
        config.out += "P" + std::to_string(i) + ":" + std::to_string(waiting_time[i]) + "ms\n";
    }
    cout << "Average Waiting Time:" << avg/config.nb_process << "ms" << endl;
    config.out += "Average Waiting Time:" + std::to_string(avg/config.nb_process) + "ms\n";
    writeOutput();
}
void sjf_function()
{
    if(config.preemptive_mode){
        config.out = "";
        config.out += "Scheduling Method: Shortest Job First - Preemptive\n";
        config.out += "Process Waiting Times:\n";    
        cout << "Scheduling Method: Shortest Job First - Preemptive\n";
        cout << "Process Waiting Times:\n";
        vector<Process> processes;
        int current_time = 0; 
        int executed = 1;
        int x = 0;
        struct node* temp = l_header;
        int l =1;
        float avg =0.0;
        while (temp != nullptr)
        {
            struct Process p;
            p.arrival_time = temp->data.arrival_time;
            p.burst_time = temp->data.burst_time;
            p.priority = temp->data.priority;
            p.c = l;
            temp = temp->next;
            l++;
            processes.push_back(p);
        }
        
        int right[config.nb_process];
        int waiting_time[config.nb_process]={0};
        for (int i = 0; i < config.nb_process; i++)
        {
            right[i] = processes[i].arrival_time;
        }
        
        struct Process fp = processes[0];
        struct Process short_job = processes[0];
        processes.erase(processes.begin());
        vector<Process> next_executions;
        vector<int> executions;
        while (true)
        {
            bool gl = true, gl2 = true; 
            for (int i = 0; i < processes.size(); ++i) {
                if (processes[i].arrival_time == current_time) {
                    for (int j = 0; j < next_executions.size(); ++j) {
                        if (next_executions[j].burst_time > processes[i].burst_time) {
                            gl = false;
                            next_executions.insert(next_executions.begin() + j, processes[i]);
                            break;
                        }
                    }
                    if (gl) {
                        next_executions.push_back(processes[i]);
                    }
                }
            }
            
            for(int i = 0; i < next_executions.size(); i++){
                if(short_job.burst_time > next_executions[i].burst_time){
                    struct Process temp = short_job;
                    short_job = next_executions[i];

                    right[temp.c - 1] = current_time;
                    waiting_time[short_job.c - 1] += current_time - right[short_job.c - 1];
                    next_executions.erase(std::next(next_executions.begin(),i));
                    for(int j = 0; j < next_executions.size(); j++){
                        if(temp.burst_time < next_executions[j].burst_time){
                            gl2 = false;
                            next_executions.insert(next_executions.begin() + j, temp);
                            break;
                        }else if(temp.burst_time == next_executions[j].burst_time){
                            gl2 = false;
                            int  d = 0;
                            if(temp.c > next_executions[j].c ){
                                d++;
                            }
                            next_executions.insert(next_executions.begin() + j + d, temp);
                            break;
                        }
                    }
                    if(gl2){
                        next_executions.push_back(temp);
                    }
                    break;
                }
            }
            current_time += 1;
            short_job.burst_time -= 1;
            executions.push_back(short_job.c);
            x += 1;
            if (short_job.burst_time <= 0) {
                if (!next_executions.empty()) {
                    right[short_job.c - 1] = current_time;
                    waiting_time[next_executions[0].c - 1] += current_time - right[next_executions[0].c - 1];
                    short_job = next_executions[0];
                    next_executions.erase(next_executions.begin());
                    executed += 1;
                } else {
                    break;  
                }
            }
        }
        for (int i = 0; i < config.nb_process; i++){
            avg += waiting_time[i];
            cout << "P" << i + 1 << ":" << waiting_time[i] << "ms\n";
            config.out += "P" + std::to_string(i) + ":" + std::to_string(waiting_time[i]) + "ms\n";
        }
        cout << "Average Waiting Time:" << avg/config.nb_process << "ms" << endl;
        config.out += "Average Waiting Time:" + std::to_string(avg/config.nb_process) + "ms\n";
            
    }else{    
        config.out = "";
        config.out += "Scheduling Method: Shortest Job First - Non-Preemptive\n";
        config.out += "Process Waiting Times:\n";    
        cout << "Scheduling Method: Shortest Job First - Non-Preemptive\n";
        cout << "Process Waiting Times:\n";
        vector<Process> processes;
        int current_time = 0; 
        int executed = 1;
        int x = 0;
        struct node* temp = l_header;
        int l =1;
        float avg =0.0;
        while (temp != nullptr)
        {
            struct Process p;
            p.arrival_time = temp->data.arrival_time;
            p.burst_time = temp->data.burst_time;
            p.priority = temp->data.priority;
            p.c = l;
            temp = temp->next;
            l++;
            processes.push_back(p);
        }
        int pref[config.nb_process];
        for (int i = 0; i < config.nb_process; i++)
        {
            pref[i] = i + 1;
        }
        
        struct Process fp = processes[0];
        struct Process short_job = processes[0];
        processes.erase(processes.begin());
        vector<Process> next_executions;
        vector<int> executions;
        while (true) {
            bool gl = true;

            for (int i = 0; i < processes.size(); ++i) {
                if (processes[i].arrival_time == current_time) {
                    for (int j = 0; j < next_executions.size(); ++j) {
                        if (next_executions[j].burst_time > processes[i].burst_time) {
                            gl = false;
                            next_executions.insert(next_executions.begin() + j, processes[i]);
                            break;
                        }
                    }
                    if (gl) {
                        next_executions.push_back(processes[i]);
                    }
                }
            }

            current_time += 1;
            short_job.burst_time -= 1;
            executions.push_back(short_job.c);
            x += 1;

            if (short_job.burst_time <= 0) {
                if (!next_executions.empty()) {
                    short_job = next_executions[0];
                    next_executions.erase(next_executions.begin());
                    executed += 1;
                } else {
                    break;  
                }
            }
        }
        processes.insert(processes.begin(), fp);
        vector<int> start_time(processes.size(), 0);
        int pro = 1;

        for (int i = 0; i < processes.size(); ++i) {
            for (int j = 0; j < executions.size(); ++j) {
                if (executions[j] == pro) {
                    start_time[i] = j;
                    pro += 1;
                    break;
                }
            }
        }
        for (int i = 0; i < processes.size(); ++i) {
            int wt = start_time[i] - processes[i].arrival_time;
                avg += wt;
            cout << "P" << processes[i].c << ": " << wt << "ms" << endl;
            config.out += "P" + std::to_string(processes[i].c) + ":" + std::to_string(wt) + "ms\n";
        }
        cout << "Average Waiting Time:" << avg/config.nb_process << "ms" << endl;
        config.out += "Average Waiting Time:" + std::to_string(avg/config.nb_process) + "ms\n";
        writeOutput();
    }
    
}
void priority_function()
{
    if(config.preemptive_mode){

    }else{
        config.out = "";
        config.out += "Scheduling Method: Shortest Job First - Non-Preemptive\n";
        config.out += "Process Waiting Times:\n";    
        cout << "Scheduling Method: Shortest Job First - Non-Preemptive\n";
        cout << "Process Waiting Times:\n";
        vector<Process> processes;
        int current_time = 0; 
        int executed = 1;
        int x = 0;
        struct node* temp = l_header;
        int l =1;
        float avg =0.0;
        while (temp != nullptr){
            struct Process p;
            p.arrival_time = temp->data.arrival_time;
            p.burst_time = temp->data.burst_time;
            p.priority = temp->data.priority;
            p.c = l;
            temp = temp->next;
            l++;
            processes.push_back(p);
        }
        
        int pref[config.nb_process];
        for (int i = 0; i < config.nb_process; i++)
        {
            pref[i] = i + 1;
        }
        struct Process fp = processes[0];
        struct Process short_job = processes[0];
        processes.erase(processes.begin());
        vector<Process> next_executions;
        vector<int> executions;
        while (true) {
            bool gl = true;

            for (int i = 0; i < processes.size(); ++i) {
                if (processes[i].arrival_time == current_time) {
                    for (int j = 0; j < next_executions.size(); ++j) {
                        if (next_executions[j].priority < processes[i].priority) {
                            gl = false;
                            next_executions.insert(next_executions.begin() + j, processes[i]);
                            break;
                        }
                    }
                    if (gl) {
                        next_executions.push_back(processes[i]);
                    }
                }
            }

            current_time += 1;
            short_job.burst_time -= 1;
            executions.push_back(short_job.c);
            x += 1;
            if (short_job.burst_time <= 0) {
                if (!next_executions.empty()) {
                    short_job = next_executions[0];
                    next_executions.erase(next_executions.begin());
                    executed += 1;
                } else {
                    break;  
                }
            }
        }
        processes.insert(processes.begin(), fp);
        vector<int> start_time(processes.size(), 0);
        int pro = 1;

        for (int i = 0; i < processes.size(); ++i) {
            for (int j = 0; j < executions.size(); ++j) {
                if (executions[j] == pro) {
                    start_time[i] = j;
                    pro += 1;
                    break;
                }
            }
        }
        for (int i = 0; i < processes.size(); ++i) {
            int wt = start_time[i] - processes[i].arrival_time;
                avg += wt;
            cout << "P" << processes[i].c << ": " << wt << "ms" << endl;
            config.out += "P" + std::to_string(processes[i].c) + ":" + std::to_string(wt) + "ms\n";
        }
        cout << "Average Waiting Time:" << avg/config.nb_process << "ms" << endl;
        config.out += "Average Waiting Time:" + std::to_string(avg/config.nb_process) + "ms\n";
        writeOutput();
    }
    
}
void rr_function(){
    config.out = "";
    config.out += "Scheduling Method: Round Robin Scheduling - time_quantum = " + std::to_string(config.time_quantum) + "\n";
    config.out += "Process Waiting Times:\n";    
    cout << "Scheduling Method: Round Robin Scheduling - time_quantum = " + std::to_string(config.time_quantum) + "\n";
    cout << "Process Waiting Times:\n";
    int burst_time[config.nb_process];
    int border[config.nb_process] = {0};
    int left[config.nb_process];
    int right[config.nb_process];

    int waiting_time[config.nb_process]={0};
    float avg =0.0;
    int last = 0;
    Process processes [config.nb_process];
    struct node* temp = l_header;
    int l = 0;
    bool repeat = true;
    while (temp != nullptr)
    {
        processes[l].arrival_time = temp->data.arrival_time;
        processes[l].burst_time = temp->data.burst_time;
        processes[l].priority = temp->data.priority;
        temp = temp->next;
        l++;
    }
    for (int i = 0; i < config.nb_process; i++)
    {
        burst_time[i] = processes[i].burst_time;
        right[i] = processes[i].arrival_time;
    }
    while (repeat)
    {
        for (int i = 0; i < config.nb_process; i++)
        {
            if(burst_time[i] > 0){
                left[i] = last;
                waiting_time[i] += left[i] - right[i];

                last += burst_time[i] - config.time_quantum >= 0 ? config.time_quantum : burst_time[i];
                right[i] = last;
                burst_time[i] = burst_time[i] - config.time_quantum >= 0 ? burst_time[i] - config.time_quantum : 0;
            }
        }
        repeat = false;
        for (int i = 0; i < config.nb_process; i++)
        {
            if(burst_time[i] > 0){
                repeat = true;
                break;
            }
        }
    } 
    for (int i = 0; i < config.nb_process; i++){
        avg += waiting_time[i];
        cout << "P" << i + 1 << ":" << waiting_time[i] << "ms\n";
        config.out += "P" + std::to_string(i) + ":" + std::to_string(waiting_time[i]) + "ms\n";
    }
    cout << "Average Waiting Time:" << avg/config.nb_process << "ms" << endl;
    config.out += "Average Waiting Time:" + std::to_string(avg/config.nb_process) + "ms\n";
    writeOutput();
}
int is_empty(struct node *header)
{
    if(header == NULL)
        return 1;
    else 
        return 0;
}


