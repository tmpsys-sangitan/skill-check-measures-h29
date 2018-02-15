/***********************************************************************/
/*                                                                     */
/*  FILE        :task02.c                                               */
/*  DATE        :Mon, Oct 06, 2017                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3052F                                              */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.16).    */
/*                                                                     */
/***********************************************************************/

#include "iodefine.h"
#include "conv.h"
#include "lcd.h"
#include "rsctl.h"
#pragma interrupt INT_IMIA0(vect=24)

unsigned int c = 0x8fff;
unsigned char md[] = {0x30, 0x60, 0xC0, 0x90};

void INT_IMIA0(void) {
    ITU0.TSR.BIT.IMFA = 0;      // �t���O������
    c += 1;                     // �J�E���g�_�E��
    if(c < 600){                  // �J�E���g��0�ȉ��Ȃ�
        PA.DR.BYTE = md[c%4];   // �M���ύX
    }
}

void main(void)
{
    int i;
    char str[17];

    // �����ݒ�
    init_lcd();                 // LCD������
    clear_lcd();                // LCD��ʃ��Z�b�g
    PA.DDR = 0xf0;              // PA�o�̓��[�h
    ITU0.TCR.BIT.CCLR = 1;      // �J�E���^���Z�b�g�v��
    ITU0.TCR.BIT.TPSC = 3;
    ITU0.GRA = 62499;
    ITU0.TIER.BIT.IMIEA = 1;
    ITU.TSTR.BIT.STR0 = 1;
    
    while(1){
        if(!PA.DR.BIT.B0){
              c = 500;  
        }
        if(!PA.DR.BIT.B1){
              c = 0;  
        }
        
        locate_lcd(0, 0);		// LCD1�s�ڂɃJ�[�\�����ړ�
        itoa(c, str);		    // ��荞�񂾒l�𕶎���ɕϊ�
        lcd_prints(str);        // LCD�ɕ������\��
        lcd_prints("   ");      // LCD�̕s�v�������㏑��
    }
}