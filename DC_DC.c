// Compiler: Avrgcc Device: atmega328

#define F_CPU 16000000UL // Defining the CPU Frequency
// Define baud rate
#define BAUD 9600
// Calculate UBRR value for baud rate
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)
#include <avr/io.h>      // Contains all the I/O Register Macros
#include <util/delay.h>  // Generates a Blocking Delay

void setupTimer1(uint16_t top) {
 DDRB   = (1 << 1); // OC1B is B1  
TCCR1A = (1 << COM1A1) | (1 << WGM11); // Fast PWM, modo 14, saída em OC1A
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8
  ICR1 = top; // Define TOP para a frequência desejada
  OCR1A = top / 2; // Duty cycle fixo em 50%
}

uint16_t readADC(uint8_t channel) {
  ADMUX = (1 << REFS0) | (channel & 0x0F); // Referência AVCC, canal selecionado
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1); // Habilita ADC, prescaler 64
  while (ADCSRA & (1 << ADSC)); // Aguarda conversão
  return ADC;
}


void usart_init(void) {
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8); // High byte
    UBRR0L = (uint8_t)(UBRR_VALUE);      // Low byte

    // Enable transmitter
    UCSR0B = (1 << TXEN0);

    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_transmit(uint8_t data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // Put data into buffer, sends the data
    UDR0 = data;
}

void usart_print(const char *str) {
    // Transmit each character until null terminator
    while (*str) {
        usart_transmit(*str++);
    }
}
// Convert uint16_t to string
void uint16_to_string(uint16_t valuee, char *buffer) {
    uint8_t i = 0;
    char temp[7]; // Temporary buffer for digits (max 5 digits + null)

    // Handle zero case
    if (valuee == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

// Convert digits to characters in reverse order
    while (valuee > 0) {
        temp[i++] = (valuee % 10) + '0'; // Get digit and convert to char
        valuee /= 10;
    }

    // Reverse the string into the output buffer
    uint8_t j = 0;
    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    buffer[j] = '\0'; // Null terminate
}

//int main(void) {
    // Initialize USART
   // usart_init();

    //while (1) {
        // Print "Hello, World!" every second
//        usart_print("Hello, World!\r\n");
     //   _delay_ms(1000);
   // }

int main(void)
{
usart_init();
char buffer[7]; // Enough for uint16_t (up to 65535) + null
    uint16_t value = 12345; // Example uint16_t value

  //  while (1) {
        // Convert and print uint16_t
     //   uint16_to_string(value, buffer);
    //    usart_print(buffer);
   //     usart_print("\r\n"); // Newline
    //    value++; // Increment for demonstration
    //    _delay_ms(1000);
   // }


   // DDRD   = (1 << 2); // OC3B is  D2
   // ICR3   = 10000;
   // OCR3B  =  5000;    
   // TCCR3A = (1 << COM3B1) | (1 << WGM31);
   // TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31);
    
   // DDRB   = (1 << 2); // OC1B is B2
   //ICR1   = 10000;
   // OCR1B  =  5000;
   // TCCR1A = (1 << COM1B1) | (1 << WGM11);
   // TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

   // DDRB |= 1;
   //void loop() {
   
	float Kp = 9 ;
	float Ki = 30 ;
	float integral = 0.0 ; 
	float DT = 0.001 ;
while (1){
		   
  uint16_t vref = readADC(0); // Lê Vref (A0)
  uint16_t vfb = readADC(1);  // Lê Vfb (A1)
  
  // Calcula erro
  //float
 uint16_t error = (uint16_t)((vref - vfb) * (5.0 / 1023.0)); // Converte para volts (0-5V)
  
  // Calcula termos PI
 // float 
 uint16_t proportional = (uint16_t)(Kp * error);
uint16_t integral_p = integral;  
integral += Ki * error * DT; // DT é o intervalo de amostragem (ex.: 0.001s para 1kHz)
  uint16_t integrall = (uint16_t)(integral);
  // Calcula saída (frequência)
  //float

 uint16_t u = (uint16_t)(proportional + integrall);
  
  // Limita a saída para o intervalo de frequência desejado
  //uint16_t top = constrain(200 + (u * (667 - 200)), 200, 667); // Mapeia para 3kHz (667) a 10kHz (200)
        // Option 2: Explicit bounds checking (already in your code)
     // uint16_t top = (uint16_t)(200 + (u * (667 - 200))); // Mapeia para 3kHz (667) a 10kHz (200)
      uint16_t top = (uint16_t)(500 - (u /* * (710-300)*/));
	  if (top < 445){ top = 445; integral = integral_p; }
      if (top > 500){ top = 500; integral = integral_p;}
      // Alternative using constrain macro (uncomment if macro defined)
      // ui
value = integrall;//top;
  uint16_to_string(value, buffer);
        usart_print(buffer);
        usart_print("\r\n"); // Newline
       // value++; // Increment for demonstration
       // _delay_ms(1000);       
 usart_print("Hello, World!\r\n");

  // Atualiza Timer1
  setupTimer1(top);
  //delay(1); // Ajuste para a taxa de amostragem desejada (ex.: 1ms = 1kHz)
  _delay_ms(1);
  }
  //  while (1)
  //  {
    //    PORTB ^= 1;
    //    _delay_ms( 500 );
    //}
}
