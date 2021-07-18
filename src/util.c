#include "../include/discon.h"

#include <time.h>
#include <stdarg.h>

CMD_ARGS;

int concat(char *buffer, const char *fmt, ...) {
    va_list ls;
    va_start(ls, fmt);

    int bytes = vsprintf(buffer + strlen(buffer), fmt, ls);
    va_end(ls);

    return bytes;
}

void getline(char *buffer, size_t max) {
    memset(buffer, 0, max);

    for(size_t i = 0; i < max; i++) {
        char curr_char = fgetc(stdin);

        if(curr_char == '\n')
            break;

        buffer[i] = curr_char;
    }
}

cargs match_carg(char *strrep) {
    for(int i = 0; i < sizeof(cline_args) / sizeof(char*); i++)
        if(!strcmp(strrep, cline_args[i]))
            return i;

    return -1;
}

char *get_current_date(void) {
    time_t curr_time = time(NULL);
    char *date = ctime(&curr_time);

    date[strlen(date) - 1] = '\0';
    return date;
}

char *serialize_embed(embed eb) {
    char *embed_str = calloc(MAX_EMBED_LEN, sizeof(char));
    embed_str[0] = '{';

    if(eb.flags & EB_TITLE) SERIALIZE_FIELD(embed_str, eb, title);
    if(eb.flags & EB_DESC) SERIALIZE_FIELD(embed_str, eb, description);
    if(eb.flags & EB_URL) SERIALIZE_FIELD(embed_str, eb, url);
    if(eb.flags & EB_COL) SERIALIZE_FIELD_UINT(embed_str, eb, color);

    size_t len = strlen(embed_str);
    if(embed_str[len - 1] == ',')
        len--;

    embed_str[len] = '}';

    return embed_str;
}

char *serialize_message(message msg) {
    char *json_str = calloc(MAX_JSON, sizeof(char));

    json_str[0] = '{';

    if(msg.flags & DMS_TEXT) SERIALIZE_FIELD(json_str, msg, content);
    if(msg.flags & DMS_EMBED) {
        char *embed_str = serialize_embed(msg.embeds[0]);

        concat(json_str, "\"embeds\":[%s", embed_str);
        free(embed_str);

        for(int i = 1; i < MAX_EMBEDS; i++) {
            if(!msg.embeds[i].flags) break;

            embed_str = serialize_embed(msg.embeds[i]);

            concat(json_str, ",%s", embed_str);
            free(embed_str);
        }

        concat(json_str, "],");
    }

    size_t len = strlen(json_str);
    if(json_str[len - 1] == ',')
        len--;

    json_str[len] = '}';

    return json_str;
}