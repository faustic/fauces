/*
Licensed under the MIT License

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

package com.faustic.cpu.wdc65x;

import com.faustic.memory.*;

public class Parser
{
    Parser(Processor processor, Mem mem)
    {
        this.processor = processor;
        this.mem = mem;
    }
    
    public int parse(int addr)
    {
        int opcode = mem.read8(addr++);
        program_counter = instruction[opcode].execute(processor, addr);
        return program_counter;
    }
    
    public void parse()
    {
        parse(program_counter);
    }

    public Mem mem()
    {
        return mem;
    }
    
    private Processor processor;
    private Mem mem;
    private int program_counter;

    private Instruction adc_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_abs(pc);
        }
    };    
    
    private Instruction adc_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_abs_x(pc);
        }
    };    
    
    private Instruction adc_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_abs_y(pc);
        }
    };    
    
    private Instruction adc_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_abslong(pc);
        }
    };    
    
    private Instruction adc_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_abslong_x(pc);
        }
    };    
    
    private Instruction adc_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_dp(pc);
        }
    };    
    
    private Instruction adc_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_dp__(pc);
        }
    };    
    
    private Instruction adc_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_dp_x(pc);
        }
    };    
    
    private Instruction adc_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_dp_x__(pc);
        }
    };    
    
    private Instruction adc_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_dp___y(pc);
        }
    };    
    
    private Instruction adc_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_dp__long(pc);
        }
    };    
    
    private Instruction adc_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_dp__long_y(pc);
        }
    };    
    
    private Instruction adc_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_imm(pc);
        }
    };    
    
    private Instruction adc_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_sr(pc);
        }
    };    
    
    private Instruction adc_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.adc_sr__y(pc);
        }
    };    
    
    private Instruction and_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_abs(pc);
        }
    };    
    
    private Instruction and_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_abs_x(pc);
        }
    };    
    
    private Instruction and_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_abs_y(pc);
        }
    };    
    
    private Instruction and_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_abslong(pc);
        }
    };    
    
    private Instruction and_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_abslong_x(pc);
        }
    };    
    
    private Instruction and_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_dp(pc);
        }
    };    
    
    private Instruction and_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_dp__(pc);
        }
    };    
    
    private Instruction and_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_dp_x(pc);
        }
    };    
    
    private Instruction and_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_dp_x__(pc);
        }
    };    
    
    private Instruction and_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_dp___y(pc);
        }
    };    
    
    private Instruction and_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_dp__long(pc);
        }
    };    
    
    private Instruction and_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_dp__long_y(pc);
        }
    };    
    
    private Instruction and_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_imm(pc);
        }
    };    
    
    private Instruction and_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_sr(pc);
        }
    };    
    
    private Instruction and_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.and_sr__y(pc);
        }
    };    
    
    private Instruction asl_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.asl_abs(pc);
        }
    };    
    
    private Instruction asl_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.asl_abs_x(pc);
        }
    };    
    
    private Instruction asl_acc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.asl_acc(pc);
        }
    };    
    
    private Instruction asl_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.asl_dp(pc);
        }
    };    
    
    private Instruction asl_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.asl_dp_x(pc);
        }
    };    
    
    private Instruction bcc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bcc(pc);
        }
    };    
    
    private Instruction bcs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bcs(pc);
        }
    };    
    
    private Instruction beq = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.beq(pc);
        }
    };    
    
    private Instruction bit_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bit_abs(pc);
        }
    };    
    
    private Instruction bit_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bit_abs_x(pc);
        }
    };    
    
    private Instruction bit_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bit_dp(pc);
        }
    };    
    
    private Instruction bit_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bit_dp_x(pc);
        }
    };    
    
    private Instruction bit_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bit_imm(pc);
        }
    };    
    
    private Instruction bmi = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bmi(pc);
        }
    };    
    
    private Instruction bne = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bne(pc);
        }
    };    
    
    private Instruction bpl = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bpl(pc);
        }
    };    
    
    private Instruction bra = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bra(pc);
        }
    };    
    
    private Instruction brk = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.brk(pc);
        }
    };    
    
    private Instruction brl = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.brl(pc);
        }
    };    
    
   private Instruction bvc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bvc(pc);
        }
    };    
    
    private Instruction bvs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.bvs(pc);
        }
    };    
    
    private Instruction clc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.clc(pc);
        }
    };    
    
    private Instruction cld = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cld(pc);
        }
    };    
    
    private Instruction cli = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cli(pc);
        }
    };    
    
    private Instruction clv = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.clv(pc);
        }
    };    
    
    private Instruction cmp_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_abs(pc);
        }
    };    
    
    private Instruction cmp_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_abs_x(pc);
        }
    };    
    
    private Instruction cmp_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_abs_y(pc);
        }
    };    
    
    private Instruction cmp_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_abslong(pc);
        }
    };    
    
    private Instruction cmp_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_abslong_x(pc);
        }
    };    
    
    private Instruction cmp_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_dp(pc);
        }
    };    
    
    private Instruction cmp_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_dp__(pc);
        }
    };    
    
    private Instruction cmp_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_dp_x(pc);
        }
    };    
    
    private Instruction cmp_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_dp_x__(pc);
        }
    };    
    
    private Instruction cmp_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_dp___y(pc);
        }
    };    
    
    private Instruction cmp_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_dp__long(pc);
        }
    };    
    
    private Instruction cmp_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_dp__long_y(pc);
        }
    };    
    
    private Instruction cmp_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_imm(pc);
        }
    };    
    
    private Instruction cmp_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_sr(pc);
        }
    };    
    
    private Instruction cmp_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cmp_sr__y(pc);
        }
    };    
    
    private Instruction cop = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cop(pc);
        }
    };    
    
    private Instruction cpx_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cpx_abs(pc);
        }
    };    
    
    private Instruction cpx_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cpx_dp(pc);
        }
    };    
    
    private Instruction cpx_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cpx_imm(pc);
        }
    };    
    
    private Instruction cpy_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cpy_abs(pc);
        }
    };    
    
    private Instruction cpy_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cpy_dp(pc);
        }
    };    
    
    private Instruction cpy_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.cpy_imm(pc);
        }
    };    
    
    private Instruction dec_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.dec_abs(pc);
        }
    };    
    
    private Instruction dec_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.dec_abs_x(pc);
        }
    };    
    
    private Instruction dec_acc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.dec_acc(pc);
        }
    };    
    
    private Instruction dec_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.dec_dp(pc);
        }
    };    
    
    private Instruction dec_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.dec_dp_x(pc);
        }
    };    
    
    private Instruction dex = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.dex(pc);
        }
    };    
    
    private Instruction dey = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.dey(pc);
        }
    };    
    
    private Instruction eor_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_abs(pc);
        }
    };    
    
    private Instruction eor_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_abs_x(pc);
        }
    };    
    
    private Instruction eor_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_abs_y(pc);
        }
    };    
    
    private Instruction eor_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_abslong(pc);
        }
    };    
    
    private Instruction eor_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_abslong_x(pc);
        }
    };    
    
    private Instruction eor_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_dp(pc);
        }
    };    
    
    private Instruction eor_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_dp__(pc);
        }
    };    
    
    private Instruction eor_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_dp_x(pc);
        }
    };    
    
    private Instruction eor_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_dp_x__(pc);
        }
    };    
    
    private Instruction eor_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_dp___y(pc);
        }
    };    
    
    private Instruction eor_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_dp__long(pc);
        }
    };    
    
    private Instruction eor_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_dp__long_y(pc);
        }
    };    
    
    private Instruction eor_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_imm(pc);
        }
    };    
    
    private Instruction eor_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_sr(pc);
        }
    };    
    
    private Instruction eor_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.eor_sr__y(pc);
        }
    };    
    
    private Instruction inc_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.inc_abs(pc);
        }
    };    
    
    private Instruction inc_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.inc_abs_x(pc);
        }
    };    
    
    private Instruction inc_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.inc_dp_x(pc);
        }
    };    
    
    private Instruction inc_acc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.inc_acc(pc);
        }
    };    
    
    private Instruction inc_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.inc_dp(pc);
        }
    };    
    
    private Instruction inx = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.inx(pc);
        }
    };    
    
    private Instruction iny = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.iny(pc);
        }
    };    
    
    private Instruction jml = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jml(pc);
        }
    };
    
    private Instruction jml__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jml__(pc);
        }
    };
    
    private Instruction jmp_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jmp_abs(pc);
        }
    };    
    
    private Instruction jmp_abs__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jmp_abs__(pc);
        }
    };    
    
    private Instruction jmp_abs_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jmp_abs_x__(pc);
        }
    };    
    
    private Instruction jsl = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jsl(pc);
        }
    };    
    
    private Instruction jsr_abs_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jsr_abs_x__(pc);
        }
    };
    
    private Instruction jsr_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.jsr_abs(pc);
        }
    };    
    
    private Instruction lda_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_abs(pc);
        }
    };    
    
    private Instruction lda_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_abs_x(pc);
        }
    };    
    
    private Instruction lda_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_abs_y(pc);
        }
    };    
    
    private Instruction lda_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_abslong(pc);
        }
    };    
    
    private Instruction lda_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_abslong_x(pc);
        }
    };    
    
    private Instruction lda_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_dp(pc);
        }
    };    
    
    private Instruction lda_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_dp__(pc);
        }
    };    
    
    private Instruction lda_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_dp_x(pc);
        }
    };    
    
    private Instruction lda_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_dp_x__(pc);
        }
    };    
    
    private Instruction lda_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_dp___y(pc);
        }
    };    
    
    private Instruction lda_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_dp__long(pc);
        }
    };    
    
    private Instruction lda_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_dp__long_y(pc);
        }
    };    
    
    private Instruction lda_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_imm(pc);
        }
    };    
    
    private Instruction lda_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_sr(pc);
        }
    };    
    
    private Instruction lda_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lda_sr__y(pc);
        }
    };    
    
    private Instruction ldx_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldx_abs(pc);
        }
    };    
    
    private Instruction ldx_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldx_abs_y(pc);
        }
    };    
    
    private Instruction ldx_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldx_dp(pc);
        }
    };    
    
    private Instruction ldx_dp_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldx_dp_y(pc);
        }
    };    
    
    private Instruction ldx_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldx_imm(pc);
        }
    };    
    
    private Instruction ldy_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldy_abs(pc);
        }
    };    
    
    private Instruction ldy_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldy_abs_x(pc);
        }
    };    
    
    private Instruction ldy_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldy_dp(pc);
        }
    };    
    
    private Instruction ldy_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldy_dp_x(pc);
        }
    };    
    
    private Instruction ldy_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ldy_imm(pc);
        }
    };    
    
    private Instruction lsr_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lsr_abs(pc);
        }
    };    
    
    private Instruction lsr_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lsr_abs_x(pc);
        }
    };    
    
    private Instruction lsr_acc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lsr_acc(pc);
        }
    };    
    
    private Instruction lsr_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lsr_dp(pc);
        }
    };    
    
    private Instruction lsr_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.lsr_dp_x(pc);
        }
    };    
    
    private Instruction mvn = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.mvn(pc);
        }
    };
    
    private Instruction mvp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.mvp(pc);
        }
    };
    
    private Instruction nop = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.nop(pc);
        }
    };    
    
    private Instruction ora_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_abs(pc);
        }
    };    
    
    private Instruction ora_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_abs_x(pc);
        }
    };    
    
    private Instruction ora_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_abs_y(pc);
        }
    };    
    
    private Instruction ora_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_abslong(pc);
        }
    };    
    
    private Instruction ora_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_abslong_x(pc);
        }
    };    
    
    private Instruction ora_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_dp(pc);
        }
    };    
    
    private Instruction ora_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_dp__(pc);
        }
    };    
    
    private Instruction ora_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_dp_x(pc);
        }
    };    
    
    private Instruction ora_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_dp_x__(pc);
        }
    };    
    
    private Instruction ora_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_dp___y(pc);
        }
    };    
    
    private Instruction ora_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_dp__long(pc);
        }
    };    
    
    private Instruction ora_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_dp__long_y(pc);
        }
    };    
    
    private Instruction ora_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_imm(pc);
        }
    };    
    
    private Instruction ora_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_sr(pc);
        }
    };    
    
    private Instruction ora_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ora_sr__y(pc);
        }
    };    
    
    private Instruction pea = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.pea(pc);
        }
    };
    
    private Instruction pei = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.pei(pc);
        }
    };
    
    private Instruction per = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.per(pc);
        }
    };    
    
    private Instruction pha = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.pha(pc);
        }
    };    
    
    private Instruction phb = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.phb(pc);
        }
    };    
    
    private Instruction phd = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.phd(pc);
        }
    };    
    
    private Instruction phk = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.phk(pc);
        }
    };    
    
    private Instruction php = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.php(pc);
        }
    };    
    
    private Instruction phx = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.phx(pc);
        }
    };    
    
    private Instruction phy = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.phy(pc);
        }
    };    
    
    private Instruction pla = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.pla(pc);
        }
    };    
    
    private Instruction plb = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.plb(pc);
        }
    };    
    
    private Instruction pld = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.pld(pc);
        }
    };    
    
    private Instruction plp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.plp(pc);
        }
    };    
    
    private Instruction plx = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.plx(pc);
        }
    };    
    
    private Instruction ply = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ply(pc);
        }
    };    
    
    private Instruction rep = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rep(pc);
        }
    };    
    
    private Instruction rol_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rol_abs(pc);
        }
    };    
    
    private Instruction rol_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rol_abs_x(pc);
        }
    };    
    
    private Instruction rol_acc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rol_acc(pc);
        }
    };    
    
    private Instruction rol_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rol_dp(pc);
        }
    };    
    
    private Instruction rol_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rol_dp_x(pc);
        }
    };    
    
    private Instruction ror_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ror_abs(pc);
        }
    };    
    
    private Instruction ror_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ror_abs_x(pc);
        }
    };    
    
    private Instruction ror_acc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ror_acc(pc);
        }
    };    
    
    private Instruction ror_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ror_dp(pc);
        }
    };    
    
    private Instruction ror_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.ror_dp_x(pc);
        }
    };    
    
    private Instruction rti = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rti(pc);
        }
    };    
    
    private Instruction rtl = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rtl(pc);
        }
    };    
    
    private Instruction rts = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.rts(pc);
        }
    };    
    
    private Instruction sbc_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_abs(pc);
        }
    };    
    
    private Instruction sbc_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_abs_x(pc);
        }
    };    
    
    private Instruction sbc_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_abs_y(pc);
        }
    };    
    
    private Instruction sbc_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_abslong(pc);
        }
    };    
    
    private Instruction sbc_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_abslong_x(pc);
        }
    };    
    
    private Instruction sbc_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_dp(pc);
        }
    };    
    
    private Instruction sbc_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_dp__(pc);
        }
    };    
    
    private Instruction sbc_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_dp_x(pc);
        }
    };    
    
    private Instruction sbc_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_dp_x__(pc);
        }
    };    
    
    private Instruction sbc_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_dp___y(pc);
        }
    };    
    
    private Instruction sbc_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_dp__long(pc);
        }
    };    
    
    private Instruction sbc_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_dp__long_y(pc);
        }
    };    
    
    private Instruction sbc_imm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_imm(pc);
        }
    };    
    
    private Instruction sbc_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_sr(pc);
        }
    };    
    
    private Instruction sbc_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sbc_sr__y(pc);
        }
    };    
    
    private Instruction sec = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sec(pc);
        }
    };    
    
    private Instruction sed = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sed(pc);
        }
    };    
    private Instruction sei = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sei(pc);
        }
    };    
    
    private Instruction sep = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sep(pc);
        }
    };    
    
    private Instruction sta_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_abs(pc);
        }
    };    
    
    private Instruction sta_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_abs_x(pc);
        }
    };    
    
    private Instruction sta_abs_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_abs_y(pc);
        }
    };    
    
    private Instruction sta_abslong = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_abslong(pc);
        }
    };    
    
    private Instruction sta_abslong_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_abslong_x(pc);
        }
    };    
    
    private Instruction sta_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_dp(pc);
        }
    };    
    
    private Instruction sta_dp__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_dp__(pc);
        }
    };    
    
    private Instruction sta_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_dp_x(pc);
        }
    };    
    
    private Instruction sta_dp_x__ = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_dp_x__(pc);
        }
    };    
    
    private Instruction sta_dp___y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_dp___y(pc);
        }
    };    
    
    private Instruction sta_dp__long = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_dp__long(pc);
        }
    };    
    
    private Instruction sta_dp__long_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_dp__long_y(pc);
        }
    };    
    
    private Instruction sta_sr = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_sr(pc);
        }
    };    
    
    private Instruction sta_sr__y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sta_sr__y(pc);
        }
    };    
    
    private Instruction stp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stp(pc);
        }
    };

    private Instruction stx_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stx_abs(pc);
        }
    };    
    
    private Instruction stx_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stx_dp(pc);
        }
    };    
    
    private Instruction stx_dp_y = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stx_dp_y(pc);
        }
    };    
    
    private Instruction sty_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sty_abs(pc);
        }
    };    
    
    private Instruction sty_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sty_dp(pc);
        }
    };    
    
    private Instruction sty_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.sty_dp_x(pc);
        }
    };    
    
    private Instruction stz_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stz_abs(pc);
        }
    };    
    
    private Instruction stz_abs_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stz_abs_x(pc);
        }
    };    
    
    private Instruction stz_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stz_dp(pc);
        }
    };    
    
    private Instruction stz_dp_x = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.stz_dp_x(pc);
        }
    };    
    
    private Instruction tax = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tax(pc);
        }
    };    
    
    private Instruction tay = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tay(pc);
        }
    };    
    
    private Instruction tcd = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tcd(pc);
        }
    };    
    
    private Instruction tcs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tcs(pc);
        }
    };    
    
    private Instruction tdc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tdc(pc);
        }
    };    
    
   private Instruction trb_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.trb_abs(pc);
        }
    };    
    
    private Instruction trb_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.trb_dp(pc);
        }
    };    
    
    private Instruction tsb_abs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tsb_abs(pc);
        }
    };    
    
    private Instruction tsb_dp = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tsb_dp(pc);
        }
    };    
    
    private Instruction tsc = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tsc(pc);
        }
    };    
    
    private Instruction tsx = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tsx(pc);
        }
    };    
    
    private Instruction txa = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.txa(pc);
        }
    };    
    
    private Instruction txs = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.txs(pc);
        }
    };    
    
    private Instruction txy = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.txy(pc);
        }
    };    
    
    private Instruction tya = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tya(pc);
        }
    };

    private Instruction tyx = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.tyx(pc);
        }
    };    
    
    private Instruction wai = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.wai(pc);
        }
    };

    private Instruction wdm = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.wdm(pc);
        }
    };    
    
    private Instruction xba = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.xba(pc);
        }
    };    
    
    private Instruction xce = new Instruction()
    {
        public int execute(Processor processor, int pc)
        {
            return processor.xce(pc);
        }
    };    
    
    private Instruction[] instruction = new Instruction[]
    {
        brk, ora_dp_x__, cop, ora_sr, //00-03
        tsb_dp, ora_dp, asl_dp, ora_dp__long, 
        php, ora_imm, asl_acc, phd,
        tsb_abs, ora_abs, asl_abs, ora_abslong, 

        bpl, ora_dp___y, ora_dp__, ora_sr__y, //10-13
        trb_dp, ora_dp_x, asl_dp_x, ora_dp__long_y, 
        clc, ora_abs_y, inc_acc, tcs,
        trb_abs, ora_abs_x, asl_abs_x, ora_abslong_x, 

        jsr_abs, and_dp_x__, jsl, and_sr, // 20-23
        bit_dp, and_dp, rol_dp, and_dp__long, 
        plp, and_imm, rol_acc, pld,
        bit_abs, and_abs, rol_abs, and_abslong, 

        bmi, and_dp___y, and_dp__, and_sr__y, // 30-33
        bit_dp_x, and_dp_x, rol_dp_x, and_dp__long_y, 
        sec, and_abs_y, dec_acc, tsc,
        bit_abs_x, and_abs_x, rol_abs_x, and_abslong_x, 

        rti, eor_dp_x__, wdm, eor_sr, // 40-43
        mvp, eor_dp, lsr_dp, eor_dp__long, 
        pha, eor_imm, lsr_acc, phk,
        jmp_abs, eor_abs, lsr_abs, eor_abslong, 

        bvc, eor_dp___y, eor_dp__, eor_sr__y, // 50-53
        mvn, eor_dp_x, lsr_dp_x, eor_dp__long_y, 
        cli, eor_abs_y, phy, tcd,
        jml, eor_abs_x, lsr_abs_x, eor_abslong_x, 

        rts, adc_dp_x__, per, adc_sr, // 60-63
        stz_dp, adc_dp, ror_dp, adc_dp__long, 
        pla, adc_imm, ror_acc, rtl,
        jmp_abs__, adc_abs, ror_abs, adc_abslong, 

        bvs, adc_dp___y, adc_dp__, adc_sr__y, // 70-73
        stz_dp_x, adc_dp_x, ror_dp_x, adc_dp__long_y, 
        sei, adc_abs_y, ply, tdc,
        jmp_abs_x__, adc_abs_x, ror_abs_x, adc_abslong_x, 

        bra, sta_dp_x__, brl, sta_sr, // 80-83
        sty_dp, sta_dp, stx_dp, sta_dp__long, 
        dey, bit_imm, txa, phb,
        sty_abs, sta_abs, stx_abs, sta_abslong, 

        bcc, sta_dp___y, sta_dp__, sta_sr__y, // 90-93
        sty_dp_x, sta_dp_x, stx_dp_y, sta_dp__long_y, 
        tya, sta_abs_y, txs, txy,
        stz_abs, sta_abs_x, stz_abs_x, sta_abslong_x, 

        ldy_imm, lda_dp_x__, ldx_imm, lda_sr, // a0-a3
        ldy_dp, lda_dp, ldx_dp, lda_dp__long, 
        tay, lda_imm, tax, plb,
        ldy_abs, lda_abs, ldx_abs, lda_abslong, 

        bcs, lda_dp___y, lda_dp__, lda_sr__y, // b0-b3
        ldy_dp_x, lda_dp_x, ldx_dp_y, lda_dp__long_y, 
        clv, lda_abs_y, tsx, tyx,
        ldy_abs_x, lda_abs_x, ldx_abs_y, lda_abslong_x, 

        cpy_imm, cmp_dp_x__, rep, cmp_sr, // c0-c3
        cpy_dp, cmp_dp, dec_dp, cmp_dp__long, 
        iny, cmp_imm, dex, wai,
        cpy_abs, cmp_abs, dec_abs, cmp_abslong, 

        bne, cmp_dp___y, cmp_dp__, cmp_sr__y, // d0-d3
        pei, cmp_dp_x, dec_dp_x, cmp_dp__long_y, 
        cld, cmp_abs_y, phx, stp,
        jml__, cmp_abs_x, dec_abs_x, cmp_abslong_x, 

        cpx_imm, sbc_dp_x__, sep, sbc_sr, // e0-e3
        cpx_dp, sbc_dp, inc_dp, sbc_dp__long, 
        inx, sbc_imm, nop, xba,
        cpx_abs, sbc_abs, inc_abs, sbc_abslong, 

        beq, sbc_dp___y, sbc_dp__, sbc_sr__y, // f0-f3
        pea, sbc_dp_x, inc_dp_x, sbc_dp__long_y, 
        sed, sbc_abs_y, plx, xce,
        jsr_abs_x__, sbc_abs_x, inc_abs_x, sbc_abslong_x
    };
}

interface Instruction
{
    int execute(Processor processor, int pc);
}
