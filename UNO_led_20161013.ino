class IR_led{
 public:
  IR_led() {
  }
  void setup(){
    pinMode(ir_led_one, OUTPUT);
    tone(ir_led_one, frequency);
    pinMode(ir_led_two, OUTPUT);
    tone(ir_led_two, frequency);//1
    pinMode(ir_led_three, OUTPUT);
    tone(ir_led_three, frequency);
    pinMode(ir_led_four, OUTPUT);
    tone(ir_led_four, frequency);//2
    pinMode(ir_led_five, OUTPUT);
    tone(ir_led_five, frequency);
    pinMode(ir_led_six, OUTPUT);
    tone(ir_led_six, frequency); //3          
    pinMode(ir_led_seven, OUTPUT);
    tone(ir_led_seven, frequency);
    pinMode(ir_led_eight, OUTPUT);
    tone(ir_led_eight, frequency);//4
    pinMode(ir_led_nine, OUTPUT);
    tone(ir_led_nine, frequency);
    pinMode(ir_led_ten, OUTPUT);//5
    tone(ir_led_ten, frequency);
    pinMode(ir_led_eleven, OUTPUT);
    tone(ir_led_eleven, frequency);
    pinMode(ir_led_twelve, OUTPUT);
    tone(ir_led_twelve, frequency);//6
    pinMode(ir_led_thirteen, OUTPUT);
    tone(ir_led_thirteen, frequency);
    pinMode(ir_led_fourteen, OUTPUT);
    tone(ir_led_fourteen, frequency);//7
    pinMode(ir_led_fifteen, OUTPUT);
    tone(ir_led_fifteen, frequency);
    pinMode(ir_led_sixteen, OUTPUT);
    tone(ir_led_sixteen, frequency);//8

  }

 private:
  const int ir_led_one = 2;
  const int ir_led_two = 3;
  const int ir_led_three = 4;
  const int ir_led_four = 5;
  const int ir_led_five = 6;
  const int ir_led_six = 7;
  const int ir_led_seven = 54;
  const int ir_led_eight = 55;
  const int ir_led_nine = 56;
  const int ir_led_ten = 57;
  const int ir_led_eleven = 58;
  const int ir_led_twelve = 59;
  const int ir_led_thirteen = 60;
  const int ir_led_fourteen = 61;
  const int ir_led_fifteen = 62;
  const int ir_led_sixteen = 63;
  const  unsigned int frequency = 38000;  
  
  
};
  
class IR_recever{
 private:
  int mothNum = 0;
  int pre_one = 0;
  int pre_two = 0; 
  int flag_one_in = 0;
  int flag_two_in = 0;
  int flag_one_out = 0;
  int flag_two_out = 0;
  uint8_t MyVal_one = 0;
  uint8_t MyVal_two = 0;
  const int ir_re_one = 22;
  const int ir_re_two = 23;
  const int ir_re_three = 24;
  const int ir_re_four = 25;
  const int ir_re_five = 26;
  const int ir_re_six = 27;
  const int ir_re_seven = 28;
  const int ir_re_eight = 29;
  const int ir_re_nine = 30;
  const int ir_re_ten = 31;
  const int ir_re_eleven = 40;
  const int ir_re_twelve = 41;
  const int ir_re_thirteen = 42;
  const int ir_re_fourteen = 43;
  const int ir_re_fifteen = 44;
  const int ir_re_sixteen = 45;

  
 public:
  IR_recever(){
  }
  int read(){
    mothNum = 0;
    //1
    int ir_status_one = digitalRead(ir_re_one);
    int ir_status_two = digitalRead(ir_re_two);

    //2
    int ir_status_three = digitalRead(ir_re_three);
    int ir_status_four = digitalRead(ir_re_four);

    //3
    int ir_status_five = digitalRead(ir_re_five);
    int ir_status_six = digitalRead(ir_re_six);

    //4
    int ir_status_seven = digitalRead(ir_re_seven);
    int ir_status_eight = digitalRead(ir_re_eight);

    //5
    int ir_status_nine = digitalRead(ir_re_nine);
    int ir_status_ten = digitalRead(ir_re_ten);

    //6
    int ir_status_eleven = digitalRead(ir_re_eleven);
    int ir_status_twelve = digitalRead(ir_re_twelve);

    //7
    int ir_status_thirteen = digitalRead(ir_re_thirteen);
    int ir_status_fourteen = digitalRead(ir_re_fourteen);

    //8
    int ir_status_fifteen = digitalRead(ir_re_fifteen);
    int ir_status_sixteen = digitalRead(ir_re_sixteen);


    int ir_setOne = (ir_status_eight << 7) + (ir_status_seven << 6) + (ir_status_six << 5) + (ir_status_five << 4) + \
      (ir_status_four << 3) + (ir_status_three << 2) + (ir_status_two << 1) + ir_status_one ;
    int ir_setTwo = (ir_status_sixteen << 7) + (ir_status_fifteen << 6) + (ir_status_fourteen << 5) + (ir_status_thirteen << 4) + \
      (ir_status_twelve << 3) + (ir_status_eleven << 2) + (ir_status_ten << 1) + ir_status_nine ;


    MyVal_one = ir_setOne;
    MyVal_two = ir_setTwo;

    int mcpOne =  MyVal_one;
    int mcpTwo =  MyVal_two;


    //1
    flag_one_out |= ((0b01010101 & pre_one) & ~mcpOne);
    flag_one_in   = ((0b10101010 & pre_one) & ~mcpOne);
    int cha_one_1 =  flag_one_out;
    int cha_one_2 =  (flag_one_in >> 1);
    for(int i=0;i<4;i++){
      if (0b00000001 & cha_one_1 & cha_one_2){
	mothNum ++;
	flag_one_out -= (1 << i*2);
      }
      cha_one_1 >>= 2;
      cha_one_2 >>= 2;
    }
    //2
    flag_two_out |= ((0b01010101 & pre_two) & ~mcpTwo);
    flag_two_in   = ((0b10101010 & pre_two) & ~mcpTwo);
    int cha_two_1 =  flag_two_out;
    int cha_two_2 =  (flag_two_in >> 1);
    for(int i=0;i<4;i++){
      if (0b00000001 & cha_two_1 & cha_two_2){
	mothNum ++;
	flag_two_out -= (1 << i*2);
      }
      cha_two_1 >>= 2;
      cha_two_2 >>= 2;
    }
    //save
    pre_one = mcpOne;
    pre_two = mcpTwo;


    return mothNum;
  };
  void setup(){
    pinMode(ir_re_one, INPUT);
    pinMode(ir_re_two, INPUT);//1
    pinMode(ir_re_three, INPUT);
    pinMode(ir_re_four, INPUT);//2
    pinMode(ir_re_five, INPUT);
    pinMode(ir_re_six, INPUT);//3
    pinMode(ir_re_seven, INPUT);
    pinMode(ir_re_eight, INPUT);//4
    pinMode(ir_re_nine, INPUT);
    pinMode(ir_re_ten, INPUT);//5
    pinMode(ir_re_eleven, INPUT);
    pinMode(ir_re_twelve, INPUT);//6
    pinMode(ir_re_thirteen, INPUT);
    pinMode(ir_re_fourteen, INPUT);//7
    pinMode(ir_re_fifteen, INPUT);
    pinMode(ir_re_sixteen, INPUT);//8
  }
};

IR_led myled;
IR_recever myir;
void setup(){
  Serial.begin(9600);
  myir.setup();
  myled.setup();
  delay(1500);
}
void loop(){
  int mothNum = myir.read();
  Serial.write(mothNum);
  delay(100);
};


