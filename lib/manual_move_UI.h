enum ManualUIDataDisplay {
    UI_SHOW_RGB = 0, UI_SHOW_COORDS = !(UI_SHOW_RGB)
};

void manual_ui(enum ManualUIDataDisplay mode);
