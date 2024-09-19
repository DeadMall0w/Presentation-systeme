#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeOrion.h>

// Initialisation nécessaire pour le bon fonctionnement du système (créé automatiquement par mBlock)
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;

// Initialisation des différentes fonctions
void Initialisation();
void ChangeSlowDistance(double count);
void ChangeStopDistance(double count);
void SetUpSlowDistance();
void SetUpStopDistance();

// InInitialisation des variables limites de ralentissement et d'arrêt selon la distance capté par le capteur ultrason 
double stopDistance;
double slowDistance;

// Initialisation des différents capteurs et actionneurs 
MeRGBLed rgbled_7(7, 4); // 4 leds RGB
MeJoystick joystick_6(6); // Joystick
Me7SegmentDisplay seg7_4(4); //afficheur 7 segment
MeDCMotor motor_9(9); // moteur
MeUltrasonicSensor ultrasonic_3(3);  // capteur ultrasons

// Définir les valeurs par défaut de la distance d'arrêt et de ralentissement
void Initialisation()
{
    stopDistance = 20;
    slowDistance = 40;
}

// Fonction de changement de la distance de ralentissement et vérification des bornes
void ChangeSlowDistance(double count)
{
    slowDistance += count;

    // Verifie si 'slowDistance' ne depasse pas la limite (80)
    if((slowDistance) > (80)){
        slowDistance = 80;
    }else{
        // Verifie si 'slowDistance' ne descend pas en dessous la limite (20)
        if((slowDistance) < (20)){
            slowDistance = 20;
        }
        // Permet de s'assurer que la distance de ralentissement est 5 cm plus grande que la distance d'arret
        if((slowDistance) < ((stopDistance) + (5))){
            slowDistance = (stopDistance) + (5);
        }
    }
}

// Fonction de changement de la distance d'arret et vérification des bornes
void ChangeStopDistance(double count)
{
    stopDistance += count;

    if((stopDistance) > (40)){
        stopDistance = 40;
    }else{
        if((stopDistance) < (10)){
            stopDistance = 10;
        }
    }
}

// Fonction appelée quand l'utilisateur veut modifie la distance de ralentissement
void SetUpSlowDistance()
{
    // Change la couleur des LEDs pour informer l'utilisateur de la modification de la LED
    rgbled_7.setColor(2,255,217,5);
    rgbled_7.show();
    rgbled_7.setColor(4,255,217,5);
    rgbled_7.show();
    ChangeSlowDistance(0);
    _delay(1);
    while(!((joystick_6.read(2)) > (440)))
    {
        _loop();
        _delay(0.1);
        seg7_4.display((float)slowDistance);
        if((joystick_6.read(1)) > (440)){
            ChangeSlowDistance(0.5);
        }else{
            if((joystick_6.read(1)) > (220)){
                ChangeSlowDistance(0.2);
            }else{
                if((joystick_6.read(1)) > (110)){
                    ChangeSlowDistance(0.05);
                }
            }
        }
        if((joystick_6.read(1)) < (-440)){
            ChangeSlowDistance(-0.5);
        }else{
            if((joystick_6.read(1)) < (-220)){
                ChangeSlowDistance(-0.2);
            }else{
                if((joystick_6.read(1)) < (-110)){
                    ChangeSlowDistance(-0.05);
                }
            }
        }
    }
    seg7_4.display((float)slowDistance);
}

void SetUpStopDistance()
{
    rgbled_7.setColor(2,255,0,0);
    rgbled_7.show();
    rgbled_7.setColor(4,255,0,0);
    rgbled_7.show();
    while(!((joystick_6.read(2)) > (440)))
    {
        _loop();
        _delay(0.1);
        seg7_4.display((float)stopDistance);
        if((joystick_6.read(1)) > (440)){
            ChangeStopDistance(0.5);
        }else{
            if((joystick_6.read(1)) > (220)){
                ChangeStopDistance(0.2);
            }else{
                if((joystick_6.read(1)) > (110)){
                    ChangeStopDistance(0.05);
                }
            }
        }
        if((joystick_6.read(1)) < (-440)){
            ChangeStopDistance(-0.5);
        }else{
            if((joystick_6.read(1)) < (-220)){
                ChangeStopDistance(-0.2);
            }else{
                if((joystick_6.read(1)) < (-110)){
                    ChangeStopDistance(-0.05);
                }
            }
        }
    }
    seg7_4.display((float)stopDistance);
}

void setup(){
    motor_9.run(0);
    rgbled_7.setColor(1,0,0,0);
    rgbled_7.show();
    rgbled_7.setColor(3,0,0,0);
    rgbled_7.show();
    Initialisation();
    SetUpStopDistance();
    SetUpSlowDistance();
    rgbled_7.setColor(2,0,255,0);
    rgbled_7.show();
    rgbled_7.setColor(4,0,255,0);
    rgbled_7.show();
}

void loop(){
    seg7_4.display((float)ultrasonic_3.distanceCm());
    if((joystick_6.read(2)) < (-440)){
        motor_9.run(0);
        rgbled_7.setColor(1,0,0,0);
        rgbled_7.show();
        rgbled_7.setColor(3,0,0,0);
        rgbled_7.show();
        SetUpStopDistance();
        SetUpSlowDistance();
        rgbled_7.setColor(2,0,255,0);
        rgbled_7.show();
        rgbled_7.setColor(4,0,255,0);
        rgbled_7.show();
    }
    rgbled_7.setColor(1,255,0,0);
    rgbled_7.show();
    rgbled_7.setColor(3,255,0,0);
    rgbled_7.show();
    if((ultrasonic_3.distanceCm()) < (stopDistance)){
        rgbled_7.setColor(2,0,0,0);
        rgbled_7.show();
        rgbled_7.setColor(4,0,0,0);
        rgbled_7.show();
        motor_9.run(0);
    }else{
        if((ultrasonic_3.distanceCm()) < (slowDistance)){
            motor_9.run(45);
            rgbled_7.setColor(1,217,5,255);
            rgbled_7.show();
            rgbled_7.setColor(3,217,5,255);
            rgbled_7.show();
        }else{
            motor_9.run(90);
            rgbled_7.setColor(1,0,255,0);
            rgbled_7.show();
            rgbled_7.setColor(3,0,255,0);
            rgbled_7.show();
        }
    }
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
}
