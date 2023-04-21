/***************************
 * Author: Boris Vesely (xvesel92)
 *
 * Subject: IMP
 * Year: 2022/2023
 * ************************/


/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"


/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))


/* Constants specifying delay loop duration */
#define	tdelay1			10000
#define tdelay2 		20

// Buttons
#define BUTTON_RIGHT_MASK 0x400
#define BUTTON_CENT_MASK 0x800
#define BUTTON_DOWN_MASK 0x1000
#define BUTTON_UP_MASK 0x4000000
#define BUTTON_LEFT_MASK 0x8000000


int chosenText = 0;
int interupt = 0;

int characterA[5] = {63, 68, 68, 68, 63};

int characterB[5] = {127, 73, 73, 73, 54};

int characterC[5] = {62, 65, 65, 65, 34};

int characterD[5] = {127, 65, 65, 65, 62};

int characterE[5] = {127, 73, 73, 73, 73};

int characterF[5] = {127, 72, 72, 72, 72};

int characterG[5] = {62, 65, 65, 69, 39};

int characterH[5] = {127, 8, 8, 8, 127};

int characterI[5] = {65, 65, 127, 65, 65};

int characterJ[5] = {2, 5, 1, 1, 126};

int characterK[5] = {127, 8, 20, 34, 65};

int characterL[5] = {127, 1, 1, 1, 1};

int characterM[5] = {127, 32, 16, 32, 127};

int characterN[5] = {127, 48, 8, 6, 127};

int characterO[5] = {127, 65, 65, 65, 127};

int characterP[5] = {127, 72, 72, 72, 48};

int characterQ[5] = {126, 66, 70, 66, 127};

int characterR[5] = {127, 72, 76, 74, 49};

int characterS[5] = {49, 73, 73, 73, 70};

int characterT[5] = {64, 64, 127, 64, 64};

int characterU[5] = {126, 1, 1, 1, 126};

int characterV[5] = {124, 6, 1, 6, 124};

int characterW[5] = {124, 3, 124, 3, 124};

int characterX[5] = {65, 54, 8, 54, 65};

int characterY[5] = {64, 32, 31, 32, 64};

int characterZ[5] = {67, 69, 73, 81, 97};

int characterSpace[5] = {0, 0, 0, 0, 0};

int character0[5] = {127, 67, 93, 97, 127};

int character1[5] = {33, 65, 127, 1, 1};

int character2[5] = {38, 73, 73, 73, 49};

int character3[5] = {34, 73, 73, 73, 54};

int character4[5] = {28, 36, 79, 4, 4};

int character5[5] = {121, 73, 73, 73, 79};

int character6[5] = {127, 73, 73, 73, 79};

int character7[5] = {65, 66, 76, 80, 96};

int character8[5] = {127, 73, 73, 73, 127};

int character9[5] = {121, 73, 73, 73, 127};

int characterQuestionMark[5] = {32, 64, 77, 80, 32};


/* Configuration of the necessary MCU peripherals */
void SystemConfig() {
/* using code from second IMP laboratory
 *
 * needed to change portB to portE
 */
	MCG->C4 |= (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01)); /* Nastav hodinovy podsystem */
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);

	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; /* Aktivuj hodiny pro PORTE */

	PORTE->PCR[10] = ( PORT_PCR_ISF(0x01) /* Nuluj ISF (Interrupt Status Flag) */
                | PORT_PCR_IRQC(0x0A) /* Interrupt enable on failing edge */
                | PORT_PCR_MUX(0x01) /* Pin Mux Control to GPIO */
                | PORT_PCR_PE(0x01) /* Pull resistor enable... */
                | PORT_PCR_PS(0x01)); /* ...select Pull-Up */


    PORTE->PCR[11] = ( PORT_PCR_ISF(0x01) /* Nuluj ISF (Interrupt Status Flag) */
                | PORT_PCR_IRQC(0x0A) /* Interrupt enable on failing edge */
                | PORT_PCR_MUX(0x01) /* Pin Mux Control to GPIO */
                | PORT_PCR_PE(0x01) /* Pull resistor enable... */
                | PORT_PCR_PS(0x01)); /* ...select Pull-Up */

    PORTE->PCR[12] = ( PORT_PCR_ISF(0x01) /* Nuluj ISF (Interrupt Status Flag) */
                | PORT_PCR_IRQC(0x0A) /* Interrupt enable on failing edge */
                | PORT_PCR_MUX(0x01) /* Pin Mux Control to GPIO */
                | PORT_PCR_PE(0x01) /* Pull resistor enable... */
                | PORT_PCR_PS(0x01)); /* ...select Pull-Up */

    PORTE->PCR[26] = ( PORT_PCR_ISF(0x01) /* Nuluj ISF (Interrupt Status Flag) */
                | PORT_PCR_IRQC(0x0A) /* Interrupt enable on failing edge */
                | PORT_PCR_MUX(0x01) /* Pin Mux Control to GPIO */
                | PORT_PCR_PE(0x01) /* Pull resistor enable... */
                | PORT_PCR_PS(0x01)); /* ...select Pull-Up */

    PORTE->PCR[27] = ( PORT_PCR_ISF(0x01) /* Nuluj ISF (Interrupt Status Flag) */
                | PORT_PCR_IRQC(0x0A) /* Interrupt enable on failing edge */
                | PORT_PCR_MUX(0x01) /* Pin Mux Control to GPIO */
                | PORT_PCR_PE(0x01) /* Pull resistor enable... */
                | PORT_PCR_PS(0x01)); /* ...select Pull-Up */

    NVIC_ClearPendingIRQ(PORTE_IRQn); /* Nuluj priznak preruseni od portu E */
    NVIC_EnableIRQ(PORTE_IRQn);       /* Povol preruseni od portu E */

	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;


	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // A0
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A1
	PORTA->PCR[6] = ( 0|PORT_PCR_MUX(0x01) );  // A2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // A3

	/* Set corresponding PTA pins (rows selectors of 74HC154) for GPIO functionality */
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) );  // R0
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) );  // R1
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // R2
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) );  // R3
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) );  // R4
	PORTA->PCR[7] = ( 0|PORT_PCR_MUX(0x01) );   // R5
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) );  // R6
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) );  // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // #EN


	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

	/* Change corresponding PTE port pins as outputs */
	PTE->PDDR = GPIO_PDDR_PDD( GPIO_PIN(28) );

	/* enable clock for timer */
	PIT->MCR = 0x0;

	/* Set timer value and start counting */
	PIT->CHANNEL[0].TCTRL = 0x0u;
	PIT->CHANNEL[0].LDVAL = 0x679B61;
	PIT->CHANNEL[0].TCTRL = 0x1u;
}




/* Variable delay loop */
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
}

void PORTE_IRQHandler() {
	interupt = 1;
    delay(100,20);

    if (PORTE->ISFR & BUTTON_DOWN_MASK && ((GPIOE_PDIR & BUTTON_DOWN_MASK) == 0))
     {
    	chosenText = 1;
     }

    else if (PORTE->ISFR & BUTTON_UP_MASK && ((GPIOE_PDIR & BUTTON_UP_MASK) == 0))
    {
    	chosenText = 2;
    }

    else if (PORTE->ISFR & BUTTON_LEFT_MASK && ((GPIOE_PDIR & BUTTON_LEFT_MASK) == 0))
    {
    	chosenText = 3;
    }

    else if (PORTE->ISFR & BUTTON_RIGHT_MASK && ((GPIOE_PDIR & BUTTON_RIGHT_MASK) == 0))
    {
    	chosenText = 4;
    }

    else if (PORTE->ISFR & BUTTON_CENT_MASK && ((GPIOE_PDIR & BUTTON_CENT_MASK) == 0))
    {
    	chosenText = 5;
    }

    PORTE->ISFR = BUTTON_UP_MASK | BUTTON_LEFT_MASK | BUTTON_DOWN_MASK | BUTTON_RIGHT_MASK | BUTTON_CENT_MASK;

}

/* Conversion of requested column number into the 4-to-16 decoder control.  */
void column_select(unsigned int col_num)
{
	unsigned i, result, col_sel[4];

	for (i =0; i<4; i++) {
		result = col_num / 2;	  // Whole-number division of the input number
		col_sel[i] = col_num % 2;
		col_num = result;

		switch(i) {

			// Selection signal A0
		    case 0:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(8))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(8)));
				break;

			// Selection signal A1
			case 1:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(10))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(10)));
				break;

			// Selection signal A2
			case 2:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(6))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(6)));
				break;

			// Selection signal A3
			case 3:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(11))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(11)));
				break;

			// Otherwise nothing to do...
			default:
				break;
		}
	}
}

/* Conversion of requested row number */
void rows_select(unsigned int rows_num)
{
	unsigned i, result, row_sel[8];

	for (i =0; i<8; i++) {
		result = rows_num / 2;	  // Whole-number division of the input number
		row_sel[i] = rows_num % 2;
		rows_num = result;

		switch(i) {

			// Selection signal R0
		    case 0:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(26)));
				break;

			// Selection signal R1
			case 1:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(24)));
				break;

			// Selection signal R2
			case 2:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(9)));
				break;

			// Selection signal R3
			case 3:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(25)));
				break;

			// Selection signal R4
			case 4:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(28)));
				break;

			// Selection signal R5
			case 5:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(7)));
				break;

			// Selection signal R6
			case 6:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(27)));
				break;

			// Selection signal R7
			case 7:
				((row_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(29)));
				break;


			// Otherwise nothing to do...
			default:
				break;
		}
	}
}

void display_char(int *mask, int position){

	if (position < 0 || position > 19) {
		return;
	}

	int repeat = 5;
	int start = 0;
	if (position < 4) {
		repeat = position + 1;
	}

	if (position > 15) {
		start = position - 15;
		position = 15 + start;
	}

	for (int i = start; i < repeat; i++){
		column_select(position - i);
		rows_select(mask[i]);

		delay(50,20);

		rows_select(0);
	}
}

int strlen(char *text){
	int cnt = 0;

	for (int i = 0; text[i] != '\0'; i++){
		cnt++;
	}

	return cnt;
}



void procesor(char *defaultText){

	start:;

	interupt = 0;
	char *text = defaultText;

	if (chosenText == 1) {

		text = "HELLO";

	} else if (chosenText == 2) {

		text = "IMP 2022";

	} else if (chosenText == 3) {

		text = "XVESEL92";

	} else if (chosenText == 4) {

		text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 ?";

	} else if (chosenText == 5) {

		text = "VUT FIT BRNO";
	}


	int textLen = strlen(text);

	int charPosition[textLen];

	int cnt = 0;

	for(int i = 0; i < textLen; i++){
		charPosition[i] = cnt;
		cnt -= 6 ;
	}

	int positionMove = -1;
	while (1){
		for(int i = 0; i < textLen; i++){

			if(interupt == 1){
				goto start;
			}

			switch (text[i]){

			case 'A':
				display_char(characterA, charPosition[i] + positionMove);
				break;

			case 'B':
				display_char(characterB, charPosition[i] + positionMove);
				break;

			case 'C':
				display_char(characterC, charPosition[i] + positionMove);
				break;

			case 'D':
				display_char(characterD, charPosition[i] + positionMove);
				break;

			case 'E':
				display_char(characterE, charPosition[i] + positionMove);
				break;

			case 'F':
				display_char(characterF, charPosition[i] + positionMove);
				break;

            case 'G':
                display_char(characterG, charPosition[i] + positionMove);
                break;

            case 'H':
                display_char(characterH, charPosition[i] + positionMove);
                break;

            case 'I':
                display_char(characterI, charPosition[i] + positionMove);
                break;

            case 'J':
                display_char(characterJ, charPosition[i] + positionMove);
                break;

            case 'K':
                display_char(characterK, charPosition[i] + positionMove);
                break;

            case 'L':
                display_char(characterL, charPosition[i] + positionMove);
                break;

            case 'M':
                display_char(characterM, charPosition[i] + positionMove);
                break;

            case 'N':
                display_char(characterN, charPosition[i] + positionMove);
                break;
            case 'O':
                display_char(characterO, charPosition[i] + positionMove);
                break;

            case 'P':
                display_char(characterP, charPosition[i] + positionMove);
                break;

            case 'Q':
                display_char(characterQ, charPosition[i] + positionMove);
                break;

            case 'R':
                display_char(characterR, charPosition[i] + positionMove);
                break;

            case 'S':
                display_char(characterS, charPosition[i] + positionMove);
                break;

            case 'T':
                display_char(characterT, charPosition[i] + positionMove);
                break;

            case 'U':
                display_char(characterU, charPosition[i] + positionMove);
                break;

            case 'V':
                display_char(characterV, charPosition[i] + positionMove);
                break;

            case 'W':
                display_char(characterW, charPosition[i] + positionMove);
                break;

            case 'X':
                display_char(characterX, charPosition[i] + positionMove);
                break;

            case 'Y':
                display_char(characterY, charPosition[i] + positionMove);
                break;

            case 'Z':
                display_char(characterZ, charPosition[i] + positionMove);
                break;

            case ' ':
                display_char(characterSpace, charPosition[i] + positionMove);
                break;

            case '?':
            	display_char(characterQuestionMark, charPosition[i] + positionMove);
            	break;

            case '0':
            	display_char(character0, charPosition[i] + positionMove);
                break;

            case '1':
            	display_char(character1, charPosition[i] + positionMove);
                break;

            case '2':
            	display_char(character2, charPosition[i] + positionMove);
                break;

            case '3':
                display_char(character3, charPosition[i] + positionMove);
                break;

            case '4':
                display_char(character4, charPosition[i] + positionMove);
                break;

            case '5':
                display_char(character5, charPosition[i] + positionMove);
                break;

            case '6':
                display_char(character6, charPosition[i] + positionMove);
                break;

            case '7':
                display_char(character7, charPosition[i] + positionMove);
                break;

            case '8':
                display_char(character8, charPosition[i] + positionMove);
                break;

            case '9':
                display_char(character9, charPosition[i] + positionMove);
                break;

            default:
            	display_char(characterQuestionMark, charPosition[i] + positionMove);
            	break;
			}
		}

		if (PIT->CHANNEL[0].TFLG){

			positionMove++;
			PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

			if((charPosition[textLen - 1] + positionMove) == 20 ){
				delay(10000, 100);
				positionMove = -5;
			}


		}
	}

}


int main(void)
{
	SystemConfig();

	char *text = "DEFAULT TEXT";


	procesor(text);


    for (;;) {}
    /* Never leave main */
    return 0;
}
