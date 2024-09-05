#ifndef ERRORID_H
#define ERRORID_H

typedef unsigned long int dtErrorID;


#define NO_ERROR                        ((dtErrorID)(0u))
#define ERR_SYSTEM_IS_ALREADY_INITED    ((dtErrorID)(1u))
#define ERR_SYSTEM_COULD_NOT_BE_INITED  ((dtErrorID)(2u))
#define ERR_NO_VALID_SYSTEM             ((dtErrorID)(3u))
#define ERR_NOT_VALID_BODY              ((dtErrorID)(4u))
#define ERR_COULD_NOT_CREATE_NEW_BODY   ((dtErrorID)(5u))
#define ERR_MAX_BODY_LIMIT              ((dtErrorID)(6u))
#define ERR_COULDN_OPEN_FILE            ((dtErrorID)(7u))



#endif