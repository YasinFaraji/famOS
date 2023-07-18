#include "shell.h"
#include "stdio.h"
#include "stdlib.h"
#include "famos.h"

int main(int argc, char** argv)
{
    print("famOS v1.0.0\n");
    while(1) 
    {
        print("> ");
        char buf[1024];
        famos_terminal_readline(buf, sizeof(buf), true);
        print("\n");
        famos_system_run(buf);
        
        print("\n");
    }
    
    return 0;
}