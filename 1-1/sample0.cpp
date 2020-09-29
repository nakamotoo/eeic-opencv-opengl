#include <stdio.h>

class myclass{
public:
    void set_x(int num);
    int get_x();

    int x;
};

void myclass::set_x(int num){
    x = num;
};
int myclass::get_x(){
    return x;
};

int main(){
    myclass a;

    a.set_x(1);
    printf("x = %d\n", a.get_x());

    a.x = 2;
    printf("x = %d\n", a.x);

    return 0;
};