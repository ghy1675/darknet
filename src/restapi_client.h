#include <curl/curl.h>

CURLcode request(CURL *hCURL, char *image_fd);
CURL* curl_init();
void curl_end(CURL *hCURL);