#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
//make_helper_v_v(push_i)
make_helper_v(seta_rm);
make_helper_v(setae_rm);
make_helper_v(setb_rm);
make_helper_v(setbe_rm);
make_helper_v(setc_rm);
make_helper_v(sete_rm);
make_helper_v(setg_rm);
make_helper_v(setge_rm);
make_helper_v(setl_rm);
make_helper_v(setle_rm);
make_helper_v(setna_rm);
make_helper_v(setne_rm);
make_helper_v(setno_rm);
make_helper_v(setnp_rm);
make_helper_v(setns_rm);
make_helper_v(seto_rm);
make_helper_v(setp_rm);
make_helper_v(sets_rm);
make_helper_v(setz_rm);
