#include "../include/discon.h"

curl_slist *setup(CURL *curl_inst, const char *req_url) {
    curl_slist *ls = NULL;
    ls = curl_slist_append(ls, "Content-Type: application/json");

    curl_easy_setopt(curl_inst, CURLOPT_URL, req_url);
    curl_easy_setopt(curl_inst, CURLOPT_HTTPHEADER, ls);
    curl_easy_setopt(curl_inst, CURLOPT_SSL_VERIFYPEER, False);

    return ls;
}

void perform(CURL *curl_inst) {
    CURLcode res = curl_easy_perform(curl_inst);
    if(res != CURLE_OK) {
        fprintf(stderr, "Something went wrong: Error %d\n", res);
        
        curl_easy_cleanup(curl_inst);
        exit(EXIT_FAILURE);
    }
}