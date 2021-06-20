#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>
#define PS2_DAT        13   
#define PS2_CMD        11  
#define PS2_SEL        10  
#define PS2_CLK        12
#define LEFT_UP        8
#define LEFT_DOWN      7
#define RIGHT_DOWN     6
#define RIGHT_UP       5
//#define beel           2
/******************************************************************
 * select modes of PS2 controller:
 *   - pressures = analog reading of push-butttons 
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures   true
//#define pressures   false
#define rumble      true
//#define rumble      false
PS2X ps2x; //创建PS2X类
Servo myservo;  // create servo object to control a servo
//不支持热插拔
int error = 0;
byte type = 0;
byte vibrate = 0;
int pos = 105;    // 舵机角度variable to store the servo position0~141°。90为中间位置
int weidu = 3;
void setup(){
 myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  //pinMode(beel,OUTPUT);
  pinMode(LEFT_UP,OUTPUT);
  pinMode(LEFT_DOWN,OUTPUT);
  pinMode(RIGHT_UP,OUTPUT);
  pinMode(RIGHT_DOWN,OUTPUT);
  Serial.begin(9600);
  
  delay(300);  
   
 //设置引脚和设置：游戏板（时钟，命令，注意，数据，压力？，振动？）检查是否配置错误
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.print("Found Controller, configured successful ");  //找到控制器，配置成功
    Serial.print("pressures = ");
	if (pressures)
	  Serial.println("true ");
	else
	  Serial.println("false");
	Serial.print("rumble = ");
	if (rumble)
	  Serial.println("true)");
	else
	  Serial.println("false");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;"); //试一下所有的按钮，你按下更狠，X会振动控制器振动越快
    Serial.println("holding L1 or R1 will print out the analog stick values.");//按住L1或R1将打印模拟棒值
  }  
//报错情况
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug.");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug.");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); //确定控制器的类型
  switch(type) {
    case 0:
      Serial.print("Unknown Controller type found ");   //找到未知的控制器类型
      break;
    case 1:
      Serial.print("DualShock Controller found ");      //找到DualShock控制器
      break;
    case 2:
      Serial.print("GuitarHero Controller found ");     //找到GuitarHero控制器
      break;
	case 3:
      Serial.print("Wireless Sony DualShock Controller found ");    //找到无线Sony DualShock控制器
      break;
   }
}

void loop() {
  /* 您必须阅读Gamepad才能获得新值并设置振动值ps2x.read\u gamepad（小电机开/关，大电机强度0-255）
如果不启用隆隆声，请使用ps2x.read_gamepad（）；没有值你应该至少每秒钟打一次电话*/  
 // digitalWrite(beel,HIGH);
        digitalWrite(LEFT_UP,LOW);
        digitalWrite(RIGHT_UP,LOW);
        digitalWrite(LEFT_DOWN,LOW);
        digitalWrite(RIGHT_DOWN,LOW);
  if(error == 1) //skip loop if no controller found
    return; 
 //DualShock Controller   (实际的控制）
 /************************************************************  
  *   ps2x.Button(按钮名称)               只要按下就有效
  *  ps2x.ButtonPressed(按钮名称)     如果按下按钮，则为真
  *  ps2x.NewButtonState(按钮名称)     如果按下或松开按钮，则为真
  *  ps2x.ButtonReleased(按钮名称)    如果按钮刚刚被释放，则为真
  ************************************************************ 
  */
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");  
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");      

    if(ps2x.Button(PSB_PAD_UP)) {      //will be TRUE as long as button is pressed
      //digitalWrite(beel,LOW);
      //Serial.print("Up held this hard: ");
      //Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
        digitalWrite(LEFT_UP,HIGH);
        digitalWrite(RIGHT_UP,HIGH);
        Serial.println(LEFT_UP);
        Serial.println(RIGHT_UP);
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
       //digitalWrite(beel,LOW);
      Serial.print("Right held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.print("LEFT held this hard: ");
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
    }
    if(ps2x.Button(PSB_PAD_DOWN)){
//      Serial.print("DOWN held this hard: ");
//      Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
      digitalWrite(LEFT_DOWN,HIGH);
      digitalWrite(RIGHT_DOWN,HIGH);
      Serial.println(LEFT_DOWN);
      Serial.println(RIGHT_DOWN);
    
    } 

      if(ps2x.Button(PSB_SQUARE))   //如果方形被按下，则左转（一直按，一直转），48度之后停止。       
      {
        if(pos>141 || pos==141)
          pos=141;
        if(pos>48)
        {
          pos-=weidu;
          }  
        else
          pos=48;
       myservo.write(pos);
      Serial.println(pos, DEC);
      }
      
      
      //Serial.println(ps2x.Analog(PSB_SQUARE), DEC);
    if(ps2x.Button(PSB_CIRCLE))      //如果圆形被按下，则右转（一直按，一直转），141度之后停止。        
      //Serial.println(ps2x.Analog(PSB_CIRCLE), DEC);
      {
        if(pos<48 || pos==48)
          pos=48;
        if(pos<141)
          pos+=weidu;
        else
          pos=141;
          
          myservo.write(pos);
          Serial.println(pos, DEC);
      }



      
//这将根据您按下蓝色（X）按钮的力度设置大电机振动速度，如果任何按钮改变状态（on到off，或off到on），蓝色（X）按钮将为真
    vibrate = ps2x.Analog(PSAB_CROSS);  //这将根据您按下蓝色（X）按钮的力度设置大电机振动速度
    if (ps2x.NewButtonState()) {        //如果任何按钮改变状态（on到off或off到on），则为真
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed");
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed");
      if(ps2x.Button(PSB_L2))
        {//Serial.println("L2 pressed");
         if(weidu >2 && weidu<40)
            weidu-=3;
        else 
            weidu=3;
        }
      if(ps2x.Button(PSB_R2))
      {
        //Serial.println("R2 pressed");
        if(weidu >2 && weidu<40)
            weidu+=3;
        else 
            weidu=3;
        
      }
      if(ps2x.Button(PSB_TRIANGLE))
       myservo.write(105);
       // Serial.println("Triangle pressed");        
    }


    if(ps2x.NewButtonState(PSB_CROSS))               //如果按下或松开按钮，则为真
      Serial.println("X just changed");


    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { //如果其中一个为真，则打印条值
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  左斗杆，Y轴。其他选项：LX、RY、RX
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); 
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); 
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); 
   // if(ps2x.Analog(PSS_RX)==0)
   //  digitalWrite(beel,LOW);
    }     
  
  delay(50);  
}
