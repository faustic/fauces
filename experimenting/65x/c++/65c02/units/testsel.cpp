// Selection of test generator
//
/*
Licensed under the MIT License.
 
Copyright (c) Faustic Inferno SL
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "testsel.hpp"

 #include "testgen/Adc_abs_1.hpp"
 #include "testgen/Adc_absx_1.hpp"
 #include "testgen/Adc_absy_1.hpp"
 #include "testgen/Adc_dp_1.hpp"
 #include "testgen/Adc_dpi_1.hpp"
 #include "testgen/Adc_dpiy_1.hpp"
 #include "testgen/Adc_dpx_1.hpp"
 #include "testgen/Adc_dpxi_1.hpp"
 #include "testgen/Adc_imm_1.hpp"
 #include "testgen/And_abs_1.hpp"
 #include "testgen/And_absx_1.hpp"
 #include "testgen/And_absy_1.hpp"
 #include "testgen/And_dp_1.hpp"
 #include "testgen/And_dpi_1.hpp"
 #include "testgen/And_dpiy_1.hpp"
 #include "testgen/And_dpx_1.hpp"
 #include "testgen/And_dpxi_1.hpp"
 #include "testgen/And_imm_1.hpp"
 #include "testgen/Asl_abs_1.hpp"
 #include "testgen/Asl_absx_1.hpp"
 #include "testgen/Asl_acc_1.hpp"
 #include "testgen/Asl_dp_1.hpp"
 #include "testgen/Asl_dpx_1.hpp"
 #include "testgen/Bbr_1.hpp"
 #include "testgen/Bbs_1.hpp"
 #include "testgen/Bcc_1.hpp"
 #include "testgen/Bcs_1.hpp"
 #include "testgen/Beq_1.hpp"
 #include "testgen/Bit_abs_1.hpp"
 #include "testgen/Bit_absx_1.hpp"
 #include "testgen/Bit_dp_1.hpp"
 #include "testgen/Bit_dpx_1.hpp"
 #include "testgen/Bit_imm_1.hpp"
 #include "testgen/Bmi_1.hpp"
 #include "testgen/Bne_1.hpp"
 #include "testgen/Bpl_1.hpp"
 #include "testgen/Bra_1.hpp"
 #include "testgen/Brk_0.hpp"
 #include "testgen/Bvc_1.hpp"
 #include "testgen/Bvs_1.hpp"
 #include "testgen/Clc_1.hpp"
 #include "testgen/Cld_1.hpp"
 #include "testgen/Cli_1.hpp"
 #include "testgen/Clv_1.hpp"
 #include "testgen/Cmp_abs_1.hpp"
 #include "testgen/Cmp_absx_1.hpp"
 #include "testgen/Cmp_absy_1.hpp"
 #include "testgen/Cmp_dp_1.hpp"
 #include "testgen/Cmp_dpi_1.hpp"
 #include "testgen/Cmp_dpiy_1.hpp"
 #include "testgen/Cmp_dpx_1.hpp"
 #include "testgen/Cmp_dpxi_1.hpp"
 #include "testgen/Cmp_imm_1.hpp"
 #include "testgen/Cpx_abs_1.hpp"
 #include "testgen/Cpx_dp_1.hpp"
 #include "testgen/Cpx_imm_1.hpp"
 #include "testgen/Cpy_abs_1.hpp"
 #include "testgen/Cpy_dp_1.hpp"
 #include "testgen/Cpy_imm_1.hpp"
 #include "testgen/Dec_abs_1.hpp"
 #include "testgen/Dec_absx_1.hpp"
 #include "testgen/Dec_acc_1.hpp"
 #include "testgen/Dec_dp_1.hpp"
 #include "testgen/Dec_dpx_1.hpp"
 #include "testgen/Dex_1.hpp"
 #include "testgen/Dey_1.hpp"
 #include "testgen/Eor_abs_1.hpp"
 #include "testgen/Eor_absx_1.hpp"
 #include "testgen/Eor_absy_1.hpp"
 #include "testgen/Eor_dp_1.hpp"
 #include "testgen/Eor_dpi_1.hpp"
 #include "testgen/Eor_dpiy_1.hpp"
 #include "testgen/Eor_dpx_1.hpp"
 #include "testgen/Eor_dpxi_1.hpp"
 #include "testgen/Eor_imm_1.hpp"
 #include "testgen/Hola_1.hpp"
 #include "testgen/Identify_processor_1.hpp"
 #include "testgen/Inc_abs_1.hpp"
 #include "testgen/Inc_absx_1.hpp"
 #include "testgen/Inc_acc_1.hpp"
 #include "testgen/Inc_dp_1.hpp"
 #include "testgen/Inc_dpx_1.hpp"
 #include "testgen/Inx_1.hpp"
 #include "testgen/Iny_1.hpp"
 #include "testgen/Jmp_abs_1.hpp"
 #include "testgen/Jmp_absi_1.hpp"
 #include "testgen/Jmp_absxi_1.hpp"
 #include "testgen/Jsr_abs_1.hpp"
 #include "testgen/Lda_abs_1.hpp"
 #include "testgen/Lda_absx_1.hpp"
 #include "testgen/Lda_absy_1.hpp"
 #include "testgen/Lda_dp_1.hpp"
 #include "testgen/Lda_dpi_1.hpp"
 #include "testgen/Lda_dpiy_1.hpp"
 #include "testgen/Lda_dpx_1.hpp"
 #include "testgen/Lda_dpxi_1.hpp"
 #include "testgen/Lda_imm_1.hpp"
 #include "testgen/Ldx_abs_1.hpp"
 #include "testgen/Ldx_absy_1.hpp"
 #include "testgen/Ldx_dp_1.hpp"
 #include "testgen/Ldx_dpy_1.hpp"
 #include "testgen/Ldx_imm_1.hpp"
 #include "testgen/Ldy_abs_1.hpp"
 #include "testgen/Ldy_absx_1.hpp"
 #include "testgen/Ldy_dp_1.hpp"
 #include "testgen/Ldy_dpx_1.hpp"
 #include "testgen/Ldy_imm_1.hpp"
 #include "testgen/Lsr_abs_1.hpp"
 #include "testgen/Lsr_absx_1.hpp"
 #include "testgen/Lsr_acc_1.hpp"
 #include "testgen/Lsr_dp_1.hpp"
 #include "testgen/Lsr_dpx_1.hpp"
 #include "testgen/Nop_1.hpp"
 #include "testgen/Ora_abs_1.hpp"
 #include "testgen/Ora_absx_1.hpp"
 #include "testgen/Ora_absy_1.hpp"
 #include "testgen/Ora_dp_1.hpp"
 #include "testgen/Ora_dpi_1.hpp"
 #include "testgen/Ora_dpiy_1.hpp"
 #include "testgen/Ora_dpx_1.hpp"
 #include "testgen/Ora_dpxi_1.hpp"
 #include "testgen/Ora_imm_1.hpp"
 #include "testgen/Pha_1.hpp"
 #include "testgen/Phx_1.hpp"
 #include "testgen/Phy_1.hpp"
 #include "testgen/Rmb_1.hpp"
 #include "testgen/Rol_abs_1.hpp"
 #include "testgen/Rol_absx_1.hpp"
 #include "testgen/Rol_acc_1.hpp"
 #include "testgen/Rol_dp_1.hpp"
 #include "testgen/Rol_dpx_1.hpp"
 #include "testgen/Ror_abs_1.hpp"
 #include "testgen/Ror_absx_1.hpp"
 #include "testgen/Ror_acc_1.hpp"
 #include "testgen/Ror_dp_1.hpp"
 #include "testgen/Ror_dpx_1.hpp"
 #include "testgen/Rti_1.hpp"
 #include "testgen/Rts_1.hpp"
 #include "testgen/Sbc_abs_1.hpp"
 #include "testgen/Sbc_absx_1.hpp"
 #include "testgen/Sbc_absy_1.hpp"
 #include "testgen/Sbc_dp_1.hpp"
 #include "testgen/Sbc_dpi_1.hpp"
 #include "testgen/Sbc_dpiy_1.hpp"
 #include "testgen/Sbc_dpx_1.hpp"
 #include "testgen/Sbc_dpxi_1.hpp"
 #include "testgen/Sbc_imm_1.hpp"
 #include "testgen/Sec_1.hpp"
 #include "testgen/Sed_1.hpp"
 #include "testgen/Sei_1.hpp"
 #include "testgen/Smb_1.hpp"
 #include "testgen/Sta_abs_1.hpp"
 #include "testgen/Sta_absx_1.hpp"
 #include "testgen/Sta_absy_1.hpp"
 #include "testgen/Sta_dp_1.hpp"
 #include "testgen/Sta_dpi_1.hpp"
 #include "testgen/Sta_dpiy_1.hpp"
 #include "testgen/Sta_dpx_1.hpp"
 #include "testgen/Sta_dpxi_1.hpp"
 #include "testgen/Stp_0.hpp"
 #include "testgen/Stx_abs_1.hpp"
 #include "testgen/Stx_dp_1.hpp"
 #include "testgen/Stx_dpy_1.hpp"
 #include "testgen/Sty_abs_1.hpp"
 #include "testgen/Sty_dp_1.hpp"
 #include "testgen/Sty_dpx_1.hpp"
 #include "testgen/Stz_abs_1.hpp"
 #include "testgen/Stz_absx_1.hpp"
 #include "testgen/Stz_dp_1.hpp"
 #include "testgen/Stz_dpx_1.hpp"
 #include "testgen/Tax_1.hpp"
 #include "testgen/Tay_1.hpp"
 #include "testgen/Trb_abs_1.hpp"
 #include "testgen/Trb_dp_1.hpp"
 #include "testgen/Tsb_abs_1.hpp"
 #include "testgen/Tsb_dp_1.hpp"
 #include "testgen/Tsx_1.hpp"
 #include "testgen/Txa_1.hpp"
 #include "testgen/Txs_1.hpp"
 #include "testgen/Tya_1.hpp"
 #include "testgen/Wai_0.hpp"



#include <unordered_map>
#include <string>
#include <functional>

using std::string;
using std::unordered_map;
using std::function;
using std::unique_ptr;
using std::make_unique;

namespace w65c02
{

static unordered_map<string, function<unique_ptr<Test>(Mem&)>> fmap
{
    {"Adc_abs_1", [](Mem& mem) {return make_unique<Adc_abs_1>(mem);}},
    {"Adc_absx_1", [](Mem& mem) {return make_unique<Adc_absx_1>(mem);}},
    {"Adc_absy_1", [](Mem& mem) {return make_unique<Adc_absy_1>(mem);}},
    {"Adc_dp_1", [](Mem& mem) {return make_unique<Adc_dp_1>(mem);}},
    {"Adc_dpi_1", [](Mem& mem) {return make_unique<Adc_dpi_1>(mem);}},
    {"Adc_dpiy_1", [](Mem& mem) {return make_unique<Adc_dpiy_1>(mem);}},
    {"Adc_dpx_1", [](Mem& mem) {return make_unique<Adc_dpx_1>(mem);}},
    {"Adc_dpxi_1", [](Mem& mem) {return make_unique<Adc_dpxi_1>(mem);}},
    {"Adc_imm_1", [](Mem& mem) {return make_unique<Adc_imm_1>(mem);}},
    {"And_abs_1", [](Mem& mem) {return make_unique<And_abs_1>(mem);}},
    {"And_absx_1", [](Mem& mem) {return make_unique<And_absx_1>(mem);}},
    {"And_absy_1", [](Mem& mem) {return make_unique<And_absy_1>(mem);}},
    {"And_dp_1", [](Mem& mem) {return make_unique<And_dp_1>(mem);}},
    {"And_dpi_1", [](Mem& mem) {return make_unique<And_dpi_1>(mem);}},
    {"And_dpiy_1", [](Mem& mem) {return make_unique<And_dpiy_1>(mem);}},
    {"And_dpx_1", [](Mem& mem) {return make_unique<And_dpx_1>(mem);}},
    {"And_dpxi_1", [](Mem& mem) {return make_unique<And_dpxi_1>(mem);}},
    {"And_imm_1", [](Mem& mem) {return make_unique<And_imm_1>(mem);}},
    {"Asl_abs_1", [](Mem& mem) {return make_unique<Asl_abs_1>(mem);}},
    {"Asl_absx_1", [](Mem& mem) {return make_unique<Asl_absx_1>(mem);}},
    {"Asl_acc_1", [](Mem& mem) {return make_unique<Asl_acc_1>(mem);}},
    {"Asl_dp_1", [](Mem& mem) {return make_unique<Asl_dp_1>(mem);}},
    {"Asl_dpx_1", [](Mem& mem) {return make_unique<Asl_dpx_1>(mem);}},
    {"Bbr_1", [](Mem& mem) {return make_unique<Bbr_1>(mem);}},
    {"Bbs_1", [](Mem& mem) {return make_unique<Bbs_1>(mem);}},
    {"Bcc_1", [](Mem& mem) {return make_unique<Bcc_1>(mem);}},
    {"Bcs_1", [](Mem& mem) {return make_unique<Bcs_1>(mem);}},
    {"Beq_1", [](Mem& mem) {return make_unique<Beq_1>(mem);}},
    {"Bit_abs_1", [](Mem& mem) {return make_unique<Bit_abs_1>(mem);}},
    {"Bit_absx_1", [](Mem& mem) {return make_unique<Bit_absx_1>(mem);}},
    {"Bit_dp_1", [](Mem& mem) {return make_unique<Bit_dp_1>(mem);}},
    {"Bit_dpx_1", [](Mem& mem) {return make_unique<Bit_dpx_1>(mem);}},
    {"Bit_imm_1", [](Mem& mem) {return make_unique<Bit_imm_1>(mem);}},
    {"Bmi_1", [](Mem& mem) {return make_unique<Bmi_1>(mem);}},
    {"Bne_1", [](Mem& mem) {return make_unique<Bne_1>(mem);}},
    {"Bpl_1", [](Mem& mem) {return make_unique<Bpl_1>(mem);}},
    {"Bra_1", [](Mem& mem) {return make_unique<Bra_1>(mem);}},
    {"Brk_0", [](Mem& mem) {return make_unique<Brk_0>(mem);}},
    {"Bvc_1", [](Mem& mem) {return make_unique<Bvc_1>(mem);}},
    {"Bvs_1", [](Mem& mem) {return make_unique<Bvs_1>(mem);}},
    {"Clc_1", [](Mem& mem) {return make_unique<Clc_1>(mem);}},
    {"Cld_1", [](Mem& mem) {return make_unique<Cld_1>(mem);}},
    {"Cli_1", [](Mem& mem) {return make_unique<Cli_1>(mem);}},
    {"Clv_1", [](Mem& mem) {return make_unique<Clv_1>(mem);}},
    {"Cmp_abs_1", [](Mem& mem) {return make_unique<Cmp_abs_1>(mem);}},
    {"Cmp_absx_1", [](Mem& mem) {return make_unique<Cmp_absx_1>(mem);}},
    {"Cmp_absy_1", [](Mem& mem) {return make_unique<Cmp_absy_1>(mem);}},
    {"Cmp_dp_1", [](Mem& mem) {return make_unique<Cmp_dp_1>(mem);}},
    {"Cmp_dpi_1", [](Mem& mem) {return make_unique<Cmp_dpi_1>(mem);}},
    {"Cmp_dpiy_1", [](Mem& mem) {return make_unique<Cmp_dpiy_1>(mem);}},
    {"Cmp_dpx_1", [](Mem& mem) {return make_unique<Cmp_dpx_1>(mem);}},
    {"Cmp_dpxi_1", [](Mem& mem) {return make_unique<Cmp_dpxi_1>(mem);}},
    {"Cmp_imm_1", [](Mem& mem) {return make_unique<Cmp_imm_1>(mem);}},
    {"Cpx_abs_1", [](Mem& mem) {return make_unique<Cpx_abs_1>(mem);}},
    {"Cpx_dp_1", [](Mem& mem) {return make_unique<Cpx_dp_1>(mem);}},
    {"Cpx_imm_1", [](Mem& mem) {return make_unique<Cpx_imm_1>(mem);}},
    {"Cpy_abs_1", [](Mem& mem) {return make_unique<Cpy_abs_1>(mem);}},
    {"Cpy_dp_1", [](Mem& mem) {return make_unique<Cpy_dp_1>(mem);}},
    {"Cpy_imm_1", [](Mem& mem) {return make_unique<Cpy_imm_1>(mem);}},
    {"Dec_abs_1", [](Mem& mem) {return make_unique<Dec_abs_1>(mem);}},
    {"Dec_absx_1", [](Mem& mem) {return make_unique<Dec_absx_1>(mem);}},
    {"Dec_acc_1", [](Mem& mem) {return make_unique<Dec_acc_1>(mem);}},
    {"Dec_dp_1", [](Mem& mem) {return make_unique<Dec_dp_1>(mem);}},
    {"Dec_dpx_1", [](Mem& mem) {return make_unique<Dec_dpx_1>(mem);}},
    {"Dex_1", [](Mem& mem) {return make_unique<Dex_1>(mem);}},
    {"Dey_1", [](Mem& mem) {return make_unique<Dey_1>(mem);}},
    {"Eor_abs_1", [](Mem& mem) {return make_unique<Eor_abs_1>(mem);}},
    {"Eor_absx_1", [](Mem& mem) {return make_unique<Eor_absx_1>(mem);}},
    {"Eor_absy_1", [](Mem& mem) {return make_unique<Eor_absy_1>(mem);}},
    {"Eor_dp_1", [](Mem& mem) {return make_unique<Eor_dp_1>(mem);}},
    {"Eor_dpi_1", [](Mem& mem) {return make_unique<Eor_dpi_1>(mem);}},
    {"Eor_dpiy_1", [](Mem& mem) {return make_unique<Eor_dpiy_1>(mem);}},
    {"Eor_dpx_1", [](Mem& mem) {return make_unique<Eor_dpx_1>(mem);}},
    {"Eor_dpxi_1", [](Mem& mem) {return make_unique<Eor_dpxi_1>(mem);}},
    {"Eor_imm_1", [](Mem& mem) {return make_unique<Eor_imm_1>(mem);}},
    {"Hola_1", [](Mem& mem) {return make_unique<Hola_1>(mem);}},
    {"Identify_processor_1", [](Mem& mem) {return make_unique<Identify_processor_1>(mem);}},
    {"Inc_abs_1", [](Mem& mem) {return make_unique<Inc_abs_1>(mem);}},
    {"Inc_absx_1", [](Mem& mem) {return make_unique<Inc_absx_1>(mem);}},
    {"Inc_acc_1", [](Mem& mem) {return make_unique<Inc_acc_1>(mem);}},
    {"Inc_dp_1", [](Mem& mem) {return make_unique<Inc_dp_1>(mem);}},
    {"Inc_dpx_1", [](Mem& mem) {return make_unique<Inc_dpx_1>(mem);}},
    {"Inx_1", [](Mem& mem) {return make_unique<Inx_1>(mem);}},
    {"Iny_1", [](Mem& mem) {return make_unique<Iny_1>(mem);}},
    {"Jmp_abs_1", [](Mem& mem) {return make_unique<Jmp_abs_1>(mem);}},
    {"Jmp_absi_1", [](Mem& mem) {return make_unique<Jmp_absi_1>(mem);}},
    {"Jmp_absxi_1", [](Mem& mem) {return make_unique<Jmp_absxi_1>(mem);}},
    {"Jsr_abs_1", [](Mem& mem) {return make_unique<Jsr_abs_1>(mem);}},
    {"Lda_abs_1", [](Mem& mem) {return make_unique<Lda_abs_1>(mem);}},
    {"Lda_absx_1", [](Mem& mem) {return make_unique<Lda_absx_1>(mem);}},
    {"Lda_absy_1", [](Mem& mem) {return make_unique<Lda_absy_1>(mem);}},
    {"Lda_dp_1", [](Mem& mem) {return make_unique<Lda_dp_1>(mem);}},
    {"Lda_dpi_1", [](Mem& mem) {return make_unique<Lda_dpi_1>(mem);}},
    {"Lda_dpiy_1", [](Mem& mem) {return make_unique<Lda_dpiy_1>(mem);}},
    {"Lda_dpx_1", [](Mem& mem) {return make_unique<Lda_dpx_1>(mem);}},
    {"Lda_dpxi_1", [](Mem& mem) {return make_unique<Lda_dpxi_1>(mem);}},
    {"Lda_imm_1", [](Mem& mem) {return make_unique<Lda_imm_1>(mem);}},
    {"Ldx_abs_1", [](Mem& mem) {return make_unique<Ldx_abs_1>(mem);}},
    {"Ldx_absy_1", [](Mem& mem) {return make_unique<Ldx_absy_1>(mem);}},
    {"Ldx_dp_1", [](Mem& mem) {return make_unique<Ldx_dp_1>(mem);}},
    {"Ldx_dpy_1", [](Mem& mem) {return make_unique<Ldx_dpy_1>(mem);}},
    {"Ldx_imm_1", [](Mem& mem) {return make_unique<Ldx_imm_1>(mem);}},
    {"Ldy_abs_1", [](Mem& mem) {return make_unique<Ldy_abs_1>(mem);}},
    {"Ldy_absx_1", [](Mem& mem) {return make_unique<Ldy_absx_1>(mem);}},
    {"Ldy_dp_1", [](Mem& mem) {return make_unique<Ldy_dp_1>(mem);}},
    {"Ldy_dpx_1", [](Mem& mem) {return make_unique<Ldy_dpx_1>(mem);}},
    {"Ldy_imm_1", [](Mem& mem) {return make_unique<Ldy_imm_1>(mem);}},
    {"Lsr_abs_1", [](Mem& mem) {return make_unique<Lsr_abs_1>(mem);}},
    {"Lsr_absx_1", [](Mem& mem) {return make_unique<Lsr_absx_1>(mem);}},
    {"Lsr_acc_1", [](Mem& mem) {return make_unique<Lsr_acc_1>(mem);}},
    {"Lsr_dp_1", [](Mem& mem) {return make_unique<Lsr_dp_1>(mem);}},
    {"Lsr_dpx_1", [](Mem& mem) {return make_unique<Lsr_dpx_1>(mem);}},
    {"Nop_1", [](Mem& mem) {return make_unique<Nop_1>(mem);}},
    {"Ora_abs_1", [](Mem& mem) {return make_unique<Ora_abs_1>(mem);}},
    {"Ora_absx_1", [](Mem& mem) {return make_unique<Ora_absx_1>(mem);}},
    {"Ora_absy_1", [](Mem& mem) {return make_unique<Ora_absy_1>(mem);}},
    {"Ora_dp_1", [](Mem& mem) {return make_unique<Ora_dp_1>(mem);}},
    {"Ora_dpi_1", [](Mem& mem) {return make_unique<Ora_dpi_1>(mem);}},
    {"Ora_dpiy_1", [](Mem& mem) {return make_unique<Ora_dpiy_1>(mem);}},
    {"Ora_dpx_1", [](Mem& mem) {return make_unique<Ora_dpx_1>(mem);}},
    {"Ora_dpxi_1", [](Mem& mem) {return make_unique<Ora_dpxi_1>(mem);}},
    {"Ora_imm_1", [](Mem& mem) {return make_unique<Ora_imm_1>(mem);}},
    {"Pha_1", [](Mem& mem) {return make_unique<Pha_1>(mem);}},
    {"Phx_1", [](Mem& mem) {return make_unique<Phx_1>(mem);}},
    {"Phy_1", [](Mem& mem) {return make_unique<Phy_1>(mem);}},
    {"Rmb_1", [](Mem& mem) {return make_unique<Rmb_1>(mem);}},
    {"Rol_abs_1", [](Mem& mem) {return make_unique<Rol_abs_1>(mem);}},
    {"Rol_absx_1", [](Mem& mem) {return make_unique<Rol_absx_1>(mem);}},
    {"Rol_acc_1", [](Mem& mem) {return make_unique<Rol_acc_1>(mem);}},
    {"Rol_dp_1", [](Mem& mem) {return make_unique<Rol_dp_1>(mem);}},
    {"Rol_dpx_1", [](Mem& mem) {return make_unique<Rol_dpx_1>(mem);}},
    {"Ror_abs_1", [](Mem& mem) {return make_unique<Ror_abs_1>(mem);}},
    {"Ror_absx_1", [](Mem& mem) {return make_unique<Ror_absx_1>(mem);}},
    {"Ror_acc_1", [](Mem& mem) {return make_unique<Ror_acc_1>(mem);}},
    {"Ror_dp_1", [](Mem& mem) {return make_unique<Ror_dp_1>(mem);}},
    {"Ror_dpx_1", [](Mem& mem) {return make_unique<Ror_dpx_1>(mem);}},
    {"Rti_1", [](Mem& mem) {return make_unique<Rti_1>(mem);}},
    {"Rts_1", [](Mem& mem) {return make_unique<Rts_1>(mem);}},
    {"Sbc_abs_1", [](Mem& mem) {return make_unique<Sbc_abs_1>(mem);}},
    {"Sbc_absx_1", [](Mem& mem) {return make_unique<Sbc_absx_1>(mem);}},
    {"Sbc_absy_1", [](Mem& mem) {return make_unique<Sbc_absy_1>(mem);}},
    {"Sbc_dp_1", [](Mem& mem) {return make_unique<Sbc_dp_1>(mem);}},
    {"Sbc_dpi_1", [](Mem& mem) {return make_unique<Sbc_dpi_1>(mem);}},
    {"Sbc_dpiy_1", [](Mem& mem) {return make_unique<Sbc_dpiy_1>(mem);}},
    {"Sbc_dpx_1", [](Mem& mem) {return make_unique<Sbc_dpx_1>(mem);}},
    {"Sbc_dpxi_1", [](Mem& mem) {return make_unique<Sbc_dpxi_1>(mem);}},
    {"Sbc_imm_1", [](Mem& mem) {return make_unique<Sbc_imm_1>(mem);}},
    {"Sec_1", [](Mem& mem) {return make_unique<Sec_1>(mem);}},
    {"Sed_1", [](Mem& mem) {return make_unique<Sed_1>(mem);}},
    {"Sei_1", [](Mem& mem) {return make_unique<Sei_1>(mem);}},
    {"Smb_1", [](Mem& mem) {return make_unique<Smb_1>(mem);}},
    {"Sta_abs_1", [](Mem& mem) {return make_unique<Sta_abs_1>(mem);}},
    {"Sta_absx_1", [](Mem& mem) {return make_unique<Sta_absx_1>(mem);}},
    {"Sta_absy_1", [](Mem& mem) {return make_unique<Sta_absy_1>(mem);}},
    {"Sta_dp_1", [](Mem& mem) {return make_unique<Sta_dp_1>(mem);}},
    {"Sta_dpi_1", [](Mem& mem) {return make_unique<Sta_dpi_1>(mem);}},
    {"Sta_dpiy_1", [](Mem& mem) {return make_unique<Sta_dpiy_1>(mem);}},
    {"Sta_dpx_1", [](Mem& mem) {return make_unique<Sta_dpx_1>(mem);}},
    {"Sta_dpxi_1", [](Mem& mem) {return make_unique<Sta_dpxi_1>(mem);}},
    {"Stp_0", [](Mem& mem) {return make_unique<Stp_0>(mem);}},
    {"Stx_abs_1", [](Mem& mem) {return make_unique<Stx_abs_1>(mem);}},
    {"Stx_dp_1", [](Mem& mem) {return make_unique<Stx_dp_1>(mem);}},
    {"Stx_dpy_1", [](Mem& mem) {return make_unique<Stx_dpy_1>(mem);}},
    {"Sty_abs_1", [](Mem& mem) {return make_unique<Sty_abs_1>(mem);}},
    {"Sty_dp_1", [](Mem& mem) {return make_unique<Sty_dp_1>(mem);}},
    {"Sty_dpx_1", [](Mem& mem) {return make_unique<Sty_dpx_1>(mem);}},
    {"Stz_abs_1", [](Mem& mem) {return make_unique<Stz_abs_1>(mem);}},
    {"Stz_absx_1", [](Mem& mem) {return make_unique<Stz_absx_1>(mem);}},
    {"Stz_dp_1", [](Mem& mem) {return make_unique<Stz_dp_1>(mem);}},
    {"Stz_dpx_1", [](Mem& mem) {return make_unique<Stz_dpx_1>(mem);}},
    {"Tax_1", [](Mem& mem) {return make_unique<Tax_1>(mem);}},
    {"Tay_1", [](Mem& mem) {return make_unique<Tay_1>(mem);}},
    {"Trb_abs_1", [](Mem& mem) {return make_unique<Trb_abs_1>(mem);}},
    {"Trb_dp_1", [](Mem& mem) {return make_unique<Trb_dp_1>(mem);}},
    {"Tsb_abs_1", [](Mem& mem) {return make_unique<Tsb_abs_1>(mem);}},
    {"Tsb_dp_1", [](Mem& mem) {return make_unique<Tsb_dp_1>(mem);}},
    {"Tsx_1", [](Mem& mem) {return make_unique<Tsx_1>(mem);}},
    {"Txa_1", [](Mem& mem) {return make_unique<Txa_1>(mem);}},
    {"Txs_1", [](Mem& mem) {return make_unique<Txs_1>(mem);}},
    {"Tya_1", [](Mem& mem) {return make_unique<Tya_1>(mem);}},
    {"Wai_0", [](Mem& mem) {return make_unique<Wai_0>(mem);}}
};

static unordered_map<string, function<unique_ptr<Test>(Mem&)>>::iterator it =
    fmap.begin();

unique_ptr<Test> named_test(const string& testname, Mem& mem)
{
    return fmap.at(testname)(mem);
}

void start_tests()
{
    it = fmap.begin();
}

string next_test()
{
    if (it == fmap.end())
        return "";
    else
        return (*it++).first;
}


} // namespace w65c02
