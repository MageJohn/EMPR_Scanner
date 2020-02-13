void sensor_init(void);
void sensor_tick(void);

void sensor_direct_get_data(union ColourData *data);
void sensor_wait_for_good_data(void);
