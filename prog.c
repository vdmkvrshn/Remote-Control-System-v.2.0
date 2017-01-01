
#include <xc.h>
#include <stdio.h>
#include <string.h>
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
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1024     // Watchdog Timer Postscale Select bits (1:32768)

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


#define CELL_CAPACITY 8
#define END_OF_CELLS (CELL_CAPACITY * 48)
#define LAST_CELL (END_OF_CELLS - CELL_CAPACITY)


#define PHONEBOOK_START_ADRESS (END_OF_CELLS)
#define PHONEBOOK_CAPACITY 16
#define END_OF_PHONEBOOK (PHONEBOOK_START_ADRESS + PHONEBOOK_CAPACITY * sizeof(Phone))
#define LAST_PHONEBOOK_CELL (END_OF_PHONEBOOK - sizeof(Phone))


#define TIME_CORRECT_VALUE_CELL	1012
#define cMinutesAdress 1014
#define cHoursAdress 1015
#define cWeekDayAdress 1016
#define cDaysAdress 1017
#define cMonthsAdress 1018
#define cYearsAdress 1019

#define LOCK_SIGNALS_FLAG_CELL	1020
#define USART_SPEED_NUMBER_CELL 1021
#define TIME_SOURCE_CELL 1022
#define USE_GSM_MODULE_CELL 1023


unsigned char digits[32];
unsigned char digits_system_message[32];

typedef struct {
	unsigned blink : 1;
	unsigned modifided : 1;
} digits_atributes;

digits_atributes digitsAtrib[32];
digits_atributes digitsAtrib_system_message[32];
unsigned long int _systemCounter = 0;
long int Clock;
long int lastClock;
int clock_delta = 0;
long int Minutes = 0;
#define LCD_ON_TIMEOUT_CONST 5000 // 3 min
unsigned int LCD_ON_TIMEOUT = LCD_ON_TIMEOUT_CONST;
volatile unsigned char KeyCode;
unsigned int CorrectTimeConst;
unsigned int AdressOfNextStartCell = END_OF_CELLS;
unsigned long int NearTimeStart;
unsigned long int NearTimeStop;
unsigned int CurrentReadingCell = 0;
unsigned char CurrentSignalsData = 0;
volatile unsigned char SignalsOut = 0;
volatile unsigned char SignalsForInd = 0;

struct {
	unsigned DetailModeOfViewSheduler : 1;
	unsigned RelevanceOfNextStartCell : 1;
	unsigned LCD_Light_On : 1;
	unsigned LCD_Power_On : 1;
	unsigned LockSignals : 1;
	unsigned IsLCDModified : 1;
	unsigned GlobalBlink : 1;
	unsigned GSM_Connected : 1;
	unsigned ActiveCall : 1;
	unsigned IncommingCall : 1;
	unsigned UnreadSystemMessage : 1;
	unsigned UnprocessedIncommingUartData : 1;
	unsigned SignalsAreChanged : 1;
	unsigned RemoteControlIsEnabled : 1;
	unsigned TimeSource : 2;
	unsigned SendTimeRequest : 1;
	unsigned UsartExchangeEnabled : 1;
	unsigned StatusIsRequested : 1;
	unsigned IsUartOK : 1;
	unsigned IsTemperatureRead : 1;
	unsigned Is1wireOK : 1;
	unsigned UseGSM : 1;
} flags;

unsigned char cMinutes = 0;
unsigned char cHours = 0;
unsigned char cWeekDay = 1;
unsigned char cDays = 1;
unsigned char cMonths = 1;
unsigned char cYears = 1;

unsigned char CurrentSignals = 0;
unsigned char GlobalBlinkCycleTime = 70;

typedef struct {
	unsigned blink : 1;
	unsigned const_symbol : 1;
} symbol_properties;

enum InputType {
	aa,
	Aa,
	AA
};

enum Devices {
	NON = 0,
	RTC = 1,
	GSM = 2
};

enum InputLang {
	English,
	Russian,
	Numeric
};

enum NoYes {
	No,
	Yes
};

enum CurrentShowing {
	csTime,
	csTemperature
};

typedef struct {
	unsigned Aa : 2;
	unsigned Language : 2;
	unsigned NotClearInd : 1;
	unsigned ReturnAfterInput : 1;
	unsigned BlockCharSet : 1;
	unsigned PhoneKeyboard : 1;
	unsigned char InputChar;
} input_text_props;

typedef struct {
	symbol_properties props;
	unsigned char number;
	unsigned char symbol;
	unsigned char index;
} SymbolData;

typedef struct {
	SymbolData *Time [5];
	SymbolData *WeekDay [1];
	SymbolData *Date [10];
	unsigned char TimeSource;
} TimeEditData;

typedef struct {
	unsigned char name[17];
	unsigned char phone [14];
	unsigned char blocked[1];
	unsigned char filled[1];
} Phone;


#define _XTAL_FREQ 40000000

#define E					LC0
#define RS					LC1
#define LCD_Power_On_PIN	LC2
#define GSM_PowerKey		LC5
#define GSM_PowerKey_State	TRISC5

#define EMPTY_SYMBOL_VALUE ' '
#define UNDERSCORE_SYMBOL_VALUE '_' //_
#define EMPTY_STRING_16 "                "
#define BELL_SYMBOL '\x07'
#define ANTENNA_SYMBOL '\x08'

#define BUFFER_STRING_LENGTH 256
#define MAX_INCOMMING_BUFF_INDEX 3
#define LAST_INCOMMING_BUFF_INDEX (MAX_INCOMMING_BUFF_INDEX - 1)
#define INCOMMING_BUFF_LENGTH (MAX_INCOMMING_BUFF_INDEX + 1)
/*
unsigned char buff_string_0 [BUFFER_STRING_LENGTH];
unsigned char buff_string_1 [BUFFER_STRING_LENGTH];
unsigned char buff_string_2 [BUFFER_STRING_LENGTH];
unsigned char buff_string_3 [BUFFER_STRING_LENGTH];
unsigned char buff_string_4 [BUFFER_STRING_LENGTH];
unsigned char buff_string_5 [BUFFER_STRING_LENGTH];
unsigned char buff_string_6 [BUFFER_STRING_LENGTH];
unsigned char buff_string_7 [BUFFER_STRING_LENGTH];
 */
unsigned char buff_string_8 [BUFFER_STRING_LENGTH];
unsigned char buff_string_9 [BUFFER_STRING_LENGTH];
unsigned char buff_string_10 [BUFFER_STRING_LENGTH];
unsigned char buff_string_11 [BUFFER_STRING_LENGTH];

unsigned char *IncommingBuffer[INCOMMING_BUFF_LENGTH];

unsigned char *OutcommingBuffer = NULL;

const unsigned char StandardAnswer_OK [] = "OK";
//	+CCLK: "16/05/15,22:37:52+03"
const unsigned char StandardAnswer_CLOCK [] = "+CCLK: \"\x10\x10/\x10\x10/\x10\x10,\x10\x10:\x10\x10:\x10\x10\x10\x10\x10\"";
//	+CLIP: "+380957075762",145,"",,"",0
const unsigned char StandardAnswer_INCCALL [] = "+CLIP: \"\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\x10\",145,\"\",,\"\",0";
const unsigned char StandardAnswer_BUSY [] = "BUSY";
const unsigned char StandardAnswer_NO_ANSWER [] = "NO ANSWER";
const unsigned char StandardAnswer_NO_CARRIER [] = "NO CARRIER";
const unsigned char StandardAnswer_NO_DIALTONE [] = "NO DIALTONE";
const unsigned char StandardAnswer_DTMF [] = "+DTMF:\x10";
const unsigned char StandardAnswer_ERROR [] = "ERROR";
const unsigned char StandardAnswer_NORMAL_POWER_DOWN [] = "NORMAL POWER DOWN";
const unsigned char StandardAnswer_STAT [] = "+CPAS: \x10";
/*
const unsigned char StandardAnswer_ [] = "";
const unsigned char StandardAnswer_ [] = "";
const unsigned char StandardAnswer_ [] = "";
const unsigned char StandardAnswer_ [] = "";
 */

unsigned char Clock_from_GSM[30];
unsigned char Incomming_Call_Data[36];
unsigned char Incomming_DTMF_Data[8];
unsigned char DTMF_Symbol;

unsigned char SMS [512];
unsigned char active_phone [14];

int temperature = 0; //�����������
unsigned char temp_drob = 0; //������� ����� �����������
unsigned char temp_sign; //���� �����������
unsigned char current_showing = csTime;
unsigned char Broadcasting_1_Wire;

struct {
	unsigned Init : 1;
	unsigned Send_Address : 1;
	unsigned CountAddressBytes : 4;
	unsigned SendConvertTemp : 1;
	unsigned int PauseValue;
	unsigned SendGetTemp : 1;
	unsigned ReadData : 1;
	unsigned CountDataBytes : 4;
	unsigned DataIsRead : 1;
	unsigned Error : 1;
	unsigned ActiveProcess : 1;
	unsigned Line : 8;
} getTemp_flags;

unsigned char DS_Address [] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char DS_ReadData [] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define STATE_REG TRISA
#define PIN_REG PORTA
#define FIRST_LINE_1_WIRE 0b0000001

unsigned int COUNT = 0;

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
void OutputString(unsigned char * stringData, unsigned char line, unsigned char position, unsigned char trasnsfer_line);
unsigned char I2CRead(void);
void ReadTime();
void WriteTime(unsigned long int lClock, unsigned char days, unsigned char months, unsigned char years);
unsigned char BCD_to_DEC(unsigned char BCD);
unsigned char DEC_to_BCD(unsigned char DEC);
void FillArrayFromEEPROM(unsigned char * container, unsigned int adress_start, unsigned int quantity);
void WriteArrayToEEPROM(unsigned char * container, unsigned int adress_start, unsigned int quantity);
void ReadDataOfCell(unsigned char *CellsData, unsigned long int *Data, unsigned int TargetAdress);
void CleanStringArray(unsigned char *myArray, unsigned int size, unsigned char settingData);
unsigned long int getSystemTimePoint(void);
unsigned char testTimePoint(unsigned long int point, unsigned long int value);
void PhonebookEdit(void);
unsigned char *InputText(unsigned char *text, input_text_props *_props, unsigned char capacity);
unsigned char Select_OK_NO(unsigned char *text);
unsigned char FindIncommingData(unsigned char *regexp, unsigned char *container, int history_index);
unsigned char _FindIncommingData_interrupt(unsigned char *regexp, unsigned char *container, int history_index);
char ShowMenu(unsigned char *menu[], unsigned char item);
void OutputSystemMessage(unsigned char *stringData);
void SendCommandToUSART(unsigned char *command, unsigned char wait);
unsigned char USART_Speed_Choise(void);
void Save_USART_SpeedNumber(unsigned char speed);
void Settings(void);
unsigned char Read_USART_SpeedNumber(void);
unsigned int GetStringLength(unsigned char *container);
unsigned int _GetStringLength_interr(unsigned char *container);
void Format_EEPROM_Memory(unsigned int first_cell, unsigned int last_cell, unsigned char interactive);
unsigned char GetDayOfWeekByDate(unsigned char _year, unsigned char _mounth, unsigned char _day);
void SendSMS(unsigned char *, unsigned char *);

/* ������� ��������� 1-wire */
unsigned char Init_1wire(unsigned char line) {
	unsigned char One = line;
	unsigned char Zero = One ^ 0b11111111;

	unsigned char b;
	b = 0;
	STATE_REG |= One;
	__delay_us(20);
	STATE_REG &= Zero;
	__delay_us(500);
	STATE_REG |= One;
	__delay_us(65);
	b = (PIN_REG & One) > 0;
	__delay_us(450);

	return !b;
}

void TX_1wire(unsigned char cmd, unsigned char line) {
	unsigned char One = line;
	unsigned char Zero = One ^ 0b11111111;

	unsigned char temp = 0;
	unsigned char i = 0;
	temp = cmd;
	for (i = 0; i < 8; i++) {
		if (temp & 0x01) {
			STATE_REG &= Zero;
			__delay_us(5);
			STATE_REG |= One;
			__delay_us(70);
		} else {
			STATE_REG &= Zero;
			__delay_us(70);
			STATE_REG |= One;
			__delay_us(5);
		}
		temp >>= 1;
	}
}

unsigned char RX_1wire(unsigned char line) {
	unsigned char One = line;
	unsigned char Zero = One ^ 0b11111111;

	unsigned char d = 0;
	for (unsigned char i = 0; i < 8; i++) {
		STATE_REG &= Zero;
		__delay_us(6);
		STATE_REG |= One;
		__delay_us(4);
		d >>= 1;
		if ((PIN_REG & One) > 0) {
			d |= 0x80;
		}
		__delay_us(60);
	}
	return d;
}//

unsigned char calc_crc(unsigned char *mas, unsigned char len) {
	unsigned char crc = 0;
	while (len--) {
		unsigned char dat = *mas++;
		for (unsigned char i = 0; i < 8; i++) { // ������� ����� � �����
			unsigned char fb = (crc ^ dat) & 1;
			crc >>= 1;
			dat >>= 1;
			if (fb) crc ^= 0x8c; // �������
		}
	}
	return crc;
}


void Run_getTemp(unsigned char line) {

	getTemp_flags.Init = 1;
	getTemp_flags.Send_Address = 1;
	getTemp_flags.CountAddressBytes = 0;
	getTemp_flags.SendConvertTemp = 1;
	getTemp_flags.PauseValue = 120;
	getTemp_flags.SendGetTemp = 1;
	getTemp_flags.ReadData = 1;
	getTemp_flags.CountDataBytes = 0;
	getTemp_flags.Error = 0;
	getTemp_flags.DataIsRead = 0;
	getTemp_flags.Line = line;

	getTemp_flags.ActiveProcess = 1;

}

void Run_getInit(unsigned char line) {

	getTemp_flags.Init = 1;
	getTemp_flags.Send_Address = 0;
	getTemp_flags.CountAddressBytes = 0;
	getTemp_flags.SendConvertTemp = 0;
	getTemp_flags.PauseValue = 0;
	getTemp_flags.SendGetTemp = 0;
	getTemp_flags.ReadData = 0;
	getTemp_flags.CountDataBytes = 0;
	getTemp_flags.Error = 0;
	getTemp_flags.DataIsRead = 0;
	getTemp_flags.Line = line;

	getTemp_flags.ActiveProcess = 1;

}

void get_temp_Async() {

	if (!getTemp_flags.ActiveProcess) {
		return;
	}

	unsigned char line = getTemp_flags.Line;

	// Step 1, 5
	if (getTemp_flags.Init) {
		if (Init_1wire(line)) {
			getTemp_flags.Init = 0;

			getTemp_flags.Send_Address = 1;
			getTemp_flags.CountAddressBytes = 0;
		} else {
			getTemp_flags.ActiveProcess = 0;
			getTemp_flags.Error = 1;
		}
	} else
		// Step 2, 6
		if (getTemp_flags.Send_Address) {
		if (Broadcasting_1_Wire) { //|| getTemp_flags.SendConvertTemp
			TX_1wire(0xCC, line);
			getTemp_flags.CountAddressBytes = 1;
			getTemp_flags.Send_Address = 0;
		} else if (getTemp_flags.CountAddressBytes < sizeof (DS_Address)) {
			if (getTemp_flags.CountAddressBytes == 0) {
				TX_1wire(0x55, line);
			}
			TX_1wire(DS_Address[getTemp_flags.CountAddressBytes], line);
			getTemp_flags.CountAddressBytes++;

			if (getTemp_flags.CountAddressBytes == sizeof (DS_Address)) {
				getTemp_flags.Send_Address = 0;
			}
		}
	} else
		// Step 3
		if (getTemp_flags.SendConvertTemp) {
		TX_1wire(0x44, line);
		getTemp_flags.SendConvertTemp = 0;

	} else
		// Step 4
		if (getTemp_flags.PauseValue > 0) {
		getTemp_flags.PauseValue--;
		if (getTemp_flags.PauseValue == 0) {
			getTemp_flags.Init = 1;
		}
	} else
		// Step 7
		if (getTemp_flags.SendGetTemp) {
		TX_1wire(0xBE, line);
		getTemp_flags.SendGetTemp = 0;
	} else
		// Step 8
		if (getTemp_flags.ReadData) {
		if (getTemp_flags.CountDataBytes < sizeof (DS_ReadData)) {
			for (unsigned char i = 0; i < 3 && getTemp_flags.CountDataBytes < sizeof (DS_ReadData); i++) {
				DS_ReadData[getTemp_flags.CountDataBytes] = RX_1wire(line);
				getTemp_flags.CountDataBytes++;
			}
			if (getTemp_flags.CountDataBytes == sizeof (DS_ReadData)) {
				if (DS_ReadData[sizeof (DS_ReadData) - 1] != calc_crc(DS_ReadData, sizeof (DS_ReadData) - 1)) {
					getTemp_flags.Error = 1;
				} else {

					unsigned char temp1 = DS_ReadData[0];
					unsigned char temp2 = DS_ReadData[1];

					temp_drob = temp1 & 0b00001111; //���������� ������� ����� � ��������� ����������
					temp_drob = ((temp_drob * 6) + 2) / 10; //��������� � ������ ������� �����
					temp1 >>= 4;
					temp_sign = temp2 & 0x80;
					temp2 <<= 4;
					temp2 &= 0b01110000;
					temp2 |= temp1;

					if (temp_sign) {
						temperature = 127 - temp2;
						temp_drob = 10 - temp_drob;
						if (temp_drob == 10) {
							temp_drob = 0;
							temperature++;
						}
					} else {
						temperature = temp2;
					}
				}
				getTemp_flags.ReadData = 0;
				getTemp_flags.ActiveProcess = 0;
				getTemp_flags.DataIsRead = 1;
			}
		}
	} else {
		getTemp_flags.ActiveProcess = 0;
	}
}

unsigned char getDigit(char line, char symbol) {
	return *(digits + 16 * line + symbol);
}

void setDigit(char line, char symbol, unsigned char data) {
	unsigned char value = data;
	unsigned char value0 = *(digits + 16 * line + symbol);
	(*(digitsAtrib + 16 * line + symbol)).modifided = (*(digitsAtrib + 16 * line + symbol)).modifided > 0 || (value != value0);
	flags.IsLCDModified = flags.IsLCDModified || (value != value0);
	*(digits + 16 * line + symbol) = value;
}

void setBlink(char line, char symbol, unsigned char value) {
	if (value != 0) {
		(*(digitsAtrib + 16 * line + symbol)).blink = 1;
	} else {
		(*(digitsAtrib + 16 * line + symbol)).blink = 0;
	}
	(*(digitsAtrib + 16 * line + symbol)).modifided = 1;
	flags.IsLCDModified = 1;
}

void clrInd() {
	for (char line = 0; line < 2; line++) {
		for (char symb = 0; symb < 16; symb++) {
			*(digits + line * 16 + symb) = ' ';
			(*(digitsAtrib + line * 16 + symb)).blink = 0;
			(*(digitsAtrib + line * 16 + symb)).modifided = 1;
		}
	}
	flags.IsLCDModified = 1;
}

void CalculateClockDelta(long int newTime) {
	if (lastClock != 0) {
		clock_delta = clock_delta + (newTime - Clock) / (newTime / 100 - lastClock / 100);
	}
	lastClock = newTime;
}

unsigned int GetUnicodeSymbol(unsigned char symb) {
	switch (symb) {
		case 0x00: return 0x0000; // #NULL
		case 0x01: return 0x0001; // #START OF HEADING
		case 0x02: return 0x0002; // #START OF TEXT
		case 0x03: return 0x0003; // #END OF TEXT
		case 0x04: return 0x0004; // #END OF TRANSMISSION
		case 0x05: return 0x0005; // #ENQUIRY
		case 0x06: return 0x0006; // #ACKNOWLEDGE
		case 0x07: return 0x0007; // #BELL
		case 0x08: return 0x0008; // #BACKSPACE
		case 0x09: return 0x0009; // #HORIZONTAL TABULATION
		case 0x0A: return 0x000A; // #LINE FEED
		case 0x0B: return 0x000B; // #VERTICAL TABULATION
		case 0x0C: return 0x000C; // #FORM FEED
		case 0x0D: return 0x000D; // #CARRIAGE RETURN
		case 0x0E: return 0x000E; // #SHIFT OUT
		case 0x0F: return 0x000F; // #SHIFT IN
		case 0x10: return 0x0010; // #DATA LINK ESCAPE
		case 0x11: return 0x0011; // #DEVICE CONTROL ONE
		case 0x12: return 0x0012; // #DEVICE CONTROL TWO
		case 0x13: return 0x0013; // #DEVICE CONTROL THREE
		case 0x14: return 0x0014; // #DEVICE CONTROL FOUR
		case 0x15: return 0x0015; // #NEGATIVE ACKNOWLEDGE
		case 0x16: return 0x0016; // #SYNCHRONOUS IDLE
		case 0x17: return 0x0017; // #END OF TRANSMISSION BLOCK
		case 0x18: return 0x0018; // #CANCEL
		case 0x19: return 0x0019; // #END OF MEDIUM
		case 0x1A: return 0x001A; // #SUBSTITUTE
		case 0x1B: return 0x001B; // #ESCAPE
		case 0x1C: return 0x001C; // #FILE SEPARATOR
		case 0x1D: return 0x001D; // #GROUP SEPARATOR
		case 0x1E: return 0x001E; // #RECORD SEPARATOR
		case 0x1F: return 0x001F; // #UNIT SEPARATOR
		case 0x20: return 0x0020; // #SPACE
		case 0x21: return 0x0021; // #EXCLAMATION MARK
		case 0x22: return 0x0022; // #QUOTATION MARK
		case 0x23: return 0x0023; // #NUMBER SIGN
		case 0x24: return 0x0024; // #DOLLAR SIGN
		case 0x25: return 0x0025; // #PERCENT SIGN
		case 0x26: return 0x0026; // #AMPERSAND
		case 0x27: return 0x0027; // #APOSTROPHE
		case 0x28: return 0x0028; // #LEFT PARENTHESIS
		case 0x29: return 0x0029; // #RIGHT PARENTHESIS
		case 0x2A: return 0x002A; // #ASTERISK
		case 0x2B: return 0x002B; // #PLUS SIGN
		case 0x2C: return 0x002C; // #COMMA
		case 0x2D: return 0x002D; // #HYPHEN-MINUS
		case 0x2E: return 0x002E; // #FULL STOP
		case 0x2F: return 0x002F; // #SOLIDUS
		case 0x30: return 0x0030; // #DIGIT ZERO
		case 0x31: return 0x0031; // #DIGIT ONE
		case 0x32: return 0x0032; // #DIGIT TWO
		case 0x33: return 0x0033; // #DIGIT THREE
		case 0x34: return 0x0034; // #DIGIT FOUR
		case 0x35: return 0x0035; // #DIGIT FIVE
		case 0x36: return 0x0036; // #DIGIT SIX
		case 0x37: return 0x0037; // #DIGIT SEVEN
		case 0x38: return 0x0038; // #DIGIT EIGHT
		case 0x39: return 0x0039; // #DIGIT NINE
		case 0x3A: return 0x003A; // #COLON
		case 0x3B: return 0x003B; // #SEMICOLON
		case 0x3C: return 0x003C; // #LESS-THAN SIGN
		case 0x3D: return 0x003D; // #EQUALS SIGN
		case 0x3E: return 0x003E; // #GREATER-THAN SIGN
		case 0x3F: return 0x003F; // #QUESTION MARK
		case 0x40: return 0x0040; // #COMMERCIAL AT
		case 0x41: return 0x0041; // #LATIN CAPITAL LETTER A
		case 0x42: return 0x0042; // #LATIN CAPITAL LETTER B
		case 0x43: return 0x0043; // #LATIN CAPITAL LETTER C
		case 0x44: return 0x0044; // #LATIN CAPITAL LETTER D
		case 0x45: return 0x0045; // #LATIN CAPITAL LETTER E
		case 0x46: return 0x0046; // #LATIN CAPITAL LETTER F
		case 0x47: return 0x0047; // #LATIN CAPITAL LETTER G
		case 0x48: return 0x0048; // #LATIN CAPITAL LETTER H
		case 0x49: return 0x0049; // #LATIN CAPITAL LETTER I
		case 0x4A: return 0x004A; // #LATIN CAPITAL LETTER J
		case 0x4B: return 0x004B; // #LATIN CAPITAL LETTER K
		case 0x4C: return 0x004C; // #LATIN CAPITAL LETTER L
		case 0x4D: return 0x004D; // #LATIN CAPITAL LETTER M
		case 0x4E: return 0x004E; // #LATIN CAPITAL LETTER N
		case 0x4F: return 0x004F; // #LATIN CAPITAL LETTER O
		case 0x50: return 0x0050; // #LATIN CAPITAL LETTER P
		case 0x51: return 0x0051; // #LATIN CAPITAL LETTER Q
		case 0x52: return 0x0052; // #LATIN CAPITAL LETTER R
		case 0x53: return 0x0053; // #LATIN CAPITAL LETTER S
		case 0x54: return 0x0054; // #LATIN CAPITAL LETTER T
		case 0x55: return 0x0055; // #LATIN CAPITAL LETTER U
		case 0x56: return 0x0056; // #LATIN CAPITAL LETTER V
		case 0x57: return 0x0057; // #LATIN CAPITAL LETTER W
		case 0x58: return 0x0058; // #LATIN CAPITAL LETTER X
		case 0x59: return 0x0059; // #LATIN CAPITAL LETTER Y
		case 0x5A: return 0x005A; // #LATIN CAPITAL LETTER Z
		case 0x5B: return 0x005B; // #LEFT SQUARE BRACKET
		case 0x5C: return 0x005C; // #REVERSE SOLIDUS
		case 0x5D: return 0x005D; // #RIGHT SQUARE BRACKET
		case 0x5E: return 0x005E; // #CIRCUMFLEX ACCENT
		case 0x5F: return 0x005F; // #LOW LINE
		case 0x60: return 0x0060; // #GRAVE ACCENT
		case 0x61: return 0x0061; // #LATIN SMALL LETTER A
		case 0x62: return 0x0062; // #LATIN SMALL LETTER B
		case 0x63: return 0x0063; // #LATIN SMALL LETTER C
		case 0x64: return 0x0064; // #LATIN SMALL LETTER D
		case 0x65: return 0x0065; // #LATIN SMALL LETTER E
		case 0x66: return 0x0066; // #LATIN SMALL LETTER F
		case 0x67: return 0x0067; // #LATIN SMALL LETTER G
		case 0x68: return 0x0068; // #LATIN SMALL LETTER H
		case 0x69: return 0x0069; // #LATIN SMALL LETTER I
		case 0x6A: return 0x006A; // #LATIN SMALL LETTER J
		case 0x6B: return 0x006B; // #LATIN SMALL LETTER K
		case 0x6C: return 0x006C; // #LATIN SMALL LETTER L
		case 0x6D: return 0x006D; // #LATIN SMALL LETTER M
		case 0x6E: return 0x006E; // #LATIN SMALL LETTER N
		case 0x6F: return 0x006F; // #LATIN SMALL LETTER O
		case 0x70: return 0x0070; // #LATIN SMALL LETTER P
		case 0x71: return 0x0071; // #LATIN SMALL LETTER Q
		case 0x72: return 0x0072; // #LATIN SMALL LETTER R
		case 0x73: return 0x0073; // #LATIN SMALL LETTER S
		case 0x74: return 0x0074; // #LATIN SMALL LETTER T
		case 0x75: return 0x0075; // #LATIN SMALL LETTER U
		case 0x76: return 0x0076; // #LATIN SMALL LETTER V
		case 0x77: return 0x0077; // #LATIN SMALL LETTER W
		case 0x78: return 0x0078; // #LATIN SMALL LETTER X
		case 0x79: return 0x0079; // #LATIN SMALL LETTER Y
		case 0x7A: return 0x007A; // #LATIN SMALL LETTER Z
		case 0x7B: return 0x007B; // #LEFT CURLY BRACKET
		case 0x7C: return 0x007C; // #VERTICAL LINE
		case 0x7D: return 0x007D; // #RIGHT CURLY BRACKET
		case 0x7E: return 0x007E; // #TILDE
		case 0x7F: return 0x007F; // #DELETE
		case 0x80: return 0x0402; // #CYRILLIC CAPITAL LETTER DJE
		case 0x81: return 0x0403; // #CYRILLIC CAPITAL LETTER GJE
		case 0x82: return 0x201A; // #SINGLE LOW-9 QUOTATION MARK
		case 0x83: return 0x0453; // #CYRILLIC SMALL LETTER GJE
		case 0x84: return 0x201E; // #DOUBLE LOW-9 QUOTATION MARK
		case 0x85: return 0x2026; // #HORIZONTAL ELLIPSIS
		case 0x86: return 0x2020; // #DAGGER
		case 0x87: return 0x2021; // #DOUBLE DAGGER
		case 0x88: return 0x20AC; // #EURO SIGN
		case 0x89: return 0x2030; // #PER MILLE SIGN
		case 0x8A: return 0x0409; // #CYRILLIC CAPITAL LETTER LJE
		case 0x8B: return 0x2039; // #SINGLE LEFT-POINTING ANGLE QUOTATION MARK
		case 0x8C: return 0x040A; // #CYRILLIC CAPITAL LETTER NJE
		case 0x8D: return 0x040C; // #CYRILLIC CAPITAL LETTER KJE
		case 0x8E: return 0x040B; // #CYRILLIC CAPITAL LETTER TSHE
		case 0x8F: return 0x040F; // #CYRILLIC CAPITAL LETTER DZHE
		case 0x90: return 0x0452; // #CYRILLIC SMALL LETTER DJE
		case 0x91: return 0x2018; // #LEFT SINGLE QUOTATION MARK
		case 0x92: return 0x2019; // #RIGHT SINGLE QUOTATION MARK
		case 0x93: return 0x201C; // #LEFT DOUBLE QUOTATION MARK
		case 0x94: return 0x201D; // #RIGHT DOUBLE QUOTATION MARK
		case 0x95: return 0x2022; // #BULLET
		case 0x96: return 0x2013; // #EN DASH
		case 0x97: return 0x2014; // #EM DASH
		case 0x98: return 0x003F; // #UNDEFINED
		case 0x99: return 0x2122; // #TRADE MARK SIGN
		case 0x9A: return 0x0459; // #CYRILLIC SMALL LETTER LJE
		case 0x9B: return 0x203A; // #SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
		case 0x9C: return 0x045A; // #CYRILLIC SMALL LETTER NJE
		case 0x9D: return 0x045C; // #CYRILLIC SMALL LETTER KJE
		case 0x9E: return 0x045B; // #CYRILLIC SMALL LETTER TSHE
		case 0x9F: return 0x045F; // #CYRILLIC SMALL LETTER DZHE
		case 0xA0: return 0x00A0; // #NO-BREAK SPACE
		case 0xA1: return 0x040E; // #CYRILLIC CAPITAL LETTER SHORT U
		case 0xA2: return 0x045E; // #CYRILLIC SMALL LETTER SHORT U
		case 0xA3: return 0x0408; // #CYRILLIC CAPITAL LETTER JE
		case 0xA4: return 0x00A4; // #CURRENCY SIGN
		case 0xA5: return 0x0490; // #CYRILLIC CAPITAL LETTER GHE WITH UPTURN
		case 0xA6: return 0x00A6; // #BROKEN BAR
		case 0xA7: return 0x00A7; // #SECTION SIGN
		case 0xA8: return 0x0401; // #CYRILLIC CAPITAL LETTER IO
		case 0xA9: return 0x00A9; // #COPYRIGHT SIGN
		case 0xAA: return 0x0404; // #CYRILLIC CAPITAL LETTER UKRAINIAN IE
		case 0xAB: return 0x00AB; // #LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
		case 0xAC: return 0x00AC; // #NOT SIGN
		case 0xAD: return 0x00AD; // #SOFT HYPHEN
		case 0xAE: return 0x00AE; // #REGISTERED SIGN
		case 0xAF: return 0x0407; // #CYRILLIC CAPITAL LETTER YI
		case 0xB0: return 0x00B0; // #DEGREE SIGN
		case 0xB1: return 0x00B1; // #PLUS-MINUS SIGN
		case 0xB2: return 0x0406; // #CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
		case 0xB3: return 0x0456; // #CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
		case 0xB4: return 0x0491; // #CYRILLIC SMALL LETTER GHE WITH UPTURN
		case 0xB5: return 0x00B5; // #MICRO SIGN
		case 0xB6: return 0x00B6; // #PILCROW SIGN
		case 0xB7: return 0x00B7; // #MIDDLE DOT
		case 0xB8: return 0x0451; // #CYRILLIC SMALL LETTER IO
		case 0xB9: return 0x2116; // #NUMERO SIGN
		case 0xBA: return 0x0454; // #CYRILLIC SMALL LETTER UKRAINIAN IE
		case 0xBB: return 0x00BB; // #RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
		case 0xBC: return 0x0458; // #CYRILLIC SMALL LETTER JE
		case 0xBD: return 0x0405; // #CYRILLIC CAPITAL LETTER DZE
		case 0xBE: return 0x0455; // #CYRILLIC SMALL LETTER DZE
		case 0xBF: return 0x0457; // #CYRILLIC SMALL LETTER YI
		case 0xC0: return 0x0410; // #CYRILLIC CAPITAL LETTER A
		case 0xC1: return 0x0411; // #CYRILLIC CAPITAL LETTER BE
		case 0xC2: return 0x0412; // #CYRILLIC CAPITAL LETTER VE
		case 0xC3: return 0x0413; // #CYRILLIC CAPITAL LETTER GHE
		case 0xC4: return 0x0414; // #CYRILLIC CAPITAL LETTER DE
		case 0xC5: return 0x0415; // #CYRILLIC CAPITAL LETTER IE
		case 0xC6: return 0x0416; // #CYRILLIC CAPITAL LETTER ZHE
		case 0xC7: return 0x0417; // #CYRILLIC CAPITAL LETTER ZE
		case 0xC8: return 0x0418; // #CYRILLIC CAPITAL LETTER I
		case 0xC9: return 0x0419; // #CYRILLIC CAPITAL LETTER SHORT I
		case 0xCA: return 0x041A; // #CYRILLIC CAPITAL LETTER KA
		case 0xCB: return 0x041B; // #CYRILLIC CAPITAL LETTER EL
		case 0xCC: return 0x041C; // #CYRILLIC CAPITAL LETTER EM
		case 0xCD: return 0x041D; // #CYRILLIC CAPITAL LETTER EN
		case 0xCE: return 0x041E; // #CYRILLIC CAPITAL LETTER O
		case 0xCF: return 0x041F; // #CYRILLIC CAPITAL LETTER PE
		case 0xD0: return 0x0420; // #CYRILLIC CAPITAL LETTER ER
		case 0xD1: return 0x0421; // #CYRILLIC CAPITAL LETTER ES
		case 0xD2: return 0x0422; // #CYRILLIC CAPITAL LETTER TE
		case 0xD3: return 0x0423; // #CYRILLIC CAPITAL LETTER U
		case 0xD4: return 0x0424; // #CYRILLIC CAPITAL LETTER EF
		case 0xD5: return 0x0425; // #CYRILLIC CAPITAL LETTER HA
		case 0xD6: return 0x0426; // #CYRILLIC CAPITAL LETTER TSE
		case 0xD7: return 0x0427; // #CYRILLIC CAPITAL LETTER CHE
		case 0xD8: return 0x0428; // #CYRILLIC CAPITAL LETTER SHA
		case 0xD9: return 0x0429; // #CYRILLIC CAPITAL LETTER SHCHA
		case 0xDA: return 0x042A; // #CYRILLIC CAPITAL LETTER HARD SIGN
		case 0xDB: return 0x042B; // #CYRILLIC CAPITAL LETTER YERU
		case 0xDC: return 0x042C; // #CYRILLIC CAPITAL LETTER SOFT SIGN
		case 0xDD: return 0x042D; // #CYRILLIC CAPITAL LETTER E
		case 0xDE: return 0x042E; // #CYRILLIC CAPITAL LETTER YU
		case 0xDF: return 0x042F; // #CYRILLIC CAPITAL LETTER YA
		case 0xE0: return 0x0430; // #CYRILLIC SMALL LETTER A
		case 0xE1: return 0x0431; // #CYRILLIC SMALL LETTER BE
		case 0xE2: return 0x0432; // #CYRILLIC SMALL LETTER VE
		case 0xE3: return 0x0433; // #CYRILLIC SMALL LETTER GHE
		case 0xE4: return 0x0434; // #CYRILLIC SMALL LETTER DE
		case 0xE5: return 0x0435; // #CYRILLIC SMALL LETTER IE
		case 0xE6: return 0x0436; // #CYRILLIC SMALL LETTER ZHE
		case 0xE7: return 0x0437; // #CYRILLIC SMALL LETTER ZE
		case 0xE8: return 0x0438; // #CYRILLIC SMALL LETTER I
		case 0xE9: return 0x0439; // #CYRILLIC SMALL LETTER SHORT I
		case 0xEA: return 0x043A; // #CYRILLIC SMALL LETTER KA
		case 0xEB: return 0x043B; // #CYRILLIC SMALL LETTER EL
		case 0xEC: return 0x043C; // #CYRILLIC SMALL LETTER EM
		case 0xED: return 0x043D; // #CYRILLIC SMALL LETTER EN
		case 0xEE: return 0x043E; // #CYRILLIC SMALL LETTER O
		case 0xEF: return 0x043F; // #CYRILLIC SMALL LETTER PE
		case 0xF0: return 0x0440; // #CYRILLIC SMALL LETTER ER
		case 0xF1: return 0x0441; // #CYRILLIC SMALL LETTER ES
		case 0xF2: return 0x0442; // #CYRILLIC SMALL LETTER TE
		case 0xF3: return 0x0443; // #CYRILLIC SMALL LETTER U
		case 0xF4: return 0x0444; // #CYRILLIC SMALL LETTER EF
		case 0xF5: return 0x0445; // #CYRILLIC SMALL LETTER HA
		case 0xF6: return 0x0446; // #CYRILLIC SMALL LETTER TSE
		case 0xF7: return 0x0447; // #CYRILLIC SMALL LETTER CHE
		case 0xF8: return 0x0448; // #CYRILLIC SMALL LETTER SHA
		case 0xF9: return 0x0449; // #CYRILLIC SMALL LETTER SHCHA
		case 0xFA: return 0x044A; // #CYRILLIC SMALL LETTER HARD SIGN
		case 0xFB: return 0x044B; // #CYRILLIC SMALL LETTER YERU
		case 0xFC: return 0x044C; // #CYRILLIC SMALL LETTER SOFT SIGN
		case 0xFD: return 0x044D; // #CYRILLIC SMALL LETTER E
		case 0xFE: return 0x044E; // #CYRILLIC SMALL LETTER YU
		case 0xFF: return 0x044F; // #CYRILLIC SMALL LETTER YA
		default: return 0x003F;
	}
}

unsigned char GetTimeSource() {
	flags.TimeSource = EERD(TIME_SOURCE_CELL);
	return flags.TimeSource;
}

void SetTimeSource(unsigned char time_source) {
	EEWR(TIME_SOURCE_CELL, time_source);
	flags.TimeSource = time_source;
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
		case '�': return 0x2D;
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
		case '�': return 0x27;
		case '+': return 0x2B;
		case '<': return 0x3C;
		case '=': return 0x3D;
		case '>': return 0x3E;
		case '�': return 0xEF;
		case '�': return 0xC8;
		case '�': return 0xC9;
		case '\\':return 0x2F;
		case '~': return 0xE9;
			//	case '�': return 
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
		case '�': return 0xCC;
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
		case '�': return 0x41;
		case '�': return 0x61;
		case '�': return 0xA0;
		case '�': return 0xB2;
		case '�': return 0x42;
		case '�': return 0xB3;
		case '�': return 0xA1;
		case '�': return 0xB4;
		case '�': return 0xE0;
		case '�': return 0xE3;
		case '�': return 0x45;
		case '�': return 0x65;
		case '�': return 0xA2;
		case '�': return 0xB5;
		case '�': return 0xA3;
		case '�': return 0xB6;
		case '�': return 0xA4;
		case '�': return 0xB7;
		case '�': return 0xA5;
		case '�': return 0xB8;
		case '�': return 0xA6;
		case '�': return 0xB9;
		case '�': return 0x4B;
		case '�': return 0xBA;
		case '�': return 0xA7;
		case '�': return 0xBB;
		case '�': return 0xBC;
		case '�': return 0x4D;
		case '�': return 0x48;
		case '�': return 0xBD;
		case '�': return 0x4F;
		case '�': return 0x6F;
		case '�': return 0xA8;
		case '�': return 0xBE;
		case '�': return 0x50;
		case '�': return 0x70;
		case '�': return 0x43;
		case '�': return 0x63;
		case '�': return 0xBF;
		case '�': return 0x54;
		case '�': return 0xA9;
		case '�': return 0x79;
		case '�': return 0xAA;
		case '�': return 0xE4;
		case '�': return 0x58;
		case '�': return 0x78;
		case '�': return 0xE1;
		case '�': return 0xE5;
		case '�': return 0xAB;
		case '�': return 0xC0;
		case '�': return 0xAC;
		case '�': return 0xC1;
		case '�': return 0xE2;
		case '�': return 0xE6;
		case '�': return 0xAD;
		case '�': return 0xC2;
		case '�': return 0xAE;
		case '�': return 0xC3;
		case '�': return 0xC4;
		case '�': return 0xC4;
		case '�': return 0xAF;
		case '�': return 0xC5;
		case '�': return 0xB0;
		case '�': return 0xC6;
		case '�': return 0xB1;
		case '�': return 0xC7;
		case '\x07': return 0xED; //bell
		case '\x08': return 0xDD; //antenna

		default: return 0x3F; // '?'
	}
}

unsigned char ConvertCharToNumeric(unsigned char symbol) {
	switch (symbol) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		default: return 0;
	}
}

unsigned char CheckoutRegisterOfSymbol(unsigned char dig) {
	switch (dig) {
		case 'A': return 'a';
		case 'a': return 'A';
		case 'B': return 'b';
		case 'b': return 'B';
		case 'C': return 'c';
		case 'c': return 'C';
		case 'D': return 'd';
		case 'd': return 'D';
		case 'E': return 'e';
		case 'e': return 'E';
		case 'F': return 'f';
		case 'f': return 'F';
		case 'G': return 'g';
		case 'g': return 'G';
		case 'H': return 'h';
		case 'h': return 'H';
		case 'I': return 'i';
		case 'i': return 'I';
		case 'J': return 'j';
		case 'j': return 'J';
		case 'K': return 'k';
		case 'k': return 'K';
		case 'L': return 'l';
		case 'l': return 'L';
		case 'M': return 'm';
		case 'm': return 'M';
		case 'N': return 'n';
		case 'n': return 'N';
		case 'O': return 'o';
		case 'o': return 'O';
		case 'P': return 'p';
		case 'p': return 'P';
		case 'Q': return 'q';
		case 'q': return 'Q';
		case 'R': return 'r';
		case 'r': return 'R';
		case 'S': return 's';
		case 's': return 'S';
		case 'T': return 't';
		case 't': return 'T';
		case 'U': return 'u';
		case 'u': return 'U';
		case 'V': return 'v';
		case 'v': return 'V';
		case 'W': return 'w';
		case 'w': return 'W';
		case 'X': return 'x';
		case 'x': return 'X';
		case 'Y': return 'y';
		case 'y': return 'Y';
		case 'Z': return 'z';
		case 'z': return 'Z';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		case '�': return '�';
		default: return dig;
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

void NumericToString(long int n, unsigned char * mySring, unsigned char size) {
	if (mySring[size - 1] == '\0') {
		size--;
	}
	unsigned char displacement = 1;
	unsigned char negative = 0;
	if (n < 0) {
		negative = 1;
		n *= -1;
	}
	do {
		mySring[size - displacement] = getNumChar(n % 10);
		n /= 10;
		displacement++;
	} while ((size - (displacement - 1) > 0) && (n > 0));

	if (negative && (size - (displacement - 1) > 0)) {
		mySring[size - displacement] = '-';
	}
}

void NumericToHEXString(long int n, unsigned char * mySring, unsigned char size) {
	if (mySring[size - 1] == '\0') {
		size--;
	}
	unsigned char displacement = 1;
	do {
		mySring[size - displacement] = getNumChar(n % 16);
		n /= 16;
		displacement++;
	} while ((size - (displacement - 1) > 0) && (n > 0));
}

unsigned char *GetDayOfWeek(unsigned char day) {
	if (day == 1) {
		return "��";
	} else if (day == 2) {
		return "��";
	} else if (day == 3) {
		return "��";
	} else if (day == 4) {
		return "��";
	} else if (day == 5) {
		return "��";
	} else if (day == 6) {
		return "��";
	} else if (day == 7) {
		return "��";
	}
	return "??";
}

void TimeToInd() {

	if (flags.SignalsAreChanged) {
		flags.SignalsAreChanged = 0;
		clrInd();
	}

	if (KeyCode == 32 && (AdressOfNextStartCell != END_OF_CELLS || flags.DetailModeOfViewSheduler)) {
		KeyCode = 0;
		clrInd();
		flags.DetailModeOfViewSheduler = !flags.DetailModeOfViewSheduler;
	}
	if (KeyCode == 45 && AdressOfNextStartCell != END_OF_CELLS && flags.DetailModeOfViewSheduler) {
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

	if (AdressOfNextStartCell == END_OF_CELLS || !flags.DetailModeOfViewSheduler) {
		unsigned char SignalsFinal = CurrentSignals | SignalsForInd;
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
			OutputString(D, 0, 13, 1);
		}

		unsigned char TimeData [] = "--:--:--"; // ��:��:��
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


		if (current_showing == csTemperature) {
			static long int tClock = 0;
			long int ElapsedTime = Clock - tClock;
			if (ElapsedTime >= 200) {
				tClock = Clock;
				current_showing = csTime;
			}
		}

		SignalsFinal = CurrentSignals | SignalsForInd;
		if (current_showing == csTime) {
			OutputString(TimeData, 0, (SignalsFinal == 7) ? 4 : 5, 0);
		} else if (current_showing == csTemperature) {
			unsigned char tString [9];
			if (flags.IsTemperatureRead) {
				strcpy(tString, "    .  C");
				NumericToString(temp_drob, tString, 6);
				NumericToString(temperature, tString, 4);
			} else {
				strcpy(tString, "  ERROR ");
			}
			OutputString(tString, 0, (SignalsFinal == 7) ? 4 : 5, 0);
		}
		OutputString(GetDayOfWeek(day), 0, (SignalsFinal == 7) ? 1 : 2, 0);
		
		if(flags.GSM_Connected){
			char s [] = " ";
			s[0] = ANTENNA_SYMBOL;
			OutputString(s, 0, 0, 0);
		}
	}

	if (flags.LockSignals && !flags.DetailModeOfViewSheduler) {
		OutputString("���������� ����.", 1, 0, 0);
		return;
	}


	if (AdressOfNextStartCell != END_OF_CELLS) {

		unsigned char DataArray[] = EMPTY_STRING_16;
		DataArray[0] = BELL_SYMBOL;

		char line = 1;
		if (flags.DetailModeOfViewSheduler) {
			line = 0;
			DataArray[5] = '�';
		}

		unsigned long int Data = 0;
		unsigned char CellsData [CELL_CAPACITY];
		ReadDataOfCell(&CellsData, &Data, AdressOfNextStartCell);

		unsigned int TimeStart;
		unsigned int TimeStop;
		unsigned char Days;
		unsigned char Signals;
		ParseDataRecord(Data, &TimeStart, &TimeStop, &Days, &Signals);

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

		OutputString(DataArray, line, 0, 0);

		if (flags.DetailModeOfViewSheduler) {
			unsigned char DataArrayOfLine2[] = "�   �� --:--"; // �12   �� 12:30
			int j2 = sizeof (DataArrayOfLine2) - 1;
			DataArrayOfLine2[--j2] = getNumChar(TimeStop % 10);
			TimeStop /= 10;
			DataArrayOfLine2[--j2] = getNumChar(TimeStop % 6);
			TimeStop /= 6;
			j2--;
			DataArrayOfLine2[--j2] = getNumChar(TimeStop % 10);
			DataArrayOfLine2[--j2] = getNumChar(TimeStop / 10);

			unsigned char CellsNumber = AdressOfNextStartCell / CELL_CAPACITY + 1;
			do {
				char d = getNumChar(CellsNumber % 10);
				DataArrayOfLine2[2] = DataArrayOfLine2[1];
				DataArrayOfLine2[1] = d;
				CellsNumber /= 10;
			} while (CellsNumber > 0);

			OutputString(DataArrayOfLine2, 1, 0, 0);
		}
	} else {
		if (flags.DetailModeOfViewSheduler) {
			clrInd();
			flags.DetailModeOfViewSheduler = 0;
		}
		OutputString(EMPTY_STRING_16, 1, 0, 0);
	}
}

void EEWR(unsigned int adress, unsigned char data) {
	volatile unsigned char INTCON_BUP = INTCON;
	INTCONbits.GIEH = 0;
	INTCONbits.GIEL = 0;
	ClrWdt();
	EEADRH = adress >> 8;
	EEADR = adress % 256;
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
	EEADR = adress % 256;
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.RD = 1;
	EEDATA_BUP = EEDATA;
	INTCON = INTCON_BUP;
	return EEDATA_BUP;
}

void OutputInteractiveData(SymbolData *data[], unsigned char length, unsigned char start_symbol) {
	for (unsigned char i = 0; i < length; i++) {
		unsigned char s = start_symbol + (*(data[i])).index;
		unsigned char line = s / 16;
		if (line == 1) {
			s -= 16;
		}
		setDigit(line, s, (*(data[i])).symbol);
		setBlink(line, s, (*(data[i])).props.blink);
	}
}

void ReIndTimeEdit(TimeEditData *TimeData) {
	for (char i = 0; i < 10; i++) {
		if ((*((*TimeData).Date[i])).props.const_symbol == 0) {
			(*((*TimeData).Date[i])).symbol = getNumChar((*((*TimeData).Date[i])).number);
		}
		(*((*TimeData).Date[i])).index = i;
	}
	for (char i = 0; i < 5; i++) {
		if ((*((*TimeData).Time[i])).props.const_symbol == 0) {
			(*((*TimeData).Time[i])).symbol = getNumChar((*((*TimeData).Time[i])).number);
		}
		(*((*TimeData).Time[i])).index = i;
	}
	(*((*TimeData).WeekDay[0])).symbol = getNumChar((*((*TimeData).WeekDay[0])).number);
	(*((*TimeData).WeekDay[0])).index = 0;

	char n = 0;
	OutputInteractiveData((*TimeData).Date, 10, n);
	OutputInteractiveData((*TimeData).Time, 5, n + 4 + 16);
	OutputString(GetDayOfWeek((*((*TimeData).WeekDay[0])).number), 1, n + 1, 0);
	//OutputInteractiveData((*TimeData).WeekDay, 1, n + 1 + 16);

	unsigned char *sources_of_time [] = {
		"<\\>",
		"RTC",
		"GSM"
	};
	unsigned char value = (*TimeData).TimeSource;
	if (value != 0 && value != 1 && value != 2) {
		value = 0;
	}
	OutputString("Src:", 0, n + 12, 0);
	OutputString(sources_of_time[value], 1, n + 12, 0);
}

void InteractiveData_BlinkOFF(SymbolData * InteractiveData[], unsigned char leght) {
	for (char i = 0; i < leght; i++) {
		(*(*(InteractiveData + i))).props.blink = 0;
	}
}

void TimeData_BlinkOFF(TimeEditData * TimeData) {
	InteractiveData_BlinkOFF((*TimeData).Date, 10);
	InteractiveData_BlinkOFF((*TimeData).Time, 5);
	InteractiveData_BlinkOFF((*TimeData).WeekDay, 1);
}

void TimeData_INIT(TimeEditData * TimeData) {
	for (char i = 0; i < 10; i++) {
		(*((*TimeData).Date[i])).props.blink = 0;
		(*((*TimeData).Date[i])).props.const_symbol = 0;
	}
	for (char i = 0; i < 5; i++) {
		(*((*TimeData).Time[i])).props.blink = 0;
		(*((*TimeData).Time[i])).props.const_symbol = 0;
	}
	(*((*TimeData).WeekDay[0])).props.blink = 0;
	(*((*TimeData).WeekDay[0])).props.const_symbol = 0;

	(*((*TimeData).Time[2])).symbol = ':';
	(*((*TimeData).Time[2])).props.const_symbol = 1;
	(*((*TimeData).Date[5])).symbol = '/';
	(*((*TimeData).Date[5])).props.const_symbol = 1;
	(*((*TimeData).Date[2])).symbol = '/';
	(*((*TimeData).Date[2])).props.const_symbol = 1;

	(*TimeData).TimeSource = flags.TimeSource;
}

void TimeEdit() {

	clrInd();

	unsigned char time_is_changed = 0;

	long int temp = Clock / 6000;
	long int n = temp;

	TimeEditData TimeData;
	SymbolData _time[5];
	SymbolData _weekday[1];
	SymbolData _date[10];

	for (unsigned char i = 0; i < 5; i++) {
		TimeData.Time[i] = &(_time[i]);
	}
	for (unsigned char i = 0; i < 1; i++) {
		TimeData.WeekDay[i] = &(_weekday[i]);
	}
	for (unsigned char i = 0; i < 10; i++) {
		TimeData.Date[i] = &(_date[i]);
	}

	TimeData_INIT(&TimeData);


	(*(TimeData.Time[4])).number = temp % 10;
	temp /= 10;
	(*(TimeData.Time[3])).number = temp % 6;

	temp /= 6;
	temp %= 24;

	(*(TimeData.Time[1])).number = temp % 10;
	(*(TimeData.Time[0])).number = temp / 10;

	(*(TimeData.WeekDay[0])).number = 1 + n / 1440;

	(*(TimeData.Date[9])).number = cYears % 10;
	(*(TimeData.Date[8])).number = cYears / 10;
	(*(TimeData.Date[7])).symbol = '0';
	(*(TimeData.Date[7])).props.const_symbol = 1;
	(*(TimeData.Date[6])).symbol = '2';
	(*(TimeData.Date[6])).props.const_symbol = 1;

	(*(TimeData.Date[4])).number = cMonths % 10;
	(*(TimeData.Date[3])).number = cMonths / 10;

	(*(TimeData.Date[1])).number = cDays % 10;
	(*(TimeData.Date[0])).number = cDays / 10;


	SymbolData * symb_array [10];
	unsigned char symb_array_max_index = 9; // sizeof(symb_array) / sizeof(SymbolData);
	unsigned char p = 0;
	symb_array[p++] = TimeData.Date[0]; // day_H
	symb_array[p++] = TimeData.Date[1]; // day_L
	symb_array[p++] = TimeData.Date[3]; // month_H
	symb_array[p++] = TimeData.Date[4]; // month_L
	symb_array[p++] = TimeData.Date[8]; // year_H
	symb_array[p++] = TimeData.Date[9]; // year_L

	symb_array[p++] = TimeData.Time[0]; // hour_H
	symb_array[p++] = TimeData.Time[1]; // hour_L
	symb_array[p++] = TimeData.Time[3]; // minute_H
	symb_array[p++] = TimeData.Time[4]; // minute_L	

	char blinking = 6;
	char input = 255;
	while (1) {
		(*symb_array[blinking]).props.blink = 1;

		(*(TimeData.WeekDay[0])).number = GetDayOfWeekByDate(
				(*(TimeData.Date[8])).number * 10 + (*(TimeData.Date[9])).number,
				(*(TimeData.Date[3])).number * 10 + (*(TimeData.Date[4])).number,
				(*(TimeData.Date[0])).number * 10 + (*(TimeData.Date[1])).number
				);

		ReIndTimeEdit(&TimeData);

		input = entNum(0, 9);

		if (input != 255) {
			(*symb_array[blinking]).number = input;
			TimeData_BlinkOFF(&TimeData);
			blinking == symb_array_max_index ? blinking = 0 : blinking++;
			time_is_changed = 1;
		}

		if (KeyCode == 40) {
			KeyCode = 0;
			TimeData_BlinkOFF(&TimeData);
			blinking == symb_array_max_index ? blinking = 0 : blinking++;
		} else if (KeyCode == 44) {
			KeyCode = 0;
			if (TimeData.TimeSource != 0 && TimeData.TimeSource != 1) {
				TimeData.TimeSource = 0;
			} else {
				TimeData.TimeSource++;
			}
		} else if (KeyCode == 41) {
			KeyCode = 0;
			TimeData_BlinkOFF(&TimeData);
			blinking == 0 ? blinking = symb_array_max_index : blinking--;
		} else if (KeyCode == 42) {
			KeyCode = 0;
			unsigned char t = 0;
			if (time_is_changed) {
				do {
					t = (*symb_array[2]).number * 10 + (*symb_array[3]).number;
					if (t > 12) {
						TimeData_BlinkOFF(&TimeData);
						blinking = 2;
						break;
					}

					unsigned char month = t;

					unsigned char MonthDays [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
					t = (*symb_array[4]).number * 10 + (*symb_array[5]).number;
					if (t % 4 == 0) {
						MonthDays[1] = 29;
					}
					t = (*symb_array[0]).number * 10 + (*symb_array[1]).number;
					if (MonthDays[month - 1] < t) {
						(*symb_array[0]).number = MonthDays[month - 1] / 10;
						(*symb_array[1]).number = MonthDays[month - 1] % 10;
						TimeData_BlinkOFF(&TimeData);
						blinking = 0;
						break;
					}

					t = (*symb_array[6]).number * 10 + (*symb_array[7]).number;
					if (t > 23) {
						TimeData_BlinkOFF(&TimeData);
						blinking = 7;
						break;
					}

					t = (*symb_array[8]).number * 10 + (*symb_array[9]).number;
					if (t > 59) {
						TimeData_BlinkOFF(&TimeData);
						blinking = 9;
						break;
					}

					t = 0;

					Clock = ((long int) ((*(TimeData.WeekDay[0])).number - 1) * 86400
							+ (long int) ((*symb_array[6]).number * 10 + (*symb_array[7]).number) * 3600
							+ (long int) ((*symb_array[8]).number * 10 + (*symb_array[9]).number) * 60) * 100;

					cDays = (*symb_array[0]).number * 10 + (*symb_array[1]).number;
					cMonths = (*symb_array[2]).number * 10 + (*symb_array[3]).number;
					cYears = (*symb_array[4]).number * 10 + (*symb_array[5]).number;

					if (TimeData.TimeSource != flags.TimeSource) {
						SetTimeSource(TimeData.TimeSource);
					}

					WriteTime(Clock, cDays, cMonths, cYears);

				} while (0);

			}

			if (t == 0) {
				if (!time_is_changed && TimeData.TimeSource != flags.TimeSource) {
					SetTimeSource(TimeData.TimeSource);
				}
				clrInd();
				break;
			}
		} else if (KeyCode == 43) {
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

void FillArrayFromEEPROM(unsigned char *container, unsigned int adress_start, unsigned int quantity) {
	for (unsigned char i = 0; i < quantity; i++) {
		container[i] = EERD(adress_start + i);
	}
}

void FillPhonebookCellFromEEPROM(Phone *container, unsigned int adress_start) {
	FillArrayFromEEPROM((*container).name, adress_start, sizeof ((*container).name));
	adress_start += sizeof ((*container).name);
	FillArrayFromEEPROM((*container).phone, adress_start, sizeof ((*container).phone));
	adress_start += sizeof ((*container).phone);
	FillArrayFromEEPROM((*container).blocked, adress_start, sizeof ((*container).blocked));
	adress_start += sizeof ((*container).blocked);
	FillArrayFromEEPROM((*container).filled, adress_start, sizeof ((*container).filled));
}

void FillArrayFromEEPROM_interr(unsigned char * container, unsigned int adress_start, unsigned int quantity) {
	for (unsigned char i = 0; i < quantity; i++) {
		container[i] = EERD(adress_start + i);
	}
}

void WriteFourBytesEE(unsigned int adress, unsigned long int data) {
	for (int i = 0; i < 4; i++) {
		EEWR(adress + i, data % 256);
		data /= 256;
	}
}

void WriteArrayToEEPROM(unsigned char * container, unsigned int adress_start, unsigned int quantity) {
	for (unsigned char i = 0; i < quantity; i++) {
		EEWR(adress_start + i, container[i]);
	}
}

void WritePhonebookCellToEEPROM(Phone *container, unsigned int adress_start) {
	WriteArrayToEEPROM((*container).name, adress_start, sizeof ((*container).name));
	adress_start += sizeof ((*container).name);
	WriteArrayToEEPROM((*container).phone, adress_start, sizeof ((*container).phone));
	adress_start += sizeof ((*container).phone);
	WriteArrayToEEPROM((*container).blocked, adress_start, sizeof ((*container).blocked));
	adress_start += sizeof ((*container).blocked);
	WriteArrayToEEPROM((*container).filled, adress_start, sizeof ((*container).filled));
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
	unsigned char Smb = 0;
	for (int a = 8; a > 0; a--) {
		Smb *= 2;
		if (Symb % 2 == 1) {
			Smb++;
		}
		Symb /= 2;
	//	CLRWDT();
	}

	RS = 1;
	LATA = (0b11110000 & (Smb * 16)) / 4 | (flags.LCD_Light_On == 1 ? 0b00000010 : 0b00000000);
	E = 1;
	//CLRWDT();
	delay(10, 1);
	E = 0;
	RS = 0;
	//CLRWDT();
	delay(25, 5);

	RS = 1;
	LATA = (0b11110000 & Smb) / 4 | (flags.LCD_Light_On == 1 ? 0b00000010 : 0b00000000);
	E = 1;
	//CLRWDT();
	delay(10, 1);
	E = 0;
	RS = 0;
	//CLRWDT();
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
	unsigned char data_temp = 0;
	//CLRWDT();
	for (int a = 4; a > 0; a--) {
		data_temp *= 2;
		if (data % 2 == 1) {
			data_temp++;
		}
		data /= 2;
	//	CLRWDT();
	}
	LATA = (0b00111100 & (data_temp * 4)) | (flags.LCD_Light_On == 1 ? 0b00000010 : 0b00000000);
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
	LCD_Power_On_PIN = 0;
}

void lcd_on() {
	LCD_Power_On_PIN = 1;
	lcd_init();
	lcd_send_byte(0b00001100);
	flags.LCD_Power_On = 1;
	LCD_ON_TIMEOUT = LCD_ON_TIMEOUT_CONST;
}

void lcd_init() {
	unsigned char INTCON_BUF = INTCON;
	INTCONbits.GIEH = 0;
	INTCONbits.GIEL = 0;
	E = 0;
	RS = 0;
	// http://cxem.net/mcmc89.php
	__delay_ms(10);
	__delay_ms(10);
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

	INTCON = (INTCON_BUF | 0b11000000) | (INTCON & 0b00111111);
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
		case 0: return 39;
		case 1: return 43;
		case 2: return 42;
		case 3: return 41;
		case 4: return 40;
		case 5: return 45;
		case 6: return 33;
		case 7: return 36;
		case 8: return 31;
		case 9: return 32;
		case 10: return 35;
		case 11: return 34;
		case 12: return 38;
		case 13: return 30;
		case 14: return 44;
		case 15: return 37;
		default: return 0;
	}
}

unsigned int FindCell(unsigned int adressStart, char New, unsigned int previous) {

	if (adressStart == END_OF_CELLS) {
		adressStart = LAST_CELL;
	}
	unsigned int adress = adressStart;
	unsigned int adressNew = END_OF_CELLS;
	do {

		if (!previous) {
			adress == LAST_CELL ? adress = 0 : adress += CELL_CAPACITY;
		} else {
			adress == 0 ? adress = LAST_CELL : adress -= CELL_CAPACITY;
		}

		unsigned long int Data = 0;
		unsigned char CellsData [CELL_CAPACITY];
		ReadDataOfCell(&CellsData, &Data, adress);
		unsigned char CellIsEmpty = 1;
		for (unsigned char i = 0; i < CELL_CAPACITY; i++) {
			if (CellsData[i] != 0xFF) {
				CellIsEmpty = 0;
			}
		}

		if (New != 1 ? CellIsEmpty == 0 : CellIsEmpty == 1) {
			adressNew = adress;
			break;
		}
	} while (adress != adressStart);
	return adressNew;
}

unsigned int RefreshSchedulerIndicator(unsigned int adress, char New, unsigned int CopyFrom) {

	unsigned int cell = 0;

	unsigned long int Data = 0;
	unsigned char CellsData [CELL_CAPACITY];
	ReadDataOfCell(&CellsData, &Data, adress);
	unsigned char CellIsEmpty = 1;
	for (unsigned char i = 0; i < CELL_CAPACITY; i++) {
		if (CellsData[i] != 0xFF) {
			CellIsEmpty = 0;
		}
	}

	if (adress != END_OF_CELLS && (New != 1 ? CellIsEmpty == 0 : CellIsEmpty == 1)) {
		cell = adress / CELL_CAPACITY + 1;
	} else {
		adress = FindCell(adress, New, 0);
		if (adress != END_OF_CELLS) {
			cell = adress / CELL_CAPACITY + 1;
		}
	}

	if (cell != 0) {
		unsigned char num [] = "  ";
		NumericToString(cell, num, sizeof (num));
		OutputString(num, 0, 14, 0);
	} else {
		OutputString("--", 0, 14, 0);
	}

	if (New == 1) {
		if (CopyFrom != END_OF_CELLS) {
			OutputString("���������� �", 1, 0, 0);
			unsigned char num [] = "  ";
			NumericToString(1 + CopyFrom / CELL_CAPACITY, num, sizeof (num));
			OutputString(num, 1, 14, 0);
		}
		OutputString("����� ", 0, 0, 0);
	}
	OutputString("������ ", 0, 6, 0);

	return adress;
}

void ItemsToIndicator(unsigned char items, unsigned char max_i, unsigned char firstDigit) {
	for (char i = 1; i < max_i; i++) {
		items % 2 == 1 ? setDigit(0, 16 - (firstDigit - i), getNumChar(i)) : setDigit(0, 16 - (firstDigit - i), '-');
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

void ReadDataOfCell(unsigned char *CellsData, unsigned long int *Data, unsigned int TargetAdress) {
	FillArrayFromEEPROM(CellsData, TargetAdress, CELL_CAPACITY);
	for (int i = 3; i >= 0; i--) {
		*Data = *Data * 256 + CellsData[i];
	}
}

void ReadDataOfCell_interr(unsigned char *CellsData, unsigned long int *Data, unsigned int TargetAdress) {
	FillArrayFromEEPROM_interr(CellsData, TargetAdress, CELL_CAPACITY);
	for (int i = 3; i >= 0; i--) {
		*Data = *Data * 256 + CellsData[i];
	}
}

void WriteDataOfCell(unsigned char *CellsData, unsigned long int *Data, unsigned int TargetAdress) {
	for (unsigned char i = 0; i < 4; i++) {
		CellsData[i] = (unsigned char) (*Data % 256);
		*Data /= 256;
	}
	WriteArrayToEEPROM(CellsData, TargetAdress, CELL_CAPACITY);
}

void CleanStringArray(unsigned char *myArray, unsigned int size, unsigned char settingData) {
	for (unsigned int i = 0; i < size; i++) {
		*(myArray + i) = settingData;
	}
}

void _CleanStringArray_interrupt(unsigned char *myArray, unsigned int size, unsigned char settingData) {
	for (unsigned int i = 0; i < size; i++) {
		*(myArray + i) = settingData;
	}
}

void _CleanStringArray_interrupt2(unsigned char *myArray, unsigned int size, unsigned char settingData) {
	for (unsigned int i = 0; i < size; i++) {
		*(myArray + i) = settingData;
	}
}

void DeleteDataOfCell(unsigned int TargetAdress) {
	unsigned char CellsData [CELL_CAPACITY];
	CleanStringArray(CellsData, sizeof (CellsData), 0xFF);
	WriteArrayToEEPROM(CellsData, TargetAdress, CELL_CAPACITY);
}

unsigned char EditSchedule(unsigned int adress, unsigned int SourceOfRecord) {

	unsigned int TargetAdress;
	if (SourceOfRecord == END_OF_CELLS) {
		TargetAdress = adress;
	} else {
		TargetAdress = SourceOfRecord;
	}

	unsigned long int Data = 0;
	unsigned char CellsData [CELL_CAPACITY];
	ReadDataOfCell(&CellsData, &Data, TargetAdress);

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

	clrInd();

	char mode = 1;
	unsigned char key;

	SymbolData * TimesOutput[11];
	SymbolData _times[11];
	for (unsigned char i = 0; i < 11; i++) {
		_times[i].index = i;
		_times[i].number = 0;
		_times[i].symbol = '0';
		_times[i].props.blink = 0;
		_times[i].props.const_symbol = 0;
		TimesOutput[i] = &(_times[i]);
	}

	_times[2].symbol = ':';
	_times[2].props.const_symbol = 1;
	_times[5].symbol = '-';
	_times[5].props.const_symbol = 1;
	_times[8].symbol = ':';
	_times[8].props.const_symbol = 1;

	SymbolData * Times[8];
	unsigned char i = 0;
	Times[i++] = &(_times[0]);
	Times[i++] = &(_times[1]);
	Times[i++] = &(_times[3]);
	Times[i++] = &(_times[4]);
	Times[i++] = &(_times[6]);
	Times[i++] = &(_times[7]);
	Times[i++] = &(_times[9]);
	Times[i++] = &(_times[10]);


	while (1) {
		if (KeyCode == 43) { // Cancel
			KeyCode = 0;
			clrInd();
			return 0;
		} else if (KeyCode == 42) { // Enter
			KeyCode = 0;
			Data = (unsigned long int) Signals * 0x20000000 + (unsigned long int) Days * 0x400000 + (unsigned long int) TimeStop * 0x800 + (unsigned long int) TimeStart;
			WriteDataOfCell(&CellsData, &Data, adress);
			clrInd();
			return 1;
		}

		switch (mode) {
			case 1:
			{
				unsigned int temp;

				temp = TimeStop;
				_times[10].number = temp % 10;
				temp /= 10;
				_times[9].number = temp % 6;
				temp = temp / 6;
				_times[7].number = temp % 10;
				_times[6].number = temp / 10;

				temp = TimeStart;
				_times[4].number = temp % 10;
				temp /= 10;
				_times[3].number = temp % 6;
				temp = temp / 6;
				_times[1].number = temp % 10;
				_times[0].number = temp / 10;

				for (char i = 0; i < 8; i++) {
					(*(Times[i])).symbol = getNumChar((*(Times[i])).number);
				}
				OutputInteractiveData(TimesOutput, 11, 5);

				if (KeyCode == 45 || BeginEditTimes == 1) {
					KeyCode = 0;
					BeginEditTimes = 0;
					unsigned char blinking = 0;

					while (1) {

						InteractiveData_BlinkOFF(Times, 8);
						(*(Times[blinking])).props.blink = 1;
						for (char i = 0; i < 8; i++) {
							(*(Times[i])).symbol = getNumChar((*(Times[i])).number);
						}
						OutputInteractiveData(TimesOutput, 11, 5);

						if (KeyCode == 43) { // Cancel
							KeyCode = 0;
							clrInd();
							break;
						} else if (KeyCode == 42) { // Enter
							KeyCode = 0;

							unsigned int T[2];
							unsigned char OK = 1;
							for (char j = 0; j < 2 && OK; j++) {
								char f = j * 4;
								unsigned char HH = (*(Times[f + 0])).number;
								unsigned char HL = (*(Times[f + 1])).number;
								unsigned char MH = (*(Times[f + 2])).number;
								unsigned char ML = (*(Times[f + 3])).number;

								if (HH * 10 + HL > 23) {
									blinking = 0 + f;
									OK = 0;
									continue;
								}

								if (MH * 10 + ML > 59) {
									blinking = 2 + f;
									OK = 0;
									continue;
								}

								T[j] = HH * 600 + HL * 60 + MH * 10 + ML;
							}
							if (OK) {
								if (T[0] > T[1]) {
									for (char j = 0; j < 4; j++) {
										(*(Times[j + 4])).number = (*(Times[j])).number;
									}
									blinking = 4;
									OK = 0;
								}
							}
							if (OK) {
								InteractiveData_BlinkOFF(Times, 8);
								TimeStart = T[0];
								TimeStop = T[1];
								break;
							}
						} else if (KeyCode == 40) { // NextDig
							KeyCode = 0;
							if (blinking < 7) {
								blinking++;
							}
						} else if (KeyCode == 41) { //PrevDig
							KeyCode = 0;
							if (blinking > 0) {
								blinking--;
							}
						}

						key = entNum(0, 9);
						if (key != 255) {
							(*(Times[blinking])).number = key;
							if (blinking < 7) {
								blinking++;
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
			if (mode == 3) {
				mode = 1;
			} else {
				mode++;
			}
		} else if (KeyCode == 41) {
			KeyCode = 0;
			clrInd();
			if (mode == 1) {
				mode = 3;
			} else {
				mode--;
			}
		}
	}
	return 0;
}

void ShowSchedulerMenu(unsigned int NumberOfCells) {
	clrInd();

	unsigned char *menu[] = {
		"��������",
		"��������",
		"����������",
		"�������",
		"�������� ������",
		NULL
	};
	unsigned char item = ShowMenu(menu, 0);

	if (item == 1) { // ��������
		SchedulerNew(END_OF_CELLS);
	} else if (NumberOfCells != END_OF_CELLS && item == 2) { // ��������
		EditSchedule(NumberOfCells, END_OF_CELLS);
	} else if (NumberOfCells != END_OF_CELLS && item == 3) { // ����������
		SchedulerNew(NumberOfCells);
	} else if (NumberOfCells != END_OF_CELLS && item == 4) { // �������
		DeleteDataOfCell(NumberOfCells);
	} else if (item == 5) { // �������� ������
		if (Select_OK_NO("�������� ������ ������������?")) {
			OutputSystemMessage("�������� ������ ...");
			Format_EEPROM_Memory(0, END_OF_CELLS - 1, 0);
			OutputSystemMessage("������!");
		}
	}
}

void Scheduler(unsigned int StartFrom) {
	clrInd();
	volatile static unsigned int NumberOfCells = 0;
	if (StartFrom != END_OF_CELLS) {
		NumberOfCells = StartFrom;
	}

	while (1) {
		NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, 0, END_OF_CELLS);

		if (KeyCode == 45) { // Show menu
			KeyCode = 0;
			ShowSchedulerMenu(NumberOfCells);
		} else if (KeyCode == 40) { // Next
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 0, 0);
		} else if (KeyCode == 41) { // Prev
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 0, 1);
		} else if (KeyCode == 43) { // Exit
			KeyCode = 0;
			clrInd();
			return;
		} else if (KeyCode == 30) { // Adress = 0
			KeyCode = 0;
			NumberOfCells = 0;
		}
	}
}

void SchedulerNew(unsigned int SourceOfRecord) {
	clrInd();
	volatile static unsigned int NumberOfCells = 0;
	while (1) {
		NumberOfCells = RefreshSchedulerIndicator(NumberOfCells, 1, SourceOfRecord);
		if (NumberOfCells != END_OF_CELLS && KeyCode == 45) { // Edit
			KeyCode = 0;
			if (EditSchedule(NumberOfCells, SourceOfRecord) == 1) {
				NumberOfCells = FindCell(NumberOfCells, 1, 0);
			}
		} else if (KeyCode == 40) { // Next
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 1, 0);
		} else if (KeyCode == 41) { // Prev
			KeyCode = 0;
			NumberOfCells = FindCell(NumberOfCells, 1, 1);
		} else if (KeyCode == 43) { // Exit
			KeyCode = 0;
			clrInd();
			return;
		} else if (KeyCode == 30) { // Adress = 0
			KeyCode = 0;
			NumberOfCells = 0;
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
	//return 0;
	unsigned int adress = 0;
	unsigned int adressStart = END_OF_CELLS;
	unsigned char TimeIsFound = 0;
	unsigned long int LastFoundTimeStart;

	do {
		unsigned long int Data = 0;
		unsigned char CellsData [CELL_CAPACITY];
		ReadDataOfCell(&CellsData, &Data, adress);

		adress += CELL_CAPACITY;
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
				adressStart = adress - CELL_CAPACITY;
			}
		}
	} while (adress < END_OF_CELLS);

	if (TimeIsFound == 1) {
		*TimeFrom = LastFoundTimeStart;
	} else {
		*TimeFrom = 0;
	}
	return adressStart;
}

void Format_EEPROM_Memory(unsigned int first_cell, unsigned int last_cell, unsigned char interactive) {
	if (interactive != 1 || Select_OK_NO("�������� ������?") && Select_OK_NO("�������� ����������! ����������?")) {
		if (interactive == 1) {
			OutputSystemMessage("�������� ������ ...");
		}
		for (unsigned int i = first_cell; i < last_cell - 1; i++) {
			EEWR(i, 0xFF);
		}
		if (interactive == 1) {
			OutputSystemMessage("������!");
		}
	}
}

void SignalsOnOff() {
	if (!flags.LockSignals && CurrentReadingCell < END_OF_CELLS) {

		unsigned long int Data = 0;
		unsigned char CellsData [CELL_CAPACITY];
		ReadDataOfCell_interr(&CellsData, &Data, CurrentReadingCell);

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
				if (((ThisDay & Days) > 0) && (TimeStart <= ThisTime) && (TimeStop >= ThisTime)) {
					SignalsOut = Signals | SignalsOut;
				}
			}
		}
		CurrentReadingCell += CELL_CAPACITY;
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
			flags.SignalsAreChanged = 1;
		}
		CurrentSignalsData = Data & 0b11100000;
		PORTB = Data;
	}
}

void FillMinutes() {
	Minutes = Clock - 3000;
}

Phone *FindPhoneContactByNumber(unsigned char *number) {
	Phone container;

	unsigned char found = 0;

	for (unsigned char i = 0; i < PHONEBOOK_CAPACITY; i++) {
		FillPhonebookCellFromEEPROM(&container, PHONEBOOK_START_ADRESS + sizeof (container) * i);
		if (container.filled[0] != 1) {
			continue;
		}

		unsigned char end_of_phone = 0;
		while (container.phone[end_of_phone + 1] != '\0') {
			end_of_phone++;
		}

		for (unsigned char j = 0; j < 10 && (end_of_phone - j + 1) > 0; j++) {
			if (container.phone[end_of_phone - j] != *(number + 9 - j)) {
				found = 0;
				break;
			} else {
				found = 1;
			}
		}

		if (found) {
			break;
		}
	}

	if (found) {
		return &container;
	} else {
		return NULL;
	}
}

unsigned char *_getContainer_Clock_from_GSM() {
	CleanStringArray(Clock_from_GSM, sizeof (Clock_from_GSM), '\0');
	return Clock_from_GSM;
}

unsigned char *_getContainer_Incomming_Call_Data() {
	CleanStringArray(Incomming_Call_Data, sizeof (Incomming_Call_Data), '\0');
	return Incomming_Call_Data;
}

unsigned char *_getContainer_Incomming_DTMF_Data() {
	CleanStringArray(Incomming_DTMF_Data, sizeof (Incomming_DTMF_Data), '\0');
	return Incomming_DTMF_Data;
}

unsigned char GetDayOfWeekByDate(unsigned char _year, unsigned char _mounth, unsigned char _day) {
	const unsigned char century_number = 20;
	if (_mounth < 3) {
		_mounth += 12;
		_year--;
	}
	return ((_day + 13 * (_mounth + 1) / 5 + _year + _year / 4 - 2 * (century_number % 4)) + 5) % 7 + 1;
}

unsigned char GetNumericFromString(unsigned char *string, unsigned char index) {
	return ConvertCharToNumeric(*(string + index));
}

void ProcessIncommingUartData() {

	if (flags.UnprocessedIncommingUartData) {
		flags.UnprocessedIncommingUartData = 0;

		if (!flags.UsartExchangeEnabled) {
			return;
		}

		flags.IsUartOK = 0;

		if (_FindIncommingData_interrupt(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			flags.IsUartOK = 1;
		} else if (_FindIncommingData_interrupt("RING", NULL, LAST_INCOMMING_BUFF_INDEX)) {
			// do nothing
		} else if (_FindIncommingData_interrupt(StandardAnswer_CLOCK, _getContainer_Clock_from_GSM(), LAST_INCOMMING_BUFF_INDEX)) {

			// +CCLK: "16/05/15,22:37:52+03"

			cYears = GetNumericFromString(Clock_from_GSM, 8) * 10 + GetNumericFromString(Clock_from_GSM, 9);
			cMonths = GetNumericFromString(Clock_from_GSM, 11) * 10 + GetNumericFromString(Clock_from_GSM, 12);
			cDays = GetNumericFromString(Clock_from_GSM, 14) * 10 + GetNumericFromString(Clock_from_GSM, 15);
			long int tClock = ((long int) (GetDayOfWeekByDate(cYears, cMonths, cDays) - 1) * 86400
					+ (long int) (GetNumericFromString(Clock_from_GSM, 17) * 10 + GetNumericFromString(Clock_from_GSM, 18)) * 3600
					+ (long int) (GetNumericFromString(Clock_from_GSM, 20) * 10 + GetNumericFromString(Clock_from_GSM, 21)) * 60
					+ (long int) (GetNumericFromString(Clock_from_GSM, 23) * 10 + GetNumericFromString(Clock_from_GSM, 24))) * 100;

			CalculateClockDelta(tClock);
			Clock = tClock;

		} else if (_FindIncommingData_interrupt(StandardAnswer_INCCALL, _getContainer_Incomming_Call_Data(), LAST_INCOMMING_BUFF_INDEX)) {
			unsigned char number [11];
			_CleanStringArray_interrupt(number, sizeof (number), '\0');

			for (unsigned char i = 0; i < sizeof (number) - 1; i++) {
				number[i] = Incomming_Call_Data[i + 11];
			}
			Phone *contact = FindPhoneContactByNumber(number);
			if (contact == NULL) {
				SendCommandToUSART("ATH0", 0);
				flags.ActiveCall = 0;
				
				unsigned char msg [33];
				_CleanStringArray_interrupt(msg, sizeof (msg), '\0');
				strcat(msg, "�����������        ");
				strcat(msg, number);
				OutputSystemMessage(msg);
			} else {
				SendCommandToUSART("ATA", 1);
				while (!_FindIncommingData_interrupt(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX));
				flags.ActiveCall = 1;
				flags.IncommingCall = 1;
				strcpy(active_phone, (*contact).phone);
				SendCommandToUSART("AT+DDET=1", 1);
				flags.RemoteControlIsEnabled = 0;
			}
			
			if (flags.LCD_Power_On == 0) {
				lcd_on();
				flags.LCD_Light_On = 1;
				LCD_ON_TIMEOUT = LCD_ON_TIMEOUT_CONST / 10;
			}
		} else if (_FindIncommingData_interrupt(StandardAnswer_DTMF, _getContainer_Incomming_DTMF_Data(), LAST_INCOMMING_BUFF_INDEX)) {
			
			DTMF_Symbol = Incomming_DTMF_Data[sizeof (Incomming_DTMF_Data) - 2];
			if (flags.RemoteControlIsEnabled) {
				unsigned char answerString [] = "AT+VTS=\" \"";
				answerString[8] = DTMF_Symbol;
				_CleanStringArray_interrupt(IncommingBuffer[LAST_INCOMMING_BUFF_INDEX], BUFFER_STRING_LENGTH, '\0');
				SendCommandToUSART(answerString, 1);
				while (!_FindIncommingData_interrupt(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX));
				
				if (DTMF_Symbol == '4') {
					CurrentSignals = 0;
					flags.SignalsAreChanged = 1;
				} else if (DTMF_Symbol == '6') {
					CurrentSignals &= 0b00000010;
					flags.SignalsAreChanged = 1;
				} else if (DTMF_Symbol == '1') {
					CurrentSignals = CurrentSignals | 0b00000001;
					flags.SignalsAreChanged = 1;
				} else if (DTMF_Symbol == '2') {
					CurrentSignals = CurrentSignals | 0b00000010;
					flags.SignalsAreChanged = 1;
				} else if (DTMF_Symbol == '3') {
					CurrentSignals = CurrentSignals | 0b00000100;
					flags.SignalsAreChanged = 1;
				} else if (DTMF_Symbol == '0') {
					flags.LockSignals = 1;
					CurrentSignals = 0;
					EEWR(LOCK_SIGNALS_FLAG_CELL, flags.LockSignals);
					flags.SignalsAreChanged = 1;
				} else if (DTMF_Symbol == '5') {
					flags.LockSignals = 0;
					flags.SignalsAreChanged = 1;
					EEWR(LOCK_SIGNALS_FLAG_CELL, flags.LockSignals);
				} else if (DTMF_Symbol == '#') {
					flags.StatusIsRequested = 1;
					flags.ActiveCall = 0;
					_CleanStringArray_interrupt(IncommingBuffer[LAST_INCOMMING_BUFF_INDEX], BUFFER_STRING_LENGTH, '\0');
					SendCommandToUSART("ATH0", 1);
					while (!_FindIncommingData_interrupt(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX));
					flags.RemoteControlIsEnabled = 0;
				}
			} else if (DTMF_Symbol == '*') {
				flags.RemoteControlIsEnabled = 1;
			}

		} else if (_FindIncommingData_interrupt(StandardAnswer_NO_CARRIER, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			flags.ActiveCall = 0;
			flags.RemoteControlIsEnabled = 0;

		} else if (_FindIncommingData_interrupt(StandardAnswer_BUSY, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			flags.ActiveCall = 0;
			flags.RemoteControlIsEnabled = 0;

		} else if (_FindIncommingData_interrupt(StandardAnswer_NO_ANSWER, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			flags.ActiveCall = 0;
			flags.RemoteControlIsEnabled = 0;

		} else if (_FindIncommingData_interrupt(StandardAnswer_NO_DIALTONE, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			flags.ActiveCall = 0;
			flags.RemoteControlIsEnabled = 0;

		} else if (_FindIncommingData_interrupt(StandardAnswer_ERROR, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			OutputSystemMessage("������ GSM");

		} else if (_FindIncommingData_interrupt(StandardAnswer_NORMAL_POWER_DOWN, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			flags.ActiveCall = 0;
			flags.RemoteControlIsEnabled = 0;
			flags.GSM_Connected = 0;
		}

	}
}

void AddByteToUSARTbuff(unsigned char byte) {

	static struct {
		unsigned is_r : 1;
		unsigned is_n : 1;
		unsigned is_rn : 1;
	} lastdata;

	if (byte == '\r') {
		if (!lastdata.is_rn) {
			lastdata.is_r = 1;
		}
	} else if (byte == '\n' && lastdata.is_r) {
		if (!lastdata.is_rn) {
			lastdata.is_n = 1;
		}
	} else {
		if (lastdata.is_r || lastdata.is_n) {
			lastdata.is_r = 0;
			lastdata.is_n = 0;
		}
		if (byte != '\n') {
			lastdata.is_rn = 0;
			for (unsigned int i = 0; i < BUFFER_STRING_LENGTH - 2; i++) {
				*(*(IncommingBuffer + MAX_INCOMMING_BUFF_INDEX) + i) = *(*(IncommingBuffer + MAX_INCOMMING_BUFF_INDEX) + i + 1);
			}
			*(*(IncommingBuffer + MAX_INCOMMING_BUFF_INDEX) + BUFFER_STRING_LENGTH - 2) = byte;
		}
	}

	if (lastdata.is_r && lastdata.is_n) {
		lastdata.is_r = 0;
		lastdata.is_n = 0;

		lastdata.is_rn = 1;

		unsigned char *buff = IncommingBuffer[0];
		for (unsigned char i = 0; i < MAX_INCOMMING_BUFF_INDEX; i++) {
			IncommingBuffer[i] = IncommingBuffer[i + 1];
		}
		IncommingBuffer[MAX_INCOMMING_BUFF_INDEX] = buff;
		_CleanStringArray_interrupt2(IncommingBuffer[MAX_INCOMMING_BUFF_INDEX], BUFFER_STRING_LENGTH, '\0');

		flags.UnprocessedIncommingUartData = 1;
	}
}

void _load_TXREG() {
	if (OutcommingBuffer == NULL && TXSTAbits.TRMT) {
		TXSTAbits.TXEN = 0;
		return;
	}
	if ((*OutcommingBuffer) == '\0') {
		while (!TXSTAbits.TRMT);
		TXREG = 0x0D;
		TXREG = 0x0A;
		OutcommingBuffer = NULL;
	} else if (OutcommingBuffer != NULL) {
		TXREG = *OutcommingBuffer++;
	}
}

void interrupt high_priority F_h() {
	
	if (PIR1bits.RCIF == 1) {
		AddByteToUSARTbuff(RCREG);
	} else if (PIR1bits.TXIF == 1) {
		_load_TXREG();
	} else if (TMR1IF) {
		_systemCounter++;
		TMR1IF = 0;
		SignalsOnOff();
		Clock > 60479900 ? Clock = 0 : Clock++;
		static unsigned char dddd = 0;
		if (dddd == 99) {
			Clock += clock_delta;
			dddd = 0;
		} else {
			dddd++;
		}
		char MinuteAgo = (Clock - Minutes > 6000);
		if (MinuteAgo > 0) {
			FillMinutes();
			flags.RelevanceOfNextStartCell = 0;
		}
	}
}

unsigned long int getSystemTimePoint() {
	return _systemCounter;
}

unsigned char testTimePoint(unsigned long int point, unsigned long int value) {
	unsigned long int _point = getSystemTimePoint();
	if (_point < point) {
		_point = _point + (0xFFFFFFFF - point);
	} else {
		_point = _point - point;
	}
	return _point >= value;
}

void system_BlinkReset(unsigned char val) {
	flags.IsLCDModified = 1;
	GlobalBlinkCycleTime = 100;
	flags.GlobalBlink = val;
}

void ProcessSystemMessageShowing() {
	if (flags.LCD_Power_On && flags.UnreadSystemMessage) {
		KeyCode = 0;
		flags.UnreadSystemMessage = 0;
	}
}

void interrupt low_priority F_l() {
	ClrWdt();
	if (TMR0IF) {
		TMR0IF = 0;

		/*	static long int tClock = 0;
			unsigned char _5seconds = 0;
			long int ElapsedTime = Clock - tClock;
			if(ElapsedTime >= 500){
				tClock = Clock;
				_5seconds = 1;
			}
		 */
		unsigned char *digs;
		digits_atributes *digsAtrib;

		if (flags.UnreadSystemMessage) {
			digs = digits_system_message;
			digsAtrib = digitsAtrib_system_message;
		} else {
			digs = digits;
			digsAtrib = digitsAtrib;
		}

		if (flags.LCD_Power_On && (GlobalBlinkCycleTime == 100 || flags.IsLCDModified)) {
			for (char line = 0; line < 2; line++) {
				for (int symbol = 0; symbol < 16; symbol++) {
					if (GlobalBlinkCycleTime == 20 || (*(digsAtrib + 16 * line + symbol)).modifided > 0) {
						unsigned char digit = *(digs + 16 * line + symbol);
						lcd_send_byte((line * 0x40 + symbol) | 0b10000000); //SetDDRAM
						if (!((*(digsAtrib + 16 * line + symbol)).blink == 0 || (*(digsAtrib + 16 * line + symbol)).blink == 1 && flags.GlobalBlink)) {
							digit = '_';
						}
						SendSymbolToLCD(getLcdCodeOfChar(digit));
						(*(digsAtrib + 16 * line + symbol)).modifided = 0;
					}
				}
			}
			flags.IsLCDModified = 0;
		}

		if (GlobalBlinkCycleTime > 0) {
			GlobalBlinkCycleTime--;
		} else {
			flags.IsLCDModified = 1;
			GlobalBlinkCycleTime = 20;
			flags.GlobalBlink = !flags.GlobalBlink;

			ReadTime();
		}

		static unsigned char ButtonPressTimeOut;

		static unsigned char KeyIndex;
		static unsigned char PressedKeyIndex = 0xFF;

		if (ButtonPressTimeOut > 0) {
			ButtonPressTimeOut--;
		} else {
			KeyCode = 0;
			if (RB0) {
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

				LATB = (LATB & 0b11100000) | (KeyIndex * 2);
				PressedKeyIndex = 0xFF;
				if (LCD_ON_TIMEOUT > 0) {
					LCD_ON_TIMEOUT--;
				}
			} else {
				if (PressedKeyIndex == 0xFF) {
					PressedKeyIndex = KeyIndex;
					KeyCode = ConvKeyNum(PressedKeyIndex);
					ButtonPressTimeOut = 1;
					LCD_ON_TIMEOUT = LCD_ON_TIMEOUT_CONST; // 3 min
					ProcessSystemMessageShowing();
				}
			}
		}

		ProcessIncommingUartData();

		if(flags.ActiveCall == 0) {
			flags.IncommingCall = 0;
		}
		
		get_temp_Async();
		
		COUNT++;
	}
}

void Init_USART(unsigned char SpeedNumber) {

	unsigned int _value = 0;
	if (SpeedNumber == 1) { // 1200
		_value = 8332;
	} else if (SpeedNumber == 2) { // 2400
		_value = 4165;
	} else if (SpeedNumber == 4) { // 19200
		_value = 520;
	} else if (SpeedNumber == 5) { // 57600
		_value = 172;
	} else { // 9600
		_value = 1040;
	}

	TRISCbits.RC6 = 1; //TX pin set as output
	TRISCbits.RC7 = 1; //RX pin set as input
	BAUDCONbits.BRG16 = 1;
	TXSTAbits.BRGH = 1;
	SPBRG = (unsigned char) (_value % 256);
	SPBRGH = (unsigned char) (_value / 256);

	IPR1bits.RCIP = 1; // ������� ��������� ���������� �� ��������� USART
	IPR1bits.TXIP = 1; // ������� ��������� ���������� �� ����������� USART

	TXSTAbits.SYNC = 0; // ����������� �����
	RCSTAbits.SPEN = 1; // uart on
	PIE1bits.RCIE = 1; // ���������� �� ������ ��������
	PIE1bits.TXIE = 1; // ���������� �� �������� ��������

	RCSTAbits.CREN = 1; // receiving enabled

}

void main() {

	flags.GSM_Connected = 0;

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

	CurrentSignals = 0;

	clrInd();

	clock_delta = 31;

	T1CONbits.T1CKPS = 1;

	RCONbits.IPEN = 1; // ��������� ��������������� ����������
	INTCONbits.PEIE = 1; // ������������ ���������� ���������

	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;

	T2CONbits.TMR2ON = 1;

	T1CONbits.RD16 = 1;
	TMR1 = 51200;
	T1CONbits.TMR1ON = 1;
	PIE1bits.TMR1IE = 1;
	IPR1bits.TMR1IP = 1;

	T0CON = 0b10000001; // timer0 is ON, 16 bit, prescaler 1:4
	TMR0IE = 1;

	ADCON1 = 0b00001111;
	TRISA = 0b00000000;
	TRISB = 0b00000001;
	TRISC = 0b00100000;

	Clock = ((long int) cMinutes * 60
			+ (long int) cHours * 3600
			+ ((long int) cWeekDay - 1) * 86400) * 100;

	LATA = 0b00000000;
	LATB = 0b00000000;
	LATC = 0b00000000;
	T2CON = 0b00100100;

	flags.LockSignals = EERD(LOCK_SIGNALS_FLAG_CELL);

	unsigned char i = 0;
	/*
	IncommingBuffer[i++] = buff_string_0;
	IncommingBuffer[i++] = buff_string_1;
	IncommingBuffer[i++] = buff_string_2;
	IncommingBuffer[i++] = buff_string_3;
	IncommingBuffer[i++] = buff_string_4;
	IncommingBuffer[i++] = buff_string_5;
	IncommingBuffer[i++] = buff_string_6;
	IncommingBuffer[i++] = buff_string_7;
	 */
	IncommingBuffer[i++] = buff_string_8;
	IncommingBuffer[i++] = buff_string_9;
	IncommingBuffer[i++] = buff_string_10;
	IncommingBuffer[i++] = buff_string_11;

	for (unsigned char i = 0; i < INCOMMING_BUFF_LENGTH; i++) {
		CleanStringArray(IncommingBuffer[i], BUFFER_STRING_LENGTH, '\0');
	}

	GetTimeSource();
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

void OutputString(unsigned char *stringData, unsigned char line, unsigned char position, unsigned char transfer_line) {
	unsigned char i = 0;
	unsigned char cont = 1;
	while ((*(stringData + i)) != '\0' && cont == 1) {
		for (; line < 2; line++) {
			for (; (position < 16) && ((*(stringData + i)) != '\0');) {
				setDigit(line, position, *(stringData + i));
				i++;
				position++;
			}
			position = 0;
			if (!transfer_line) {
				cont = 0;
				break;
			}
		}
		line = 0;
	}
}

void OutputSystemMessage(unsigned char *stringData) {
	for (char line = 0; line < 2; line++) {
		for (char symb = 0; symb < 16; symb++) {
			*(digits_system_message + line * 16 + symb) = EMPTY_SYMBOL_VALUE;
			(*(digitsAtrib_system_message + line * 16 + symb)).blink = 0;
			(*(digitsAtrib_system_message + line * 16 + symb)).modifided = 1;
		}
	}
	unsigned int i = 0;
	while (*(stringData + i) != '\0') {
		for (unsigned char l = 0; l < 2; l++) {
			unsigned char s = 0;
			for (unsigned char position = 0; (s < 16) && (*(stringData + i) != '\0'); s++) {
				(*(digitsAtrib_system_message + 16 * l + s)).modifided = 1;
				*(digits_system_message + 16 * l + s) = *(stringData + i);
				i++;
			}
		}
	}
	flags.UnreadSystemMessage = 1;
	flags.IsLCDModified = 1;
}

void drowText(unsigned char *stringData, int startNum, int direction) {
	unsigned char srcLine, destLine;
	if (direction >= 0) {
		srcLine = 1;
		destLine = 0;
	} else {
		srcLine = 0;
		destLine = 1;
	}

	for (unsigned char symbol = 0; symbol < 16; symbol++) {
		*(digits + destLine * 16 + symbol) = *(digits + srcLine * 16 + symbol);
		(*(digitsAtrib + destLine * 16 + symbol)).blink = (*(digitsAtrib + srcLine * 16 + symbol)).blink;
		(*(digitsAtrib + destLine * 16 + symbol)).modifided = 1;
	}

	flags.IsLCDModified = 1;

	unsigned char cutedString[17] = EMPTY_STRING_16;
	for (unsigned char symbol = 0; symbol < 16; symbol++) {
		unsigned char a = stringData[symbol + startNum];
		if (a == '\0') {
			break;
		}
		cutedString[symbol] = a;
	}

	OutputString(cutedString, (direction >= 0 ? 1 : 0), 0, 1);
}

unsigned char FindIncommingData(unsigned char *regexp, unsigned char *container, int history_index) {

	unsigned char *pointer;
	unsigned char *end_of_regexp = GetStringLength(regexp) - 1 + regexp;

	unsigned int start_index, finish_index;
	if (history_index >= 0) {
		start_index = history_index;
		finish_index = history_index;
	} else {
		start_index = 0;
		finish_index = MAX_INCOMMING_BUFF_INDEX - 1;
	}

	for (unsigned int i = start_index; i <= finish_index; i++) {
		pointer = IncommingBuffer[i];
		unsigned int last_s, not_found = 0;
		for (unsigned int j = 0; end_of_regexp - j >= regexp && j < BUFFER_STRING_LENGTH - 1; j++) {
			unsigned int s0 = *(end_of_regexp - j);
			last_s = BUFFER_STRING_LENGTH - 2 - j;
			unsigned int s1 = *(pointer + last_s);
			if (s0 != s1 && s0 != '\x10') {
				not_found = 1;
				break;
			}
		}
		if (!not_found) { // if regexp is found to copy data to container
			if (container != NULL) {
				for (unsigned int a = 0; last_s < BUFFER_STRING_LENGTH - 1;) {
					*(container + a) = *(pointer + last_s);
					a++;
					last_s++;
				}
				CleanStringArray(pointer, BUFFER_STRING_LENGTH, '\0');
			}
			return 1;
		}
	}
	return 0;
}

unsigned int GetStringLength(unsigned char *container) {
	unsigned int i = 0;
	while (*(container) != '\0') {
		i++;
		container++;
	}
	return i;
}

unsigned int _GetStringLength_interr(unsigned char *container) {
	unsigned int i = 0;
	while (*(container) != '\0') {
		i++;
		container++;
	}
	return i;
}

unsigned char _FindIncommingData_interrupt(unsigned char *regexp, unsigned char *container, int history_index) {

	unsigned char *pointer;

	unsigned char *end_of_regexp = _GetStringLength_interr(regexp) - 1 + regexp;

	unsigned int start_index, finish_index;
	if (history_index >= 0) {
		start_index = history_index;
		finish_index = history_index;
	} else {
		start_index = 0;
		finish_index = LAST_INCOMMING_BUFF_INDEX;
	}

	for (unsigned int i = start_index; i <= finish_index; i++) {
		pointer = IncommingBuffer[i];
		unsigned int last_s, not_found = 0;
		for (unsigned int j = 0; end_of_regexp - j >= regexp && j < BUFFER_STRING_LENGTH - 1; j++) {
			unsigned int s0 = *(end_of_regexp - j);
			last_s = BUFFER_STRING_LENGTH - 2 - j;
			unsigned int s1 = *(pointer + last_s);
			if (s0 != s1 && s0 != '\x10') {
				not_found = 1;
				break;
			}
		}
		if (!not_found) { // if regexp is found copy data to container
			if (container != NULL) {
				for (unsigned int a = 0; last_s < BUFFER_STRING_LENGTH - 1;) {
					*(container + a) = *(pointer + last_s);
					a++;
					last_s++;
				}
				_CleanStringArray_interrupt(pointer, BUFFER_STRING_LENGTH, '\0');
			}
			return 1;
		}
	}
	return 0;
}

void _setInputTextProps(input_text_props *_props) {
	clrInd();

	input_text_props props;
	props.Aa = (*_props).Aa;
	props.Language = (*_props).Language;

	const unsigned char source_A [] = " �� �� ��";
	const unsigned char source_L [] = "English�������    123";
	unsigned char Aa [] = "   ";
	unsigned char Lang [] = "       ";
	while (1) {

		for (unsigned char i = 0; i < 3; i++) {
			Aa[i] = source_A[i + 3 * props.Aa];
		}

		for (unsigned char i = 0; i < 7; i++) {
			Lang[i] = source_L[i + 7 * props.Language];
		}

		OutputString(Aa, 0, 2, 0);
		OutputString(Lang, 0, 7, 0);

		if (KeyCode == 35) {
			KeyCode = 0;
			props.Aa < 2 ? props.Aa++ : props.Aa = 0;
		} else if (KeyCode == 36) {
			KeyCode = 0;
			props.Language < 2 ? props.Language++ : props.Language = 0;
		} else if (KeyCode == 38) {
			KeyCode = 0;
			props.Aa > 0 ? props.Aa-- : props.Aa = 2;
		} else if (KeyCode == 39) {
			KeyCode = 0;
			props.Language > 0 ? props.Language-- : props.Language = 2;
		} else if (KeyCode == 42) { // enter
			KeyCode = 0;
			(*_props).Aa = props.Aa;
			(*_props).Language = props.Language;
			clrInd();
			break;
		} else if (KeyCode == 43) { // cancel
			KeyCode = 0;
			clrInd();
			break;
		}
	}
}

unsigned char *InputText(unsigned char *text, input_text_props *_props, unsigned char capacity) {
	input_text_props props = {AA, English, No, No};

	if (_props != NULL) {
		props.Aa = (*_props).Aa;
		props.Language = (*_props).Language;
		props.NotClearInd = (*_props).NotClearInd;
		props.ReturnAfterInput = (*_props).ReturnAfterInput;
		props.BlockCharSet = (*_props).BlockCharSet;
		props.PhoneKeyboard = (*_props).PhoneKeyboard;
	}

	if (!props.NotClearInd) {
		clrInd();
	}

	unsigned char *symbols [30];

	unsigned char s_0 [] = " 0";
	symbols[0] = s_0;
	unsigned char s_1 [] = "1-=?+\"*#;:().,!<>~/";
	symbols[1] = s_1;
	unsigned char s_2 [] = "abc2";
	symbols[2] = s_2;
	unsigned char s_3 [] = "def3";
	symbols[3] = s_3;
	unsigned char s_4 [] = "ghi4";
	symbols[4] = s_4;
	unsigned char s_5 [] = "jkl5";
	symbols[5] = s_5;
	unsigned char s_6 [] = "mno6";
	symbols[6] = s_6;
	unsigned char s_7 [] = "pqrs7";
	symbols[7] = s_7;
	unsigned char s_8 [] = "tuv8";
	symbols[8] = s_8;
	unsigned char s_9 [] = "wxyz9";
	symbols[9] = s_9;

	symbols[10] = s_0;
	symbols[11] = s_1;
	unsigned char s_12 [] = "����2";
	symbols[12] = s_12;
	unsigned char s_13 [] = "����3";
	symbols[13] = s_13;
	unsigned char s_14 [] = "����4";
	symbols[14] = s_14;
	unsigned char s_15 [] = "����5";
	symbols[15] = s_15;
	unsigned char s_16 [] = "����6";
	symbols[16] = s_16;
	unsigned char s_17 [] = "����7";
	symbols[17] = s_17;
	unsigned char s_18 [] = "����8";
	symbols[18] = s_18;
	unsigned char s_19 [] = "����9";
	symbols[19] = s_19;

	unsigned char s_20 [] = "0+";
	symbols[20] = s_20;
	unsigned char s_21 [] = "1";
	symbols[21] = s_21;
	unsigned char s_22 [] = "2";
	symbols[22] = s_22;
	unsigned char s_23 [] = "3";
	symbols[23] = s_23;
	unsigned char s_24 [] = "4";
	symbols[24] = s_24;
	unsigned char s_25 [] = "5";
	symbols[25] = s_25;
	unsigned char s_26 [] = "6";
	symbols[26] = s_26;
	unsigned char s_27 [] = "7";
	symbols[27] = s_27;
	unsigned char s_28 [] = "8";
	symbols[28] = s_28;
	unsigned char s_29 [] = "9";
	symbols[29] = s_29;

	if (capacity == 0) {
		capacity = 31;
	} else {
		capacity--;
	}

	SymbolData _input_data [33];
	SymbolData * InputData [33];
	for (unsigned char i = 0; i < 33; i++) {
		_input_data[i].index = i;
		_input_data[i].props.blink = 0;
		_input_data[i].props.const_symbol = 1;
		_input_data[i].number = '\0';
		_input_data[i].symbol = ' ';
		InputData[i] = &(_input_data[i]);
	}
	_input_data[32].symbol = '\0';

	unsigned char active_cell = 0;
	unsigned char lastKeyCode = 0;
	unsigned long int time_point = 0;
	unsigned char *cur_char;

	if (text != NULL) {
		unsigned char i = 0;
		for (; *(text + i) != '\0'; i++) {
			_input_data[i].symbol = *(text + i);
			_input_data[i].number = 1;
			active_cell = i;
		}
		if (active_cell < capacity) {
			active_cell++;
		}
	}

	while (1) {
		if (lastKeyCode != 0 && (props.PhoneKeyboard || testTimePoint(time_point, 300))) {
			lastKeyCode = 0;
			if (active_cell < capacity && (*InputData[capacity]).number == '\0') {
				active_cell++;
			}
		}

		if (KeyCode >= 30 && KeyCode <= 39 || props.PhoneKeyboard && (KeyCode == 43 || KeyCode == 44)) {

			if (!props.PhoneKeyboard && lastKeyCode == KeyCode) {
				if (*(cur_char + 1) == '\0') {
					cur_char = symbols[((KeyCode - 30) + 10 * props.Language)];
				} else {
					cur_char++;
				}
			} else {
				if (KeyCode == 44) { // *
					cur_char = symbols[1] + 6;
				} else if (KeyCode == 43) { // #
					cur_char = symbols[1] + 7;
				} else {
					cur_char = symbols[((KeyCode - 30) + 10 * props.Language)];
				}
			}

			SymbolData *temp;
			if (lastKeyCode != KeyCode && (*InputData[capacity]).number == '\0') {
				if (active_cell < capacity && lastKeyCode != 0) {
					active_cell++;
				}
				temp = InputData[capacity];
				for (unsigned char i = capacity; i > active_cell; i--) {
					InputData[i] = InputData[i - 1];
					(*InputData[i]).index++;
				}
				InputData[active_cell] = temp;
			} else {
				temp = InputData[active_cell];
			}

			(*temp).symbol = *cur_char;
			if (props.Aa == 1 && active_cell == 0 || props.Aa == 2) {
				(*temp).symbol = CheckoutRegisterOfSymbol((*temp).symbol);
			}
			(*temp).index = active_cell;
			(*temp).number = 1;

			lastKeyCode = KeyCode;
			KeyCode = 0;
			system_BlinkReset(1);

			time_point = getSystemTimePoint();

			if (props.ReturnAfterInput) {
				if (_props != NULL) {
					(*_props).InputChar = (*temp).symbol;
				}
				unsigned char ret_string [33];
				unsigned char i = 0;
				for (; i < 33; i++) {
					if ((*InputData[i]).number == '\0') {
						break;
					}
					ret_string[i] = (*InputData[i]).symbol;
				}
				ret_string[i] = '\0';
				if (!props.NotClearInd) {
					clrInd();
				}
				return ret_string;
			}
		}
		InteractiveData_BlinkOFF(InputData, 32);
		(*InputData[active_cell]).props.blink = 1;

		OutputInteractiveData(InputData, 32, 0);

		if (KeyCode == 40) { // next
			KeyCode = 0;
			if (active_cell < capacity && (*InputData[active_cell]).number != '\0') {
				active_cell++;
				system_BlinkReset(0);

				lastKeyCode = 0;
			}
		} else if (KeyCode == 41) { // prev
			KeyCode = 0;
			if (active_cell > 0) {
				active_cell--;
				system_BlinkReset(0);

				lastKeyCode = 0;
			}
		} else if (KeyCode == 42) { // enter
			KeyCode = 0;
			(*_props).InputChar = NULL;
			unsigned char ret_string [33];
			unsigned char i = 0;
			for (; i < 33; i++) {
				if ((*InputData[i]).number == '\0') {
					break;
				}
				ret_string[i] = (*InputData[i]).symbol;
			}
			ret_string[i] = '\0';
			if (!props.NotClearInd) {
				clrInd();
			}
			return ret_string;
		} else if (!props.PhoneKeyboard && KeyCode == 43) { // cancel
			KeyCode = 0;
			if (!props.NotClearInd) {
				clrInd();
			}
			return NULL;
		} else if (!props.PhoneKeyboard && KeyCode == 44) { // backspace
			KeyCode = 0;
			if (lastKeyCode != 0) {
				active_cell++;
			}
			if (active_cell > 0) {
				SymbolData *temp = InputData[active_cell - 1];
				(*temp).number = '\0';
				(*temp).symbol = ' ';
				(*temp).index = 31;
				for (unsigned char i = active_cell - 1; i < 32; i++) {
					InputData[i] = InputData[i + 1];
					(*InputData[i]).index--;
				}
				InputData[31] = temp;
				active_cell--;

				lastKeyCode = 0;
			}
		} else if (KeyCode == 45) { // properties
			KeyCode = 0;
			if(props.PhoneKeyboard){
				if (!props.NotClearInd) {
					clrInd();
				}
				return NULL;
			} else {
				_setInputTextProps(&props);
			}
		}
	}
}

void SendCommandToUSART(unsigned char *command, unsigned char wait) {

	if (command != NULL) {
		OutcommingBuffer = command;
		TXSTAbits.TXEN = 1;
		if (wait) {
			while (OutcommingBuffer != NULL);
		}
	}

}

void WorkingWithGSM() {
	clrInd();

	unsigned char myOutputString [33];
	myOutputString[32] = '\0';
	static unsigned char line = LAST_INCOMMING_BUFF_INDEX;

	while (1) {

		unsigned char *p = IncommingBuffer[line] + BUFFER_STRING_LENGTH - 33;
		for (unsigned char i = 0; i < 32; i++) {
			*(myOutputString + i) = *p++;
			if (*(myOutputString + i) == '\0') {
				*(myOutputString + i) = EMPTY_SYMBOL_VALUE;
			}
		}

		OutputString(myOutputString, 0, 0, 1);

		if (KeyCode == 33) {
			KeyCode = 0;
			unsigned char speed = USART_Speed_Choise();
			if (speed != 0) {
				Save_USART_SpeedNumber(speed);
				Init_USART(speed);
			}
		} else if (KeyCode == 45) {
			KeyCode = 0;
			unsigned char *command = InputText(NULL, NULL, 0);
			SendCommandToUSART(command, 0);
		} else if (KeyCode == 40) {
			KeyCode = 0;
			if (line > 0) {
				line--;
			}
		} else if (KeyCode == 41) {
			KeyCode = 0;
			if (line < MAX_INCOMMING_BUFF_INDEX) {
				line++;
			}
		} else if (KeyCode == 43) {
			KeyCode = 0;
			clrInd();
			break;
		}
	}
}

void testSystemTimer() {
	clrInd();

	unsigned char string [] = EMPTY_STRING_16;
	while (1) {

		unsigned long int a = getSystemTimePoint();
		for (unsigned char i = 0; i < sizeof (string) - 1; i++) {
			string[i] = ' ';
		}
		NumericToString(a, string, sizeof (string));
		OutputString(string, 1, 0, 0);
		if (KeyCode == 43) {
			KeyCode = 0;
			clrInd();
			break;
		}
	}
}

void UserDelay(unsigned int value) {
	unsigned long int timestamp = getSystemTimePoint();
	while (!testTimePoint(timestamp, value));
}

unsigned char Init_GSM(unsigned char show) {

	do {
		flags.GSM_Connected = 0;

		Init_USART(Read_USART_SpeedNumber());

		if (show) {
			OutputSystemMessage("����� GSM-      ������...");
		}
		SendCommandToUSART("AT", 1);
		SendCommandToUSART("AT", 1);
		UserDelay(20);
		if (!FindIncommingData(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX)) {
			if (show) {
				UserDelay(200);
				OutputSystemMessage("GSM-������ ��   ������");
				if (FindIncommingData("> ", NULL, MAX_INCOMMING_BUFF_INDEX)) {
					SendCommandToUSART("\x1B", 1);
				}
			}
			break;
		}

		if (show) {
			OutputSystemMessage("��������� ������...");
		}
		unsigned long int tt = getSystemTimePoint();
		unsigned char result = 0;
		SendCommandToUSART("ATE0", 1);
		while (!(result = FindIncommingData(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX)) && !testTimePoint(tt, 50));
		if (!result) {
			OutputSystemMessage("��������� ������... ������");
			break;
		}
		if (show) {
			OutputSystemMessage("��������� ������... OK");
		}
		SendCommandToUSART("AT+CMGF=0", 1);
		while (!(result = FindIncommingData(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX)) && !testTimePoint(tt, 50));
		if (!result) {
			OutputSystemMessage("��������� ������... ������");
			break;
		}
		if (show) {
			OutputSystemMessage("��������� ������... OK  OK");
		}

		flags.GSM_Connected = 1;

		return 1;

	} while (0);

	if (show) {
		clrInd();
	}
	return 0;
}

unsigned char PowerOnGSM() {
	GSM_PowerKey = 0;
	GSM_PowerKey_State = 0;
	UserDelay(200);
	GSM_PowerKey_State = 1;
}

void TelephoneCall(unsigned char *phone_number) {
	if (phone_number == NULL) {
		return;
	}

	volatile unsigned char command[] = "ATD              ";
	unsigned char i;
	for (i = 0; *(phone_number + i) != '\0'; i++) {
		command[i + 3] = *(phone_number + i);
	}
	command[i + 3] = ';';
	command[i + 4] = '\0';
	SendCommandToUSART(command, 1);

}

unsigned char Select_OK_NO(unsigned char *text) {

	clrInd();

	while (1) {

		OutputString(text, 0, 0, 1);

		if (KeyCode == 42) { // Enter
			KeyCode = 0;
			clrInd();
			return 1;
		} else if (KeyCode == 43) { // Cancel
			KeyCode = 0;
			clrInd();
			return 0;
		}
	}
}

unsigned char ShowMenu(unsigned char *menu[], unsigned char item) {

	clrInd();

	static unsigned char line = 0;
	if (item > 0) {
		item--;
	}
	if (item == 0) {
		line = 0;
	}

	unsigned char result = 0;

	unsigned char item_max = 0;
	while (menu[item_max] != NULL) {
		item_max++;
	}

	item_max--;

	while (1) {

		if (line == 0) {
			OutputString(menu[item], 0, 1, 0);
			if (item_max > item) {
				OutputString(menu[item + 1], 1, 1, 0);
			}
		} else if (line == 1) {
			if (item > 0) {
				OutputString(menu[item - 1], 0, 1, 0);
			}
			OutputString(menu[item], 1, 1, 0);
		}

		OutputString("�", line, 0, 0);

		if (KeyCode == 42) { // Enter
			KeyCode = 0;
			result = item + 1;
			break;
		} else if (KeyCode == 43) { // Cancel
			KeyCode = 0;
			result = 0;
			break;
		} else if (KeyCode == 40) { // Up
			KeyCode = 0;
			if (item > 0) {
				item--;
				if (line == 1) {
					line = 0;
				}
			}
			clrInd();
		} else if (KeyCode == 41) { // Down
			KeyCode = 0;
			if (item < item_max) {
				item++;
				if (line == 0) {
					line = 1;
				}
			}
			clrInd();
		}
	}

	clrInd();
	return result;
}

void DialDTMF() {
	clrInd();

	input_text_props props = {AA, Numeric, Yes, Yes, Yes, Yes};
	unsigned char *string = NULL;
	while ((string = InputText(string, &props, 32)) != NULL) {
		if(props.InputChar != NULL){
			unsigned char s [] = "AT+VTS=\" \"";
			s[8] = props.InputChar;
			SendCommandToUSART(s, 1);
		}
	}
	clrInd();
}

void PhonebookEdit() {
	clrInd();

	Phone Contact;

	unsigned int cell = PHONEBOOK_START_ADRESS;

	unsigned char read = 0;

	while (1) {

		if (!read) {
			FillPhonebookCellFromEEPROM(&Contact, cell);
			Contact.name[sizeof (Contact.name) - 1] = '\0';
			Contact.phone[sizeof (Contact.phone) - 1] = '\0';
			read = 1;
			if (Contact.filled[0] != 1) {
				Contact.name[0] = '\0';
				Contact.name[sizeof (Contact.name) - 1] = '\0';
				Contact.phone[0] = '\0';
				Contact.phone[sizeof (Contact.phone) - 1] = '\0';
			}
		}

		unsigned char num_cell[] = "  ";
		NumericToString((cell - PHONEBOOK_START_ADRESS) / sizeof (Contact) + 1, num_cell, sizeof (num_cell));
		OutputString(num_cell, 1, 14, 0);
		if (Contact.filled[0] == 1) {
			OutputString(Contact.name, 0, 0, 0);
			OutputString(Contact.phone, 1, 0, 0);
		} else {
			OutputString("     <�����>    ", 0, 0, 0);
			OutputString("             ", 1, 0, 0);
		}

		if (KeyCode == 45) { // Menu
			KeyCode = 0;

			unsigned char *menu[] = {
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
			};

			char o = 0;
			if (flags.ActiveCall) {
				menu[o++] = "������� �����";
			}
			menu[o++] = "��������";
			menu[o++] = "������� �����";
			menu[o++] = "�������";
			menu[o++] = "������� ���";

			o = 1;
			char result = ShowMenu(menu, 0);
			if ((flags.ActiveCall ? result == o++ : 0)) { // ������� �����
				DialDTMF();
			} else if (result == o++) { // ��������
				clrInd();

				unsigned char *field;
				input_text_props input_props = {Aa, Russian, No, No};
				if ((field = InputText(
						(Contact.filled[0] == 1)
						?
						Contact.name
						:
						NULL,
						&input_props, sizeof (Contact.name) - 1)) != NULL) {

					unsigned char i;
					for (i = 0; (*(field + i) != '\0'); i++) {
						Contact.name[i] = *(field + i);
					}
					Contact.name[i] = '\0';
					input_props.Language = Numeric;

					if ((field = InputText(
							(Contact.filled[0] == 1)
							?
							Contact.phone
							:
							NULL,
							&input_props, sizeof (Contact.phone) - 1)) != NULL) {

						for (i = 0; (*(field + i) != '\0'); i++) {
							Contact.phone[i] = *(field + i);
						}
						Contact.phone[i] = '\0';

						Contact.filled[0] = 1;
						WritePhonebookCellToEEPROM(&Contact, cell);
					} else {
						read = 0;
					}
				}
			} else if (result == o++) { // ������� �����

				input_text_props input_props = {Aa, Numeric, No, No};
				unsigned char *number = InputText(NULL, &input_props, 13);
				if (number != NULL) {
					TelephoneCall(number);
					flags.ActiveCall = 1;
				}

			} else if (result == o++) { // �������

				if (Contact.filled[0] == 1 && Select_OK_NO("������� �����?")) {
					Phone aaa;
					WritePhonebookCellToEEPROM(&aaa, cell);
					read = 0;
					clrInd();
				}
			} else if (result == o++) { // ������� ���
				if (Select_OK_NO("�������� ���������� �����?")) {
					OutputSystemMessage("�������� ������ ...");
					Format_EEPROM_Memory(PHONEBOOK_START_ADRESS, END_OF_PHONEBOOK - 1, 0);
					OutputSystemMessage("������!");
				}
			}

		} else if (KeyCode == 31) {
			KeyCode = 0;
			if (flags.ActiveCall) {
				SendCommandToUSART("ATA", 0); // ������� �����
			} else if (Contact.filled[0] == 1 && Select_OK_NO("��������� �����?")) {
				if (Contact.filled[0] == 1) {
					TelephoneCall(Contact.phone);
					flags.ActiveCall = 1;
				}
			}
		} else if (KeyCode == 33) { // ��������� �����
			KeyCode = 0;
			SendCommandToUSART("ATH0", 0);
			flags.ActiveCall = 0;
			flags.RemoteControlIsEnabled = 0;
		} else if (KeyCode == 40) { // Next
			KeyCode = 0;
			if (cell < LAST_PHONEBOOK_CELL) {
				cell = cell + sizeof (Contact);
			} else {
				cell = PHONEBOOK_START_ADRESS;
			}
			read = 0;
			clrInd();
		} else if (KeyCode == 41) { // Prev
			KeyCode = 0;
			if (cell > PHONEBOOK_START_ADRESS) {
				cell = cell - sizeof (Contact);
			} else {
				cell = LAST_PHONEBOOK_CELL;
			}
			read = 0;
			clrInd();
		} else if (KeyCode == 43) { // Exit
			KeyCode = 0;
			clrInd();
			return;
		} else if (KeyCode == 30) { // Adress = 0
			KeyCode = 0;
			cell = PHONEBOOK_START_ADRESS;
			read = 0;
			clrInd();
		}
	}
}

unsigned char Read_USART_SpeedNumber() {
	unsigned char v = EERD(USART_SPEED_NUMBER_CELL);
	if (v > 3) {
		v = 1;
	}
	return v;
}

void Save_USART_SpeedNumber(unsigned char speed) {
	if (speed == 0 || speed == 0xFF) {
		speed == 3;
	}
	EEWR(USART_SPEED_NUMBER_CELL, speed);
}

unsigned char USART_Speed_Choise() {
	unsigned char *menu[] = {
		" 1200",
		" 2400",
		" 9600",
		//" 19200",
		//" 57600",
		NULL
	};
	return ShowMenu(menu, Read_USART_SpeedNumber());
}

void Settings() {

	char item = 0;
	unsigned char *menu[] = {
		"�������. �����",
		"�������� USART",
		"�������. GSM",
		"�������� GSM",
		"��������� GSM",
		"���������. GSM",
		"���� GSM-����",
		"�����. ������",
		NULL
	};

	do {

		unsigned char num = 1;

		item = ShowMenu(menu, item);

		if (item == num++) { // 
			TimeEdit();
		} else if (item == num++) { // 
			unsigned char speed = USART_Speed_Choise();
			if (speed != 0) {
				Save_USART_SpeedNumber(speed);
			}
		} else if (item == num++) { // 

			unsigned char *menu_GSM_on_off[] = {
				"���������",
				"��������",
				NULL
			};
			unsigned char GSM_Condition = EERD(USE_GSM_MODULE_CELL);
			if (GSM_Condition != 1) {
				GSM_Condition = 0;
			}
			unsigned char GSM_On_Off = ShowMenu(menu_GSM_on_off, GSM_Condition + 1);
			if (GSM_On_Off != 0) {
				if (GSM_On_Off == 1) {
					flags.GSM_Connected = 0;
				}
				EEWR(USE_GSM_MODULE_CELL, GSM_On_Off - 1);
			}
		} else if (item == num++) { // 
			PowerOnGSM();
			OutputSystemMessage("��");
		} else if (item == num++) { // 
			SendCommandToUSART("AT+CPOWD=1", 1);
		} else if (item == num++) { // 
			Init_GSM(1);
		} else if (item == num++) { // 
			if (flags.GSM_Connected) {
				unsigned long int tt = getSystemTimePoint();
				CleanStringArray(IncommingBuffer[LAST_INCOMMING_BUFF_INDEX], BUFFER_STRING_LENGTH, '\0');
				SendCommandToUSART("AT+CSQ", 1);
				unsigned char result = 0;
				while (!(result = FindIncommingData(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX)) && !testTimePoint(tt, 20));
				if (result) {
					unsigned char myOutputString [33];
					myOutputString[32] = '\0';

					unsigned char *p = IncommingBuffer[LAST_INCOMMING_BUFF_INDEX - 1] + BUFFER_STRING_LENGTH - 33;
					for (unsigned char i = 0; i < 32; i++) {
						*(myOutputString + i) = *p++;
						if (*(myOutputString + i) == '\0') {
							*(myOutputString + i) = EMPTY_SYMBOL_VALUE;
						}
					}

					OutputSystemMessage(myOutputString);
				} else {
					OutputSystemMessage("��� ������");
				}
			} else {
				OutputSystemMessage("������ �� ���������");
			}
		} else if (item == num++) { //
			Format_EEPROM_Memory(0, 1023, 1);
		}
	} while (item > 0);
}

unsigned char *GetSystemInfo() {

	unsigned char info [73];
	CleanStringArray(info, sizeof (info), '\0');
	strcat(info, "���. ����.:\n");

	unsigned char d;
	if (!flags.LockSignals) {
		strcat(info, "����: ");
		unsigned char s_auto [] = "   \n";
		d = 0b00001000;
		for (unsigned char i = 3; i > 0; i--) {
			d = d >> 1;
			if ((SignalsForInd & d) != 0) {
				s_auto[i - 1] = getNumChar(i);
			} else {
				s_auto[i - 1] = '-';
			}
		}
		strcat(info, s_auto);
	}

	strcat(info, "����: ");
	unsigned char s_hand [] = "   \n";
	d = 0b00001000;
	for (unsigned char i = 3; i > 0; i--) {
		d = d >> 1;
		if ((CurrentSignals & d) != 0) {
			s_hand[i - 1] = getNumChar(i);
		} else {
			s_hand[i - 1] = '-';
		}
	}
	strcat(info, s_hand);

	if (flags.LockSignals) {
		strcat(info, "���������� ����.\n");
	}
	/*
	���.����.:
	����:123
	����:123
	���-���: ����
	����.���.:
	�� 11:00-11:59 123
	 */
	if (!flags.LockSignals) {

		long int cTime = Clock;
		unsigned int NextCell = FindNextTimeStart(&cTime);

		if (NextCell == END_OF_CELLS) {
			cTime = 0;
			NextCell = FindNextTimeStart(&cTime);
		}
		if (NextCell == END_OF_CELLS) {
			strcat(info, "���������� �� ���.\n");
		} else {
			strcat(info, "����. ���.:\n");

			unsigned char DataArray [] = "�� 11:00-11:59 123";
			CleanStringArray(DataArray, sizeof (DataArray), '\0');

			unsigned long int Data = 0;
			unsigned char CellsData [CELL_CAPACITY];
			ReadDataOfCell(&CellsData, &Data, NextCell);

			unsigned int TimeStart;
			unsigned int TimeStop;
			unsigned char Days;
			unsigned char Signals;
			ParseDataRecord(Data, &TimeStart, &TimeStop, &Days, &Signals);

			unsigned int Time;
			unsigned char Day;
			ParseTime(cTime, &Time, &Day);

			unsigned char m_1 [] = "00";
			NumericToString(Time % 60, m_1, 2);

			Time /= 60;

			unsigned char h_1 [] = "00";
			NumericToString(Time, h_1, 2);

			unsigned char m_2 [] = "00";
			NumericToString(TimeStop % 60, m_2, 2);

			TimeStop /= 60;

			unsigned char h_2 [] = "00";
			NumericToString(TimeStop, h_2, 2);
			strcat(DataArray, GetDayOfWeek(Day));
			strcat(DataArray, " ");
			strcat(DataArray, h_1);
			strcat(DataArray, ":");
			strcat(DataArray, m_1);
			strcat(DataArray, "-");
			strcat(DataArray, h_2);
			strcat(DataArray, ":");
			strcat(DataArray, m_2);
			strcat(DataArray, " ");

			unsigned char sss [] = "   \n";
			d = 0b00001000;
			for (unsigned char i = 3; i > 0; i--) {
				d = d >> 1;
				if ((Signals & d) != 0) {
					sss[i - 1] = getNumChar(i);
				} else {
					sss[i - 1] = '-';
				}
			}
			strcat(DataArray, sss);
			strcat(info, DataArray);
		}
	}
	//strcat(info, "+/-20�C");
	return info;
}

void main2() {

	flags.UsartExchangeEnabled = 1;
	
	flags.UseGSM = EERD(USE_GSM_MODULE_CELL);
	
	if (flags.UseGSM == 1) {
		for (unsigned char i = 0; i < 5; i++) {
			if (Init_GSM(1)) {
				flags.SendTimeRequest = 1;
				break;
			} else {
				if (KeyCode == 43) {
					KeyCode = 0;
					break;
				}
				PowerOnGSM();
				UserDelay(50);
			}
		}
	}

	NearTimeStart = Clock;
	AdressOfNextStartCell = FindNextTimeStart(&NearTimeStart);

	unsigned long int t1 = getSystemTimePoint();
	
	while (1) {

		if(flags.UseGSM && testTimePoint(t1, 3000)){
			t1 = getSystemTimePoint();
			CleanStringArray(IncommingBuffer[LAST_INCOMMING_BUFF_INDEX], BUFFER_STRING_LENGTH, '\0');
			SendCommandToUSART("AT", 1);
			unsigned long int t2 = getSystemTimePoint();
			while (!FindIncommingData(StandardAnswer_OK, NULL, LAST_INCOMMING_BUFF_INDEX) && !testTimePoint(t2, 50));
			if(testTimePoint(t2, 50)){
				flags.ActiveCall = 0;
				flags.RemoteControlIsEnabled = 0;
				flags.StatusIsRequested = 0;
				CleanStringArray(active_phone, sizeof (active_phone), '\0');
				PowerOnGSM();
				Init_GSM(0);
			}else{
				flags.GSM_Connected = 1;
			}
		}

		if (flags.StatusIsRequested) {
			flags.StatusIsRequested = 0;
			SendSMS(active_phone, GetSystemInfo());
			CleanStringArray(active_phone, sizeof (active_phone), '\0');
		}

		if (flags.LCD_Power_On == 1 && LCD_ON_TIMEOUT == 0) { //&& CurrentSignals == 0
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
			char item = 0;
			unsigned char *menu[] = {
				"���������",
			//	"�������� 1-WIRE",
				NULL
			};
			do {

				item = ShowMenu(menu, item);

			/*	if (item == 1) { // 
					Scheduler(END_OF_CELLS);
				} else if (item == 2) { // 
					PhonebookEdit();
				} else*/
				if (item == 1) { // ���������
					Settings();
				} else if (item == 2) { // 
				//	DS_Edit;
				}
			} while (item > 0);
			clrInd();
		} else if (KeyCode == 40) {
			KeyCode = 0;
			Scheduler(END_OF_CELLS);
		} else if (KeyCode == 41) {
			KeyCode = 0;
			PhonebookEdit();
		} else if (KeyCode == 44) {
			KeyCode = 0;
			if (flags.LCD_Power_On) {
				flags.LCD_Light_On = !flags.LCD_Light_On;
			}
		} else if (KeyCode == 30) {
			KeyCode = 0;
			flags.LockSignals = 1;
			CurrentSignals = 0;
			EEWR(LOCK_SIGNALS_FLAG_CELL, flags.LockSignals);
			flags.SignalsAreChanged = 1;
		} else if (KeyCode == 37 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			CurrentSignals = CurrentSignals^0b00000001;
			flags.SignalsAreChanged = 1;
		} else if (KeyCode == 38 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			CurrentSignals = CurrentSignals^0b00000010;
			flags.SignalsAreChanged = 1;
		} else if (KeyCode == 39 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			CurrentSignals = CurrentSignals^0b00000100;
			flags.SignalsAreChanged = 1;
		} else if (KeyCode == 34 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			CurrentSignals = 0;
			flags.SignalsAreChanged = 1;
		} else if (KeyCode == 35 && !flags.DetailModeOfViewSheduler) {
			KeyCode = 0;
			flags.LockSignals = 0;
			EEWR(LOCK_SIGNALS_FLAG_CELL, flags.LockSignals);
			flags.SignalsAreChanged = 1;
		} else if (KeyCode == 31) {
			KeyCode = 0;
			lcd_off();
		} else if (KeyCode == 33) {
			KeyCode = 0;
			WorkingWithGSM();
		} else if (KeyCode == 42) {
			KeyCode = 0;
			current_showing = csTemperature;
			//	SendSMS("0957075762", GetSystemInfo());
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

	if (flags.TimeSource == NON) {
		return;
	}

	static unsigned long int lastTimeStamp = 0;

	if (testTimePoint(lastTimeStamp, 4000) || lastTimeStamp == 0 || flags.SendTimeRequest) {
		lastTimeStamp = getSystemTimePoint();
		flags.SendTimeRequest = 0;
	} else {
		return;
	}

	if (flags.TimeSource == RTC) {
		return;

		unsigned char Seconds = 0x00;

		I2CInit();
		I2CStart();
		I2CSend(0xD0);
		I2CSend(0x00);
		I2CRestart();
		I2CSend(0xD1);


		unsigned char I = I2CRead();

		if ((I & 0b10000000) == 0) {
			I2CAck();
			Seconds = BCD_to_DEC(I);
			cMinutes = BCD_to_DEC(I2CRead());
			I2CAck();
			cHours = BCD_to_DEC(I2CRead());
			I2CAck();
			cWeekDay = BCD_to_DEC(I2CRead());
			I2CAck();
			cDays = BCD_to_DEC(I2CRead());
			I2CAck();
			cMonths = BCD_to_DEC(I2CRead());
			I2CAck();
			cYears = BCD_to_DEC(I2CRead());
		} else {
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
	} else if (flags.TimeSource == GSM && flags.GSM_Connected && flags.UsartExchangeEnabled) {
		SendCommandToUSART("AT+CCLK?", 0);
	}
}

void WriteTime(unsigned long int lClock, unsigned char days, unsigned char months, unsigned char years) {

	if (flags.TimeSource == RTC) {
		return;

		volatile unsigned char INTCON_BUP = INTCON;
		INTCONbits.GIEH = 0;
		INTCONbits.GIEL = 0;
		//ClrWdt();

		unsigned char week_day;
		unsigned char hours;
		unsigned char minutes;

		lClock /= 6000;
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
	} else if (flags.TimeSource == GSM && flags.GSM_Connected && flags.UsartExchangeEnabled) {
		unsigned char comand [] = "AT+CCLK=\"yy/mm/dd,hh:mm:00+03\"";
		long int temp = Clock / 6000;
		comand[22] = getNumChar(temp % 10);
		temp /= 10;
		comand[21] = getNumChar(temp % 6);
		temp /= 6;
		temp = temp % 24;
		comand[19] = getNumChar(temp % 10);
		temp /= 10;
		comand[18] = getNumChar(temp);

		comand[16] = getNumChar(cDays % 10);
		comand[15] = getNumChar(cDays / 10);
		comand[13] = getNumChar(cMonths % 10);
		comand[12] = getNumChar(cMonths / 10);
		comand[10] = getNumChar(cYears % 10);
		comand[9] = getNumChar(cYears / 10);

		SendCommandToUSART(comand, 1);
	}
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

unsigned char *EncodePhoneNumberToPDU(unsigned char *phone_number) {
	if (*phone_number == '+') {
		phone_number++;
	}
	unsigned char number_legth = strlen(phone_number);

	unsigned char encoded [15];
	strcpy(encoded, phone_number);
	if (number_legth % 2 != 0) {
		strcat(encoded, "F");
	}

	unsigned char temp;
	for (unsigned char i = 0; encoded[i] != '\0'; i += 2) {
		temp = encoded[i];
		encoded[i] = encoded[i + 1];
		encoded[i + 1] = temp;
	}
	return encoded;
}

void cp1251_to_unicode(unsigned char *dest, unsigned char *src, unsigned char *eof) {
	unsigned int a = 0;
	while (dest < eof && *src != '\0') {
		a = GetUnicodeSymbol(*(src));
		unsigned char t [] = "0000";
		NumericToHEXString(a, t, 4);
		strcat(dest, t);
		dest += 4;
		src++;
	}
}

void SendSMS(unsigned char *PhoneNumber, unsigned char *Text) {

	if (flags.GSM_Connected != 1) {
		return;
	}

	CleanStringArray(SMS, sizeof (SMS), '\0');

	strcat(SMS, "00");

	unsigned int sc_len = strlen(SMS);

	unsigned char *pn = PhoneNumber;
	if (*pn == '+') {
		pn++;
	}

	strcat(SMS, "0100");
	unsigned char tempString [] = "00";
	NumericToHEXString(strlen(pn), tempString, 2);
	strcat(SMS, tempString);
	strcat(SMS, "91");
	unsigned char *myNumber = EncodePhoneNumberToPDU(PhoneNumber);
	strcat(SMS, myNumber);
	strcat(SMS, "00");
	strcat(SMS, "18");

	unsigned int l = strlen(Text) * 2;
	unsigned char e [] = "00";
	NumericToHEXString(l, e, 2);
	strcat(SMS, e);

	cp1251_to_unicode(&(SMS[0]) + strlen(SMS), Text, &(SMS[0]) + sizeof (SMS) - 1);
	sc_len = (strlen(SMS) - sc_len) / 2;
	strcat(SMS, "\x1A");
	unsigned char y [] = "\0\0\0";
	NumericToString(sc_len, y, (sc_len > 99 ? 4 : (sc_len > 9 ? 3 : 2)));
	unsigned char command [] = "AT+CMGS=\0\0\0";
	strcat(command, y);

	flags.UsartExchangeEnabled = 0;
	unsigned long int tt = getSystemTimePoint();
	SendCommandToUSART(command, 1);
	while (!FindIncommingData("> ", NULL, MAX_INCOMMING_BUFF_INDEX) && !testTimePoint(tt, 1500));
	if (FindIncommingData("> ", NULL, MAX_INCOMMING_BUFF_INDEX)) {
		SendCommandToUSART(SMS, 1);
	} else {
		OutputSystemMessage("�� ������� ��������� ���������");
	}
	flags.UsartExchangeEnabled = 1;
}