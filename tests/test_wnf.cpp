#include <windows.h>
#include <stdio.h>
#include "../src/core/wnf_client.h"

int main() {
    printf("=== WNF Client Test ===
");
    WnfClient client;
    if (!client.Initialize()) {
        printf("[-] WNF client init failed
");
        return 1;
    }
    client.EnumerateStates();
    printf("[+] Test completed
");
    return 0;
}
