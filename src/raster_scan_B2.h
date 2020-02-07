void raster_scan(int16_t x, int16_t y, int16_t z, int16_t grid_size_x, int16_t grid_size_y);
//returns a pointer to a static array inside the function. must use a local pointer to retrieve
void detect_flag(uint16_t mid_x, uint16_t mid_y, uint16_t z, uint16_t *x_var, uint16_t *y_var);
void one_dimensional_scan_y(int16_t x, int16_t y, int16_t z, int8_t step, uint16_t *p);
//returns a pointer to a static array inside the function. must use a local pointer to retrieve
void send_data(void);