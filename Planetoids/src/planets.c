#include "planets.h"
#include <errno.h>

#define MAX_COLUMN_IN_FILE (1024)
#define COMMENT_CHAR       '#'
#define IN_DELIM       ';'

static unsigned int numberOfBodies = 0u;
static unsigned int capacityofBodies = SYS_INCREMENT;

dtMassPoint ** ppBodies = NULL;


 unsigned int getNumPlanets(void) 
 {
    return numberOfBodies;
 }


dtPolar2 convToPolar2(const Vector2 * const cartesian)
{
    dtPolar2 ret = {
        .r = Vector2Length(*cartesian),
        .t = RAD2DEG*atanf(cartesian->y / cartesian->x)
    };
    return ret;
}

Vector2 convToCartesian2(const dtPolar2 * const polar)
{
    Vector2 ret = {
        .x = polar->r * cosf32(DEG2RAD*polar->t),
        .y = polar->r * sinf32(DEG2RAD*polar->t),
    };
    return ret;
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
            pNewMP->trail[j].alpha = 0.f;
            pNewMP->trail[j].position.x = NAN;
            pNewMP->trail[j].position.y = NAN;
        }
    }
    
    *(ppBodies+numberOfBodies) = pNewMP;
    printf("Object is added with ID:\t%lu\n", (*(ppBodies+numberOfBodies))->ID);

    numberOfBodies++;
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
            printf("\tdeleted\n");
        }
    }
    free(ppBodies);
    return NO_ERROR;
}

static dtErrorID lineToMassPoint (char * line, dtMassPoint * const pmP, const char* delimiter) 
{
    char * currWord = strtok(line,delimiter);
    strcpy(pmP->name, currWord);
    pmP->name[MAXNAME_LENGTH-1] = 0;

    currWord = strtok(NULL,delimiter);
    pmP->mass = atof(currWord);

    currWord = strtok(NULL,delimiter);
    pmP->radius = atof(currWord)*1e3f;
    
    currWord = strtok(NULL,delimiter);
    pmP->position.y = atof(currWord) * 1e9f;
    pmP->position.x = 0.f;
    
    currWord = strtok(NULL,delimiter);
    pmP->velocity.x = atof(currWord) * 1e3f;
    pmP->velocity.y = 0.f;
    
    return NO_ERROR;
}

dtErrorID readSystemFromFile(const char * const fName, const char* delimiter)
{
    FILE *fp = fopen(fName, "r");
    char currLine [MAX_COLUMN_IN_FILE] = {0};
    dtErrorID err = NO_ERROR;

    if(!fp) {
        printf("Couldn't open file: %s\n",fName);
        return ERR_COULDN_OPEN_FILE;
    }
    while ( memset (currLine, 0x0,MAX_COLUMN_IN_FILE), fgets(currLine, MAX_COLUMN_IN_FILE, fp) ) {

        dtMassPoint tempMass = {0};
        if(COMMENT_CHAR == currLine[0]) {
            continue;
        }
        if ((err = lineToMassPoint(currLine, &tempMass, delimiter))) {
            break;
        }
        tempMass.color = WHITE;
        tempMass.movable = true;
        if((err = addBody(&tempMass))) {
            break;
        }
    }

    fclose(fp);
    return err;
}


dtMassPoint ** createRingOfBalls(
    const Vector2 center,
    const float radiusOfCircle,
    const float radiusOfBall,
    const float mass,
    const unsigned int num)
{
    dtMassPoint ** ppFirstBody = (ppBodies+numberOfBodies);
    float delTheta = 360.f / num;

    for (size_t i = 0; i < num; i++) {
        dtMassPoint tempMass = {0};
        dtPolar2 tempPol = {
            .r = radiusOfCircle,
            .t = i * delTheta
        };
        Vector2 tempCart = Vector2Add(center, convToCartesian2(&tempPol));

        tempMass.mass = mass;
        tempMass.movable = true;
        tempMass.position.x = tempCart.x;
        tempMass.position.y = tempCart.y;
        tempMass.radius = radiusOfBall;
        tempMass.color = RED;

        addBody(&tempMass);
    }
    return ppFirstBody;
}
