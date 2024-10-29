int countDigits(int);
void addOffsetToTime(int &heures, int &minutes, int offset);
String formatTime(int timeNumber);
void convertTimeStringToInts(const String& timeStr, int& hours, int& minutes);
bool parseDaysString(const String& daysStr, bool daysArray[], int& arraySize);

void sleep_for(int seconds);
void set_hv_sw(bool state);