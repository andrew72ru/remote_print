char* RUS(char* str){                                                                                                                      // Определяем функцию которая преобразует код русских символов из кодировки UTF-8 в кодировку CP866
    uint8_t i=0, j=0;                                                                                                                      // Определяем переменные: i - счетчик входящих символов, j - счетчик исходящих символов
    while(str[i]){                                                                                                                         // Проходим по всем символам строки str, пока не встретим символ конца строки (код 0)
        if(uint8_t(str[i]) == 0xD0 && uint8_t(str[i+1]) >= 0x90 && uint8_t(str[i+1]) <= 0xBF ){str[j] = (uint8_t) str[i+1]-0x10; i++;}else // Символы «А-Я а-п» (код UTF-8: D090-D0AF D0B0-D0BF) сохраняем в кодировке CP866: код 80-9F A0-AF (символ занимал 2 байта, а стал занимать 1 байт)
        if(uint8_t(str[i]) == 0xD1 && uint8_t(str[i+1]) >= 0x80 && uint8_t(str[i+1]) <= 0x8F ){str[j] = (uint8_t) str[i+1]+0x60; i++;}else // Символы «р-я»     (код UTF-8: D180-D18F)           сохраняем в кодировке CP866: код E0-EF       (символ занимал 2 байта, а стал занимать 1 байт)
        if(uint8_t(str[i]) == 0xD0 && uint8_t(str[i+1]) == 0x81                              ){str[j] = 0xF0;                    i++;}else // Символ «Ё»        (код UTF-8: D081)                сохраняем в кодировке CP866: код F0          (символ занимал 2 байта, а стал занимать 1 байт)
        if(uint8_t(str[i]) == 0xD1 && uint8_t(str[i+1]) == 0x91                              ){str[j] = 0xF1;                    i++;}else // Символ «ё»        (код UTF-8: D191)                сохраняем в кодировке CP866: код F1          (символ занимал 2 байта, а стал занимать 1 байт)
                                                                                              {str[j] = (uint8_t) str[i];}  j++; i++;      // Остальные символы оставляем как есть, без преобразования, но их место в строке могло сдвинуться, если до них были встречены русские символы
    }   while(j<i){str[j]=0; j++;} return str;                                                                                             // Так как место занимаемое символами в строке могло уменьшиться, заполняем оставщиеся байты символами конца строки (код 0)
}

