#include "Pagedata.h"
#include "OBP60ExtensionPort.h"

class PageClock : public Page
{
bool keylock = false;               // Keylock
int16_t lp = 80;                    // Pointer length

public:
    PageClock(CommonData &common){
        common.logger->logDebug(GwLog::LOG,"Show PageClock");
    }

    // Key functions
    virtual int handleKey(int key){
        // Keylock function
        if(key == 11){              // Code for keylock
            keylock = !keylock;     // Toggle keylock
            return 0;               // Commit the key
        }
        return key;
    }

    virtual void displayPage(CommonData &commonData, PageData &pageData)
    {
        GwConfigHandler *config = commonData.config;
        GwLog *logger=commonData.logger;

        static String svalue1old = "";
        static String unit1old = "";
        static String svalue2old = "";
        static String unit2old = "";

        // Get config data
        String lengthformat = config->getString(config->lengthFormat);
        bool simulation = config->getBool(config->useSimuData);
        String displaycolor = config->getString(config->displaycolor);
        bool holdvalues = config->getBool(config->holdvalues);
        String flashLED = config->getString(config->flashLED);
        String backlightMode = config->getString(config->backlight);
        int timezone = config->getInt(config->timeZone);

        // Get boat values for GPS time
        GwApi::BoatValue *bvalue1 = pageData.values[0]; // First element in list (only one value by PageOneValue)
        String name1 = bvalue1->getName().c_str();      // Value name
        name1 = name1.substring(0, 6);                  // String length limit for value name
        double value1 = bvalue1->value;                 // Value as double in SI unit
        bool valid1 = bvalue1->valid;                   // Valid information 
        String svalue1 = formatValue(bvalue1, commonData).svalue;    // Formatted value as string including unit conversion and switching decimal places
        String unit1 = formatValue(bvalue1, commonData).unit;        // Unit of value
        if(valid1 == true){
            svalue1old = svalue1;   	                // Save old value
            unit1old = unit1;                           // Save old unit
        }

        // Get boat values for GPS date
        GwApi::BoatValue *bvalue2 = pageData.values[1]; // First element in list (only one value by PageOneValue)
        String name2 = bvalue2->getName().c_str();      // Value name
        name2 = name2.substring(0, 6);                  // String length limit for value name
        double value2 = bvalue2->value;                 // Value as double in SI unit
        bool valid2 = bvalue2->valid;                   // Valid information 
        String svalue2 = formatValue(bvalue2, commonData).svalue;    // Formatted value as string including unit conversion and switching decimal places
        String unit2 = formatValue(bvalue2, commonData).unit;        // Unit of value
        if(valid2 == true){
            svalue2old = svalue2;   	                // Save old value
            unit2old = unit2;                           // Save old unit
        }

        // Optical warning by limit violation (unused)
        if(String(flashLED) == "Limit Violation"){
            setBlinkingLED(false);
            setPortPin(OBP_FLASH_LED, false); 
        }

        // Logging boat values
        if (bvalue1 == NULL) return;
        LOG_DEBUG(GwLog::LOG,"Drawing at PageClock, %s:%f,  %s:%f", name1, value1, name2, value2);

        // Draw page
        //***********************************************************

        // Set background color and text color
        int textcolor = GxEPD_BLACK;
        int pixelcolor = GxEPD_BLACK;
        int bgcolor = GxEPD_WHITE;
        if(displaycolor == "Normal"){
            textcolor = GxEPD_BLACK;
            pixelcolor = GxEPD_BLACK;
            bgcolor = GxEPD_WHITE;
        }
        else{
            textcolor = GxEPD_WHITE;
            pixelcolor = GxEPD_WHITE;
            bgcolor = GxEPD_BLACK;
        }
        // Clear display by call in obp60task.cpp in main loop

        // Show values GPS date
        display.setTextColor(textcolor);
        if(holdvalues == false){
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(10, 65);
            display.print(svalue2);                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(10, 95);
            display.print("Date");                      // Name

            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(185, 100);
            display.print(" ");
            display.print(unit2);                       // Unit
        }
        else{
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(10, 65);
            display.print(svalue2old);                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(10, 95);
            display.print("Date");                       // Name

            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(185, 100);
            display.print(" ");
            display.print(unit2old);                       // Unit
        }

        // Horizintal separator left
        display.fillRect(0, 149, 60, 3, pixelcolor);

        // Show values GPS time
        display.setTextColor(textcolor);
        if(holdvalues == false){
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(10, 250);
            display.print(svalue1);                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(10, 220);
            display.print("Time");                       // Name
        }
        else{
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(10, 250);
            display.print(svalue1old);                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(10, 220);
            display.print("Time");                       // Name
        }

        // Show values sunrise
        display.setTextColor(textcolor);
        if(holdvalues == false){
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(335, 65);
            display.print("06:32");                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(335, 95);
            display.print("SunR");                       // Name
        }
        else{
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(335, 65);
            display.print("06:32");                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(335, 95);
            display.print("SunR");                       // Name
        }

        // Horizintal separator right
        display.fillRect(340, 149, 80, 3, pixelcolor);

        // Show values sunset
        display.setTextColor(textcolor);
        if(holdvalues == false){
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(335, 250);
            display.print("18:22");                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(335, 220);
            display.print("SunS");                       // Name
        }
        else{
            display.setFont(&Ubuntu_Bold8pt7b);
            display.setCursor(335, 250);
            display.print("18:22");                     // Value
            display.setFont(&Ubuntu_Bold12pt7b);
            display.setCursor(335, 220);
            display.print("SunS");                       // Name
        }

//*******************************************************************************************
        
        // Draw wind rose
        int rWindGraphic = 110;     // Radius of grafic instrument
        float pi = 3.141592;

        display.fillCircle(200, 150, rWindGraphic + 10, pixelcolor);    // Outer circle
        display.fillCircle(200, 150, rWindGraphic + 7, bgcolor);        // Outer circle     

        for(int i=0; i<360; i=i+10)
        {
            // Scaling values
            float x = 200 + (rWindGraphic-30)*sin(i/180.0*pi);  //  x-coordinate dots
            float y = 150 - (rWindGraphic-30)*cos(i/180.0*pi);  //  y-coordinate cots 
            const char *ii;
            switch (i)
            {
            case 0: ii="12"; break;
            case 30 : ii=""; break;
            case 60 : ii=""; break;
            case 90 : ii="3"; break;
            case 120 : ii=""; break;
            case 150 : ii=""; break;
            case 180 : ii="6"; break;
            case 210 : ii=""; break;
            case 240 : ii=""; break;
            case 270 : ii="9"; break;
            case 300 : ii=""; break;
            case 330 : ii=""; break;
            default: break;
            }

            // Print text centered on position x, y
            int16_t x1, y1;     // Return values of getTextBounds
            uint16_t w, h;      // Return values of getTextBounds
            display.getTextBounds(ii, int(x), int(y), &x1, &y1, &w, &h); // Calc width of new string
            display.setCursor(x-w/2, y+h/2);
            if(i % 30 == 0){
                display.setFont(&Ubuntu_Bold12pt7b);
                display.print(ii);
            }

            // Draw sub scale with dots
            float x1c = 200 + rWindGraphic*sin(i/180.0*pi);
            float y1c = 150 - rWindGraphic*cos(i/180.0*pi);
            display.fillCircle((int)x1c, (int)y1c, 2, pixelcolor);
            float sinx=sin(i/180.0*pi);
            float cosx=cos(i/180.0*pi); 

            // Draw sub scale with lines (two triangles)
            if(i % 30 == 0){
                float dx=2;   // Line thickness = 2*dx+1
                float xx1 = -dx;
                float xx2 = +dx;
                float yy1 =  -(rWindGraphic-10);
                float yy2 =  -(rWindGraphic+10);
                display.fillTriangle(200+(int)(cosx*xx1-sinx*yy1),150+(int)(sinx*xx1+cosx*yy1),
                        200+(int)(cosx*xx2-sinx*yy1),150+(int)(sinx*xx2+cosx*yy1),
                        200+(int)(cosx*xx1-sinx*yy2),150+(int)(sinx*xx1+cosx*yy2),pixelcolor);
                display.fillTriangle(200+(int)(cosx*xx2-sinx*yy1),150+(int)(sinx*xx2+cosx*yy1),
                        200+(int)(cosx*xx1-sinx*yy2),150+(int)(sinx*xx1+cosx*yy2),
                        200+(int)(cosx*xx2-sinx*yy2),150+(int)(sinx*xx2+cosx*yy2),pixelcolor);  
            }
        }

        // Clock values
        double hour = 0;
        double minute = 0;
        hour = (value1 / 3600.0);
        if(hour > 12) hour = hour - 12.0;
        hour = hour + timezone;
        minute = (hour - int(hour)) * 3600.0 / 60.0;

        // Draw hour pointer
        float startwidth = 8;       // Start width of pointer
        if(valid1 == true || holdvalues == true){
            float sinx=sin(hour * 30.0 * pi / 180);     // Hour
            float cosx=cos(hour * 30.0 * pi / 180);
            // Normal pointer
            // Pointer as triangle with center base 2*width
            float xx1 = -startwidth;
            float xx2 = startwidth;
            float yy1 = -startwidth;
            float yy2 = -(rWindGraphic * 0.5); 
            display.fillTriangle(200+(int)(cosx*xx1-sinx*yy1),150+(int)(sinx*xx1+cosx*yy1),
                200+(int)(cosx*xx2-sinx*yy1),150+(int)(sinx*xx2+cosx*yy1),
                200+(int)(cosx*0-sinx*yy2),150+(int)(sinx*0+cosx*yy2),pixelcolor);   
            // Inverted pointer
            // Pointer as triangle with center base 2*width
            float endwidth = 2;         // End width of pointer
            float ix1 = endwidth;
            float ix2 = -endwidth;
            float iy1 = -(rWindGraphic * 0.5);
            float iy2 = -endwidth;
            display.fillTriangle(200+(int)(cosx*ix1-sinx*iy1),150+(int)(sinx*ix1+cosx*iy1),
                200+(int)(cosx*ix2-sinx*iy1),150+(int)(sinx*ix2+cosx*iy1),
                200+(int)(cosx*0-sinx*iy2),150+(int)(sinx*0+cosx*iy2),pixelcolor);
        }

        // Draw minute pointer
        startwidth = 8;       // Start width of pointer
        if(valid1 == true || holdvalues == true){
            float sinx=sin(minute * 6.0 * pi / 180);     // Minute
            float cosx=cos(minute * 6.0 * pi / 180);
            // Normal pointer
            // Pointer as triangle with center base 2*width
            float xx1 = -startwidth;
            float xx2 = startwidth;
            float yy1 = -startwidth;
            float yy2 = -(rWindGraphic - 15); 
            display.fillTriangle(200+(int)(cosx*xx1-sinx*yy1),150+(int)(sinx*xx1+cosx*yy1),
                200+(int)(cosx*xx2-sinx*yy1),150+(int)(sinx*xx2+cosx*yy1),
                200+(int)(cosx*0-sinx*yy2),150+(int)(sinx*0+cosx*yy2),pixelcolor);   
            // Inverted pointer
            // Pointer as triangle with center base 2*width
            float endwidth = 2;         // End width of pointer
            float ix1 = endwidth;
            float ix2 = -endwidth;
            float iy1 = -(rWindGraphic - 15);
            float iy2 = -endwidth;
            display.fillTriangle(200+(int)(cosx*ix1-sinx*iy1),150+(int)(sinx*ix1+cosx*iy1),
                200+(int)(cosx*ix2-sinx*iy1),150+(int)(sinx*ix2+cosx*iy1),
                200+(int)(cosx*0-sinx*iy2),150+(int)(sinx*0+cosx*iy2),pixelcolor);
        }

        // Center circle
        display.fillCircle(200, 150, startwidth + 6, bgcolor);
        display.fillCircle(200, 150, startwidth + 4, pixelcolor);

//*******************************************************************************************
        // Key Layout
        display.setTextColor(textcolor);
        display.setFont(&Ubuntu_Bold8pt7b);
        display.setCursor(130, 290);
        if(keylock == false){
            display.print("[  <<<<  " + String(commonData.data.actpage) + "/" + String(commonData.data.maxpage) + "  >>>>  ]");
            if(String(backlightMode) == "Control by Key"){              // Key for illumination
                display.setCursor(343, 290);
                display.print("[ILUM]");
            }
        }
        else{
            display.print(" [    Keylock active    ]");
        }

        // Update display
        display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true);    // Partial update (fast)

    };
};

static Page *createPage(CommonData &common){
    return new PageClock(common);
}
/**
 * with the code below we make this page known to the PageTask
 * we give it a type (name) that can be selected in the config
 * we define which function is to be called
 * and we provide the number of user parameters we expect (0 here)
 * and will will provide the names of the fixed values we need
 */
PageDescription registerPageClock(
    "Clock",         // Page name
    createPage,         // Action
    0,                  // Number of bus values depends on selection in Web configuration
    {"GPST", "GPSD"},    // Bus values we need in the page
    true                // Show display header on/off
);