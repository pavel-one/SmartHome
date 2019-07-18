/* Залить все ленту */
void fillAll() {
    strip.clear();
    for (int i = 0; i < stripCount; i++ ) {   // всю ленту
    strip.setPixelColor(i, strip.Color(red, green, blue));     // залить белым
    }
    strip.show();
    strobo_start = false;
}

/* Залить только часть */
void fillHalf(bool one = true) {
    //Если первая часть
    if (one)
    {
        for (int i = 0; i < (stripCount / 2); i++ ) {   // всю ленту
            strip.setPixelColor(i, strip.Color(red, green, blue));     // залить белым
        }
        strip.show();
    } else {
        for (int i = stripCount / 2; i < stripCount; i++ ) {   // всю ленту
        strip.setPixelColor(i, strip.Color(red, green, blue));     // залить белым
        }
        strip.show();
    }
    strobo_start = false;
}

/* Стробоскоп */
void strobo() {
  if (strobo_start) {
    if ((mls % frequency) == 0)
    {
      if (strobo_flag)
      {
        strip.clear();
        for (int i = 0; i < stripCount; i++ ) {
          strip.setPixelColor(i, strip.Color(red, green, blue));
        }
        strip.show();
        strobo_flag = false;
      } else {
        strip.clear();
        for (int i = 0; i < stripCount; i++ ) {
          strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
        strip.show();
        strobo_flag = true;
      }
      
    }
    
  }
}