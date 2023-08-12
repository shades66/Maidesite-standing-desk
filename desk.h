class MyCustomComponent : public Component, public UARTDevice {
 public:
  MyCustomComponent(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override {
    // nothing to do here
  }
  
    int state=0;
    int value;
    int command;
    int chksum=0;
    int bufsize=0;
    int bufread=0;
    float valueF;
    uint8_t buffer[10]; 

  
  void loop() override {
    while (available()) {
      uint8_t c;
      this->read_byte(&c);
      //ESP_LOGD("custom", "We have a char");
      switch(state) {
        case 0:
            if (c==0xf2) {
                state=1;
                //ESP_LOGD("custom", "Byte 1 passed");
                }
            break;
        case 1:
            if (c==0xf2) {
                state=2;
                chksum=0;
                //ESP_LOGD("custom", "Byte 2 passed");
            } else {
                state=0;
                //ESP_LOGD("custom", "Byte 2 failed");
                }
            break;
        case 2:
            command=c;
            state=3;
            chksum+=command;
            //ESP_LOGD("custom", "Byte 3 command");
            break;
        case 3:
            bufsize=c;
            bufread=0;
            chksum+=bufsize;
            state=4;
            //ESP_LOGD("custom", "The expected buffer size %i", this->bufsize);
            break;
        case 4:
            if (bufread < bufsize) {
                buffer[bufread]=c;
                chksum+=c;
                //ESP_LOGD("custom", "The loaded value %i into slot %i", c,this->bufread);
                bufread++;
            }
            if (bufread == bufsize) state=5;
            break;
        case 5:
            //ESP_LOGD("custom", "The checksum expected is: %i", c);
            //ESP_LOGD("custom", "The checksum calculated is: %i", this->chksum);
            chksum=(chksum & 0xff);
            if (chksum == c) {
                state=6;
                //ESP_LOGD("custom", "checksum passed");
                }
            else
            {
                state=0;
                //ESP_LOGD("custom", "checksum failed");
                }
            break;
        case 6:
            // final check and then process the command /reponse
            state=0;
            if (c==0x7e) {
                //ESP_LOGD("custom", "The value of command is: %i", this->command);
                //ESP_LOGD("custom", "The value of length of response is: %i", this->bufsize);
                //ESP_LOGD("custom", "The value of chksum %i", this->chksum);
            switch(command) {
                case 1:
                    valueF= ((buffer[0]*256) + buffer[1]);
                    id(deskSlider).publish_state(valueF); 
                    //ESP_LOGD("custom", "Height updated");
                    break;
                case 0x25:
                case 0x26:
                case 0x27:
                case 0x28:
                    valueF= ((buffer[0]*256) + buffer[1]);
                    if (command == 0x25) id(deskm1).publish_state(valueF); 
                    if (command == 0x26) id(deskm2).publish_state(valueF); 
                    if (command == 0x27) id(deskm3).publish_state(valueF); 
                    if (command == 0x28) id(deskm4).publish_state(valueF); 
                    break;
                default:
                    // an error occured
                    break;
            }
            }
        default:
            // no idea what happened.   
            state=0;
            break;
      }
            


      
    }
    // etc
  }
};
