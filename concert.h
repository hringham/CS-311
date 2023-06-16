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

#define UNSOLD_SYM '~'
#define SOLD_SYM '&'
#define SEATS_PER_ROW 20

void fail(char *msg);

struct Act 
{
    char actName[MAX_ACT_NAME + 1];
    int baseTicketPrice;
    int basePayment;
    double cut;
};

struct Act *newAct(char *actName, int baseTicketPrice, int basePayment, double cut);
void freeAct(struct Act *act);

/***** Event structure and functions. *****/
struct EventSales;
struct VenueSection;

struct Event
{
    struct Venue *venue;
    struct Act *act;
    long date;
    struct EventSales *sales;
    int totalSales;
};

struct Event *newEvent(struct Venue *venue, struct Act *act, long date);
void printEventReport(struct Event *event);
int sellTicket(struct Event *event, char sectionId, int seat);
int ticketPrice(struct Event *event, char sectionId);
int checkSeat(struct Event *event, char sectionId, int seat);
int actPayment(struct Event *event);
int venueProceeds(struct Event *event);
void freeEvent(struct Event *event);

/***** Venue structure and functions. *****/

struct Venue 
{
    char venueName[MAX_VENUE_NAME + 1];
    char venueCity[MAX_CITY_NAME + 1];
    char venueState[MAX_STATE_NAME + 1];
    struct VenueSection *sections[MAX_SECTIONS];
    int numSections;
};

struct Venue *newVenue(char *venueName, char *venueCity, char *venueState);
int addSection(struct Venue *venue, struct VenueSection *section);
void freeVenue(struct Venue *venue);

/***** VenueSections structure and functions. *****/
struct VenueSection
{
    char sectionId;
    int numSeats;
    int level;

    #define VENUE_FLAGS_LUXURY (0x1u<<0)
    #define VENUE_FLAGS_STAGEFRONT (0x1u<<1)
    #define VENUE_FLAGS_POORVIS (0x1u<<2)
    unsigned short flags; 

    #define IS_LUXURY(dp) ( (dp)->flags & VENUE_FLAGS_LUXURY)
    #define IS_STAGEFRONT(dp) ( (dp)->flags & VENUE_FLAGS_STAGEFRONT)
    #define IS_POORVIS(dp) ( (dp)->flags & VENUE_FLAGS_POORVIS)

    #define SET_LUXURY(dp) ( (dp)->flags |= VENUE_FLAGS_LUXURY)
    #define SET_STAGEFRONT(dp) ( (dp)->flags |= VENUE_FLAGS_STAGEFRONT)
    #define SET_POORVIS(dp) ( (dp)->flags |= VENUE_FLAGS_POORVIS)
};

struct VenueSection *newVenueSection(char sectionId, int numSeats, int level);
struct VenueSection *getSection(struct Venue *venue, char sectionId);
double calculatePrice(struct VenueSection *section, int basePrice);
void freeVenueSection(struct VenueSection *section);

/***** EventSales structure and functions. *****/
#define WORD_BITS 32

struct EventSales
{
    struct Venue *venue;
    unsigned int **sectionBitArrays;
    int *sectionCounts;
};

struct EventSales *newEventSales(struct Venue *venue);
int isSold(struct EventSales *sales, char sectionId, int seat);
void setSold(struct EventSales *sales, char sectionId, int seat);
int numSold(struct EventSales *sales);
void printSeatingChart(struct EventSales *sales);
void freeEventSales(struct EventSales *sales);

#if __aarch64__
// Assembler implementation of isSold
int isSoldARM64(struct EventSales *sales, char sectionId, int seat);
#else
int isSold(struct EventSales *sales, char sectionId, int seat);
#endif