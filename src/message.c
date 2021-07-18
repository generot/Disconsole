#include "../include/discon.h"

embed make_embed(void) {
    embed new_embed = {0};
    colour_val colr = 0, colg = 0, colb = 0;

    new_embed.flags = EB_COL;
    
    printf("Title: ");
    getline(new_embed.title, MAX_STR);

    printf("Description: ");
    getline(new_embed.description, MAX_STR);

    printf("URL: ");
    getline(new_embed.url, MAX_STR);

    printf("Colour(R G B): ");
    scanf("%u %u %u", &colr, &colg, &colb);

    new_embed.color = COLRGB(colr, colg, colb);

    if(strcmp(new_embed.title, "null"))       new_embed.flags |= EB_TITLE;
    if(strcmp(new_embed.description, "null")) new_embed.flags |= EB_DESC;
    if(strcmp(new_embed.url, "null"))         new_embed.flags |= EB_URL;

    return new_embed;
}

message make_text_msg(Bool is_embedded) {
    message new_msg = { .flags = DMS_TEXT };

    if(is_embedded) {
        char line[MAX_MSG - 6] = {0};
        getline(line, MAX_MSG - 6);

        sprintf(new_msg.content, "```%s```", line);
    } else getline(new_msg.content, MAX_MSG);

    return new_msg;
}

//Should be invoked after the cURL instance was set up.
void send_embed(CURL *curl_inst, embed to_send) {
    message msg = {
        .flags = DMS_EMBED,
        .embeds[0] = to_send
    };

    send_text_msg(curl_inst, msg);

    return;
}

void send_text_msg(CURL *curl_inst, message to_send) {
    char *serialized_msg = serialize_message(to_send);
    curl_easy_setopt(curl_inst, CURLOPT_POSTFIELDS, serialized_msg);

    perform(curl_inst);
    free(serialized_msg);

    return;
}