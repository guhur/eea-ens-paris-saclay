
void initialize_stepper() {
  // initialize the 8 pin as an output:
  pinMode(STEPPER1, OUTPUT);
  pinMode(STEPPER2, OUTPUT); 
  pinMode(STEPPER3, OUTPUT); 
  pinMode(STEPPER4, OUTPUT);

  step_OFF();         //turning all coils off
}


//these functions set the pin settings for each of the four steps per rotation of the motor (keepp in mind that the motor in the kit is geared down,
//i.e. there are many steps necessary per rotation

void Step_A(){
  digitalWrite(STEPPER1, HIGH);//turn on coil 1 
  digitalWrite(STEPPER2, LOW); 
  digitalWrite(STEPPER3, LOW); 
  digitalWrite(STEPPER4, LOW); 
}
void Step_B(){
  digitalWrite(STEPPER1, LOW); 
  digitalWrite(STEPPER2, HIGH);//turn on coil 2
  digitalWrite(STEPPER3, LOW); 
  digitalWrite(STEPPER4, LOW); 
}
void Step_C(){
  digitalWrite(STEPPER1, LOW); 
  digitalWrite(STEPPER2, LOW); 
  digitalWrite(STEPPER3, HIGH); //turn on coil 3
  digitalWrite(STEPPER4, LOW); 
}
void Step_D(){
  digitalWrite(STEPPER1, LOW); 
  digitalWrite(STEPPER2, LOW); 
  digitalWrite(STEPPER3, LOW); 
  digitalWrite(STEPPER4, HIGH); //turn on coil 4
}
void step_OFF(){
  digitalWrite(STEPPER1, LOW); //power all coils down
  digitalWrite(STEPPER2, LOW); 
  digitalWrite(STEPPER3, LOW); 
  digitalWrite(STEPPER4, LOW); 
}

//these functions run the above configurations in forward and reverse order
//the direction of a stepper motor depends on the order in which the coils are turned on.
void forward(){//one tooth forward
  Step_A();
  delay(STEPPERDELAY);
  Step_B();
  delay(STEPPERDELAY);
  Step_C();
  delay(STEPPERDELAY);
  Step_D();
  delay(STEPPERDELAY);
}

void backward(){//one tooth backward
  Step_D();
  delay(STEPPERDELAY);
  Step_C();
  delay(STEPPERDELAY);
  Step_B();
  delay(STEPPERDELAY);
  Step_A();
  delay(STEPPERDELAY);
}

