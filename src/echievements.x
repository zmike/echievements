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
};

/* ec may be NULL */
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

#undef ECB
#define ECB(NAME) [ECHIEVEMENT_ID_##NAME] = echievement_init_cb_##NAME

Echievement_Init_Cb Echievement_Callbacks[] =
{
   ECB(SHELF_POSITIONS),
   ECB(NOTHING_ELSE_MATTERS),
   ECB(CAVE_DWELLER),
   ECB(WINDOW_ENTHUSIAST),
   ECB(WINDOW_LOVER),
   ECB(WINDOW_STALKER),
   ECB(WINDOW_MOVER),
   ECB(WINDOW_OCD),
   NULL
};

#undef ECB
