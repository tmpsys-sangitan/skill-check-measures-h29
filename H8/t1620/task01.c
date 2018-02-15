/***********************************************************************/
/*                                                                     */
/*  FILE        :task01.c                                               */
/*  DATE        :Mon, Oct 02, 2017                                     */
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

void main(void)
{
	unsigned int i;
	double j;
	char str[10];
	
	// �����ݒ�
	initSCI1(BR38400);					// �{�[���[�g
	init_lcd();							// LCD������
	clear_lcd();
	AD.ADCSR.BYTE = 0x08;				// AN0�P�ꃂ�[�h
	
	while(1){		
		// �A�i���O�M���擾
		AD.ADCSR.BIT.ADST = 1;			// �X�L�����J�n
		while(AD.ADCSR.BIT.ADF != 1);	// �X�L�����I���܂őҋ@
		i = AD.ADDRA >> 6;				// �l��荞��
		clear_lcd();
	
		// �A�i���O�M���\��
		locate_lcd(0, 0);				// LCD1�s�ڂɃJ�[�\�����ړ�
		itoa(i, str);					// ��荞�񂾒l�𕶎���ɕϊ�
		lcd_prints(str);				// LCD�ɕ������\��
		
		// �A�i���O�M���d���l�ϊ��\��
		j = (double)(i * 5) / 1023;		// �d���l�֕ϊ�
		locate_lcd(0, 1);				// LCD2�s�ڂɃJ�[�\�����ړ�
		dtoa(j, 3, str);				// �d���l�𕶎���ɕϊ�
		lcd_prints(str);				// LCD�ɕ������\��
		lcd_prints("V");				// LCD�ɒP�ʂ�\��

		// �V���A�����M
		btoa(i, str);					// ��荞�񂾒l��2�i���ŕ�����ɕϊ�
		SCI1_OUT_s(str);				// �V���A���|�[�g�ɕ�����𑗐M
		
		// �{�^�������������܂őҋ@
		while(PA.DR.BIT.B0);
	}
}