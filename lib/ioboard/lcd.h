/*------------------------------
 * LCD INSTRUCTION MACROS
 *------------------------------*/

/* control byte macro */
#define Control_byte(CO, RS) (0x00 | ((CO & 1) << 7) | ((RS & 1) << 6))

/* Basic instruction macros */
#define NOP 0x00
#define Function_set(DL, M, SL, H) (0x20 | ((DL & 1) << 4) | ((M & 1) << 2) | ((SL & 1) << 1) | (H & 1))

/* Standard instruction macros */
#define Clear_display 0x01
#define Return_home 0x02
#define Entry_mode_set(I_D, S) (0x40 | ((I_D & 1) << 1) | (S & 1))
#define Display_ctl(D, C, B) (0x08 | ((D & 1) << 2) | ((C & 1) << 1) | (B & 1))
#define Curs_disp_shift(SC, RL) (0x10 | ((SC & 1) << 3) | ((RL & 1) << 2))
#define Set_CGRAM(ACG) (0x40 | (ACG & 0x3f))
#define Set_DDRAM(ADD) (0x80 | (ADD & 0x7f))

/* Extended instruction macros */
#define Screen_conf(L) (0x02 | (L & 1))
#define Disp_conf(P, Q) (0x04 | ((P & 1) << 1) | (Q & 1))
#define Icon_ctl(IM, IB, DM) (0x08 | ((IM & 1) << 2) | ((IB & 1) << 1) | (DM & 1))
#define Temp_ctl(TC1, TC2) (0x10 | ((TC1 & 1) << 1) | (TC2 & 1))
#define HV_gen(S1, S0) (0x40 | ((S1 & 1) << 1) | (S0 & 1))
#define VLCD_set(V, Vx) (0x80 | ((V & 1) << 6) | (Vx & 0x3f))

