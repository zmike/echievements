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
   [ECH(MOUSE_HERO)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_HAULER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_SLINGER)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(WINDOW_SHERPA)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(PHYSICIST)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(QUICKDRAW)] = ETROPHY_TROPHY_STATE_VISIBLE,
   [ECH(OPAQUE)] = ETROPHY_TROPHY_STATE_VISIBLE,
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
   [ECH(MOUSE_RUNNER)] = 50000,
   [ECH(MOUSE_MARATHONER)] = 150000,
   [ECH(MOUSE_HERO)] = 500000,
   [ECH(WINDOW_HAULER)] = 10000,
   [ECH(WINDOW_SLINGER)] = 50000,
   [ECH(WINDOW_SHERPA)] = 250000,
   [ECH(PHYSICIST)] = 1,
   [ECH(QUICKDRAW)] = 1,
   [ECH(OPAQUE)] = 1,
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
   [ECH(WINDOW_HAULER)] = "Window Hauler",
   [ECH(WINDOW_SLINGER)] = "Window Slinger",
   [ECH(WINDOW_SHERPA)] = "Window Sherpa",
   [ECH(PHYSICIST)] = "Physicist",
   [ECH(QUICKDRAW)] = "Quickdraw",
   [ECH(OPAQUE)] = "Opaque",
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
   [ECH(MOUSE_RUNNER)] = "Mouse over 50,000 pixels",
   [ECH(MOUSE_MARATHONER)] = "Mouse over 150,000 pixels",
   [ECH(MOUSE_HERO)] = "Mouse over 500,000 pixels",
   [ECH(WINDOW_HAULER)] = "Move windows 10,000 pixels",
   [ECH(WINDOW_SLINGER)] = "Move windows 50,000 pixels",
   [ECH(WINDOW_SHERPA)] = "Move windows 250,000 pixels",
   [ECH(PHYSICIST)] = "Load the Physics module",
   [ECH(QUICKDRAW)] = "Load the Quickaccess module",
   [ECH(OPAQUE)] = "Unload the Composite module",
};

#define ECB(NAME) void echievement_init_cb_##NAME(Echievement *ec)

ECB(SHELF_POSITIONS);
ECB(NOTHING_ELSE_MATTERS);
ECB(AFRAID_OF_THE_DARK);
ECB(CAVE_DWELLER);
ECB(WINDOW_ENTHUSIAST);
ECB(WINDOW_LOVER);
ECB(WINDOW_STALKER);
ECB(WINDOW_MOVER);
ECB(WINDOW_OCD);
ECB(MOUSE_RUNNER);
ECB(MOUSE_MARATHONER);
ECB(MOUSE_HERO);
ECB(WINDOW_HAULER);
ECB(WINDOW_SLINGER);
ECB(WINDOW_SHERPA);
ECB(PHYSICIST);
ECB(QUICKDRAW);
ECB(OPAQUE);

#undef ECB
#define ECB(NAME) [ECHIEVEMENT_ID_##NAME] = echievement_init_cb_##NAME

Echievement_Init_Cb Echievement_Callbacks[] =
{
   ECB(SHELF_POSITIONS),
   ECB(NOTHING_ELSE_MATTERS),
   ECB(AFRAID_OF_THE_DARK),
   ECB(CAVE_DWELLER),
   ECB(WINDOW_ENTHUSIAST),
   ECB(WINDOW_LOVER),
   ECB(WINDOW_STALKER),
   ECB(WINDOW_MOVER),
   ECB(WINDOW_OCD),
   ECB(MOUSE_RUNNER),
   ECB(MOUSE_MARATHONER),
   ECB(MOUSE_HERO),
   ECB(WINDOW_HAULER),
   ECB(WINDOW_SLINGER),
   ECB(WINDOW_SHERPA),
   ECB(PHYSICIST),
   ECB(QUICKDRAW),
   ECB(OPAQUE),
   NULL
};

#undef ECB
