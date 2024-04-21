#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

//定义进程PCB
typedef struct {
    int ID;                       //进程ID
    int arr_time;               //进程到达时间
    int ser_time;               //进程服务时间
    int sub_time;             //进程提交时间
    int fin_time;             //进程完成时间
    int priority;              //进程优先级
    int cyc_time = 0;           //周转时间
    float autcyc_time = 0;      //带权周转时间
    int last_sertime;             //剩余服务时间
    bool flag = false;      //表示进程是否是第一次执行
}PCB;

//计算周转时间
float CalcuTime(vector<PCB>& pcb) {
    int num = pcb.size();
    float sum = 0;
    for (int i = 0; i < num; i++) {
        pcb[i].cyc_time = pcb[i].fin_time - pcb[i].sub_time;
        pcb[i].autcyc_time = (float)pcb[i].cyc_time / pcb[i].ser_time;
        sum += pcb[i].cyc_time;
    }
    return  sum / num;
}

//初始化进程
void CreatePCB(vector<PCB>& ptr) {
    int num = ptr.size();
    cout << "请输入以下信息（中间以空格分隔）" << endl;
    cout << "进程ID";
    cout.width(16); cout << "进程到达时间";
    cout.width(16); cout << "进程服务时间";
    cout.width(16); cout << "进程优先级" << endl;
    for (int i = 0; i < num; i++) {
        cin >> ptr[i].ID >> ptr[i].arr_time >> ptr[i].ser_time >> ptr[i].priority;
    }
}

//打印进程信息
void PrintPCB(vector<PCB>& ptr, bool flag) {
    int num = ptr.size();
    if (flag == false) {
        cout << "进程的初始信息为：" << endl;
        cout << "进程ID";
        cout.width(16); cout << "进程到达时间";
        cout.width(16); cout << "进程服务时间";
        cout.width(16); cout << "进程优先级" << endl;
        for (int i = 0; i < num; i++) {
            cout << ptr[i].ID;
            cout.width(16); cout << ptr[i].arr_time;
            cout.width(16); cout << ptr[i].ser_time;
            cout.width(16); cout << ptr[i].priority << endl;
        }
    }
    else {
        cout << "进程ID";
        cout.width(16); cout << "进程到达时间";
        cout.width(16); cout << "进程提交时间";
        cout.width(16); cout << "进程服务时间";
        cout.width(16); cout << "进程完成时间";
        cout.width(16); cout << "进程周转时间";
        cout.width(24); cout << "进程带权周转时间" << endl;
        for (int i = 0; i < num; i++) {
            cout << ptr[i].ID;
            cout.width(16); cout << ptr[i].arr_time;
            cout.width(16); cout << ptr[i].sub_time;
            cout.width(16); cout << ptr[i].ser_time;
            cout.width(16); cout << ptr[i].fin_time;
            cout.width(16); cout << ptr[i].cyc_time;
            cout.width(16); cout << ptr[i].autcyc_time << endl;
        }
    }
}

//比较函数，根据进程的到达时间进行排序
bool comp_arrtime(PCB p1, PCB p2) {
    if (p1.arr_time == p2.arr_time)
        return p1.ser_time < p2.ser_time;
    return p1.arr_time < p2.arr_time;
}

//FCFS算法，通过排序，将进程按到达时间从小到大进行排序
void FCFS(vector<PCB> ptr) {
    int num = ptr.size();
    sort(ptr.begin(), ptr.end(), comp_arrtime);
    int t = ptr[0].arr_time;
    int t0 = ptr[0].arr_time;
    for (int i = 0; i < num - 1; i++) {
        t = t0;
        ptr[i].sub_time = t;
        ptr[i].fin_time = ptr[i].sub_time + ptr[i].ser_time;
        t = t + ptr[i].ser_time;
        t0 = max(t, ptr[i + 1].arr_time);
    }
    ptr[num - 1].sub_time = t0;
    ptr[num - 1].fin_time = ptr[num - 1].sub_time + ptr[num - 1].ser_time;
    float avecyc_time = CalcuTime(ptr);
    cout << "FCFS调度已完成" << endl;
    PrintPCB(ptr, true);
    cout << "进程的平均周转时间：" << avecyc_time << endl;
}
//比较函数，根据进程的服务时间进行排序
bool comp_sertime(PCB p1, PCB p2) {
    if (p1.ser_time == p2.ser_time)
        return p1.arr_time < p2.arr_time;
    return p1.ser_time < p2.ser_time;
}

//短作业优先算法
void SJF(vector<PCB> pcb) {
    int num = pcb.size();
    //先根据进程的到达时间进行排序，如果到达时间相同，则运行所需时间短的排在前面
    sort(pcb.begin(), pcb.end(), comp_arrtime);
    int now_t = pcb[0].arr_time;
    int t = pcb[0].arr_time;
    for (int i = 0; i < num; i++) {
        now_t = t;
        pcb[i].sub_time = now_t;
        pcb[i].fin_time = pcb[i].sub_time + pcb[i].ser_time;
        int k = 0;                      //计数在一个进程运行期间，有几个进程到了就绪队列
        for (int j = i + 1; j < num; j++) {
            if (pcb[j].arr_time <= pcb[i].fin_time)
                k++;
        }
        int temp = i + 1;
        //将到达的进程按照运行所需时间从小到大的顺序进行排序
        sort(pcb.begin() + temp, pcb.begin() + temp + k, comp_sertime);
        if (i + 1 < num)
            t = max(pcb[i].fin_time, pcb[i + 1].arr_time);
    }
    float avecyc_time = CalcuTime(pcb);
    cout << "SJF调度已完成" << endl;
    PrintPCB(pcb, true);
    cout << "进程的平均周转时间：" << avecyc_time << endl;
}

//RR轮转调度算法
void RR(vector<PCB> pcb, int time_p) {
    int num = pcb.size();
    sort(pcb.begin(), pcb.end(), comp_arrtime);            //将进程按照到达时间从小到大进行排列
    for (int i = 0; i < num; i++)                                    //初始化进程的剩余执行时间
        pcb[i].last_sertime = pcb[i].ser_time;
    queue<int> wait_q;                                               //等待队列
    int t = pcb[0].arr_time;
    wait_q.push(0);                                                     //第一个进程入队
    while (!wait_q.empty()) {                                       //当队列非空
        int t0 = t;                                         //表示一个进程开始执行时的本地时间
        int run = wait_q.front();
        wait_q.pop();
        if (pcb[run].flag == false) {               //如果进程是第一次执行，则初始化进程的提交时间
            pcb[run].sub_time = t;
            pcb[run].flag = true;                     //修改标志
        }
        t = t + min(pcb[run].last_sertime, time_p);           //t表示原来的时间加上时间片和进程剩余执行时间的最小值
        pcb[run].fin_time = t;                                       //更新每次进程执行结束的时间
        pcb[run].last_sertime = pcb[run].last_sertime - min(pcb[run].last_sertime, time_p);
        if (t0 < pcb[num - 1].arr_time) {                //表示当前有进程到达，但还没有进入就绪队列
            for (int i = 0; i < num; i++)                 //将当前未进就绪队列的进程入队
                if (pcb[i].arr_time > t0 && pcb[i].arr_time <= t)
                    wait_q.push(i);
        }
        if (pcb[run].last_sertime != 0)               //如果当前进程已经执行完毕，则不用再进队
            wait_q.push(run);
    }
    float avecyc_time = CalcuTime(pcb);
    cout << "RR轮转调度已完成" << endl;
    PrintPCB(pcb, true);
    cout << "进程的平均周转时间：" << avecyc_time << endl;
}

//根据优先级对进程进行排序，优先级相同的，则按照到达时间进行排序
bool pri(PCB p1, PCB p2) {
    if (p1.priority == p2.priority)
        return p1.arr_time < p2.arr_time;
    return p1.priority < p2.priority;
}
//优先级调度算法
void PSA(vector<PCB> pcb) {
    vector<PCB> copy;
    for (int i = 0; i < pcb.size(); i++)
        pcb[i].last_sertime = pcb[i].ser_time;
    sort(pcb.begin(), pcb.end(), pri);
    int time = pcb[0].arr_time;
    while (!pcb.empty()) {
        if (pcb[0].flag == false) {
            pcb[0].sub_time = time;
            pcb[0].flag = true;
        }
        pcb[0].priority++;
        time++;
        pcb[0].last_sertime--;
        if (pcb[0].last_sertime == 0) {
            pcb[0].fin_time = time;
            copy.push_back(pcb[0]);
            pcb.erase(pcb.begin());
        }
        else {
            PCB temp = pcb[0];
            pcb.erase(pcb.begin());
            pcb.push_back(temp);
            sort(pcb.begin(), pcb.end(), pri);
        }
    }
    pcb = copy;
    float avecyc_time = CalcuTime(pcb);
    cout << "PSA调度已完成" << endl;
    PrintPCB(pcb, true);
    cout << "进程的平均周转时间：" << avecyc_time << endl;
}

void menu() {
    int pro_num;
    cout << "请输入进程数："; cin >> pro_num;
    vector<PCB> PCBarrays(pro_num);
    CreatePCB(PCBarrays);
    PrintPCB(PCBarrays, false);
    cout << "请选择进程调度算法：" << endl;
//    cout << "1 先来先服务算法（FCFS）" << endl;
//    cout << "2 短进程优先服务算法（SJF）" << endl;
//    cout << "3 优先级调度算法（PSA）" << endl;
//    cout << "4 时间片轮转算法（RR）" << endl;
	cout << "1 先来先服务算法（FCFS） 短进程优先服务算法（SJF）优先级调度算法（PSA）" << endl;
    cout << "2 时间片轮转算法（RR）" << endl;
    int num;
    cout << "请选择："; cin >> num;
    switch (num) {
//    case 1:FCFS(PCBarrays);
//        break;
//    case 2:SJF(PCBarrays);
//        break;
//    case 3:PSA(PCBarrays);
//        break;
	case 1:{
		FCFS(PCBarrays);
		SJF(PCBarrays);
		PSA(PCBarrays);
        break;}
    case 2:
        int time;
        cout << "请输入时间片："; cin >> time;
        RR(PCBarrays, time);
          break;
//    case 4:
//        int time;
//        cout << "请输入时间片："; cin >> time;
//        RR(PCBarrays, time);
//          break;
    default:"请输入正确的序号!";
    }
}

int main() {
    menu();
    system("pause");
    return 0;
}