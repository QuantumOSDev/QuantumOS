#include <sys/utsname.h>

int main(int argc, char** argv) {
    struct utsname buf;
    printf("%d\n", uname(&buf));
    printf("version: %s\n", buf.version);
    printf("nodename: %s\n", buf.nodename);
    printf("release: %s\n", buf.release);
    printf("sysname: %s\n", buf.sysname);
    printf("version: %s\n", buf.version);
}