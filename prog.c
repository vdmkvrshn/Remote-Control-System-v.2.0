
#include <xc.h>
#include <stdio.h>
#include <pic18f2620.h>


// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON        // Internal/External Oscillator Switchover bit (Oscillator Switchover mode enabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = ON     // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for low-power operation)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

volatile unsigned char digits[2][16];

typedef struct {
	unsigned blink: 1;
	unsigned modifided: 1;
} digits_atributes;

volatile digits_atributes digitsAtrib[2][16];
long int Clock;
long int Minutes = 0;
unsigned int LCD_ON_TIMEOUT = 60000; // 3 min
volatile unsigned char KeyCode;
unsigned int CorrectTimeConst;
unsigned int AdressOfNextStartCell = 240;
unsigned long int NearTimeStart;
unsigned long int NearTimeStop;
unsigned int CurrentReadingCell = 0;
unsigned char CurrentSignalsData = 0;
volatile unsigned char SignalsOut = 0;
volatile unsigned char SignalsForInd = 0;

struct {
	unsigned DetailModeOfViewSheduler: 1;
	unsigned RelevanceOfNextStartCell: 1;
	unsigned ModeOfFirstLine: 1;
	unsigned TimeIsRead: 1;
	unsigned isTimeSetting: 1;
	unsigned LCD_Light_On: 1;
	unsigned LCD_Power_On: 1;
	unsigned LockSignals: 1;
	unsigned IsLCDModified: 1;
	unsigned GlobalBlink: 1;
} flags;


unsigned char cMinutes = 0;
unsigned char cHours = 0;
unsigned char cWeekDay = 1;
unsigned char cDays = 1;
unsigned char cMonths = 1;
unsigned char cYears = 1;

unsigned char const cMinutesAdress = 240;
unsigned char const cHoursAdress = 241;
unsigned char const cWeekDayAdress = 242;
unsigned char const cDaysAdress = 243;
unsigned char const cMonthsAdress = 244;
unsigned char const cYearsAdress = 245;

unsigned char CurrentSignals = 0;
unsigned char GlobalBlinkCycleTime = 70;

typedef struct {
	unsigned blink: 1;
//	unsigned modifided: 1;
	unsigned const_symbol: 1;
} symbol_properties;

typedef struct {
    symbol_properties props;
    unsigned char number;
    unsigned char symbol;
    unsigned char index;
} SymbolData;

typedef struct {
    SymbolData Time [5];
    SymbolData WeekDay [1];
    SymbolData Date [10];
} TimeEditData;

typedef struct {
	unsigned char fio[16];
	unsigned char num [12];
} tel;

//	1 - 9	абвг	дежз
//			abc		def
//
//	ийкл4	мноп	рсту
//	ghi		jkl		mno
//	
//	фчцч	шщъы	ьэюя
//	pqrs	tuv		wxyz


#define _XTAL_FREQ 39400000

#define E	RC0
#define RS	RC2
//char LCD_Power_On_PIN = 0;
//#define LCD_Power_On_PIN	RC2
#define EMPTY_SYMBOL_VALUE ' '
#define UNDERSCORE_SYMBOL_VALUE '_' //_
#define EMPTY_STRING_16 "                "
#define BELL_SYMBOL 'Ґ'

void main2(void);
void lcd_init(void);
void lcd_on(void);
void lcd_off(void);
void SendArrayToLCD(unsigned char A[], char B, char C);
void ParseTime(long int TimeValue, unsigned int *Time, unsigned char *Day);
void ParseDataRecord(unsigned long int Data, unsigned int *TimeStart, unsigned int *TimeStop, unsigned char *Days, unsigned char *Signals);
void EEWR(unsigned int adress, unsigned char data);
void Scheduler(unsigned int StartFrom);
void SchedulerNew(unsigned int StartFrom);
void SignalsOnOff(void);
void FillMinutes(void);
unsigned char EERD(unsigned int adress);
unsigned long int ReadFourBytesEE(unsigned int adress);
unsigned char *GetDayOfWeek(unsigned char day);
unsigned char EditSchedule(unsigned int adress, unsigned int SourceOfRecord);
unsigned int FindNextTimeStart(unsigned long int *TimeFrom);
unsigned char getNumChar(unsigned char num);
void I2CInit(void);
void I2CStart(void);
void I2CStop(void);
void I2CRestart(void);
void I2CAck(void);
void I2CNak(void);
void I2CWait(void);
void I2CSend(unsigned char dat);
void outputString(unsigned char * stringData, unsigned char line, unsigned char position);
unsigned char I2CRead(void);
void ReadTime();
void WriteTime(unsigned long int lClock, unsigned char days, unsigned char months, unsigned char years);
unsigned char BCD_to_DEC(unsigned char BCD);
unsigned char DEC_to_BCD(unsigned char DEC);



unsigned char getDigit(char line, char symbol) {
	return digits[line][symbol];
}

void setDigit(char line, char symbol, unsigned char data) {
	unsigned char value = data;
	unsigned char value0 = digits[line][symbol];
	digitsAtrib[line][symbol].modifided = digitsAtrib[line][symbol].modifided > 0 || (value != value0);
	flags.IsLCDModified = flags.IsLCDModified || (value != value0);
	digits[line][symbol] = value;
}

void setBlink(char line, char symbol, unsigned char value) {
	if (value != 0) {
		digitsAtrib[line][symbol].blink = 1;
	} else {
		digitsAtrib[line][symbol].blink = 0;
	}
	digitsAtrib[line][symbol].modifided = 1;
	flags.IsLCDModified = 1;
}

void clrInd() {
	for (char line = 0; line < 2; line++) {
		for (char symb = 0; symb < 16; symb++) {
			digits[line][symb] = ' ';
			digitsAtrib[line][symb].blink = 0;
			digitsAtrib[line][symb].modifided = 1;
		}
	}
	flags.IsLCDModified = 1;
}

unsigned char getLcdCodeOfChar(unsigned char dig) {
	switch (dig) {
	case ' ': return 0x20;
	case '0': return 0x30;
	case '1': return 0x31;
	case '2': return 0x32;
	case '3': return 0x33;
	case '4': return 0x34;
	case '5': return 0x35;
	case '6': return 0x36;
	case '7': return 0x37;
	case '8': return 0x38;
	case '9': return 0x39;
	case '-': return 0x2D;
	case '—': return 0x2D;
	case '!': return 0x21;
	case '"': return 0x22;
	case '#': return 0x23;
	case '$': return 0x24;
	case '%': return 0x25;
	case '&': return 0x26;
	case '(': return 0x28;
	case ')': return 0x29;
	case '*': return 0x2A;
	case ',': return 0x2C;
	case '.': return 0x2E;
	case '/': return 0x2F;
	case ':': return 0x3A;
	case ';': return 0x3B;
	case '?': return 0x3F;
	case '@': return 0x40;
	case '[': return 0x5B;
	case ']': return 0x5D;
	case '^': return 0x5E;
	case '_': return 0x5F;
	case '`': return 0x60;
	case '’': return 0x27;
	case '+': return 0x2B;
	case '<': return 0x3C;
	case '=': return 0x3D;
	case '>': return 0x3E;
	case '°': return 0xEF;
	case '«': return 0xC8;
	case '»': return 0xC9;
	case '\\':return 0x2F;
	case '~': return 0xE9;
	case 'A': return 0x41;
	case 'a': return 0x61;
	case 'B': return 0x42;
	case 'b': return 0x62;
	case 'C': return 0x43;
	case 'c': return 0x63;
	case 'D': return 0x44;
	case 'd': return 0x64;
	case 'E': return 0x45;
	case 'e': return 0x65;
	case 'F': return 0x46;
	case 'f': return 0x66;
	case 'G': return 0x47;
	case 'g': return 0x67;
	case 'H': return 0x48;
	case 'h': return 0x68;
	case 'I': return 0x49;
	case 'i': return 0x69;
	case 'J': return 0x4A;
	case 'j': return 0x6A;
	case 'K': return 0x4B;
	case 'k': return 0x6B;
	case 'L': return 0x4C;
	case 'l': return 0x6C;
	case 'M': return 0x4D;
	case 'm': return 0x6D;
	case 'N': return 0x4E;
	case 'n': return 0x6E;
	case '№': return 0xCC;
	case 'O': return 0x4F;
	case 'o': return 0x6F;
	case 'P': return 0x50;
	case 'p': return 0x70;
	case 'Q': return 0x51;
	case 'q': return 0x71;
	case 'R': return 0x52;
	case 'r': return 0x72;
	case 'S': return 0x53;
	case 's': return 0x73;
	case 'T': return 0x54;
	case 't': return 0x74;
	case 'U': return 0x55;
	case 'u': return 0x75;
	case 'V': return 0x56;
	case 'v': return 0x76;
	case 'W': return 0x57;
	case 'w': return 0x77;
	case 'X': return 0x58;
	case 'x': return 0x78;
	case 'Y': return 0x59;
	case 'y': return 0x79;
	case 'Z': return 0x5A;
	case 'z': return 0x7A;
	case 'А': return 0x41;
	case 'а': return 0x61;
	case 'Б': return 0xA0;
	case 'б': return 0xB2;
	case 'В': return 0x42;
	case 'в': return 0xB3;
	case 'Г': return 0xA1;
	case 'г': return 0xB4;
	case 'Д': return 0xE0;
	case 'д': return 0xE3;
	case 'Е': return 0x45;
	case 'е': return 0x65;
	case 'Ё': return 0xA2;
	case 'ё': return 0xB5;
	case 'Ж': return 0xA3;
	case 'ж': return 0xB6;
	case 'З': return 0xA4;
	case 'з': return 0xB7;
	case 'И': return 0xA5;
	case 'и': return 0xB8;
	case 'Й': return 0xA6;
	case 'й': return 0xB9;
	case 'К': return 0x4B;
	case 'к': return 0xBA;
	case 'Л': return 0xA7;
	case 'л': return 0xBB;
	case 'м': return 0xBC;
	case 'М': return 0x4D;
	case 'Н': return 0x48;
	case 'н': return 0xBD;
	case 'О': return 0x4F;
	case 'о': return 0x6F;
	case 'П': return 0xA8;
	case 'п': return 0xBE;
	case 'Р': return 0x50;
	case 'р': return 0x70;
	case 'С': return 0x43;
	case 'с': return 0x63;
	case 'т': return 0xBF;
	case 'Т': return 0x54;
	case 'У': return 0xA9;
	case 'у': return 0x79;
	case 'Ф': return 0xAA;
	case 'ф': return 0xE4;
	case 'Х': return 0x58;
	case 'х': return 0x78;
	case 'Ц': return 0xE1;
	case 'ц': return 0xE5;
	case 'Ч': return 0xAB;
	case 'ч': return 0xC0;
	case 'Ш': return 0xAC;
	case 'ш': return 0xC1;
	case 'Щ': return 0xE2;
	case 'щ': return 0xE6;
	case 'Ъ': return 0xAD;
	case 'ъ': return 0xC2;
	case 'Ы': return 0xAE;
	case 'ы': return 0xC3;
	case 'Ь': return 0xC4;
	case 'ь': return 0xC4;
	case 'Э': return 0xAF;
	case 'э': return 0xC5;
	case 'Ю': return 0xB0;
	case 'ю': return 0xC6;
	case 'Я': return 0xB1;
	case 'я': return 0xC7;
    case 'Ґ': return 0xED; //bell

	default: return 0x3F; // '?'
	}
}

unsigned char entNum(char borderDown, char borderUp) { //0-9		
	if (KeyCode >= 30 && KeyCode <= 39) {
		char KeyValue = KeyCode - 30;
		if (borderDown <= KeyValue && borderUp >= KeyValue) {
			KeyCode = 0;
			return (unsigned char) KeyValue;
		} else return 255;
	} else return 255;
}

void NumericToString(unsigned long int n, unsigned char * mySring, unsigned char size) {
	unsigned char displacement = 1;
	do {
		mySring[size - displacement] = getNumChar(n % 10);
		n /= 10;
		displacement++;
	} while ((size - (displacement - 1) > 0) && (n > 0));
}

unsigned char *GetDayOfWeek(unsigned char day) {
	if (day == 1) {
        return "пн";
	}
	else if (day == 2) {
        return "вт";
	}
	else if (day == 3) {
        return "ср";
	}
	else if (day == 4) {
        return "чт";
	}
	else if (day == 5) {
        return "пт";
	}
	else if (day == 6) {
        return "сб";
	}
	else if (day == 7) {
        return "вс";
	}
	return "??";
}

void TimeToInd() {
	if (KeyCode == 38 && (AdressOfNextStartCell != 240 || flags.DetailModeOfViewSheduler)) {
		KeyCode = 0;
		clrInd();
		flags.DetailModeOfViewSheduler = !flags.DetailModeOfViewSheduler;
	}
	if (KeyCode == 45 && AdressOfNextStartCell != 240 && flags.DetailModeOfViewSheduler) {
		KeyCode = 0;
		Scheduler(AdressOfNextStartCell);
	}
	if (!flags.RelevanceOfNextStartCell) {
		NearTimeStart = Clock;
		AdressOfNextStartCell = FindNextTimeStart(&NearTimeStart);
		if (flags.DetailModeOfViewSheduler) {
			flags.DetailModeOfViewSheduler = 0;
			clrInd();
		}
		flags.RelevanceOfNextStartCell = 1;
	}
	if (AdressOfNextStartCell == 240 || !flags.DetailModeOfViewSheduler) {
		unsigned char SignalsFinal = CurrentSignals | SignalsForInd;
		if (flags.ModeOfFirstLine != (SignalsFinal > 0)) {
			clrInd();
			flags.ModeOfFirstLine = SignalsFinal > 0;
		}
		if (SignalsFinal > 0) {

			unsigned char D [] = "   ";
			for (unsigned char i = 1; i < 4; i++) {
				unsigned char SignalOn = SignalsFinal % 2;
				SignalsFinal /= 2;
				if (SignalOn == 1) {
					D[0] = D[1];
					D[1] = D[2];
					D[2] = getNumChar(i);
				}
			}
			outputString(D, 0, 13);
		}
        
        unsigned char TimeData [] = "--:--:--"; // чч:мм:сс
		long int temp = Clock / 100;
        TimeData[7] = getNumChar(temp % 10);
		temp /= 10;
        TimeData[6] = getNumChar(temp % 6);
		temp /= 6;
        TimeData[4] = getNumChar(temp % 10);
		temp /= 10;
        TimeData[3] = getNumChar(temp % 6);
		temp /= 6;
		unsigned char day = temp / 24 + 1;
		temp %= 24;
        TimeData[1] = getNumChar(temp % 10);
        TimeData[0] = getNumChar(temp / 10);
        
        SignalsFinal = CurrentSignals | SignalsForInd;
        outputString(TimeData, 0, (SignalsFinal == 7) ? 4 : 5);
        outputString(GetDayOfWeek(day), 0, (SignalsFinal == 7) ? 1 : 2);
	}
    
	if (flags.LockSignals && !flags.DetailModeOfViewSheduler) {
		outputString("Расписание откл.", 1, 0);
		return;
	}

	if (AdressOfNextStartCell != 240) {

    	unsigned char DataArray[] = EMPTY_STRING_16;
		DataArray[0] = BELL_SYMBOL;

		char line = 1;
		if (flags.DetailModeOfViewSheduler) {
			line = 0;
            DataArray[5] = 'с';
		}
        
		unsigned int TimeStart;
		unsigned int TimeStop;
		unsigned char Days;
		unsigned char Signals;
		ParseDataRecord(ReadFourBytesEE(AdressOfNextStartCell), &TimeStart, &TimeStop, &Days, &Signals);
        
		unsigned int Time;
		unsigned char Day;
		ParseTime(NearTimeStart, &Time, &Day);

		DataArray[11] = getNumChar(Time % 10);
		Time /= 10;
		DataArray[10] = getNumChar(Time % 6);
		Time /= 6;
		DataArray[9] = ':';
		DataArray[8] = getNumChar(Time % 10);
		DataArray[7] = getNumChar(Time / 10);
        
		for (unsigned char i = 1; i < 4; i++) {
			unsigned char SignalOn = Signals % 2;
			Signals /= 2;
			if (SignalOn == 1) {
				DataArray[13] = DataArray[14];
				DataArray[14] = DataArray[15];
				DataArray[15] = getNumChar(i);
			}
		}
        
		unsigned char *DataArray2 = GetDayOfWeek(Day);
		DataArray[2] = DataArray2[0];
		DataArray[3] = DataArray2[1];

        outputString(DataArray, line, 0);

		if (flags.DetailModeOfViewSheduler) {
			unsigned char DataArrayOfLine2[] = "№   по --:--"; // №12   по 12:30
            int j2 = sizeof(DataArrayOfLine2)-1;
			DataArrayOfLine2[--j2] = getNumChar(TimeStop % 10);
			TimeStop /= 10;
			DataArrayOfLine2[--j2] = getNumChar(TimeStop % 6);
			TimeStop /= 6;
            j2--;
			DataArrayOfLine2[--j2] = getNumChar(TimeStop % 10);
			DataArrayOfLine2[--j2] = getNumChar(TimeStop / 10);

			unsigned char CellsNumber = AdressOfNextStartCell / 4 + 1;
			do {
				char d = getNumChar(CellsNumber % 10);
				DataArrayOfLine2[2] = DataArrayOfLine2[1];
				DataArrayOfLine2[1] = d;
				CellsNumber /= 10;
			} while (CellsNumber > 0);

			outputString(DataArrayOfLine2, 1, 0);
		}
	} else {
		if (flags.DetailModeOfViewSheduler) {
			clrInd();
			flags.DetailModeOfViewSheduler = 0;
		}
        outputString(EMPTY_STRING_16, 1, 0);
	}
}

void EEWR(unsigned int adress, unsigned char data) {
	volatile unsigned char INTCON_BUP = INTCON;
	INTCONbits.GIEH = 0;
	INTCONbits.GIEL = 0;
	CLRWDT();
	EEADRH = adress >> 8;
	EEADR = adress % 255;
	EEDATA = data;
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.WREN = 1;
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1bits.WR = 1;
	EECON1bits.WREN = 0;
	while (EECON1bits.WR == 1) {
	}
	PIR2bits.EEIF = 0;
	INTCON = INTCON_BUP;
}

unsigned char EERD(unsigned int adress) {
	volatile unsigned char INTCON_BUP = INTCON;
	volatile unsigned char EEDATA_BUP;
	INTCONbits.GIEH = 0;
	INTCONbits.GIEL = 0;
	ClrWdt();
	EEADRH = adress >> 8;
	EEADR = adress % 255;
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.RD = 1;
	EEDATA_BUP = EEDATA;
	INTCON = INTCON_BUP;
	return EEDATA_BUP;
}

void outputDateData(SymbolData *data, unsigned char length, unsigned char line, unsigned char start_symbol){
	for(unsigned char i = 0; i < length; i++){
		setDigit(line, start_symbol + data[i].index, data[i].symbol);
		setBlink(line, start_symbol + data[i].index, data[i].props.blink);
	}
}

void ReIndTimeEdit(TimeEditData *TimeData) {
	for(char i = 0; i < 10; i++){
		if((*TimeData).Date[i].props.const_symbol == 0){
			(*TimeData).Date[i].symbol = getNumChar((*TimeData).Date[i].number);
		}
		(*TimeData).Date[i].index = i;
	}
	for(char i = 0; i < 5; i++){
		if((*TimeData).Time[i].props.const_symbol == 0){
			(*TimeData).Time[i].symbol = getNumChar((*TimeData).Time[i].number);
		}
		(*TimeData).Time[i].index = i;
	}
	(*TimeData).WeekDay[0].symbol = getNumChar((*TimeData).WeekDay[0].number);
	(*TimeData).WeekDay[0].index = 0;
	
	char n = 1;
	outputDateData((*TimeData).Date, 10, 0, n);
	outputDateData((*TimeData).Time, 5, 1, n+4);
	outputDateData((*TimeData).WeekDay, 1, 1, n+1);
	
}

void TimeData_BlinkOFF(TimeEditData * TimeData){
	for(char i = 0; i < 10; i++){
		(*TimeData).Date[i].props.blink = 0;
	}
	for(char i = 0; i < 5; i++){
		(*TimeData).Time[i].props.blink = 0;
	}
	(*TimeData).WeekDay[0].props.blink = 0;
}

void TimeData_INIT(TimeEditData * TimeData){
	for(char i = 0; i < 10; i++){
		(*TimeData).Date[i].props.blink = 0;
		(*TimeData).Date[i].props.const_symbol = 0;
	}
	for(char i = 0; i < 5; i++){
		(*TimeData).Time[i].props.blink = 0;
		(*TimeData).Time[i].props.const_symbol = 0;
	}
	(*TimeData).WeekDay[0].props.blink = 0;
	(*TimeData).WeekDay[0].props.const_symbol = 0;

	(*TimeData).Time[2].symbol = ':';
	(*TimeData).Time[2].props.const_symbol = 1;
	(*TimeData).Date[5].symbol = '/';
	(*TimeData).Date[5].props.const_symbol = 1;
	(*TimeData).Date[2].symbol = '/';
	(*TimeData).Date[2].props.const_symbol = 1;
}

void TimeEdit() {
	
	clrInd();
	flags.isTimeSetting = 1;
    
	long int temp = Clock / 6000;
	long int n = temp;
    
    TimeEditData TimeData;
	
	TimeData_INIT(&TimeData);
	
	
    TimeData.Time[4].number = temp % 10;
	temp /= 10;
	TimeData.Time[3].number = temp % 6;
	
	temp /= 6;
	temp %= 24;
	
	TimeData.Time[1].number = temp % 10;
	TimeData.Time[0].number = temp / 10;
	
	TimeData.WeekDay[0].number = 1 + n / 1440;

	TimeData.Date[9].number = cYears % 10;
	TimeData.Date[8].number = cYears / 10;
	TimeData.Date[7].symbol = '0';
	TimeData.Date[7].props.const_symbol = 1;
	TimeData.Date[6].symbol = '2';
	TimeData.Date[6].props.const_symbol = 1;
	
	TimeData.Date[4].number = cMonths % 10;
	TimeData.Date[3].number = cMonths / 10;
	
	TimeData.Date[1].number = cDays % 10;
	TimeData.Date[0].number = cDays / 10;
	
	
	SymbolData *symb_array [11];
	
	symb_array[0] = &TimeData.Date[0];	// day_H
	symb_array[1] = &TimeData.Date[1];	// day_L
	symb_array[2] = &TimeData.Date[3];	// month_H
	symb_array[3] = &TimeData.Date[4];	// month_L
	symb_array[4] = &TimeData.Date[8];	// year_H
	symb_array[5] = &TimeData.Date[9];	// year_L
	
	symb_array[6] = &TimeData.WeekDay[0];// day_of_week
	
	symb_array[7] = &TimeData.Time[0];	// hour_H
	symb_array[8] = &TimeData.Time[1];	// hour_L
	symb_array[9] = &TimeData.Time[3];	// minute_H
	symb_array[10] = &TimeData.Time[4];	// minute_L	
	
	
	char blinking = 7;
	char input = 255;
	while(1){
		(*symb_array[blinking]).props.blink = 1;
		
		ReIndTimeEdit(&TimeData);
		
		input = entNum(0, 9);
		
		if(input != 255){
			(*symb_array[blinking]).number = input;
			TimeData_BlinkOFF(&TimeData);
			blinking == 10 ? blinking = 0 : blinking++;
		}
		
		if(KeyCode == 40){
			KeyCode = 0;
			TimeData_BlinkOFF(&TimeData);
			blinking == 10 ? blinking = 0 : blinking++;
		}else if(KeyCode == 41){
			KeyCode = 0;
			TimeData_BlinkOFF(&TimeData);
			blinking == 0 ? blinking = 10 : blinking--;
		}else if(KeyCode == 42){
			KeyCode = 0;
			unsigned char t;
			do{
				t = (*symb_array[2]).number * 10 + (*symb_array[3]).number;
				if(t > 12){
					TimeData_BlinkOFF(&TimeData);
					blinking = 2;
					break;
				}
				
				unsigned char month = t;
				
				unsigned char MonthDays [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
				t = (*symb_array[4]).number * 10 + (*symb_array[5]).number;
				if(t % 4 == 0){
					MonthDays[1] = 29;
				}
				t = (*symb_array[0]).number * 10 + (*symb_array[1]).number;
				if(MonthDays[month-1] < t){
					(*symb_array[0]).number = MonthDays[month-1] / 10;
					(*symb_array[1]).number = MonthDays[month-1] % 10;
					TimeData_BlinkOFF(&TimeData);
					blinking = 0;
					break;
				}
				
				t = (*symb_array[6]).number;
				if(t > 7){
					TimeData_BlinkOFF(&TimeData);
					blinking = 6;
					break;
				}
				
				t = (*symb_array[7]).number * 10 + (*symb_array[8]).number;
				if(t > 23){
					TimeData_BlinkOFF(&TimeData);
					blinking = 7;
					break;
				}
				
				t = (*symb_array[9]).number * 10 + (*symb_array[10]).number;
				if(t > 59){
					TimeData_BlinkOFF(&TimeData);
					blinking = 9;
					break;
				}
				
				t = 0;
				
				Clock = ((long int)((*symb_array[6]).number - 1) * 86400
						+ (long int)((*symb_array[7]).number * 10 + (*symb_array[8]).number) * 3600
						+ (long int)((*symb_array[9]).number * 10 + (*symb_array[10]).number) * 60) * 100;

				cDays = (*symb_array[0]).number * 10 + (*symb_array[1]).number;
				cMonths = (*symb_array[2]).number * 10 + (*symb_array[3]).number;
				cYears = (*symb_array[4]).number * 10 + (*symb_array[5]).number;
				/*
				WriteTime(Clock, cDays, cMonths, cYears);
				*/
				
			}while(0);
			
			if(t == 0){
				clrInd();
				break;
			}
		}else if(KeyCode == 43){
			KeyCode = 0;
			clrInd();
			break;
		}
	}
}

unsigned long int ReadFourBytesEE(unsigned int adress) {
	unsigned long int x;
	for (int i = 3; i >= 0; i--) {
		x = x * 256 + EERD(adress + i);
	}
	return x;
}

void WriteFourBytesEE(unsigned int adress, unsigned long int data) {
	for (char i = 0; i < 4; i++) {
		EEWR(adress + i, data % 256);
		data /= 256;
	}
}

void delay(unsigned char del, unsigned int count) {
	while (count > 0) {
		TMR2 = 0;
		PR2 = del;
		TMR2IF = 0;
		while (!TMR2IF);
		count--;
	};
	return;
}

void SendSymbolToLCD(unsigned char Symb) {
	RS = 1;
	PORTA = (0b11110000 & Symb) / 8 | (flags.LCD_Light_On == 1 ? 0b00100000 : 0b00000000);
	E = 1;
	delay(10, 1);
	E = 0;
	RS = 0;
	delay(25, 5);

	RS = 1;
	PORTA = (0b11110000 & (Symb * 16)) / 8 | (flags.LCD_Light_On == 1 ? 0b00100000 : 0b00000000);
	E = 1;
	delay(10, 1);
	E = 0;
	RS = 0;
	delay(25, 5);
}

void SendArrayToLCD(unsigned char *Symb, char line, char position) {
	unsigned int i = 0;
	while (Symb[i] != 255 && position < 17) {
		setDigit(line, position, Symb[i]);
		i++;
		position++;
	}
}

void lcd_send_initial_half_byte(unsigned char data) {
	RS = 0;
	PORTA = (0b00011110 & (data * 2)) | (flags.LCD_Light_On == 1 ? 0b00100000 : 0b00000000);
	E = 1;
	E = 0;
}

void lcd_send_byte(unsigned char data) {
	lcd_send_initial_half_byte((data & 0b11110000) / 16);
	__delay_us(40);
	lcd_send_initial_half_byte(data & 0b00001111);
	__delay_us(40);
}

void lcd_off() {
	flags.LCD_Light_On = 0;
	lcd_send_byte(0b00001000);
	flags.LCD_Power_On = 0;
	//LCD_Power_On_PIN = 0;
}

void lcd_on() {
	lcd_init();
	lcd_send_byte(0b00001100);
	flags.LCD_Power_On = 1;
	//LCD_Power_On_PIN = 1;
}

void lcd_init() {
	unsigned char INTCON_BUF = INTCON;
	INTCONbits.GIEH = 0;
	INTCONbits.GIEL = 0;
	E = 0;
	RS = 0;
	// http://cxem.net/mcmc89.php
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

	//lcd_clear
	E = 0;
	RS = 0;
	//RW = 0;
	lcd_send_byte(0b00000001);
	__delay_ms(2);

	INTCON = (INTCON_BUF | 0b10000000) | (INTCON & 0b01111111);
}

unsigned char TestKeyGroup() {
	if (PORTBbits.RB6 == 0) {
		return 1;
	} else if (PORTBbits.RB4 == 0) {
		return 2;
	} else if (PORTBbits.RB2 == 0) {
		return 3;
	} else if (PORTBbits.RB0 == 0) {
		return 4;
	} else {
		return 17;
	}
}

unsigned char ConvKeyNum(unsigned char Num) {
	switch (Num) {
	case 0: return 33;
	case 1: return 43;
	case 2: return 42;
	case 3: return 41;
	case 4: return 40;
	case 5: return 45;
	case 6: return 39;
	case 7: return 36;
	case 8: return 37;
	case 9: return 38;
	case 10: return 35;
	case 11: return 34;
	case 12: return 32;
	case 13: return 30;
	case 14: return 44;
	case 15: return 31;
	default: return 0;
	}
}

unsigned char FindCell(unsigned int adressStart, char New, unsigned char previous) {
	if (adressStart == 240)adressStart = 236;
	unsigned char adress = adressStart;
	unsigned char adressNew = 240;
	do {
		if (!previous) {
			adress == 236 ? adress = 0 : adress += 4;
		} else {
			adress == 0 ? adress = 236 : adress -= 4;
		}
		unsigned char D = EERD(adress + 2);
		if (New != 1 ? D != 0xFF : D == 0xFF) {
			adressNew = adress;
			break;
		}
	} while (adress != adressStart);
	return adressNew;
}
 
unsigned char RefreshSchedulerIndicator(unsigned int adress, char New, unsigned int CopyFrom) {
    
    unsigned char cell = 0;
    
	if (adress != 240 && (New != 1 ? EERD(adress + 2) != 0xFF : EERD(adress + 2) == 0xFF)) {
		cell = adress / 4 + 1, 1;
	} else {
		adress = FindCell(adress, New, 0);
		if (adress != 240) {
        	cell = adress / 4 + 1, 1;
		}
	}
    
    if(cell != 0){
        unsigned char num [] = "  ";
        NumericToString(cell, num, sizeof(num)-1);
        outputString(num, 0, 13);
    }else{
        setDigit(0, 13, '-');
        setDigit(0, 14, '-');
    }
    
	if (New == 1) {
		if (CopyFrom != 240) {
            outputString("to ", 0, 1);
            outputString("copy from ", 1, 1);
			
            unsigned char NumberFrom = CopyFrom / 4 + 1;
			if (NumberFrom > 9) {
				setDigit(1, 13, getNumChar(NumberFrom / 10));
			}
			setDigit(1, 14, getNumChar(NumberFrom % 10));
		}
        outputString("new ", 0, 4);
	}
    outputString("cell ", 0, 8);
    
	return adress;
}

void TimesToTimeData(unsigned int * Times, unsigned char * TimeData) {
	unsigned int temp;
	for (char j = 0; j < 2; j++) {
		temp = Times[j];
        char d = 4 * j;
		TimeData[3 + d] = temp % 10;
		temp /= 10;
		TimeData[2 + d] = temp % 6;
		temp = temp / 6;
		TimeData[1 + d] = temp % 10;
		TimeData[0 + d] = temp / 10;
	}
}

void TimeDataToString(unsigned char * TimeData, unsigned char * string) {
    unsigned char j = 0;
	for (unsigned char i = 0; i < 12; i++) {
        if(string[i] == '*'){
            string[i] = getNumChar(TimeData[j]);
            j++;
        }
	}
}

void ItemsToIndicator(unsigned char items, unsigned char max_i, unsigned char firstDigit) {
	for (char i = 1; i < max_i; i++) {
		items % 2 == 1 ? setDigit(0, 16 -(firstDigit - i), getNumChar(i)) : setDigit(0, 16 - (firstDigit - i), '-');
		items /= 2;
	}
}

unsigned char getBorderUp(char n) {
	unsigned char BorderUp;
	switch (n) {
	case 11: case 5: BorderUp = 2;
		break;
	case 10: case 4: getDigit(0, n + 1) == 2 ? BorderUp = 3: BorderUp = 9;
		break;
	case 8: case 2: BorderUp = 5;
		break;
	case 7: case 1: BorderUp = 9;
		break;
	}
	return BorderUp;
}

unsigned char ConvertDayToBit(unsigned char DayNumber) {
	switch (DayNumber) {
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

unsigned char EditSchedule(unsigned int adress, unsigned int SourceOfRecord) {
	
	unsigned char TargetAdress;
	if (SourceOfRecord == 240) {
		TargetAdress = adress;
	} else {
		TargetAdress = SourceOfRecord;
	}
	unsigned long int Data = ReadFourBytesEE(TargetAdress);

	char BeginEditTimes = 0;
	if (Data == 0xFFFFFFFF) {
		Data = 0;
		BeginEditTimes = 1;
	}

	unsigned int TimeStart;
	unsigned int TimeStop;
	unsigned char Days;
	unsigned char Signals;
	ParseDataRecord(Data, &TimeStart, &TimeStop, &Days, &Signals);

	unsigned int Times[2];
	Times[0] = TimeStart;
	Times[1] = TimeStop;
    
    unsigned char TimeData[8];
    TimesToTimeData(Times, TimeData);
    
	clrInd();
    
	char mode = 1;
	unsigned char key;
    
    while (1) {
        if (KeyCode == 43) { // Cancel
            KeyCode = 0;
            clrInd();
            return 0;
        } else if (KeyCode == 42) { // Enter
            KeyCode = 0;
            Data = (unsigned long int) Signals * 0x20000000 + (unsigned long int) Days * 0x400000 + (unsigned long int) Times[1]*0x800 + (unsigned long int) Times[0];
            WriteFourBytesEE(adress, Data);
            clrInd();
            return 1;
        }    
        
        switch (mode) {
            case 1:
            {
                unsigned char string [] = "**:**-**:**";
                TimeDataToString(TimeData, string);
                outputString(string, 0, 5);
                
                if (KeyCode == 45 || BeginEditTimes == 1) {
                    KeyCode = 0;
                    BeginEditTimes = 0;
                    char n = 5;
                    setBlink(0, n, 1);
                    unsigned char BorderUp = 2;
                    while (1) {
                        if (KeyCode == 43) { // Cancel
                            KeyCode = 0;
                            clrInd();
                            break;
                        } else if (KeyCode == 42) { // Enter
                            KeyCode = 0;
                            unsigned int T[2];
                            for (char j = 0; j < 2; j++) {
                                char f = j * 4;
                                T[j] = TimeData[f + 0] * 600
                                        + TimeData[f + 1] * 60
                                        + TimeData[f + 2] * 10
                                        + TimeData[f + 3];
                            }
                            Times[0] = T[0];
                            if (T[0] < T[1]) {
                                Times[1] = T[1];
                                clrInd();
                                break;
                            } else {
                                setBlink(0, n, 0);
                                n = 11;
                                BorderUp = getBorderUp(n);
                                Times[1] = T[0];
                                TimesToTimeData(Times, TimeData);
                                setBlink(0, n, 1);
                            }
                        } else if (KeyCode == 41) { // NextDig
                            KeyCode = 0;
                            if (n > 5) {
                                setBlink(0, n, 0);
                                (n == 8 || n == 11 || n == 14) ? n -= 2 : n--;
                                BorderUp = getBorderUp(n);
                                setBlink(0, n, 1);
                            }
                        } else if (KeyCode == 40) { //PrevDig
                            KeyCode = 0;
                            if (n < 15) {
                                setBlink(0, n, 0);
                                n == 6 || n == 9 || n == 12 ? n += 2 : n++;
                                BorderUp = getBorderUp(n);
                                setBlink(0, n, 1);
                            }
                        }
                        
                        unsigned char string2 [] = "**:**-**:**";
                        TimeDataToString(TimeData, string2);
                        outputString(string2, 0, 5);
                        
                        char d = 0;
                        switch(n){
                            case 5: d = 0; break;
                            case 6: d = 1; break;
                            case 8: d = 2; break;
                            case 9: d = 3; break;
                            case 11: d = 4; break;
                            case 12: d = 5; break;
                            case 14: d = 6; break;
                            case 15: d = 7; break;
                        }
                        
                        key = entNum(0, BorderUp);
                        if (key != 255) {
                            TimeData[d] = key;
                            if ((d == 0 || d == 4) && key == 2 && (TimeData[d+1] > 3)){
                                TimeData[d+1] = 3;
                            }
                            if (n > 1) {
                                KeyCode = 40;
                            } else {
                                KeyCode = 42;
                            }
                        }
                    }
                }
            }
                break;
            case 2:
            {
                ItemsToIndicator(Days, 8, 9);
                key = entNum(1, 7);
                if (key != 255) {
                    Days ^= ConvertDayToBit(key);
                } else if (KeyCode == 30) {
                    KeyCode = 0;
                    Days = 0;
                } else if (KeyCode == 45) {
                    KeyCode = 0;
                    Days = 0b01111111;
                }
            }
                break;
            case 3:
            {
                ItemsToIndicator(Signals, 4, 8);
                key = entNum(1, 3);
                if (key != 255) {
                    Signals ^= ConvertDayToBit(key);
                } else if (KeyCode == 30) {
                    KeyCode = 0;
                    Signals = 0;
                } else if (KeyCode == 45) {
                    KeyCode = 0;
                    Signals = 0b00000111;
                }
            }
        }       
        if (KeyCode == 40) {
            KeyCode = 0;
            clrInd();
            if(mode == 3){
                mode = 1;
            }else{
                mode++;
            }
        } else if (KeyCode == 41) {
            KeyCode = 0;
            clrInd();
            if(mode == 1){
                mode = 3;
            }else{
                mode--;
            }
        }
    }
    return 0;
}

void Scheduler(unsigned int StartFrom) {
	clrInd();
	volatile static unsigned int NumberOfCells = 0;
	if (StartFrom != 240) {
		NumberOfCells = StartFrom;
	}
	NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, 0, 240);
	while (1) {
		if (NumberOfCells != 240 && KeyCode == 44) { // Delete
			KeyCode = 0;
			WriteFourBytesEE(NumberOfCells, 0xFFFFFFFF);
			NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, 0, 240);
		} else if (KeyCode == 35) { // New
			KeyCode = 0;
			SchedulerNew(240);
			RefreshSchedulerIndicator(NumberOfCells, 0, 240);
		} else if (NumberOfCells != 240 && KeyCode == 39) { // New copy
			KeyCode = 0;
			SchedulerNew(NumberOfCells);
			RefreshSchedulerIndicator(NumberOfCells, 0, 240);
		} else if (NumberOfCells != 240 && KeyCode == 45) { // Edit
			KeyCode = 0;
			EditSchedule(NumberOfCells, 240);
			RefreshSchedulerIndicator(NumberOfCells, 0, 240);
		} else if (KeyCode == 40) { // Next
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 0, 0);
			RefreshSchedulerIndicator(NumberOfCells, 0, 240);
		} else if (KeyCode == 41) { // Prev
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 0, 1);
			RefreshSchedulerIndicator(NumberOfCells, 0, 240);
		} else if (KeyCode == 43) { // Exit
			KeyCode = 0;
			clrInd();
			return;
		} else if (KeyCode == 30) { // Adress = 0
			KeyCode = 0;
			NumberOfCells = 0;
			NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, 0, 240);
		}
	}
}

void SchedulerNew(unsigned int SourceOfRecord) {
	clrInd();
	volatile static unsigned char NumberOfCells = 0;
	NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, 1, SourceOfRecord);
	while (1) {
		if (NumberOfCells != 240 && KeyCode == 45) { // Edit
			KeyCode = 0;
			if (EditSchedule(NumberOfCells, SourceOfRecord) == 1) {
				NumberOfCells = FindCell(NumberOfCells, 1, 0);
			}
			RefreshSchedulerIndicator(NumberOfCells, 1, SourceOfRecord);
		} else if (KeyCode == 40) { // Next
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 1, 0);
			RefreshSchedulerIndicator(NumberOfCells, 1, SourceOfRecord);
		} else if (KeyCode == 41) { // Prev
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 1, 1);
			RefreshSchedulerIndicator(NumberOfCells, 1, SourceOfRecord);
		} else if (KeyCode == 43) { // Exit
			KeyCode = 0;
			clrInd();
			return;
		} else if (KeyCode == 30) { // Adress = 0
			KeyCode = 0;
			NumberOfCells = 0;
			NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, 1, SourceOfRecord);
		}
	}
}

void ParseTime(long int TimeValue, unsigned int *Time, unsigned char *Day) {
	unsigned int DayTime = (unsigned int) (TimeValue / 6000);
	*Time = DayTime % 1440;
	*Day = DayTime / 1440 + 1;
}

void ParseDataRecord(unsigned long int Data, unsigned int *TimeStart, unsigned int *TimeStop, unsigned char *Days, unsigned char *Signals) {
	//	if(Data!=0xFFFFFFFF){
	*TimeStart = (unsigned int) (Data % (unsigned long int) 2048);
	Data /= 2048;
	*TimeStop = (unsigned int) (Data % (unsigned long int) 2048);
	Data /= 2048;
	*Days = (unsigned char) (Data % (unsigned long int) 128);
	Data /= 128;
	*Signals = (unsigned char) (Data % (unsigned long int) 8);
	//	}
}

unsigned int FindNextTimeStart(unsigned long int *TimeFrom) {

	unsigned char adress = 0;
	unsigned char adressStart = 240;
	unsigned char TimeIsFound = 0;
	unsigned long int LastFoundTimeStart;

	do {
		unsigned long int Data = ReadFourBytesEE(adress);

		adress += 4;
		if (Data == 0xFFFFFFFF) {
			continue;
		}
		unsigned int TimeStart;
		unsigned int TimeStop;
		unsigned char Days;
		unsigned char Signals;
		ParseDataRecord(Data, &TimeStart, &TimeStop, &Days, &Signals);

		if (Signals == 0) {
			continue;
		}
		for (char DayIndex = 0; DayIndex < 7; DayIndex++) {
			unsigned char DayOn = Days % 2;
			Days /= 2;
			if (DayOn != 1) {
				continue;
			}
			unsigned long int TimeOfSignals = ((unsigned long int) DayIndex * 86400 + (unsigned long int) TimeStart * 60) * 100;

			if ((TimeOfSignals < LastFoundTimeStart || TimeIsFound == 0) && TimeOfSignals > *TimeFrom) {
				TimeIsFound = 1;
				LastFoundTimeStart = TimeOfSignals;
				adressStart = adress - 4;
			}
		}
	} while (adress < 240);

	if (TimeIsFound == 1) {
		*TimeFrom = LastFoundTimeStart;
	} else {
		*TimeFrom = 0;
	}
	return adressStart;
}

void SignalsOnOff() {
	if (!flags.TimeIsRead) return;
	if (!flags.LockSignals && CurrentReadingCell < 240) {
		unsigned long int Data = ReadFourBytesEE(CurrentReadingCell);
		if (Data != 0xFFFFFFFF) {
			unsigned int TimeStart;
			unsigned int TimeStop;
			unsigned char Days;
			unsigned char Signals;
			TimeStart = Data % 2048;
			Data /= 2048;
			TimeStop = Data % 2048;
			Data /= 2048;
			Days = Data % 128;
			Data /= 128;
			Signals = Data % 8;
			if (Signals != 0 && Days != 0) {
				unsigned int ThisTime;
				unsigned char ThisDay;
				unsigned int DayTime = (unsigned int) (Clock / 6000);
				ThisTime = DayTime % 1440;
				ThisDay = DayTime / 1440 + 1;
				ThisDay = ConvertDayToBit(ThisDay);
				if (((ThisDay & Days) > 0) && (TimeStart <= ThisTime) && (TimeStop > ThisTime)) {
					SignalsOut = Signals | SignalsOut;
				}
			}
		}
		CurrentReadingCell += 4;
	} else {
		SignalsForInd = (flags.LockSignals ? 0 : SignalsOut);
		SignalsOut = 0;
		CurrentReadingCell = 0;
		unsigned char Data = (PORTB & 0b00011111) | ((CurrentSignals | SignalsForInd)*32);
		if ((Data & 0b11100000) != CurrentSignalsData) {
			EEWR(cMinutesAdress, cMinutes);
			EEWR(cHoursAdress, cHours);
			EEWR(cWeekDayAdress, cWeekDay);
			EEWR(cDaysAdress, cDays);
			EEWR(cMonthsAdress, cMonths);
			EEWR(cYearsAdress, cYears);
		}
		CurrentSignalsData = Data & 0b11100000;
		PORTB = Data;
	}
}

void FillMinutes() {
	Minutes = Clock - 3000;
}

void interrupt high_priority F_h() {
	ClrWdt();
}

void interrupt low_priority F_l() {
	ClrWdt();
	if (TMR1IF) {
		TMR1IF = 0;
		TMR1 += 51200;
		SignalsOnOff();
		char MinuteAgo = (Clock - Minutes > 6000);
		if (MinuteAgo > 0) {
			FillMinutes();
			flags.RelevanceOfNextStartCell = 0;
		}

		if (flags.LCD_Power_On && (GlobalBlinkCycleTime == 100 || flags.IsLCDModified)) {
			for (char line = 0; line < 2; line++) {
				for (int symbol = 0; symbol < 16; symbol++) {
					if (GlobalBlinkCycleTime == 100 || digitsAtrib[line][symbol].modifided > 0) {
						unsigned char digit = digits[line][symbol];
						lcd_send_byte((line * 0x40 + symbol) | 0b10000000); //SetDDRAM
						if (!(digitsAtrib[line][symbol].blink == 0 || digitsAtrib[line][symbol].blink == 1 && flags.GlobalBlink)) {
							digit = '_';
						}
						SendSymbolToLCD(getLcdCodeOfChar(digit));
						digitsAtrib[line][symbol].modifided = 0;
					}
				}
			}
			flags.IsLCDModified = 0;
		}

		if (GlobalBlinkCycleTime > 0) {
			GlobalBlinkCycleTime--;
		} else {
			flags.IsLCDModified = 1;
			GlobalBlinkCycleTime = 100;
			flags.GlobalBlink = !flags.GlobalBlink;

			// 	ReadTime();
		}

		static unsigned char ButtonPressTimeOut;

		static unsigned char KeyIndex;
		static unsigned char PressedKeyIndex = 0xFF;

		if (ButtonPressTimeOut > 0) {
			ButtonPressTimeOut--;
		} else {
			KeyCode = 0;
			if (RB7) {
				if (KeyIndex == 0x0F) {
					KeyIndex = 0;
				} else {
					KeyIndex++;
				}
				unsigned char data_temp = KeyIndex;
				unsigned char _KeyIndex = 0;
				for (int a = 4; a > 0; a--) {
					_KeyIndex *= 2;
					if (data_temp % 2 == 1) {
						_KeyIndex++;
					}
					data_temp /= 2;
				}

				PORTB = _KeyIndex * 2; //(PORTB & 0b0000000) | ()
				PressedKeyIndex = 0xFF;
			} else {
				if (PressedKeyIndex == 0xFF) {
					PressedKeyIndex = KeyIndex;
					KeyCode = ConvKeyNum(PressedKeyIndex);
					ButtonPressTimeOut = 30;
				}
			}
		}
		T0IF = 0;
	}
}

void main() {

	flags.TimeIsRead = 0;

	INTCON = 0;
	INTCON2 = 0;
	INTCON3 = 0;
	PIR1 = 0;
	PIR2 = 0;
	PIE1 = 0;
	PIE2 = 0;
	IPR1 = 0;
	IPR2 = 0;
	RCON = 0;

	clrInd();

	RCONbits.IPEN = 1; // Разрешить двуприоритетные прерывания

	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;

	T2CONbits.TMR2ON = 1;

	T1CONbits.RD16 = 1;
	TMR1 = 51200;
	T1CONbits.TMR1ON = 1;
	PIE1bits.TMR1IE = 1;

	ADCON1 = 0b00001111;
	TRISA = 0b00000000;
	TRISB = 0b10000000;
	TRISC = 0b00000000;

	Clock = 0;
	LATA = 0b00000000;
	LATB = 0b00000000;
	LATC = 0b00000000;
	T2CON = 0b00100100;

	flags.LockSignals = EERD(251);

	Clock = ((long int) cMinutes * 60
		+ (long int) cHours * 3600
		+ ((long int) cWeekDay - 1) * 86400) * 100;

	lcd_on();
	main2();
}

unsigned char getNumChar(unsigned char num) {
	switch (num) {
		case 0: return '0';
		case 1: return '1';
		case 2: return '2';
		case 3: return '3';
		case 4: return '4';
		case 5: return '5';
		case 6: return '6';
		case 7: return '7';
		case 8: return '8';
		case 9: return '9';
		case 10: return 'A';
		case 11: return 'B';
		case 12: return 'C';
		case 13: return 'D';
		case 14: return 'E';
		case 15: return 'F';
		default: return '?';
	}
}

void outputString(unsigned char * stringData, unsigned char line, unsigned char position) {
	unsigned int i = 0;
	while(stringData[i] != '\0'){
		unsigned char l = line;
		for(line = 0; l < 2; l++) {
			unsigned char s = position;
			for (position = 0; (s < 16) && (stringData[i] != '\0'); s++) {
				setDigit(l, s, stringData[i]);
				i++;
			}
		}
	}
}

void drowText(unsigned char * stringData, int startNum, int direction){
	unsigned char srcLine, destLine;
	if(direction >= 0){
		srcLine = 1;
		destLine = 0;
	}else{
		srcLine = 0;
		destLine = 1;
	}
	
	for(unsigned char symbol = 0; symbol < 16; symbol++){
		digits[destLine][symbol] = digits[srcLine][symbol];
		digitsAtrib[destLine][symbol].blink = digitsAtrib[srcLine][symbol].blink;
		digitsAtrib[destLine][symbol].modifided = 1;
	}
	
	flags.IsLCDModified = 1;
	
	unsigned char cutedString[17] = EMPTY_STRING_16;
	for(unsigned char symbol = 0; symbol < 16; symbol++) {
		unsigned char a = stringData[symbol + startNum];
		if(a == '\0'){
			break;
		}
		cutedString[symbol] = a;
	}
	
	outputString(cutedString, (direction >= 0 ? 1 : 0), 0);
}

void main2() {
	
    flags.LCD_Light_On = 1;
    
	flags.isTimeSetting = 0;

	NearTimeStart = Clock;
	AdressOfNextStartCell = FindNextTimeStart(&NearTimeStart);
		
	while (1) {
		
		if (flags.LCD_Power_On == 1 && LCD_ON_TIMEOUT == 0 && CurrentSignals == 0) {
			lcd_off();
		} else if (flags.LCD_Power_On == 0 && KeyCode != 0) {
			KeyCode = 0;
			lcd_on();
		}

		if (flags.LCD_Power_On == 0) {
			continue;
		}

		TimeToInd();

		if (KeyCode == 45 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			TimeEdit();
		} else if (KeyCode == 40 || KeyCode == 41) {
			KeyCode = 0;
			Scheduler(240);
		} else if (KeyCode == 44) {
			KeyCode = 0;
			if (flags.LCD_Power_On) {
				flags.LCD_Light_On = !flags.LCD_Light_On;
			}
		} else if (KeyCode == 30) {
			KeyCode = 0;
			flags.LockSignals = 1;
			CurrentSignals = 0;
			EEWR(251, flags.LockSignals);
		} else if (KeyCode == 31 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			clrInd();
			CurrentSignals = CurrentSignals^0b00000001;
		} else if (KeyCode == 32 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			clrInd();
			CurrentSignals = CurrentSignals^0b00000010;
		} else if (KeyCode == 33 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			clrInd();
			CurrentSignals = CurrentSignals^0b00000100;
		} else if (KeyCode == 34 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			clrInd();
			CurrentSignals = 0;
		} else if (KeyCode == 35 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			flags.LockSignals = 0;
			EEWR(251, flags.LockSignals);
			clrInd();
		} else if (KeyCode == 37) {
			KeyCode = 0;
			lcd_off();
		} else if (KeyCode == 43 || (KeyCode == 36 && (!flags.LockSignals || flags.DetailModeOfViewSheduler))) {
			if (KeyCode == 43) {
				NearTimeStart = Clock;
				if (flags.DetailModeOfViewSheduler) {
					flags.DetailModeOfViewSheduler = 0;
					clrInd();
				}
			}
			KeyCode = 0;
			FillMinutes();
			AdressOfNextStartCell = FindNextTimeStart(&NearTimeStart);
		}
	}
}

unsigned char BCD_to_DEC(unsigned char BCD) {
	return ((BCD & 0b11110000) >> 4) * 10 + (BCD & 0b00001111);
}

unsigned char DEC_to_BCD(unsigned char DEC) {
	if (DEC > 99) {
		DEC = 99;
	}
	return ((DEC / 10) << 4) +DEC % 10;
}

void ReadTime() {

	unsigned char Seconds = 0x00;
	
	I2CInit();
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x00);
	I2CRestart();
	I2CSend(0xD1);


	unsigned char I = I2CRead();

	if ((I & 0b10000000) == 0) {
		flags.TimeIsRead = 1;
		I2CAck();
		Seconds = BCD_to_DEC(I);
		cMinutes = BCD_to_DEC(I2CRead());
		I2CAck();
		cHours = BCD_to_DEC(I2CRead());
		I2CAck();
		cWeekDay = BCD_to_DEC(I2CRead());
		if (!flags.isTimeSetting) {
			I2CAck();
			cDays = BCD_to_DEC(I2CRead());
			I2CAck();
			cMonths = BCD_to_DEC(I2CRead());
			I2CAck();
			cYears = BCD_to_DEC(I2CRead());
		}
	} else {
		flags.TimeIsRead = 0;
		I2CNak();
		I2CStop();
		cDays = EERD(cDaysAdress);
		cMonths = EERD(cMonthsAdress);
		cYears = EERD(cYearsAdress);
		
		return;
	}
	I2CNak();

	I2CStop();

	Clock = ((long int) Seconds
		+ (long int) cMinutes * 60
		+ (long int) cHours * 3600
		+ ((long int) cWeekDay - 1) * 86400) * 100;

}

void WriteTime(unsigned long int lClock, unsigned char days, unsigned char months, unsigned char years) {

	volatile unsigned char INTCON_BUP = INTCON;
	INTCONbits.GIEH = 0;
	INTCONbits.GIEL = 0;
	ClrWdt();

	unsigned char week_day;
	unsigned char hours;
	unsigned char minutes;
	
	lClock/=6000;
	minutes = DEC_to_BCD(lClock % 60);
	lClock /= 60;
	hours = DEC_to_BCD(lClock % 24);
	lClock /= 24;
	week_day = DEC_to_BCD(lClock + 1);
	
	I2CInit();
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x00);

	I2CSend(0x00); // Seconds
	I2CSend(minutes); // Minutes
	I2CSend(hours); // Hours
	I2CSend(week_day); // DayOfWeek
	I2CSend(DEC_to_BCD(days)); // Days
	I2CSend(DEC_to_BCD(months)); // Mouths
	I2CSend(DEC_to_BCD(years)); // Years
	I2CSend(0b00010000); // Settings

	I2CStop();

	INTCON = INTCON_BUP;
	
}

void I2CInit(void) {
	TRISCbits.TRISC3 = 1; /* SDA and SCL as input pin */
	TRISCbits.TRISC4 = 1; /* these pins can be configured either i/p or o/p */
	SSPSTAT |= 0b10000000; /* Slew rate disabled */
	SSPADD = 0x18; /* 100Khz @ 40Mhz Fosc */
	SSPCON1 = 0b00101000; /* SSPEN = 1, I2C Master mode, clock = FOSC/(4 * (SSPADD + 1)) */
}

void I2CStart() {
	SSPCON2bits.SEN = 1; /* Start condition enabled */
	while (SSPCON2bits.SEN); /* automatically cleared by hardware */
	/* wait for start condition to finish */
}

void I2CStop() {
	SSPCON2bits.PEN = 1; /* Stop condition enabled */
	while (SSPCON2bits.PEN); /* Wait for stop condition to finish */
	/* PEN automatically cleared by hardware */
}

void I2CRestart() {
	SSPCON2bits.RSEN = 1; /* Repeated start enabled */
	while (SSPCON2bits.RSEN); /* wait for condition to finish */
}

void I2CAck() {
	SSPCON2bits.ACKDT = 0; /* Acknowledge data bit, 0 = ACK */
	SSPCON2bits.ACKEN = 1; /* Ack data enabled */
	while (SSPCON2bits.ACKEN); /* wait for ack data to send on bus */
}

void I2CNak() {
	SSPCON2bits.ACKDT = 1; /* Acknowledge data bit, 1 = NAK */
	SSPCON2bits.ACKEN = 1; /* Ack data enabled */
	while (SSPCON2bits.ACKEN); /* wait for ack data to send on bus */
}

void I2CSend(unsigned char dat) {
	SSPBUF = dat; /* Move data to SSPBUF */
	while (SSPSTATbits.BF); /* wait till complete data is sent from buffer */
	while ((SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100)); //I2CWait();       /* wait for any pending transfer */
}

unsigned char I2CRead(void) {
	unsigned char temp;
	/* Reception works if transfer is initiated in read mode */
	SSPCON2bits.RCEN = 1; /* Enable data reception */
	while (!SSPSTATbits.BF); /* wait for buffer full */
	temp = SSPBUF; /* Read serial buffer and store in temp register */
	while ((SSPCON2 & 0b00011111) || (SSPSTAT & 0b00000100)); //I2CWait();       /* wait to check any pending transfer */
	return temp; /* Return the read data from bus */
}