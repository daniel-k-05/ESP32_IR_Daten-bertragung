/*
 * 
 * https://www.reddit.com/r/arduino/comments/krv55r/convert_an_int_to_a_binary_array/
 * https://forum.arduino.cc/t/byte-array-to-string/129708/6
 *  
 * daniel-k-05
 */



String input = "AHello World!";
String sendData = " ";

const int send_ledPin = 15;           //Pin wahl ist bisher willkürlich
const int recieve_sensorPin = 20;
const int debug_ledPin = 17;
const int posPin = 5;



const int blockLength = 5;  // so in the end it's 5+1
const int ZeitProZeichen = 500;
int clkSpeed = ZeitProZeichen / 6;


void setup() {
  //Anfangs Setup
  pinMode(send_ledPin, OUTPUT);     // ggf. mit einem pin vorher testen und nach gegebenem fall zwischen in-/output wechseln (Jumper PIN)
  pinMode(debug_ledPin, OUTPUT);
  pinMode(posPin, INPUT);

  Serial.begin(9600);
  delay(500);
  
  if(digitalRead(posPin) == LOW){
    Serial.println("MODE :: SEND");
    Serial.println(" ");
    Serial.println("Input Message: " + input);   //Später ggf mit Serial oder App Input
  }
  else{
    Serial.println("MODE :: READ");
  }


  input.toUpperCase();
  sendData = generateData(input);
  Serial.println("Compiled Code: " + sendData);
}


void loop() {
  if(digitalRead(posPin) == LOW){
    sendMSG();
  }
  else{    
    readMSG();
  }
}


String generateData(String message){
  String compiledMessage = " ";
    
  for (int i = 0; i < message.length(); i++){
    // + "1"
    compiledMessage = compiledMessage + "1";
    if((int)message.charAt(i) - 64 > 0 && (int)message.charAt(i) - 64 < 65){
      compiledMessage = compiledMessage + intToBinary((int)message.charAt(i) - 64);     // + "(Ascii wert - 64 --> binär)"
    }
    else{
      compiledMessage = compiledMessage + "00000";
    }

    //   \/ DEBUG \/
    Serial.print(message.charAt(i)); 
    Serial.print("; ");
    //Serial.print((int)message.charAt(i)); 
    Serial.print("; ");
    Serial.print((int)message.charAt(i) - 64);
    Serial.print("; ");
    Serial.println(intToBinary((int)message.charAt(i) - 64));
    //   /\ DEBUG /\

  }
  return (compiledMessage);
}


String intToBinary(int num){  
  byte byteArray[blockLength] = {};
  int i; 
  String str; 
  for (i = blockLength - 1; i > 0; i--) {  //Convert the Int to a Binary
    byteArray[i] = num%2;
    num=num/2;
  }  
  for (int c : byteArray) str += c;        //Convert the Array to a string
  return(str);
}


void sendMSG(){
  Serial.println("Starting from the beginning");
  digitalWrite(send_ledPin, LOW);       
  digitalWrite(debug_ledPin, LOW);
  delay(5*6*clkSpeed);    // 5 leer durchläufe

  // Sende Alle Daten
  for(int i = 0; i < sendData.length(); i++){
    switch ((int)sendData.charAt(i)){
      case ('1'):
        digitalWrite(send_ledPin, HIGH);       
        digitalWrite(debug_ledPin, HIGH);
        Serial.print("1"); 
        break;          
      case ('0'):
        digitalWrite(send_ledPin, LOW);       
        digitalWrite(debug_ledPin, LOW);
        Serial.print("0");
        break;  
      default:
        break;
    }
    delay(clkSpeed);
  } 
  Serial.println();
  Serial.println("Message Sent Succesfully"); 
  Serial.println("--------------------------------------------------------");
  return;
}


void readMSG(){
  Serial.println("Message Succesfully Recieved");
  return;
}
