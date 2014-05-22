
void setup() {
  Serial.begin(9600);
}



void displayInitSequence()
{
  int i = 1;  
  while( i < 9 ) {
    MON_toggleGPIOLed1( ((i%2)==1) );
    MON_toggleGPIOLed2( ((i%2)==1) );
    SPI_writeConfigurationRegister();
    delay(200);
    i++;
  }
}


char user_input = -1;
int loop_ctr = 0;

void loop()
{ 
  if(loop_ctr == 0) {
    SPI_openBus();
    Serial.println("Opened SPI bus.");
    displayInitSequence();
  }
  if( 1000 <= loop_ctr ) {
    loop_ctr = 1;
  }
  updateErrMsgCtr();


  user_input = Serial.read();
  if( user_input != -1 ) {
    handleInput(user_input);
  }


  SPI_sendCommandToMonitor(MON_CMD_START_DIAG_REG);
  delay(20);
  if( do_adc_conversion ) {
    SPI_sendCommandToMonitor(MON_CMD_START_ADC_CONVERSION_ALL);
  }
  if( do_adc_conversion_discharge ) {
    SPI_sendCommandToMonitor(MON_CMD_START_ADC_CONVERSION_ALL_ALLOW_DISCH);
  }

  SPI_readDiagnostics();
  SPI_readConfigurationRegister();
  SPI_readAllVoltages();
  
  checkVoltages();
  
  delay(80);
  loop_ctr++;
}

