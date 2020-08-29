#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct RES_BUF {
  char *memory;
  size_t size;
};

static size_t
cb_write_memory(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct RES_BUF *mem = (struct RES_BUF *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


int decrypt(const char *encoded, char *decoded)
{
  CURL *curl_handle;;
  CURLcode res;
  struct RES_BUF resBuf;
  char postData[1024];

  struct curl_slist *headerlist = NULL;
  headerlist = curl_slist_append(headerlist, "Content-Type: application/json");

  resBuf.memory = malloc(1);  /* will be grown as needed by the realloc above */
  resBuf.size = 0;    /* no data at this point */

  sprintf(postData, "{ \"payload\" : \"%s\" }", encoded);
  printf("[%s]\n", postData);
  printf("====================================\n");
  resBuf.memory = malloc(1);  /* will be grown as needed by the realloc above */
  resBuf.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://13.124.146.48:3000/service");
  curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headerlist);
  curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
  curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postData);
  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb_write_memory);
  /* we pass our 'resBuf' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&resBuf);

  /* get it! */
  res = curl_easy_perform(curl_handle);

  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    return -1;
  }

   /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);
  strcpy(decoded, (char *)resBuf.memory);
  free(resBuf.memory);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  return 1;
}
