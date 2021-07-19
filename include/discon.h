#ifndef __DISCON_H__
#define __DISCON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl.h>

//The DC_ID(Webhook ID) and DC_TOKEN(Webhook token) macros are defined here.
//You have to provide your own ones
#include "webhook_inf.h"

#define MAX_STR 1024
#define MAX_MSG 2000
#define MAX_EMBEDS 10

#define MAX_UINT_STRLEN 10
#define MAX_EMBED_LEN MAX_EMBEDS * (MAX_STR * 3 + MAX_UINT_STRLEN)
#define MAX_JSON MAX_MSG + 1 + MAX_EMBED_LEN // > 4 KB

#define COLRGB(r, g, b) ((r) << 16 | (g) << 8 | b)

//Use _Generic if possible later on
#define SERIALIZE_FIELD(buffer, object, field) \
    concat((buffer), "\"%s\":\"%s\",", #field, object.field)

#define SERIALIZE_FIELD_UINT(buffer, object, field) \
    concat((buffer), "\"%s\":\"%u\",", #field, object.field)

#define CMD_ARGS                    \
    const char *cline_args[] = {    \
        "--help", "--text",         \
        "--embeds", "--embed-msg",  \
        "--send-arg"                \
    }

typedef struct curl_slist curl_slist;
typedef unsigned int colour_val;

typedef struct _Embed embed;
typedef struct _Message message;

typedef enum _Boolean_Val Bool;
typedef enum _Cmd_Args cargs;

enum _Boolean_Val { 
    False, 
    True 
};

enum _Message_Flags {
    DMS_TEXT = 0x01,
    DMS_EMBED = 0x02
};

enum _Embed_Flags {
    EB_TITLE = 0x01,
    EB_DESC = 0x02,
    EB_URL = 0x04,
    EB_COL = 0x08
};

enum _Cmd_Args {
    CMD_HELP,
    CMD_TEXT,
    CMD_EMBEDS,
    CMD_EMBED_TEXT,
    CMD_PASS_ARG
};

struct _Embed {
    char title[MAX_STR],
         description[MAX_STR],
         url[MAX_STR];
    
    colour_val color;

    int flags;
};

struct _Message {
    char content[MAX_MSG + 1];
    embed embeds[MAX_EMBEDS];

    int flags;
};

//Utilities
char *serialize_message(message msg);
char *serialize_embed(embed eb);
char *get_current_date(void);
void getline(char *buffer, size_t max);
int concat(char *buffer, const char *fmt, ...);
cargs match_carg(char *strrep);

//HTTP procedures
curl_slist *setup(CURL *curl_inst, const char *req_url);
void perform(CURL *curl_inst);

//Message utilities
embed make_embed(void);
message make_text_msg(Bool is_embedded);
void send_embed(CURL *curl_inst, embed to_send);
void send_text_msg(CURL *curl_inst, message to_send);

#endif //__DISCON_H__