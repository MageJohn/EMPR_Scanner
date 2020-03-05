#include "platform.h"
#include "platform_lcd.h"
#include "serial.h"

uint16_t colour[4];

#define THRESHOLD 300
#define TOLERANCE 0.15
#define X_CENTRE 450
#define Y_CENTRE 625
#define Z 300
#define X_STEP 300
#define Y_STEP 175

enum Countries {
    FRANCE, CZECHIA, BULGARIA, SUDAN, BURUNDI, BURKINO_FASO, UK, SYRIA, BELARUS,
    CAR, ICELAND, MACEDONIA, UNDEFINED
};

char country_names[13][13] = {"France", "Czechia", "Bulgaria", "Sudan","Burundi", "Burkino Faso", "UK", "Syria", "Belarus",
                              "C.A.R", "Iceland", "Macedonia", "UNDEFINED"};

char raw_data_to_char(uint16_t *colours);
int shade(float red, float green, float blue, float tolerance);
uint16_t max(float num1, float num2);
uint16_t min(float num1, float num2);
enum Countries find_country(void);

uint16_t max(float num1, float num2) {
    return (num1 > num2) ? num1 : num2;
}

uint16_t min(float num1, float num2) {
    return (num1 > num2) ? num2 : num1;
}

int shade(float red, float green, float blue, float tolerance) {
    uint16_t largest = max(max(red, green), blue);
    uint16_t smallest = min(min(red, green), blue);
    if ((smallest + smallest*tolerance) >= largest) {
        return 1;
    }
    return 0;
}

char raw_data_to_char(uint16_t *colours) {
    float clear = colours[0];
    float red = colours[1];
    float green = colours[2];
    float blue = colours[3];

    float largest = max(max(red, green), blue);
    float smallest = min(min(red, green), blue);
    float second;

    if ((red == largest && green == smallest) || (green == largest && red== smallest)) {
        second = blue;
    }
    else if ((green == largest && blue == smallest) || (blue == largest && green == smallest)) {
        second = red;
    }
    else {
        second = green;
    }

    if(shade(red, green, blue, TOLERANCE)) {
        if(clear >= THRESHOLD) {
            // Defined as white
            return 'w';
        }
        return 'k';
    }

    else if (((red == largest) && (green * 1.2 >= red) || (green == largest) && (red * 1.2 >= green)) && (blue * 1.5 >= largest)) {
        return 'k';
    }

    else if((red == largest) && (green * 1.2 >= red) || (green == largest) && (red * 1.2 >= green)) {
        return 'u';
    }

    else if(red == largest) {
        return 'r';
    }
    else if(green == largest) {
        return 'g';
    }
    else if(blue == largest) {
        return 'b';
    }
}

void main(void) {
    platform_init();
    serial_init();
    platform_lcd_init();

    platform_calibrate_head();
    while(!platform_calibrated());
    platform_lcd_clear_display();

    enum Countries country = find_country();

    platform_lcd_write_ascii(country_names[country], 0x00);

}

enum Countries find_country(void) {
    char val;

    platform_head_set_coords(X_CENTRE, Y_CENTRE + Y_STEP, Z);
    while(!platform_head_at_coords());
    platform_sensor_get_data(colour);

    val = raw_data_to_char(colour);
    serial_write_b((char *)&val, 1);

    while(1) {

        switch (val) {
            case 'w':
                platform_head_set_coords(X_CENTRE, Y_CENTRE, Z);
                while(!platform_head_at_coords());
                platform_sensor_get_data(colour);
                val = raw_data_to_char(colour);
                serial_write_b((char *)&val, 1);

                switch (val) {
                    case 'w':
                        platform_head_set_coords(X_CENTRE, Y_CENTRE - Y_STEP, Z);
                        while(!platform_head_at_coords());
                        platform_sensor_get_data(colour);
                        val = raw_data_to_char(colour);
                        serial_write_b((char *)&val, 1);

                        switch (val) {
                            case 'w':
                                return FRANCE;
                            case 'r':
                                return CZECHIA;
                            default:
                                return UNDEFINED;
                        }
                    case 'r':
                        return CZECHIA;
                    case 'g':
                        return BULGARIA;
                    default:
                        return UNDEFINED;
                }

            case 'r':
                platform_head_set_coords(X_CENTRE, Y_CENTRE, Z);
                while(!platform_head_at_coords());
                platform_sensor_get_data(colour);
                val = raw_data_to_char(colour);
                serial_write_b((char *)&val, 1);

                switch (val) {
                    case 'w':
                        platform_head_set_coords(X_CENTRE, Y_CENTRE - Y_STEP, Z);
                        while(!platform_head_at_coords());
                        platform_sensor_get_data(colour);
                        val = raw_data_to_char(colour);
                        serial_write_b((char *)&val, 1);

                        switch (val) {
                            case 'k':
                                platform_head_set_coords(X_CENTRE - X_STEP, Y_CENTRE, Z);
                                while(!platform_head_at_coords());
                                platform_sensor_get_data(colour);
                                val = raw_data_to_char(colour);
                                serial_write_b((char *)&val, 1);

                                switch (val) {
                                    case 'g':
                                        return SUDAN;
                                    case 'w':
                                        return SYRIA;
                                    default:
                                        return UNDEFINED;
                                }
            


                            case 'r':
                                return BURUNDI;
                            default:
                                return UNDEFINED;
                        }



                    case 'r':
                        platform_head_set_coords(X_CENTRE, Y_CENTRE - Y_STEP, Z);
                        while(!platform_head_at_coords());
                        platform_sensor_get_data(colour);
                        val = raw_data_to_char(colour);
                        serial_write_b((char *)&val, 1);

                        switch (val) {
                            case 'r':
                                platform_head_set_coords(X_CENTRE - X_STEP, Y_CENTRE, Z);
                                while(!platform_head_at_coords());
                                platform_sensor_get_data(colour);
                                val = raw_data_to_char(colour);
                                serial_write_b((char *)&val, 1);

                                switch (val) {
                                    case 'r':
                                        return UK;
                                    
                                    default:
                                        return CAR;
                                }

                            case 'g':
                                return BELARUS;
                            default:
                                return UNDEFINED;
                        }


                    default:
                        return BURKINO_FASO;
                }

            case 'b':
                return ICELAND;

            default:
                return MACEDONIA;
        }

    }
}