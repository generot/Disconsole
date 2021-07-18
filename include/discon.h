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

#define COLRGB(r, g, b) ((r) << 31 | (g) << 16 | b)

//Use _Generic if possible later on
#define SERIALIZE_FIELD(buffer, object, field) \
    concat((buffer), "\"%s\":\"%s\",", #field, object.field)

#define SERIALIZE_FIELD_UINT(buffer, object, field) \
    concat((buffer), "\"%s\":\"%u\",", #field, object.field)

typedef struct curl_slist curl_slist;
typedef unsigned int colour_val;

typedef struct _Embed embed;
typedef struct _Message message;
typedef enum _BooleanVal Bool;

enum _BooleanVal { 
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

struct _Embed {
    char title[MAX_STR],
         description[MAX_STR],
         url[MAX_STR];
    
    colour_val colour;

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
void getline(char *buffer, size_t max);

//HTTP procedures
curl_slist *setup(CURL *curl_inst, const char *req_url);
void perform(CURL *curl_inst);

#endif //__DISCON_H__