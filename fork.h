#include <v8.h>

extern "C" void init (v8::Handle<v8::Object> target);

static v8::Handle<v8::Value> fork_fn (const v8::Arguments& args);
static v8::Handle<v8::Value> daemonize_fn (const v8::Arguments& args);

static void child_handler (int signum);

#define EXPORTS_FN(__name, __fn) do {                \
    target->Set(String::New(__name),                 \
        FunctionTemplate::New(__fn)->GetFunction()); \
} while (0)
