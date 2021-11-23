#include <stdlib.h>
#include <string.h>

#include "restapi_client.h"
#include "cJSON.h"

CURLcode request(CURL *hCURL, char *img_fname){
    struct curl_slist *list = NULL;
    curl_mime *form;
    curl_mimepart *field;

    const char* base_url = "http://seven.iacryl.com:65501/api/semiconductor/semiconductor";
    const char* api_url = "http://flightbase.iacryl.com/deployment/hc383188c34d3ffc328e4df17b9f92740/input";
    const int dataset_id = 12;

    char POST_DATA[256] = {0};

    /* HTTP URL Setting */
    curl_easy_setopt(hCURL, CURLOPT_URL, base_url);


    /* HTTP Header Setting */
    //list = curl_slist_append(list, "Content-Type: application/json");
    list = curl_slist_append(list, "jf-User: keti");
    list = curl_slist_append(list, "jf-Token: robert");

    curl_easy_setopt(hCURL, CURLOPT_HTTPHEADER, list);


    /* HTTP Post Setting */
    //sprintf(POST_DATA, "api_url=%s&dataset_id=%d", api_url, dataset_id);

    curl_easy_setopt(hCURL, CURLOPT_POST, 1L);
    //curl_easy_setopt(hCURL, CURLOPT_POSTFIELDS, POST_DATA);

    /* HTTP Multipart File Upload */
    form = curl_mime_init(hCURL);
    
    field = curl_mime_addpart(form);
    curl_mime_name(field, "doc");
    curl_mime_filedata(field, img_fname);

    field = curl_mime_addpart(form);
    curl_mime_name(field, "filename");
    curl_mime_data(field, img_fname, CURL_ZERO_TERMINATED);


    field = curl_mime_addpart(form);
    curl_mime_name(field, "mimetype");
    curl_mime_data(field, "Content-Type: image/jpeg", CURL_ZERO_TERMINATED);

    field = curl_mime_addpart(form);
    curl_mime_name(field, "dataset_id");
    curl_mime_data(field, "12", CURL_ZERO_TERMINATED);

    field = curl_mime_addpart(form);
    curl_mime_name(field, "api_url");
    curl_mime_data(field, api_url, CURL_ZERO_TERMINATED);

    curl_easy_setopt(hCURL, CURLOPT_MIMEPOST, form);


    /* SSL Setting */
    curl_easy_setopt(hCURL, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(hCURL, CURLOPT_SSL_VERIFYHOST, 1L);

    CURLcode res = curl_easy_perform(hCURL);
    curl_slist_free_all(list);

    if(res != CURLE_OK)
      printf("curl_easy_perform() failed: %s\n",curl_easy_strerror(res));

}

CURL* curl_init(){
    CURL *curl;

    /* CURL Global Init (This Function Not Thread Safe) */
    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();
    if(!curl){ 
        printf("curl init error!!");
        return NULL;
    }

    return curl;
}

void curl_end(CURL *hCURL){
    curl_easy_cleanup(hCURL);

    curl_global_cleanup();
}
