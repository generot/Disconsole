#include "../include/discon.h"

void print_help(void) {
    ISSUE_ERROR(
        "--help: prints this list\n"
        "--text: initializes Disconsole in text message mode\n"
        "--embeds: initializes Disconsole in embed mode\n"
        "--embed-msg: embeds a text message\n"
        "--send-arg: sends the following argument(Example: ./discon --send-arg \"To be sent\")"
    );

    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "No options provided. Use --help for displaying all possible options.\n");
        return EXIT_FAILURE;
    }

    int num_of_msgs = 0;
    const char base_url[] = "https://discord.com/api/webhooks";
    char req_url[MAX_STR] = {0};

    sprintf(req_url, "%s/%s/%s", base_url, DC_ID, DC_TOKEN);

    CURL *curl_inst = curl_easy_init();
    curl_slist *ls = setup(curl_inst, req_url);

    switch(match_carg(argv[1])) {
    case CMD_HELP:
        print_help();
    case CMD_TEXT:
        while(True) {
            printf("[%s]: ", get_current_date());

            fflush(stdin);
            send_text_msg(curl_inst, make_text_msg(argc > 2 && match_carg(argv[2]) == CMD_EMBED_TEXT));
        } break;
    case CMD_EMBEDS:
        printf("[%s - Number of embeds(Max 10)]: ", get_current_date());
        scanf("%d", &num_of_msgs);

        if(num_of_msgs > 10) {
            ISSUE_ERROR("[%s - ERROR]: Maximum number of embeds exceeded.", get_current_date());
            break;
        }

        while(num_of_msgs--) {
            fflush(stdin);
            send_embed(curl_inst, make_embed());
        }

        break;
    case CMD_PASS_ARG:
        if(argc < 3) {
            ISSUE_ERROR("[%s - ERROR]: This option requires an additional argument, "
                            "containing the data to be sent.", get_current_date());

            break;
        }

        message new_msg = { .flags = DMS_TEXT };
        concat(new_msg.content, "```%s```", argv[2]);

        send_text_msg(curl_inst, new_msg);

        break;
    case CMD_EMBED_TEXT:
        break;
    }

    curl_slist_free_all(ls);
    curl_easy_cleanup(curl_inst);

    return EXIT_SUCCESS;
}