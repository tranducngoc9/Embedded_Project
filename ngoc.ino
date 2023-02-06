//Thư viện dành cho LCD
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Thư viện của RFID
int UID[4], i;
int ID1[4] = {236, 186, 76, 46}; //Thẻ trẻ em
int Kid_card = random(2,4);
int ID2[4] = {253, 208, 132, 175}; // Thẻ người lớn
int Adult_card = random(2,4);
#include <SPI.h>   // Chuẩn giao tiếp của RFID là SPI
#include <MFRC522.h> // Thư viện này cũng liên quan đến RFID

//Thư viện của Servo Motor
#include <Servo.h>  

// 0x27 là địa chỉ mặc định của màn hình
// Màn hình là kích thức là 16x2 nên để là 16 ,2
//Tạo thuộc tính tên lcd
LiquidCrystal_I2C lcd(0x27, 16, 2); //Thiết lập địa chỉ và loại LCD
#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

#define SS_PIN D2  //--> SDA / SS is connected to pinout D2
#define RST_PIN D1  //--> RST is connected to pinout D1
MFRC522 mfrc522(SS_PIN, RST_PIN);  //--> Create MFRC522 instance.

Servo myservo;

void setup() {
  Serial.begin(115200); //--> Initialize serial communications with the PC   ! cái này ko quan trọng lắm
  SPI.begin();      //--> Init SPI bus   // Khởi động SPI
  Wire.begin(2, 0);  // Đặt chân SDA và SCL cho đường truyền tín hiệu. số 2 là GPIO2, số 0 là GPIO0 . 
                    // Có thể Custom chân nào cũng được, miễn là chân GPIO là được

  myservo.attach(15);
  mfrc522.PCD_Init(); //--> Init MFRC522 card
  delay(500);
  Serial.println("");
  lcd.clear();
  lcd.init();                 //Bắt đầu màn hình
  lcd.backlight();            // Bật đèn nền
  lcd.setCursor(0, 0); lcd.print("    Welcome     ");
  lcd.setCursor(0, 1); lcd.print("Quet The Tai Day");
//  lcd.display();              // Hiển thị lên màn hình.
  lcd.blink();                // Nhấp nháy con trỏ ở vị trí cuối cùng
  pinMode(ON_Board_LED, OUTPUT);

}
void loop()
{
    // Nếu để thẻ mới vào, đọc 1 lần thôi; nếu để thẻ lâu thì sẽ nhận biết là thẻ đọc rồi không đọc tiếp nữa
  // Nhận biết thẻ mới thẻ cũ
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  { 
    return; 
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {  
    return;  
  }
  //Đổi UID từ Hexa sang hệ 10
  Serial.print("UID của thẻ: ");  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  { 
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");   
    UID[i] = mfrc522.uid.uidByte[i];
    Serial.print(UID[i]);
    Serial.print("\n");
  }
  if (UID[i] == ID1[i])
  {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("  The Tre Em!!");

    if (Kid_card > 0)
    {
      lcd.setCursor(0, 1); lcd.print("So du: ");
      Kid_card -= 1;
      lcd.print(Kid_card*10000);
    }
    else{
      lcd.setCursor(0, 1); lcd.print("So Du Khong Du! ");  
      return;
      }
    lcd.display();
    myservo.write(0); delay(3000);
    myservo.write(90);delay(250); 
  }
  else if (UID[i] == ID2[i])
  {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("  The Nguoi Lon!!");

    if (Adult_card > 0)
    {
      lcd.setCursor(0, 1); lcd.print("So du: ");
      Adult_card -= 1;
      lcd.print(Adult_card*20000);
    }
    else{
      lcd.setCursor(0, 1); lcd.print("So Du Khong Du! ");  
      return;
      }
    lcd.display();
    myservo.write(0); delay(3000);
    myservo.write(90);delay(250); 
  }

  else
  {
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("    Sai The!");
    lcd.setCursor(0, 1); lcd.print("   Nhap Lai!");
    lcd.display();
    return;

  }

  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    mfrc522.PICC_HaltA();  
  mfrc522.PCD_StopCrypto1();
}
