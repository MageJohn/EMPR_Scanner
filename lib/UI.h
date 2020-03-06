#include "lpc_types.h"

struct UI_elem {
    char *text;
    void (*callback)(void);
    struct UI_elem *next;
    struct UI_elem *prev;
};

void UI_build_sequence(struct UI_elem *last, struct UI_elem *this);
void UI_run(struct UI_elem *first);
