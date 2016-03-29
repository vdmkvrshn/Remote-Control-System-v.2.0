#line 1 "prog_1.c"
#line 1 "prog_1.c"

#line 1 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"

#line 3 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"

#line 1 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdarg.h"
 


#line 5 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdarg.h"

typedef void* va_list;
#line 8 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdarg.h"
#line 9 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdarg.h"
#line 10 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdarg.h"
#line 11 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdarg.h"
#line 12 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdarg.h"
#line 4 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"

#line 1 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"
 

#line 4 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"

typedef unsigned char wchar_t;


#line 10 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"
 
typedef signed short int ptrdiff_t;
typedef signed short int ptrdiffram_t;
typedef signed short long int ptrdiffrom_t;


#line 20 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"
 
typedef unsigned short int size_t;
typedef unsigned short int sizeram_t;
typedef unsigned short long int sizerom_t;


#line 34 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"
 
#line 36 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"


#line 41 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"
 
#line 43 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"

#line 45 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stddef.h"
#line 5 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"



#line 9 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"
 
#line 11 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"

#line 13 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"


typedef unsigned char FILE;

 
#line 19 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"
#line 20 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"

extern FILE *stderr;
extern FILE *stdout;


int putc (auto char c, auto FILE *f);
int vsprintf (auto char *buf, auto const far  rom char *fmt, auto va_list ap);
int vprintf (auto const far  rom char *fmt, auto va_list ap);
int sprintf (auto char *buf, auto const far  rom char *fmt, ...);
int printf (auto const far  rom char *fmt, ...);
int fprintf (auto FILE *f, auto const far  rom char *fmt, ...);
int vfprintf (auto FILE *f, auto const far  rom char *fmt, auto va_list ap);
int puts (auto const far  rom char *s);
int fputs (auto const far  rom char *s, auto FILE *f);

#line 36 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdio.h"
#line 2 "prog_1.c"

#line 1 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 

#line 4 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"

#line 6 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"

#line 9 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
 

#line 16 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
double atof (const auto char *s);

#line 28 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
signed char atob (const auto char *s);


#line 39 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
int atoi (const auto char *s);

#line 47 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
long atol (const auto char *s);

#line 58 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
unsigned long atoul (const auto char *s);


#line 71 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
char *btoa (auto signed char value, auto char *s);

#line 83 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
char *itoa (auto int value, auto char *s);

#line 95 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
char *ltoa (auto long value, auto char *s);

#line 107 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
char *ultoa (auto unsigned long value, auto char *s);
 


#line 112 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
 

#line 116 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
#line 118 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"


#line 124 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
int rand (void);

#line 136 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
 
void srand (auto unsigned int seed);
 
#line 140 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
#line 149 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"

#line 151 "C:/Program Files (x86)/Microchip/MPLABC18/v3.45/bin/../h/stdlib.h"
#line 3 "prog_1.c"



__CONFIG(FOSC_HS & WDTE_ON & PWRTE_ON & BOREN_OFF & LVP_OFF & CPD_OFF & WRT_OFF & CP_ON);

volatile unsigned char digits[2][16];
volatile unsigned char modifide_digits[2][16];
long int Clock;
long int Minutes = 0;
unsigned int LCD_ON_TIMEOUT = 60000; 
volatile unsigned char KeyCode;
unsigned int CorrectTimeConst;
unsigned char AdressOfNextStartCell = 240;
unsigned long int NearTimeStart;
unsigned long int NearTimeStop;
unsigned char CurrentReadingCell = 0;
unsigned char CurrentSignalsData = 0;
volatile unsigned char SignalsOut = 0;
volatile unsigned char SignalsForInd = 0;
bit DetailModeOfViewSheduler = 1;
bit RelevanceOfNextStartCell;
bit ModeOfFirstLine = 0;
bit TimeIsRead = 0;

unsigned char       cMinutes = 0;
unsigned char       cHours   = 0;
unsigned char       cWeekDay = 4;
unsigned char       cDays    = 1;
unsigned char       cMonths  = 1;
unsigned char const	cYears   = 0x16; 

unsigned char const cMinutesAdress = 240;
unsigned char const cHoursAdress   = 241;
unsigned char const cWeekDayAdress = 242;
unsigned char const cDaysAdress    = 243;
unsigned char const cMonthsAdress  = 244;

bit isTimeSetting  = 0;

bit LCD_Light_On = 0;
bit LCD_Power_On = 0;
bit LockSignals = 0;
unsigned char CurrentSignals = 0;
bit IsLCDModified;
bit GlobalBlink;
unsigned char GlobalBlinkCycleTime = 100;

#line 51 "prog_1.c"

#line 53 "prog_1.c"
#line 54 "prog_1.c"
#line 55 "prog_1.c"

#line 57 "prog_1.c"
#line 58 "prog_1.c"
#line 59 "prog_1.c"


void main2 (void);
void lcd_init (void);
void lcd_on (void);
void lcd_off (void);
void SendArrayToLCD(unsigned char A[], char B, char C);
void ParseTime(long int TimeValue, unsigned int *Time, unsigned char *Day);
void ParseDataRecord(unsigned long int Data, unsigned int *TimeStart, unsigned int *TimeStop, unsigned char *Days, unsigned char *Signals);
void EEWR(unsigned char adress, unsigned char data);
void Scheduler(unsigned char StartFrom, unsigned char new);
void SignalsOnOff(void);
void FillMinutes(void);
unsigned char EERD(unsigned char adress);
unsigned long int ReadFourBytesEE(unsigned char adress);
unsigned char *GetDayOfWeek(unsigned char day);
unsigned char EditSchedule(unsigned char adress, unsigned char SourceOfRecord);
unsigned char FindNextTimeStart(unsigned long int *TimeFrom);void I2CInit(void);
void I2CStart(void);
void I2CStop(void);
void I2CRestart(void);
void I2CAck(void);
void I2CNak(void);
void I2CWait(void);
void I2CSend(unsigned char dat);
unsigned char I2CRead(void);
void ReadTime();
void WriteTime(unsigned char Minutes, unsigned char Hours, unsigned char DayOfWeek);
unsigned char BCD_to_DEC(unsigned char BCD);
unsigned char DEC_to_BCD(unsigned char DEC);

unsigned char getDigit(char line, char symbol){
	return (digits[line][symbol-1] & 0b01111111);
}
void setDigit(char line, char symbol, unsigned char data){
	unsigned char value  = data & 0b01111111;
	unsigned char value0 = digits[line][symbol-1];
	unsigned char e0     = value0 & 0b10000000;
	value0 &= 0b01111111;
	modifide_digits[line][symbol-1] = modifide_digits[line][symbol-1]>0 || (value != value0);
	IsLCDModified = IsLCDModified || (value != value0);
	digits[line][symbol-1] = value | e0;
}
void setBlink(char line, char symbol, char value){
	unsigned char value0 = digits[line][symbol-1];
	if(value > 0){
		digits[line][symbol-1] |= 0b10000000;
	}else{
		digits[line][symbol-1] &= 0b01111111;
	}
	if(digits[line][symbol-1] != value0){
		modifide_digits[line][symbol-1] = 1;
		IsLCDModified = 1;
	}
}
void clrInd(){
	for(char b = 0; b < 2; b++){
		for(char a = 0; a < 16; a++){
			modifide_digits[b][a] = 1;
			digits[b][a] = 34;
		}
	}
	IsLCDModified = 1;
}
unsigned char convDig_LCD(unsigned char dig){
	switch (dig){
		case  0: return '0';
		case  1: return '1';
		case  2: return '2';
		case  3: return '3';
		case  4: return '4';
		case  5: return '5';
		case  6: return '6';
		case  7: return '7';
		case  8: return '8';
		case  9: return '9';
		case 10: return 'a';
		case 11: return 'b';
		case 12: return 'c';
		case 13: return 'd';
		case 14: return 'e';
		case 15: return 'f';
		case 16: return 'g';
		case 17: return 'h';
		case 18: return 'i';
		case 19: return 'j';
		case 20: return 'l';
		case 21: return 'n';
		case 22: return 'o';
		case 23: return 'p';
		case 24: return 'r';
		case 25: return 's';
		case 26: return 't';
		case 27: return 'u';
		case 28: return 'v';
		case 29: return 'y';
		case 30: return 'z';
		case 31: return '°';
		case 32: return '-';
		case 33: return '_';
		case 34: return ' ';
		case 35: return ':';
		case 36: return 'w';
		case 37: return 'm';
		case 38: return 'k';
		case 39: return 'x';
		case 40: return 'q';
		case 41: return 0b01100001;
		case 42: return 0b10110010;
		case 43: return 0b10110011;
		case 44: return 0b10110100;
		case 45: return 0b11100011;
		case 46: return 0b01100101;
		case 47: return 0b10110101;
		case 48: return 0b10110110;
		case 49: return 0b10110111;
		case 50: return 0b10111000;
		case 51: return 0b10111001;
		case 52: return 0b10111010;
		case 53: return 0b10111011;
		case 54: return 0b10111100;
		case 55: return 0b10111101;
		case 56: return 'o';
		case 57: return 0b10111110;
		case 58: return 'p';
		case 59: return 'c';
		case 60: return 0b10111111;
		case 61: return 0b01111001;
		case 62: return 0b11100100;
		case 63: return 0b01111000;
		case 64: return 0b11100101;
		case 65: return 0b11000000;
		case 66: return 0b11000001;
		case 67: return 0b11100110;
		case 68: return 0b11000010;
		case 69: return 0b11000011;
		case 70: return 0b11000100;
		case 71: return 0b11000101;
		case 72: return 0b11000110;
		case 73: return 0b11000111;
		case 74: return '!';
		case 75: return '"';
		case 76: return 0b11001100;
		case 77: return ';';
		case 78: return '%';
		case 79: return '?';
		case 80: return '*';
		case 81: return '(';
		case 82: return ')';
		case 83: return '+';
		case 84: return '^';
		case 85: return ',';
		case 86: return '.';
		case 87: return '/';
		case 88: return '\\';
		case 89: return '|';
		case 90: return 0b11101101;
		case 91: return 0b11011001;
		case 92: return 0b11011010;
		case 93: return 0b01011110;
		default: return ' ';
	}
}
unsigned char entNum(char borderDown, char borderUp){	
	if(KeyCode>=30 && KeyCode<=39){
		char KeyValue = KeyCode - 30;
		if(borderDown <= KeyValue && borderUp >= KeyValue){
			KeyCode = 0;
			return (unsigned char)KeyValue;
		}else return 255;
	}else return 255;
}
void NumericToIndicator(unsigned long int n, unsigned char displacement){
	clrInd();
	displacement++;
	do{
		setDigit(0,displacement, n%10);
		n /=10;
		displacement++;
	}while((displacement<10)&(n>0));
}
unsigned char *GetDayOfWeek(unsigned char day){
	static unsigned char array[3] = {0,0,255};
		 if(day==1){array[1] = 57; array[0] = 55;} 
	else if(day==2){array[1] = 43; array[0] = 60;} 
	else if(day==3){array[1] = 59; array[0] = 58;} 
	else if(day==4){array[1] = 65; array[0] = 60;} 
	else if(day==5){array[1] = 57; array[0] = 60;} 
	else if(day==6){array[1] = 59; array[0] = 42;} 
	else if(day==7){array[1] = 43; array[0] = 59;} 
	else{array[1] = 34; array[0] = 34;}
	return array;
}
void TimeToInd(){
	if(LCD_Power_On && KeyCode==37){
		KeyCode = 0;
		lcd_off();
	}

	if(!RelevanceOfNextStartCell){
		NearTimeStart = Clock;
		AdressOfNextStartCell = FindNextTimeStart(&NearTimeStart);
		RelevanceOfNextStartCell = 1;
	}
	char line = 0;
	char symbol = 3;
	unsigned char SignalsFinal = CurrentSignals|SignalsForInd;
	if(ModeOfFirstLine != (SignalsFinal>0)){
		clrInd();
		ModeOfFirstLine = SignalsFinal > 0;
	}
	if(SignalsFinal > 0){
		symbol = 4;
		
		unsigned char D[4] = {34,34,34,255};
		for(unsigned char i = 1; i<4; i++){
			unsigned char SignalOn = SignalsFinal%2;
			SignalsFinal/=2;
			if(SignalOn==1){
				D[2] = D[1];
				D[1] = D[0];
				D[0] = i;
			}
		}
		SendArrayToLCD(D, line, 1);
	}

	setDigit(line, symbol+3, 35);
	setDigit(line, symbol+6, 35);
	long int temp = Clock/100;
	setDigit(line, symbol+1, temp%10);
	temp /= 10;
	setDigit(line, symbol+2, temp%6);
	temp /= 6;
	setDigit(line, symbol+4, temp%10);
	temp /= 10;
	setDigit(line, symbol+5, temp%6);
	temp /= 6;
	unsigned char day = temp/24 + 1;
	temp %= 24;
	setDigit(line, symbol+7, temp%10);
	setDigit(line, symbol+8, temp/10);

	SendArrayToLCD(GetDayOfWeek(day), line, symbol + 10);
	
	unsigned char DataArray[] = {34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,255};
	if(AdressOfNextStartCell!=240){
		
		if(LockSignals){
			DataArray[0]  = 86;
			DataArray[1]  = 53;
			DataArray[2]  = 52;
			DataArray[3]  = 60;
			DataArray[4]  = 56;
			DataArray[5]  = 34;
			DataArray[6]  = 46;
			DataArray[7]  = 50;
			DataArray[8]  = 55;
			DataArray[9]  = 41;
			DataArray[10] = 59;
			DataArray[11] = 50;
			DataArray[12] = 57;
			DataArray[13] = 59;
			DataArray[14] = 41;
			DataArray[15] = 58; 
		 
			SendArrayToLCD(DataArray, 1, 1);
			return;
		}
		DataArray[15] = 90;
		
		char line = 1;
		unsigned int TimeStart;
		unsigned int TimeStop;
		unsigned char Days;
		unsigned char Signals;
		ParseDataRecord(ReadFourBytesEE(AdressOfNextStartCell), &TimeStart, &TimeStop, &Days, &Signals);
		
		unsigned int Time;
		unsigned char Day;
		ParseTime(NearTimeStart, &Time, &Day);
		
		char nSymb = 0;
		DataArray[8-nSymb] = 35;
		DataArray[6-nSymb] = Time%10;
		Time/=10;
		DataArray[7-nSymb] = Time%6;
		Time/=6;
		DataArray[9-nSymb] = Time%10;
		Time/=10;
		DataArray[10-nSymb] = Time%10;
		
		for(unsigned char i = 1; i<4; i++){
			unsigned char SignalOn = Signals%2;
			Signals/=2;
			if(SignalOn==1){
				char j = nSymb/2;
				DataArray[3-j] = DataArray[2-j];
				DataArray[2-j] = DataArray[1-j];
				DataArray[1-j] = i;
			}
		}
		unsigned char *DataArray2 = GetDayOfWeek(Day);
		DataArray[12] = DataArray2[0];
		DataArray[13] = DataArray2[1];
		
		SendArrayToLCD(DataArray, line, 1);
		
	
#line 386 "prog_1.c"
 
	}else{
		SendArrayToLCD(DataArray, 1, 1);
	}
}
void EEWR(unsigned char adress, unsigned char data){
	volatile unsigned char INTCON_BUP = INTCON;
	GIE = 0;
	CLRWDT();
	EEADR = adress;
	EEDATA = data;
	EEPGD = 0;
	WREN = 1;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	WR = 1;
	WREN = 0;
	while(WR==1){}
	EEIF = 0;
	INTCON = INTCON_BUP;
}
unsigned char EERD(unsigned char adress){
	volatile unsigned char INTCON_BUP = INTCON;
	GIE = 0;
	CLRWDT();
	EEADR = adress;
	EEPGD = 0;
	RD = 1;
	volatile unsigned char EEDATA_BUP = EEDATA;
	INTCON = INTCON_BUP;
	return EEDATA_BUP;
}
void ReIndTimeEdit(long int n){
	long int temp = n;
	setDigit(0, 2, temp%10);
	temp /= 10;
	setDigit(0, 3, temp%6);
	temp /= 6;
	temp %= 24;
	setDigit(0, 4, 35);
	setDigit(0, 5, temp%10);
	setDigit(0, 6, temp/10);
	setDigit(0, 8, 1 + n/1440);
	
	
	setDigit(1, 1, cYears%16);
	setDigit(1, 2, cYears/16);
	setDigit(1, 3, 0);
	setDigit(1, 4, 2);
	setDigit(1, 5, 86);
	setDigit(1, 6, cMonths%16);
	setDigit(1, 7, cMonths/16);
	setDigit(1, 8, 86);
	setDigit(1, 9, cDays%16);
	setDigit(1,10, cDays/16);
}
void TimeEdit(){
	isTimeSetting = 1;
	clrInd();
	long int t = Clock/6000;
	setBlink(0,6,1);
	char j;
	unsigned char c = 2;
	unsigned char borders[] = {0,2};
	
	while(1){
		j = entNum(borders[0],borders[1]);
		if(j!=255 && c==1){
			t = t + (j-getDigit(0,8))*1440;
			borders[0]=0;
			borders[1]=2;
			c++;
			setBlink(0,8,0);
			setBlink(0,6,1);
		}else if(j!=255 && c==2){
			t = t + (j-getDigit(0,6))*600;
			if(j>1){
				borders[0]=0;
				borders[1]=3;
				if(getDigit(0,5)>3){
					t = t + (3-getDigit(0,5))*60;
				}
			}else{
				borders[0]=0;
				borders[1]=9;
			}
			c++;
			setBlink(0,6,0);
			setBlink(0,5,1);
		}else if(j!=255 && c==3){
			t = t + (j-getDigit(0,5))*60;
			borders[0]=0;
			borders[1]=5;
			c++;
			setBlink(0,5,0);
			setBlink(0,3,1);
		}else if(j!=255 && c==4){
			t = t + (j-getDigit(0,3))*10;
			borders[0]=0;
			borders[1]=9;
			c++;
			setBlink(0,3,0);
			setBlink(0,2,1);
		}else if(j!=255 & c==5){
			t = t + j-getDigit(0,2);
			borders[0]=0;
			borders[1]=2;
			c++;
			setBlink(0,2,0);
			setBlink(1,10,1);
		}
		
		else if(j!=255 && c==6){
			cDays = (j*16) + getDigit(1,9);
			if(j==0){
				borders[0]=1;
				borders[1]=9;
				if(getDigit(1,9)==0) setDigit(1,9,1);
			}else if(j<3){
				borders[0]=0;
				borders[1]=9;
			}else if(j==3){
				borders[0]=0;
				borders[1]=1;
			}
			c++;
			setBlink(1,10,0);
			setBlink(1,9,1);
		}else if(j!=255 && c==7){
			cDays = (getDigit(1,10)*16) + j;
			borders[0]=0;
			borders[1]=1;
			c++;
			setBlink(1,9,0);
			setBlink(1,7,1);
		}else if(j!=255 && c==8){
			cMonths = (j*16) + getDigit(1,6);
			if(j==0){
				borders[0]=1;
				borders[1]=9;
				if(getDigit(1,7)==0) setDigit(1,6,1);
			}else if(j==1){
				borders[0]=0;
				borders[1]=2;
			}
			c++;
			setBlink(1,7,0);
			setBlink(1,6,1);
		}else if(j!=255 && c==9){
			cMonths = (getDigit(1,7)*16) + j;
			borders[0]=0;
			borders[1]=9;
			c = 2;
			
			
			setBlink(1,6,0);
			setBlink(0,6,1);
	
#line 556 "prog_1.c"
 
		}
		
		if(KeyCode==44){
			KeyCode = 0;
			clrInd();
			if(c==1){
				c = 2;
				borders[0]=0;
				borders[1]=2;
				setBlink(0,8,0);
				setBlink(0,6,1);
			}else if(c>1 && c<6){
				c = 6;
				borders[0]=0;
				borders[1]=3;
				setBlink(1,10,1);
			}else if(c>5){
				c = 1;
				borders[0]=1;
				borders[1]=7;
				setBlink(0,8,1);
			}
		}else if(KeyCode==43){
			KeyCode = 0;
			clrInd();
			main2();
		}else if(KeyCode==42){
			KeyCode = 0;
			WriteTime(getDigit(0,3)*16 + getDigit(0,2), getDigit(0,6)*16 + getDigit(0,5), getDigit(0,8));
			clrInd();
			main2();
		}
		ReIndTimeEdit(t);
	}
}
unsigned long int ReadFourBytesEE(unsigned char adress){
	unsigned long int x;
	for(int i=3;i>=0;i--){
		x = x*256 + EERD(adress+i);
	}
	return x;
}
void WriteFourBytesEE(unsigned char adress, unsigned long int data){
	for(char i=0;i<4;i++){
		EEWR(adress+i,data%256);
		data /= 256;
	}
}
void delay(unsigned char del, unsigned int count){
	while(count > 0){
		TMR2 = 0;
		PR2 = del;
		TMR2IF = 0;
		while(!TMR2IF);
		count--;
	};			
	return;
}
void SendSymbolToLCD(unsigned char Symb){
	unsigned char Smb = 0;
	for(int a = 8; a>0; a--){
		Smb*=2;
		if(Symb%2==1){
			Smb++;
		}
		Symb/=2;
		CLRWDT();
	}
	
	RC1  = 1;
	PORTA = (0b11110000 & (Smb * 16)) / 4 | (LCD_Light_On == 1 ? 0b00000010 : 0b00000000);
	RC0  = 1;
	CLRWDT();
	delay(10, 1);
	RC0  = 0;
	RC1  = 0;
	CLRWDT();
	delay(25, 5);

	RC1  = 1;	
	PORTA = (0b11110000 & Smb) / 4 | (LCD_Light_On == 1 ? 0b00000010 : 0b00000000);
	RC0  = 1;
	CLRWDT();
	delay(10, 1);
	RC0  = 0;
	RC1  = 0;
	CLRWDT();
	delay(25, 5);
}
void SendStringToLCD(unsigned char Symb[]){	

#line 651 "prog_1.c"
 
}
void SendArrayToLCD(unsigned char Symb[], char line, char position){	
	unsigned int i = 0;
	while(Symb[i]!=255 && position<17){
		setDigit(line, position, Symb[i]);
		i++;
		position++;
	}
}
void lcd_send_initial_half_byte(unsigned char data){
	RC1  = 0;
	unsigned char data_temp = 0;
	CLRWDT();
	for(int a = 4; a>0; a--){
		data_temp*=2;
		if(data%2==1){
			data_temp++;
		}
		data/=2;
		CLRWDT();
	}
	PORTA = (0b00111100 & (data_temp * 4)) | (LCD_Light_On == 1 ? 0b00000010 : 0b00000000);
	RC0  = 1;
	RC0  = 0;
}
void lcd_send_byte(unsigned char data){
	lcd_send_initial_half_byte((data & 0b11110000) / 16);
	__delay_us(40);
	lcd_send_initial_half_byte(data & 0b00001111);
	__delay_us(40);
}
void lcd_off(){
	LCD_Light_On = 0;	
	lcd_send_byte(0b00001000);
	LCD_Power_On = 0;
	RC2  = 0;
}
void lcd_on(){
	lcd_init();
	lcd_send_byte(0b00001100);
	LCD_Power_On = 1;
	RC2  = 1;

}
void lcd_init(){
	unsigned char INTCON_BUF = INTCON;
	GIE = 0;
	CLRWDT();
	RC0   = 0;
	RC1  = 0;	
	
	__delay_ms(20);
	lcd_send_initial_half_byte(0b00000011);
	__delay_ms(5);
	lcd_send_initial_half_byte(0b00000011);
	__delay_us(110);
	lcd_send_initial_half_byte(0b00000011);
	__delay_us(40);
	lcd_send_initial_half_byte(0b00000010);
	__delay_us(40);
	lcd_send_byte(0b00101100);
	__delay_us(40);
	lcd_send_byte(0b00001000);
	__delay_us(40);
	lcd_send_byte(0b00000001);
	__delay_us(40);
	lcd_send_byte(0b00010010);
	__delay_us(40);
	
	
	RC0   = 0;
	RC1  = 0;
	
	lcd_send_byte(0b00000001);
	__delay_ms(2);
	
	INTCON = (INTCON_BUF | 0b10000000) | (INTCON & 0b01111111);
}
unsigned char TestKeyGroup(){
	if(RB6==0){
		return 1;
	}else if(RB4==0){
		return 2;
	}else if(RB2==0){
		return 3;
	}else if(RB0==0){
		return 4;
	}else{
		return 17;
	}
}
unsigned char ConvKeyNum(unsigned char Num){
	switch (Num){
		case  0: return 33;
		case  1: return 43;
		case  2: return 42;
		case  3: return 41;
		case  4: return 40;
		case  5: return 45;
		case  6: return 39;
		case  7: return 36;
		case  8: return 37;
		case  9: return 38;
		case 10: return 35;
		case 11: return 34;
		case 12: return 32;
		case 13: return 30;
		case 14: return 44;
		case 15: return 31;
		default: return 0;
	}
}
unsigned char FindCell(unsigned char adressStart, char New, unsigned char previous){
	if(adressStart==240)adressStart=236;
	unsigned char adress = adressStart;
	unsigned char adressNew = 240;
	do{
		if(!previous){
			adress==236 ? adress=0 : adress+=4;
		}else{
			adress==0 ? adress=236 : adress-=4;
		}
		unsigned char D = EERD(adress+2);
		if(New!=1 ? D!=0xFF : D==0xFF){
			adressNew = adress;
			break;
		}
	}while(adress!=adressStart);
	return adressNew;
}
unsigned char RefreshSchedulerIndicator(unsigned char adress, char New, unsigned char CopyFrom){
	if(adress!=240 && (New!=1 ? EERD(adress+2)!=0xFF : EERD(adress+2)==0xFF)){
		NumericToIndicator(adress/4+1,1);
	}else{
		adress = FindCell(adress, New, 0);
		if(adress!=240){
			NumericToIndicator(adress/4+1,1);
		}else{
			setDigit(0,3,32);
			setDigit(0,2,32);
		}
	}
	if(New==1){
	
#line 815 "prog_1.c"
 
		setDigit(0,12,21);
		setDigit(0,11,14);
		setDigit(0,10,36);
	}
	setDigit(0,8,12);
	setDigit(0,7,14);
	setDigit(0,6,20);
	setDigit(0,5,20);
	return adress;
}
void TimesToIndicator(unsigned int Times[]){
	unsigned int temp;
	for(char j=0;j<7;j+=6){	
		temp = Times[j/5];
		setDigit(0,1+j,temp%10);
		temp/=10;
		setDigit(0,2+j,temp%6);
		setDigit(0,3+j,35);
		temp=temp/6;
		setDigit(0,4+j,temp%10);
		setDigit(0,5+j,temp/10);
	}
	setDigit(0,6,32);
}
void ItemsToIndicator(unsigned char items, unsigned char max_i, unsigned char firstDigit){
	for(char i=1;i<max_i;i++){
		items%2==1 ? setDigit(0,firstDigit-i,i) : setDigit(0,firstDigit-i,32);
		items/=2;
	}


}
unsigned char getBorderUp(char n){
	unsigned char BorderUp;
	switch(n){
		case 11: case 5:	BorderUp = 2; break;
		case 10: case 4:	getDigit(0,n+1)==2 ? BorderUp = 3 : BorderUp = 9; break;
		case 8:  case 2:	BorderUp = 5; break;
		case 7:  case 1:	BorderUp = 9; break;
	}
	return BorderUp;
}
unsigned char ConvertDayToBit(unsigned char DayNumber){
	switch (DayNumber){
		case 1: return 0b00000001;
		case 2: return 0b00000010;
		case 3: return 0b00000100;
		case 4: return 0b00001000;
		case 5: return 0b00010000;
		case 6: return 0b00100000;
		case 7: return 0b01000000;
		default: return 0;
	}
}
unsigned char EditSchedule(unsigned char adress, unsigned char SourceOfRecord){
	unsigned char TargetAdress;
	if(SourceOfRecord==240){
		TargetAdress = adress;
	}else{
		TargetAdress = SourceOfRecord;
	}
	unsigned long int Data = ReadFourBytesEE(TargetAdress);
	
	char BeginEditTimes = 0;
	if(Data == 0xFFFFFFFF){
		Data = 0;
		BeginEditTimes = 1;
	}

	unsigned int TimeStart;
	unsigned int TimeStop;
	unsigned char Days;
	unsigned char Signals;
	ParseDataRecord(Data, &TimeStart, &TimeStop, &Days, &Signals);
	
	unsigned int Times[2];
	Times[1] = TimeStart;
	Times[0] = TimeStop;
	
	clrInd();
	char mode = 1;
	unsigned char key;
	while(1){
		if(KeyCode==43){ 
			KeyCode = 0;
			clrInd();
			return 0;
		}else if(KeyCode==42){ 
			KeyCode = 0;
			Data = (unsigned long int)Signals*0x20000000+(unsigned long int)Days*0x400000+(unsigned long int)Times[0]*0x800+(unsigned long int)Times[1];
			WriteFourBytesEE(adress, Data);
			clrInd();
			return 1;
		}
		switch(mode){
			case 1:{
				TimesToIndicator(Times);
				if(KeyCode==45 || BeginEditTimes==1){
					KeyCode = 0;
					BeginEditTimes = 0;
					setBlink(0,11,1);
					char n=11;
					unsigned char BorderUp = 2;
					while(1){
						if(KeyCode==43){ 
							KeyCode = 0;
							clrInd();
							break;
						}else if(KeyCode==42){ 
							KeyCode = 0;
							unsigned int T[2];
							for(char j=0;j<7;j+=6){
								T[j/6] = ((unsigned int)getDigit(0,j+5))*600+getDigit(0,j+4)*60+getDigit(0,j+2)*10+getDigit(0,j+1);
							}
							if(T[0]>T[1]){
								Times[0] = T[0];
								Times[1] = T[1];
								clrInd();
								break;
							}else{
								setBlink(0,n,0);
								n=5;
								BorderUp = getBorderUp(n);
								setBlink(0,n,1);
								setDigit(0,5,getDigit(0,11));
								setDigit(0,4,getDigit(0,10));
								setDigit(0,2,getDigit(0,8));
								setDigit(0,1,getDigit(0,7));
							}
						}else if(KeyCode==40){ 
							KeyCode = 0;
							if(n>1){
								setBlink(0,n,0);
								n==10 || n==7 || n==4 ? n-=2 : n--;
								BorderUp = getBorderUp(n);
								setBlink(0,n,1);
							}
						}else if(KeyCode==41){ 
							KeyCode = 0;
							if(n<11){
								setBlink(0,n,0);
								n==8 || n==5 || n==2 ? n+=2 : n++;
								BorderUp = getBorderUp(n);
								setBlink(0,n,1);
							}
						}
						key = entNum(0,BorderUp);
						if(key!=255){
							setDigit(0,n,key);
							if((n==11||n==5)&&key==2&&(getDigit(0,n-1)>3))setDigit(0,n-1,3);
							if(n>1){
								KeyCode=40;
							}else{
								KeyCode=42;
							}
						}
					}
				}else if(KeyCode==40){
					KeyCode = 0;
					clrInd();
					mode = 2;
				}else if(KeyCode==41){
					KeyCode = 0;
					clrInd();
					mode = 3;
				}
			} break;
			case 2:{
				ItemsToIndicator(Days, 8, 9);
				key = entNum(1,7);
				if(key!=255){
					Days^=ConvertDayToBit(key);
				}else if(KeyCode==30){
					KeyCode = 0;
					Days=0;
				}else if(KeyCode==45){
					KeyCode = 0;
					Days=0b01111111;
				}else if(KeyCode==40){
					KeyCode = 0;
					clrInd();
					mode = 3;
				}else if(KeyCode==41){
					KeyCode = 0;
					clrInd();
					mode = 1;
				}
			} break;
			case 3:{
				ItemsToIndicator(Signals, 4, 8);
				key = entNum(1,3);
				if(key!=255){
					Signals^=ConvertDayToBit(key);
				}else if(KeyCode==30){
					KeyCode = 0;
					Signals=0;
				}else if(KeyCode==45){
					KeyCode = 0;
					Signals=0b00000111;
				}else if(KeyCode==40){
					KeyCode = 0;
					clrInd();
					mode = 1;
				}else if(KeyCode==41){
					KeyCode = 0;
					clrInd();
					mode = 2;
				}
			}
		}
	}
	return 0;
}
void Scheduler(unsigned char StartFrom, unsigned char new){
	clrInd();
	volatile static unsigned char NumberOfCells = 0;



	unsigned char isDataModified = 1;
	while(1){
		if(new==0 && NumberOfCells!=240 && KeyCode==44){ 
			KeyCode = 0;
			WriteFourBytesEE(NumberOfCells,0xFFFFFFFF);
			isDataModified = 1;
		}else if(new==0 && KeyCode==35){ 
			KeyCode = 0;
			Scheduler(240, 1);
			isDataModified = 1;
	
#line 1048 "prog_1.c"
 
		}else if(NumberOfCells!=240 && KeyCode==45){ 
			KeyCode = 0;
			EditSchedule(NumberOfCells, StartFrom);
			isDataModified = 1;
		}else if(KeyCode==40){ 
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, new, 0);
			isDataModified = 1;
		}else if(KeyCode==41){ 
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, new, 1);
			isDataModified = 1;
		}else if(KeyCode==30){ 
			KeyCode = 0;
			NumberOfCells = 0;
			isDataModified = 1;
		}
		if(isDataModified == 1){
			NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, new, StartFrom);
			isDataModified = 0;
		}
		
		if(KeyCode==43){ 
			KeyCode = 0;
			clrInd();
			if(new==1){
				Scheduler(240, 0);
			}else{
				main2();
			}
		}
	}
}
void ParseTime(long int TimeValue, unsigned int *Time, unsigned char *Day){
	unsigned int DayTime = (unsigned int)(TimeValue/6000);
	*Time = DayTime%1440;
	*Day  = DayTime/1440 + 1;
}
void ParseDataRecord(unsigned long int Data, unsigned int *TimeStart, unsigned int *TimeStop, unsigned char *Days, unsigned char *Signals){

		*TimeStart = (unsigned int)(Data%(unsigned long int)2048);
		Data/=2048;
		*TimeStop = (unsigned int)(Data%(unsigned long int)2048);
		Data/=2048;
		*Days = (unsigned char)(Data%(unsigned long int)128);
		Data/=128;
		*Signals = (unsigned char)(Data%(unsigned long int)8);

}
unsigned char FindNextTimeStart(unsigned long int *TimeFrom){
	
	unsigned char adress = 0;
	unsigned char adressStart = 240;
	unsigned char TimeIsFound = 0;
	unsigned long int LastFoundTimeStart;
		
	do{
		unsigned long int Data = ReadFourBytesEE(adress);
		
		adress+=4;
		if(Data==0xFFFFFFFF){
			continue;
		}
		unsigned int TimeStart;
		unsigned int TimeStop;
		unsigned char Days;
		unsigned char Signals;
		ParseDataRecord(Data, &TimeStart, &TimeStop, &Days, &Signals);
		
		if(Signals==0){
			continue;
		}
		for(char DayIndex = 0; DayIndex<7; DayIndex++){
			unsigned char DayOn = Days%2;
			Days /=2;
			if(DayOn!=1){
				continue;
			}
			unsigned long int TimeOfSignals = ((unsigned long int)DayIndex * 86400 + (unsigned long int)TimeStart * 60) * 100;
			
			if((TimeOfSignals < LastFoundTimeStart || TimeIsFound==0) && TimeOfSignals > *TimeFrom){
				TimeIsFound = 1;
				LastFoundTimeStart = TimeOfSignals;
				adressStart = adress-4;
			}
		}
	}while(adress<240);
	
	if(TimeIsFound==1){
		*TimeFrom = LastFoundTimeStart;
	}else{
		*TimeFrom = 0;
	}
	return adressStart;
}
void SignalsOnOff(){
	if(!TimeIsRead) return;
	if(!LockSignals && CurrentReadingCell<240){
		unsigned long int Data = ReadFourBytesEE(CurrentReadingCell);		
		if(Data!=0xFFFFFFFF){
			unsigned int TimeStart;
			unsigned int TimeStop;
			unsigned char Days;
			unsigned char Signals;
			TimeStart = Data%2048;
			Data/=2048;
			TimeStop = Data%2048;
			Data/=2048;
			Days = Data%128;
			Data/=128;
			Signals = Data%8;
			if(Signals!=0 && Days!=0){
				unsigned int ThisTime;
				unsigned char ThisDay;
				unsigned int DayTime = (unsigned int)(Clock/6000);
				ThisTime = DayTime%1440;
				ThisDay  = DayTime/1440 + 1;
				ThisDay = ConvertDayToBit(ThisDay);
				if(((ThisDay&Days) > 0) && (TimeStart<=ThisTime) && (TimeStop>ThisTime)){
					SignalsOut = Signals|SignalsOut;
				}
			}
		}
		CurrentReadingCell+=4;
	}else{
		SignalsForInd = (LockSignals ? 0 : SignalsOut);
		SignalsOut = 0;
		CurrentReadingCell = 0;
		unsigned char Data = (PORTB & 0b00011111) | ((CurrentSignals|SignalsForInd)*32);
		if((Data&0b11100000)!=CurrentSignalsData){
			EEWR(cMinutesAdress, cMinutes);
			EEWR(cHoursAdress  , cHours);
			EEWR(cWeekDayAdress, cWeekDay);
			EEWR(cDaysAdress   , cDays);
			EEWR(cMonthsAdress , cMonths);
		}
		CurrentSignalsData = Data&0b11100000;
		PORTB = Data;
	}
}
void FillMinutes(){
	Minutes = Clock - 3000;
}
void interrupt F(){
	CLRWDT();
	if(TMR1IF){
		TMR1IF = 0;
		TMR1 += 51200;
		SignalsOnOff();
		char MinuteAgo = (Clock - Minutes > 6000);
		if(MinuteAgo > 0){
			FillMinutes();
			RelevanceOfNextStartCell = 0;
		}
		
		if(LCD_Power_On && (GlobalBlinkCycleTime==100 || IsLCDModified)){
			for(char line = 0; line<2; line++){	
				for(int symbol = 0; symbol<16; symbol++){
					if(GlobalBlinkCycleTime==100 || modifide_digits[line][symbol]>0){
						unsigned char digit = digits[line][symbol];
						lcd_send_byte((line * 0x40 + 15 - symbol) | 0b10000000); 
						unsigned char DigitValue;
						if(((digit & 0b10000000) == 0) || (((digit & 0b10000000) != 0) && GlobalBlink)){
							DigitValue = digit & 0b01111111;
						}else{
							DigitValue = 34;
						}
						SendSymbolToLCD(convDig_LCD(DigitValue));
						modifide_digits[line][symbol] = 0;
					}
				}
			}
			IsLCDModified = 0;
			
		}
		
		if(GlobalBlinkCycleTime>0){
			GlobalBlinkCycleTime--;
		}else{
			IsLCDModified = 1;
			GlobalBlinkCycleTime = 100;
			GlobalBlink = !GlobalBlink;
			
			ReadTime();
		}
		
		static unsigned char ButtonPressTimeOut;
		
		static unsigned char KeyIndex;
		static unsigned char PressedKeyIndex = 0xFF;
		
		if(ButtonPressTimeOut>0){
			ButtonPressTimeOut--;
		}else{
			KeyCode = 0;
			if(RB0){
				if(KeyIndex==0x0F){
					KeyIndex = 0;
				}else{
					KeyIndex++;	
				}
				PORTB = (PORTB & 0b11100000) | (KeyIndex*2);
				PressedKeyIndex = 0xFF;
				if(LCD_ON_TIMEOUT>0){
					LCD_ON_TIMEOUT--;
				}
			}else{
				if(PressedKeyIndex==0xFF){
					PressedKeyIndex = KeyIndex;
					ButtonPressTimeOut = 30;
					LCD_ON_TIMEOUT = 60000; 
					KeyCode = ConvKeyNum(PressedKeyIndex);
				}
			}
		}
	}
}
void main(){
	TimeIsRead = 0;
	INTCON = 0;
	clrInd();
	OPTION_REG = 0b00001011;
	ADCON1 = 0b00000111;
	TRISA = 0b00000000;
	TRISB = 0b00000001;
	TRISC = 0b00000000;

	Clock = 0;
	LockSignals      = EERD(251);
	TMR1 = 51200;
	TMR1ON = 1;
	TMR1IE = 1;
	INTCON = 0b11000000;
	PORTA = 0b00000000;
	PORTB = 0b00000000;
	PORTC = 0b00000000;
	T2CON = 0b00100100;
	lcd_on();
	main2();
}
void main2(){
	
	isTimeSetting = 0;
	
	NearTimeStart = Clock;
	AdressOfNextStartCell = FindNextTimeStart(&NearTimeStart);
	
	while(1){
		if(LCD_Power_On==1 && LCD_ON_TIMEOUT==0 && CurrentSignals==0){
			lcd_off();
		}else if(LCD_Power_On==0 && KeyCode!=0){
			KeyCode=0;
			lcd_on();
		}
			
		if(LCD_Power_On==0) continue;
		
		TimeToInd();
		if(KeyCode==45){
			KeyCode = 0;
			TimeEdit();
		}else if(KeyCode==40 || KeyCode==41){
			KeyCode = 0;
			Scheduler(240, 0);
		}else if(KeyCode==44){
			KeyCode = 0;
			if(LCD_Power_On){
				LCD_Light_On = !LCD_Light_On;
			
			}
		}else if(KeyCode==30){
			KeyCode = 0;
			LockSignals = 1;
			CurrentSignals = 0;
			EEWR(251, LockSignals);
		}else if(KeyCode==31){
			KeyCode = 0;
			clrInd();
			CurrentSignals = CurrentSignals^0b00000001;
		}else if(KeyCode==32){
			KeyCode = 0;
			clrInd();
			CurrentSignals = CurrentSignals^0b00000010;
		}else if(KeyCode==33){
			KeyCode = 0;
			clrInd();
			CurrentSignals = CurrentSignals^0b00000100;
		}else if(KeyCode==34){
			KeyCode = 0;
			clrInd();
			CurrentSignals = 0;
		}else if(KeyCode==35){
			KeyCode = 0;
			LockSignals = 0;
			EEWR(251, LockSignals);
			clrInd();
		}else if(KeyCode==43 || KeyCode==36){
			if(KeyCode==43){
				NearTimeStart = Clock;
			}
			KeyCode = 0;
			FillMinutes();
			AdressOfNextStartCell = FindNextTimeStart(&NearTimeStart);
		}
	}
}
unsigned char BCD_to_DEC(unsigned char BCD){
	return (BCD & 0b11110000) / 16 * 10 + (BCD & 0b00001111);
}
unsigned char DEC_to_BCD(unsigned char DEC){
	if(DEC > 99){
		DEC = 99;
	}
	return DEC / 10 * 16 + DEC % 10;
}
void ReadTime(){
			
	unsigned char Seconds	= 0x00;
	
	I2CInit();
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x00);
	I2CRestart();
	I2CSend(0xD1);
	
	unsigned char I = I2CRead();
	
	if((I & 0b10000000) == 0){ 
		TimeIsRead = 1;
		I2CAck();
		Seconds		= I;
		cMinutes	= I2CRead(); I2CAck();
		cHours		= I2CRead(); I2CAck();
		cWeekDay	= I2CRead();
		if(!isTimeSetting){ 
			I2CAck();
			cDays		= I2CRead(); I2CAck();
			cMonths		= I2CRead();
		
		}
	}else{
		TimeIsRead = 0;
		I2CNak();
		I2CStop();
		cDays		= EERD(cDaysAdress);
		cMonths		= EERD(cMonthsAdress);
		WriteTime(
			EERD(cMinutesAdress),
			EERD(cHoursAdress),
			EERD(cWeekDayAdress)
		);
		return;
	}
	I2CNak();
	
	I2CStop();
	
	Clock = ( (long int)BCD_to_DEC(Seconds)
			+ (long int)BCD_to_DEC(cMinutes)     * 60
			+ (long int)BCD_to_DEC(cHours)       * 3600
			+           ((long int)cWeekDay - 1) * 86400) * 100;
	
}
void WriteTime(unsigned char Minutes, unsigned char Hours, unsigned char DayOfWeek){
		
	volatile unsigned char INTCON_BUP = INTCON;
	GIE = 0;
	CLRWDT();
	
	I2CInit();
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x00);
 
	I2CSend(0x00);		
	I2CSend(Minutes);	
	I2CSend(Hours);		
	I2CSend(DayOfWeek);	
	I2CSend(cDays); 	
	I2CSend(cMonths); 	
	I2CSend(cYears); 	
	I2CSend(0b00010000);
	
	I2CStop();
	
	Clock = (  (long int)Minutes * 60
			+  (long int)Hours   * 3600
			+ ((long int)DayOfWeek - 1)  * 86400) * 100;
	
	INTCON = INTCON_BUP;
}
void I2CInit(void){
	TRISC3 = 1;       
	TRISC4 = 1;       
	SSPSTAT |= 0x80;  
	SSPCON = 0x28;    
	SSPADD = 0x27;    
}
void I2CStart(){
	SEN = 1;          
	while(SEN);       
                      
}
void I2CStop(){
	PEN = 1;          
	while(PEN);       
                      
}
void I2CRestart(){
	RSEN = 1;         
	while(RSEN);      
}
void I2CAck(){
	ACKDT = 0;        
	ACKEN = 1;        
	while(ACKEN);     
}
void I2CNak(){
	ACKDT = 1;        
	ACKEN = 1;        
	while(ACKEN);     
}
void I2CSend(unsigned char dat){
	SSPBUF = dat;     
	while(BF);        
	while ((SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100));
}
unsigned char I2CRead(void){
	unsigned char temp;
 
	RCEN = 1;         
	while(!BF);       
	temp = SSPBUF;    
	while ((SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100));
	return temp;      
}