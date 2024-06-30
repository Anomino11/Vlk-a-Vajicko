//KNIHOVNY
#include "PinChangeInt.h" 
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <UTFT.h>
#include "SevSeg.h"
MCUFRIEND_kbv tft;
SevSeg myDisplay;
//BARVY
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0xD6
#define BROWN   0x79E0
#define DARKGREEN  0x03E0
//ANALOGY
#define pinX A14
#define pinY A15
extern int vlk[];
extern int vlk_opacne[];
extern int Pad_Vlka[];
extern int Pad_Vlka_opacne[];
extern int Krizek[];
extern int Travicka[];
extern int prekHlavyDva[];
//Srandy kolem
byte strike = 0;
int xhodnota = 0; //hodnota z packy x
int yhodnota = 0; // hodnota z packy y
byte vratitPozici = 1; 
long predVD;
unsigned long aktualniCas;
long timer;
int yD = 0;
int yJ = 0;
int yT = 0;
int yC = 0;
int rychlost = 1000;
byte aktualniPad = 0;
byte poziceV;
unsigned long casovac;
int aktualniSkore = 0;
bool pridat = false;
void setup() 
{
 
myDisplay.Begin(COMMON_CATHODE, 4, 35, 36, 37, 38, 28, 29, 30, 31, 32, 33, 34, 55); 
myDisplay.SetBrightness(30);
Serial.begin(9600);
randomSeed(analogRead(6)); 
//PCintPort::attachInterrupt(pinX, gameplay, FALLING); //HW přerušení 
//PCintPort::attachInterrupt(pinY, gameplay, FALLING);  //HW přerušení
tft.begin(0x9486); //kalibrace
tft.fillScreen(GREY);
tft.setRotation(1); // nastaví obrázek na správnou stranu
tft.setCursor(130, 3);
tft.setTextSize(3);
tft.println("VLK A VAJICKO"); 
tft.fillRect(0,235, 480, 270, DARKGREEN);
tft.fillRect(0,270, 480, 320, BROWN);
drawBitmap(20, 200, Travicka, 40, 40, DARKGREEN);
drawBitmap(180, 98,vlk, 120, 140, BLACK);
delay(500);
}

void loop() 
{
  aktualniCas = millis();
if(strike == 3)
{
        tft.fillScreen(GREY);
      tft.println("GAME OVER !"); 
      drawBitmap(180, 100,Pad_Vlka, 160, 100, BLACK);
      delay(10000);
  }
 if(strike != 3)
 {
   pad();
   krizek_vypis(strike);
   if(poziceV == 1)
   {
    strike = dropEggJedna(strike);
    }
    if(poziceV == 2)
    {
      strike = dropEggDva(strike);
      }
      if(poziceV == 3)
      {
        strike = dropEggTri(strike);
        }
    if(poziceV == 4)
    {
      strike = dropEggCtyri(strike);
      }
  }
}
void pad()
{
  poziceV = random(1, 5); //od 1 do 4 !
  if(aktualniSkore <= 20)
  {
    rychlost = 1000;
    }
    if(aktualniSkore > 20 && aktualniSkore <= 80)
    {
      rychlost = 500;
      }
  }
void drawBitmap(int16_t x, int16_t y, int *bitmap, int16_t w, int16_t h, uint16_t color) 
{
  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;
    if(w == 160 || w == 120)
    { 
//Serial.println("JSEM TADY");
      }
  for(j=0; j<h; j++) 
  {     
    //skore();

    for(i=0; i<w; i++) 
    {

      if(i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x80) tft.drawPixel(x+i, y+j, color);
    }
  }

}
byte dropEggCtyri(byte vratit)
{
   
  for (yC =20; yC<300; yC++)
  {
    skore();
    aktualniPad = 3;
    yC--;
       
      vratitPozici = ovladani(vratitPozici);
      aktualniCas = millis();
          if(( (unsigned long)(aktualniCas - predVD) >= rychlost))
      {
    yC = yC + 20;
    tft.fillRoundRect(285, yC-20, 18, 22, 8, GREY);//vymaže předchozí snímek
    tft.fillRoundRect(285, yC, 18, 22, 8, WHITE);
    
          predVD = (predVD * 0)+ aktualniCas;
          if(vratitPozici == 3 && (yC > 140 && yC < 160))
          {
            vykresleni(3);
            }
            if(vratitPozici == 4 && yC >= 140)
            {
              vykresleni(4);
              }
              if(vratitPozici == 1 && (yC > 140 && yC < 160))
              {
              vykresleni(1);
              }
        }
    if((vratitPozici !=3 && yC >= 180) || (vratitPozici ==3 && yC >= 180))
    {
      //nechytil
     vratit++; //přidá strike
     aktualniSkore = aktualniSkore - 3;
          tft.fillRoundRect(285, 199, 18, 22, 8, GREY);
          if(vratitPozici == 4)
          {
          vykresleni(4);
          }
          return vratit;
          skore();
           break;
      }
      if((vratitPozici == 3 && yC >= 140) && (vratitPozici == 3 && yC < 180))
      {
        //chytil
        pridat = true; // +5 bodu
        skore();
         tft.fillRoundRect(285, 159, 18, 22, 8, GREY);
         return vratit;
        break;
        }

              
  }
  }
byte dropEggTri(byte vratit)
{
   
  for (yT =20; yT<300; yT++)
  {
    skore();
    aktualniPad = 1;
    yT--;
       
      vratitPozici = ovladani(vratitPozici);
      aktualniCas = millis();
          if(( (unsigned long)(aktualniCas - predVD) >= rychlost))
      {
    yT = yT + 20;
    tft.fillRoundRect(180, yT-20, 18, 22, 8, GREY);//vymaže předchozí snímek
    tft.fillRoundRect(180, yT, 18, 22, 8, WHITE);
    
          predVD = (predVD * 0)+ aktualniCas;
          if(vratitPozici == 1 && (yT > 140 && yT < 160))
          {
            vykresleni(1);
            }
            if(vratitPozici == 2 && yT >= 160)
            {
              vykresleni(2);
              }
              if(vratitPozici == 3 && (yT > 140 && yT < 160))
              {
              vykresleni(3);
              }
        }
    if((vratitPozici !=1 && yT >= 180) || (vratitPozici ==1 && yT >= 180))
    {
      //nechytil
     vratit++; //přidá strike
     aktualniSkore = aktualniSkore - 3;
          tft.fillRoundRect(180, 199, 18, 22, 8, GREY);
          if(vratitPozici == 2)
          {
          vykresleni(2);
          }
          return vratit;
          skore();
           break;
      }
      if((vratitPozici == 1 && yT >= 140) && (vratitPozici == 1 && yT < 180))
      {
        //chytil
        pridat = true; // +5 bodu
        skore();
         tft.fillRoundRect(180, 159, 18, 22, 8, GREY);
         return vratit;
        break;
        }

              
  }
  }
byte dropEggDva(byte vratit)
{
 
  for (yD =20; yD<300; yD++)
  {
    skore();
    aktualniPad = 2;
    yD--;
       
      vratitPozici = ovladani(vratitPozici);
      aktualniCas = millis();
          if(( (unsigned long)(aktualniCas - predVD) >= rychlost))
      {
    yD = yD + 20;
    tft.fillRoundRect(100, yD-20, 18, 22, 8, GREY);//vymaže předchozí snímek
    tft.fillRoundRect(100, yD, 18, 22, 8, WHITE);
    
          predVD = (predVD * 0)+ aktualniCas;
          if(vratitPozici == 2 && yD >= 140)
          {
            vykresleni(2);
            }
        }
    if(vratitPozici !=2 && yD >= 180)
    {
      //nechytil
     vratit++; //přidá strike
     aktualniSkore = aktualniSkore - 3;
          tft.fillRoundRect(100, 199, 18, 22, 8, GREY);
          return vratit;
          skore();
           break;
      }
      if(vratitPozici == 2 && yD >= 180)
      {
        //chytil
        pridat = true; // +5 bodu
        skore();
         tft.fillRoundRect(100, 199, 18, 22, 8, GREY);
        break;
        }

              
  }

}
  byte dropEggJedna(byte vratit)
{
  
  for (int yJ =20; yJ<300; yJ++)
  {
    skore();
    aktualniPad = 4;
    yJ--;
     vratitPozici = ovladani(vratitPozici);
     aktualniCas = millis();
     if(((unsigned long)(aktualniCas - predVD) >= rychlost))
      {
    yJ = yJ + 20;
    tft.fillRoundRect(380, yJ-20, 18, 22, 8, GREY);
    tft.fillRoundRect(380, yJ, 18, 22, 8, WHITE);
            predVD = (predVD * 0)+ aktualniCas;
       if(vratitPozici == 4 && yJ >= 140)
          {
            vykresleni(4);
            }
      }
    if(vratitPozici !=4 && yJ >= 180)
    {
      //NECHYTIL
      
     vratit++; //přidá strike
     
     
     aktualniSkore = aktualniSkore - 3;
     
     tft.fillRoundRect(380, 199, 18, 22, 8, GREY);
     return vratit;
     skore();
     
                break;
      }
     else if(vratitPozici == 4 && yJ >= 180)
      {
        //chytil
        pridat = true; //+5 bodů
        skore();
         tft.fillRoundRect(380, 199, 18, 22, 8, GREY);
        break;
        }

        }
    
  }
  void krizek_vypis(byte chyba)
  {
    if(chyba == 1)
    {
     drawBitmap(20, 10, Krizek, 40, 40, RED); 
    }
    if(chyba == 2)
    {
       drawBitmap(40, 10, Krizek, 40, 40, RED); 
      }
    }
        
      byte ovladani(byte pozice)
      {
        byte predchozi = (predchozi * 0) + pozice; //ulozime do vynulovaneho predchoziho stavu stav novy
      xhodnota = analogRead(pinX); //precte
      yhodnota = analogRead(pinY); //precte
      if(((xhodnota <= 506 && xhodnota >= 497) && (yhodnota >= 501 && yhodnota <= 502)))
      {
       vykresleni(predchozi);
        }
    else
     {   
      if(yhodnota <= 512)
      {
        if(xhodnota <= 512 && yhodnota <= 10)
        {   
        pozice = 3; //PRAVO NAHORE
        mazani(pozice, predchozi);
        vykresleni(pozice);
          }
          if(!(xhodnota <= 512))
          {
             
            pozice = 4; //PRAVO DOLE
             mazani(pozice, predchozi);
             vykresleni(pozice);
          }
        }
         
     
        if(yhodnota <= 1023 && yhodnota > 512)
        {
          if(xhodnota  <= 512)
          {    
        pozice = 1; // LEVO NAHORE
         mazani(pozice, predchozi);
         vykresleni(pozice);
          }
          
          if(!(xhodnota <= 512))
          {    
            pozice = 2; //LEVO DOLE
             mazani(pozice, predchozi); 
             vykresleni(pozice);
            }
        }
     }     
            return pozice;
        }

        void mazani(byte nova, byte pojistka)
        {
        if(nova != pojistka)
        {
          skore();
          if(pojistka == 1)
          {
            tft.fillRect(180,95,120,140, GREY);
          }
          if(pojistka == 2)
          {
            tft.fillRect(100,133,160,100, GREY);

          }
          if(pojistka == 3)
          {
            tft.fillRect(180,95,120,140, GREY);
          }
          if(pojistka == 4)
          {
            tft.fillRect(240,133,160,100, GREY);
            
          }
          }
          }
          
      void vykresleni(byte aktualni)
      {
        if(aktualni == 1)
        {
          drawBitmap(180, 98,vlk, 120, 140, BLACK);
          }
          
        if(aktualni == 2)
        {
          drawBitmap(100, 137,Pad_Vlka, 160, 100, BLACK); 
          }
          
        if(aktualni == 3)
        {
          drawBitmap(180, 98,vlk_opacne, 120, 140, BLACK);
          }
          
        if(aktualni == 4)
        {
          drawBitmap(240, 137,Pad_Vlka_opacne, 160, 100, BLACK);

          }
        }
void skore()
        {         
           if (millis() - timer >= 10) //zrychluji načtení
  {
  char tempString[10];
        if(aktualniSkore < 0)
      {
        aktualniSkore = aktualniSkore * 0;
        }
  if(pridat == true)
    {


    aktualniSkore = aktualniSkore + 5;
        
    pridat = false;
    }
  sprintf(tempString, "%4d", aktualniSkore); //Convert deciSecond into a string that is right adjusted
  myDisplay.DisplayString(tempString, 3);

    timer = millis();
         
  }
          }
