#include "sys.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "droneCV2" << endl;
    sys=Sys(argc,argv);
    sys.Show_status();
    sys.init();
    sys.start();
    sys.wait();
    return 0;
}
