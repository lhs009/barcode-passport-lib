#include <stdio.h>
#include <dlfcn.h>
#include "./lib/passportLib.h"

int main(int argc, char **argv)
{
  char result[1024];
  int (*ptrDecrypt)(const char *, char *) = NULL;
  char *error = NULL;

  if (argc != 2) {
    printf("[Usage]:[Program name][encoded string])\n");
    return -1;
  }
  printf("started %s\n", argv[1]);
  
  void* pHandle = dlopen("./lib/libpassport.so", RTLD_LAZY);
  printf("get handle\n");
  
  ptrDecrypt = dlsym(pHandle, "decrypt"); 
  if ((error = dlerror()) != NULL) { 
    printf("dlsym error: %s\n", error); 
    dlclose(pHandle); 
    return -1; 
  }
  (*ptrDecrypt)(argv[1], result);
  printf("executed: %s\n", result);
  dlclose(pHandle);
  
  return 0;
}

