#include <hexdump.h>
#include <Arduino.h>
#include <stdlib.h>

void hexDump (const char *desc, void *addr, int len) {
    char buf[32];
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;

    // Output description if given.
    if (desc != NULL)
        Serial.print(desc); Serial.print(" ");

    if (len == 0) {
        Serial.print("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        return;
    }

    // Process every byte in the data.
    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Just don't print ASCII for the zeroth line.
            if (i != 0) {
    			sprintf(buf, "  %s\n", buff);
    			Serial.println(buf);
	    }

            // Output the offset.
	    Serial.print("  ");
	    Serial.print(i, HEX);
	    Serial.print("  ");
        }

        // Now the hex code for the specific character.
	//
	Serial.print("  ");
	Serial.print(pc[i], HEX);

        // And store a printable ASCII character for later.
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }
    // Pad out last line if not exactly 16 characters.
    while ((i % 16) != 0) {
        Serial.print ("   ");
        i++;
    }

    // And print the final ASCII bit.
    sprintf(buf, "  %s\n", buff);
    Serial.println(buf);

}
