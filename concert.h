/**
 * concert.h - declarations for the concert program
*/

/***** Act structure and functions. *****/
#define MAX_ACT_NAME 32
#define FIRST_SECTION_ID 'A'
#define MAX_SECTIONS 26
#define MAX_VENUE_NAME 64
#define MAX_CITY_NAME 32
#define MAX_STATE_NAME 2

struct Act 
{
    char actName[MAX_ACT_NAME + 1];
    int baseTicketPrice;
    int basePayment;
    double cut;
};

struct Act *newAct(char *actName, int baseTicketPrice, int basePayment, double cut);
void printAct(struct Act *act);
void freeAct(struct Act *act);

/***** Event structure and functions. *****/

struct Event
{
    struct Venue *venue;
    struct Act *act;
};

struct Event *newEvent(struct Venue *venue, struct Act *act);
void printEventReport(struct Event *event);
void freeEvent(struct Event *event);

/***** Venue structure and functions. *****/

struct Venue 
{
    char venueName[MAX_VENUE_NAME + 1];
    char venueCity[MAX_CITY_NAME + 1];
    char venueState[MAX_STATE_NAME + 1];
};

struct Venue *newVenue(char *venueName, char *venueCity, char *venueState);
void printVenue(struct Venue *venue);
void freeVenue(struct Venue *venue);