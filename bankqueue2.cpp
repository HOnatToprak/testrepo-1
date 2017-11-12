#include <iostream>

using namespace std;
class customer{
    friend class customerqueue;
    private:
        int num;
        int arrive;
        int stay;
        int waittime;
        customer* next;
        customer(int a,int b,int c){
            num = a;
            arrive = b;
            stay = c;
            waittime = 0;
            next = nullptr;
            }
};
static class customerqueue{
    private:
        customer* first;
        customer* last;
        void trysend(){
            if(first == nullptr){
                cout << "No Customers Left" << endl;
                exit(1);
            }
            else if(employeestack::match(first)){
                customer* temp = first;
                first = first->next;
                delete temp;
            }
            else{
                first->arrive++;
                first->waittime++;
            }
                
        }
    public:
        void add(int a,int b, int c){
            if(first == nullptr){
                first = new customer(a,b,c);
                first = last;
            }
            else{
                last->next=new customer(a,b,c);
                last = last->next;
            }
        }
        void sendall(){
            while(1){
                trysend();
            }
        }

};
class employee{
    friend class employeestack;
    private:
        int employeenum;
        int matchtime;
        int busy;
        employee* next;
        employee(int a){
            employeenum = a;
            ismatched = false;
            busy = 0;
            next = nullptr;
        }
};
static class employeestack{
    private:
        employee* first;
        int employeenum;
        employeestack(int num){
        employeenum = num;
            for(int i = 1;i<num;i++){
                first = new employee(i);
                employee* temp = new employee(i+1)
                temp->next = first;
                first = temp;
            }
        }
        bool match(customer* a){
            for(employee* iter = first;iter->next != nullptr;iter = iter->next){
                if(!(iter->busy)){
                    iter->matchtime = a->arrive;
                    iter->busy = a->stay;
                    cout << iter->employeenum << "  " << iter->matchtime << "  " << iter->matchtime+iter->busy << "  " << a.waittime << endl;
                    return true;
                }
            }
            return false;
        }
};
int main()
{
    customerqueue myqueue;
    employeestack mystack(6); //takes employee numbers
    myqueue.sendall();
    
    return 0;
}
