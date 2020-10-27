const int z = 10;
const int k = 20;
const float values[] = {1.0f, 0.234f, 4.343f};
const char *name = "test";

extern void print_number(int *n);
extern void print_string(char *s);

typedef struct WasmMemory
{
    char *start;
    int size;
    int current;
} WasmMemory;

WasmMemory memory;

extern int get_memory_size();
extern char *get_memory_start();
extern void wasm_error(char *msg);

void *get_memory(unsigned int size)
{
   if (memory.current + size > memory.size) return 0; 

   memory.current += size;

   return (void *) (memory.start + memory.current - size);
}

void tg_strcpy(char *dst, char *src)
{
    while (*src)
    {
        *dst++=*src++;
    }
}

void init()
{
    print_string("Initializing...");
    memory.start = get_memory_start();
    memory.size = get_memory_size();

    print_number(&memory.size);
    print_number((int *) &memory.start);

    char *str = (char *) get_memory(100000000);
    if (!str) {
        wasm_error("Out of memory!");
        return;
    }
    tg_strcpy(str, "WebAssembly memory test!!!!!!!!!!!!!!!!!!!!!!!!!!");
    print_string("Done!");
}

int inc(x)
{
    return x + 1;
}
void add(int x, int y, int *out) 
{
    *out = x + y;
    print_number(out);
}
