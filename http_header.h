/**
 * Get string value from Http header
 */
String getStringFromHeader(String header) {
  int colon = header.indexOf(':');
  String valuePart = header.substring(colon + 2); // "+2" for ": "
  valuePart.trim();

  return valuePart;
}

/**
 * Get integer value from Http header. E.g. "Content-Length: 186"
 */
long getIntegerValueFromHeader(String header) {
  String valuePart = getStringFromHeader(header);
  long result = valuePart.toInt();

  return result;
}

/**
 * Get char value from Http header
 */
char* getCharFromHeader(String header, int length) {
  String valuePart = getStringFromHeader(header);
  char * response = (char *) malloc(length);
  char st[length];
  valuePart.toCharArray(st, length);
  strcpy(response, st);  

  return response;
}


