#include <queue>
#include <string>
#include <iostream>
typedef struct{
    std::string tag;    //作业标识
    std::string user;   //用户名称
    unsigned int id;    //用户id
    int type;   //作业类型（0、cpu繁忙，1、I/O繁忙，2、批量型，3、终端型）
    int status; //作业状态
    //调度信息
    int priority;   //优先级
    int up_time;   //提交时间
    int run_time;
    //资源需求
    int begin_time; //开始执行时间
    int value_time; //预计运行时间
    int end_time;   //结束时间
    int size_memo;  //要求内存大小
    int flag;
    bool runtrue;
}PCB;

std::queue <PCB> PCB_Queue; //后备队列
std::queue <PCB> ready_queue;   //就绪队列
std::queue <PCB> pro_queue;
int m_system_time = 0;
int m_job_num = 0;
int m_time_slice = 0;

int main()
{
    void input();    
    void FCFS();
    void SJF();
    void PSA();
    void RRO();
    void statisticalData();

    input();
    std::cout<<"select scheduling algorithm:";
    int select;
    std::cin>>select;
    switch (select)
    {
    case 0:
        FCFS();
        /* code */
        break;
    case 1:
        SJF();
        break;
    case 2:
        PSA();
        break;
    case 3:
        std::cout<<"time slise:";
        std::cin>>m_time_slice;
        RRO();
        break;


    default:
        break;
    }

    statisticalData();

    return 0;
}
void RRO()
{
    //时间片轮转
    void runRro(PCB job);
    std::queue<PCB>tempPcb;
    while(!PCB_Queue.empty()||!tempPcb.empty()||!ready_queue.empty())
    {
        
       // PCB_Queue.pop();    //从后备队列删除该作业
        if(PCB_Queue.front().up_time==m_system_time)
        {
            tempPcb.push(PCB_Queue.front());
            PCB_Queue.pop();
        }
        if(!ready_queue.empty())
        {
            tempPcb.push(ready_queue.front());
            ready_queue.pop();
        }
        runRro(tempPcb.front()); //运行后备队列中第一个作业，
        tempPcb.pop();
    }



}
void runRro(PCB job)
{
       //进程运行
    std::cout<<"run job "<<job.tag<<std::endl;
    //m_system_time+=
    if(!job.runtrue)
    {
        job.begin_time = m_system_time;
        job.runtrue = true;
    }
    
    job.end_time = m_system_time+m_time_slice;
    m_system_time+= m_time_slice;
    job.run_time++;
    if(job.run_time<job.value_time)
    {
        ready_queue.push(job);  //放入就绪队列
    }else{
        pro_queue.push(job);
    }
   
    
}
void PSA()
{
    //优先级0最优先
    void run(PCB job);
    PCB tempPCB[PCB_Queue.size()];
    int n = PCB_Queue.size();
    for(int i=0;i<n;i++)
    {
        tempPCB[i] = PCB_Queue.front();
        PCB_Queue.pop();
    }
    int fl = 0;
    for(int i=0;i<n-1;i++)
    {
        for(int j=n-1;j>i;j--)
        {
            if(tempPCB[j-1].priority>tempPCB[j].priority)
            {
                PCB temp = tempPCB[j];
                tempPCB[j] = tempPCB[j-1];
                tempPCB[j-1] = temp;
                fl = 1;
            }
        }
        if(fl == 0)
            break;
    }
    for(int i=0;i<n;i++)
    {
        std::cout<<tempPCB[i].value_time<<std::endl;
        run(tempPCB[i]);
    }

}

void SJF()
{
    void run(PCB job);
    //短作业优先
    PCB tempPCB[PCB_Queue.size()];
    int n = PCB_Queue.size();
    for(int i=0;i<n;i++)
    {
        tempPCB[i] = PCB_Queue.front();
        PCB_Queue.pop();
    }
    int fl = 0;
    for(int i=0;i<n-1;i++)
    {
        for(int j=n-1;j>i;j--)
        {
            if(tempPCB[j-1].value_time>tempPCB[j].value_time)
            {
                PCB temp = tempPCB[j];
                tempPCB[j] = tempPCB[j-1];
                tempPCB[j-1] = temp;
                fl = 1;
            }
        }
        if(fl == 0)
            break;
    }
    for(int i=0;i<n;i++)
    {
        //std::cout<<tempPCB[i].value_time<<std::endl;
        run(tempPCB[i]);
    }
    

}

void FCFS()
{
    void run(PCB job);
    //先来先服务
    //PCB temp = PCB_Queue.front();
    while(!PCB_Queue.empty())
    {
        run(PCB_Queue.front()); //运行后备队列中第一个作业，
        PCB_Queue.pop();    //从后备队列删除该作业
    
    }
}

void run(PCB job)
{
    //进程运行
    std::cout<<"run job "<<job.tag<<std::endl;
    //m_system_time+=
    job.begin_time = m_system_time;
    job.end_time = job.begin_time+job.value_time;
    m_system_time = job.end_time;
    ready_queue.push(job);  //放入就绪队列
}

void statisticalData()
{
    //计算完成、周转时间等
    /****
     * 周转时间 = 完成时间-到达时间
     * 带权周转时间 = 周转时间/服务时间
     * 
     * ***/

    std::cout<<"tag\tbegintime\tend_time\tcycling\tweighttime"<<std::endl;
    if(m_time_slice!=0)
    {
        //使用的时间片轮转
        while(!pro_queue.empty())
        {
            std::cout<<pro_queue.front().tag<<"\t"<<pro_queue.front().begin_time<<"\t"<<pro_queue.front().end_time
            <<"\t"<<pro_queue.front().end_time-pro_queue.front().up_time
            <<"\t"<<(pro_queue.front().end_time-pro_queue.front().up_time)/pro_queue.front().value_time<<std::endl;
            pro_queue.pop();
        }
    }else{
        while(!ready_queue.empty())
        {
            std::cout<<ready_queue.front().tag<<"\t"<<ready_queue.front().begin_time<<"\t"<<ready_queue.front().end_time
            <<"\t"<<ready_queue.front().end_time-ready_queue.front().up_time
            <<"\t"<<(ready_queue.front().end_time-ready_queue.front().up_time)/ready_queue.front().value_time<<std::endl;
            ready_queue.pop();
        }
    }

}

void input()
{
    //将用户从键盘提交的作业放入后备队列中
    PCB temp;
    temp.user = "admin";
    temp.id = 'adminsystem';
    temp.status = 0;
    temp.run_time = 0;
    temp.runtrue = false;
    std::cout<<"job numbers:";
    std::cin>>m_job_num;
    int i=0;
    while(i<m_job_num)
    {
        std::cout<<"inout tag priority value_time uptime"<<std::endl;
        std::cin>>temp.tag>>temp.priority>>temp.value_time>>temp.up_time;
        //std::cout<<temp.tag<<temp.type<<temp.priority<<temp.value_time<<temp.size_memo;
        PCB_Queue.push(temp);   //插入后备队列
        i++;
    }
   

}




