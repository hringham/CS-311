/*
    concert.c - Ticket sales simulator
*/

#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

#include "concert.h"

int main(int argc, char *argv[])
{
    struct Act *act = newAct("Lizzo", 50, 10000, 0.30);
    struct Venue *venue = newVenue("DPAC", "Durham", "NC");
    struct Event *event = newEvent(venue, act);
    printEventReport(event);
    freeEvent(event);
    return EXIT_SUCCESS;
}

struct Act *newAct(char name[], int baseTicketPrice, int basePayment, double cut)
{
    struct Act *act = malloc(sizeof(struct Act));
    strcpy(act->actName, name);
    act->baseTicketPrice = baseTicketPrice;
    act->basePayment = basePayment;
    act->cut = cut;
    return act;
}

struct Venue *newVenue(char venueName[], char venueCity[], char venueState[])
{
    struct Venue *venue = malloc(sizeof(struct Venue));
    strcpy(venue->venueName, venueName);
    strcpy(venue->venueCity, venueCity);
    strcpy(venue->venueState, venueState);
    return venue;
}

struct Event *newEvent(struct Venue *venue, struct Act *act)
{
    struct Event *event = malloc(sizeof(struct Event));
    event->act = act;
    event->venue = venue;
    return event;
}

void printEventReport(struct Event *event)
{
    printf("\n");
    printf("=== %s at %s ===\n", event->act->actName, event->venue->venueName);
    printf("\n");
}

void freeAct(struct Act *act)
{
    free(act);
}

void freeVenue(struct Venue *venue)
{
    free(venue);
}

void freeEvent(struct Event *event)
{
    free(event);
}