//Wire end is right side
//non-wire end is left side

//NOTEEEEE: Case Right should work button broken need to fix that

#include <NeoPixelBus.h>
#define RIGHT     0
#define LEFT      1
#define START     12
#define STOP     4
#define POINT      5
#define POSTART     0 //NOt sure what this is!!!

#define RT_BUTTON 5
#define LF_BUTTON 6
#define SAFEZONE  15 //number of pixels in the safe zone on either side
#define STRIP_LENGTH  130 //Total number of pixels in strip
#define STRIP_END STRIP_LENGTH -1
#define RIGHT_OFFSET   2 //This is the offset from the right side
#define LEFT_OFFSET   2//This is the offset from the left

#define POSEND 129

#define LEFTZONECOLOR  orange // color of safe zone
#define RIGHTZONECOLOR blue // color of safe zone
#define BALLCOLOR  white // color of dot moving back and forth
#define SCOREPIXELCOLOR  red // color of safe zone when player messes up

const uint16_t PixelCount = STRIP_LENGTH;

const uint8_t DotClockPin = 13;
const uint8_t DotDataPin = 11;

NeoPixelBus<DotStarBgrFeature, DotStarSpiMethod> strip(PixelCount);
RgbColor red(30, 0, 0);
RgbColor orange(55, 13, 0);
RgbColor blue(0, 50, 38);
RgbColor white(255);
RgbColor black(0);
RgbColor purple (225, 0, 175);


int game_state = START;
int ball = 65; //Middle pixel
int x = 100; //100
int speedPixel = 10000;
int rightScore = 0;
int leftScore = 0;
int z = 143;
int y = 0;
int t = 0;
int sound;
long randside;
void setup() {
  pinMode(LF_BUTTON, INPUT);
  //digitalWrite(6, HIGH);

  pinMode(RT_BUTTON, INPUT);
  //digitalWrite(5, HIGH);

  Serial.begin(115200);

  strip.Begin();
  strip.ClearTo(black);
  strip.Show();
  // while (1);

}


void loop() {
  ///  Serial.print(rightScore);
  // Serial.print(':');
  //  Serial.println(leftScore);
  //Reseting zones in all states including scores

  //Set up right zone
  for (int y = RIGHT_OFFSET ; y < RIGHT_OFFSET + SAFEZONE; y++) {
    strip.SetPixelColor( y, LEFTZONECOLOR);
  }

  //Set up left zone
  for (int y = STRIP_END - LEFT_OFFSET ; y > STRIP_END - (LEFT_OFFSET + SAFEZONE); y--) {
    strip.SetPixelColor(  y, RIGHTZONECOLOR);
  }

  //Sets color for score on right side
  for (int y = RIGHT_OFFSET; y < RIGHT_OFFSET + rightScore; y++) {
    strip.SetPixelColor(y, SCOREPIXELCOLOR );
   
  }

  //Sets color for score on left side
  for (int y = STRIP_END - LEFT_OFFSET; y > STRIP_END - (LEFT_OFFSET + leftScore); y--) {
    // strip.SetPixelColor( (STRIP_LENGTH - LEFT_OFFSET) - y, SCOREPIXELCOLOR );
    strip.SetPixelColor(y, LEFTZONECOLOR);

  }

  switch (game_state) {
    case START:

      //This will flash the ball continuously until a button is pressed.
      //NOTE: Probably better way to do this. The causes the button to not read a press in time
      strip.SetPixelColor(ball, BALLCOLOR);
      strip.Show();
      // delay (500);
      strip.SetPixelColor(ball, black);
      strip.Show();
      // delay (500);

      //  randside = random(3);
      //Serial.println(randside);
      /*
               if (randside <= 1)
                 game_state = LEFT;
               else
                 game_state = RIGHT;
      */
      if (digitalRead(LF_BUTTON) == HIGH) { // when left button is clicked the ball goes to the right
        game_state = RIGHT;
       pinMode(2, OUTPUT);
  for (sound = 0; sound < 500; sound++) {
    digitalWrite(2, HIGH);
    delayMicroseconds(100);
    digitalWrite(2, LOW);
    delayMicroseconds(sound);
    //tone(2,1000);
  } 
      }

      if (digitalRead(RT_BUTTON) == HIGH) {// when the right button is clicked the ball will go to the left side
        game_state = LEFT;
         pinMode(2, OUTPUT);
  for (sound = 0; sound < 500; sound++) {
    digitalWrite(2, HIGH);
    delayMicroseconds(100);
    digitalWrite(2, LOW);
    delayMicroseconds(sound);
    //tone(2,1000);
  }
      }


      // game_state = RIGHT;
      break;
    case RIGHT: // this means the ball will move to the right/ towards oragne

      //Move pixel one step left until it reaches the end of the right side
      if (ball-- < RIGHT_OFFSET) ball = RIGHT_OFFSET;
      strip.SetPixelColor(ball, BALLCOLOR );
      strip.Show();
      delayMicroseconds(speedPixel);
      strip.SetPixelColor(ball, black);

      //when right button is pressed the ball will move left
      if (digitalRead(RT_BUTTON) == HIGH) {
         pinMode(2, OUTPUT);
  for (sound = 0; sound < 500; sound++) {
    digitalWrite(2, HIGH);
    delayMicroseconds(100);
    digitalWrite(2, LOW);
    delayMicroseconds(sound);
    //tone(2,1000);
  }
        
        // Serial.println("right");
        // speedPixel -= 200;    //Increase speed.

        if (speedPixel < 1) speedPixel = 1;  //Limit lowest speed to one

        //Check if button pressed in safe zone
        if (( ball < SAFEZONE) && ( ball > RIGHT_OFFSET)) {
          game_state = LEFT;
        }
        else {
          rightScore++;
          Serial.println(rightScore);

          game_state = POINT;

        }
      }

      break;
    //case left means the ball is moving left
    case LEFT:
      //ball++;
      // moving ball left amd checking if it is going off the end
      if (ball++ > STRIP_LENGTH - LEFT_OFFSET) ball = STRIP_LENGTH - LEFT_OFFSET;
      strip.SetPixelColor(ball, BALLCOLOR);
      strip.Show();
      delayMicroseconds(speedPixel);
      strip.SetPixelColor(ball, black);

      if (digitalRead(LF_BUTTON) == HIGH) {
         pinMode(2, OUTPUT);
  for (sound = 0; sound < 500; sound++) {
    digitalWrite(2, HIGH);
    delayMicroseconds(100);
    digitalWrite(2, LOW);
    delayMicroseconds(sound);
    //tone(2,1000);
  }
         speedPixel -= 200;

           if (speedPixel < 1) speedPixel = 1;
        //checking to see if the ball is in the safe zone
        if (( ball > STRIP_LENGTH - SAFEZONE) && (ball < STRIP_LENGTH - LEFT_OFFSET)) {
          game_state = RIGHT;
        }
        else {
          leftScore++;
          Serial.println(leftScore);

          game_state = POINT;
        }
      }

      break;
    case POINT:
      Serial.println("point");
      //fix
      if (rightScore > 4 ) {
        //  strip.Begin();
        for (int x = 0; x < 20; x++) {
          strip.ClearTo(RIGHTZONECOLOR);
          strip.Show();
          delay(100);
          strip.ClearTo(black);
          strip.Show();
          delay(100);
        }
        for (int x = 0; x < 10; x++) {
          for (int z = 143; z > RIGHT_OFFSET; z--) {
            strip.SetPixelColor(z, white );
            strip.Show();
            delayMicroseconds(2000);
            strip.SetPixelColor(z + 4, RIGHTZONECOLOR);
            delayMicroseconds(10);
            strip.SetPixelColor(z + 25, black);
            delayMicroseconds(10);

          }
          strip.ClearTo(black);
          strip.Show();
        }


        game_state = STOP;
      }
      if (leftScore > 4) {
        for (int x = 0; x < 20; x++) {
          strip.ClearTo(LEFTZONECOLOR);
          strip.Show();
          delay(100);
          strip.ClearTo(black);
          strip.Show();
          delay(100);
        }
        for (int x = 0; x < 10; x++) {
          for (int t = 0; t < STRIP_LENGTH - LEFT_OFFSET; t++) {
            strip.SetPixelColor(t, white );
            strip.Show();
            delayMicroseconds(2000);
            strip.SetPixelColor(t - 4, LEFTZONECOLOR);
            delayMicroseconds(10);
            strip.SetPixelColor(t - 25, black);
            delayMicroseconds(10);
          }
          strip.ClearTo(black);
          strip.Show();

        }

        game_state = STOP;
      }



      if ((digitalRead(RT_BUTTON) == HIGH) || (digitalRead(LF_BUTTON) == HIGH)  ) {
        game_state = START;
        ball = 65;

      }

      break;

    case STOP:
      z = 143;
      t = 0;
      speedPixel = 10000;
      y = 0;
      rightScore = 0;
      leftScore = 0;
      ball = 65; // starting pixel of white dot
      game_state = START;

      break;

  }
}
