# Disconsole
A simple C console interface that lets you send messages to a Discord channel via a given webhook. To use Disconsole with your own webhook, you have two options:<br>

- Making your own `webhook_inf.h` header with `DC_TOKEN` and `DC_ID`(both of which should be strings, by the way) defined there or straight up defining them in `discon.h`.
- Adding the following options to the compilation batch file:<br>
  - `-DDC_TOKEN=<your_webhook_token> -DDC_ID=<your_webhook_id>`

Both options are completely valid, it's up to you to choose which one is more convenient for you.

## Dependencies
- [libcurl](https://curl.se/libcurl/c/)
