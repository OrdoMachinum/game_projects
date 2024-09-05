#include "planets.h"

#define MAX_COLUMN_IN_FILE (1024)
#define COMMENT_CHAR       '#'
#define IN_DELIM       ';'

static unsigned int numberOfBodies = 0u;
static unsigned int capacityofBodies = SYS_INCREMENT;

dtMassPoint ** ppBodies = NULL;


dtMassPoint planets[] = {
    {   // Sun
        .mass = M_SUN,
        .radius = R_SUN,
        .movable = true,
        .position = ORIG_2,
        .summedForce = ORIG_2,
        .velocity = ORIG_2,
        .color = YELLOW,
    },
    {   // EARTH
        .mass = M_EARTH,
        .radius = R_EARTH,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_EARTH,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_EARTH,
            .y = 0.f
            },        
            .color = GREEN,
    },
    {   // MOON
        .mass = M_MOON,
        .radius = R_MOON,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_EARTH + D_EARTH_MOON,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_EARTH - V_MOON,
            .y = 0.f
            },
        .color = GRAY,
    },
    {   // Mercury
        .mass = M_MERCURY,
        .radius = R_MERCURY,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_MERCURY,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_MERCURY,
            .y = 0.f
            },        
            .color = GRAY,
    },
    {   // Venus
        .mass = M_VENUS,
        .radius = R_VENUS,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_VENUS,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_VENUS,
            .y = 0.f
            },        
            .color = ORANGE,
    },
    {   // MARS
        .mass = M_MARS,
        .radius = R_MARS,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_MARS,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_MARS,
            .y = 0.f
            },        
            .color = RED,
    },
    {   // JUPITER
        .mass = M_JUPITER,
        .radius = R_JUPITER,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_JUPITER,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_JUPITER,
            .y = 0.f
            },        
            .color = ORANGE,
    },
    {   // SATURN
        .mass = M_SATURN,
        .radius = R_SATURN,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_SATURN,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_SATURN,
            .y = 0.f
            },        
            .color = GOLD,
    },
    {   // URANUS
        .mass = M_URANUS,
        .radius = R_URANUS,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_URANUS,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_URANUS,
            .y = 0.f
            },        
            .color = BLUE,
    },
    {   // NEPTUNE
        .mass = M_NEPTUNE,
        .radius = R_NEPTUNE,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_NEPTUNE,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_NEPTUNE,
            .y = 0.f
            },        
            .color = DARKBLUE,
    },
    {   // PLUTO
        .mass = M_PLUTO,
        .radius = R_PLUTO,
        .movable = true,
        .position = {
            .x = 0.f,
            .y = D_SUN_PLUTO,
            },
        .summedForce = ORIG_2,
        .velocity = {
            .x = V_PLUTO,
            .y = 0.f
            },        
            .color = BROWN,
    },
 };



 unsigned int getNumPlanets(void) 
 {
    return (unsigned int)(sizeof(planets) / sizeof(planets[0]));
 }


dtErrorID initSystem(void)
{
    if (ppBodies) {
        return ERR_SYSTEM_IS_ALREADY_INITED;
    }
    ppBodies = calloc(SYS_INCREMENT, sizeof(dtMassPoint*));

    if(!ppBodies) {
        return ERR_SYSTEM_COULD_NOT_BE_INITED;
    }
    return NO_ERROR;
}

dtErrorID addBody(const dtMassPoint * const pBody)
{
    dtMassPoint * pNewMP = NULL;

    if(!ppBodies) {
        return ERR_NO_VALID_SYSTEM;
    }
    if(!pBody) {
        return ERR_NOT_VALID_BODY;
    }
    pNewMP = calloc(1, sizeof(dtMassPoint));

    if(!pNewMP) {
        return ERR_COULD_NOT_CREATE_NEW_BODY;
    }

    if(numberOfBodies >= capacityofBodies) {
        dtMassPoint ** newArray = NULL;
        capacityofBodies*=1.2;
        newArray = realloc(ppBodies, capacityofBodies*sizeof(dtMassPoint*));
        if(!newArray) {
            return ERR_MAX_BODY_LIMIT;
        }
        ppBodies = newArray;
    }
    
    memcpy(pNewMP, pBody, sizeof(dtMassPoint));

    pNewMP->ID = numberOfBodies;
    
    pNewMP->trail = calloc(TRAIL_LENGTH, sizeof(dtTrace));

    if(!pNewMP->trail) {
        printf("Warrning : Could not create trail for object\t%lu\n",pNewMP->ID);
    } else {
        for(unsigned int j = 0u; j < TRAIL_LENGTH; j++)  {
            pNewMP->trail[j].alpha =255.f;
            pNewMP->trail[j].position.x = NAN;
            pNewMP->trail[j].position.y = NAN;
        }
    }
    
    *(ppBodies+numberOfBodies) = pNewMP;
   
    numberOfBodies++;

    printf("Object is added with ID:\t%lu\n", (*(ppBodies+numberOfBodies))->ID);

    return NO_ERROR;
}

dtErrorID destroySystem(void)
{
    if(!ppBodies) {
        return ERR_NO_VALID_SYSTEM;
    }
    for (unsigned long int i = 0u ; i < numberOfBodies; i++) {
        dtMassPoint * pmP = *(ppBodies+i);

        if(pmP){
            printf("ID:\t%lu", pmP->ID);

            if(pmP->trail) {
                free(pmP->trail);
            }

            free(*(ppBodies+i));
            printf("\tdeleted");
        }
    }
    free(ppBodies);
    return NO_ERROR;
}

static dtErrorID lineToMassPoint (char * line, dtMassPoint * const pmP, const char* delimiter) 
{
    char * currWord = strtok(line,delimiter);
    strcpy(pmP->name, currWord);

    currWord = strtok(line,delimiter);
    pmP->mass = atof(currWord);

    currWord = strtok(line,delimiter);
    pmP->radius = atof(currWord);
    
    currWord = strtok(line,delimiter);
    pmP->position.y = atof(currWord);
    pmP->position.x = 0.f;
    
    currWord = strtok(line,delimiter);
    pmP->velocity.x = atof(currWord);
    pmP->velocity.y = 0.f;
    

    return NO_ERROR;
}

dtErrorID readSystemFromFile(const char * const fName, const char* delimiter)
{
    FILE *fp = fopen(fName, "r");
    char * currLine = NULL;
    dtErrorID err = NO_ERROR;

    if(!fp) {
        printf("Couldn't open file: %s\n",fName);
        return ERR_COULDN_OPEN_FILE;
    }

    while (fgets(currLine, MAX_COLUMN_IN_FILE, fp)) {
        dtMassPoint tempMass = {0};
        if(COMMENT_CHAR == currLine[0]) {
            continue;
        }
        if ((err = lineToMassPoint(currLine, &tempMass, delimiter))) {
            break;
        }

        if((err = addBody(&tempMass))) {
            break;
        }
    }
    return err;
}


