#ifndef __PHONE_ENUMS_H
#define __PHONE_ENUMS_H

/*------------------------------------------*/

enum PhoneStates{ STATE_HOOK_DOWN,
                  STATE_HOOK_PICK_UP,
                  STATE_DIAL,
                  STATE_WAIT_FOR_INPUT
                };

enum PhoneModes { MODE_SPEEDDIAL,
                  MODE_URL,
                  MODE_CALCULATOR
                };                

/*do not change the order of this enum (unless you really know what you are doing)*/
enum PhoneEvents{ PHONE_URL_0,        /*0*/
                  PHONE_URL_1,        /*1*/
                  PHONE_URL_2,        /*2*/
                  PHONE_URL_3,        /*3*/
                  PHONE_URL_4,        /*4*/
                  PHONE_URL_5,        /*5*/
                  PHONE_URL_6,        /*6*/
                  PHONE_URL_7,        /*7*/
                  PHONE_URL_8,        /*8*/
                  PHONE_URL_9,        /*9*/
                  
                  PHONE_SPEEDDIAL_1,  /*dial 1 directly after taking the phone from the hook*/              
                  PHONE_SPEEDDIAL_2,  /*     2                                              */
                  PHONE_SPEEDDIAL_3,  /*     3                                              */
                  PHONE_SPEEDDIAL_4,  /*     4                                              */
                  PHONE_SPEEDDIAL_5,  /*     5                                              */
                  PHONE_SPEEDDIAL_6,  /*     6                                              */
                  PHONE_SPEEDDIAL_7,  /*     7                                              */
                  PHONE_SPEEDDIAL_8,  /*     8                                              */
                  PHONE_CALCULATOR,   /*     9                                              */
                  PHONE_URL,          /*     0                                              */
                                    
                  PHONE_CALC_0,       /*0*/
                  PHONE_CALC_1,       /*1*/
                  PHONE_CALC_2,       /*2*/
                  PHONE_CALC_3,       /*3*/
                  PHONE_CALC_4,       /*4*/
                  PHONE_CALC_5,       /*5*/
                  PHONE_CALC_6,       /*6*/
                  PHONE_CALC_7,       /*7*/
                  PHONE_CALC_8,       /*8*/
                  PHONE_CALC_9,       /*9*/
                  
                  PHONE_CALC_RETURN,  /*press button, then dial 0*/                                    
                  PHONE_CALC_DELETE,  /*press button, then dial 1*/
                  PHONE_CALC_DOT,     /*press button, then dial 2*/                  
                  PHONE_CALC_DOTCOM,  /*press button, then dial 3*/
                  PHONE_CALC_DOTCN,   /*press button, then dial 4*/                  
                  PHONE_CALC_MINUS,   /*press button, then dial 5*/
                  PHONE_CALC_PLUS,    /*press button, then dial 6*/                  
                  PHONE_CALC_STAR,    /*press button, then dial 7*/                  
                  PHONE_CALC_SLASH,   /*press button, then dial 8*/
                  PHONE_CALC_EQUAL,   /*press button, then dial 9*/

                  PHONE_URL_RETURN,   /*press button, then dial 0*/                                    
                  PHONE_URL_DELETE,   /*press button, then dial 1*/
                  PHONE_URL_DOT,      /*press button, then dial 2*/                  
                  PHONE_URL_DOTCOM,   /*press button, then dial 3*/
                  PHONE_URL_DOTCN,    /*press button, then dial 4*/                  
                  PHONE_URL_MINUS,    /*press button, then dial 5*/
                  PHONE_URL_PLUS,     /*press button, then dial 6*/                  
                  PHONE_URL_STAR,     /*press button, then dial 7*/                  
                  PHONE_URL_SLASH,    /*press button, then dial 8*/
                  PHONE_URL_EQUAL,    /*press button, then dial 9*/                                         
                  
                  PHONE_OFF_HOOK,           /*the user took the handpiece of the hook, this event is generated only when the handpieces state goes from ON-HOOK to OFF-HOOK*/
                  PHONE_SPEEDDIAL_HANG_UP,  /*the user has put the handpiece back onto the hook, the phonecall is over*/
                  PHONE_CALCULATOR_HANG_UP, /*the user has put the handpiece back onto the hook, the phonecall is over*/                  
                  PHONE_URL_HANG_UP,        /*the user has put the handpiece back onto the hook, the phonecall is over*/
                  PHONE_TIMEOUT,            /*the user didn't do anything for a defined period of time*/
                  PHONE_BUTTON,             /*user presses the white button on the front panel of the phone*/
                  PHONE_IDLE
                };

/*------------------------------------------*/
#endif
