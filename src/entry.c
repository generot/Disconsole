#include "../include/discon.h"

int main(int argc, char **argv) {
    CURL *curl_inst = curl_easy_init();

    const char base_url[] = "https://discord.com/api/webhooks";
    char req_url[MAX_STR] = {0};

    sprintf(req_url, "%s/%s/%s", base_url, DC_ID, DC_TOKEN);

    curl_slist *ls = setup(curl_inst, req_url);
    message msg = { .flags = DMS_TEXT };

    while(True) {
        getline(msg.content, MAX_MSG);
        
        if(!strcmp(msg.content, ":q"))
            break;

        const char *msg_str = serialize_message(msg);

        curl_easy_setopt(curl_inst, CURLOPT_POSTFIELDS, msg_str);
        perform(curl_inst);

        free((void*)msg_str);
    }

    curl_slist_free_all(ls);
    curl_easy_cleanup(curl_inst);

    return EXIT_SUCCESS;
}