/* TO ADD A NEW ECHIEVEMENT:
 *
 * 1) Append new Echievement_Id before ECH(LAST) in e_mod_main.h
 * 2) Add Echievement_Hide_States
 * 3) Add Echievement_Goals
 * 4) Add Echievement_Strings
 * 5) Add Echievement_Descs
 * 6) Add Echievement_Callbacks
 * 7) Add related ECH_INIT and ECH_EH functions in echievements.c
 */

static Etrophy_Trophy_Visibility Echievement_Hide_States[] =
{
   [ECH(SHELF_POSITIONS)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(NOTHING_ELSE_MATTERS)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(AFRAID_OF_THE_DARK)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(CAVE_DWELLER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_ENTHUSIAST)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_LOVER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_STALKER)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(WINDOW_MOVER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_OCD)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(MOUSE_RUNNER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(MOUSE_MARATHONER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(MOUSE_HERO)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(MIGHTY_MOUSE)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(WINDOW_HAULER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_SLINGER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_SHERPA)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(PHYSICIST)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(QUICKDRAW)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(OPAQUE)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(KEYBOARD_USER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(KEYBOARD_TAPPER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(KEYBOARD_ABUSER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(KEYBOARD_NINJA)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(CLICKER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(SUPER_CLICKER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(SUPER_DUPER_CLICKER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(CLICK_MANIAC)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(SLEEPER)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(HALTER)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(EDGY)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(EDGIER)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(EDGIEST)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(EDGAR)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(WHEELY)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(ROLLIN)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(ROLY_POLY)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(SIGNALLER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(SIGNAL_CONTROLLER)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(SIGNAL_MASTER)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(SIGURD)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(NOT_SO_INCOGNITO)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(DUALIST)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(THRICE)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(GOING_HD)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(REAL_ESTATE_MOGUL)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(MAXIMUM_DEFINITION)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(TERMINOLOGIST)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(LIFE_ON_THE_EDGE)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(BILINGUAL)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(POLYGLOT)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(GADGETEER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(INSPECTOR_GADGET)] = ETROPHY_TROPHY_STATE_HIDDEN,
   [ECH(GADGET_HACKWRENCH)] = ETROPHY_TROPHY_STATE_HIDDEN,
};

static unsigned int Echievement_Goals[] =
{
                                      /* LAST - first possible shelf orient */
   [ECH(SHELF_POSITIONS)] = E_GADCON_ORIENT_LAST - E_GADCON_ORIENT_LEFT,
   [ECH(NOTHING_ELSE_MATTERS)] = 1,
   [ECH(AFRAID_OF_THE_DARK)] = 1,
   [ECH(CAVE_DWELLER)] = 1,
   [ECH(WINDOW_ENTHUSIAST)] = 100,
   [ECH(WINDOW_LOVER)] = 1000,
   [ECH(WINDOW_STALKER)] = 10000,
   [ECH(WINDOW_MOVER)] = 50,
   [ECH(WINDOW_OCD)] = 200,
   [ECH(MOUSE_RUNNER)] = 250000,
   [ECH(MOUSE_MARATHONER)] = 1000000,
   [ECH(MOUSE_HERO)] = 10000000,
   [ECH(MIGHTY_MOUSE)] = 100000000,
   [ECH(WINDOW_HAULER)] = 100000,
   [ECH(WINDOW_SLINGER)] = 500000,
   [ECH(WINDOW_SHERPA)] = 2500000,
   [ECH(PHYSICIST)] = 1,
   [ECH(QUICKDRAW)] = 1,
   [ECH(OPAQUE)] = 1,
   [ECH(KEYBOARD_USER)] = 5,
   [ECH(KEYBOARD_TAPPER)] = 10,
   [ECH(KEYBOARD_ABUSER)] = 20,
   [ECH(KEYBOARD_NINJA)] = 30,
   [ECH(CLICKER)] = 5,
   [ECH(SUPER_CLICKER)] = 10,
   [ECH(SUPER_DUPER_CLICKER)] = 20,
   [ECH(CLICK_MANIAC)] = 30,
   [ECH(SLEEPER)] = 2,
   [ECH(HALTER)] = 5,
   [ECH(EDGY)] = 2,
   [ECH(EDGIER)] = 4,
   [ECH(EDGIEST)] = 6,
   [ECH(EDGAR)] = 8,
   [ECH(WHEELY)] = 3,
   [ECH(ROLLIN)] = 6,
   [ECH(ROLY_POLY)] = 10,
   [ECH(SIGNALLER)] = 5,
   [ECH(SIGNAL_CONTROLLER)] = 10,
   [ECH(SIGNAL_MASTER)] = 20,
   [ECH(SIGURD)] = 30,
   [ECH(NOT_SO_INCOGNITO)] = 1,
   [ECH(DUALIST)] = 2,
   [ECH(THRICE)] = 3,
   [ECH(GOING_HD)] = 1920 * 1080,
   [ECH(REAL_ESTATE_MOGUL)] = 1920 * 1080 + 1280 * 1024,
   [ECH(MAXIMUM_DEFINITION)] = 2,
   [ECH(TERMINOLOGIST)] = 1,
   [ECH(LIFE_ON_THE_EDGE)] = 1,
   [ECH(BILINGUAL)] = 2,
   [ECH(POLYGLOT)] = 4,
   [ECH(GADGETEER)] = 5,
   [ECH(INSPECTOR_GADGET)] = 10,
   [ECH(GADGET_HACKWRENCH)] = 15,
};

/* sighhh the amount of time it actually took me to build the list below...
 * update periodically from http://www.ovguide.com/adult-tubesites-visits-alltime.html
 * to maintain hilarity
 */
static const char *const Echievement_NOT_SO_INCOGNITO_Strings[] =
{
   "Porn Tube, You Porn, Free Porn Movies", //youjizz
   "Free Porn Videos - XVIDEOS.COM", //xvideos
   "Free Porn Sex Videos - Redtube", //redtube
   "Free sex videos, Sex tube", //tube8
   "xHamster's Free Porn Videos", //xhamster
   "Video - Free Porn Videos", //yuvutu <--I guess they just gave up at naming?
   "Free porno tube site!", //koostube
   "Alot Porn Tube", //alotporn <--nice grammar, jackasses
   "Free Porn Videos, Porn Tube, Sex Videos", //mofosex
   "Free Porn Tube Movies & XXX Sex Pics", //keezmovies
   "Free Porn Videos, Porn Tube, Free Porn", //tnaflix
   "Porn Videos, Sex, XXX, Free Porn Tube", //youporn
   "Free Porn, Sex Videos 4 You", //spankwire
   "Free Porn Videos, Free Sex, XXX", //empflix
   "Free Porn Videos & Sex Movies", //pornhub
   NULL
};

static const char *const Echievement_Strings[] =
{
   [ECH(SHELF_POSITIONS)] = "Shelves Everywhere",
   [ECH(NOTHING_ELSE_MATTERS)] = "Nothing Else Matters",
   [ECH(AFRAID_OF_THE_DARK)] = "Afraid Of The Dark",
   [ECH(CAVE_DWELLER)] = "Cave Dweller",
   [ECH(WINDOW_ENTHUSIAST)] = "Window Enthusiast",
   [ECH(WINDOW_LOVER)] = "Window Lover",
   [ECH(WINDOW_STALKER)] = "Window Stalker",
   [ECH(WINDOW_MOVER)] = "Window Mover",
   [ECH(WINDOW_OCD)] = "Window OCD",
   [ECH(MOUSE_RUNNER)] = "Mouse Runner",
   [ECH(MOUSE_MARATHONER)] = "Mouse Marathoner",
   [ECH(MOUSE_HERO)] = "Mouse Hero",
   [ECH(MIGHTY_MOUSE)] = "Mighty Mouse",
   [ECH(WINDOW_HAULER)] = "Window Hauler",
   [ECH(WINDOW_SLINGER)] = "Window Slinger",
   [ECH(WINDOW_SHERPA)] = "Window Sherpa",
   [ECH(PHYSICIST)] = "Physicist",
   [ECH(QUICKDRAW)] = "Quickdraw",
   [ECH(OPAQUE)] = "Opaque",
   [ECH(KEYBOARD_USER)] = "Keyboard User",
   [ECH(KEYBOARD_TAPPER)] = "Keyboard Tapper",
   [ECH(KEYBOARD_ABUSER)] = "Keyboard Abuser",
   [ECH(KEYBOARD_NINJA)] = "Keyboard Ninja",
   [ECH(CLICKER)] = "Clicker",
   [ECH(SUPER_CLICKER)] = "Super Clicker",
   [ECH(SUPER_DUPER_CLICKER)] = "Super Duper Clicker",
   [ECH(CLICK_MANIAC)] = "Click Maniac",
   [ECH(SLEEPER)] = "Sleeper",
   [ECH(HALTER)] = "Halter",
   [ECH(EDGY)] = "Edgy",
   [ECH(EDGIER)] = "Edgier",
   [ECH(EDGIEST)] = "Edgiest",
   [ECH(EDGAR)] = "Edgar",
   [ECH(WHEELY)] = "Wheely",
   [ECH(ROLLIN)] = "Rollin'",
   [ECH(ROLY_POLY)] = "Roly Poly",
   [ECH(SIGNALLER)] = "Signaller",
   [ECH(SIGNAL_CONTROLLER)] = "Signal Controller",
   [ECH(SIGNAL_MASTER)] = "Signal Master",
   [ECH(SIGURD)] = "Sigurd",
   [ECH(NOT_SO_INCOGNITO)] = "Not So Incognito",
   [ECH(DUALIST)] = "Dualist",
   [ECH(THRICE)] = "Thrice",
   [ECH(GOING_HD)] = "Going HD",
   [ECH(REAL_ESTATE_MOGUL)] = "Real Estate Mogul",
   [ECH(MAXIMUM_DEFINITION)] = "Maximum Definition",
   [ECH(TERMINOLOGIST)] = "Terminologist",
   [ECH(LIFE_ON_THE_EDGE)] = "Life On The Edge",
   [ECH(BILINGUAL)] = "Bilingual",
   [ECH(POLYGLOT)] = "Polyglot",
   [ECH(GADGETEER)] = "Gadgeteer",
   [ECH(INSPECTOR_GADGET)] = "Inspector Gadget",
   [ECH(GADGET_HACKWRENCH)] = "Gadget Hackwrench",
};

static const char *const Echievement_Descs[] =
{
   [ECH(SHELF_POSITIONS)] = "Have a shelf in every position",
   [ECH(NOTHING_ELSE_MATTERS)] = "Use E17 with no shelves",
   [ECH(AFRAID_OF_THE_DARK)] = "Set backlight level to 100%",
   [ECH(CAVE_DWELLER)] = "Set backlight level to 0%",
   [ECH(WINDOW_ENTHUSIAST)] = "Open 100 windows",
   [ECH(WINDOW_LOVER)] = "Open 1000 windows",
   [ECH(WINDOW_STALKER)] = "Open 10000 windows",
   [ECH(WINDOW_MOVER)] = "Move 50 windows",
   [ECH(WINDOW_OCD)] = "Move 250 windows",
   [ECH(MOUSE_RUNNER)] = "Mouse over 250,000 pixels",
   [ECH(MOUSE_MARATHONER)] = "Mouse over 1,000,000 pixels",
   [ECH(MOUSE_HERO)] = "Mouse over 10,000,000 pixels",
   [ECH(MIGHTY_MOUSE)] = "Mouse over 100,000,000 pixels",
   [ECH(WINDOW_HAULER)] = "Move windows 100,000 pixels",
   [ECH(WINDOW_SLINGER)] = "Move windows 500,000 pixels",
   [ECH(WINDOW_SHERPA)] = "Move windows 2,500,000 pixels",
   [ECH(PHYSICIST)] = "Load the Physics module",
   [ECH(QUICKDRAW)] = "Load the Quickaccess module",
   [ECH(OPAQUE)] = "Unload the Composite module",
   [ECH(KEYBOARD_USER)] = "Change 5 key bindings",
   [ECH(KEYBOARD_TAPPER)] = "Change 10 key bindings",
   [ECH(KEYBOARD_ABUSER)] = "Change 20 key bindings",
   [ECH(KEYBOARD_NINJA)] = "Change 30 key bindings",
   [ECH(CLICKER)] = "Change 5 mouse bindings",
   [ECH(SUPER_CLICKER)] = "Change 10 mouse bindings",
   [ECH(SUPER_DUPER_CLICKER)] = "Change 20 mouse bindings",
   [ECH(CLICK_MANIAC)] = "Change 30 mouse bindings",
   [ECH(SLEEPER)] = "Change 2 ACPI bindings",
   [ECH(HALTER)] = "Change 5 ACPI bindings",
   [ECH(EDGY)] = "Change 2 Edge bindings",
   [ECH(EDGIER)] = "Change 4 Edge bindings",
   [ECH(EDGIEST)] = "Change 6 Edge bindings",
   [ECH(EDGAR)] = "Change 8 Edge bindings",
   [ECH(WHEELY)] = "Change 3 Mouse Wheel bindings",
   [ECH(ROLLIN)] = "Change 6 Mouse Wheel bindings",
   [ECH(ROLY_POLY)] = "Change 10 Mouse Wheel bindings",
   [ECH(SIGNALLER)] = "Change 5 Edje Signal Bindings",
   [ECH(SIGNAL_CONTROLLER)] = "Change 10 Edje Signal Bindings",
   [ECH(SIGNAL_MASTER)] = "Change 20 Edje Signal Bindings",
   [ECH(SIGURD)] = "Change 30 Edje Signal Bindings",
   [ECH(NOT_SO_INCOGNITO)] = "Watch porn while using E17",
   [ECH(DUALIST)] = "Connect two monitors simultaneously",
   [ECH(THRICE)] = "Connect three monitors simultaneously",
   [ECH(GOING_HD)] = "Have 1080p on a single monitor",
   [ECH(REAL_ESTATE_MOGUL)] = "At least 3,384,320 pixels",
   [ECH(MAXIMUM_DEFINITION)] = "Dual 1080p monitors",
   [ECH(TERMINOLOGIST)] = "Open a Terminology window",
   [ECH(LIFE_ON_THE_EDGE)] = "Load a module which taints E17",
   [ECH(BILINGUAL)] = "Have two languages available",
   [ECH(POLYGLOT)] = "Have four languages available",
   [ECH(GADGETEER)] = "Have 5 gadgets on the desktop",
   [ECH(INSPECTOR_GADGET)] = "Have 10 gadgets on the desktop",
   [ECH(GADGET_HACKWRENCH)] = "Have 15 gadgets on the desktop",
};

#define ECB(NAME) EINTERN void echievement_init_cb_##NAME(Echievement *ec)

ECB(SHELF_POSITIONS);
ECB(NOTHING_ELSE_MATTERS);
ECB(AFRAID_OF_THE_DARK);
ECB(CAVE_DWELLER);
ECB(WINDOW_ENTHUSIAST);
ECB(WINDOW_MOVER);
ECB(MOUSE_RUNNER);
ECB(WINDOW_HAULER);
ECB(PHYSICIST);
ECB(QUICKDRAW);
ECB(OPAQUE);
ECB(KEYBOARD_USER);
ECB(CLICKER);
ECB(SLEEPER);
ECB(EDGY);
ECB(WHEELY);
ECB(SIGNALLER);
ECB(NOT_SO_INCOGNITO);
ECB(DUALIST);
ECB(GOING_HD);
ECB(REAL_ESTATE_MOGUL);
ECB(MAXIMUM_DEFINITION);
ECB(TERMINOLOGIST);
ECB(LIFE_ON_THE_EDGE);
ECB(BILINGUAL);
ECB(GADGETEER);

#undef ECB
#define ECB(NAME) [ECHIEVEMENT_ID_##NAME] = echievement_init_cb_##NAME
#define ECB_REUSE(NAME, REUSE) [ECHIEVEMENT_ID_##NAME] = echievement_init_cb_##REUSE

Echievement_Init_Cb Echievement_Callbacks[] =
{
   ECB(SHELF_POSITIONS),
   ECB(NOTHING_ELSE_MATTERS),
   ECB(AFRAID_OF_THE_DARK),
   ECB(CAVE_DWELLER),
   ECB(WINDOW_ENTHUSIAST),
   ECB_REUSE(WINDOW_LOVER, WINDOW_ENTHUSIAST),
   ECB_REUSE(WINDOW_STALKER, WINDOW_ENTHUSIAST),
   ECB(WINDOW_MOVER),
   ECB_REUSE(WINDOW_OCD, WINDOW_MOVER),
   ECB(MOUSE_RUNNER),
   ECB_REUSE(MOUSE_MARATHONER, MOUSE_RUNNER),
   ECB_REUSE(MOUSE_HERO, MOUSE_RUNNER),
   ECB_REUSE(MIGHTY_MOUSE, MOUSE_RUNNER),
   ECB(WINDOW_HAULER),
   ECB_REUSE(WINDOW_SLINGER, WINDOW_HAULER),
   ECB_REUSE(WINDOW_SHERPA, WINDOW_HAULER),
   ECB(PHYSICIST),
   ECB(QUICKDRAW),
   ECB(OPAQUE),
   ECB(KEYBOARD_USER),
   ECB_REUSE(KEYBOARD_TAPPER, KEYBOARD_USER),
   ECB_REUSE(KEYBOARD_ABUSER, KEYBOARD_USER),
   ECB_REUSE(KEYBOARD_NINJA, KEYBOARD_USER),
   ECB(CLICKER),
   ECB_REUSE(SUPER_CLICKER, CLICKER),
   ECB_REUSE(SUPER_DUPER_CLICKER, CLICKER),
   ECB_REUSE(CLICK_MANIAC, CLICKER),
   ECB(SLEEPER),
   ECB_REUSE(HALTER, SLEEPER),
   ECB(EDGY),
   ECB_REUSE(EDGIER, EDGY),
   ECB_REUSE(EDGIEST, EDGY),
   ECB_REUSE(EDGAR, EDGY),
   ECB(WHEELY),
   ECB_REUSE(ROLLIN, WHEELY),
   ECB_REUSE(ROLY_POLY, WHEELY),
   ECB(SIGNALLER),
   ECB_REUSE(SIGNAL_CONTROLLER, SIGNALLER),
   ECB_REUSE(SIGNAL_MASTER, SIGNALLER),
   ECB_REUSE(SIGURD, SIGNALLER),
   ECB(NOT_SO_INCOGNITO),
   ECB(DUALIST),
   ECB_REUSE(THRICE, DUALIST),
   ECB(GOING_HD),
   ECB(REAL_ESTATE_MOGUL),
   ECB(MAXIMUM_DEFINITION),
   ECB(TERMINOLOGIST),
   ECB(LIFE_ON_THE_EDGE),
   ECB(BILINGUAL),
   ECB_REUSE(POLYGLOT, BILINGUAL),
   ECB(GADGETEER),
   ECB_REUSE(INSPECTOR_GADGET, GADGETEER),
   ECB_REUSE(GADGET_HACKWRENCH, GADGETEER),
   NULL
};

#undef ECB
