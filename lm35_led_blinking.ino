// C++ code
//
const int lm35_pin = A0;  /* LM35 O/P pin */
bool  LED_STATE=true;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
 
  cli();                      //stop interrupts for till we make the settings
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0  
  TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescalar 256  
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A  
  OCR1A = 15625;//Finally we set compare register A to this value  
  
  sei();
  
  
}

int flag1=0,flag2=0;

void loop() 
{
  int analog_val;
  float temp_val;
  
  analog_val = analogRead(lm35_pin);  /* Read Temperature */
  temp_val = (analog_val * 4.88); /* Convert adc value to equivalent voltage */
  temp_val = (temp_val/10); /* LM35 gives output of 10mv/°C */

  if(temp_val<=30)
  {
    //250ms - blinking
    if(flag1)
    {
    LED_STATE = !LED_STATE;      //Invert LED state
    digitalWrite(LED_BUILTIN,LED_STATE); 
    flag1=0;
    }
  }
  else
  {
    //on every 500ms
    if(flag2==2)
    {
    LED_STATE = !LED_STATE;      //Invert LED state
    digitalWrite(LED_BUILTIN,LED_STATE); 
    flag2=0;
    }
  }
  
}

ISR(TIMER1_COMPA_vect)
{
  //for each 250ms
  TCNT1  = 0; //First, set the timer back to 0 so it resets for next interrupt
  flag1++;
  flag2++;
  
}
