#include <string>
#ifndef REGDEF_H
#define REGDEF_H
enum Register{
	R_ZERO=0,
	R_AT,
	R_V0,
	R_V1,
	R_A0,
	R_A1,
	R_A2,
	R_A3,
	R_T0,
	R_T1,
	R_T2,
	R_T3,
	R_T4,
	R_T5,
	R_T6,
	R_T7,
	R_S0,
	R_S1,
	R_S2,
	R_S3,
	R_S4,
	R_S5,
	R_S6,
	R_S7,
	R_T8,
	R_T9,
	R_K0,
	R_K1,
	R_GP,
	R_SP,
	R_FP,
	R_RA,
};
extern std::string regto_string[32];
#endif
