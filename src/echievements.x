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
};

/* ec may be NULL */
static unsigned int Echievement_Goals[] =
{
                                      /* LAST - first possible shelf orient */
   [ECH(SHELF_POSITIONS)] = E_GADCON_ORIENT_LAST - E_GADCON_ORIENT_LEFT,
   [ECH(NOTHING_ELSE_MATTERS)] = 1,
};

static const char *const Echievement_Strings[] =
{
   [ECH(SHELF_POSITIONS)] = "Shelves Everywhere",
   [ECH(NOTHING_ELSE_MATTERS)] = "Nothing Else Matters",
};

static const char *const Echievement_Descs[] =
{
   [ECH(SHELF_POSITIONS)] = "Have a shelf in every position",
   [ECH(NOTHING_ELSE_MATTERS)] = "Use E17 with no shelves",
};

#define ECB(NAME) void echievement_init_cb_##NAME(Echievement *ec)

ECB(SHELF_POSITIONS);
ECB(NOTHING_ELSE_MATTERS);

#undef ECB
#define ECB(NAME) [ECHIEVEMENT_ID_##NAME] = echievement_init_cb_##NAME

Echievement_Init_Cb Echievement_Callbacks[] =
{
   ECB(SHELF_POSITIONS),
   ECB(NOTHING_ELSE_MATTERS),
   NULL
};

#undef ECB
