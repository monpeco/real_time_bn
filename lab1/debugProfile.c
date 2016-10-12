uint32_t Debug_time[20];
uint8_t Debug_place[20];
uint32_t n;

void Debug_Profile(uint8_t p){
  if(n < 20){
    Debug_time[n] = STCURRENT; // record current time
    Debug_place[n] = p;
  n++;
  }
}

uint32_t sqrt(uint32_t s){
uint32_t t; // t*t becomes s
int n; // loop counter 
  Debug_Profile(0);
  t = s/10+1; // initial guess 
  Debug_Profile(1);
  for(n = 16; n; --n){ // will finish
    Debug_Profile(2);
    t = ((t*t+s)/t)/2; 
  }
  Debug_Profile(3);
  return t; 
}