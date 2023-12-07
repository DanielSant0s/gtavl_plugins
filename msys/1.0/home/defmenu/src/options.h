#include <stdint.h> 

typedef struct {
    const char* label;
    void* value;
} OptionSelector;

typedef struct {
    const char* label;
    OptionSelector* options;
    size_t options_size;
    void* (*set)(void*);
    void* (*get)(void);
    void* ptr;
} OptionEntry;

typedef struct {
    const char* label;
    OptionEntry* entries;
    size_t entries_size;
} OptionSection;


