
void br_init(bool fill);
void* br_alloc(size_t size);
void br_free(void* ptr);
size_t br_size();
size_t br_used();

#define br_alloc_array(type, size) br_alloc(sizeof(type)*size)
