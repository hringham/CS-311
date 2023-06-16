/*
    Filename: concert.c - Ticket sales simulator
    Author: Hannah Ingham
    Date: 3/8/23
*/

//Import statements

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "concert.h"

/* 
    This function creates a new event, assigns it an act
    and a venue, prints the report, and frees the structs from 
    memory
*/
int main(int argc, char *argv[])
{
    struct Act *act = newAct("Lizzo", 50, 10000, 0.40);
    struct Venue *venue = newVenue("DPAC", "Durham", "NC");

    struct VenueSection *dpacStageFront = newVenueSection('A', 100, 1);
    SET_STAGEFRONT(dpacStageFront);
    addSection(venue, dpacStageFront);

    struct VenueSection *dpacMain = newVenueSection('B', 500, 1);
    addSection(venue, dpacMain);

    struct VenueSection *dpacMezz = newVenueSection('C', 200, 2);
    addSection(venue, dpacMezz);

    struct VenueSection *dpacLuxBalcony = newVenueSection('D', 50, 3);
    SET_LUXURY(dpacLuxBalcony);
    addSection(venue, dpacLuxBalcony);

    struct VenueSection *dpacCheapBalcony = newVenueSection('E', 100, 3);
    SET_POORVIS(dpacCheapBalcony);
    addSection(venue, dpacCheapBalcony);

    struct Event *event = newEvent(venue, act, 0);

    for (int i=1; i<=dpacStageFront->numSeats-2; i+=3 ) {
            sellTicket(event, 'A', i);
            sellTicket(event, 'A', i+1);
        }

         /* Section B: sell four of every complete five */
        for (int i=1; i<=dpacMain->numSeats-4; i+=5 ) {
            sellTicket(event, 'B', i);
            sellTicket(event, 'B', i+1);
            sellTicket(event, 'B', i+2);
            sellTicket(event, 'B', i+3);
        }

         /* Section C: sell one of every complete four */
         for (int i=1; i<=dpacMezz->numSeats-3; i+=4 ) {
            /* Sell one of every complete 4 */
            sellTicket(event, 'C', i);
        }

        /* Section D: sell every other pair (first 2 of complete 4) */
        for (int i=1; i<=dpacLuxBalcony->numSeats-3; i+=4 ) {
            sellTicket(event, 'D', i);
            sellTicket(event, 'D', i+1);
        }

        /* Section E: Sell every 10th */
        for (int i=1; i<=dpacCheapBalcony->numSeats-9; i+=10 ) {
            sellTicket(event, 'E', i+9);
        }

    printEventReport(event);
    freeAct(act);
    freeVenue(venue);
    freeEvent(event);
    return EXIT_SUCCESS;
}

/*
    This function creates a new act with a name, base ticket price, 
    base payment for the act, and their cut of sales. 
*/
struct Act *newAct(char name[], int baseTicketPrice, int basePayment, double cut)
{
    struct Act *act = malloc(sizeof(struct Act));
    strcpy(act->actName, name);
    act->baseTicketPrice = baseTicketPrice;
    act->basePayment = basePayment;
    act->cut = cut;
    return act;
}

void freeAct(struct Act *act)
{
    free(act);
}

/*
    This function creates a new event with a venue and an act.
*/
struct Event *newEvent(struct Venue *venue, struct Act *act, long date)
{
    struct Event *event = malloc(sizeof(struct Event));
    event->act = act;
    event->venue = venue;
    event->sales = newEventSales(venue);
    date = 0;
    return event;
}

/*
    The printEventReport function prints the event's act and 
    venue name.
*/
void printEventReport(struct Event *event)
{
    printf("\n");
    printf("=== %s at %s ===\n", event->act->actName, event->venue->venueName);
    printf("\n");
    printf("== Sales summary");
    printf("\n");
    printf("Sold %d tickets.\n", numSold(event->sales));
    printf("Total ticket sales: $%d.\n", event->totalSales);
    printf("Band payment is: $%d.\n", actPayment(event));
    printf("Venue proceeds: $%d.\n", venueProceeds(event));
    printf("\n");

    printSeatingChart(event->sales);
}

int sellTicket(struct Event *event, char sectionId, int seat)
{
    assert(!checkSeat(event, sectionId, seat));
    struct EventSales *sales = event->sales;
    setSold(sales, sectionId, seat);
    int charge = ticketPrice(event, sectionId);
    event->totalSales += charge;
    return charge;
}

int ticketPrice(struct Event *event, char sectionId)
{
    struct Venue *venue = event->venue;
    struct Act *act = event->act;
    struct VenueSection *section = getSection(venue, sectionId);
    double basePrice = calculatePrice(section, act->baseTicketPrice);
    int price = (int) ceil(basePrice);
    return price;
}

int checkSeat(struct Event *event, char sectionId, int seat)
{
    struct EventSales *sales = event->sales;
    return isSold(sales, sectionId, seat);
}

int actPayment(struct Event *event)
{
    struct Act *act = event->act;
    int actBase = act->basePayment;
    int shared = event->totalSales - actBase;
    int actShare = (int) floor(shared * act->cut);
    return actBase + actShare;
}

int venueProceeds(struct Event *event)
{
    return event->totalSales - actPayment(event);
}

void freeEvent(struct Event *event)
{
    freeEventSales(event->sales);
    free(event);
}

/*
    This function creates a new venue with the venue's name,
    city location, and state.
*/
struct Venue *newVenue(char venueName[], char venueCity[], char venueState[])
{
    struct Venue *venue = malloc(sizeof(struct Venue));
    strcpy(venue->venueName, venueName);
    strcpy(venue->venueCity, venueCity);
    strcpy(venue->venueState, venueState);
    return venue;
}

int addSection(struct Venue *venue, struct VenueSection *section)
{
    int slot = venue->numSections;
    assert((section->sectionId - FIRST_SECTION_ID) == slot);
    venue->sections[slot] = section;
    venue->numSections++;
    return slot;
}

void freeVenue(struct Venue *venue)
{
    for (int i=0; i<venue->numSections; i++) 
    {
        struct VenueSection *section = venue->sections[i];
        free(section);
        venue->sections[i] = NULL;
    }
    free(venue);
}

struct VenueSection *newVenueSection(char sectionId, int numSeats, int level)
{
    struct VenueSection *section = malloc(sizeof(struct VenueSection));
    section->sectionId = sectionId;
    section->numSeats = numSeats;
    section->level = level;
    return section;
}

struct VenueSection *getSection(struct Venue *venue, char sectionId)
{
    int slot = sectionId - FIRST_SECTION_ID;
    assert (slot<venue->numSections);
    return venue->sections[slot];
}

double calculatePrice(struct VenueSection *section, int basePrice)
{
    double multiplier;

    if (IS_LUXURY(section))
    {
        multiplier = 4.0;
    }
    else if (IS_STAGEFRONT(section))
    {
        multiplier = 2.0;
    }
    else
    {
        multiplier = 1.0 - (0.10 * (section->level - 1));
            if (IS_POORVIS(section)) 
            {
                multiplier = multiplier / 2;
            }
    }
    return multiplier * basePrice;
}


struct EventSales *newEventSales(struct Venue *venue)
{
    struct EventSales *sales = malloc(sizeof(struct EventSales));
    sales->venue = venue;
    sales->sectionBitArrays = calloc(venue->numSections, sizeof(int *));
    sales->sectionCounts = calloc(venue->numSections, sizeof(int));
    for (int i=0; i < venue->numSections; i++) 
    {
        struct VenueSection *section = venue->sections[i];
        int seats = section->numSeats;

        int numWords = seats / WORD_BITS;
        if (seats % WORD_BITS > 0) 
        {
            numWords++;
        }
        sales->sectionBitArrays[i] = calloc(numWords, sizeof(int));
    }
    return sales;
}

int isSold(struct EventSales *sales, char sectionId, int seat)
{
    int sectionIndex = sectionId - FIRST_SECTION_ID;
    int seatIndex = seat - 1;
    int word = seatIndex / WORD_BITS;
    int bitOffset = seatIndex % WORD_BITS; //This is where I stopped (this code to be implemented)
    int mask = 0x1 << bitOffset;
    unsigned int *sectionBitArray = sales->sectionBitArrays[sectionIndex];
    return ((sectionBitArray[word] & mask ) != 0 ) ? 1 : 0;
}

void setSold(struct EventSales *sales, char sectionId, int seat)
{
    int sectionIndex = sectionId - FIRST_SECTION_ID;
    sales->sectionCounts[sectionIndex]++;
    int seatIndex = seat - 1;
    int word = seatIndex / WORD_BITS;
    int bitOffset = seatIndex % WORD_BITS;
    int mask = 0x1 << bitOffset;
    unsigned int *sectionBitArray = sales->sectionBitArrays[sectionIndex];
    sectionBitArray[word] = sectionBitArray[word] | mask;
}

int numSold(struct EventSales *sales)
{
    int result = 0;
    for (int i=0; i<sales->venue->numSections; i++) {
        result += sales->sectionCounts[i];
    }
    return result;
}

void printSeatingChart(struct EventSales *sales)
{
    printf("== Seating Chart");
    for (int secIndex=0; secIndex < sales->venue->numSections; secIndex++) 
        {
            struct VenueSection *section;
            section = sales->venue->sections[secIndex];
            printf("\n");
            printf("Section: %c - %d seats (%d tickets sold)\n", section->sectionId,
                              section->numSeats, sales->sectionCounts[secIndex]);
            for (int seatNumber=1; seatNumber <= section->numSeats; seatNumber++) 
            {
                #if __aarch64__
                    int sold = isSoldARM64(sales, section->sectionId, seatNumber);
                #else
                    int sold = isSold(sales, section->sectionId, seatNumber);
                #endif

                char symbol = sold ? SOLD_SYM : UNSOLD_SYM;
                printf("%c", symbol);
                if (seatNumber%20==0) printf("\n");
            }
            printf("\n");
        }
}

void freeEventSales(struct EventSales *sales)
{
    for (int i=0; i<sales->venue->numSections; i++) 
    {
        free(sales->sectionBitArrays[i]);
    }
    free(sales->sectionBitArrays);
    free(sales->sectionCounts);
    free(sales);
}

void fail(char *msg) {
    fputs(msg, stderr);
    exit(EXIT_FAILURE);
}