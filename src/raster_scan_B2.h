void raster_scan(uint16_t x, uint16_t y, uint16_t z, uint16_t grid_size_x, uint16_t grid_size_y);
//returns a pointer to a static array inside the function. must use a local pointer to retrieve
void detect_flag(uint16_t mid_x, uint16_t mid_y, uint16_t z, uint16_t *x_var, uint16_t *y_var);
bool one_dimensional_scan(uint16_t x, uint16_t y, uint16_t z, uint16_t stop_coord, uint8_t step);
//returns a pointer to a static array inside the function. must use a local pointer to retrieve